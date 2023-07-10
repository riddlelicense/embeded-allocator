#include <std/memory.hpp>

extern std::type::usize __heap_base;

namespace std::memory {
	namespace internal {
		block* head = nullptr;
		block* tail = nullptr;

		std::type::pointer end = &__heap_base;
	}

	bool is_valid_size(std::type::usize size) {
		return ((size & (size - 1)) == 0 || size >= sizeof(internal::block));
	}

	std::expected<std::type::pointer, error> allocate(std::type::usize size) {
		if (!is_valid_size(size)) {
			return std::unexpected(error::invalid_size);
		}

		if (internal::head != nullptr) {
			if (internal::tail == internal::head) {
				if (internal::tail->size == size) {
					std::type::pointer pointer = internal::tail;

					internal::tail = nullptr;
					internal::head = nullptr;

					return pointer;
				}
				else if (internal::tail->size > size) {
					internal::tail->size -= size;
					return reinterpret_cast<std::type::int8*>(internal::tail) + internal::tail->size;
				}
			}
			else {
				internal::block* minimum = nullptr;

				internal::block* left = nullptr;
				internal::block* right = internal::head;

				do {
					if (right->size == size) {
						if (left == nullptr) {
							internal::head = internal::head->next;
						}
						else {
							left->next = right->next;
						}

						return right;
					}
					else if (right->size > size && (minimum == nullptr || minimum->size > right->size)) {
						minimum = right;
					}

					left = right;
					right = right->next;
				} while (right != nullptr);

				if (minimum != nullptr) {
					minimum->size -= size;
					return reinterpret_cast<std::type::int8*>(minimum) + minimum->size;
				}
			}
		}

		std::type::pointer pointer = internal::end;
		internal::end = static_cast<std::type::int8*>(internal::end) + size;

		return pointer;
	}

	std::expected<void, error> free(std::type::pointer pointer, std::type::usize size) {
		if (!is_valid_size(size)) {
			return std::unexpected(error::invalid_size);
		}

		if (internal::head == nullptr) {
			internal::head = static_cast<internal::block*>(pointer);

			internal::head->size = size;
			internal::head->next = nullptr;

			internal::tail = internal::head;
		}
		else if (internal::head > pointer) {
			if (static_cast<std::type::int8*>(pointer) + size == static_cast<std::type::pointer>(internal::head)) {
				internal::block* block = static_cast<internal::block*>(pointer);

				block->size = size + internal::head->size;
				block->next = internal::head->next;

				if (internal::tail == internal::head) {
					internal::tail = block;
				}

				internal::head = block;
			}
			else {
				internal::block* block = static_cast<internal::block*>(pointer);

				block->size = size;
				block->next = internal::head;

				internal::head = block;
			}
		}
		else if (internal::tail < pointer) {
			if (static_cast<std::type::int8*>(pointer) - internal::tail->size == static_cast<std::type::pointer>(internal::tail)) {
				internal::tail->size += size;
			}
			else {
				internal::block* block = static_cast<internal::block*>(pointer);

				block->size = size;
				block->next = nullptr;

				internal::tail->next = block;
				internal::tail = block;
			}
		}
		else {
			internal::block* left = internal::head;
			internal::block* right = internal::head->next;

			while (right < pointer) {
				left = right;
				right = right->next;
			}

			if (reinterpret_cast<std::type::int8*>(left) + left->size  == pointer) {
				left->size += size;

				if (reinterpret_cast<std::type::int8*>(right) - size == pointer) {
					left->size += right->size;
					left->next = right->next;

					if (internal::tail == right) {
						internal::tail = left;
					}
				}
			}
			else if (reinterpret_cast<std::type::int8*>(right) - size == pointer) {
				internal::block* block = static_cast<internal::block*>(pointer);

				block->size = size + right->size;
				block->next = right->next;

				left->next = block;

				if (internal::tail == right) {
					internal::tail = block;
				}
			}
			else {
				internal::block* block = static_cast<internal::block*>(pointer);

				block->size = size;
				block->next = right;

				left->next = block;
			}
		}

		return {};
	}

	std::expected<void, error> copy(std::type::pointer source, std::type::pointer destination, std::type::usize size) {
		if (!is_valid_size(size)) {
			return std::unexpected(error::invalid_size);
		}

		for (std::type::usize byte = 0; byte < size; byte++) {
			static_cast<std::type::int8*>(destination)[byte] = static_cast<std::type::int8*>(source)[byte];
		}

		return {};
	}
}