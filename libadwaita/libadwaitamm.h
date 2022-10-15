/* Copyright (c) 2019  Roger Ferrer <rofirrim@gmail.com>
 *
 * This file is part of libadwaitamm.
 *
 * libadwaitamm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * libadwaitamm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBADWAITAMM_H_INCLUDED
#define LIBADWAITAMM_H_INCLUDED

/** @mainpage libadwaitamm Reference Manual
 *
 * @section description Description
 *
 * The libadwaitamm C++ binding provides a C++ interface on top of the libadwaita
 * C library.
 *
 * @section overview Overview
 *
 * [...]
 *
 * @section use Use
 *
 * To use libadwaitamm in your C++ application, include the central header file
 * <tt>\<libadwaitamm.h\></tt>.  The libadwaitamm package ships a @c pkg-config
 * file with the correct include path and link command-line for the compiler.
 */

#include <libadwaitammconfig.h>
#include <libadwaitamm/aboutwindow.h>
#include <libadwaitamm/actionrow.h>
#include <libadwaitamm/animation.h>
#include <libadwaitamm/animationtarget.h>
#include <libadwaitamm/application.h>
#include <libadwaitamm/applicationwindow.h>
#include <libadwaitamm/avatar.h>
#include <libadwaitamm/bin.h>
#include <libadwaitamm/buttoncontent.h>
#include <libadwaitamm/callbackanimationtarget.h>
#include <libadwaitamm/carousel.h>
#include <libadwaitamm/carouselindicatordots.h>
#include <libadwaitamm/carouselindicatorlines.h>
#include <libadwaitamm/clamp.h>
#include <libadwaitamm/clamplayout.h>
#include <libadwaitamm/clampscrollable.h>
#include <libadwaitamm/comborow.h>
#include <libadwaitamm/entryrow.h>
#include <libadwaitamm/enumlistitem.h>
#include <libadwaitamm/enumlistmodel.h>
#include <libadwaitamm/expanderrow.h>
#include <libadwaitamm/flap.h>
#include <libadwaitamm/foldthresholdpolicy.h>
#include <libadwaitamm/headerbar.h>
#include <libadwaitamm/leaflet.h>
#include <libadwaitamm/leafletpage.h>
#include <libadwaitamm/messagedialog.h>
#include <libadwaitamm/navigationdirection.h>
#include <libadwaitamm/passwordentryrow.h>
#include <libadwaitamm/preferencesgroup.h>
#include <libadwaitamm/preferencespage.h>
#include <libadwaitamm/preferencesrow.h>
#include <libadwaitamm/preferenceswindow.h>
#include <libadwaitamm/propertyanimationtarget.h>
#include <libadwaitamm/splitbutton.h>
#include <libadwaitamm/springanimation.h>
#include <libadwaitamm/springparams.h>
#include <libadwaitamm/squeezer.h>
#include <libadwaitamm/squeezerpage.h>
#include <libadwaitamm/statuspage.h>
#include <libadwaitamm/stylemanager.h>
#include <libadwaitamm/swipeable.h>
#include <libadwaitamm/swipetracker.h>
#include <libadwaitamm/tabbar.h>
#include <libadwaitamm/tabpage.h>
#include <libadwaitamm/tabview.h>
#include <libadwaitamm/timedanimation.h>
#include <libadwaitamm/toast.h>
#include <libadwaitamm/toastoverlay.h>
#include <libadwaitamm/viewstack.h>
#include <libadwaitamm/viewstackpage.h>
#include <libadwaitamm/viewswitcher.h>
#include <libadwaitamm/viewswitcherbar.h>
#include <libadwaitamm/viewswitchertitle.h>
#include <libadwaitamm/window.h>
#include <libadwaitamm/windowtitle.h>

#endif /* !LIBADWAITAMM_H_INCLUDED */
