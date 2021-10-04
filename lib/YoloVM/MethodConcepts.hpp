#pragma once

#include <concepts>

#include <YoloVM/Types.hpp>

namespace YoloVM
{

template <typename T>
concept HasGetAt =
	requires(T t, Integer i)
	{
		{ t.get_at(i) };
	};

template <typename T, typename ValueType>
concept HasStoreAt =
	requires(T t, Integer i, ValueType v)
	{
		{ t.store_at(i, std::move(v)) };
	};

template <typename T>
concept HasSize =
	requires(const T& t)
	{
		{ t.size() };
	};

template <typename T>
concept HasResize =
	requires(T t, Integer i)
	{
		{ t.resize(i) };
	};

template <typename T, typename ValueType>
concept HasInsert =
	requires(T t, Integer i, ValueType v)
	{
		{ t.insert(i, std::move(v)) };
	};

template <typename T, typename ValueType>
concept HasPushBack =
	requires(T t, ValueType v)
	{
		{ t.push_back(std::move(v)) };
	};

template <typename T, typename U>
concept HasConcat =
	requires(const T& t, const U& other)
	{
		{ t.concat(other) };
	};

template <typename T>
concept HasErase =
	requires(const T& t, Integer i)
	{
		{ t.erase(i) };
	};


}