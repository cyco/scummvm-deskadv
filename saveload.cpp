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

#include "deskadv/deskadv.h"
#include "deskadv/saveload.h"

namespace Deskadv {

Saveload::Saveload(DeskadvEngine *vm) : _vm(vm) {
}

Saveload::~Saveload() {
}

} // End of namespace Deskadv

/* save game format (yoda stories)
 *
 * [9] file magic ('INDYSAV44' or 'YODASAV44')
 * [4] unknown
 * [4] unknown
 * [4] unknown
 * [2] puzzle count 1
 *     [2] puzzle idx
 * [2] puzzle count 2
 *     [2] puzzle idx
 * world(2*2) (dagobah)
 * world(10*10)
 * [4] inventory count
 *     [2] tile idx
 * [2] current zone id
 * [4] current zone x (in world coordinates)
 * [4] current zone y (in world coordinates)
 * [2] current weapon
 *     if weapon != 0xFFFF: [2] ammo
 * [2] force ammo
 * [2] blaster ammo
 * [2] blaster rifle ammo
 * [4] hero x on zone (in px)
 * [4] hero y on zone (in px)
 * [4] damage
 * [4] lives
 * [4*4] unknown
 * [4] end puzzle id
 * [4] end puzzle id again
 *
 * world(n) :=
 * n zone headers
 * [4] x
 * [4] y
 * while x,y != 0xFFFF FFFF
 *     [2] zone id
 *     [4] visited
 *     [4] unknown
 *     if visited:
 *          [16] unknown
 *          [zwidth * zheight * 3] tile id
 *          [4] unknown
 *     [4] hotspot count
 *          [2] arg1
 *          [2] arg2
 *          [4] type
 *          [2] x
 *          [2] y
 *     if visited:
 *          [4] npc count
 *              [2] char idx
 *              [2] x
 *              [2] y
 *              [86] unknown
 *          [4] action count
 *              [2] action state info 1
 *              [2] action state info 2
 *      for each 'DoorIn' hotspot with a valid zone id
 *           goto loop body without updating x,y
 *
 * zone header :=
 * [1]  visited
 * [19] unknown
 * [2]  zoen id
 * [2]  unknown
 * [2]  item to be used / lost
 * [2]  item to be gained
 * [2]  unknown
 * [2]  another item to be used / lost
 * [2]  another item to be gained
 * [8]  unknown
 *
 */
