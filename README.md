                                                       Title of the project
                                                      ----------------------

                                                       -------------------
                                                       |  LSB Image Steganography |
                                                       -------------------


                                                     Name                 :kota muralidhar reddy
                                                     Batch                :24021E
                                                     Roll No              :24021__006
                                                     Institute            :Emertxe Information Technologies
                                                     Mentor               :Ms.Anusha
                                                     Lab mentor 1         :Mr.poovarasan
                                                     Lab mentor 2         :Mr.Biplap
                                                     Project Guides       :Mr.poovarasan,Mr.Biplap
                                                     Project title        :LSB Image Steganography 
                                                     Type                 :Individual project
                                                     Diffculty            :Intermediate
                                                     Estimated time       :120 hours
                                                     contact              :kota.muralidharreddy@gmail.com  



                                                       What is Steganography?
                                                       ------------------------
Steganography is the art and science of hiding secret information within ordinary, non-secret digital files to prevent detection. The hidden information can only be extracted by the intended recipient, making it a powerful tool for covert communication. Unlike encryption, which scrambles data to make it unreadable without a decryption key, steganography conceals the fact that a message is being sent at all.

Key Features:
-------------
Data Concealment: Hides data within digital files such as images, videos, audio, or text.

Covert Communication: Avoids detection by embedding information in innocuous files.

Integration with Encryption: Can be combined with encryption for added security.

Steganography Meaning: Derived from the Greek words steganos (hidden) and graph (to write).

Objective:
-----------
The objective of this project is to develop a steganography tool that can hide secret information within images
To hide secret messages inside digital images using the Least Significant Bit (LSB) technique.

LSB Steganography Technique:

Concept: Modify the least significant bits of image pixel values to encode secret data.

How It Works:
-------------

Digital images are made up of pixels, each represented by values (e.g., RGB values for color images).
The LSB of a pixel value is changed to encode a bit of the secret data.
Since the LSB change is minimal, it does not noticeably alter the image quality.

Advantages of LSB Technique:
-------------------------------
Simple and easy to implement.
Minimal impact on the carrier file's appearance or functionality.
Works well for images where slight pixel variations are undetectable.

Applications:
-------------
Secure communication in hostile environments.
Digital watermarking to protect copyrights.
Covert data storage for sensitive information


                                                     Why BMP Files are Ideal for LSB Steganography
                                                     ----------------------------------------------

1. Uncompressed Format
BMP files store raw pixel data without applying compression algorithms like JPEG or PNG.
This uncompressed nature ensures that the original data integrity is preserved, making it suitable for embedding secret messages.
Key Advantage: Compression algorithms often alter pixel data, which could distort or destroy the hidden information.

2. Bit-Level Manipulation
BMP files provide direct access to each pixel's bit values (e.g., Red, Green, and Blue channels for RGB images).
This precision enables seamless manipulation of the Least Significant Bits (LSBs) to encode secret data without noticeable changes in the image.

Key Advantage: The straightforward structure of BMP files ensures accurate embedding and retrieval of secret messages.

3. Simplicity and Compatibility
BMP format is relatively simple compared to other formats like JPEG or PNG.
Its straightforward header and pixel data structure make it easy to write and debug steganography algorithms.
BMP files are widely supported across platforms, allowing easy testing and use of steganography programs.

Key Advantage: The simplicity reduces the risk of errors during encoding or decoding.
Example Use Case
Suppose you want to hide a text message inside a BMP file using the LSB technique. Here’s how BMP’s properties help:
The uncompressed format ensures no loss of data integrity during the process.
Bit-level manipulation allows you to embed the message in the pixel’s LSB.
The simple structure makes the implementation straightforward and debugging easier.


                                                    What It Does: Features and Functionality
                                                    ---------------------------------------
Primary Functions
Hide Message

Encodes a secret text message within a BMP image using the Least Significant Bit (LSB) steganography technique.
The image quality remains virtually unchanged to avoid detection.

Extract Message

Decodes and retrieves the hidden message from a modified BMP image.
Ensures the original secret message is intact and readable.

Input/Output
Input:

Accepts BMP images as input for hiding or extracting messages.
Takes a text message for encoding (in "Hide Message" mode).
Output:

Hide Message: Produces a modified BMP image with the encoded text.
Extract Message: Outputs the extracted text from the modified BMP image as a text file.




