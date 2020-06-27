#pragma once

#include "Mach-O/Binary.hpp"

#include <string>

namespace Declarations {
	std::string_view headerName(MachO::Architecture architecture);
	std::string_view architectureName(MachO::Architecture architecture);
}
