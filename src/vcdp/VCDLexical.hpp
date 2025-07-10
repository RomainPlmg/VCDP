#pragma once

#include <tao/pegtl.hpp>

#include "Config.hpp"

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace VCDP_NAMESPACE::lexical {

// Basic caracters
struct space : pegtl::one<' ', '\t'> {};
struct newline : pegtl::one<'\n', '\r'> {};
struct whitespace : pegtl::sor<space, newline> {};
struct whitespaces : pegtl::star<whitespace> {};
struct mandatory_space : pegtl::plus<whitespace> {};

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

struct scope_identifier : identifier {};
struct variable_identifier : symbol {};

// Numbers & values
struct number : pegtl::plus<pegtl::ascii::digit> {};
struct decimal_number : pegtl::seq<pegtl::star<pegtl::ascii::digit>, pegtl::opt<pegtl::seq<pegtl::one<'.'>, pegtl::plus<pegtl::ascii::digit>>>> {};
struct time_number : pegtl::sor<TAO_PEGTL_STRING("1"), TAO_PEGTL_STRING("10"), TAO_PEGTL_STRING("100")> {};

// Declaration keywords
struct dkw_comment : TAO_PEGTL_STRING("$comment") {};
struct dkw_date : TAO_PEGTL_STRING("$date") {};
struct dkw_enddefinitions : TAO_PEGTL_STRING("$enddefinitions") {};
struct dkw_scope : TAO_PEGTL_STRING("$scope") {};
struct dkw_timescale : TAO_PEGTL_STRING("$timescale") {};
struct dkw_upscope : TAO_PEGTL_STRING("$upscope") {};
struct dkw_var : TAO_PEGTL_STRING("$var") {};
struct dkw_version : TAO_PEGTL_STRING("$version") {};

// Simulation keywords
struct skw_dumpall : TAO_PEGTL_STRING("$dumpall") {};
struct skw_dumpoff : TAO_PEGTL_STRING("$dumpoff") {};
struct skw_dumpon : TAO_PEGTL_STRING("$dumpon") {};
struct skw_dumpvars : TAO_PEGTL_STRING("$dumpvars") {};

struct time_unit : pegtl::sor<TAO_PEGTL_STRING("s"), TAO_PEGTL_STRING("ms"), TAO_PEGTL_STRING("us"), TAO_PEGTL_STRING("ns"), TAO_PEGTL_STRING("ps"),
                              TAO_PEGTL_STRING("fs")> {};
struct scope_type : pegtl::sor<TAO_PEGTL_STRING("begin"), TAO_PEGTL_STRING("fork"), TAO_PEGTL_STRING("function"), TAO_PEGTL_STRING("module"),
                               TAO_PEGTL_STRING("task")> {};

// Keyword commands
struct command_comment : pegtl::seq<dkw_comment, mandatory_space, text_comment, mandatory_space, kw_end> {};
struct command_date : pegtl::seq<dkw_date, mandatory_space, text_date, mandatory_space, kw_end> {};
struct command_enddefinition : pegtl::seq<dkw_enddefinitions, mandatory_space, kw_end> {};
struct command_scope : pegtl::seq<dkw_scope, mandatory_space, scope_type, mandatory_space, scope_identifier, mandatory_space, kw_end> {};
struct command_timescale : pegtl::seq<dkw_timescale, mandatory_space, time_number, mandatory_space, time_unit, mandatory_space, kw_end> {};
struct command_upscope : pegtl::seq<dkw_upscope, mandatory_space, kw_end> {};
struct command_version : pegtl::seq<dkw_version, mandatory_space, text_version, mandatory_space, kw_end> {};

struct declaration_command : pegtl::sor<command_comment, command_date, command_scope, command_timescale, command_upscope, command_version> {};

// Sections
struct header_section : pegtl::star<whitespaces, declaration_command, whitespaces> {};
struct declaration_section : pegtl::seq<header_section, whitespaces, command_enddefinition, whitespaces> {};

// Complete file structure
struct vcd_file : pegtl::seq<whitespaces, declaration_section, whitespaces, pegtl::eof> {};

// Entry point
struct grammar : vcd_file {};

}  // namespace VCDP_NAMESPACE::lexical
