#pragma once

#include <cstdint>

namespace Utilities {
	template<typename T>
	T swapEndianness(T value) {
		switch (sizeof(T)) {
			case sizeof(std::int16_t):
				return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
			case sizeof(std::int32_t):
				return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00) |
				       ((value << 8) & 0x00FF0000)  | ((value << 24) & 0xFF000000);
			case sizeof(std::int64_t):
				return ((value >> 56) & 0x00000000000000FF) | ((value >> 40) & 0x000000000000FF00) |
				       ((value >> 24) & 0x0000000000FF0000) | ((value >> 8)  & 0x00000000FF000000) |
				       ((value <<  8) & 0x000000FF00000000) | ((value << 24) & 0x0000FF0000000000) |
				       ((value << 40) & 0x00FF000000000000) | ((value << 56) & 0xFF00000000000000);
			default:
				return value;
		}
	}
}
