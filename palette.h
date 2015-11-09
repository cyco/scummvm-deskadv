/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef DESKADV_PALETTE_H
#define DESKADV_PALETTE_H

namespace Deskadv {

// TODO: Not sure if palette uses any of the Win3.1 default palette indexes
//       i.e. whether the palette setting skips these or overwrites them.
// 1 - dark red
// 2 - dark green
// 3 - dark yellow
// 4 - dark blue
// 5 - dark magenta
// 6 - dark cyan
// 7 - light grey
// 8 - money green
// 9 - sky blue
// --
// 246 - cream
// 247 - medium grey
// 248 - dark grey
// 249 - red
// 250 - green
// 251 - yellow
// 252 - blue
// 253 - magenta
// 254 - cyan
// --
// 0 - black and 255 - white are also in W3.1 default palette, but are set here
// to the same values so not critical if they are set or skipped.

const static uint   TRANSPARENT =   0;
const static uint         BLACK =   0;
const static uint         WHITE = 255;

const static uint    LIGHT_GREY =   7;
const static uint   MEDIUM_GREY = 247;
const static uint     DARK_GREY = 248;

const static uint  ARROW_SHADOW =  24;
const static uint         GREEN = 200;
const static uint HEALTH_YELLOW = 176;
const static uint    HEALTH_RED = 204;
const static uint    POWER_BLUE = 203;

} // End of namespace Deskadv

#endif
