#ifndef included_Data_Maybe_hpp
#define included_Data_Maybe_hpp

#include <string>
#include <sstream>

namespace Data {
namespace Maybe {

template < typename X >
class Maybe;

template < typename X >
Maybe< X >
Just(const X& x);

template < typename X >
Maybe< X >
Nothing();

template < typename X >
bool
isJust(const Maybe< X >& x);

template < typename X >
X
fromJust(const Maybe< X >& x);

template < typename X >
class Maybe
{
public:
    Maybe() : just(false) {}
    Maybe(const X& x) : just(true), x(x) {}
    Maybe(const Maybe& src) : just(src.just), x(src.x) {}
    
private:
    bool just;
    X x;
    
    friend Maybe< X >
    Just< X >(const X&);
    
    friend Maybe< X >
    Nothing< X >();
    
    friend bool
    isJust< X >(const Maybe< X >&);
    
    friend X
    fromJust< X >(const Maybe< X >&);
};

template < typename X >
Maybe< X >
Just(const X& x)
{
    return Maybe< X >(x);
}

template < typename X >
Maybe< X >
Nothing()
{
    return Maybe< X >();
}

template < typename X >
bool
isJust(const Maybe< X >& x)
{
    return x.just;
}

template < typename X >
X
fromJust(const Maybe< X >& x)
{
    if (!x.just)
        throw std::string("*** Exception: Maybe.fromJust: Nothing");
    return x.x;
}

} // namespace Maybe
} // namespace Data

#include "Text/Show.hpp"

namespace Text {
namespace Show {

template < typename X >
struct instance_Show< Data::Maybe::Maybe< X > >
{
    static std::string
    _show(const Data::Maybe::Maybe< X >& x) {
        using namespace Data::Maybe;
        std::ostringstream oss;
        oss << (isJust(x) ?
                "Just " + Text::Show::show< X >(fromJust(x)) :
                "Nothing");
        return oss.str();
    }
};

} // namespace Show
} // namespace Text

#include "Control/Monad.hpp"

namespace Control {
namespace Monad {

template <>
struct instance_Monad< Data::Maybe::Maybe >
{
    template < typename X >
    static Data::Maybe::Maybe< X >
    _return(const X& x) {
        using namespace Data::Maybe;
        return Just(x);
    }
    
    template < typename F >
    static typename F::result_type
    _bind(const Data::Maybe::Maybe< typename F::argument_type >& m, const F& f) {
        using namespace Data::Maybe;
        typename F::result_type nothing;
        return isJust(m) ? f(fromJust(m)) : nothing;
    }
};

template <>
struct instance_MonadPlus< Data::Maybe::Maybe >
{
    template < typename X >
    static Data::Maybe::Maybe< X >
    _mzero() {
        return Data::Maybe::Nothing< X >();
    }
    
    template < typename X >
    class _mplus_X
        : public std::unary_function< Data::Maybe::Maybe< X >,
                                      Data::Maybe::Maybe< X > > {
    public:
        _mplus_X(const Data::Maybe::Maybe< X >& a) : a(a) {}
        
        Data::Maybe::Maybe< X >
        operator()(const Data::Maybe::Maybe< X >& b) const {
            return Data::Maybe::isJust(a) ? a : b;
        }
    private:
        Data::Maybe::Maybe< X > a;
    };
};

} // namespace Monad
} // namespace Show

#endif
