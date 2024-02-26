#pragma once
#include <vector>
#include <string>

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