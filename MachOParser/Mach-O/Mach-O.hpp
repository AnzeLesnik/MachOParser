// ReSharper disable CppInconsistentNaming
#pragma once

#include <cstdint>

namespace Apple {
	// header magic
	constexpr auto FAT_MAGIC   {0xCAFEBABE};
	constexpr auto FAT_CIGAM   {0xBEBAFECA};
	constexpr auto MH_MAGIC    {0xFEEDFACE};
	constexpr auto MH_CIGAM    {0xCEFAEDFE};
	constexpr auto MH_MAGIC_64 {0xFEEDFACF};
	constexpr auto MH_CIGAM_64 {0xCFFAEDFE};

	// CPU type
	constexpr auto CPU_ARCH_ABI64 {0x1000000};
	
	constexpr auto CPU_TYPE_ANY        {-1};
	constexpr auto CPU_TYPE_X86        {7};
	constexpr auto CPU_TYPE_X86_64     {CPU_TYPE_X86 | CPU_ARCH_ABI64};
	constexpr auto CPU_TYPE_MC98000    {10};
	constexpr auto CPU_TYPE_ARM        {12};
	constexpr auto CPU_TYPE_ARM64      {CPU_TYPE_ARM | CPU_ARCH_ABI64};
	constexpr auto CPU_TYPE_SPARC      {14};
	constexpr auto CPU_TYPE_POWERPC    {18};
	constexpr auto CPU_TYPE_POWERPC64  {CPU_TYPE_POWERPC | CPU_ARCH_ABI64};
	
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