# Problem definition
What was required of me in this project is to read the 3D point data separately, from all the files in a folder and store the information in the memory. The files are saved as .nkt extension. The 3D point data is stored in the point data file in the format specified below.

The first four lines of the file shows the descriptions.

If AREAS is specified as x y z #, it is assumed that the information of the points is given in 3 dimensions. If AREAS are specified as x y z r g b #, it is assumed that the points are given with the red, green, blue codes as well as the 3-dimensional information of the points.Points are stored in float format and color information is stored in integer format.

POINTS, indicates the total number of points found in the file.

In the DATA, it is stated whether the coordinate information of the points in the file is stored in ASCII or BINARY format.

# Requirements

We need to perform a check during or after reading from the file. If the header information of the file does not comply with the given format, it should not be read from the file and an error message should be given.

After reading from the file, if the number of points is not the same as the number specified with "POINTS" in the header, an error message should be printed. If points are not given in “FIELDS”, in required formats, an error message should be given.

The following issues should be calculated separately for the points read from each file:
- Information and point numbers of the closest and farthest points to each other
- Edge point information of a cube that will include all points
- After taking 3D coordinates of center and radius information from user, information of the points within the sphere and point numbers
- Average of the distances of each point from each other
- The results of all requests must be saved in the "output.nkt" file and this file needs to be in required format.

# Ease of Use 

- There is no need to enter any name when the program is run.
The files in the program folder will be read automatically.

- The user is expected to enter a value between 1-5.

- Selection 1 must be made at least once for the program to work properly.
Otherwise, the program will warn and ask the user to choose again.

- If the user enters a value other than 1-5, the program is exited.

