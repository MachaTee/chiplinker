#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <numeric>
#include <vector>
#include <algorithm>
#define DEBUG


enum maptoken
{
    exename,
    segname,
    comment
};

// Custom split function
std::vector<std::string> vsplit(std::string text, char sep=' ')
{
    // Declare return string vector
    std::vector<std::string> output_vector;

    int pos = text.find(sep);
    int initial_pos = 0;

    while (pos != std::string::npos) 
    {
        output_vector.push_back(text.substr(initial_pos, pos - initial_pos));
        initial_pos = pos + 1;
        pos = text.find(sep, initial_pos);
    }

    // Add final string
    output_vector.push_back(text.substr(initial_pos, std::min((size_t)pos, text.size()) - initial_pos + 1));
    return output_vector;
}

std::vector<std::byte> process_segment(std::string segment_name, unsigned int starting_address, unsigned int ending_address)
{
    // Read segment data
    std::ifstream segment_file(segment_name, std::ios::binary);

    // Put file into modifiable buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(segment_file), {});

    // Get segment length
    size_t segment_length = segment_file.tellg();

    // Close file
    segment_file.close();

    // Get target length
    size_t target_length = ending_address - starting_address;

    // Ensure that target length is not smaller than segment length
    if (target_length < segment_length)
    {
        throw std::length_error("Object " + segment_name + " is too rotund ... lay off the fries!");
    }

    // If no issue then proceed
    std::vector<std::byte> output_segment;

    // Put segment into output
    // std::cout << segment_length << std::endl;
    for (auto i : buffer)
    {
        // std::cout << i << std::endl;
        output_segment.push_back((std::byte)i);
    }

    // Pad with zeroes
    while (output_segment.size() < target_length)
    {
        output_segment.push_back((std::byte)'\00');
    }

    return output_segment;
}


int main(int argc, char* argv[])
{
    std::unordered_map<std::string, maptoken> token_lookup
    {
        {"EXENAME", maptoken::exename},
        {"SEGMENT", maptoken::segname},
        {";", maptoken::comment},
    };



    // Declare variables
    std::string output_file_name = "output.rom";
    std::string file_path = "map.txt";

    // Read file
    std::ifstream input(file_path);

#undef DEBUG


    // Split file lines into vectors of strings into input_data
    std::string line;
    std::vector<std::vector<std::string>> input_data;

    while (std::getline(input, line))
    {
        input_data.push_back(vsplit(line));
    }

    // Strip comments
    for (size_t line = 0; line < input_data.size(); line++)
    {
        if (!input_data[line][0].compare(";"))
        {
            input_data.erase(input_data.begin() + line);
        }
    }

#ifdef DEBUG
    for (auto i : input_data)
    {
        for (auto j : i)
        {
            std::cout << j << std::endl;
        }
    }
#endif

    int file_size = 0;

    // Get output file size
    for (size_t line = 0; line < input_data.size(); line++)
    {
        if (token_lookup[input_data[line][0]] == maptoken::segname)
        {
            std::vector<std::string> temp_vector = vsplit(input_data[line].back(), '-');
            if (std::stoi(temp_vector.back(), 0, 16) > file_size)
            {
                file_size = std::stoi(temp_vector.back(), 0, 16);
            }
        }
    }

    // Create output byte array
    std::vector<std::byte> output_bytearray;

    // Pad byte array to desired size
    output_bytearray.resize(file_size);
    std::fill(output_bytearray.begin(), output_bytearray.end(), (std::byte)'\00');

    // Process tokens
    for (auto lines : input_data)
    {
        switch (token_lookup[lines[0]])
        {
            case maptoken::exename:
            {
                output_file_name = std::accumulate(lines.begin() + 1, lines.end(), std::string(" "));
                output_file_name.erase(std::remove(output_file_name.begin(), output_file_name.end(), '"'), output_file_name.end());
                output_file_name.erase(std::remove(output_file_name.begin(), output_file_name.end(), ' '), output_file_name.end());
                break;
            }
            case maptoken::segname:
            {
                lines[1].erase(std::remove(lines[1].begin(), lines[1].end(), '"'), lines[1].end());
                std::vector<std::string> temp_vector = vsplit(lines.back(), '-');
                std::vector<std::byte> segment_data = process_segment(lines[1], std::stoi(temp_vector.front(), 0, 16), std::stoi(temp_vector.back(), 0, 16));
                for (int addr = std::stoi(temp_vector.front(), 0, 16); addr < std::stoi(temp_vector.back(), 0, 16); addr++)
                {
                    output_bytearray[addr] = segment_data[addr-std::stoi(temp_vector.front(), 0, 16)];
                }
                break;
            }
            default:
                break;
        }
    }

    std::cout << "Hello world " << std::endl;
    // Output to file
    std::ofstream fs(output_file_name, std::ios::out | std::ios::binary);
    fs.write((const char*)output_bytearray.data(), output_bytearray.size());
    fs.close();
    input.close();
    return 0;
}

