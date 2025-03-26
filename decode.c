#include <stdio.h>
#include <string.h>
#include "types.h"
#include "decode.h"

// Function to read and validate decode arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Check if the stego file has a valid extension
    char *src_extn = strchr(argv[2], '.');
    // Check if the extension exists and if it's ".bmp"
    if (src_extn == NULL || strcmp(src_extn, ".bmp") != 0)
    {
        printf("INFO: Validation Error. The file should have a .bmp extension.\n");
        return e_failure;
    }

    // Store the stego file name in the structure
    decInfo->stego_fname = argv[2];

    // Check if an output file name is provided
    if (argv[3] != NULL)
    {
        // Extract extension part from the output file name
        char *out_extn = strchr(argv[3], '.');
        if (out_extn != NULL)
        {
            // Copy the file name before the extension
            strncpy(decInfo->out_fname, argv[3], out_extn - argv[3]);
            decInfo->out_fname[out_extn - argv[3]] = '\0'; // Null-terminate the string
        }
        else
        {
            // Copy the output file name as it is if no extension is provided
            strcpy(decInfo->out_fname, argv[3]);
        }
    }
    else
    {
        // Set a default output file name if none is provided
        decInfo->out_flag = 1;
        strcpy(decInfo->out_fname, "decoded");
    }
    return e_success;
}

/* Function to perform the decoding process */
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: ## Decoding Procedure Started. ##\n");

    // Step 1: Open the stego (input) file
    if (open_stego_file(decInfo) == e_failure)
    {
        printf("Error opening files.\n");
        return e_failure;
    }

    // Step 2: Skip the header of the BMP file
    if (skip_header(decInfo->fptr_stego) == e_failure)
    {
        printf("Error skipping header data.\n");
        return e_failure;
    }

    // Step 3: Decode the magic string
    char magic_string[10];
    printf("Enter the magic string:\n");
    scanf("%s", magic_string); // Get the magic string from the user
    if (decode_magic_string(magic_string, decInfo) == e_failure)
    {
        printf("Magic String not decoded.\n");
        return e_failure;
    }

    // Step 4: Decode the size of the secret file extension
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("Error decoding secret file extension length.\n");
        return e_failure;
    }

    // Step 5: Decode the secret file extension
    if (decode_secret_file_extention(decInfo) == e_failure)
    {
        printf("Error decoding secret file extension.\n");
        return e_failure;
    }

    // Step 6: Open the output file to write the decoded data
    if (open_output_file(decInfo) == e_failure)
    {
        printf("Error opening output file.\n");
        return e_failure;
    }
    printf("INFO: Done. Opened all required files.\n");

    // Step 7: Decode the size of the secret file
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("Error decoding secret file size.\n");
        return e_failure;
    }

    // Step 8: Decode the secret file data
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("Error decoding secret data.\n");
        return e_failure;
    }

    printf("INFO: ## Decoding Done Successfully. ##\n");
    return e_success;
}

/* Function to open the stego file */
Status open_stego_file(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files.\n");
    decInfo->fptr_stego = fopen(decInfo->stego_fname, "r");
    if (decInfo->fptr_stego == NULL)
    {
        return e_failure;
    }
    printf("INFO: Opened %s.\n", decInfo->stego_fname);
    return e_success;
}

/* Function to open the output file */
Status open_output_file(DecodeInfo *decInfo)
{
    decInfo->fptr_output = fopen(decInfo->out_fname, "w");
    if (decInfo->fptr_output == NULL)
    {
        return e_failure;
    }
    if (decInfo->out_flag)
    {
        printf("INFO: Output file not mentioned. Creating 'decoded.txt' as default.\n");
    }
    printf("INFO: Opened %s.\n", decInfo->out_fname);
    return e_success;
}

/* Function to skip the header of the BMP file */
Status skip_header(FILE *fptr)
{
    fseek(fptr, 54, SEEK_SET); // BMP headers are typically 54 bytes
    return e_success;
}

/* Function to decode the magic string */
Status decode_magic_string(const char *user_string, DecodeInfo *decInfo)
{
    printf("INFO: Decoding Magic String Signature.\n");
    char buffer[8], magic[4];

    // Decode the magic string from the file
    for (int i = 0; i < 2; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego);
        magic[i] = decode_lsb_to_byte(buffer); // Extract each byte
    }

    // Compare the user-entered magic string with the decoded string
    if (strcmp(user_string, magic) == 0)
    {
        printf("INFO: Magic String matched.\n");
        return e_success;
    }
    return e_failure;
}

/* Function to decode the size of the secret file extension */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, decInfo->fptr_stego); // Read 32 bits from the stego file
    decInfo->secret_extn_length = decode_lsb_to_size(buffer); // Decode size
    return e_success;
}

/* Function to decode the secret file extension */
Status decode_secret_file_extention(DecodeInfo *decInfo)
{
    char extention[8];
    int i;

    // Decode each character of the extension
    for (i = 0; i < decInfo->secret_extn_length; i++)
    {
        fread(extention, 8, 1, decInfo->fptr_stego);
        decInfo->secret_extn[i] = decode_lsb_to_byte(extention);
    }
    decInfo->secret_extn[i] = '\0'; // Null-terminate the extension
    strcat(decInfo->out_fname, decInfo->secret_extn); // Append extension to the output file name
    return e_success;
}

/* Function to decode the size of the secret file */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding %s File Size.\n", decInfo->out_fname);
    char buffer[32];
    fread(buffer, 32, 1, decInfo->fptr_stego); // Read 32 bits from the stego file
    decInfo->secret_size = decode_lsb_to_size(buffer); // Decode size
    printf("INFO: File size: %u bytes.\n", decInfo->secret_size);
    return e_success;
}

/* Function to decode the secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("INFO: Decoding %s File Data.\n", decInfo->out_fname);
    rewind(decInfo->fptr_output); // Reset output file pointer
    char buffer[8], ch;

    // Decode each byte of the secret data
    for (int i = 0; i < decInfo->secret_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego);
        ch = decode_lsb_to_byte(buffer);
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }
    printf("INFO: Done decoding secret data.\n");
    return e_success;
}

/* Function to decode a byte from LSBs */
char decode_lsb_to_byte(char *image_buffer)
{
    char ch = 0;

    // Decode each bit and reconstruct the byte
    for (int i = 0; i < 8; i++)
    {
        char bit = image_buffer[i] & 0x01; // Extract LSB
        ch = ch | (bit << (7 - i));       // Set the bit in the correct position
    }
    return ch;
}

/* Function to decode a size (32-bit value) from LSBs */
uint decode_lsb_to_size(char *image_buffer)
{
    uint size = 0;

    // Decode each bit and reconstruct the 32-bit integer
    for (int i = 0; i < 32; i++)
    {
        char bit = image_buffer[i] & 0x01; // Extract LSB
        size = size | (bit << (31 - i));   // Set the bit in the correct position
    }
    return size;
}
