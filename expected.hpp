#pragma once

namespace std {
	template <typename E>
	class unexpected {
	public:
		unexpected(const E& error) : _error(error) {}

		unexpected(const unexpected&) = delete;
		unexpected& operator=(const unexpected&) = delete;

		unexpected(unexpected&&) = delete;
		unexpected& operator=(unexpected&&) = delete;

		const E& error() const {
			return _error;
		}

	private:
		E _error;
	};

	template <typename E>
	unexpected(E) -> unexpected<E>;

	template <typename T, typename E>
	class expected {
	public:
		expected(const T& value) : _has_value(true), _value(value) {}
		explicit expected(const E& error) : _has_value(false), _error(error) {}

		expected(const unexpected<E>& unexpected) : expected(unexpected.error()) {};
		expected& operator=(const unexpected<E>& unexpected) = delete;

		expected(unexpected<E>&& unexpected) : expected(unexpected.error()) {};
		expected& operator=(unexpected<E>&& unexpected) = delete;

		expected(const expected&) = delete;
		expected& operator=(const expected&) = delete;

		expected(expected&&) = delete;
		expected& operator=(expected&&) = delete;

		explicit operator bool() const {
			return _has_value;
		}

		const T& value() const {
			return _value;
		}

		const E& error() const {
			return _error;
		}

	private:
		bool _has_value;

		union {
			T _value;
			E _error;
		};
	};

	template <typename E>
	class expected<void, E> {
	public:
		expected() : _has_value(true) {}
		explicit expected(const E& error) : _has_value(false), _error(error) {}

		expected(const unexpected<E>& unexpected) : expected(unexpected.error()) {};
		expected& operator=(const unexpected<E>& unexpected) = delete;

		expected(unexpected<E>&& unexpected) : expected(unexpected.error()) {};
		expected& operator=(unexpected<E>&& unexpected) = delete;

		expected(const expected&) = delete;
		expected& operator=(const expected&) = delete;

		expected(expected&&) = delete;
		expected& operator=(expected&&) = delete;

		explicit operator bool() const {
			return _has_value;
		}

		const E& error() const {
			return _error;
		}

	private:
		bool _has_value;
		E _error;
	};
}