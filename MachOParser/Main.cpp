#include "Mach-O/Mach-O.hpp"
#include "Mach-O/Binary.hpp"
#include "Declarations.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

int main(const int argc, const char *argv[]) {
    std::filesystem::path filePath {};
	if (argc < 2) {
		std::wprintf(L"Please specify input file: ");

		std::wstring inputFile {};
		std::getline(std::wcin, inputFile);
		filePath = inputFile;
		std::wprintf(L"\n");
	} else
		filePath = std::string_view {argv[1]};

	if (!std::filesystem::exists(filePath)) {
		std::wprintf(L"[-] Invalid file path provided\n");
		return 1;
	}

	const auto filename = filePath.filename().wstring();
	std::fstream file {};
	try {
		file.open(filePath, std::ios::binary | std::ios::out | std::ios::in | std::ios::ate);
		file.exceptions(std::fstream::failbit);
	} catch (std::ios_base::failure&) {
		std::wprintf(L"[-] Failed to open file [%ws]\n", filename.c_str());
		return 1;
	}

	if (!file.is_open()) {
		std::wprintf(L"[-] Failed to open file [%ws]\n", filename.c_str());
		return 1;
	}

	const auto fileSize = file.tellg();
	file.seekg(0);
	std::vector<char> fileBuffer(fileSize);
	try {
		file.read(fileBuffer.data(), fileSize);
	} catch (std::ios_base::failure&) {
		std::wprintf(L"[-] Failed to read file [%ws]\n", filename.c_str());
	}

	std::wprintf(L"[+] Opened file [%ws]\n", filename.c_str());

	auto machoBinary = MachO::MachOBinary {fileBuffer};
	if (!machoBinary.valid()) {
		std::wprintf(L"[-] Provided file is not a valid Mach-O binary\n");
		return 1;
	}

	const auto architectures = machoBinary.architectures();
	for (const auto architecture : architectures) {
		std::wprintf(L"[!] %hs (%hs-endian) at offset 0x%p\n", Declarations::headerName(reinterpret_cast<MachOHeader*>(architecture)->magic).data(),
			architecture->littleEndian() ? "little" : "big",
			reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(architecture) - reinterpret_cast<std::uintptr_t>(fileBuffer.data())));
		std::wprintf(L"\tNumber of load commands: %i\n", architecture->loadCommandsCount());

		const auto commands = architecture->loadCommands();
		for (const auto command : commands) {
			std::wprintf(L"\t\t load command (offset: 0x%p)\n",
				reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(command) - reinterpret_cast<std::uintptr_t>(fileBuffer.data())));
		}
	}

	return 0;
}
