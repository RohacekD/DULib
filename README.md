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

## Strong types

Credits for the design: [Jason Turner - Strong Types – Better C++ (Keynote)](https://www.youtube.com/watch?v=UmB5oYgb3PU).

`DULib::StrongType<Underlaying, Tag>` wraps a primitive (or any type) in a distinct, zero-overhead type per `Tag`, so e.g. a `Meters` and a `Seconds` can't accidentally be added together. Every operator is opt-in: DULib's operator overloads are constrained via concepts that look up a matching free function through ADL, so an operator is only available for a given pair/combination of types once you declare that function for them.

```cpp
#include <DULib/StrongType.h>

using Meters = DULib::StrongType<float, struct meters_tag>;
using Seconds = DULib::StrongType<float, struct seconds_tag>;
using MetersPerSecond = DULib::StrongType<float, struct mps_tag>;

// only a *declaration* is required - these are never actually called at runtime, they only
// drive type deduction (decltype) and the has_xxx concept checks at compile time
MetersPerSecond divide(Meters, Seconds);
Meters add(Meters, Meters);

Meters distance{ 10.f };
Seconds time{ 2.f };
auto speed = distance / time; // MetersPerSecond, enabled by the `divide` declaration above
```

The table below lists every free function DULib's operators look for, and what declaring it enables:

| Function to declare | Signature | Enables |
|---|---|---|
| `add` | `Result add(LHS, RHS)` | `operator+`, `operator+=`; also enables `RHS + LHS` |
| `subtract` | `Result subtract(LHS, RHS)` | `operator-`, `operator-=` |
| `multiply` | `Result multiply(LHS, RHS)` | `operator*`, `operator*=`; also enables `RHS * LHS`  |
| `divide` | `Result divide(LHS, RHS)` | `operator/`, `operator/=` |
| `modulo` | `Result modulo(LHS, RHS)` | `operator%`  |
| `increment` | `Result increment(T)` | prefix `operator++` |
| `decrement` | `Result decrement(T)` | prefix `operator--` |
| `negate` | `Result negate(T)` | unary `operator-` |
| `logicalAnd` | `Result logicalAnd(LHS, RHS)` | `operator&&` |
| `logicalOr` | `Result logicalOr(LHS, RHS)` | `operator\|\|` |
| `binaryAnd` | `Result binaryAnd(LHS, RHS)` | `operator&` |
| `binaryOr` | `Result binaryOr(LHS, RHS)` | `operator\|` |
| `equals` | `bool equals(StrongType, StrongType)` | `operator==`, `operator!=` |
| `order` | `std::strong_ordering order(StrongType, StrongType)` | `operator<=>`, and therefore `<`, `<=`, `>`, `>=` |

For the arithmetic operators (`add`/`subtract`/`multiply`/`divide`), `RHS` doesn't have to be a `StrongType` - declaring e.g. `Meters multiply(Meters, float)` enables scaling a `StrongType` by a plain POD value directly (`distance * 2.f`).
