#include <format>
#include <iostream>

#include <apbr/misc.hpp>
#include <internal/config/version.hpp>

void apbr::display_info()
{
    constexpr auto title   = apbr::internal::project::title;
    constexpr auto version = apbr::internal::project::version;
    constexpr auto name    = apbr::internal::project::name;

    std::clog << std::format("Welcome to {} version {}\n", name, version);
}