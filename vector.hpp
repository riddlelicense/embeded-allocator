#pragma once

#include <std/memory.hpp>
#include <std/utility.hpp>

namespace std {
	template <typename T>
	class vector;

	namespace resource {
		template <typename T>
		void capture(vector<T>& vector) {
			vector._begin = nullptr;

			vector._size = 0;
			vector._capacity = 0;
		}

		template <typename T>
		void release(vector<T>& vector) {
			if (vector._begin != nullptr) {
				std::memory::free(vector._begin, vector._capacity);

				vector._begin = nullptr;
			
				vector._size = 0;
				vector._capacity = 0;
			}
		}

		template <typename T>
		void capture(vector<T>* vector) {
			vector->_begin = nullptr;

			vector->_size = 0;
			vector->_capacity = 0;
		}

		template <typename T>
		void release(vector<T>* vector) {
			if (vector->_begin != nullptr) {
				std::memory::free(vector->_begin, vector->_capacity);
			}
			
			vector->_begin = nullptr;

			vector->_size = 0;
			vector->_capacity = 0;
		}
	}

	template <typename T>
	class alignas(16) /* crutch */ vector {
		enum class error {
			invalid_size,
			is_empty
		};
		
		friend void resource::capture(vector<T>& vector);
		friend void resource::release(vector<T>& vector);

		friend void resource::capture(vector<T>* vector);
		friend void resource::release(vector<T>* vector);

	public:
		vector() = delete;
		~vector() = delete;

		vector(const vector&) = delete;
		vector& operator=(const vector&) = delete;

		vector(vector&&) = delete;
		vector& operator=(vector&&) = delete;

		const std::type::usize size() const {
			return _size;
		}

		const std::type::usize capacity() const {
			return _capacity;
		}

		std::expected<void, error> reserve(std::type::usize capacity) {
			if (capacity <= _capacity || !std::memory::is_valid_size(capacity)) {
				return std::unexpected(error::invalid_size);
			}

			if (_begin == nullptr) {
				_begin = static_cast<T*>(std::memory::allocate(capacity).value());
			}
			else {
				T* new_begin = new_begin = static_cast<T*>(std::memory::allocate(capacity).value());

				std::memory::copy(_begin, new_begin, this->_capacity);
				std::memory::free(_begin, this->_capacity);

				_begin = new_begin;
			}

			this->_capacity = capacity;

			return {};
		}

		void push_back(const T& value) {
			if (_size == _capacity) {
				if (_capacity == 0) {
					reserve(sizeof(std::memory::internal::block));
				}
				else {
					reserve(_capacity * 2);
				}
			}

			_begin[_size++] = value;
		}

		void push_back(T&& value) {
			if (_size == _capacity) {
				if (_capacity == 0) {
					reserve(sizeof(std::memory::internal::block));
				}
				else {
					reserve(_capacity * 2);
				}
			}

			_begin[_size++] = std::utility::move(value);
		}

		std::expected<void, error> pop_back() {
			if (_begin == nullptr) {
				return std::unexpected(error::is_empty);
			}

			_begin[_size--].~T();

			return {};
		}

	private:
		T* _begin;

		std::type::usize _size;
		std::type::usize _capacity;
	};
}