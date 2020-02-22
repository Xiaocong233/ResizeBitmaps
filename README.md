## Description
This is my program created for pset3 cs50. As described, it will simply resize a bitmap image file by an integer factor; it takes 3 inputs from the user: "n"(integer factor), "infile", "outfile". Unfortunately, it is limited to resizing by an integer, it can not take a float input nor resizing an image to be smaller. I will update this program later to be able to handle those scenarios, as required by "more".

## Usage
```python
$ make resize
$ ./resize n infile outfile # n is the multiple factor you want to expand infile by, it can be all positive integer
```

## What I have learned
* Applying pointer variables
* Access and manipulation of files (read and write) in C
* Access and manipulation of metadata in bitmap files
* Pixel color manipulation via RGBTRIPLE
