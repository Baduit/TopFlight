#pragma once

#include <concepts>

namespace YoloVM
{

template <typename A, typename B>
concept Addable =
	requires(const A& a, const B& b)
	{
		{ a + b };
	};

template <typename A, typename B>
concept Substracable =
	requires(const A& a, const B& b)
	{
		{ a - b };
	};

template <typename A, typename B>
concept Multiplyable =
	requires(const A& a, const B& b)
	{
		{ a * b };
	};
template <typename A, typename B>
concept Dividable =
	requires(const A& a, const B& b)
	{
		{ a / b };
	};

template <typename A, typename B>
concept Modulable =
	requires(const A& a, const B& b)
	{
		{ a % b };
	};

template <typename A, typename B>
concept HasLogicAnd =
	requires(const A& a, const B& b)
	{
		{ a && b };
	};

template <typename A, typename B>
concept HasLogicOr =
	requires(const A& a, const B& b)
	{
		{ a || b };
	};

template <typename T>
concept HasLogicNot = requires(const T& a)
	{
		{ !a };
	};

} // YoloVM