Basic image segmentation using HOG descriptors. 
The project also includes ranking detected objects using
brightness, sizes, contrast, etc.

Build:
Run 'make' and it will generate 'saliency' binary.

Usage:
./saliency <filename>.ppm

This will generate an output ppm file with bounding
boxes on the detected objects with their respective
ranks.
