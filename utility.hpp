#pragma once

#include <std/meta.hpp>

namespace std::utility {
	template <typename T>
	typename std::meta::remove_reference<T>::type&& move(T&& value) noexcept {
		return static_cast<typename std::meta::remove_reference<T>::type&&>(value);
	}
}