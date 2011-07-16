#include "Data/Maybe.hpp"
#include "Control/Monad.hpp"

#include <cppcutter.h>
#include <functional>
#include <string>
#include <sstream>

namespace calc {

void test_List_Nothing()
{
    using namespace Data::Maybe;
    cut_assert_false(isJust(Nothing< int >()),
                     cppcut_message("False == isJust Nothing"));
    try
    {
        fromJust(Nothing< int >());
        cut_error("fromJust Nothing must be Exception.");
    }
    catch(const std::string& exception)
    {
        std::string e("*** Exception: Maybe.fromJust: Nothing");
        cut_assert(exception == e, exception);
    }
    
}

void test_List_Just()
{
    using namespace Data::Maybe;
    cut_assert_true(isJust(Just< int >(0)),
                    cppcut_message("True == isJust (Just 0)"));
    cppcut_assert_equal(0, fromJust(Just< int >(0)),
                        cppcut_message("0 == fromJust (Just 0)"));
}

namespace {

// TODO: Eqを先にどうにかすべきだった気がする.
template < typename X >
bool
equal(const Data::Maybe::Maybe< X >& a,
      const Data::Maybe::Maybe< X >& b)
{
    cppcut_assert_equal(isJust(a), isJust(b));
    if (isJust(a))
    {
        X x = fromJust(a);
        X y = fromJust(b);
        cppcut_assert_equal(x, y);
    }
    return true;
}

struct positive
    : public std::unary_function< int, Data::Maybe::Maybe< int > >
{
    Data::Maybe::Maybe< int >
    operator()(int n) const {
        using namespace Data::Maybe;
        return 0 < n ? Just(n) : Nothing< int >();
    }
} f;

struct to_s
    : public std::unary_function< int, Data::Maybe::Maybe< std::string > >
{
    Data::Maybe::Maybe< std::string >
    operator()(int n) const {
        using namespace Data::Maybe;
        std::ostringstream oss;
        oss << n;
        return Just(oss.str());
    }
} g;

struct lambda_x_f_x_bind_g
    : public std::unary_function< int, Data::Maybe::Maybe< std::string > >
{
    Data::Maybe::Maybe< std::string >
    operator()(int n) const {
        using namespace Control::Monad;
        return f(n) >>= g;
    }
} lambda_x_f_x_bind_g;

struct ret
    : public std::unary_function< int, Data::Maybe::Maybe< int > >
{
    Data::Maybe::Maybe< int >
    operator()(int n) const {
        return Control::Monad::returnM< Data::Maybe::Maybe >(n);
    }
} ret;

} // anonymous namespace

void test_Maybe_Monad_Law()
{
    using namespace Control::Monad;
    using namespace Data::Maybe;
    cut_assert_equal(equal, returnM< Maybe >(1) >>= f, f(1),
                     cppcut_message("return x >>= f == f x"));
    cut_assert_equal(equal, returnM< Maybe >(0) >>= f, f(0),
                     cppcut_message("return x >>= f == f x"));
    {
        Maybe< int > m = Just(1);
        cut_assert_equal(equal, m >>= ret, m,
                         cppcut_message("m >>= return == m"));
        Maybe< std::string > lhs = (m >>= f) >>= g;
        Maybe< std::string > rhs = m >>= lambda_x_f_x_bind_g;
        cut_assert_equal(equal, lhs, rhs,
                         cppcut_message("m >>= f >>= g == m >>= \\x -> f x >>= g"));
    }
    {
        Maybe< int > m = Nothing< int >();
        cut_assert_equal(equal, m >>= ret, m,
                         cppcut_message("m >>= return == m where m = Just 0"));
        Maybe< std::string > lhs = (m >>= f) >>= g;
        Maybe< std::string > rhs = m >>= lambda_x_f_x_bind_g;
        cut_assert_equal(equal, lhs, rhs,
                         cppcut_message("m >>= f >>= g == m >>= \\x -> f x >>= g"));
    }
}

void test_Maybe_instance_Show()
{
    using namespace Data::Maybe;
    using namespace Text::Show;
    Maybe< int > just = Just(1);
    Maybe< int > nothing = Nothing< int >();
    cut_assert(std::string("Just 1") == show(just),
               cppcut_message("\"Just 1\" == show (Just 1)"));
    cut_assert(std::string("Nothing") == show(nothing),
               cppcut_message("\"Nothing\" == show Nothing"));
}

void test_Maybe_instance_MonadPlus()
{
    using namespace Control::Monad;
    using namespace Data::Maybe;
    Maybe< int > zero = mzero< Maybe, int >();
    cut_assert_equal(equal, zero >>= f, zero,
                     cppcut_message("mzero >>= f == mzero"));
    Maybe< int > xs = Just(1);
    cut_assert_equal(equal, mplus(zero)(xs), xs,
                     cppcut_message("mzero `mplus` xs == xs"));
    cut_assert_equal(equal, mplus(xs)(zero), xs,
                     cppcut_message("xs `mplus` mzero == xs"));
}

} // namespace calc
