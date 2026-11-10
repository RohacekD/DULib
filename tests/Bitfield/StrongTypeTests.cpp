#include <catch.hpp>

#include <DULib/StrongType.h>

using position = DULib::StrongType<float, struct position_tag>;
using distance = DULib::StrongType<float, struct distance_tag>;
using velocity = DULib::StrongType<float, struct velocity_tag>;
using time_type = DULib::StrongType<float, struct time_tag>;
using acceleration = DULib::StrongType<float, struct acceleration_tag>;

velocity multiply(time_type, acceleration);
velocity add(velocity, velocity);
bool equals(velocity, velocity);
bool equals(position, position);
bool equals(time_type, time_type);
position add(position, distance);
time_type add(time_type, float);

auto order(velocity, velocity) -> std::strong_ordering;

auto subtract(position, distance) -> position;

bool equals(distance, distance);
distance multiply(distance, float);
distance divide(distance, float);
velocity divide(distance, time_type);
time_type subtract(time_type, float);

static_assert(std::is_same_v<velocity, decltype(std::declval<time_type>() * std::declval<acceleration>())>);
static_assert(std::is_same_v<velocity, decltype(std::declval<velocity>() + std::declval<velocity>())>);

//static_assert(std::is_standard_layout_v<distance> && std::is_trivially_constructible_v<distance>);

TEST_CASE("Strong types", "[StrongType]") {
	constexpr time_type t(5.f);
	constexpr acceleration a(2.f);

	REQUIRE(t.get() == 5.f);

	constexpr auto v = t * a;
	constexpr auto v2 = a * t;
	static_assert(v.get() == 10.f);
	static_assert(v == velocity{10.f});
	auto v1 = v;
	v1 += v;
	REQUIRE(v1 == velocity{ 20.f });
	REQUIRE(v1 + v == velocity{ 30.f });

	REQUIRE(v1 > v);

	constexpr position p{ 10.f };
	constexpr distance d{ 5.f };

	constexpr position p1 = p - d;

	constexpr position p2{ 5.f };
	constexpr distance d1{ 1.f };

	auto x = p2 + d1;

	REQUIRE(p2 + d1 == position{ 6.f });
	REQUIRE(d1 + p2== position{ 6.f });

	REQUIRE(t + 2.f == time_type{ 7.f });

	//static_assert(std::is_pod_v<time_type>);
}

TEST_CASE("StrongType add", "[StrongType][add]") {
	constexpr velocity v1{ 10.f };
	constexpr velocity v2{ 5.f };
	constexpr auto sum = v1 + v2;
	static_assert(sum.get() == 15.f);
	REQUIRE(sum == velocity{ 15.f });

	// StrongType + POD, backed only by the declared (never defined) `add(time_type, float)` stub
	constexpr time_type t{ 5.f };
	constexpr auto t2 = t + 2.f;
	static_assert(t2.get() == 7.f);
	REQUIRE(t2 == time_type{ 7.f });

	// POD + StrongType: only add(time_type, float) is declared, not add(float, time_type),
	// so this must go through the commutative fallback operator+
	constexpr auto t3 = 2.f + t;
	static_assert(t3.get() == 7.f);
	REQUIRE(t3 == time_type{ 7.f });
}

TEST_CASE("StrongType subtract", "[StrongType][subtract]") {
	constexpr position p{ 10.f };
	constexpr distance d{ 5.f };
	constexpr position p1 = p - d;
	static_assert(p1.get() == 5.f);
	REQUIRE(p1 == position{ 5.f });

	// StrongType - POD, backed by the declared `subtract(time_type, float)` stub
	constexpr time_type t{ 10.f };
	constexpr auto t2 = t - 2.f;
	static_assert(t2.get() == 8.f);
	REQUIRE(t2 == time_type{ 8.f });
}

TEST_CASE("StrongType multiply", "[StrongType][multiply]") {
	constexpr time_type t{ 5.f };
	constexpr acceleration a{ 2.f };

	constexpr auto v = t * a;
	static_assert(v.get() == 10.f);
	REQUIRE(v == velocity{ 10.f });

	// commutative fallback: only multiply(time_type, acceleration) is declared
	constexpr auto v2 = a * t;
	static_assert(v2.get() == 10.f);
	REQUIRE(v2 == velocity{ 10.f });

	// StrongType * POD, backed by the declared `multiply(distance, float)` stub
	constexpr distance d{ 4.f };
	constexpr auto scaled = d * 2.f;
	static_assert(scaled.get() == 8.f);
	REQUIRE(scaled == distance{ 8.f });

	// POD * StrongType: only multiply(distance, float) is declared, not multiply(float, distance),
	// so this must go through the commutative fallback operator*
	constexpr auto scaled2 = 2.f * d;
	static_assert(scaled2.get() == 8.f);
	REQUIRE(scaled2 == distance{ 8.f });
}

TEST_CASE("StrongType divide", "[StrongType][divide]") {
	constexpr distance d{ 10.f };
	constexpr time_type t{ 2.f };

	// StrongType / StrongType, backed by the declared `divide(distance, time_type)` stub
	constexpr auto v = d / t;
	static_assert(v.get() == 5.f);
	REQUIRE(v == velocity{ 5.f });

	// StrongType / POD, backed by the declared `divide(distance, float)` stub
	constexpr auto halved = d / 2.f;
	static_assert(halved.get() == 5.f);
	REQUIRE(halved == distance{ 5.f });
}

