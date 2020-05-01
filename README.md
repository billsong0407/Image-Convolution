# Image-Convolution
Image Convolution is the process of adding each element of the image to its local neighbors, weighted by the kernel.
It is used for blurring, sharpening, embossing, edge detection, and more. This is accomplished by doing a convolution between a kernel and an image.

The program reads a PPM image stored in **input.ppm**, reads a Kernel stores in **kernel**, applies the kernel on the image, writes the resulting image into file output.ppm

### Format of an n x n Kernel
n<br />
scale<br />
n x n matrix<br />

Example:<br />

    [ 1 1 1 ]. <br />
1/9 [ 1 1 1 ]. <br />
    [ 1 1 1 ]. <br />
    
is represented in the **kernel** as<br />
3<br />
9<br />
1 1 1<br />
1 1 1<br />
1 1 1<br />

### Program execution
```bash
./filter input.ppm kernel output.ppm
```

Credit:
The house image is downloaded from https://www.deviantart.com/uncledave/art/FARM-HOUSE-BLACK-AND-WHITE-36822107
