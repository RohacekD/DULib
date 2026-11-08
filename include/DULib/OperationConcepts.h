#pragma once

#include <concepts>

namespace DULib {

#pragma region -- arithmetics
template<typename LHS>
concept has_increment = requires(LHS x) { increment(x); };
template<typename LHS>
concept has_decrement = requires(LHS x) { decrement(x); };
template<typename LHS>
concept has_negation = requires(LHS x) { negate(x); };

template<typename LHS, typename RHS>
concept has_add = requires (LHS x, RHS y) { add(x, y); };
template<typename LHS, typename RHS>
concept has_subtract = requires (LHS x, RHS y) { subtract(x, y); };
template<typename LHS, typename RHS>
concept has_multiply = requires (LHS x, RHS y) { multiply(x, y); };
template<typename LHS, typename RHS>
concept has_divide = requires (LHS x, RHS y) { divide(x, y); };
template<typename LHS, typename RHS>
concept has_modulo = requires (LHS x, RHS y) { modulo(x, y); };
#pragma endregion

#pragma region -- logical
template<typename LHS, typename RHS>
concept has_logical_and = requires (LHS x, RHS y) { logicalAnd(x, y); };
template<typename LHS, typename RHS>
concept has_logical_or = requires (LHS x, RHS y) { logicalOr(x, y); };
#pragma endregion

#pragma region -- binary
template<typename LHS, typename RHS>
concept has_binary_and = requires (LHS x, RHS y) { binaryAnd(x, y); };
template<typename LHS, typename RHS>
concept has_binary_or = requires (LHS x, RHS y) { binaryOr(x, y); };
#pragma endregion

#pragma region -- comparators
template<typename LHS, typename RHS>
concept has_equals = requires (LHS x, RHS y) { equals(x, y); };
#pragma endregion

}