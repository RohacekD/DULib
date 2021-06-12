#include <DULib/BitField.h>
#include <cstdint>

enum class E_TestEnum : std::uint8_t
{
	flag1 = 1,
	flag2 = 2,
	flag3 = 4,
	flag4 = 8,
};

static_assert(sizeof(DULib::BitField<E_TestEnum>) == sizeof(std::uint8_t));

int main()
{
	return 0;
}