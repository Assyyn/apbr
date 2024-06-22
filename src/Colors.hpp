#pragma once

namespace Log::Color {
constexpr auto reset      = "\033[0m";

constexpr auto red        = "\033[31m";
constexpr auto green      = "\033[92m";
constexpr auto light_gray = "\033[37m";
constexpr auto blue       = "\033[34m";
constexpr auto yellow     = "\033[33m";
constexpr auto magenta    = "\033[35m";

constexpr auto trace      = light_gray;
constexpr auto debug      = blue;
constexpr auto info       = green;
constexpr auto warn       = yellow;
constexpr auto error      = red;
constexpr auto fatal      = magenta;

}    // namespace Log::Color
