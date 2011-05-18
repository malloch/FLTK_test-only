//
// "$Id: Device.h 8529 2011-03-23 12:49:30Z AlbrechtS $"
//
// Definition of classes Fl_Device, Fl_Graphics_Driver, Fl_Surface_Device, Fl_Display_Device
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2011 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
/** \file Device.h 
 \brief declaration of classes Fl_Device, Fl_Graphics_Driver, Fl_Surface_Device, 
 Fl_Display_Device, Fl_Device_Plugin.
*/

#ifndef Fltk3_Device_H
#define Fltk3_Device_H

#include <fltk3/x.h>
#include <fltk3/Plugin.h>
#include <fltk3/Image.h>
#include <fltk3/Bitmap.h>
#include <fltk3/Pixmap.h>
#include <fltk3/RGBImage.h>

class Fl_Graphics_Driver;
class Fl_Font_Descriptor;
/** \brief Points to the driver that currently receives all graphics requests */
FLTK3_EXPORT extern Fl_Graphics_Driver *fl_graphics_driver;

/**
 signature of image generation callback function.
 \param[in]  data  user data passed to function
 \param[in]  x,y,w position and width of scan line in image
 \param[out] buf   buffer for generated image data. You must copy \p w
 pixels from scanline \p y, starting at pixel \p x
 to this buffer.
 */
typedef void (*Fl_Draw_Image_Cb)(void* data,int x,int y,int w,uchar* buf);

// typedef what the x,y fields in a point are:
#ifdef WIN32
typedef int COORD_T;
#  define XPOINT XPoint
#elif defined(__APPLE__)
typedef float COORD_T;
typedef struct { float x; float y; } QPoint;
#  define XPOINT QPoint
extern float fl_quartz_line_width_;
#else
typedef short COORD_T;
#  define XPOINT XPoint
#endif

/**
 \brief All graphical output devices and all graphics systems.
 */
class FLTK3_EXPORT Fl_Device {
public:
  /** A string that identifies each subclass of Fl_Device.
     Function class_name() applied to a device of this class returns this string.
   */
  static const char *class_id;
  /** 
   Returns the name of the class of this object. 
   The class of an instance of an Fl_Device subclass can be checked with code such as:
   \code
   if ( instance->class_name() == Fl_Printer::class_id ) { ... }
   \endcode
   */
  virtual const char *class_name() {return class_id;};
  /** 
   Virtual destructor.
   
   The destructor of Fl_Device must be virtual to make the destructors of
   derived classes being called correctly on destruction.
   */
  virtual ~Fl_Device() {};
};

#define FL_REGION_STACK_SIZE 10
#define FL_MATRIX_STACK_SIZE 32
/**
 \brief A virtual class subclassed for each graphics driver FLTK uses.
 *
 The virtual methods of this class are those that a graphics driver should implement to
 support all of FLTK drawing functions.
 <br> The public API for drawing operations is functionally presented in \ref drawing and as function lists
 in the \ref fl_drawings and \ref fl_attributes modules. 
  */
class FLTK3_EXPORT Fl_Graphics_Driver : public Fl_Device {
public:
  /** A 2D coordinate transformation matrix
   */
  struct matrix {double a, b, c, d, x, y;};
private:
  static const matrix m0;
  fltk3::Font font_; // current font
  fltk3::Fontsize size_; // current font size
  fltk3::Color color_; // current color
  enum {LINE, LOOP, POLYGON, POINT_};
  int sptr;
  static const int matrix_stack_size = FL_MATRIX_STACK_SIZE;
  matrix stack[FL_MATRIX_STACK_SIZE];
  matrix m;
  int n, p_size, gap_;
  XPOINT *p;
  int what;
  int fl_clip_state_number;
  int rstackptr;
  static const int region_stack_max = FL_REGION_STACK_SIZE - 1;
  Fl_Region rstack[FL_REGION_STACK_SIZE];
#ifdef WIN32
  int numcount;
  int counts[20];
#endif
  Fl_Font_Descriptor *font_descriptor_;
  void transformed_vertex0(COORD_T x, COORD_T y);
  void fixloop();
  
public:

