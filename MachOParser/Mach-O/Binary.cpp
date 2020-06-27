#include "Binary.hpp"

#include "Mach-O.hpp"

using namespace MachO;
using namespace Apple;

bool MachOBinary::valid() const {
	const auto magic = *reinterpret_cast<std::uint32_t*>(_binary.data());
	return magic == FAT_MAGIC || magic == MH_MAGIC || magic == MH_MAGIC_64 ||
		   magic == FAT_CIGAM || magic == MH_CIGAM || magic == MH_CIGAM_64;
}

bool MachOBinary::fat() const {
	const auto header = reinterpret_cast<MachOHeader*>(_binary.data());
	return header->magic == FAT_MAGIC || header->magic == FAT_CIGAM;
}

std::vector<MachOArchitecture*> MachOBinary::architectures() const {
	const auto binary = _binary.data();

	auto header = reinterpret_cast<MachOHeader*>(const_cast<char*>(binary));
	std::vector<MachOArchitecture*> architectures {};

	// If the binary is fat, iterate through the fat_arch array and return all the architectures headers
	if (!fat()) {
		const auto arch = new MachOArchitecture(header);
		if (arch->valid())
			architectures.emplace_back(arch);
		return architectures;
	}

	// Get the current systems endianness (1 if little-endian, 0 if big-endian)
	std::uint16_t temp {1};
	const auto endian = *reinterpret_cast<std::uint8_t*>(&temp);

	const auto fatArchArray = reinterpret_cast<fat_arch*>(reinterpret_cast<std::uintptr_t>(header) + sizeof(fat_header));
	// Fat header is big-endian by default in all binaries
	const auto archCount = endiannessBasedValue(header->fatHeader.nfat_arch, Endianness::BigEndian);
	for (std::size_t i {}; i < archCount; ++i) {
		const auto fatArch = &fatArchArray[i];
		const auto archHeader = reinterpret_cast<MachOHeader*>(binary +
			static_cast<std::uintptr_t>(endiannessBasedValue(fatArch->offset, Endianness::BigEndian)));
		if (reinterpret_cast<std::uintptr_t>(archHeader) > reinterpret_cast<std::uintptr_t>(_binary.data() + _binary.size()))
			continue;

		const auto arch = new MachOArchitecture(archHeader);
		if (!arch->valid())
			continue;

		architectures.emplace_back(arch);
	}

	return architectures;
}