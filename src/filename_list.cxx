//
// "$Id: filename_list.cxx,v 1.17 2002/01/20 07:37:16 spitzak Exp $"
//
// Filename list routines for the Fast Light Tool Kit (FLTK).
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

// Wrapper for scandir with a prototype that matches on all systems and
// a "user friendly" sorting function that puts numbered files into the
// order the user expects.

#include <config.h>
#include <fltk/filename.h>
#include <ctype.h>

extern "C" {
static int
numericsort(dirent **A, dirent **B)
{
  const char* a = (*A)->d_name;
  const char* b = (*B)->d_name;
  int ret = 0;
  for (;;) {
    if (isdigit((unsigned)*a) && isdigit((unsigned)*b)) {
      int diff,magdiff;
      while (*a == '0') a++;
      while (*b == '0') b++;
      while (isdigit((unsigned)*a) && *a == *b) {a++; b++;}
      diff = (isdigit((unsigned)*a) && isdigit((unsigned)*b)) ? *a - *b : 0;
      magdiff = 0;
      while (isdigit((unsigned)*a)) {magdiff++; a++;}
      while (isdigit((unsigned)*b)) {magdiff--; b++;}
      if (magdiff) {ret = magdiff; break;} /* compare # of significant digits*/
      if (diff) {ret = diff; break;}	/* compare first non-zero digit */
    } else {
      // compare case-insensitive:
      int t = tolower((unsigned)*a)-tolower((unsigned)*b);
      if (t) {ret = t; break;}
      // see if we reached the end:
      if (!*a) break;
      // remember the case-sensitive comparison, use it if no other diffs:
      if (!ret) ret = *a-*b;
      a++; b++;
    }
  }
  if (!ret) return 0;
  else return (ret < 0) ? -1 : 1;
}
}

#if ! HAVE_SCANDIR
extern "C" int
scandir (const char *dir, dirent ***namelist,
	 int (*select)(dirent *),
	 int (*compar)(dirent **, dirent **));
#endif

int filename_list(const char *d, dirent ***list) {
  // Nobody defines the comparison function prototype correctly!
  // It should be "const dirent* const*". I don't seem to be able to
  // do this even for our own internal version because some compilers
  // will not cast it to the non-const version! Egad. So we have to
  // use if's to go to what the various systems use:
#if defined(__hpux)
  // HP-UX seems to be partially const-correct:
  return scandir(d, list, 0, (int(*)(const dirent **, const dirent **))numericsort);
#elif !HAVE_SCANDIR || defined(__sgi) || defined(__osf__)
  // When we define our own scandir (_WIN32 and perhaps some Unix systems)
  // and also some existing Unix systems take it the way we define it:
  return scandir(d, list, 0, numericsort);
#else
  // The vast majority of Unix systems want the sort function to have this
  // prototype, most likely so that it can be passed to qsort without any
  // changes:
  return scandir(d, list, 0, (int(*)(const void*,const void*))numericsort);
#endif
}

//
// End of "$Id: filename_list.cxx,v 1.17 2002/01/20 07:37:16 spitzak Exp $".
//
