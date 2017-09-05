#ifndef __CHECK_FUNC_HPP__
#define __CHECK_FUNC_HPP__

#include <functional>

typedef std::function<bool(const void *)> CheckFunc;

template<typename T, bool inclusive=true>
CheckFunc lbound_check(const T& lbound)
{
    auto f = [&](const void *buffer)->bool
    {
        const T *v = reinterpret_cast<const T*>(buffer);
        return inclusive ? (*v >= lbound) : (*v > lbound);
    };
    return CheckFunc(f);
}

template<typename T, bool inclusive=true>
CheckFunc ubound_check(const T& ubound)
{
    auto f = [&](const void *buffer)->bool
    {
        const T *v = reinterpret_cast<const T*>(buffer);
        return inclusive ? (*v <= ubound) : (*v < ubound);
    };
    return CheckFunc(f);
}

CheckFunc operator&&(const CheckFunc& f1,
                     const CheckFunc& f2);

CheckFunc operator||(const CheckFunc& f1,
                     const CheckFunc& f2);

#endif

