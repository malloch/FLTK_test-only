// generated by Fast Light User Interface Designer (fluid) version 2.0001

#include "about_panel.h"

Fl_Window* about_panel;

Fl_Group* display_group;

static void cb_1998(Fl_Button*, void*) {
  display_group->hide();
  copyright_box->show();
}

Fl_Box* copyright_box;

static void cb_OK(Fl_Return_Button* o, void*) {
  ((Fl_Window*)(o->parent()))->hide();
}

Fl_Window* make_about_panel(const char *copyright) {
  Fl_Window* w;
   {Fl_Window* o = about_panel = new Fl_Window(303, 258, "about fluid");
    w = o;
    o->color((Fl_Color)97);
    o->selection_color((Fl_Color)47);
    ((Fl_Window*)(o))->hotspot(o);
     {Fl_Group* o = display_group = new Fl_Group(5, 5, 295, 245);
      o->box(FL_NO_BOX);
       {Fl_Group* o = new Fl_Group(55, 25, 60, 150);
        o->box(FL_UP_BOX);
        o->color((Fl_Color)14);
        o->label_color((Fl_Color)6);
        o->selection_color((Fl_Color)47);
         {Fl_Box* o = new Fl_Box(30, 90, 15, 15, "` ");
          o->box(FL_OVAL_BOX);
          o->color((Fl_Color)6);
          o->label_color((Fl_Color)7);
          o->selection_color((Fl_Color)47);
          o->label_size(20);
          o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
        }
        o->resizable(o);
        o->end();
      }
       {Fl_Box* o = new Fl_Box(40, 0, 90, 40);
        o->box(FL_FLAT_BOX);
        o->color((Fl_Color)97);
        o->selection_color((Fl_Color)47);
      }
       {Fl_Box* o = new Fl_Box(60, 45, 50, 65);
        o->box(FL_DOWN_BOX);
        o->color((Fl_Color)6);
        o->selection_color((Fl_Color)47);
      }
       {Fl_Box* o = new Fl_Box(45, 25, 80, 25);
        o->box(FL_ROUND_UP_BOX);
        o->color((Fl_Color)6);
        o->label_color((Fl_Color)6);
        o->selection_color((Fl_Color)47);
        o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
      }
       {Fl_Group* o = new Fl_Group(10, 130, 150, 105, "fluid");
        o->box(FL_ROUND_UP_BOX);
        o->label_font(fl_fonts+1);
        o->label_type(FL_SHADOW_LABEL);
        o->color((Fl_Color)14);
        o->label_color((Fl_Color)6);
        o->selection_color((Fl_Color)47);
        o->label_size(40);
        o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
         {Fl_Box* o = new Fl_Box(69, 20, 15, 15, "` ");
          o->box(FL_OVAL_BOX);
          o->color((Fl_Color)6);
          o->label_color((Fl_Color)7);
          o->selection_color((Fl_Color)47);
          o->label_size(20);
          o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
        }
         {Fl_Box* o = new Fl_Box(60, 5, 15, 15, "` ");
          o->box(FL_OVAL_BOX);
          o->color((Fl_Color)6);
          o->label_color((Fl_Color)7);
          o->selection_color((Fl_Color)47);
          o->label_size(20);
          o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
        }
        o->resizable(o);
        o->end();
      }
       {Fl_Box* o = new Fl_Box(57, 130, 56, 2);
        o->box(FL_FLAT_BOX);
        o->color((Fl_Color)14);
        o->selection_color((Fl_Color)47);
      }
       {Fl_Group* o = new Fl_Group(145, 18, 149, 122);
        o->box(FL_OFLAT_BOX);
        o->label_font(fl_fonts+1);
        o->color((Fl_Color)14);
        o->label_color((Fl_Color)7);
        o->selection_color((Fl_Color)47);
        o->label_size(18);
         {Fl_Box* o = new Fl_Box(5, 4, 140, 113, "FLTK User\nInterface\nDesigner\nVersion 2.0");
          o->box(FL_OVAL_BOX);
          o->label_font(fl_fonts+1);
          o->label_type(FL_SHADOW_LABEL);
          o->color((Fl_Color)12);
          o->label_color((Fl_Color)7);
          o->selection_color((Fl_Color)47);
          o->label_size(18);
        }
        o->resizable(o);
        o->end();
      }
       {Fl_Button* o = new Fl_Button(175, 159, 95, 41, "\251""1998-2001\nClick here for more information");
        o->box(FL_BORDER_BOX);
        o->color((Fl_Color)136);
        o->label_color((Fl_Color)6);
        o->label_size(10);
        o->callback((Fl_Callback*)cb_1998);
        o->align(FL_ALIGN_WRAP);
      }
      o->resizable(o);
      o->end();
    }
     {Fl_Box* o = copyright_box = new Fl_Box(3, 5, 297, 250);
      o->label_color((Fl_Color)6);
      o->label_size(10);
      o->align(132|FL_ALIGN_INSIDE);
      o->hide();
      Fl_Group::current()->resizable(o);
      o->label(copyright);
    }
     {Fl_Return_Button* o = new Fl_Return_Button(240, 223, 55, 22, "OK");
      o->label_size(10);
      o->shortcut(0xff0d);
      o->callback((Fl_Callback*)cb_OK);
    }
    o->set_non_modal();
    o->end();
  }
  return w;
}
