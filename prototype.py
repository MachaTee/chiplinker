from enum import Enum, auto


class maptoken(Enum):
    exename = auto()
    segment = auto()
    name = auto()
    filename = auto()
    starter_address = auto()
    ending_address = auto()
    comment = auto()


token_lookup = {
    'EXENAME': maptoken.exename,
    'SEGMENT': maptoken.segment,
    ';' : maptoken.comment
    }


def main(file_path: str = 'map.txt'):
    # Declare global variables
    output_file_name = 'output.rom'
    
    # Read file
    map_file = open(file_path, 'r')
    input_data = []
    
    for lines in map_file:
        input_data.append([word.strip() for word in lines.split(" ")])

    # Strip comments
    for line_number, lines in enumerate(input_data):
        if token_lookup[lines[0]] == maptoken.comment:
            del input_data[line_number]

    file_size = 0
    
    # Get file size
    for lines in input_data:
        if token_lookup.get(lines[0]) == maptoken.segment:
            ending_address = lines[-1].split('-')[-1]
            if eval(ending_address) > file_size:
                file_size = eval(ending_address)

    # Pad file to desired size
    output_bytes = bytes(file_size)
    muteable_bytes = list(output_bytes)
    
    # Process tokens
    for lines in input_data:
        match token_lookup.get(lines[0]):
            case maptoken.exename:
                output_file_name = ' '.join(lines[1::]).replace('"', '')
            case maptoken.segment:
                segment_name = lines[1].replace('"', '')
                starting_address = eval(lines[-1].split('-')[0])
                ending_address = eval(lines[-1].split('-')[-1])
                segment_data = process_segment(segment_name, starting_address, ending_address)
                muteable_bytes[starting_address:ending_address] = segment_data
            case _:
                pass

    # Output file
    output_bytearray = bytearray(muteable_bytes)
    
    with open(output_file_name, 'wb') as file:
        file.write(output_bytearray)
    

def process_segment(segment_name: str, starting_address: int, ending_address: int):
    # Read segment data
    segment_file = open(segment_name, 'rb')
    segment_data = segment_file.read()

    # Get segment length
    segment_length = len(segment_data)

    # Get target length
    target_length = ending_address - starting_address

    # Ensure target_length is not smaller than segment length
    if target_length < segment_length:
        print(f"Object {segment_name} is too rotund, lay off the fries ...!")    
    
    # If no issue then proceed
    # Pad with zeroes
    padded_segment = segment_data.ljust(target_length, b'\x00')
    
    return padded_segment


if __name__ == "__main__":
    main()
