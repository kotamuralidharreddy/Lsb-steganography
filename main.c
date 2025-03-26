#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

// Main function
int main(int argc, char *argv[])
{
    // Structures to hold encoding and decoding information
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    // Validate command-line arguments
    if(argc < 2)
    {
        // Print usage instructions if arguments are insufficient
        printf("%s: Encoding: %s -e <.bmp file> <.txt file> [output file]\n", argv[0], argv[0]);
        printf("%s: Decoding: %s -d <.bmp file> [output file]\n", argv[0], argv[0]);
        return e_failure;
    }

    // Determine the type of operation (encode or decode)
    OperationType op_type = check_operation_type(argv[1]);

    // Check if the operation is encoding
    if(op_type == e_encode)
    {
        // Ensure there are enough arguments for encoding
        if(argc < 4)
        {
            printf("%s: Encoding: %s -e <.bmp file> <.txt file> [output file]\n", argv[0], argv[0]);
            return e_failure;
        }

        // Read and validate encoding arguments
        if(read_and_validate_encode_args(argv, &encInfo) == e_failure)
        {
            printf("Error validating arguments for encoding.\n");
            return e_failure;
        }

        // Perform encoding
        if(do_encoding(&encInfo) == e_failure)
        {
            printf("Error during encoding.\n");
            return e_failure;
        }
    }
    // Check if the operation is decoding
    else if(op_type == e_decode)
    {
        // Ensure there are enough arguments for decoding
        if(argc < 3)
        {
            printf("%s: Decoding: %s -d <.bmp file> [output file]\n", argv[0], argv[0]);
            return e_failure;
        }

        // Read and validate decoding arguments
        if(read_and_validate_decode_args(argv, &decInfo) == e_failure)
        {
            printf("Error validating arguments for decoding.\n");
            return e_failure;
        }

        // Perform decoding
        if(do_decoding(&decInfo) == e_failure)
        {
            printf("Error during decoding.\n");
            return e_failure;
        }
    }
    else
    {
        // Handle unsupported operation types
        printf("Error: Unsupported operation type. Process stopped.\n");
        return e_failure;
    }

    return 0;
}

// Function to determine the type of operation (encoding or decoding)
OperationType check_operation_type(char *argv)
{
    // Step 1: Compare argument with "-e" for encoding
    if(!strcmp(argv, "-e"))
    {
        return e_encode;  
    }
    // Step 3: Compare argument with "-d" for decoding
    else if(!strcmp(argv, "-d"))
    {
        return e_decode;
    }
    // Step 5: Return unsupported operation for any other input
    else
    {
        return e_unsupported;
    }
}
