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

template<>
struct DULib::BitField_UsedBitsCounter<E_TestEnum> {
  static constexpr std::size_t usedBits = 4;
};

template <> struct DULib::enable_BitField_operators<E_TestEnum>  {
  static constexpr bool enable = true;
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

	SECTION("BitField(initializer_list)") {
		BitField<E_TestEnum> e2({ E_TestEnum::flag2, E_TestEnum::flag1 });
		REQUIRE(e2.CheckFlag(E_TestEnum::flag1)); //<!!!!!!!!
		REQUIRE(e2.CheckFlag(E_TestEnum::flag2)); //<!!!!!!!!
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("BitField(BitField&)") {
		e.SetFlag(E_TestEnum::flag1);
		BitField<E_TestEnum> e2(e);
		REQUIRE(e2.CheckFlag(E_TestEnum::flag1)); //<!!!!!!!!
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("BitField(BitField&&)") {
		e.SetFlag(E_TestEnum::flag1);
		BitField<E_TestEnum> e2(std::move(e));


		REQUIRE(e2.CheckFlag(E_TestEnum::flag1)); //<!!!!!!!!
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag2));
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

	SECTION("SetFlags(initializer_list)") {
		e.SetFlags({ E_TestEnum::flag2, E_TestEnum::flag1 });
		REQUIRE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));

		SECTION("ClearFlag(enum)") {
			e.ClearFlag(E_TestEnum::flag2);
			REQUIRE(e.CheckFlag(E_TestEnum::flag1));
			REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2));
			REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
			REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
		}

		SECTION("ClearFlags(initializer_list)") {
		  e.ClearFlags({ E_TestEnum::flag2, E_TestEnum::flag1, E_TestEnum::flag3 });
		  REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
		  REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2));
		  REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		  REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
		}
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

	SECTION("operator|=(enum)") {
		e |= E_TestEnum::flag1;
		REQUIRE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("operator|(enum)") {
		const auto e2 = e | E_TestEnum::flag1;
		REQUIRE(e2.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("operator&=(enum)") {
		e &= E_TestEnum::flag1;
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
		e.SetFlag(E_TestEnum::flag1);
		e &= E_TestEnum::flag1;
		REQUIRE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("operator&(enum)") {
		REQUIRE_FALSE(e & E_TestEnum::flag1);
		e.SetFlag(E_TestEnum::flag1);
		REQUIRE(e & E_TestEnum::flag1);
		REQUIRE_FALSE(e & E_TestEnum::flag2);
		REQUIRE_FALSE(e & E_TestEnum::flag3);
		REQUIRE_FALSE(e & E_TestEnum::flag4);
	}

	SECTION("operator&(BitField)") {
		const auto e2 = e & BitField<E_TestEnum>(E_TestEnum::flag1);
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
		e.SetFlag(E_TestEnum::flag1);
		const auto e3 = e & BitField<E_TestEnum>(E_TestEnum::flag1);
		REQUIRE(e3.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e3.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e3.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e3.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("operator=(value_type)") {
		BitField<E_TestEnum> e2;
		e2 = 6;
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e2.CheckFlag(E_TestEnum::flag2));
		REQUIRE(e2.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("operator=(value_type)") {
		e.SetFlag(E_TestEnum::flag2);
		e.SetFlag(E_TestEnum::flag3);
		BitField<E_TestEnum> e2;
		e2 = e;
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e.CheckFlag(E_TestEnum::flag2));
		REQUIRE(e.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("operator^=(BitField)") {
		BitField<E_TestEnum> e2;
		e2 ^= e; // 0^0 => 0
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag4));
		e.SetFlag(E_TestEnum::flag2);
		e2 ^= e;
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e2.CheckFlag(E_TestEnum::flag2)); //< !!!!
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag4));
		e2 ^= E_TestEnum::flag2;
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e2.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("operator^(BitField)") {
		BitField<E_TestEnum> e2;
		const auto e3 = e2 ^ e; // 0^0 => 0
		REQUIRE_FALSE(e3.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e3.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e3.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e3.CheckFlag(E_TestEnum::flag4));
		e.SetFlag(E_TestEnum::flag2);
		const auto e4 = e2 ^ e;
		REQUIRE_FALSE(e4.CheckFlag(E_TestEnum::flag1));
		REQUIRE(e4.CheckFlag(E_TestEnum::flag2)); //< !!!!
		REQUIRE_FALSE(e4.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e4.CheckFlag(E_TestEnum::flag4));
		const auto e5 = e4 ^ E_TestEnum::flag2;
		REQUIRE_FALSE(e5.CheckFlag(E_TestEnum::flag1));
		REQUIRE_FALSE(e5.CheckFlag(E_TestEnum::flag2));
		REQUIRE_FALSE(e5.CheckFlag(E_TestEnum::flag3));
		REQUIRE_FALSE(e5.CheckFlag(E_TestEnum::flag4));
	}

	SECTION("operator~()") {
		REQUIRE((~e).all() == true);
		REQUIRE((~e).GetFlags() == 15); // only first 4 bits should be set to 1
	}

	SECTION("all()") {
		REQUIRE_FALSE(e.all());
		e.SetFlag(E_TestEnum::flag3);
		REQUIRE_FALSE(e.all());
		e.SetFlag(E_TestEnum::flag1);
		e.SetFlag(E_TestEnum::flag2);
		e.SetFlag(E_TestEnum::flag4);
		REQUIRE(e.all());
	}

	SECTION("any()") {
		REQUIRE_FALSE(e.any());
		e.SetFlag(E_TestEnum::flag3);
		REQUIRE(e.any());
	}

	SECTION("none()") {
		REQUIRE(e.none());
		e.SetFlag(E_TestEnum::flag3);
		REQUIRE_FALSE(e.none());
	}

	SECTION("bool()") {
		REQUIRE_FALSE(static_cast<bool>(e));
		e.SetFlag(E_TestEnum::flag1);
		REQUIRE(static_cast<bool>(e));
	}
}
}