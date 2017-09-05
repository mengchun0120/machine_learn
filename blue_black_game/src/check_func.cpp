#include <check_func.hpp>

CheckFunc operator&&(const CheckFunc& f1, const CheckFunc& f2)
{
    auto f = [&](const void *buffer)->bool
    {
        return f1(buffer) && f2(buffer);
    };

    return CheckFunc(f);
}

CheckFunc operator||(const CheckFunc& f1, const CheckFunc& f2)
{
    auto f = [&](const void *buffer)->bool
    {
        return f1(buffer) || f2(buffer);
    };

    return CheckFunc(f);
}