  matrix *fl_matrix; /**< Points to the current coordinate transformation matrix */

  /** \brief The constructor. */
  Fl_Graphics_Driver();
  /** \brief see fltk3::rect(int x, int y, int w, int h). */
  virtual void rect(int x, int y, int w, int h);
  /** \brief see fltk3::rectf(int x, int y, int w, int h). */
  virtual void rectf(int x, int y, int w, int h);
  /** \brief see fltk3::line_style(int style, int width, char* dashes). */
  virtual void line_style(int style, int width=0, char* dashes=0);
  /** \brief see fltk3::xyline(int x, int y, int x1). */
  virtual void xyline(int x, int y, int x1);
  /** \brief see fltk3::xyline(int x, int y, int x1, int y2). */
  virtual void xyline(int x, int y, int x1, int y2);
  /** \brief see fltk3::xyline(int x, int y, int x1, int y2, int x3). */
  virtual void xyline(int x, int y, int x1, int y2, int x3);
  /** \brief see fltk3::yxline(int x, int y, int y1). */
  virtual void yxline(int x, int y, int y1);
  /** \brief see fltk3::yxline(int x, int y, int y1, int x2). */
  virtual void yxline(int x, int y, int y1, int x2);
  /** \brief see fltk3::yxline(int x, int y, int y1, int x2, int y3). */
  virtual void yxline(int x, int y, int y1, int x2, int y3);
  /** \brief see fltk3::line(int x, int y, int x1, int y1). */
  virtual void line(int x, int y, int x1, int y1);
  /** \brief see fltk3::line(int x, int y, int x1, int y1, int x2, int y2). */
  virtual void line(int x, int y, int x1, int y1, int x2, int y2);
  /** \brief see fltk3::draw(const char *str, int n, int x, int y). */
  virtual void draw(const char *str, int n, int x, int y) = 0;
#ifdef __APPLE__
  virtual void draw(const char *str, int n, float x, float y) = 0;
#endif
  /** \brief see fltk3::draw(int angle, const char *str, int n, int x, int y). */
  virtual void draw(int angle, const char *str, int n, int x, int y) = 0;
  /** \brief see fltk3::rtl_draw(const char *str, int n, int x, int y). */
  virtual void rtl_draw(const char *str, int n, int x, int y) = 0;
  /** \brief see fltk3::color(fltk3::Color c). */
  virtual void color(fltk3::Color c) {color_ = c;}
  /** \brief see fltk3::color(uchar r, uchar g, uchar b). */
  virtual void color(uchar r, uchar g, uchar b) = 0;
  /** \brief see fltk3::point(int x, int y). */
  virtual void point(int x, int y);
  /** \brief see fltk3::loop(int x0, int y0, int x1, int y1, int x2, int y2). */
  virtual void loop(int x0, int y0, int x1, int y1, int x2, int y2);
  /** \brief see fltk3::loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3). */
  virtual void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
  /** \brief see fltk3::polygon(int x0, int y0, int x1, int y1, int x2, int y2). */
  virtual void polygon(int x0, int y0, int x1, int y1, int x2, int y2);
  /** \brief see fltk3::polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3). */
  virtual void polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
  /** \brief see fltk3::begin_points(). */
  virtual void begin_points();
  /** \brief see fltk3::begin_line(). */
  virtual void begin_line();
  /** \brief see fltk3::begin_loop(). */
  virtual void begin_loop();
  /** \brief see fltk3::begin_polygon(). */
  virtual void begin_polygon();
  /** \brief see fltk3::vertex(double x, double y). */
  virtual void vertex(double x, double y);
  /** \brief see fltk3::curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3). */
  virtual void curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3);
  /** \brief see fltk3::circle(double x, double y, double r). */
  virtual void circle(double x, double y, double r);
  /** \brief see fltk3::arc(double x, double y, double r, double start, double end). */
  virtual void arc(double x, double y, double r, double start, double end);
  /** \brief see fltk3::arc(int x, int y, int w, int h, double a1, double a2). */
  virtual void arc(int x, int y, int w, int h, double a1, double a2);
  /** \brief see fltk3::pie(int x, int y, int w, int h, double a1, double a2). */
  virtual void pie(int x, int y, int w, int h, double a1, double a2);
  /** \brief see fltk3::end_points(). */
  virtual void end_points();
  /** \brief see fltk3::end_line(). */
  virtual void end_line();
  /** \brief see fltk3::end_loop(). */
  virtual void end_loop();
  /** \brief see fltk3::end_polygon(). */
  virtual void end_polygon();
  /** \brief see fltk3::begin_complex_polygon(). */
  virtual void begin_complex_polygon();
  /** \brief see fltk3::gap(). */
  virtual void gap();
  /** \brief see fltk3::end_complex_polygon(). */
  virtual void end_complex_polygon();
  /** \brief see fltk3::transformed_vertex(double xf, double yf). */
  virtual void transformed_vertex(double xf, double yf);
  /** \brief see fltk3::push_clip(int x, int y, int w, int h). */
  virtual void push_clip(int x, int y, int w, int h);
  /** \brief see fltk3::clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H). */
  virtual int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);
  /** \brief see fltk3::not_clipped(int x, int y, int w, int h). */
  virtual int not_clipped(int x, int y, int w, int h);
  /** \brief see fltk3::push_no_clip(). */
  virtual void push_no_clip();
  /** \brief see fltk3::pop_clip(). */
  virtual void pop_clip();
    
  /** \brief see fltk3::push_matrix(). */
  void push_matrix();
  /** \brief see fltk3::pop_matrix(). */
  void pop_matrix();
  /** \brief see fltk3::mult_matrix(double a, double b, double c, double d, double x, double y). */
  void mult_matrix(double a, double b, double c, double d, double x, double y);
  /** \brief see fltk3::scale(double x, double y). */
  inline void scale(double x, double y) { mult_matrix(x,0,0,y,0,0); }
  /** \brief see fltk3::scale(double x). */
  inline void scale(double x) { mult_matrix(x,0,0,x,0,0); }
  /** \brief see fltk3::translate(double x, double y). */
  inline void translate(double x,double y) { mult_matrix(1,0,0,1,x,y); }
  /** \brief see fltk3::rotate(double d). */
  void rotate(double d);
  /** \brief see fltk3::transform_x(double x, double y). */
  double transform_x(double x, double y);
  /** \brief see fltk3::transform_y(double x, double y). */
  double transform_y(double x, double y);
  /** \brief see fltk3::transform_dx(double x, double y). */
  double transform_dx(double x, double y);
  /** \brief see fltk3::transform_dy(double x, double y). */
  double transform_dy(double x, double y);
  /** \brief see fltk3::clip_region(). */
  Fl_Region clip_region();
  /** \brief see fltk3::clip_region(Fl_Region r). */
  void clip_region(Fl_Region r);
  /** \brief see fltk3::restore_clip(). */
  void restore_clip();
  
  // Images
  /** \brief see fltk3::draw_image(const uchar* buf, int X,int Y,int W,int H, int D, int L). */
  virtual   void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0) = 0;
  /** \brief see fltk3::draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D, int L). */
  virtual   void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0) = 0;
  /** \brief see fltk3::draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D). */
  virtual   void draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=3) = 0;
  /** \brief see fltk3::draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D). */
  virtual   void draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=1) = 0;
  // Image classes
  /** \brief Draws an Fl_RGB_Image object to the device. 
   *
   Specifies a bounding box for the image, with the origin (upper left-hand corner) of 
   the image offset by the cx and cy arguments.
   */
  virtual   void draw(Fl_RGB_Image * rgb,int XP, int YP, int WP, int HP, int cx, int cy) = 0;
  /** \brief Draws an Fl_Pixmap object to the device. 
   *
   Specifies a bounding box for the image, with the origin (upper left-hand corner) of 
   the image offset by the cx and cy arguments.
   */
  virtual   void draw(Fl_Pixmap * pxm,int XP, int YP, int WP, int HP, int cx, int cy) = 0;
  /** \brief Draws an Fl_Bitmap object to the device. 
   *
   Specifies a bounding box for the image, with the origin (upper left-hand corner) of 
   the image offset by the cx and cy arguments.
   */
  virtual void draw(Fl_Bitmap *bm, int XP, int YP, int WP, int HP, int cx, int cy) = 0;
    
