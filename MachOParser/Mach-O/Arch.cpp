#include "Binary.hpp"

#include "Mach-O.hpp"

using namespace MachO;
using namespace Apple;

bool MachOArch::valid() {
	const auto header = reinterpret_cast<MachOHeader*>(this);
	return header->magic == MH_MAGIC || header->magic == MH_CIGAM || header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64;
}

Platform MachOArch::platform() {
	const auto header = reinterpret_cast<MachOHeader*>(this);
	if (header->magic == MH_MAGIC || header->magic == MH_CIGAM)
		return Platform::x86;
	
	if (header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64)
		return Platform::x86_x64;

	return Platform::Unknown;
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

std::uint32_t MachOArch::loadCommandsCount() {
	const auto header = reinterpret_cast<mach_header*>(this);
	
	std::uint16_t temp {1};
	const auto endian = *reinterpret_cast<std::uint8_t*>(&temp);

	if (littleEndian())
		return endian ? header->ncmds : swapEndianness(header->ncmds);

	return endian ? swapEndianness(header->ncmds) : header->ncmds;
}

std::vector<LoadCommand*> MachOArch::loadCommands() {
	std::vector<LoadCommand*> commands;

	std::uint32_t headerSize {};
	const auto archPlatform = platform();
	if (archPlatform == Platform::x86)
		headerSize = sizeof(mach_header);
	else if (archPlatform == Platform::x86_x64)
		headerSize = sizeof(mach_header_64);
	else
		return {};

	std::uint16_t temp {1};
	const auto endian = *reinterpret_cast<std::uint8_t*>(&temp);
	
	auto *command = reinterpret_cast<load_command*>(reinterpret_cast<std::uintptr_t>(this) + headerSize);
	for (std::size_t i {}; i < loadCommandsCount(); ++i) {
		commands.emplace_back(reinterpret_cast<LoadCommand*>(command));

		// TODO: Organize this endianness mess
		if (littleEndian())
			command = reinterpret_cast<load_command*>(reinterpret_cast<std::uintptr_t>(command) + (endian ? command->cmdsize : swapEndianness(command->cmdsize)));
		else
			command = reinterpret_cast<load_command*>(reinterpret_cast<std::uintptr_t>(command) + (endian ? swapEndianness(command->cmdsize) : command->cmdsize));
	}
	
	return commands;
}
