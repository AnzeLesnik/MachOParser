#pragma once

#include<vector>

namespace MachO {
	enum class Platform : std::int32_t {
		Unknown,
		x86,
		x86_x64
	};
	
	struct LoadCommand {
		// TODO: Implement
	};
	
	struct MachOArch {
		[[nodiscard]] bool valid();
		[[nodiscard]] Platform platform();
		[[nodiscard]] bool littleEndian();

		[[nodiscard]] std::uint32_t loadCommandsCount();
		[[nodiscard]] std::vector<LoadCommand*> loadCommands();
	};
	
	struct MachOBinary {  // NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)
		explicit MachOBinary(std::vector<char> &binary) : _binary(binary) {}
		~MachOBinary() = default;

		[[nodiscard]] bool valid() const;
		[[nodiscard]] bool fat() const;
		[[nodiscard]] std::vector<MachOArch*> architectures() const;
	private:
		std::vector<char> &_binary;
	};
}
