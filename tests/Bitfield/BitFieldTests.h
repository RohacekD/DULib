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
	// start with default ctor
	BitField<E_TestEnum> e;

	// default ctor should init bit field to all 0
	REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
	REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2));
	REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
	REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));

	REQUIRE(e.GetFlags() == 0);


	SECTION("BitField(Enum)") {
		BitField<E_TestEnum> e2(E_TestEnum::flag2);
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e2.CheckFlag(E_TestEnum::flag2)); //<!!!!!!!!
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("SetFlag(enum)") {
		e.SetFlag(E_TestEnum::flag2);
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e.CheckFlag(E_TestEnum::flag2)); //<!!!!!!!!
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("SetFlags(BitField)") {
		BitField<E_TestEnum> e2(E_TestEnum::flag2);
		e2.SetFlag(E_TestEnum::flag1);
		REQUIRE(e2.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e2.CheckFlag(E_TestEnum::flag2));
		e.SetFlags(e2);
		REQUIRE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("ToggleFlag(enum)") {
		e.ToggleFlag(E_TestEnum::flag2);
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e.CheckFlag(E_TestEnum::flag2)); //<!!!!!!!!
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
		e.ToggleFlag(E_TestEnum::flag2);
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2)); //<!!!!!!!!
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
	}
}
}