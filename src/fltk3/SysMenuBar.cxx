//
// "$Id$"
//
// MacOS system menu bar widget for the Fast Light Tool Kit (FLTK).
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

/**
 * This code is a quick hack! It was written as a proof of concept.
 * It has been tested on the "menubar" sample program and provides
 * basic functionality. 
 * 
 * To use the System Menu Bar, simply replace the main fltk3::MenuBar
 * in an application with fltk3::SysMenuBar.
 *
 * FLTK features not supported by the Mac System menu
 *
 * - no invisible menu items
 * - no symbolic labels
 * - embossed labels will be underlined instead
 * - no font sizes
 * - Shortcut Characters should be English alphanumeric only, no modifiers yet
 * - no disable main menus
 * - changes to menubar in run-time don't update! 
 *     (disable, etc. - toggle and radio button do!)
 *
 * No care was taken to clean up the menu bar after destruction!
 * ::menu(bar) should only be called once!
 * Many other calls of the parent class don't work.
 * Changing the menu items has no effect on the menu bar.
 * Starting with OS X 10.5, FLTK applications must be created as
 * a bundle for the System Menu Bar (and maybe other features) to work!
 */

#if defined(__APPLE__) || defined(FLTK3_DOXYGEN)

#include <fltk3/x.h>
#include <fltk3/run.h>
#include <fltk3/SysMenuBar.h>

#include "flstring.h"
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

#define MenuHandle void *

typedef const fltk3::MenuItem *pFl_Menu_Item;
 

/*
 * Set a shortcut for an Apple menu item using the FLTK shortcut descriptor.
 */
static void setMenuShortcut( MenuHandle mh, int miCnt, const fltk3::MenuItem *m )
{
  if ( !m->shortcut_ ) 
    return;
  if ( m->flags & fltk3::SUBMENU )
    return;
  if ( m->flags & fltk3::SUBMENU_POINTER )
    return;
  char key = m->shortcut_ & 0xff;
  if ( !isalnum( key ) )
    return;
  
  void *menuItem = fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::itemAtIndex, mh, miCnt);
  fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::setKeyEquivalent, menuItem, m->shortcut_ & 0xff );
  fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::setKeyEquivalentModifierMask, menuItem, m->shortcut_ );
}


/*
 * Set the Toggle and Radio flag based on FLTK flags
 */
static void setMenuFlags( MenuHandle mh, int miCnt, const fltk3::MenuItem *m )
{
  if ( m->flags & fltk3::MENU_TOGGLE )
  {
	void *menuItem = fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::itemAtIndex, mh, miCnt);
	fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::setState, menuItem, m->flags & fltk3::MENU_VALUE );
  }
  else if ( m->flags & fltk3::MENU_RADIO ) {
    void *menuItem = fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::itemAtIndex, mh, miCnt);
    fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::setState, menuItem, m->flags & fltk3::MENU_VALUE );
  }
}


/*
 * create a sub menu for a specific menu handle
 */
static void createSubMenu( void * mh, pFl_Menu_Item &mm )
{
  void *submenu;
  int miCnt, flags;
  
  void *menuItem;
  submenu = fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::initWithTitle, mm->text);
  int cnt;
  fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::numberOfItems, mh, &cnt);
  cnt--;
  menuItem = fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::itemAtIndex, mh, cnt);
  fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::setSubmenu, menuItem, submenu);
  if ( mm->flags & fltk3::MENU_INACTIVE ) {
    fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::setEnabled, menuItem, 0);
  }
  mm++;
  
  while ( mm->text )
  {
    int flRank = mm - fltk3::sys_menu_bar->fltk3::Menu_::menu();
    fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::addNewItem, submenu, flRank, &miCnt);
    setMenuFlags( submenu, miCnt, mm );
    setMenuShortcut( submenu, miCnt, mm );
    if ( mm->flags & fltk3::MENU_INACTIVE ) {
      void *item = fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::itemAtIndex, submenu, miCnt);
      fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::setEnabled, item, 0);
    }
    flags = mm->flags;
    if ( mm->flags & fltk3::SUBMENU )
    {
      createSubMenu( submenu, mm );
    }
    else if ( mm->flags & fltk3::SUBMENU_POINTER )
    {
      const fltk3::MenuItem *smm = (fltk3::MenuItem*)mm->user_data_;
      createSubMenu( submenu, smm );
    }
    if ( flags & fltk3::MENU_DIVIDER ) {
      fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::addSeparatorItem, submenu);
      }
    mm++;
  }
}
 

