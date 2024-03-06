#pragma once

#ifndef _VECTOR_
    #include <vector>
#endif

#ifndef _STRING_
    #include <string>
#endif

#ifndef _IOSTREAM_
    #include <iostream>
#endif

#ifndef OBJECTS_INCLUDED
    #include "objects.hpp"
#endif

#ifndef UTILITIES_INCLUDED
#define UTILITIES_INCLUDED

const std::vector<std::string> vsplit(const std::string text, const char sep = ' ')
{
    // Declare return string vector
    std::vector<std::string> output_vector;

    size_t pos = text.find(sep);
    size_t initial_pos = 0;

    while (pos != std::string::npos)
    {
        output_vector.push_back(text.substr(initial_pos, pos - initial_pos));
        initial_pos = pos + 1;
        pos = text.find(sep, initial_pos);
    }

    // Add final string
    output_vector.push_back(text.substr(initial_pos, std::min(pos, text.size()) - initial_pos + 1));
    return output_vector;
}

namespace alerts
{
    const void display_message(const msg_type arg)
    {
        std::cout << message_lookup[arg];
    }

    const void display_message(const msg_type arg, const std::string args)
    {
        const std::string_view str_view = message_lookup[arg];
        std::cout << std::vformat(str_view, std::make_format_args(args));
    }
}

#endif