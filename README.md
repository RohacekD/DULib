[![tests](https://github.com/RohacekD/DULib/actions/workflows/build.yml/badge.svg)](https://github.com/RohacekD/DULib/actions/workflows/build.yml)

# DULib

DULib evolves from my other projects as mix of utilities I am using across my school and personal projects.

## Bitfields

Bit field class allows you to use bit flags in the intuitive way. This class implements basic binary oprations on the bit flag while maintaining minimal size. The requirement is to match the size of used enum class.

```cpp
#include <DULib/BitField.h>

enum class E_TestEnum : std::uint8_t
{
  flag1 = 1,
  flag2 = 2,
  flag3 = 4,
  flag4 = 8,
};

// For some operation BitField needs to know how many bits do you actually use from given type
// e.g. all()
template<>
struct DULib::BitField_UsedBitsCounter<E_TestEnum> {
  static constexpr std::size_t usedBits = 4;
};

static_assert(sizeof(DULib::BitField<E_TestEnum>) == sizeof(std::uint8_t));
// allows binary operations on enum class, not mandatory
template <> struct DULib::enable_BitField_operators<E_TestEnum> {
static constexpr bool enable = true;
};

const auto bitfield = E_TestEnum::flag1 | E_TestEnum::flag2;
if constexpr (bitfield.CheckFlag(E_TestEnum::flag1))
{
  ...
}

```