/*
 * convert a complete fltk3::MenuItem array into a series of menus in the top menu bar
 * ALL PREVIOUS SYSTEM MENUS, EXCEPT APPLICATION MENU, ARE REPLACED BY THE NEW DATA
 */
static void convertToMenuBar(const fltk3::MenuItem *mm)
{
  int count;//first, delete all existing system menus
  fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::numberOfItems, fl_system_menu, &count);
  for(int i = count - 1; i > 0; i--) {
	  fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::removeItem, fl_system_menu, i);
  }
  //now convert FLTK stuff into MacOS menus
  for (;;)
  {
    if ( !mm || !mm->text )
      break;
    char visible = mm->visible() ? 1 : 0;
    int flRank = mm - fltk3::sys_menu_bar->fltk3::Menu_::menu();
    fltk3::SysMenuBar::doMenuOrItemOperation(fltk3::SysMenuBar::addNewItem, fl_system_menu, flRank, NULL);
		
    if ( mm->flags & fltk3::SUBMENU )
      createSubMenu( fl_system_menu, mm );
    else if ( mm->flags & fltk3::SUBMENU_POINTER ) {
      const fltk3::MenuItem *smm = (fltk3::MenuItem*)mm->user_data_;
      createSubMenu( fl_system_menu, smm );
    }
    if ( visible ) {
      //      InsertMenu( mh, 0 );
    }
    mm++;
  }
}


/**
 * @brief create a system menu bar using the given list of menu structs
 *
 * \author Matthias Melcher
 *
 * @param m list of fltk3::MenuItem
 */
void fltk3::SysMenuBar::menu(const fltk3::MenuItem *m) 
{
  fl_open_display();
  fltk3::MenuBar::menu( m );
  convertToMenuBar(m);
}


/**
 * @brief add to the system menu bar a new menu item
 *
 * add to the system menu bar a new menu item, with a title string, shortcut int,
 * callback, argument to the callback, and flags.
 *
 * @see fltk3::Menu_::add(const char* label, int shortcut, fltk3::Callback *cb, void *user_data, int flags) 
 */
int fltk3::SysMenuBar::add(const char* label, unsigned int shortcut, fltk3::Callback *cb, void *user_data, int flags)
{
  fl_open_display();
  int rank = fltk3::Menu_::add(label, shortcut, cb, user_data, flags);
  convertToMenuBar(fltk3::Menu_::menu());
  return rank;
}

/**
 * @brief insert in the system menu bar a new menu item
 *
 * insert in the system menu bar a new menu item, with a title string, shortcut int,
 * callback, argument to the callback, and flags.
 *
 * @see fltk3::Menu_::insert(int index, const char* label, int shortcut, fltk3::Callback *cb, void *user_data, int flags) 
 */
int fltk3::SysMenuBar::insert(int index, const char* label, unsigned int shortcut, fltk3::Callback *cb, void *user_data, int flags)
{
  fl_open_display();
  int rank = fltk3::Menu_::insert(index, label, shortcut, cb, user_data, flags);
  convertToMenuBar(fltk3::Menu_::menu());
  return rank;
}

void fltk3::SysMenuBar::clear()
{
  fltk3::Menu_::clear();
  convertToMenuBar(NULL);
}

int fltk3::SysMenuBar::clear_submenu(int index)
{
  int retval = fltk3::Menu_::clear_submenu(index);
  if (retval != -1) convertToMenuBar(fltk3::Menu_::menu());
  return retval;
}

/**
 * @brief remove an item from the system menu bar
 *
 * @param rank		the rank of the item to remove
 */
void fltk3::SysMenuBar::remove(int rank)
{
  fltk3::Menu_::remove(rank);
  convertToMenuBar(fltk3::Menu_::menu());
}


/**
 * @brief rename an item from the system menu bar
 *
 * @param rank		the rank of the item to rename
 * @param name		the new item name as a UTF8 string
 */
void fltk3::SysMenuBar::replace(int rank, const char *name)
{
  doMenuOrItemOperation(renameItem, rank, name);
  fltk3::sys_menu_bar->fltk3::Menu_::replace(rank, name);
}


/*
 * Draw the menu bar. 
 * Nothing here because the OS does this for us.
 */
void fltk3::SysMenuBar::draw() {
}


#endif /* __APPLE__ */

//
// End of "$Id$".
//