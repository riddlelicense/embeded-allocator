#pragma once

namespace std::type {
	using int8 = signed char;
	using uint8 = unsigned char;

	using int16 = signed short;
	using uint16 = unsigned short;

	using int32 = signed int;
	using uint32 = unsigned int;

	using int64 = signed long;
	using uint64 = unsigned long;

	using float32 = float;
	using float64 = double;

	using pointer = void*;

#ifdef __wasm32__
	using isize = int32;
	using usize = uint32;
#elifdef __wasm64__
	using isize = int64;
	using usize = uint64;
#endif
}