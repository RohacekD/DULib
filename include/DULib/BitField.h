#pragma once

#include <type_traits>

#include <cstdio>

namespace DULib {
/**
 * Mandatory for each enum used in bitField. Define how many bits your BitField gonna use
 * Needed for some methods such as all();
 */
template<typename Enum>
struct BitField_UsedBitsCounter {
  //static constexpr std::size_t usedBits = ...
};

/**
 * Define true in order to allow bit operations directly on Enum/bits
 * e.g. Enum::flag1 | Enum::flag2 results in bitfield
 */
template <typename Enum>
struct enable_BitField_operators {
  static constexpr bool enable = false;
};


template <class Enum, typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
class BitField {
	using value_type = typename std::underlying_type<Enum>::type;

public:
	template <typename = typename std::is_integral<value_type>::type>
	BitField() noexcept
		: m_Flags(0)
	{
	}

	constexpr BitField(const Enum bit) noexcept
		: m_Flags(static_cast<value_type>(bit))
	{
	}

	constexpr BitField(const std::initializer_list<Enum>& values) noexcept
		: m_Flags(0)
	{
		SetFlags(values);
	}


	constexpr BitField(const BitField& other) noexcept
		: m_Flags(other.m_Flags)
	{
	}

	constexpr BitField(const BitField&& other) noexcept
		: m_Flags(other.m_Flags)
	{
	}

	~BitField() = default;

	[[nodiscard]] constexpr bool CheckFlag(const Enum flag) const noexcept { return !!(static_cast<value_type>(flag) & m_Flags); }

	constexpr void SetFlag(const Enum flag) noexcept { m_Flags |= static_cast<value_type>(flag); }

	constexpr void SetFlags(const BitField& field) noexcept { m_Flags |= field.GetFlags(); }

	constexpr void SetFlags(const std::initializer_list<Enum>& values) noexcept
	{
		for (const auto e : values)
		{
			SetFlag(e);
		}
	}

	constexpr void ClearFlag(const Enum flag) noexcept { m_Flags &= ~static_cast<value_type>(flag); }

	constexpr void ClearFlags(const std::initializer_list<Enum>& values) noexcept
	{
		for (const auto e : values)
		{
			ClearFlag(e);
		}
	}

	constexpr void ToggleFlag(const Enum flag) noexcept { m_Flags ^= static_cast<value_type>(flag); }

	BitField& operator|=(const Enum bit) noexcept { return (*this = *this | bit); }

	[[nodiscard]] BitField operator|(const BitField bit) const noexcept
	{
		BitField ret(*this);
		ret.SetFlags(bit);
		return ret;
	}

	BitField& operator&=(const Enum bit) noexcept {
		m_Flags &= static_cast<value_type>(bit);
		return (*this);
	}

	[[nodiscard]] BitField operator&(const BitField& other) const noexcept {
		BitField ret(*this);
		ret.m_Flags &= other.m_Flags;
		return ret;
	}

	[[nodiscard]] bool operator&(const Enum bit) const noexcept { return CheckFlag(bit); }

	BitField& operator=(const value_type& value) noexcept
	{
		m_Flags = value;
		return *this;
	}

	BitField& operator=(const BitField& other) noexcept
	{
		m_Flags = other.m_Flags;
		return *this;
	}

	BitField& operator^=(const BitField other) noexcept {
		m_Flags ^= other.m_Flags;
		return *this;
	}

	[[nodiscard]] BitField operator^(const BitField& other) const noexcept {
		BitField ret(*this);
		ret.m_Flags ^= other.m_Flags;
		return ret;
	}

	constexpr BitField operator~() const noexcept{
		constexpr auto mask = GetMaskForUnusedBits();
		// unused bits should be set to 0 all the time so 
		// negating them and than xoring with mask should set them back to false

		BitField ret(*this);
		ret.m_Flags = (~m_Flags) ^ mask;
		return ret;
	}

	[[nodiscard]] operator const bool() const noexcept { return m_Flags != 0; }

	// Checks
	[[nodiscard]] constexpr bool all() const noexcept {
		constexpr auto mask = GetMaskForUnusedBits();
		return static_cast<value_type>(~(m_Flags^mask)) == 0;
	}
	[[nodiscard]] constexpr bool any() const noexcept { return !none(); }
	[[nodiscard]] constexpr bool none() const noexcept { return m_Flags == 0; }

	[[nodiscard]] value_type GetFlags() const noexcept { return m_Flags; }
protected:
	value_type m_Flags;
private:
	constexpr static value_type GetMaskForUnusedBits() {
		// todo consteval for C++20 ?
		return static_cast<value_type>(~((static_cast<value_type>(1u) << (usedBits)) - 1u));;
	}

	template <typename T>
	class HasNumBitsDefined
	{
	private:
		typedef char YesType[1];
		typedef char NoType[2];

		template <typename C> static YesType& test(decltype(&C::usedBits));
		template <typename C> static NoType& test(...);


	public:
		enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
	};
	static_assert(HasNumBitsDefined<BitField_UsedBitsCounter<Enum>>::value, "You have to define number of used bits. See BitField_UsedBitsCounter.");
	static constexpr std::size_t usedBits = BitField_UsedBitsCounter<Enum>::usedBits;
};

} // namespace DULib

template <typename Enum>
typename std::enable_if<DULib::enable_BitField_operators<Enum>  ::enable, DULib::BitField<Enum>>::type operator|(Enum lhs, Enum rhs)
{
	DULib::BitField<Enum> field(lhs);
	field.SetFlag(rhs);
	return field;
}

template <typename Enum>
typename std::enable_if<DULib::enable_BitField_operators<Enum>::enable, DULib::BitField<Enum>>::type operator&(Enum lhs, Enum rhs)
{
	DULib::BitField<Enum> field();
	if (lhs == rhs)
		field.SetFlag(lhs);
	return field;
}