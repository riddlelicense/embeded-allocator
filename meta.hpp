#pragma once

namespace std::meta {
	template <typename T>
	struct remove_reference;

	template <typename T>
	struct remove_reference<T&> {
		using type = T;
	};

	template <typename T>
	struct remove_reference<T&&> {
		using type = T;
	};

	template <typename T>
	struct remove_const;

	template <typename T>
	struct remove_const<const T> {
		using type = T;
	};
}