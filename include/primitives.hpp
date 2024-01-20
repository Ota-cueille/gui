#pragma once

#include <concepts>		// concept and requires

namespace primitive {

	template<typename T>
	concept type = requires(T) {
		// check that the type contains a parameters type inside
		typename T::parameters;
	};

} // namespace renderer::primitive

#include "primitives/quad.hpp"
#include "primitives/texture.hpp"
#include "primitives/bezier.hpp"
