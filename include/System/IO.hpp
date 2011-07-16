#ifndef included_System_IO_hpp
#define included_System_IO_hpp

#include <iostream>

namespace System {
namespace IO {

template < typename X >
void
print(const X& x)
{
    std::cout << Text::Show::show(x) << std::endl;
}

} // namespace IO
} // namespace System

#endif
