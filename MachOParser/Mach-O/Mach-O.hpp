// ReSharper disable CppInconsistentNaming
#pragma once

#include <cstdint>

namespace Apple {
	constexpr auto FAT_MAGIC   = 0xCAFEBABE;
	constexpr auto FAT_CIGAM   = 0xBEBAFECA;
	constexpr auto MH_MAGIC    = 0xFEEDFACE;
	constexpr auto MH_CIGAM    = 0xCEFAEDFE;
	constexpr auto MH_MAGIC_64 = 0xFEEDFACF;
	constexpr auto MH_CIGAM_64 = 0xCFFAEDFE;
	
	struct fat_header {
		std::uint32_t magic;
		std::uint32_t nfat_arch;
	};
	
	struct fat_arch {
		std::uint32_t cputype;
	 	std::uint32_t cpusubtype; 
		std::uint32_t offset;
		std::uint32_t size;
		std::uint32_t align;
	};
	
	struct mach_header {
		std::uint32_t magic;
		std::int32_t cputype;
		std::int32_t cpusubtype;
		std::uint32_t filetype;
		std::uint32_t ncmds;
		std::uint32_t sizeofcmds;
		std::uint32_t flags;
	};
	
	struct mach_header_64 {
		std::uint32_t magic;
		std::int32_t cputype;
		std::int32_t cpusubtype;
		std::uint32_t filetype;
		std::uint32_t ncmds;
		std::uint32_t sizeofcmds;
		std::uint32_t flags;
		std::uint32_t reserved;
	};

	struct load_command {
		std::uint32_t cmd;
		std::uint32_t cmdsize;
	};
}

union MachOHeader {
	std::uint32_t magic;
	union {
		Apple::fat_header fatHeader;
		Apple::mach_header machHeader;
		Apple::mach_header_64 mach64Header;
	};
};

constexpr auto swapEndianness(const std::uint32_t value) {
	const auto temp = (value << 8 & 0xFF00FF00) | (value >> 8 & 0x00FF00FF);
	return temp << 16 | temp >> 16;
}