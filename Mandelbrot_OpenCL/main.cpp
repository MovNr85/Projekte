/*
Copyright (C) 2023 MovNr85

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. 
*/

#include <fstream>
#include <iostream>
#include <utility>
#include <vexcl/vexcl.hpp>
#include <omp.h>



int
main(int argc, char* argv[])
{

    /* Image dimensions and max number of iterations */
    unsigned int 
      image_width{ 100 }, 
      image_height{ 100 }, 
      max_n{ 1000 };


    if (argc > 1)
    {
      image_width = std::atoi(argv[1]);
      image_height = std::atoi(argv[2]);
    }

    

    /* Constans for the complex plane */
    float 
      min_r{ -2.2 },
      max_r{ 1.2 }, 
      min_i{ -1.7 }, 
      max_i{ 1.7 };



    /* Kernel to compute z = z² + c */



    VEX_FUNCTION(

      cl_uint3,

      mandelbrot,

      (const unsigned, a_element_index)
      (const unsigned, a_image_width)
      (const unsigned, a_image_height)
      (const unsigned, iter)
      (const float, a_r_range_unit)
      (const float, a_i_range_unit)
      (const float, a_min_r)
      (const float, a_min_i),



      int2 cord = { a_element_index % a_image_width,
                    a_element_index / a_image_height };

      double2 c = { cord.x * a_r_range_unit + a_min_r,
                    cord.y * a_i_range_unit + a_min_i };

      double2 z = { 0.0, 0.0 };

      unsigned i = 0;

      for (; (z.x * z.x + z.y * z.y) <= 4 && i < iter; i++) {
          double x_temp = z.x * z.x - z.y * z.y + c.x;

          z.y = 2 * z.x * z.y + c.y;
          z.x = x_temp;
      }

      uint4 r = 
      { 

          ((unsigned)(((sin((float)(i)) + 1) / 2) * 255)), 
          ((unsigned)(((sin((float)(i + 2)) + 1) / 2) * 255)), 
          ((unsigned)(((sin((float)(i + 4)) + 1) / 2) * 255)), 
          0 
      };

      return r;

    );

    /* Creating context on a OpenCL device */
    vex::Context ctx(vex::Filter::DoublePrecision);

    /* Stream for the finel output file */
    std::ofstream ppm_out{ "output.ppm" };

    /* PPM Magic */
    ppm_out << "P3"
            << " ";

    /* Image dimensions */
    ppm_out << image_width << " " << image_height << " ";

    /* Max RGB value */
    ppm_out << 255 << " ";

    std::cout << "List of available OpenCL devices: \n";

    if (!ctx)
        throw std::runtime_error("No devices available.");

    // Print out list of selected devices:
    std::cout << ctx << std::endl;

    vex::vector<cl_uint3> i_cl(ctx, image_width * image_height);

    std::cout << "Computing pixels \n";

    i_cl = mandelbrot(

      vex::element_index(),
      image_width,
      image_height,
      max_n,
      ((max_r - min_r) / image_width),
      ((max_i - min_i) / image_height),
      min_r,
      min_i

    );

    std::vector<cl_uint3> image(

      image_width * image_height

    );

    vex::copy(i_cl, image);

    std::cout << "Writing image to file.\n";


    for (int y = 0; y < image_height; y++) {

        for (int x = 0; x < image_width; x++) {

            ppm_out << image[y * image_width + x].x << " "
                    << image[y * image_width + x].y << " "
                    << image[y * image_width + x].z << " ";
        }
    }

    ppm_out.close();

    std::cout << "Done \n";
}
