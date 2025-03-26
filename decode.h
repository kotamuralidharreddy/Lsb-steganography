#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user-defined types like Status

/*
 * This header file defines the structures and function prototypes
 * required for decoding secret information from a stego image file.
 */

/* 
 * Structure: DecodeInfo
 * Purpose: To store all necessary information for decoding data from
 * a stego image file and writing the decoded data to an output file.
 */
typedef struct DecodeInfo
{
    /* Stego image file information */
    char *stego_fname;          // Pointer to the name of the stego image file (input file)
    FILE *fptr_stego;           // File pointer for the stego image file

    /* Output file information */
    char out_fname[20];         // Name of the output file where decoded data will be saved
    int out_flag;               // Flag to indicate whether the user provided an output file name (1 = default used)

    FILE *fptr_output;          // File pointer for the output file

    /* Secret file information */
    uint secret_extn_length;    // Length of the secret file's extension (e.g., ".txt")
    char secret_extn[10];       // Buffer to store the secret file extension
    uint secret_size;           // Size of the secret file in bytes

} DecodeInfo; // End of DecodeInfo structure definition

/* 
 * Constants to define buffer sizes for decoding data 
 */
#define MAX_SECRET_BUF_SIZE 1          // Maximum buffer size for storing secret data (1 byte)
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) // Buffer size for storing 8 bits per byte for LSB decoding
#define MAX_FILE_SUFFIX 10             // Maximum length for file extensions

/* 
 * Function Prototypes:
 * These prototypes define the functions used in the decoding process.
 */

/* 
 * Function: read_and_validate_decode_args
 * Purpose: Validates the command-line arguments passed for decoding.
 * Inputs:
 *  - argv: Command-line arguments array.
 *  - decInfo: Pointer to DecodeInfo structure to store parsed arguments.
 * Outputs:
 *  - Returns e_success if validation is successful, otherwise e_failure.
 */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* 
 * Function: do_decoding
 * Purpose: Manages the entire decoding process.
 * Inputs:
 *  - decInfo: Pointer to DecodeInfo structure containing necessary details.
 * Outputs:
 *  - Returns e_success if decoding is completed successfully, otherwise e_failure.
 */
Status do_decoding(DecodeInfo *decInfo);

/* 
 * Function: open_stego_file
 * Purpose: Opens the stego image file for reading.
 * Inputs:
 *  - decInfo: Pointer to DecodeInfo structure containing file details.
 * Outputs:
 *  - Returns e_success if the file is successfully opened, otherwise e_failure.
 */
Status open_stego_file(DecodeInfo *decInfo);

/* 
 * Function: open_output_file
 * Purpose: Opens the output file for writing decoded data.
 * Inputs:
 *  - decInfo: Pointer to DecodeInfo structure containing output file details.
 * Outputs:
 *  - Returns e_success if the file is successfully opened, otherwise e_failure.
 */
Status open_output_file(DecodeInfo *decInfo);

/* 
 * Function: skip_header
 * Purpose: Skips the header data in the BMP file to reach encoded data.
 * Inputs:
 *  - fptr: File pointer to the stego image file.
 * Outputs:
 *  - Returns e_success if the header is successfully skipped.
 */
Status skip_header(FILE *fptr);

/* 
 * Function: decode_magic_string
 * Purpose: Decodes the magic string from the stego image and validates it.
 * Inputs:
 *  - user_string: Magic string provided by the user for verification.
 *  - decInfo: Pointer to DecodeInfo structure containing file details.
 * Outputs:
 *  - Returns e_success if the magic string matches, otherwise e_failure.
 */
Status decode_magic_string(const char *user_string, DecodeInfo *decInfo);

/* 
 * Function: decode_secret_file_extn_size
 * Purpose: Decodes the size of the secret file's extension.
 * Inputs:
 *  - decInfo: Pointer to DecodeInfo structure to store decoded size.
 * Outputs:
 *  - Returns e_success if the size is successfully decoded, otherwise e_failure.
 */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* 
 * Function: decode_secret_file_extention
 * Purpose: Decodes the extension of the secret file from the stego image.
 * Inputs:
 *  - decInfo: Pointer to DecodeInfo structure to store decoded extension.
 * Outputs:
 *  - Returns e_success if the extension is successfully decoded, otherwise e_failure.
 */
Status decode_secret_file_extention(DecodeInfo *decInfo);

/* 
 * Function: decode_secret_file_size
 * Purpose: Decodes the size of the secret file embedded in the stego image.
 * Inputs:
 *  - decInfo: Pointer to DecodeInfo structure to store decoded file size.
 * Outputs:
 *  - Returns e_success if the size is successfully decoded, otherwise e_failure.
 */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* 
 * Function: decode_secret_file_data
 * Purpose: Decodes the actual data of the secret file from the stego image.
 * Inputs:
 *  - decInfo: Pointer to DecodeInfo structure to manage file pointers and buffers.
 * Outputs:
 *  - Returns e_success if the data is successfully decoded, otherwise e_failure.
 */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* 
 * Function: decode_lsb_to_byte
 * Purpose: Decodes a single byte of data using LSB (Least Significant Bit) method.
 * Inputs:
 *  - image_buffer: Buffer containing 8 bits of data from the stego image.
 * Outputs:
 *  - Returns the decoded byte.
 */
char decode_lsb_to_byte(char *image_buffer);

/* 
 * Function: decode_lsb_to_size
 * Purpose: Decodes a 32-bit integer (size) using LSB method.
 * Inputs:
 *  - image_buffer: Buffer containing 32 bits of data from the stego image.
 * Outputs:
 *  - Returns the decoded size as an unsigned integer.
 */
uint decode_lsb_to_size(char *image_buffer);

#endif // End of DECODE_H
