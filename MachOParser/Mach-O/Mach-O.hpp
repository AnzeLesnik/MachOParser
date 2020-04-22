#pragma once

#include <cstdint>

#pragma region Apple
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
#pragma endregion 

union MachOHeader {
	std::uint32_t magic;
	union {
		fat_header fatHeader;
		mach_header machHeader;
		mach_header_64 mach64Header;
	};
};