#pragma once

#ifndef _UNORDERED_MAP_
    #include <unordered_map>
#endif

#ifndef LANG
    #define LANG EN
#endif

#if(LANG==EN)
    #include "lang/en/alerts.hpp"
#endif

#ifndef OBJECTS_INCLUDED
#define OBJECTS_INCLUDED

#define EXENAME_TOKEN "EXENAME"
#define SEGMENT_TOKEN "SEGMENT"
#define PADBYTE_TOKEN "PADBYTE"
#define COMMENT_TOKEN ";"

struct link_and_address
{
    std::vector<std::string_view> link_files;
    std::vector<std::string_view> addresses;
};

enum class maptoken
{
    exename,
    segname,
    padbyte,
    comment
};

enum class arg_type
{
    help,
    version,
    links,
    addresses,
    mapfile,
    padbyte,
    output
};

enum class arg_mode
{
    output,
    link,
    address,
    mapfile,
    none
};

enum class msg_type
{
    version,
    help,
    output,
    read_file,
    parse_success,
    success
};

std::unordered_map<std::string_view, maptoken> token_lookup
{
    {EXENAME_TOKEN,     maptoken::exename},
    {SEGMENT_TOKEN,     maptoken::segname},
    {PADBYTE_TOKEN,     maptoken::padbyte},
    {COMMENT_TOKEN,     maptoken::comment},
};

std::unordered_map<std::string_view, arg_type> get_arg
{
    {"-h",              arg_type::help},
    {"--help",          arg_type::help},
    {"-v",              arg_type::version},
    {"--version",       arg_type::version},
    {"-l",              arg_type::links},
    {"-m",              arg_type::mapfile},
    {"-a",              arg_type::addresses},
    {"-o",              arg_type::output},
};

std::unordered_map<msg_type, std::string_view> message_lookup
{
    {msg_type::version,         alerts::messages::version},
    {msg_type::help,            alerts::messages::help},
    {msg_type::output,          alerts::messages::output},
    {msg_type::read_file,       alerts::messages::read_file},
    {msg_type::parse_success,   alerts::messages::parse_success},
    {msg_type::success,         alerts::messages::success},

};
#endif
