#pragma once
#include <unordered_map>

#define VERSION "v0.3\n"
#define UNKNOWN_MESSAGE_ERROR "This message should not be displayed"

#define EXENAME_TOKEN "EXENAME"
#define SEGMENT_TOKEN "SEGMENT"
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
    comment
};

enum class arg_type
{
    help,
    version,
    links,
    addresses,
    mapfile,
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

std::unordered_map<std::string_view, maptoken> token_lookup
{
    {EXENAME_TOKEN,     maptoken::exename},
    {SEGMENT_TOKEN,     maptoken::segname},
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

namespace messages
{
    namespace { 
        struct message
        {
            const std::string_view* contents;
            size_t length;
        };

        constexpr std::string_view version_message[] = 
        {
            "ChipLinker version ",
            VERSION
        }; 

        constexpr std::string_view help_message[] = {
            "Basic usage:\n",
            "\tchiplinker -h\t\t\t\t\t-\tDisplay help message\n",
            "\tchiplinker -v\t\t\t\t\t-\tShow version number\n",
            "\tchiplinker -m <filename>\t\t\t-\tSet map file to custom file\n",
            "\tchiplinker -o <filename>\t\t\t-\tSet output to custom file (ignores EXENAME in map file)\n",
            "Advanced usage:\n",
            "\tchiplinker -l <file(s)> -a <address(es)>\t-\tDisregards map file, links -l object files with -a addresses\n",
            "\t- Note that you can also specify a -o at the end in lieu of an EXENAME, by default the output file is called output.rom\n"
        };

        constexpr std::string_view unknown_message[] = 
        {
            UNKNOWN_MESSAGE_ERROR
        };
    }
    
    namespace malformed_argument
    {
        constexpr std::string_view no_file_specified = { "Malformed argument: No file specified" };
        constexpr std::string_view file_does_not_exist = { "Malformed argument: File does not exist"};
        constexpr std::string_view malformed_address = {"Malformed argument: Malformed address"};
        constexpr std::string_view invalid_address_to_link_file = {"Malformed argument: Not enough or too many addresses per object"};
    }

    const std::string object_too_large(const std::string obj)
    { 
        return "Object "+obj+" is too rotund ... lay off the fries!"; 
    };

    const void display_message(arg_type arg)
    {
        message msg;

        switch (arg)
        {
            case arg_type::help:
                msg.contents = help_message;
                msg.length = *(&help_message + 1) - help_message;
                break;
            case arg_type::version:
                msg.contents = version_message;
                msg.length = *(&version_message + 1) - version_message;
                break;
            default:
                msg.contents = unknown_message;
                msg.length = *(&unknown_message + 1) - unknown_message;
        }


        for (size_t line = 0; line < msg.length; line++)
        {
            std::cout << msg.contents[line];
        }

    }
};
