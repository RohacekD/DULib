[![tests](https://github.com/RohacekD/DULib/actions/workflows/build.yml/badge.svg)](https://github.com/RohacekD/DULib/actions/workflows/build.yml)

# DULib

DULib evolves from my other projects as a mix of utilities I am using across my school and personal projects.

Supported standard C++17 and later.
## Integration
The library is header-only so far. 

### Premake5
Requires [Premake5-beta6](https://github.com/premake/premake-core/releases/tag/v5.0.0-beta6) and newer. 
```lua
workspace "MyWorkspace"
  include "include"
  project "MyProject"
    uses{"DULib"}
```

### Natvis
If you are using Premake5 and include the project of DULib, it will automatically add the `.natvis` file into your solution. Otherwise, you need to add [util/DULib.natvis](util/DULib.natvis) into your `.sln`.

## Bitfields

The bit field class allows you to use bit flags in an intuitive way. This class implements basic binary operations on the bit flag while maintaining minimal size. The requirement is to match the size of used enum class.

```cpp
#include <DULib/BitField.h>

enum class E_TestEnum : std::uint8_t
{
  flag1 = 1,
  flag2 = 2,
  flag3 = 4,
  flag4 = 8,
};

// For some operation, BitField needs to know how many bits you actually use from the given type
// e.g. all()
template<>
struct DULib::BitField_UsedBitsCounter<E_TestEnum> {
  static constexpr std::size_t usedBits = 4;
};

// allows binary operations on enum class, not mandatory
template <> struct DULib::enable_BitField_operators<E_TestEnum> {
static constexpr bool enable = true;
};

const auto bitfield = E_TestEnum::flag1 | E_TestEnum::flag2;
if constexpr (bitfield.CheckFlag(E_TestEnum::flag1))
{
  ...
}

// The size of the bitfield does not inflate compared to the enum definition
static_assert(sizeof(DULib::BitField<E_TestEnum>) == sizeof(std::uint8_t));

// supports simple std::hash for usage with std containers
DULib::BitField<E_TestEnum> field;
std::hash<DULib::BitField<E_TestEnum>>{}(field)
```
