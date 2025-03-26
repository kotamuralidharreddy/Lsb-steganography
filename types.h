#ifndef TYPES_H
#define TYPES_H

/*
 * This header file defines common user-defined types and enumerations
 * that are used throughout the encoding and decoding processes.
 */

/* 
 * Typedef for unsigned int:
 * - This is used to simplify code readability and ensure consistent usage
 *   of unsigned integers in the program.
 */
typedef unsigned int uint;

/* 
 * Enumeration: Status
 * Purpose: Represents the return type for functions to indicate success or failure.
 * Values:
 * - `e_success`: Indicates that the function executed successfully.
 * - `e_failure`: Indicates that the function encountered an error or failed.
 */
typedef enum
{
    e_success,  // Function executed successfully
    e_failure   // Function encountered a failure
} Status;

/* 
 * Enumeration: OperationType
 * Purpose: Represents the type of operation the program will perform.
 * Values:
 * - `e_encode`: Indicates that the program will perform encoding.
 * - `e_decode`: Indicates that the program will perform decoding.
 * - `e_unsupported`: Indicates an invalid or unsupported operation type.
 */
typedef enum
{
    e_encode,       // Operation type for encoding
    e_decode,       // Operation type for decoding
    e_unsupported   // Unsupported or invalid operation
} OperationType;

#endif // End of TYPES_H
