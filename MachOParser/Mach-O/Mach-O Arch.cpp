#include "Mach-O Binary.hpp"

#include "Mach-O.hpp"

using namespace MachO;

bool MachOArch::valid() {
	const auto header = reinterpret_cast<MachOHeader*>(this);
	return header->magic == MH_MAGIC || header->magic == MH_CIGAM || header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64;
}

bool MachOArch::littleEndian() {
	const auto header = reinterpret_cast<MachOHeader*>(this);
	
	// Get the current systems endianness (1 if little-endian, 0 if big-endian)
	std::uint16_t temp {1};
	const auto endian = *reinterpret_cast<std::uint8_t*>(&temp);
	
	if (endian) {
		// Current system is little-endian, if the binary has same byte order it is also little-endian
		if (header->magic == FAT_MAGIC || header->magic == MH_MAGIC || header->magic == MH_MAGIC_64)
			return true;

		return false;
	}

	// Current system is big-endian, if the binary has reversed endianness it is little-endian
	if (header->magic == FAT_CIGAM || header->magic == MH_CIGAM || header->magic == MH_CIGAM_64)
		return true;

	return false;
}
