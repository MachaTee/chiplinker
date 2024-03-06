#pragma once

#ifndef _IOSTREAM_
    #include <iostream>
#endif

#define VERSION_UNSPECIFIED_ERROR "Version Unspecified\n"

#ifndef ALERTS_INCLUDED
#define ALERTS_INCLUDED

namespace alerts
{
    namespace messages { 
        constexpr std::string_view version = 
        {
            "ChipLinker version "
            #ifdef VERSION
            VERSION
            #else
            VERSION_UNSPECIFIED_ERROR
            #endif
        }; 

        constexpr std::string_view help = {
            "Basic usage:\n"
            "\tchiplinker -h\t\t\t\t\t-\tDisplay help message\n"
            "\tchiplinker -v\t\t\t\t\t-\tShow version number\n"
            "\tchiplinker -m <filename>\t\t\t-\tSet map file to custom file\n"
            "\tchiplinker -o <filename>\t\t\t-\tSet output to custom file (ignores EXENAME in map file)\n"
            "Advanced usage:\n"
            "\tchiplinker -l <file(s)> -a <address(es)>\t-\tDisregards map file, links -l object files with -a addresses\n"
            "\t- Note that you can also specify a -o at the end in lieu of an EXENAME, by default the output file is called output.rom\n"
            "\t- Also note that command-line parameters ALWAYS take precedence over map file data, if such data exists."
        };

        constexpr std::string_view output = {"Set output to {}\n"};
        constexpr std::string_view read_file = {"Reading {} as map file\n"};
        constexpr std::string_view parse_success = {"Parsing complete! Writing data to {} ...\n"};
        constexpr std::string_view success = {"Done\n"};
    }
    
    namespace malformed_argument
    {
        constexpr std::string_view no_file_specified = { "Malformed argument: No file specified" };
        constexpr std::string_view file_does_not_exist = { "Malformed argument: File does not exist"};
        constexpr std::string_view malformed_address = {"Malformed argument: Malformed address"};
        constexpr std::string_view invalid_address_to_link_file = {"Malformed argument: Not enough or too many addresses per object"};
    }

    namespace exceptions
    {
        constexpr std::string_view object_too_large = {"Object {} is too rotund ... lay off the fries!"};
        constexpr std::string_view file_does_not_exist = {"{}: No such file"};
    }
};

#endif