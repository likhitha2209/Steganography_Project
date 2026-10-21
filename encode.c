#include <stdio.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success 
    return e_success;
}


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /* 
        ->check the argv[3] is a valid file
            is the file present or not
            encInfo->secret_fname=argv[3]
            if present, store in file pointer 
                encInfo->fptr_secret = fopen()
            if not return e_failure

        -> check argv[4] is null or not
            if not NULL,  
                encInfo->stego_image_fname=argv[4]
            if NULL, encInfo->stego_image_fname="default.bmp"

        ->check if the file extensiton if of .bmp 
        if not return e_failure

        open the file in 'w' mode, and store the fptr to th str mem. 
    */
    /*-> check the agrv[2] must be having ".bmp" extn and last 4 char should be ".bmp"
        if yes,store file name into str mem (encInfo->source_image_fname = argv[2])
        if not, return e_failure
    */
    char *dot = strstr(argv[2], ".bmp");
    if(dot == NULL)
    {
        printf("Invalid!! The 2nd argument should consists of '.bmp'\n");
        return e_failure;
    }
    else if(strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") != 0)
    {
        printf("Invalid!! The 2nd argument should end with '.bmp'\n");
        return e_failure;
    }
    else
    {
        encInfo->src_image_fname = argv[2];
    }

    /*-> check if secret file the file is existing or not 
            open file in read mode argv[2]
            encInfo->fptr_src_image=fopen()
            if file not present return null
    */

    encInfo->secret_fname = argv[3];



}