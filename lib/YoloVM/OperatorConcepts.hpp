#pragma once

#include <concepts>

namespace YoloVM
{

template <typename A, typename B>
concept Addable =
	requires(const A& a, const B& b)
	{
		{ a + b } -> std::same_as<A>;
	} ||
	requires(const A& a, const B& b)
	{
		{ a + b } -> std::same_as<B>;
	};

template <typename A, typename B>
concept Substracable =
	requires(const A& a, const B& b)
	{
		{ a - b } -> std::same_as<A>;
	} ||
	requires(const A& a, const B& b)
	{
		{ a - b } -> std::same_as<B>;
	};

template <typename A, typename B>
concept Multiplyable =
	requires(const A& a, const B& b)
	{
		{ a * b } -> std::same_as<A>;
	} ||
	requires(const A& a, const B& b)
	{
		{ a * b } -> std::same_as<B>;
	};

template <typename A, typename B>
concept Dividable =
	requires(const A& a, const B& b)
	{
		{ a / b } -> std::same_as<A>;
	} ||
	requires(const A& a, const B& b)
	{
		{ a / b } -> std::same_as<B>;
	};

template <typename A, typename B>
concept Modulable =
	requires(const A& a, const B& b)
	{
		{ a % b } -> std::same_as<A>;
	} ||
	requires(const A& a, const B& b)
	{
		{ a % b } -> std::same_as<B>;
	};

template <typename A, typename B>
concept HasLogicAnd =
	requires(const A& a, const B& b)
	{
		{ a && b } -> std::same_as<A>;
	} ||
	requires(const A& a, const B& b)
	{
		{ a && b } -> std::same_as<B>;
	};

template <typename A, typename B>
concept HasLogicOr =
	requires(const A& a, const B& b)
	{
		{ a || b } -> std::same_as<A>;
	} ||
	requires(const A& a, const B& b)
	{
		{ a || b } -> std::same_as<B>;
	};

template <typename T>
concept HasLogicNot = requires(const T& a)
	{
		{ !a } -> std::same_as<T>;
	};

} // YoloVM