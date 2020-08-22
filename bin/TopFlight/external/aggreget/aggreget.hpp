#pragma once

#include "aggreget_generated.hpp"

namespace aggreget
{

/*
** To tuple
*/
template <Aggregate T>
constexpr auto to_tuple(const T& t)
{
	return details::to_tuple_impl_cref(t);
}

template <Aggregate T>
constexpr auto to_tuple(T&& t)
{
	return details::to_tuple_impl_fref(std::forward<T>(t));
}

/*
** Get
*/
template <std::size_t Index, Aggregate T>
constexpr auto get(const T& t)
{
	return std::get<Index>(to_tuple(t));
}

/*
** Foreach
*/
template <Aggregate T, typename Cb>
constexpr auto foreach(const T& t, Cb&& cb)
{
	return std::apply([&](auto&&... args) {(cb(args), ...);}, to_tuple(t));
}

template <Aggregate T, typename Cb>
constexpr auto foreach(T&& t, Cb&& cb)
{
	return std::apply([&](auto&&... args) {(cb(std::forward<decltype(args)>(args)), ...);}, to_tuple(std::forward<T>(t)));
}

/*
** Convert
*/
// Still early stage and expirimental
template <Aggregate Output, Aggregate Input>
constexpr auto convert(Input&& i)
{
	// Later add a static assert so that Output and Input have the same number of attributes
	return std::apply(
		[&](auto&&... args)
		{
			return Output { std::forward<decltype(args)>(args)... };
		},
		to_tuple(std::forward<Input>(i)));
}

template <Aggregate Output, Aggregate Input>
constexpr auto convert(const Input& i)
{
	// Later add a static assert so that Output and Input have the same number of attributes
	return std::apply(
		[&](const auto&... args)
		{
			return Output { args... };
		},
		to_tuple(i));
}

template <Aggregate Input>
constexpr std::size_t get_tuple_size(const Input&)
{
	return details::get_tuple_size_impl<Input>();
}

template <Aggregate Input>
constexpr std::size_t get_tuple_size()
{
	return details::get_tuple_size_impl<Input>();
}

}
