#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

// Function to calculate the size of the image for a BMP file
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to the 18th byte, which contains the width of the image
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity (width * height * 3 for RGB)
    return width * height * 3;
}

// Function to get the size of a file
uint get_file_size(FILE *fptr)
{
    // Seek to the end of the file
    fseek(fptr, 0, SEEK_END);
    // Return the current position (size of the file)
    return ftell(fptr);
}

// Function to open required files for encoding
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: Opening required files.\n");
    
    // Open the source image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Error handling for source image file
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: can't open file %s\n", encInfo->src_image_fname);
        return e_failure;
    }
    
    // Open the secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Error handling for secret file
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: can't open file %s\n", encInfo->secret_fname);
        return e_failure;
    }

    // Open the stego image file for writing
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Error handling for stego image file
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: can't open file %s\n", encInfo->stego_image_fname);
        return e_failure;
    }
    
    // Print confirmation messages
    printf("INFO: Opened beautiful.bmp\n");
    printf("INFO: Opened secret.txt\n");
    printf("INFO: Opened %s\n", encInfo->stego_image_fname);
    printf("INFO: Done\n");
    
    // No failure return e_success
    return e_success;
}

// Function to read and validate encoding arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Temporary arrays to store file extensions
    char bmp[10], txt[10], out[10];
    
    // Extract the file extension from the source BMP file (argv[2])
    strcpy(bmp, strchr(argv[2], '.'));
    
    // Extract the file extension from the secret TXT file (argv[3])
    strcpy(txt, strchr(argv[3], '.'));
    
    // Check if the output file name is provided (argv[4])
    if (argv[4] == NULL)
    {
        // If not provided, set a default output file name
        printf("INFO: Output file not mentioned. Creating stegno_image.bmp as default.\n");
        encInfo->stego_image_fname = "stegno_image.bmp";
    }
    else
    {
        // Extract the file extension from the output file (argv[4])
        strcpy(out, strchr(argv[4], '.'));
        
        // Verify if the output file has a .bmp extension
        if (strcmp(out, ".bmp"))
        {
            // If not .bmp, return failure
            return e_failure;
        }
        
        // If valid, set the output file name in encInfo structure
        encInfo->stego_image_fname = argv[4];
    }

    // Verify if the source image file has a .bmp extension
    if (strcmp(bmp, ".bmp"))
    {
        // If not .bmp, return failure
        return e_failure;
    }

    // Verify if the secret file has a .txt extension
    if (strcmp(txt, ".txt"))
    {
        // If not .txt, return failure
        return e_failure;
    }

    // Store the secret file extension in encInfo
    strcpy(encInfo->extn_secret_file, txt);
    
    // Store the source image file name and secret file name in encInfo
    encInfo->src_image_fname = argv[2];
    encInfo->secret_fname = argv[3];
    
    // If all validations pass, return success
    return e_success;
}

// Function to perform the encoding process
Status do_encoding(EncodeInfo *encInfo)
{
    // Open source image and output stego image files
    if (open_files(encInfo) == e_failure)
    {
        printf("INFO: Files are not opened.\n");
        return e_failure;
    }

    printf("INFO: ## Encoding Procedure Started. ##\n");

    // Check if there is enough capacity in the source image to store the secret data
    if (check_capacity(encInfo) == e_failure)
    {
        printf("INFO: There is not enough space.\n");
        return e_failure;
    }

    // Copy the BMP header from source image to the stego image
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("INFO: Header not copied.\n");
        return e_failure;
    }

    // Encode a predefined "magic string" to help identify the presence of encoded data
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("INFO: Error encoding magic string.\n");
        return e_failure;
    }

    // Encode the size of the secret file extension for later retrieval
    if (encode_extention_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("INFO: Error encoding extension size.\n");
        return e_failure;
    }

    // Encode the secret file's extension to ensure correct file type during decoding
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("INFO: Error encoding secret file extension.\n");
        return e_failure;
    }

    // Encode the size of the secret file to facilitate data extraction
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("INFO: Error encoding secret file size.\n");
        return e_failure;
    }

    // Encode the actual data of the secret file into the stego image
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("INFO: Error encoding secret data.\n");
        return e_failure;
    }

    // Copy any remaining image data from the source image to the stego image
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("INFO: Error copying the remaining data to output image.\n");
        return e_failure;
    }

    printf("INFO: ## Encoding Done successfully. ##\n");

    return e_success;
}

