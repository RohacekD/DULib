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

static_assert(sizeof(DULib::BitField<E_TestEnum>) == sizeof(std::uint8_t));

// #TODO: test all 
// template <> struct enable_BitField_operators<E_TestEnum> {
// static constexpr bool enable = true;
// };