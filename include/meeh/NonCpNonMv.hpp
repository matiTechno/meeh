#pragma once

namespace meeh
{

class NonCpNonMv
{
public:
    NonCpNonMv() = default;
    NonCpNonMv(const NonCpNonMv&) = delete;
    NonCpNonMv(NonCpNonMv&&) = delete;
    NonCpNonMv& operator=(const NonCpNonMv&) = delete;
    NonCpNonMv& operator=(NonCpNonMv&&) = delete;
};

} // meeeh
