#pragma once

#include <std/type.hpp>
#include <std/expected.hpp>

namespace std::memory {
	enum class error {
		invalid_size
	};

	bool is_valid_size(std::type::usize size);

	std::expected<std::type::pointer, error> allocate(std::type::usize size);
	std::expected<void, error> free(std::type::pointer pointer, std::type::usize size);
	std::expected<void, error> copy(std::type::pointer source, std::type::pointer destination, std::type::usize size);

	namespace internal {
		struct block {
			std::type::usize size; block* next;
		};
	}
}