public:
  static const char *class_id;
  virtual const char *class_name() {return class_id;};
  /** \brief see fltk3::font(fltk3::Font face, fltk3::Fontsize size). */
  virtual void font(fltk3::Font face, fltk3::Fontsize size) {font_ = face; size_ = size;}
  /** \brief see fltk3::font(void). */
  fltk3::Font font() {return font_; }
  /** \brief see fltk3::size(). */
  fltk3::Fontsize size() {return size_; }
  /** \brief see fltk3::width(const char *str, int n). */
  virtual double width(const char *str, int n) = 0;
  /** \brief see fltk3::width(unsigned int n). */
  virtual inline double width(unsigned int c) { char ch = (char)c; return width(&ch, 1); }
  /** \brief see fltk3::text_extents(const char*, int n, int& dx, int& dy, int& w, int& h). */
  virtual void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
  /** \brief see fltk3::height(). */
  virtual int height() = 0;
  /** \brief see fltk3::descent(). */
  virtual int descent() = 0;
  /** \brief see fltk3::color(void). */
  fltk3::Color color() {return color_;}
  /** Returns a pointer to the current Fl_Font_Descriptor for the graphics driver */
  inline Fl_Font_Descriptor *font_descriptor() { return font_descriptor_;}
  /** Sets the current Fl_Font_Descriptor for the graphics driver */
  inline void font_descriptor(Fl_Font_Descriptor *d) { font_descriptor_ = d;}
  /** \brief The destructor */
  virtual ~Fl_Graphics_Driver() {};
};

