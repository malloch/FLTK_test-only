//
// "$Id: Fl_Menu_Bar.cxx,v 1.34 2000/02/14 11:32:53 bill Exp $"
//
// Menu bar widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_draw.H>

extern Fl_Named_Style* fl_title_style; // in Fl_Menu.cxx

void Fl_Menu_Bar::draw() {
  if (damage()&(~FL_DAMAGE_HIGHLIGHT)) draw_box();
  if (!children()) { last_ = -1; return; }
  int X = x()+3;
  for (int i = 0; i < children(); i++) {
    Fl_Widget* o = child(i);
    if (!o->visible()) continue;
    if (!o->h() || o->damage() & FL_DAMAGE_LAYOUT) o->layout();
    int W = o->w() + 10;
    if (damage()&(~FL_DAMAGE_HIGHLIGHT) || last_ == i || highlight_ == i) {
      // If you change how the items are drawn, you probably need to
      // change MenuTitle::draw and the functions find_selected and
      // titlex in Fl_Menu.cxx.
      Fl_Flags flags = (o->flags()&FL_INACTIVE);
      Fl_Color bgcolor = color();
      Fl_Color label_color = o->label_color();
      if (i == highlight_ && highlight_color() && takesevents()) {
	flags |= FL_HIGHLIGHT;
	bgcolor = highlight_color();
	Fl_Color c = highlight_label_color(); if (c) label_color = c;
      }
      fl_title_style->box->draw(X, y()+3, W, this->h()-6, bgcolor, flags);
      o->x(X+5);
      o->y(y()+(h()-o->h())/2);
      int save_w = o->w(); o->w(W-10);
      fl_color(label_color); o->draw();
      o->w(save_w);
    }
    X += W;
  }
  last_ = highlight_;
}

int Fl_Menu_Bar::handle(int event) {
  if (!children()) return 0;
  int X = x()+3;
  int i;
  highlight_ = -1;
  // FL_LEAVE events don't get the right coordinates
  if (event != FL_LEAVE) for (i = 0; i < children(); ++i) {
    Fl_Widget* m = child(i);
    if (!m->visible()) continue;
    if (!m->h() || m->damage() & FL_DAMAGE_LAYOUT) m->layout();
    int W = m->w() + 10;
    if (Fl::event_inside(X, y() + 3, W, h() - 6)) {
      highlight_ = i;
      break;
    }
    X += W;
  }
  switch (event) {
  case FL_MOVE:
  case FL_ENTER:
  case FL_LEAVE:
    if (highlight_ == last_) return 1;
    if (highlight_color() && takesevents()) damage(FL_DAMAGE_HIGHLIGHT);
    return 1;
  case FL_PUSH:
    if (highlight_ < 0) return 0;
    value(-1);
  J1:
    highlight_ = -1; damage(FL_DAMAGE_HIGHLIGHT);
    pulldown(x(), y(), w(), h(), 0, 1);
    return 1;
  case FL_SHORTCUT:
    if (handle_shortcut()) return 1;
    for (i = 0; i < children(); i++) {
      Fl_Widget* w = child(i);
      if (w->active() && w->test_shortcut()) {value(i); goto J1;}
    }
    return 0;
  case FL_KEYUP:
    if ((Fl::event_key() == FL_Alt_L || Fl::event_key() == FL_Alt_R)
	&& Fl::event_clicks()) {
      // checking for event_clicks insures that the keyup matches the
      // keydown that preceeded it, so Alt was pressed & released without
      // any intermediate values.  On X it is false if Alt is held down
      // for a long time, too.
      for (i = 0; i < children(); i++) {
	Fl_Widget* w = child(i);
	if (w->active()) {value(i); goto J1;}
      }
      return 0;
    }
  }
  return 0;
}

static void revert(Fl_Style* s) {
  s->box = FL_FLAT_BOX;
}

static Fl_Named_Style* style = new Fl_Named_Style("Menu_Bar", revert, &style);

Fl_Menu_Bar::Fl_Menu_Bar(int x,int y,int w,int h,const char *l)
  : Fl_Menu_(x,y,w,h,l)
{
  style(::style);
}

//
// End of "$Id: Fl_Menu_Bar.cxx,v 1.34 2000/02/14 11:32:53 bill Exp $".
//
