# Image-Convolution
Image Convolution is the process of adding each element of the image to its local neighbors, weighted by the kernel.
It is used for blurring, sharpening, embossing, edge detection, and more. This is accomplished by doing a convolution between a kernel and an image.

The program reads a PPM image stored in **input.ppm**, reads a Kernel stores in **kernel**, applies the kernel on the image, writes the resulting image into file output.ppm

### Format of an n x n Kernel
n

scale

n x n matrix

Example:
    [ 1 1 1 ]
    
1/9 [ 1 1 1 ]

    [ 1 1 1 ]
    
is represented in the **kernel** as
3
9
1 1 1
1 1 1
1 1 1

### Program execution
```bash
./filter input.ppm kernel output.ppm
```

Credit:
The house image is downloaded from https://www.deviantart.com/uncledave/art/FARM-HOUSE-BLACK-AND-WHITE-36822107
