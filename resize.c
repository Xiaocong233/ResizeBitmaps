// Resize a bitmap file by size of an integer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    int n; // predeclare scaling variable
    if (argc == 4) // ensure proper usage
    {
        string userInput = argv[1];
        bool check;
        for (int i = 0; i < strlen(userInput); i++) // check if all characters in the second string input are all numbers
        {
            check = isdigit(userInput[i]);
        }
        if (check == true) // if passed the check
        {
            n = atoi(argv[1]); // assign userinput
        }
        else
        {
            fprintf(stderr, "Usage: ./resize n infile outfile\n"); // gives user correct instruction for the input
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n"); // gives user correct instruction for the input
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    FILE *inptr = fopen(infile, "r"); // open input file
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w"); // open output file
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr); // read infile's BITMAPFILEHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr); // read infile's BITMAPINFOHEADER

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0) // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // adjust metadata according to the scaling intger "n"
    int paddingOG = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // determine the original padding for scanlines
    bi.biWidth *= n; // calculate for new width
    bi.biHeight *= n; // calculate for new height
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // determine the padding after scaling
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight); // calculate for new image size
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // calculate for new file size
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr); // write outfile's BITMAPFILEHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr); // write outfile's BITMAPINFOHEADER

    int repetition = 1; // instantiate independent variable for recording iteration without disrupting variables for loops
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++) // iterate over infile's scanlines
    {
        // iterate over pixels in scanline (row)
        for (int j = 0; j < (bi.biWidth / n); j++)
        {
            RGBTRIPLE triple; // temporary storage

            fread(&triple, sizeof(RGBTRIPLE), 1, inptr); // read RGB triple from infile

            for (int k = 0; k < n; k++) // repeat the writing from RGB triple for each pixel n times
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        fseek(inptr, paddingOG, SEEK_CUR); // skip over padding, if any

        for (int k = 0; k < padding; k++) // then add it back
        {
            fputc(0x00, outptr);
        }

        if ((repetition % n) != 0) //reset the file pointer from the input file in order to repeat writing each row n times
        {
            fseek(inptr, (-1 * (sizeof(RGBTRIPLE) * (bi.biWidth / n) + paddingOG)), SEEK_CUR);
        }
        repetition++;
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
