//
// "$Id$"
//
// fltk3::HelpDialog dialog for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/* \file
 fltk3::HelpDialog widget . */

// generated by Fast Light User Interface Designer (fluid) version 1.0108

#ifndef Fltk3_Help_Dialog_H
#define Fltk3_Help_Dialog_H
#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Group.h>
#include <fltk3/Button.h>
#include <fltk3/Input.h>
#include <fltk3/Box.h>
#include <fltk3/HelpView.h>

namespace fltk3 {
  
  class FLTK3_EXPORT HelpDialog {
    int index_; 
    int max_; 
    int line_[100];                 // FIXME: we must remove those static numbers
    char file_[100][FLTK3_PATH_MAX];   // FIXME: we must remove those static numbers
    int find_pos_; 
  public:
    HelpDialog();
  private:
    fltk3::DoubleWindow *window_;
    fltk3::Button *back_;
    void cb_back__i(fltk3::Button*, void*);
    static void cb_back_(fltk3::Button*, void*);
    fltk3::Button *forward_;
    void cb_forward__i(fltk3::Button*, void*);
    static void cb_forward_(fltk3::Button*, void*);
    fltk3::Button *smaller_;
    void cb_smaller__i(fltk3::Button*, void*);
    static void cb_smaller_(fltk3::Button*, void*);
    fltk3::Button *larger_;
    void cb_larger__i(fltk3::Button*, void*);
    static void cb_larger_(fltk3::Button*, void*);
    fltk3::Input *find_;
    void cb_find__i(fltk3::Input*, void*);
    static void cb_find_(fltk3::Input*, void*);
    fltk3::HelpView *view_;
    void cb_view__i(fltk3::HelpView*, void*);
    static void cb_view_(fltk3::HelpView*, void*);
  public:
    ~HelpDialog();
    int h();
    void hide();
    void load(const char *f);
    void position(int xx, int yy);
    void resize(int xx, int yy, int ww, int hh);
    void show();
    void show(int argc, char **argv);
    void textsize(fltk3::Fontsize s);
    fltk3::Fontsize textsize();
    void topline(const char *n);
    void topline(int n);
    void value(const char *f);
    const char * value() const;
    int visible();
    int w();
    int x();
    int y();
  };
  
}

#endif

//
// End of "$Id$".
//
