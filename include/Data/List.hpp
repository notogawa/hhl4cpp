#ifndef included_Data_List_hpp
#define included_Data_List_hpp

#include <list>
#include <string>

namespace Data {
namespace List {

template < typename X >
class List;

template < typename X >
class _Cons_X;

template < typename X >
_Cons_X< X >
Cons(const X& x);

template < typename X >
List< X >
Nil();

template < typename X >
X
head(const List< X >& xs);

template < typename X >
List< X >
tail(const List< X >& xs);

template < typename X >
X
last(const List< X >& xs);

template < typename X >
List< X >
init(const List< X >& xs);

template < typename X >
bool
null(const List< X >& as);

template < typename X >
int
length(const List< X >& as);

template < typename X >
class _append_X;

template < typename X >
_append_X< X >
append(const List< X >& as);

template < typename X >
X
at(const List< X >& as, int ix);

template < typename X >
class List
{
public:
    List() : ls() {}
    List(const X& x) : ls(x) {}
    List(const List& src) : ls(src.ls) {}
    
private:
    std::list< X > ls;
    
    friend List< X >
    Nil< X >();
    
    friend X
    head< X >(const List< X >&);
    
    friend List< X >
    tail< X >(const List< X >&);
    
    friend X
    last< X >(const List< X >&);
    
    friend List< X >
    init< X >(const List< X >&);
    
    friend bool
    null< X >(const List< X >&);
    
    friend int
    length< X >(const List< X >&);
    
    friend
    _append_X< X >::_append_X(const List< X >&);
    friend List< X >
    _append_X< X >::operator()(const List< X >&) const;
    
    friend X
    at< X >(const List< X >&, int);
    
    friend List< X >
    _Cons_X< X >::operator()(const List< X >&) const;
};

template < typename X >
class _Cons_X
    : public std::unary_function< List< X >, List< X > >
{
public:
    _Cons_X(const X& x) : x(x) {}
    List< X >
    operator()(const List< X >& xs) const {
        List< X > result(xs);
        result.ls.push_front(x);
        return result;
    }
private:
    X x;
};

template < typename X >
_Cons_X< X >
Cons(const X& x)
{
    return _Cons_X< X >(x);
}

template < typename X >
List< X >
Nil()
{
    return List< X >();
}

template < typename X >
X
head(const List< X >& xs)
{
    if (xs.ls.begin() == xs.ls.end())
        throw std::string("*** Exception: Prelude.head: empty list");
    return *xs.ls.begin();
}

template < typename X >
List< X >
tail(const List< X >& xs)
{
    if (xs.ls.begin() == xs.ls.end())
        throw std::string("*** Exception: Prelude.tail: empty list");
    List< X > result(xs);
    result.ls.pop_front();
    return result;
}

template < typename X >
X
last(const List< X >& xs)
{
    if (xs.ls.begin() == xs.ls.end())
        throw std::string("*** Exception: Prelude.last: empty list");
    return *xs.ls.rbegin();
}

template < typename X >
List< X >
init(const List< X >& xs)
{
    if (xs.ls.begin() == xs.ls.end())
        throw std::string("*** Exception: Prelude.init: empty list");
    List< X > result(xs);
    result.ls.pop_back();
    return result;
}

template < typename X >
bool
null(const List< X >& xs)
{
    return xs.ls.empty();
}

template < typename X >
int
length(const List< X >& xs)
{
    return xs.ls.size();
}

// Listの特殊化時に_append_Xも特殊化されるが，こっちだとasに型がつかない
// 特殊化の相互再帰ってダメなんだっけ？
// template < typename X >
// class _append_X
//     : public std::unary_function< List< X >, List< X > >
// {
// public:
//     _append_X(const List< X >& as) : as(as) {}
//     List< X >
//     operator()(const List< X >& bs) const {
//         List< X > result;
//         result.ls.insert(result.ls.end(),
//                          bs.ls.begin(),
//                          bs.ls.end());
//         return result;
//     }
// private:
//     List< X > as;
// };

template < typename X >
class _append_X
    : public std::unary_function< List< X >, List< X > >
{
public:
    _append_X(const List< X >& as) : as(as.ls) {}
    List< X >
    operator()(const List< X >& bs) const {
        List< X > result;
        result.ls = as;
        result.ls.insert(result.ls.end(),
                         bs.ls.begin(),
                         bs.ls.end());
        return result;
    }
private:
    std::list< X > as;
};

template < typename X >
_append_X< X >
append(const List< X >& as)
{
    return _append_X< X >(as);
}

template < typename X >
X
at(const List< X >& xs, int ix)
{
    if (ix < 0)
        throw std::string("*** Exception: Prelude.(!!): negative index");
    if (length(xs) <= ix)
        throw std::string("*** Exception: Prelude.(!!): index too large");
    typename std::list< X >::const_iterator i = xs.ls.begin();
    for (;ix--;++i);
    return *i;
}

} // namespace List
} // namespace Data

#include "Text/Show.hpp"

namespace Text {
namespace Show {

template < typename X >
struct instance_Show< Data::List::List< X > >
{
    static std::string
    _show(const Data::List::List< X >& ls) {
        using namespace Data::List;
        const int N = length(ls);
        std::ostringstream oss;
        oss << "[";
        for (int i = 0; i < N; ++i)
            oss << (i ? "," : "") << Text::Show::show(at(ls, i));
        oss << "]";
        return oss.str();
    }
};

template <>
struct instance_Show< Data::List::List< unsigned char > >
{
    static std::string
    _show(const Data::List::List< unsigned char >& ls) {
        using namespace Data::List;
        const int N = length(ls);
        std::ostringstream oss;
        oss << "\"";
        for (int i = 0; i < N; ++i)
            oss << (i ? "," : "") << at(ls, i);
        oss << "\"";
        return oss.str();
    }
};

} // namespace Show
} // namespace Text

#include "Control/Monad.hpp"

namespace Control {
namespace Monad {

template <>
struct instance_Monad< Data::List::List >
{
    template < typename X >
    static Data::List::List< X >
    _return(const X& x) {
        using namespace Data::List;
        return Cons(x)(Nil< X >());
    }
    
    template < typename F >
    static typename F::result_type
    _bind(const Data::List::List< typename F::argument_type >& m, const F& f) {
        using namespace Data::List;
        typename F::result_type rs;
        for (int i = 0; i < length(m); ++i)
            rs = append(rs)(f(at(m, i)));
        return rs;
    }
};

template <>
struct instance_MonadPlus< Data::List::List >
{
    template < typename X >
    static Data::List::List< X >
    _mzero() {
        return Data::List::Nil< X >();
    }
    
    template < typename X >
    class _mplus_X
        : public std::unary_function< Data::List::List< X >,
                                      Data::List::List< X > > {
    public:
        _mplus_X(const Data::List::List< X >& a) : a(a) {}
        
        Data::List::List< X >
        operator()(const Data::List::List< X >& b) const {
            return Data::List::append(a)(b);
        }
    private:
        Data::List::List< X > a;
    };
};

} // namespace Monad
} // namespace Show

#endif
