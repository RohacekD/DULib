#pragma once

#include <DULib/OperationConcepts.h>

#include <utility>

namespace DULib {
template<typename Underlaying, typename Tag>
struct StrongType {
public:
	template<typename... Params>
	constexpr explicit StrongType(Params&&... params)
		requires std::is_constructible_v<Underlaying, Params...> : m_Value(std::forward<Underlaying>(params)...) {
	}

	[[nodiscard]] constexpr const Underlaying& get() const& { return m_Value; }
	[[nodiscard]] constexpr const Underlaying&& get()&& { return std::move(m_Value); }

	[[nodiscard]] constexpr bool operator==(const StrongType& other) const requires(has_equals<StrongType, StrongType>) {
		return get() == other.get();
	}
	[[nodiscard]] constexpr bool operator!=(const StrongType& other) const requires(has_equals<StrongType, StrongType>) {
		return get() != other.get();
	}

private:
	Underlaying m_Value;
};

#pragma region -- arithmetics
#pragma region -- unary
template<typename LHS>
[[nodiscard]] constexpr auto operator++(LHS&& x) ->decltype(increment(x)) requires(has_increment<LHS>) {
	return decltype(increment(x)){ ++std::forward<LHS>(x).get() };
}
template<typename LHS>
[[nodiscard]] constexpr auto operator--(LHS&& x) ->decltype(decrement(x)) requires(has_decrement<LHS>) {
	return decltype(decrement(x)){ --std::forward<LHS>(x).get() };
}
template<typename LHS>
[[nodiscard]] constexpr auto operator-(LHS&& x) ->decltype(negate(x)) requires(has_negation<LHS>) {
	return decltype(negate(x)){ -std::forward<LHS>(x).get() };
}
#pragma endregion

template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator+(LHS&& x, RHS&& y) ->decltype(add(x, y)) requires(has_add<LHS, RHS>) {
	return decltype(add(x, y)){ std::forward<LHS>(x).get() + std::forward<RHS>(y).get() };
}

template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator-(LHS&& x, RHS&& y) ->decltype(subtract(x, y)) requires(has_subtract<LHS, RHS>) {
	return decltype(subtract(x, y)){ std::forward<LHS>(x).get() - std::forward<RHS>(y).get() };
}

template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator*(LHS&& x, RHS&& y) ->decltype(multiply(x, y)) requires(has_multiply<LHS, RHS>) {
	return decltype(multiply(x, y)){ std::forward<LHS>(x).get()* std::forward<RHS>(y).get() };
}

template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator/(LHS&& x, RHS&& y) ->decltype(divide(x, y)) requires(has_divide<LHS, RHS>) {
	return decltype(divide(x, y)){ std::forward<LHS>(x).get() / std::forward<RHS>(y).get() };
}
#pragma endregion

#pragma region -- logical
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator&&(LHS&& x, RHS&& y) ->decltype(logicalAnd(x, y)) requires(has_logical_and<LHS, RHS>) {
	return decltype(logicalAnd(x, y)){ std::forward<LHS>(x).get() && std::forward<RHS>(y).get() };
}
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator||(LHS&& x, RHS&& y) ->decltype(logicalOr(x, y)) requires(has_logical_or<LHS, RHS>) {
	return decltype(logicalOr(x, y)){ std::forward<LHS>(x).get() || std::forward<RHS>(y).get() };
}
#pragma endregion

#pragma region -- binary
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator&(LHS&& x, RHS&& y) ->decltype(binaryAnd(x, y)) requires(has_binary_and<LHS, RHS>) {
	return decltype(binaryAnd(x, y)){ std::forward<LHS>(x).get()& std::forward<RHS>(y).get() };
}
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator|(LHS&& x, RHS&& y) ->decltype(binaryOr(x, y)) requires(has_binary_or<LHS, RHS>) {
	return decltype(binaryOr(x, y)){ std::forward<LHS>(x).get() | std::forward<RHS>(y).get() };
}

#pragma endregion

}