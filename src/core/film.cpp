#include "film.h"
#include <cmath>

#include <fstream>
#include <string>
#include <iostream>

#include "api.h"
#include "image_io.h"
#include "paramset.h"
#include "commons.h"

namespace rt3
{

  //=== Film Method Definitions
  Film::Film(const Point2i &resolution, const std::string &filename, image_type_e imgt, std::vector<unsigned char> &imgdt)
      : m_full_resolution{resolution}, m_filename{filename}, m_image_type{imgt}, m_image_data{std::move(imgdt)}
  {
    // TODO
  }

  Film::~Film()
  {
  }

  /// Add the color to image.
  void Film::add_sample(const Point2i &pixel_coord, const RGBAColor &pixel_color)
  {
    auto offset = (pixel_coord[1]*m_full_resolution[0]*4) + (pixel_coord[0]*4);
    m_image_data[offset] = pixel_color.r;
    m_image_data[offset + 1] = pixel_color.g;
    m_image_data[offset + 2] = pixel_color.b;
    m_image_data[offset + 3] = 255;
  }

  /// Convert image to RGB, compute final pixel values, write image.
  void Film::write_image(void) const
  {
    // TODO: call the proper writing function, either PPM or PNG.
    if (m_image_type == image_type_e::PNG)
    {
      save_png((unsigned char*) m_image_data.data(), m_full_resolution[0], m_full_resolution[1], 4, m_filename);
    }
    else if (m_image_type == image_type_e::PPM3)
    {
      std::vector <unsigned char> out(m_full_resolution[0] * m_full_resolution[1] * 3);

      size_t idxPpm = 0;
      for(size_t i = 0; i < m_full_resolution[0] * m_full_resolution[1] * 4; i+=4) {
        out[idxPpm] = m_image_data[i];
        out[idxPpm+1] = m_image_data[i+1];
        out[idxPpm+2] = m_image_data[i+2];
        idxPpm+=3;
      }
      
      save_ppm3(out.data(),m_full_resolution[0], m_full_resolution[1], 3, m_filename);
    }
  }

  // Factory function pattern.
  // This is the function that retrieves from the ParamSet object
  // all the information we need to create a Film object.
  Film *create_film(const ParamSet &ps)
  {
    std::cout << ">>> Inside create_film()\n";
    std::string filename;
    // Let us check whether user has provided an output file name via
    // command line arguments in main().
    if (API::curr_run_opt.outfile != "")
    {
      // Retrieve filename defined via CLI.
      filename = API::curr_run_opt.outfile;
      // Retrieve another filename, now from the ParamSet object.
      std::string filename_from_file = retrieve(ps, "filename", std::string{""});
      if (filename_from_file != "") // We also get a filename from scene file...
        RT3_WARNING(string{"Output filename supplied on command line, \""} + API::curr_run_opt.outfile + string{"\" is overriding filename provided in scene "
                                                                                                                "description file, \""} +
                    filename_from_file);
    }
    else
    {
      // Try yo retrieve filename from scene file.
      filename = retrieve(ps, "filename", std::string{"image.png"});
    }

    // Read resolution.
    // Aux function that retrieves info from the ParamSet.
    int xres = retrieve(ps, "x_res", int(1280));
    // Aux function that retrieves info from the ParamSet.
    int yres = retrieve(ps, "y_res", int(720));
    // Quick render?
    if (API::curr_run_opt.quick_render == true)
    {
      // decrease resolution.
      xres = std::max(1, xres / 4);
      yres = std::max(1, yres / 4);
    }

    // TODO
    // Read crop window information.
    std::vector<real_type> cw = retrieve(ps, "crop_window", std::vector<real_type>{0, 1, 0, 1});
    std::cout << "Crop window ";
    for (const auto &e : cw)
      std::cout << e << " ";
    std::cout << '\n';

    //Creating and allocating a image space already filled with 0's
    std::vector<unsigned char> data(xres * yres * 4);

    //Obtaining String_type
    std::string sTyp = retrieve(ps, "img_type", std::string("png"));
    std::transform(sTyp.begin(), sTyp.end(), sTyp.begin(), ::tolower);
    Film::image_type_e typ = Film::image_type_e::PNG;
    if(sTyp == "ppm3") {
      typ = Film::image_type_e::PPM3;
    }

    // Note that the image type is fixed here. Must be read from ParamSet, though.
    return new Film(Point2i{xres, yres}, filename, typ, data);
  }
} // namespace rt3
