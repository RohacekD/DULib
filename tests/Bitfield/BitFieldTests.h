#pragma once

#include <catch.hpp>

#include <DULib/BitField.h>

// this file should test just static_asserts

#include <cstdint>

enum class E_TestEnum : std::uint8_t
{
	flag1 = 1,
	flag2 = 2,
	flag3 = 4,
	flag4 = 8,
};
namespace DULib {
TEST_CASE("Bit fields", "[BitField]") {
	SECTION("Initialization") {
		BitField<E_TestEnum> e;

		// default ctor should init bit field to all 0
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));

		REQUIRE(e.GetFlags() == 0);
	}
}
}