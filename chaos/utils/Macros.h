//
// Created by 关鑫 on 2019-02-02.
//

#pragma once

#include <tuple>

#define ARGS_NUM(...)  (std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value)
#define ARGS_ARRAY(...) ARGS_NUM(__VA_ARGS__), __VA_ARGS__
