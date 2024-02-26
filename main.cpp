#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <regex>
#include "objects.h"
#include "utilities.h"

const std::vector<std::byte> process_segment(const std::string segment_name, const unsigned int starting_address, const unsigned int ending_address)
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
        throw std::length_error(messages::object_too_large(segment_name));
    
    // If no issue then proceed
    std::vector<std::byte> output_segment;

    // Put segment into output
    for (unsigned char i : buffer)
        output_segment.push_back((std::byte)i);
    
    // Pad with zeroes
    while (output_segment.size() < target_length)
        output_segment.push_back((std::byte)0U);
    
    return output_segment;
}

std::string output_file_name = "output.rom";
std::string file_path = "map.txt";

bool get_file_name = true;
bool link_mode = false;

link_and_address argument_handling(const std::vector<std::string_view> arg_vector)
{
    link_and_address lna;

    arg_mode mode = arg_mode::none;

    for (const std::string_view& arg : arg_vector)
    {
        switch(mode)
        {
            case arg_mode::none:
                break;
            case arg_mode::output:
                output_file_name = arg;
                get_file_name = false;
                std::cout << "Set output to " << output_file_name << std::endl;
                mode = arg_mode::none;
                continue;
            case arg_mode::mapfile:
                file_path = arg;
                std::cout << "Reading " << file_path << " as map file" << std::endl;
                mode = arg_mode::none;
                continue;
            case arg_mode::link:
                // Ensure is valid file name and not argument
                // If is argument, break and continue execution.
                if (arg[0] == '-')
                {
                    mode = arg_mode::none;
                    break;
                }
                // Check if file exists
                if (!std::filesystem::exists(arg)) 
                    throw std::runtime_error((std::string)arg + ": No such file");
                // Otherwise push to vector
                lna.link_files.push_back(arg);
                continue;
            case arg_mode::address:
                // Ensure is valid address and not argument
                // If is argument, break and continue execution.
                if (arg[0] == '-')
                {
                    mode = arg_mode::none;
                    break;
                }
                // If does not match regex, throw error
                if (!std::regex_match((std::string)arg, std::regex("(0x[0-9a-fA-F]+)-(0x[0-9a-fA-F]+)")))
                    throw std::runtime_error((std::string)messages::malformed_argument::malformed_address);
                // Otherwise push to vector
                lna.addresses.push_back(arg);
                continue;
        }
        
        switch(get_arg[arg])
        {
            // Quick and dirty argument handling for version and help (both output messages without doing anything else)
            case arg_type::help:
                messages::display_message(arg_type::help);
                exit(0);
            case arg_type::version:
                messages::display_message(arg_type::version);
                exit(0);
         
            // More advanced arguments set the state of the program to the specified arg_mode to be executed on the following iteration
            // Throw an exception if there aren't any more arguments to come
            case arg_type::output:
                mode = arg_mode::output;
                if (&arg == &arg_vector.back())
                    throw std::runtime_error((std::string)messages::malformed_argument::no_file_specified);
                continue;
            case arg_type::mapfile:
                mode = arg_mode::mapfile;                
                if (&arg == &arg_vector.back())
                    throw std::runtime_error((std::string)messages::malformed_argument::no_file_specified);
                continue;
            case arg_type::links:
                mode = arg_mode::link;
                link_mode = true;
                continue;
            case arg_type::addresses:
                mode = arg_mode::address;
                continue;
        }
    }

    // Post-processing of arguments
    if (link_mode)
    {
        // Check if equal number of addresses as object files
        if (lna.link_files.size() != lna.addresses.size())
            throw std::runtime_error((std::string)messages::malformed_argument::invalid_address_to_link_file);
    }

    return lna;
}

int main(int argc, char *argv[])
{
    /* TODO: commandline param handling */
    const std::vector<std::string_view> arguments(argv + 1, argv + argc);

    link_and_address lna {argument_handling(arguments)};
    std::vector<std::vector<std::string>> input_data;
    
    // Read file (if necessary)
    if (!link_mode)
    {
        std::ifstream input(file_path);

        // Split file lines into vectors of strings into input_data
        std::string line;

        while (std::getline(input, line))
            input_data.push_back(vsplit(line));
        
        // Strip comments
        for (size_t line = 0; line < input_data.size(); line++)
            if (!input_data[line][0].compare(";"))
                input_data.erase(input_data.begin() + line);
        
        // Close input file
        input.close();
    }
    else
    {
        // Generate pseudo-file from command-line arguments
        for (size_t line = 0; line < lna.link_files.size(); line++)
        {
            input_data.push_back(std::vector<std::string> 
            {
                SEGMENT_TOKEN, 
                (std::string)lna.link_files[line],
                (std::string)lna.addresses[line]
            });
        }
    }

    int file_size = 0;

    // Get output file size
    for (size_t line = 0; line < input_data.size(); line++)
    {
        if (token_lookup[input_data[line][0]] == maptoken::segname)
        {
            std::vector<std::string> temp_vector = vsplit(input_data[line].back(), '-');
            if (std::stoi(temp_vector.back(), 0, 0x10) > file_size)
                file_size = std::stoi(temp_vector.back(), 0, 0x10);
        }
    }

    // Create output byte array
    std::vector<std::byte> output_bytearray;

    // Pad byte array to desired size
    output_bytearray.resize(file_size);
    std::fill(output_bytearray.begin(), output_bytearray.end(), (std::byte)'\00');

    
    // Process tokens

    for (std::vector<std::string> lines : input_data)
    {
        switch (token_lookup[lines[0]])
        {
        case maptoken::exename:
        {
            if (get_file_name) 
            {
                output_file_name = std::accumulate(lines.begin() + 1, lines.end(), std::string(""));
                output_file_name.erase(std::remove(output_file_name.begin(), output_file_name.end(), '"'), output_file_name.end());
            }
            break;
        }
        case maptoken::segname:
        {
            lines[1].erase(std::remove(lines[1].begin(), lines[1].end(), '"'), lines[1].end());
            std::vector<std::string> temp_vector = vsplit(lines.back(), '-');
            std::vector<std::byte> segment_data = process_segment(lines[1], std::stoi(temp_vector.front(), 0, 0x10), std::stoi(temp_vector.back(), 0, 0x10));
            for (int addr = std::stoi(temp_vector.front(), 0, 0x10); addr < std::stoi(temp_vector.back(), 0, 0x10); addr++)
                output_bytearray[addr] = segment_data[addr - std::stoi(temp_vector.front(), 0, 0x10)];
            break;
        }
        default:
            break;
        }
    }

    // Otherwise process arguments
    std::cout << "Parsing complete! Writing data to " << output_file_name << "..." << std::endl;
    
    // Output to file
    std::ofstream fs(output_file_name, std::ios::out | std::ios::binary);
    fs.write((const char *)output_bytearray.data(), output_bytearray.size());
    fs.close();
    return 0;
}
