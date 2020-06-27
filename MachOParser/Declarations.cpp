#include "Declarations.hpp"

using namespace MachO;

std::string_view Declarations::headerName(const Architecture architecture) {
	if (architecture == Architecture::Unknown)
		return "Unknown";
	
	if (architecture == Architecture::x86_64 || architecture == Architecture::ARM64 || architecture == Architecture::POWERPC64)
		return "Mach64";

	return "Mach";
}

std::string_view Declarations::architectureName(Architecture architecture) {
	switch (architecture) {
		case Architecture::Unknown:
			return "Unknown";
		case Architecture::Any:
			return "Any";
		case Architecture::x86:
			return "x86";
		case Architecture::x86_64:
			return "x86_64";
		case Architecture::ARM:
			return "ARM";
		case Architecture::ARM64:
			return "ARM64";
		case Architecture::MC98000:
			return "MC98000";
		case Architecture::SPARC:
			return "SPARC";
		case Architecture::POWERPC:
			return "POWERPC";
		case Architecture::POWERPC64:
			return "POWERPC64";
		default:
			return "Unknown";
	}
}