#if defined(__APPLE__) || defined(FL_DOXYGEN)
/**
 \brief The Mac OS X-specific graphics class.
 *
 This class is implemented only on the Mac OS X platform.
 */
class FLTK3_EXPORT Fl_Quartz_Graphics_Driver : public Fl_Graphics_Driver {
public:
  static const char *class_id;
  const char *class_name() {return class_id;};
  void color(fltk3::Color c);
  void color(uchar r, uchar g, uchar b);
  void draw(const char* str, int n, int x, int y);
#ifdef __APPLE__
  void draw(const char *str, int n, float x, float y);
#endif
  void draw(int angle, const char *str, int n, int x, int y);
  void rtl_draw(const char* str, int n, int x, int y);
  void font(fltk3::Font face, fltk3::Fontsize size);
  void draw(Fl_Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw(Fl_Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw(Fl_RGB_Image *img, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0);
  void draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=3);
  void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0);
  void draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=1);
  double width(const char *str, int n);
  double width(unsigned int c);
  void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
  int height();
  int descent();
};
#endif
#if defined(WIN32) || defined(FL_DOXYGEN)
/**
 \brief The MSWindows-specific graphics class.
 *
 This class is implemented only on the MSWindows platform.
 */
class FLTK3_EXPORT Fl_GDI_Graphics_Driver : public Fl_Graphics_Driver {
public:
  static const char *class_id;
  const char *class_name() {return class_id;};
  void color(fltk3::Color c);
  void color(uchar r, uchar g, uchar b);
  void draw(const char* str, int n, int x, int y);
  void draw(int angle, const char *str, int n, int x, int y);
  void rtl_draw(const char* str, int n, int x, int y);
  void font(fltk3::Font face, fltk3::Fontsize size);
  void draw(Fl_Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw(Fl_Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw(Fl_RGB_Image *img, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0);
  void draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=3);
  void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0);
  void draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=1);
  double width(const char *str, int n);
  double width(unsigned int c);
  void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
  int height();
  int descent();
};
#endif
#if !(defined(__APPLE__) || defined(WIN32))
/**
 \brief The Xlib-specific graphics class.
 *
 This class is implemented only on the Xlib platform.
 */
