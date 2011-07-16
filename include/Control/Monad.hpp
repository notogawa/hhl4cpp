#ifndef included_Contol_Monad_hpp
#define included_Contol_Monad_hpp

#include <functional>

namespace Control {
namespace Monad {

template < template < typename X > class M >
struct instance_Monad
{
    template < typename X >
    static M< X >
    _return(const X& x);
    
    template < typename F >
    static typename F::result_type
    _bind(const M< typename F::argument_type >& m, const F& f);
};

template < template < typename X > class M, typename X >
M< X >
returnM(const X& x)
{
    return instance_Monad< M >::_return(x);
}

template < template < typename X > class M, typename F >
typename F::result_type
operator >>= (const M< typename F::argument_type >& m, const F& f)
{
    return instance_Monad< M >::_bind(m, f);
}

template < template < typename X > class M >
struct instance_MonadPlus
{
    template < typename X >
    static M< X >
    _mzero();
    
    template < typename X >
    class _mplus_X
        : public std::unary_function< M< X >, M< X > > {
    public:
        _mplus_X(const M< X >& a);
        
        M< X >
        operator()(const M< X >& b) const;
    };
};

template < template < typename X > class M, typename A >
M< A >
mzero()
{
    return instance_MonadPlus< M >::template _mzero< A >();
}

template < template < typename X > class M, typename A >
typename instance_MonadPlus< M >::template _mplus_X< A >
mplus(const M< A >& a)
{
    return typename instance_MonadPlus< M >::template _mplus_X< A >(a);
}

} // namespace Monad
} // namespace Control

#include "Data/List.hpp"

namespace Control {
namespace Monad {

template < template < typename X > class M, typename A >
M< A >
msum(const Data::List::List< M< A > >& ms)
{
    const int N = Data::List::length(ms);
    if (0 == N) return mzero< M, A >();
    M< A > result = Data::List::at(ms, 0);
    for (int i = 1; i < N; ++i)
        // TODO: result が mzero なら即return;
        result = mplus(result)(Data::List::at(ms,i));
    return result;
}

} // namespace Monad
} // namespace Control

#endif
