#pragma once

#include <type_traits>

namespace DULib {
template <class Enum, typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
class BitField {
	using value_type = typename std::underlying_type<Enum>::type;

public:
	template <typename = typename std::is_integral<value_type>::type>
	BitField()
		: m_Flags(0)
	{
	}

	constexpr BitField(const Enum bit)
		: m_Flags(static_cast<value_type>(bit))
	{
	}

	constexpr BitField(const std::initializer_list<Enum>& values)
		: m_Flags(0)
	{
		SetFlags(values);
	}


	constexpr BitField(const BitField& other)
		: m_Flags(other.m_Flags)
	{
	}

	constexpr BitField(const BitField&& other)
		: m_Flags(other.m_Flags)
	{
	}

	~BitField() = default;

	[[nodiscard]] constexpr bool CheckFlag(const Enum flag) const { return !!(static_cast<value_type>(flag) & m_Flags); }

	constexpr void SetFlag(const Enum flag) { m_Flags |= static_cast<value_type>(flag); }

	constexpr void SetFlags(const BitField& field) { m_Flags |= field.GetFlags(); }

	constexpr void SetFlags(const std::initializer_list<Enum>& values)
	{
		for (const auto e : values)
		{
			SetFlag(e);
		}
	}

	constexpr void ClearFlag(const Enum flag) { m_Flags &= ~static_cast<value_type>(flag); }

	constexpr void ClearFlags(const std::initializer_list<Enum>& values)
	{
		for (const auto e : values)
		{
			ClearFlag(e);
		}
	}

	constexpr void ToggleFlag(const Enum flag) { m_Flags ^= static_cast<value_type>(flag); }

	BitField& operator|=(const Enum bit) { return (this = this | bit); }

	[[nodiscard]] BitField operator|(const BitField bit) const
	{
		BitField ret(*this);
		ret.SetFlags(bit);
		return ret;
	}

	BitField& operator&=(const Enum bit) { return (this = this & bit); }

	[[nodiscard]] BitField& operator=(const value_type& value)
	{
		m_Flags = value;
		return *this;
	}

	BitField& operator=(const BitField& other)
	{
		m_Flags = other.m_Flags;
		return *this;
	}

	[[nodiscard]] bool operator&(const Enum bit) const { return CheckFlag(bit); }

	[[nodiscard]] operator const Enum() const { return m_Flags; }

	[[nodiscard]] operator Enum&() { return m_Flags; }

	[[nodiscard]] value_type GetFlags() const { return m_Flags; }
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