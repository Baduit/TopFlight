#pragma once

#include <concepts>

namespace YoloVM
{

template <typename T>
concept Addable = requires(const T& a, const T& b)
	{
		{ a + b } -> std::same_as<T>;
	};

template <typename T>
concept Substracable = requires(const T& a, const T& b)
	{
		{ a - b } -> std::same_as<T>;
	};

template <typename T>
concept Multiplyable = requires(const T& a, const T& b)
	{
		{ a * b } -> std::same_as<T>;
	};

template <typename T>
concept Dividable = requires(const T& a, const T& b)
	{
		{ a / b } -> std::same_as<T>;
	};

template <typename T>
concept Modulable = requires(const T& a, const T& b)
	{
		{ a % b } -> std::same_as<T>;
	};

template <typename T>
concept HasLogicAnd = requires(const T& a, const T& b)
	{
		{ a && b } -> std::same_as<T>;
	};

template <typename T>
concept HasLogicOr = requires(const T& a, const T& b)
	{
		{ a || b } -> std::same_as<T>;
	};

template <typename T>
concept HasLogicNot = requires(const T& a)
	{
		{ !a } -> std::same_as<T>;
	};

} // YoloVM