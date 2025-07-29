#pragma once

#include <tao/pegtl.hpp>

#include "Config.hpp"

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace VCDP_NAMESPACE::lexical {

// clang-format off

// Basic caracters
struct space : pegtl::one<' ', '\t'> {};
struct newline : pegtl::one<'\n', '\r'> {};
struct whitespace : pegtl::sor<space, newline> {};
struct whitespaces : pegtl::star<whitespace> {};
struct mandatory_space : pegtl::plus<whitespace> {};
struct eof : pegtl::eof {};

// End keyword
struct kw_end : TAO_PEGTL_STRING("$end") {};

// Text (for comments, etc.)
struct text_char : pegtl::sor<pegtl::ranges<' ', '~'>, whitespaces> {};
struct text : pegtl::star<pegtl::seq<pegtl::not_at<pegtl::seq<mandatory_space, kw_end>>, text_char>> {};
struct text_comment : text {};
struct text_date : text {};
struct text_version : text {};

// Identifier & symbols
struct identifier_char : pegtl::ranges<'a', 'z', 'A', 'Z', '0', '9', '_'> {};
struct identifier : pegtl::seq<pegtl::ranges<'a', 'z', 'A', 'Z', '_'>, pegtl::star<identifier_char>> {};

struct symbol_char : pegtl::ranges<'!', '~'> {};
struct symbol : pegtl::plus<symbol_char> {};

struct scope_identifier : pegtl::seq<pegtl::ranges<'a', 'z', 'A', 'Z', '_'>, pegtl::star<pegtl::sor<identifier_char, pegtl::one<'('>, pegtl::one<')'>>>> {};
struct var_identifier : symbol {};
struct var_name : identifier {};

// Numbers & values
struct number : pegtl::plus<pegtl::ascii::digit> {};
struct time_number : pegtl::sor<
    TAO_PEGTL_STRING("1"),
    TAO_PEGTL_STRING("10"),
    TAO_PEGTL_STRING("100")
> {};
struct var_size : number {};

// Declaration keywords
struct dkw_comment : TAO_PEGTL_STRING("$comment") {};
struct dkw_date : TAO_PEGTL_STRING("$date") {};
struct dkw_enddefinitions : TAO_PEGTL_STRING("$enddefinitions") {};
struct dkw_scope : TAO_PEGTL_STRING("$scope") {};
struct dkw_timescale : TAO_PEGTL_STRING("$timescale") {};
struct dkw_upscope : TAO_PEGTL_STRING("$upscope") {};
struct dkw_var : TAO_PEGTL_STRING("$var") {};
struct dkw_version : TAO_PEGTL_STRING("$version") {};

struct time_unit : pegtl::sor<
    TAO_PEGTL_STRING("s"),
    TAO_PEGTL_STRING("ms"),
    TAO_PEGTL_STRING("us"),
    TAO_PEGTL_STRING("ns"),
    TAO_PEGTL_STRING("ps"),
    TAO_PEGTL_STRING("fs")
> {};
struct scope_type : pegtl::sor<
    TAO_PEGTL_STRING("begin"),
    TAO_PEGTL_STRING("fork"),
    TAO_PEGTL_STRING("function"),
    TAO_PEGTL_STRING("module"),
    TAO_PEGTL_STRING("task")
> {};
struct var_type : pegtl::sor<
    TAO_PEGTL_STRING("event"),
    TAO_PEGTL_STRING("integer"),
    TAO_PEGTL_STRING("parameter"),
    TAO_PEGTL_STRING("realtime"),
    TAO_PEGTL_STRING("real"),
    TAO_PEGTL_STRING("reg"),
    TAO_PEGTL_STRING("supply0"),
    TAO_PEGTL_STRING("supply1"),
    TAO_PEGTL_STRING("time"),
    TAO_PEGTL_STRING("triand"),
    TAO_PEGTL_STRING("trior"),
    TAO_PEGTL_STRING("trireg"),
    TAO_PEGTL_STRING("tri0"),
    TAO_PEGTL_STRING("tri1"),
    TAO_PEGTL_STRING("tri"),
    TAO_PEGTL_STRING("wand"),
    TAO_PEGTL_STRING("wire"),
    TAO_PEGTL_STRING("wor")
> {};

struct scope_end : kw_end {};
struct var_end : kw_end {};

// In $var, the [0] or [3:0] for example
struct lsb_index : number {};
struct msb_index : number {};
struct bit_index_seq : pegtl::seq<
    pegtl::one<'['>,
    whitespaces,
    lsb_index,
    whitespaces,
    pegtl::one<']'>
> {};
struct bit_range_seq : pegtl::seq<
    pegtl::one<'['>,
    whitespaces,
    msb_index,
    whitespaces,
    pegtl::one<':'>,
    whitespaces,
    lsb_index,
    whitespaces,
    pegtl::one<']'>
> {};
struct var_reference : pegtl::seq<
    var_name,
    pegtl::opt<
        pegtl::seq<
            whitespaces,
            pegtl::sor<bit_index_seq, bit_range_seq>>>
> {};

// Keyword commands
struct command_comment : pegtl::seq<
    dkw_comment,
    pegtl::opt<pegtl::seq<mandatory_space, pegtl::not_at<kw_end>, text_comment>>,
    pegtl::must<mandatory_space>,
    pegtl::must<kw_end>
> {};
struct command_date : pegtl::seq<
    dkw_date,
    pegtl::must<mandatory_space>,
    pegtl::must<text_date>,
    pegtl::must<mandatory_space>,
    pegtl::must<kw_end>
> {};
struct command_enddefinitions : pegtl::seq<
    dkw_enddefinitions,
    pegtl::must<mandatory_space>,
    pegtl::must<kw_end>
> {};
struct command_scope : pegtl::seq<
    dkw_scope,
    pegtl::must<mandatory_space>,
    pegtl::must<scope_type>,
    pegtl::must<mandatory_space>,
    pegtl::must<scope_identifier>,
    pegtl::must<mandatory_space>,
    pegtl::must<scope_end>
> {};
struct command_timescale : pegtl::seq<
    dkw_timescale,
    pegtl::must<mandatory_space>,
    pegtl::must<time_number>,
    whitespaces,
    pegtl::must<time_unit>,
    pegtl::must<mandatory_space>,
    pegtl::must<kw_end>
> {};
struct command_upscope : pegtl::seq<
    dkw_upscope,
    pegtl::must<mandatory_space>,
    pegtl::must<kw_end>
> {};
struct command_var : pegtl::seq<
    dkw_var,
    pegtl::must<mandatory_space>,
    pegtl::must<var_type>,
    pegtl::must<mandatory_space>,
    pegtl::must<var_size>,
    pegtl::must<mandatory_space>,
    pegtl::must<var_identifier>,
    pegtl::must<mandatory_space>,
    pegtl::must<var_reference>,
    pegtl::must<mandatory_space>,
    pegtl::must<var_end>
> {};
struct command_version : pegtl::seq<
    dkw_version,
    pegtl::must<mandatory_space>,
    text_version,
    pegtl::must<mandatory_space>,
    pegtl::must<kw_end>
> {};

struct declaration_command : pegtl::sor<
    command_comment,
    command_date,
    command_scope,
    command_timescale,
    command_upscope,
    command_var,
    command_version
> {};

struct declaration_line : pegtl::seq<
    whitespaces,
    pegtl::not_at<command_enddefinitions>,
    pegtl::must<declaration_command>,
    pegtl::must<mandatory_space>
> {};

struct declaration_section : pegtl::seq<
    whitespaces,
    pegtl::star<declaration_line>,
    pegtl::must<command_enddefinitions>,
    whitespaces
> {};

// Entry point
struct grammar : pegtl::must<declaration_section> {};

// clang-format on

}  // namespace VCDP_NAMESPACE::lexical
