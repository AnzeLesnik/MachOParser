#pragma once

#include "Mach-O.hpp"
#include "Utilities.hpp"

#include<vector>

namespace MachO {
	enum class Architecture : std::int32_t {
		Unknown,
		Any,
		x86,
		x86_64,
		ARM,
		ARM64,
		MC98000,
		SPARC,
		POWERPC,
		POWERPC64
	};

	enum class Endianness : std::uint32_t {
		LittleEndian,
		BigEndian
	};
	
	struct LoadCommand {
		// TODO: Implement
	};

	template<typename T>
	T endiannessBasedValue(T value, Endianness valueEndianness) {
		// Get the current systems endianness (1 if little-endian, 0 if big-endian)
		std::uint16_t temp {1};
		const auto endian = *reinterpret_cast<std::uint8_t*>(&temp);

		if (endian && valueEndianness == Endianness::LittleEndian)
			return value;
		else
			return Utilities::swapEndianness(value);
	}
	
	struct MachOArchitecture {
		explicit MachOArchitecture(MachOHeader *archHeader);
		~MachOArchitecture() = default;
		
		[[nodiscard]] bool valid();
		[[nodiscard]] Architecture architecture();
		[[nodiscard]] Endianness endianness();

		[[nodiscard]] std::uint32_t loadCommandsCount();
		[[nodiscard]] std::vector<LoadCommand*> loadCommands();

	private:
		MachOHeader *_archHeader {};
		Endianness _endianness   {};
	};
	
	struct MachOBinary {  // NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)
		explicit MachOBinary(std::vector<char> &binary) : _binary(binary) {}
		~MachOBinary() = default;

		[[nodiscard]] bool valid() const;
		[[nodiscard]] bool fat() const;
		[[nodiscard]] std::vector<MachOArchitecture*> architectures() const;
		
	private:
		std::vector<char> &_binary;
	};
}