TEST_CASE("StrongType compound assignment", "[StrongType][compound-assignment]") {
	auto v = velocity{ 10.f };
	v += velocity{ 5.f };
	REQUIRE(v == velocity{ 15.f });

	auto p = position{ 10.f };
	p -= distance{ 3.f };
	REQUIRE(p == position{ 7.f });

	auto d = distance{ 2.f };
	d *= 3.f;
	REQUIRE(d == distance{ 6.f });

	auto d2 = distance{ 10.f };
	d2 /= 2.f;
	REQUIRE(d2 == distance{ 5.f });
}

TEST_CASE("StrongType comparisons", "[StrongType][comparisons]") {
	constexpr velocity v1{ 10.f };
	constexpr velocity v2{ 20.f };

	REQUIRE(v1 == velocity{ 10.f });
	REQUIRE(v1 != v2);
	REQUIRE(v1 < v2);
	REQUIRE(v2 > v1);
	REQUIRE(v1 <= velocity{ 10.f });
	REQUIRE(v2 >= v1);
}

// Negative tests: these are all compile-time checks - there is nothing to run, the point is that
// the operation must not be available at all for these type combinations.
//
// NOTE: `static_assert(!requires(velocity v, float f) { v + f; })` looks like the obvious way to
// write this, but on this MSVC toolset it does NOT work - a failed *built-in operator* overload
// resolution for a class type is not correctly suppressed inside a requires-expression, so instead
// of making the requires-expression false it hard-errors the build (confirmed in isolation,
// independent of this file: e.g. `requires(velocity v, int i) { v % i; }` fails with
// "error C2676: binary '%': 'velocity' does not define this operator..." even though the whole
// point of a requires-expression is to make that a no). So instead we check the library's own
// concepts directly (has_add, has_multiply, ...) - that's exactly the gate every operator in
// StrongType.h is constrained on, it's a plain function-call check (not operator lookup), and it
// is reliably SFINAE-safe.
TEST_CASE("StrongType negative - undefined operations don't compile", "[StrongType][negative]") {
	// add: only add(velocity,velocity), add(position,distance), add(time_type,float) are declared.
	// operator+ has a commutative fallback, so both directions must be checked.
	static_assert(!DULib::has_add<velocity, float> && !DULib::has_add<float, velocity>,
		"no add(velocity, float) is declared in either order");
	static_assert(!DULib::has_add<time_type, acceleration> && !DULib::has_add<acceleration, time_type>,
		"no add(time_type, acceleration) is declared in either order");

	// subtract: only subtract(position,distance), subtract(time_type,float) are declared.
	// operator- has no commutative fallback, so only the declared direction matters.
	static_assert(!DULib::has_subtract<velocity, float>, "no subtract(velocity, float) is declared");
	static_assert(!DULib::has_subtract<position, velocity>, "no subtract(position, velocity) is declared");

	// multiply: only multiply(time_type,acceleration), multiply(distance,float) are declared.
	// operator* has a commutative fallback, so both directions must be checked.
	static_assert(!DULib::has_multiply<time_type, float> && !DULib::has_multiply<float, time_type>,
		"no multiply(time_type, float) is declared in either order");
	static_assert(!DULib::has_multiply<position, distance> && !DULib::has_multiply<distance, position>,
		"no multiply(position, distance) is declared in either order");

	// divide: only divide(distance,time_type), divide(distance,float) are declared.
	// operator/ has no commutative fallback.
	static_assert(!DULib::has_divide<acceleration, float>, "no divide(acceleration, float) is declared");
	static_assert(!DULib::has_divide<velocity, time_type>, "no divide(velocity, time_type) is declared");

	// modulo: no modulo() free function is declared for any type here (and operator% isn't even
	// implemented on StrongType at all, regardless of this concept)
	static_assert(!DULib::has_modulo<velocity, int>, "no modulo(velocity, int) is declared");

	// unary ++/--/negate: no increment/decrement/negate free function is declared for any type here
	static_assert(!DULib::has_increment<time_type>, "no increment(time_type) is declared");
	static_assert(!DULib::has_decrement<time_type>, "no decrement(time_type) is declared");
	static_assert(!DULib::has_negation<time_type>, "no negate(time_type) is declared");

	// logical &&/||: no logicalAnd/logicalOr free function is declared for any type here
	static_assert(!DULib::has_logical_and<velocity, velocity>, "no logicalAnd(velocity, velocity) is declared");
	static_assert(!DULib::has_logical_or<velocity, velocity>, "no logicalOr(velocity, velocity) is declared");

	// binary &/|: no binaryAnd/binaryOr free function is declared for any type here
	static_assert(!DULib::has_binary_and<velocity, velocity>, "no binaryAnd(velocity, velocity) is declared");
	static_assert(!DULib::has_binary_or<velocity, velocity>, "no binaryOr(velocity, velocity) is declared");

	// compound assignment is gated by the exact same concepts as the free operators
	static_assert(!DULib::has_add<velocity, float>, "operator+= on velocity with a float RHS should be unavailable");
	static_assert(!DULib::has_subtract<velocity, float>, "operator-= on velocity with a float RHS should be unavailable");
	static_assert(!DULib::has_multiply<time_type, float>, "operator*= on time_type with a float RHS should be unavailable");
	static_assert(!DULib::has_divide<acceleration, float>, "operator/= on acceleration with a float RHS should be unavailable");
}