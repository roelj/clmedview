/*
 * Copyright (C) 2015 Marc Geerlings <m.geerlings@mumc.nl>
 *
 * This file is part of clmedview.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "lib-common-tree.h"
//#include "lib-common-list.h"
//#include "lib-pixeldata-plugin.h"

/**
 * @file include/libconfiguration.h
 * @brief A program-wide configurable state.
 * @author Roel Janssen
 */


/**
 * @ingroup configuration
 * @{
 *
 * This module provides a singleton to give an independent way to store the
 * program's state. The need for this module has come since there are multiple
 * user interfaces to interact with.
 */


#define KEY_TOGGLE_FOLLOW       0
#define KEY_TOGGLE_AUTOCLOSE    1
#define KEY_UNDO                2
#define KEY_REDO                3
#define KEY_VIEW_AXIAL          4
#define KEY_VIEW_SAGITAL        5
#define KEY_VIEW_CORONAL        6
#define KEY_VIEW_SPLIT          7
#define KEY_TOGGLE_TERMINAL     8
#define KEY_TOGGLE_SIDEBAR      9
#define KEY_TOGGLE_RECORDING    10
#define KEY_REPLAY_RECORDING    11
#define KEY_REPLAY_OVER_TIME    12
#define KEY_REPLAY_OVER_SLICES  13


/**
 * An opaque type providing access to common configuration items.
 * Access to its members should be done using the macro definitions.
 */
typedef struct
{
  void *memory_tree; /*< The actively maintained memory tree. */
  void *active_study; /*< The active study element in the tree. */
  void *active_serie; /*< The active serie element in the tree. */

//  Tree *active_original; /*< The active original serie in the tree. */
  void *active_mask_serie; /*< The active mask serie in the tree. */

  void *draw_tools; /*< A list of all loaded draw tools. */
  void *lookup_tables; /*< A list of lookup tables. */
  void *active_draw_tool; /*< The active draw tool element in the list. */

  unsigned char key_bindings[14]; /*< An array with key bindings. */

} Configuration;


/**
 * Returns the active memory tree.
 */
#define CONFIGURATION_MEMORY_TREE(c)       c->memory_tree


/**
 * Returns the active study in the memory tree.
 */
#define CONFIGURATION_ACTIVE_STUDY(c)      c->active_study

/**
 * Returns the active serie in the memory tree.
 */
#define CONFIGURATION_ACTIVE_SERIE(c)      c->active_serie


/**
 * Returns the active mask serie in the memory tree.
 */
#define CONFIGURATION_ACTIVE_MASK_SERIE(c) c->active_mask_serie


/**
 * Returns a list of loaded draw tools.
 */
#define CONFIGURATION_PLUGINS(c)           c->draw_tools


/**
 * Returns the active draw tool list item.
 */
#define CONFIGURATION_ACTIVE_PLUGIN(c)     c->active_draw_tool


/**
 * Returns the bound character for a key.
 */
#define CONFIGURATION_KEY(c,k)             c->key_bindings[k]


/**
 * Returns a list of lookup tables.
 */
#define CONFIGURATION_LOOKUP_TABLES(c)     c->lookup_tables


/**
 * An instance of the global configuration state.
 */
Configuration configuration_state;


/**
 * A function to get a pointer to the global configuration object.
 *
 * @return a pointer to the global configuration object.
 */
Configuration *configuration_get_default ();


/**
 * This function should be called before using any of this module's
 * functionality.
 */
void configuration_init ();


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif//CONFIGURATION_H