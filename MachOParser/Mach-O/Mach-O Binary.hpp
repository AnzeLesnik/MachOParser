#pragma once

#include<vector>

namespace MachO {
	struct MachOArch {
		[[nodiscard]] bool valid();
		[[nodiscard]] bool littleEndian();
	};
	
	struct MachOBinary {  // NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)
		explicit MachOBinary(std::vector<char> &binary) : _binary(binary) {}
		~MachOBinary() = default;

		[[nodiscard]] bool valid() const;
		[[nodiscard]] bool fat() const;
		[[nodiscard]] std::vector<MachOArch*> headers() const;
	private:
		std::vector<char> &_binary;
	};
}
