#include "Declarations.hpp"

#include "Mach-O/Mach-O.hpp"

using namespace Apple;

std::string_view Declarations::headerName(const std::uint32_t headerMagic) {
	switch (headerMagic) {
	case FAT_MAGIC:
	case FAT_CIGAM:   return "Fat header";
	case MH_MAGIC:
	case MH_CIGAM:    return "Mach header";
	case MH_MAGIC_64:
	case MH_CIGAM_64: return "Mach64 header";
	default:          return "Unknown header";
	}
}
