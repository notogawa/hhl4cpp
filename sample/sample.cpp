#include <Control/Monad.hpp>
#include <Data/List.hpp>
#include <Data/Maybe.hpp>
#include <System/IO.hpp>
#include <functional>

template < template < typename X > class M >
class divM
    : public std::unary_function< int , M< int > >
{
public:
    divM(int d) : d(d) {}
    M< int >
    operator()(int n) const {
        using namespace Control::Monad;
        return d ? returnM< M >(n/d) : mzero< M, int >();
    }
private:
    int d;
};

template < template < typename X > class M >
class incM
    : public std::unary_function< int , M< int > >
{
public:
    M< int >
    operator()(int n) const {
        using namespace Control::Monad;
        return returnM< M >(n+1);
    }
};

template < template < typename X > class M >
class showM
    : public std::unary_function< int , M< std::string > >
{
public:
    M< std::string >
    operator()(int n) const {
        using namespace Control::Monad;
        return returnM< M >(Text::Show::show(n));
    }
};

template < template < typename X > class M >
class filter_odd
    : public std::unary_function< int , M< int > >
{
public:
    M< int >
    operator()(int n) const {
        using namespace Control::Monad;
        return n%2 ? returnM< M >(n) : mzero< M, int >();
    }
};

template < typename F >
class map
    : public std::unary_function< Data::List::List< typename F::argument_type >,
                                  Data::List::List< typename F::result_type > >
{
public:
    map(const F& f) : f(f) {}
    Data::List::List< typename F::result_type >
    operator()(const Data::List::List< typename F::argument_type >& xs) const {
        using namespace Data::List;
        List< typename F::result_type > ys = Nil< typename F::result_type >();
        const int N = length(xs);
        for (int i = N; i--;) ys = Cons(f(at(xs, i)))(ys);
        return ys;
    }
    F f;
};

template < template < typename X > class M >
void
test()
{
    using namespace Control::Monad;
    using namespace System::IO;
    print(returnM< M >(1));
    print(returnM< M >(1)>>=incM< M >());
    print((returnM< M >(1)>>=divM< M >(0))>>=incM< M >());
    print(((returnM< M >(4)>>=divM< M >(2))>>=incM< M >())>>=showM< M >());
}

template < template < typename X > class M >
void
print_odd(const Data::List::List< int >& xs)
{
    using namespace Control::Monad;
    using namespace System::IO;
    // mapがまだないので
    filter_odd< M > odd;
    print(msum(map< filter_odd< M > >(odd)(xs)));
}

int main()
{
    using namespace Data::List;
    using namespace Data::Maybe;
    test< Maybe >();
    test< List >();
    List< int > xs = Cons(0)(Cons(1)(Cons(2)(Cons(3)(Nil< int >()))));
    print_odd< Maybe >(xs); // Maybeモナドなので最初のひとつが検出され Just 1
    print_odd< List >(xs); // Listモナドなので全部が検出され [1,3]
    return 0;
}
