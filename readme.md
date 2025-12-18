## Requirements

This project implements convolution kernels image processing as a project for the exam of 
Lab of programming foundations @ UnFI.

### Full statement:

(4) Classe template che rappresenta immagini a 1, 2, 3 e 4 canali. Classe che implementa kernel image processing. (https://en.wikipedia.org/wiki/Kernel_(image_processing) - http://rosettacode.org/wiki/Image_convolution). Lettura e scrittura in formato PPM/PGM. (https://it.wikipedia.org/wiki/Netpbm)

## PPM 
### REFERENCES: 

The **PPM** image format (official specification [here](https://netpbm.sourceforge.net/doc/ppm.html)) supports three channels per image. In fact,
the official specification, in the #Format section, pt 9, it explicitely states that each pixel is stored as a triplet of red, green and blue.

A *ppm* file consists of two components, a header, and a body.

### PPM HEADER
The PPM header consists of three components. Header components can be separated by any number of whitespaces. 
A **whitespace** consists of either:

- A blank space: ' '; 
- A tab: '\t';
- A line feed: '\n';
- A carriage return '\r';
- An arbitrary combination of the previous (no limit in max length, except that max line length should be <= 70 characters).

A header might contain comments. Each comment starts with a # character and it goes on until the end of line is reached.

##### PPM HEADER SECTION 1
PPM is a subset of a wider variety of image formats, all of whom share the same header structure. 
The first section of the header in PPM format consists of two characters, the first one of which is always uppercase 'P' (since the official specification talks in term of character instead of letter). The following character can be either 3 or 6, with the following meanings: 

- P3: PPM (Portable Pixmap), ASCII (color)
- P6: PPM, Binary

##### PPM HEADER SECTION 2
The second section of the header consists of two components, specifying the width and height (in this order) of the image, defined as the number of pixels that compose rows and columns of the image.
Specifically the first component defines the width of the image, that is, the number of horizontal pixels for each 'line' (columns), and the second one defines the height of the image, that is, the number of vertical pixels for each 'column' of the image (rows).

Such numbers are stored as plain text, for example the second header component for a 1920x1080px image is as follows

1920 1080

Where the whitespace between the two components can be replaced by any combination of whitespaces defined above.


##### PPM HEADER SECTION 3
The third section of the header specifies the maximum value for the color of each pixel, that is, the *color depth* of the image. It consists of a single element, representing an integer value stored as plain ASCII characters. For example a simple RGB image with 255 possible values per pixel will have the third header component as follows:

255

The specified value is to be considered as a legit pixel value.
As per specification, this number must be in ]0, 65536[.

### PPM BODY - RASTER
The raster contains the color values for each pixel. The values are stored in row-major order (LAPACK style), that is, pixel values are to be intended left->right and top->bottom.
Adjacent values are stored next to one another (whereas "next to" has a different meaning depending on the first component of the header). Color values for each channel are also stored consecutively.

#### CASE P3
When the first header component is P3, the raster consists of pixel values stored as ASCII characters. In this context, adjacent color values are separated by at least one whitespace (no maximum, except the ideal line length limit) and color values for each channel are stored next to one another. For example:

```
178 130 144 173 125 139 170 122 138 170 122 138 171 123 139 
167 119 135 162 113 132 159 110 129 159 112 130 159 112 130
158 111 131 155 108 128 152 105 125 150 103 123 148 100 122
```
In this raster, 178 is the value for the red (R) channel of the first pixel, 130 is the value for the green (G) channel of the first pixel and 144 is the value for the blue (B) channel of the first pixel. After that, 173 is the value for the red (R) channel of the second pixel, and so on. 


#### CASE P6
When the first header component is P6, the file contains color values stored as bytes. Each channel value is represented using `ceil((floor(log2(maxValue)) + 1) / 8)` bytes. Adjacent values are not separated, they're concatenated with no whitespace inbetween.

Example:

```
01010000001101100000101000100011001000000100100101101101011000010110011101100101001000000110011101100101011011100110010101110010
```

This is to be interpreted as follows:
```
01010000 / 00110110 / 00001010 / 00100011 / 00100000 / 01001001 / 01101101 / 01100001 / 01100111 / 01100101 / 00100000 / 01100111 / 01100101 / 01101110 / 01100101 / 01110010
```

That corresponds to the following:
```
80  54  10  35  32  73 109  97 103 101  32 103 101 110 101 114
```

A text editor would interpret each byte as an ASCII character, therefore will visualize this as follows:

```
P6
# Image gener
```

## PGM

### REFERENCES
**PGM** is a specification for an image format (official source [here](https://netpbm.sourceforge.net/doc/pgm.html)) used to store grayscale images. Images in this format allow one channel for each pixel representing the insensity of grey for such pixel.

#### CASE P2

In this case the raster each pixel value is represented using ASCII, and adjacent pixel values are separated by at least one whitespace. It contains values in the range `]0, maxvalue[`.
The remaining properties mimic PPM with P3. Values represented in the file are to be interpreted as linear grayscale values, where the smallest possible value represents black and the largest possible value represents white as defined by CIE Standard Illuminant D65.

#### CASE P5

In this case the raster is encoded in binary format, that is, the content of the file represents single bytes that need to be reinterpreted as color components. A color component can either be one or two bytes, in which case the leftmost value represents the high part of the value and the rightmost represents the low part of the word (2 bytes).

The encoder knows the pixel values as a luminosity value, L∈[0,1], that is encoded to a gamma-coded value using a transfer function V(L) that specifies a gamma number of 2.2 and has a linear section for small intensities. 

Specifically:

```
if( L < 0.018 ) {
    V(L) = 4.5 * L;
} else {
    V(L) = 1.099 * L ^ 0.45 - 0.099   
}
```

Observe that `V(L):[0,1]→[0,1]`, therefore a denormalization step is required: `Grey = round(maxvalue * V(L))`, where `Grey` is the number that will be output to the file with this extension. Rounding is required since only integer value are supported.


### PAM

PAM image extension is a generalization of PGM and PPM formats, allowing any number of channels per pixel and not strictly specifying the semantics of each pixel.
Official reference can be found [here](https://netpbm.sourceforge.net/doc/pam.html).

#### PAM HEADER

The PAM format header is more structured than that of the previous formats, and consists of multiple tokens defining different traits of the raster. Here is an example of a valid PAM header:

```
P7
WIDTH 227
HEIGHT 149
DEPTH 3
MAXVAL 255
TUPLTYPE RGB
ENDHDR
```

Observe that: 

- The `P7` token is always the first component of the header.
- `ENDHDR` is always the last token of the header.
- `WIDTH`, `HEIGHT`, `DEPTH`, `MAXVAL` all expect a single unsigned integer value >= 1 as subsequent value.
- `TUPLTYPE` can be any alphanumeric value, and is optional. Some standard values are `[BLACKANDWHITE | GRAYSCALE | RGB | BLACKANDWHITE_ALPHA | GRAYSCALE_ALPHA | RGB_ALPHA]`. If this token is used in association with the appropriate `DEPTH` you can get back previously defined netpbm formats.
- Except `P7` and `ENDHDR`, the order of the remaining tokens is arbitrary.
- Token names should be all uppercased for maximum compatibility.
- Usually `TUPLTYPE` hints `DEPTH` but it doesn't determine it, as in, `DEPTH` is the only source that specifies the number of channels of an image. Though certain `TUPLTYPE` values are only valid in association with the matching `DEPTH` value, eg `TUPLTYPE RGB` is expected to be paired with `DEPTH 3`.

#### PAM BODY
The raster is always encoded in binary format. No variant of PAM with ASCII encoded pixel values is officially specified. Adjacent pixel values are concatenated and a pixel value can be composed of one or more bytes. In case of a single channel being encoded over multiple pixels, the leftmost component represents the most significant component of the channel while the rightmost component of the channel represents the less significant byte.

```
IMPORTANT: In binary encoded formats, the header as well is encoded as bytes.
```

## Convolution Kernels

A convolution kernel is a matrix of values, representing a **linear** operator that act as a transformation for an image. Assume that you represent an image as a set of `C` matrices where `C` is the number of channels, and each image is represented by a `M ∈ Matrix(ℝ, height * width)`. A convolution kernel is a square matrix `K ∈ Matrix(ℝ, n * n)` where `n` is an arbitrary unsigned integer greater than or equal to one. Since the context of central pixel is useful, it's common to have convolution kernels of size `n = 2k + 1` for some unsigned integer `k`. Keep in mind that in spite of that, even sized kernels exist and it's not an hard requirement for it to be squared, even though both are preferred.   

The way you typically apply a convolution kernel to an image is that, for each pixel of the original image (if a pixel is stored on multiple channels, repeat the operation on each channel individually), you align the central element of the kernel with it, and multiply kernel values with pixel values, then take the sum of the result. The pixel value for the new image becomes such a sum.

Of course there are edge cases where part of the kernel falls outside of the images boundaries. In such case a few strategies can be used:

- **Skip**: Simply ignore all the kernel values for indices that fall outside of the image boundaries, equivalent to setting the out of bounds pixels to zero. 
- **Reflection:** To preserve continuity, one can reflect the image across boundaries, that is, if the image is `[p1 p2 p3]` it can be reflected to be `[p3 p2 p1 | p1 p2 p3]` where `|` represents a boundary, or even more generally `[p3 p2 p1 | p1 p2 p3 | p3 p2 p1]`.
- **Wrap**: Treat the image as periodic, that is, wrap around borders.
- **Repeat**: Take the nearest pixel value that's within image bounds, that is, clamp the border indices to be `0` when the expression is `< 0` and `size - 1` when the index expression is `>= size`.

# CODING OBJECTIVES

- Consider using a trie to process PAM format header components. 
- Prioritize interfaces, for example parse the header and make it available as a dictionary whose keys are enumeration components representing the tokens, and dictionary values are the values associated with the corresponding value.
- Should be easy to extend, as in add more kernels, supported image formats. 