// Author: Derek Barnett

#ifndef PBBAM_VCF_VCFFILE_H
#define PBBAM_VCF_VCFFILE_H

#include <string>

#include <pbbam/vcf/VcfHeader.h>

namespace PacBio {
namespace VCF {

class VcfFile
{
public:
    explicit VcfFile(std::string fn);

    VcfFile() = delete;
    VcfFile(const VcfFile&);
    VcfFile(VcfFile&&) noexcept;
    VcfFile& operator=(const VcfFile&);
    VcfFile& operator=(VcfFile&&) PBBAM_NOEXCEPT_MOVE_ASSIGN;
    ~VcfFile();

public:
    const std::string& Filename() const;
    const VcfHeader& Header() const;

private:
    std::string filename_;
    VcfHeader header_;
};

}  // namespace VCF
}  // namespace PacBio

#endif  // PBBAM_VCF_VCFFILE_H
