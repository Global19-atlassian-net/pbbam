#include "PbbamInternalConfig.h"

#include <pbbam/virtual/VirtualRegionTypeMap.h>

namespace PacBio {
namespace BAM {

std::map<char, VirtualRegionType> VirtualRegionTypeMap::ParseChar{
    {'A', VirtualRegionType::ADAPTER},
    {'B', VirtualRegionType::BARCODE},
    {'H', VirtualRegionType::HQREGION},
    {'F', VirtualRegionType::FILTERED},
    {'L', VirtualRegionType::LQREGION}};

}  // namespace BAM
}  // namespace PacBio