class Fl_Xlib_Graphics_Driver : public Fl_Graphics_Driver {
public:
  static const char *class_id;
  const char *class_name() {return class_id;};
  void color(fltk3::Color c);
  void color(uchar r, uchar g, uchar b);
  void draw(const char* str, int n, int x, int y);
  void draw(int angle, const char *str, int n, int x, int y);
  void rtl_draw(const char* str, int n, int x, int y);
  void font(fltk3::Font face, fltk3::Fontsize size);
  void draw(Fl_Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw(Fl_Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw(Fl_RGB_Image *img, int XP, int YP, int WP, int HP, int cx, int cy);
  void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0);
  void draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=3);
  void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0);
  void draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=1);
  double width(const char *str, int n);
  double width(unsigned int c);
  void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
  int height();
  int descent();
};
#endif

/**
 \brief A surface that's susceptible to receive graphical output.
 */
class FLTK3_EXPORT Fl_Surface_Device : public Fl_Device {
  /** \brief The graphics driver in use by this surface. */
  Fl_Graphics_Driver *_driver;
  static Fl_Surface_Device *_surface; // the surface that currently receives graphics output
protected:
  /** \brief Constructor that sets the graphics driver to use for the created surface. */
  Fl_Surface_Device(Fl_Graphics_Driver *graphics_driver) {_driver = graphics_driver; };
public:
  static const char *class_id;
  const char *class_name() {return class_id;};
  virtual void set_current(void);
  /** \brief Sets the graphics driver of this drawing surface. */
  inline void driver(Fl_Graphics_Driver *graphics_driver) {_driver = graphics_driver;};
  /** \brief Returns the graphics driver of this drawing surface. */
  inline Fl_Graphics_Driver *driver() {return _driver; };
  /** \brief the surface that currently receives graphics output */
  static inline Fl_Surface_Device *surface() {return _surface; }; 
  /** \brief The destructor. */
  virtual ~Fl_Surface_Device() {}
};

/**
 \brief A display to which the computer can draw.
 */
class FLTK3_EXPORT Fl_Display_Device : public Fl_Surface_Device {
  static Fl_Display_Device *_display; // the platform display device
public:
  static const char *class_id;
  const char *class_name() {return class_id;};
  /** \brief A constructor that sets the graphics driver used by the display */
  Fl_Display_Device(Fl_Graphics_Driver *graphics_driver);
  /** Returns the platform display device. */
  static inline Fl_Display_Device *display_device() {return _display;};
};

/**
 This plugin socket allows the integration of new device drivers for special
 window or screen types. It is currently used to provide an automated printing
 service for OpenGL windows, if linked with fltk_gl.
 */
class FLTK3_EXPORT Fl_Device_Plugin : public Fl_Plugin {
public:
  /** \brief The constructor */
  Fl_Device_Plugin(const char *name)
  : Fl_Plugin(klass(), name) { }
  /** \brief Returns the class name */
  virtual const char *klass() { return "fltk:device"; }
  /** \brief Returns the plugin name */
  virtual const char *name() = 0;
  /** \brief Prints a widget 
   \param w the widget
   \param x,y offsets where to print relatively to coordinates origin
   \param height height of the current drawing area
   */
  virtual int print(fltk3::Widget* w, int x, int y, int height) = 0;
};

#endif // Fl_Device_H

//
// End of "$Id: Device.h 8529 2011-03-23 12:49:30Z AlbrechtS $".
//