#ifndef included_Text_Show_hpp
#define included_Text_Show_hpp

#include <sstream>
#include <string>

namespace Text {
namespace Show {

template < typename X >
struct instance_Show
{
    static std::string
    _show(const X& x) {
        std::ostringstream oss;
        oss << x;
        return oss.str();
    }
};

template <>
struct instance_Show< unsigned char >
{
    static std::string
    _show(const unsigned char& x) {
        static std::string cs[] = {"'\\NUL'","'\\SOH'","'\\STX'","'\\ETX'","'\\EOT'","'\\ENQ'","'\\ACK'","'\\a'","'\\b'","'\\t'","'\\n'","'\\v'","'\\f'","'\\r'","'\\SO'","'\\SI'","'\\DLE'","'\\DC1'","'\\DC2'","'\\DC3'","'\\DC4'","'\\NAK'","'\\SYN'","'\\ETB'","'\\CAN'","'\\EM'","'\\SUB'","'\\ESC'","'\\FS'","'\\GS'","'\\RS'","'\\US'","' '","'!'","'\"'","'#'","'$'","'%'","'&'","'\\''","'('","')'","'*'","'+'","','","'-'","'.'","'/'","'0'","'1'","'2'","'3'","'4'","'5'","'6'","'7'","'8'","'9'","':'","';'","'<'","'='","'>'","'?'","'@'","'A'","'B'","'C'","'D'","'E'","'F'","'G'","'H'","'I'","'J'","'K'","'L'","'M'","'N'","'O'","'P'","'Q'","'R'","'S'","'T'","'U'","'V'","'W'","'X'","'Y'","'Z'","'['","'\\\\'","']'","'^'","'_'","'`'","'a'","'b'","'c'","'d'","'e'","'f'","'g'","'h'","'i'","'j'","'k'","'l'","'m'","'n'","'o'","'p'","'q'","'r'","'s'","'t'","'u'","'v'","'w'","'x'","'y'","'z'","'{'","'|'","'}'","'~'","'\\DEL'","'\\128'","'\\129'","'\\130'","'\\131'","'\\132'","'\\133'","'\\134'","'\\135'","'\\136'","'\\137'","'\\138'","'\\139'","'\\140'","'\\141'","'\\142'","'\\143'","'\\144'","'\\145'","'\\146'","'\\147'","'\\148'","'\\149'","'\\150'","'\\151'","'\\152'","'\\153'","'\\154'","'\\155'","'\\156'","'\\157'","'\\158'","'\\159'","'\\160'","'\\161'","'\\162'","'\\163'","'\\164'","'\\165'","'\\166'","'\\167'","'\\168'","'\\169'","'\\170'","'\\171'","'\\172'","'\\173'","'\\174'","'\\175'","'\\176'","'\\177'","'\\178'","'\\179'","'\\180'","'\\181'","'\\182'","'\\183'","'\\184'","'\\185'","'\\186'","'\\187'","'\\188'","'\\189'","'\\190'","'\\191'","'\\192'","'\\193'","'\\194'","'\\195'","'\\196'","'\\197'","'\\198'","'\\199'","'\\200'","'\\201'","'\\202'","'\\203'","'\\204'","'\\205'","'\\206'","'\\207'","'\\208'","'\\209'","'\\210'","'\\211'","'\\212'","'\\213'","'\\214'","'\\215'","'\\216'","'\\217'","'\\218'","'\\219'","'\\220'","'\\221'","'\\222'","'\\223'","'\\224'","'\\225'","'\\226'","'\\227'","'\\228'","'\\229'","'\\230'","'\\231'","'\\232'","'\\233'","'\\234'","'\\235'","'\\236'","'\\237'","'\\238'","'\\239'","'\\240'","'\\241'","'\\242'","'\\243'","'\\244'","'\\245'","'\\246'","'\\247'","'\\248'","'\\249'","'\\250'","'\\251'","'\\252'","'\\253'","'\\254'","'\\255'"};
        std::ostringstream oss;
        oss << cs[x];
        return oss.str();
    }
};

template <>
struct instance_Show< std::string >
{
    static std::string
    _show(const std::string& x) {
        std::ostringstream oss;
        oss << "\"" << x << "\""; // TODO: まじめにエスケープ
        return oss.str();
    }
};

template < typename T >
std::string
show(const T& x)
{
    return instance_Show< T >::_show(x);
}

} // namespace Show
} // namespace Text

#endif
