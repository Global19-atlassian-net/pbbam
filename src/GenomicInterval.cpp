// File Description
/// \file GenomicInterval.cpp
/// \brief Implements the GenomicInterval class.
//
// Author: Derek Barnett

#include "PbbamInternalConfig.h"

#include "pbbam/GenomicInterval.h"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "pbbam/StringUtilities.h"

namespace PacBio {
namespace BAM {
namespace {

// returns sequence name & sets begin/end, from input regionString
std::string parseRegionString(const std::string& reg, PacBio::BAM::Position* begin,
                              PacBio::BAM::Position* end)
{
    const std::vector<std::string> parts = Split(reg, ':');
    if (parts.empty() || parts.size() > 2) throw std::runtime_error{"malformed region string"};

    // given name only, default min,max intervals
    if (parts.size() == 1) {
        *begin = 0;
        *end = 1 << 29;
    }

    // parse interval from input
    else if (parts.size() == 2) {
        const std::vector<std::string> intervalParts = Split(parts.at(1), '-');
        if (intervalParts.empty() || intervalParts.size() > 2)
            throw std::runtime_error{"malformed region string"};
        *begin = std::stoi(intervalParts.at(0));
        *end = std::stoi(intervalParts.at(1));
    }

    return parts.at(0);
}

}  // anonymous

GenomicInterval::GenomicInterval(std::string name, Position start, Position stop)
    : name_{std::move(name)}, interval_{std::move(start), std::move(stop)}
{
}

GenomicInterval::GenomicInterval(const std::string& samtoolsRegionString)
{
    Position begin = UnmappedPosition;
    Position end = UnmappedPosition;

    name_ = parseRegionString(samtoolsRegionString, &begin, &end);
    if (begin == UnmappedPosition || end == UnmappedPosition)
        throw std::runtime_error{"malformed region string"};

    interval_ = PacBio::BAM::Interval<Position>(begin, end);
}

GenomicInterval::GenomicInterval() = default;

GenomicInterval::GenomicInterval(const GenomicInterval&) = default;

GenomicInterval::GenomicInterval(GenomicInterval&&) = default;

GenomicInterval& GenomicInterval::operator=(const GenomicInterval&) = default;

GenomicInterval& GenomicInterval::operator=(GenomicInterval&&) = default;

GenomicInterval::~GenomicInterval() = default;

bool GenomicInterval::operator==(const GenomicInterval& other) const
{
    return name_ == other.name_ && interval_ == other.interval_;
}

bool GenomicInterval::operator!=(const GenomicInterval& other) const { return !(*this == other); }

bool GenomicInterval::CoveredBy(const GenomicInterval& other) const
{
    if (name_ != other.name_) return false;
    return interval_.CoveredBy(other.interval_);
}

bool GenomicInterval::Covers(const GenomicInterval& other) const
{
    if (name_ != other.name_) return false;
    return interval_.Covers(other.interval_);
}

bool GenomicInterval::Intersects(const GenomicInterval& other) const
{
    if (name_ != other.name_) return false;
    return interval_.Intersects(other.interval_);
}

PacBio::BAM::Interval<Position> GenomicInterval::Interval() const { return interval_; }

GenomicInterval& GenomicInterval::Interval(PacBio::BAM::Interval<Position> interval)
{
    interval_ = std::move(interval);
    return *this;
}

bool GenomicInterval::IsValid() const
{
    return (!name_.empty() && (interval_.Start() >= 0) && (interval_.Stop() >= 0) &&
            interval_.IsValid());
}

size_t GenomicInterval::Length() const { return interval_.Length(); }

std::string GenomicInterval::Name() const { return name_; }

GenomicInterval& GenomicInterval::Name(std::string name)
{
    name_ = std::move(name);
    return *this;
}

Position GenomicInterval::Start() const { return interval_.Start(); }

GenomicInterval& GenomicInterval::Start(const Position start)
{
    interval_.Start(start);
    return *this;
}

Position GenomicInterval::Stop() const { return interval_.Stop(); }

GenomicInterval& GenomicInterval::Stop(const Position stop)
{
    interval_.Stop(stop);
    return *this;
}

}  // namespace BAM
}  // namespace PacBio
