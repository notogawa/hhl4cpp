#include "Data/List.hpp"
#include "Control/Monad.hpp"

#include <cppcutter.h>
#include <functional>
#include <string>
#include <sstream>

namespace calc {

// TODO: Eqを先にどうにかすべきだった気がする.
namespace {

template < typename X >
bool
equal(const Data::List::List< X >& as,
      const Data::List::List< X >& bs)
{
    cppcut_assert_equal(length(as), length(bs));
    for (int i = 0; i < length(as); ++i)
    {
        X x = at(as, i);
        X y = at(bs, i);
        cppcut_assert_equal(x, y);
    }
    return true;
}

} // anonymous namespace

void test_List_Nil()
{
    using namespace Data::List;
    cut_assert_true(null(Nil< int >()),
                    cppcut_message("True == null []"));
    cppcut_assert_equal(0, length(Nil< int >()),
                        cppcut_message("0 == length []"));
}

void test_List_Singleton()
{
    using namespace Data::List;
    int n = 1;
    List< int > ls = Cons(n)(Nil< int >());
    cppcut_assert_equal(1, length(ls),
                        cppcut_message("1 == length [n]"));
    cppcut_assert_equal(n, at(ls, 0),
                        cppcut_message("n == [n]!!0"));
    cut_assert_false(null(ls));
}

void test_List_Cons()
{
    using namespace Data::List;
    int a = 1;
    int b = 2;
    List< int > ls = Cons(b)(Nil< int >());
    cppcut_assert_equal(1 + length(ls),  length(Cons(a)(ls)),
                        cppcut_message("1 + length ls == length (a:ls)"));
    cppcut_assert_equal(a, at(Cons(a)(ls), 0),
                        cppcut_message("a == [a, b]!!0"));
    cppcut_assert_equal(b, at(Cons(a)(ls), 1),
                        cppcut_message("b == [a, b]!!1"));
}

void test_List_head()
{
    using namespace Data::List;
    List< int > ls = Cons(0)(Cons(1)(Cons(2)(Nil< int >())));
    cppcut_assert_equal(0, head(ls),
                        cppcut_message("0 == head [0,1,2]"));
    try
    {
        head(Nil< int >());
        cut_error("head [] must be Exception.");
    }
    catch (const std::string& exception)
    {
        std::string e("*** Exception: Prelude.head: empty list");
        cut_assert(exception == e, exception);
    }
}

void test_List_tail()
{
    using namespace Data::List;
    List< int > ls = Cons(1)(Cons(2)(Nil< int >()));
    List< int > xs = Cons(0)(ls);
    cut_assert_equal(equal, ls, tail(xs),
                     cppcut_message("[1,2] == tail [0,1,2]"));
    try
    {
        tail(Nil< int >());
        cut_error("head [] must be Exception.");
    }
    catch (const std::string& exception)
    {
        std::string e("*** Exception: Prelude.tail: empty list");
        cut_assert(exception == e, exception);
    }
}

void test_List_last()
{
    using namespace Data::List;
    List< int > ls = Cons(0)(Cons(1)(Cons(2)(Nil< int >())));
    cppcut_assert_equal(2, last(ls),
                        cppcut_message("2 == last [0,1,2]"));
    try
    {
        last(Nil< int >());
        cut_error("head [] must be Exception.");
    }
    catch (const std::string& exception)
    {
        std::string e("*** Exception: Prelude.last: empty list");
        cut_assert(exception == e, exception);
    }
}

void test_List_init()
{
    using namespace Data::List;
    List< int > xs = Cons(0)(Cons(1)(Cons(2)(Nil< int >())));
    List< int > ls = Cons(0)(Cons(1)(Nil< int >()));
    cut_assert_equal(equal, ls, init(xs),
                     cppcut_message("[0,1] == init [0,1,2]"));
    try
    {
        init(Nil< int >());
        cut_error("head [] must be Exception.");
    }
    catch (const std::string& exception)
    {
        std::string e("*** Exception: Prelude.init: empty list");
        cut_assert(exception == e, exception);
    }
}

void test_List_at()
{
    using namespace Data::List;
    List< int > ls = Cons(0)(Cons(1)(Nil< int >())); // [0, 1]
    try
    {
        at(ls, 2);
        cut_error("[0,1]!!2 must be Exception.");
    }
    catch (const std::string& exception)
    {
        std::string e("*** Exception: Prelude.(!!): index too large");
        cut_assert(exception == e, exception);
    }
    try
    {
        at(ls, -1);
        cut_error("[0,1]!!(-1) must be Exception.");
    }
    catch (const std::string& exception)
    {
        std::string e("*** Exception: Prelude.(!!): negative index");
        cut_assert(exception == e, exception);
    }
}

void test_List_append()
{
    using namespace Data::List;
    List< int > as = Cons(0)(Cons(1)(Nil< int >()));
    List< int > bs = Cons(2)(Cons(3)(Nil< int >()));
    List< int > abs = Cons(0)(Cons(1)(bs));
    List< int > bas = Cons(2)(Cons(3)(as));
    List< int > asbs = append(as)(bs);
    List< int > bsas = append(bs)(as);
    cut_assert_equal(equal, asbs, abs,
                     cppcut_message("[0,1] ++ [2,3] == [0,1,2,3]"));
    cut_assert_equal(equal, bsas, bas,
                     cppcut_message("[2,3] ++ [0,1] == [2,3,0,1]"));
}

namespace {

struct dbl
    : public std::unary_function< int, Data::List::List< int > >
{
    Data::List::List< int >
    operator()(int n) const {
        using namespace Data::List;
        return Cons(n)(Cons(n)(Nil< int >()));
    }
} f;

struct to_s
    : public std::unary_function< int, Data::List::List< std::string > >
{
    Data::List::List< std::string >
    operator()(int n) const {
        using namespace Data::List;
        std::ostringstream oss;
        oss << n;
        return Cons(oss.str())(Nil< std::string >());
    }
} g;

struct lambda_x_f_x_bind_g
    : public std::unary_function< int, Data::List::List< std::string > >
{
    Data::List::List< std::string >
    operator()(int n) const {
        using namespace Control::Monad;
        return f(n) >>= g;
    }
} lambda_x_f_x_bind_g;

struct ret
    : public std::unary_function< int, Data::List::List< int > >
{
    Data::List::List< int >
    operator()(int n) const {
        return Control::Monad::returnM< Data::List::List >(n);
    }
} ret;

} // anonymous namespace

void test_List_Monad_Law()
{
    using namespace Control::Monad;
    using namespace Data::List;
    cut_assert_equal(equal, returnM< List >(1) >>= f, f(1),
                     cppcut_message("return x >>= f == f x"));
    List< int > m = Cons(0)(Cons(1)(Nil< int >()));
    cut_assert_equal(equal, m >>= ret, m,
                     cppcut_message("m >>= return == m"));
    List< std::string > lhs = (m >>= f) >>= g;
    List< std::string > rhs = m >>= lambda_x_f_x_bind_g;
    cut_assert_equal(equal, lhs, rhs,
                     cppcut_message("m >>= f >>= g == m >>= \\x -> f x >>= g"));
}

void test_List_instance_Show()
{
    using namespace Text::Show;
    using namespace Data::List;
    List< int > nil = Nil< int >();
    List< int > xs = Cons(1)(nil);
    List< int > ys = Cons(0)(xs);
    cut_assert(std::string("[]") == show(nil),
               cppcut_message("\"[]\" == show []"));
    cut_assert(std::string("[1]") == show(xs),
               cppcut_message("\"[1]\" == show [1]"));
    cut_assert(std::string("[0,1]") == show(ys),
               cppcut_message("\"[0,1]\" == show [0,1]"));
}

void test_List_instance_MonadPlus()
{
    using namespace Control::Monad;
    using namespace Data::List;
    List< int > zero = mzero< List, int >();
    cut_assert_equal(equal, zero >>= f, zero,
                     cppcut_message("mzero >>= f == mzero"));
    List< int > xs = Cons(1)(Nil< int >());
    cut_assert_equal(equal, mplus(zero)(xs), xs,
                     cppcut_message("mzero `mplus` xs == xs"));
    cut_assert_equal(equal, mplus(xs)(zero), xs,
                     cppcut_message("xs `mplus` mzero == xs"));
}

} // namespace calc
