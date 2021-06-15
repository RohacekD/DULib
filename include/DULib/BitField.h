#pragma once

#include <type_traits>

#include <cstdio>

namespace DULib {
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


	// #TODO operator~

	[[nodiscard]] operator const bool() const noexcept { return m_Flags != 0; }

	// Checks
	[[nodiscard]] constexpr bool any() const noexcept { return !none(); }
	[[nodiscard]] constexpr bool none() const noexcept { return m_Flags == 0; }

	[[nodiscard]] value_type GetFlags() const noexcept { return m_Flags; }
protected:
	value_type m_Flags;
};

template <typename Enum> 
struct enable_BitField_operators {	static constexpr bool enable = false; };

} // namespace DULib

template <typename Enum> 
typename std::enable_if<DULib::enable_BitField_operators<Enum>::enable, DULib::BitField<Enum>>::type operator|(Enum lhs, Enum rhs)
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