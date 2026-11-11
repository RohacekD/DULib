#pragma once

// Credits for the design: JASON TURNER - Strong Types – Better C++ (Keynote)

#include <DULib/OperationConcepts.h>
#include <DULib/internals/MacroDefinitions.h>

#include <utility>

namespace DULib {
template<typename Underlaying, typename Tag>
struct StrongType;

template<typename T>
concept is_strong_type = requires(std::remove_cvref_t<T> t) {
	[]<typename Underlaying, typename Tag>(const StrongType<Underlaying, Tag>&) {}(t);
};

// unwraps a StrongType to its underlying value, passes anything else (e.g. a POD RHS) through unchanged
template<typename T>
[[nodiscard]] DU_LIB_FORCEINLINE constexpr decltype(auto) underlying_value(T&& value) {
	if constexpr (is_strong_type<T>) {
		return std::forward<T>(value).get();
	} else {
		return std::forward<T>(value);
	}
}

template<typename Underlaying, typename Tag>
struct StrongType {
public:
	template<typename... Params>
	constexpr explicit StrongType(Params&&... params)
		requires std::is_constructible_v<Underlaying, Params...> : m_Value(std::forward<Params>(params)...) {
	}

	[[nodiscard]] DU_LIB_FORCEINLINE constexpr const Underlaying& get() const& { return m_Value; }
	[[nodiscard]] DU_LIB_FORCEINLINE constexpr const Underlaying&& get()&& { return std::move(m_Value); }

	[[nodiscard]] constexpr bool operator==(const StrongType& other) const requires(has_equals<StrongType, StrongType>) {
		return get() == other.get();
	}
	[[nodiscard]] constexpr bool operator!=(const StrongType& other) const requires(has_equals<StrongType, StrongType>) {
		return get() != other.get();
	}

	[[nodiscard]] auto operator<=>(const StrongType& other) const
		requires(has_order<StrongType, StrongType>)
	= default;

#pragma region -- assignment operators arithmetics
	template<typename RHS>
	constexpr StrongType& operator+=(const RHS& other) requires(has_add<StrongType, RHS>&& std::is_same_v<decltype(add(*this, other)), StrongType>)
	{
		m_Value += underlying_value(other);
		return *this;
	}
	template<typename RHS>
	constexpr StrongType& operator-=(const RHS& other) requires(has_subtract<StrongType, RHS>&& std::is_same_v<decltype(subtract(*this, other)), StrongType>)
	{
		m_Value -= underlying_value(other);
		return *this;
	}
	template<typename RHS>
	constexpr StrongType& operator/=(const RHS& other) requires(has_divide<StrongType, RHS>&& std::is_same_v<decltype(divide(*this, other)), StrongType>)
	{
		m_Value /= underlying_value(other);
		return *this;
	}
	template<typename RHS>
	constexpr StrongType& operator*=(const RHS& other) requires(has_multiply<StrongType, RHS>&& std::is_same_v<decltype(multiply(*this, other)), StrongType>)
	{
		m_Value *= underlying_value(other);
		return *this;
	}

#pragma endregion
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

#pragma region -- add
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator+(LHS&& x, RHS&& y) ->decltype(add(x, y)) requires(has_add<LHS, RHS>) {
	return decltype(add(x, y)){ underlying_value(std::forward<LHS>(x)) + underlying_value(std::forward<RHS>(y)) };
}

// commutative property +
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator+(LHS&& x, RHS&& y) ->decltype(add(y, x)) requires(!has_add<LHS, RHS>&& has_add<RHS, LHS>) {
	return decltype(add(y, x)){ underlying_value(std::forward<LHS>(x)) + underlying_value(std::forward<RHS>(y)) };
}
#pragma endregion

#pragma region -- subtract
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator-(LHS&& x, RHS&& y) ->decltype(subtract(x, y)) requires(has_subtract<LHS, RHS>) {
	return decltype(subtract(x, y)){ underlying_value(std::forward<LHS>(x)) - underlying_value(std::forward<RHS>(y)) };
}
#pragma endregion

#pragma region -- multiply
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator*(LHS&& x, RHS&& y) ->decltype(multiply(x, y)) requires(has_multiply<LHS, RHS>) {
	return decltype(multiply(x, y)){ underlying_value(std::forward<LHS>(x)) * underlying_value(std::forward<RHS>(y)) };
}
// commutative property *
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator*(LHS&& x, RHS&& y) ->decltype(multiply(y, x)) requires(!has_multiply<LHS, RHS>&& has_multiply<RHS, LHS>) {
	return decltype(multiply(y, x)){ underlying_value(std::forward<LHS>(x)) * underlying_value(std::forward<RHS>(y)) };
}
#pragma endregion

#pragma region -- divide
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator/(LHS&& x, RHS&& y) ->decltype(divide(x, y)) requires(has_divide<LHS, RHS>) {
	return decltype(divide(x, y)){ underlying_value(std::forward<LHS>(x)) / underlying_value(std::forward<RHS>(y)) };
}
#pragma endregion

#pragma region -- modulo
template<typename LHS, typename RHS>
[[nodiscard]] constexpr auto operator%(LHS&& x, RHS&& y) ->decltype(modulo(x, y)) requires(has_modulo<LHS, RHS>) {
	return decltype(modulo(x, y)){ underlying_value(std::forward<LHS>(x)) % underlying_value(std::forward<RHS>(y)) };
}
#pragma endregion
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

#include <DULib/internals/MacroCleanup.h>