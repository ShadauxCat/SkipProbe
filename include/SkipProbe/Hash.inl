

// STL Strings

inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::string>::Compute(std::string const& value) noexcept
{
	return CityHash(value.data(), value.size());
}

inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::wstring>::Compute(std::wstring const& value) noexcept
{
	return CityHash(value.data(), value.size());
}

inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::u16string>::Compute(std::u16string const& value) noexcept
{
	return CityHash(value.data(), value.size());
}

inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::u32string>::Compute(std::u32string const& value) noexcept
{
	return CityHash(value.data(), value.size());
}

#if __cplusplus >= 201703L
inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::string_view>::Compute(std::string_view const& value) noexcept
{
	return CityHash(value.data(), value.size());
}

inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::wstring_view>::Compute(std::wstring_view const& value) noexcept
{
	return CityHash(value.data(), value.size());
}

inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::u16string_view>::Compute(std::u16string_view const& value) noexcept
{
	return CityHash(value.data(), value.size());
}

inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::u32string_view>::Compute(std::u32string_view const& value) noexcept
{
	return CityHash(value.data(), value.size());
}
#endif

// Pointer types

template <typename t_Hashable>
inline /*static*/ size_t SkipProbe::detail_::HashImpl<t_Hashable*>::Compute(t_Hashable* const& value) noexcept
{
	return HashImpl<uintptr_t>::Compute(reinterpret_cast<uintptr_t>(value));
}

template <typename t_PtrType>
inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::shared_ptr<t_PtrType>>::Compute(std::shared_ptr<t_PtrType> const& value) noexcept
{
	return HashImpl<uintptr_t>::Compute(reinterpret_cast<uintptr_t>(value.get()));
}

template <typename t_PtrType>
inline /*static*/ size_t SkipProbe::detail_::HashImpl<std::unique_ptr<t_PtrType>>::Compute(std::unique_ptr<t_PtrType> const& value) noexcept
{
	return HashImpl<uintptr_t>::Compute(reinterpret_cast<uintptr_t>(value.get()));
}

// Integer types

template<typename t_IntegerType>
inline /*static*/ size_t SkipProbe::detail_::HashImpl<t_IntegerType, typename std::enable_if<std::is_integral<t_IntegerType>::value && sizeof(t_IntegerType) <= 4>::type>::Compute(t_IntegerType const& value) noexcept
{
	// 32-bit integers use FNV rather than Murmur3 integer finisher due to superior performance characteristics.
	size_t result = c_fnvOffset;
	unsigned char const* const c = reinterpret_cast<unsigned char const* const>(&value);
	for (size_t i = 0; i < sizeof(value); ++i)
	{
		result ^= static_cast<size_t>(c[i]);
		result *= c_fnvPrime;
	}

	return size_t(result);
}

template<typename t_IntegerType>
inline /*static*/ size_t SkipProbe::detail_::HashImpl<t_IntegerType, typename std::enable_if<std::is_integral<t_IntegerType>::value && sizeof(t_IntegerType) == 8>::type>::Compute(t_IntegerType const& value) noexcept
{
#if defined(_WIN64) || defined(_M_X64) || defined(__x86_64__)
	return Murmur3::HashInt64(uint64_t(value));
#else
	return size_t(Murmur3::HashInt64(uint64_t(value))) % std::numeric_limits<size_t>::max();
#endif
}

// Floating point types

inline /*static*/ size_t SkipProbe::detail_::HashImpl<float>::Compute(float const& value) noexcept
{
	union { uint32_t i; float f; } convert;
	convert.f = value;
	return HashImpl<uint32_t>::Compute(convert.i);
}

inline /*static*/ size_t SkipProbe::detail_::HashImpl<double>::Compute(double const& value) noexcept
{
	union { uint64_t i; double f; } convert;
	convert.f = value;
	return HashImpl<uint64_t>::Compute(convert.i);
}

inline /*static*/ size_t SkipProbe::detail_::HashImpl<long double>::Compute(long double const& value) noexcept
{
	return CityHash(&value, sizeof(value));
}

template <typename t_Hashable, typename t_Enable>
inline /*static*/ size_t SkipProbe::detail_::HashImpl<t_Hashable, t_Enable>::Compute(t_Hashable const& value) noexcept
{
	return std::hash<t_Hashable>()(value);
}

template<typename t_Hashable>
size_t SkipProbe::Hash::operator()(t_Hashable const& value) const noexcept
{
	return detail_::HashImpl<adjusted_type>::Compute(value);
}