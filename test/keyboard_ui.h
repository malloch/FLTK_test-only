// generated by Fast Light User Interface Designer (fluid) version 2.0001

#ifndef keyboard_ui_h
#define keyboard_ui_h
#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Output.h>
extern Fl_Output *key_output;
extern Fl_Output *text_output;
#include <fltk/Fl_Button.h>
extern void key_cb(Fl_Button*, void*);
#include <fltk/Fl_Box.h>
extern void shift_cb(Fl_Button*, void*);
Fl_Window* make_window();
#endif
