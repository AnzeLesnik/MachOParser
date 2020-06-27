#include "Binary.hpp"

#include "Mach-O.hpp"

using namespace MachO;
using namespace Apple;

MachOArchitecture::MachOArchitecture(MachOHeader* archHeader) : _archHeader(archHeader){
	// Get the current systems endianness (1 if little-endian, 0 if big-endian)
	std::uint16_t temp {1};
	const auto endian = *reinterpret_cast<std::uint8_t*>(&temp);
	
	if (endian) {
		// Current system is little-endian, if the binary has same byte order it is also little-endian
		if (_archHeader->magic == FAT_MAGIC || _archHeader->magic == MH_MAGIC || _archHeader->magic == MH_MAGIC_64)
			_endianness = Endianness::LittleEndian;
		else
			_endianness = Endianness::BigEndian;
	} else {
		// Current system is big-endian, if the binary has reversed endianness it is little-endian
		if (_archHeader->magic == FAT_CIGAM || _archHeader->magic == MH_CIGAM || _archHeader->magic == MH_CIGAM_64)
			_endianness = Endianness::LittleEndian;
		else
			_endianness = Endianness::BigEndian;
	}
}

bool MachOArchitecture::valid() {
	return _archHeader->magic == MH_MAGIC || _archHeader->magic == MH_CIGAM || _archHeader->magic == MH_MAGIC_64 || _archHeader->magic == MH_CIGAM_64;
}

Architecture MachOArchitecture::architecture() {
	switch (_archHeader->machHeader.cputype) {
		case CPU_TYPE_ANY:
			return Architecture::Any;
		case CPU_TYPE_X86:
			return Architecture::x86;
		case CPU_TYPE_X86_64:
			return Architecture::x86_64;
		case CPU_TYPE_MC98000:
			return Architecture::MC98000;
		case CPU_TYPE_ARM:
			return Architecture::ARM;
		case CPU_TYPE_ARM64:
			return Architecture::ARM64;
		case CPU_TYPE_SPARC:
			return Architecture::SPARC;
		case CPU_TYPE_POWERPC:
			return Architecture::POWERPC;
		case CPU_TYPE_POWERPC64:
			return Architecture::POWERPC64;
		default:
			return Architecture::Unknown;
	}
}

Endianness MachOArchitecture::endianness() {
	return _endianness;
}

std::uint32_t MachOArchitecture::loadCommandsCount() {
	return endiannessBasedValue(_archHeader->machHeader.ncmds, _endianness);
}

std::vector<LoadCommand*> MachOArchitecture::loadCommands() {
	std::vector<LoadCommand*> commands;

	std::uint32_t headerSize {};
	if (_archHeader->machHeader.cputype & CPU_ARCH_ABI64)
		headerSize = sizeof(mach_header_64);
	else
		headerSize = sizeof(mach_header);
		
	
	auto *command = reinterpret_cast<load_command*>(reinterpret_cast<std::uintptr_t>(_archHeader) + headerSize);
	for (std::size_t i {}; i < loadCommandsCount(); ++i) {
		commands.emplace_back(reinterpret_cast<LoadCommand*>(command));
		
		command = reinterpret_cast<load_command*>(reinterpret_cast<std::uintptr_t>(command) + endiannessBasedValue(command->cmdsize, _endianness));
	}
	
	return commands;
}