// Function to check if the source image has enough capacity to store the secret data
Status check_capacity(EncodeInfo *encInfo)
{
    // Get Secret file size
    printf("INFO: Checking for %s size\n", encInfo->secret_fname);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if (encInfo->size_secret_file != 0)
    {
        printf("INFO: Done. Not Empty\n");
    }
    
    // Get Image size
    printf("INFO: Checking for %s capacity to handle %s\n", encInfo->src_image_fname, encInfo->secret_fname);
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    
    // Get Magic string length
    uint magic_string_length = strlen(MAGIC_STRING);
    
    // Calculate Required file size
    uint req_size = 54 + ((magic_string_length + 4 + strlen(encInfo->extn_secret_file) + 4 + encInfo->size_secret_file) * 8);
    
    // Check if the image capacity is sufficient
    if (encInfo->image_capacity < req_size)
    {
        return e_failure;
    }
    else 
    {
        printf("INFO: Done. Found OK\n");
        return e_success;
    }
}

// Function to copy the BMP header from the source image to the stego image
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    printf("INFO: Copying Image Header.\n");
    rewind(fptr_src_image);

    char temp[54];
    // Read the BMP header (first 54 bytes)
    fread(temp, 54, 1, fptr_src_image);
    // Write the BMP header to the stego image
    fwrite(temp, 54, 1, fptr_stego_image);
    printf("INFO : Done\n");
    return e_success;
}

// Function to encode a magic string into the stego image
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char magic[4];
    // Copy the magic string into a temporary buffer
    strcpy(magic, magic_string);
    printf("INFO: Encoding Magic string \n");
    // Encode the magic string into the stego image
    encode_data_to_image(magic, encInfo->fptr_src_image, encInfo->fptr_stego_image);   
    printf("INFO: Done\n");
    return e_success;
}

// Function to encode the size of the secret file extension
Status encode_extention_size(char size, EncodeInfo *encInfo)
{
    char buffer[32];
    // Read 32 bytes from the source image
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    // Encode the size into the least significant bits of the buffer
    encode_int_to_lsb(buffer, size);
    // Write the modified buffer to the stego image
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

// Function to encode the secret file extension into the stego image
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    printf("INFO: Encoding %s Extention.\n", encInfo->secret_fname);
    char image_buffer[8];
    // Copy the file extension into a temporary buffer
    strcpy(image_buffer, file_extn);
    // Encode the file extension into the stego image
    encode_data_to_image(image_buffer, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("INFO: Done\n");
    return e_success;
}

// Function to encode the size of the secret file
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding %s file size.\n", encInfo->secret_fname);
    char buffer[32];
    // Read 32 bytes from the source image
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    // Encode the file size into the least significant bits of the buffer
    encode_int_to_lsb(buffer, file_size);
    // Write the modified buffer to the stego image
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);
    printf("INFO: Done\n");
    return e_success;
}

// Function to encode the actual data of the secret file into the stego image
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding %s file data.\n", encInfo->secret_fname);
    rewind(encInfo->fptr_secret);
    char ch;
    char buffer[8];
    // Read each character from the secret file and encode it
    while ((ch = getc(encInfo->fptr_secret)) != EOF)
    {
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        // Encode the character into the buffer
        encode_byte_to_lsb(ch, buffer);
        // Write the modified buffer to the stego image
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    printf("INFO: Done.\n");
    return e_success;
}

// Function to copy any remaining image data from the source image to the stego image
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    printf("INFO: Copying Left over Data.\n");
    int c;
    // Read each byte from the source image and write it to the stego image
    while ((c = fgetc(fptr_src)) != EOF)
    {
        fputc(c, fptr_dest);
    }
    printf("INFO: Done.\n");
    return e_success;
}

// Function to encode data into the stego image
Status encode_data_to_image(char *data, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    // Read 8 bytes of data from the source image and encode each character of the data
    for (int i = 0; i < strlen(data); i++)
    {
        fread(buffer, 1, 8, fptr_src_image);
        // Encode the character into the buffer
        encode_byte_to_lsb(data[i], buffer);
        // Write the modified buffer to the stego image
        fwrite(buffer, 1, 8, fptr_stego_image);
    }
    return e_success;
}

// Function to encode a single byte into the least significant bits of the image buffer
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        // Clear the least significant bit of the current byte in the image buffer
        image_buffer[i] = (image_buffer[i] & 0xFE);
        // Extract one bit from the data and set it in the least significant bit of the image buffer
        char c = (unsigned)(data & (1 << (7 - i))) >> (7 - i);
        image_buffer[i] = (image_buffer[i] | c); // Replace the bit
    }
    return e_success;
}

// Function to encode an integer into the least significant bits of the image buffer
Status encode_int_to_lsb(char *image_buffer, int data)
{
    for (int i = 0; i < 32; i++)
    {
        // Clear the least significant bit of the current byte in the image buffer
        image_buffer[i] = (image_buffer[i] & 0xFE);
        // Extract each bit of the integer data and store it in the least significant bit of the image buffer
        image_buffer[i] = (image_buffer[i] | ((unsigned)(data & (1 << (31 - i))) >> (31 - i)));
    } // Repeat the process for 32 bits
    return e_success;
}