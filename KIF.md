# Kernel Image Format (KIF)

This text-based image format represents images using RGB pixel data in a text file.

## Format Specification

The file format consists of:

1. **Header**: 
    - Contains two integers separated by a space representing the image width and height, respectively.

2. **Pixel Data**:
    - Each line after the header contains RGB values for a single pixel.
    - RGB values are represented as integers in the range 0-255.
    - Each line contains three space-separated integers representing the red, green, and blue values of a pixel.

## Example

Consider the following example of a 3x3 image:
```
3 3
255 0 0
0 255 0
0 0 255
255 255 0
255 0 255
0 255 255
128 128 128
0 0 0
255 255 255
```

This represents a 3x3 image where each row represents the RGB values of pixels in the image.

You can use the `kif.py` tool to generate images from standard JPEG images and etc

## Usage
KIF's are used in the kernel to display simole
images like logos and icons 