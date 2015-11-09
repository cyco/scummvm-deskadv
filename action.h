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

#ifndef DESKADV_ACTION_H
#define DESKADV_ACTION_H

namespace Deskadv {

enum {
    kConditionOpcodeZoneNotInitalized =  0x00,
    kConditionOpcodeZoneEntered =  0x01,
    kConditionOpcodeBump =  0x02,
    kConditionOpcodePlaceItem =  0x03,
    kConditionOpcodeStandingOn =  0x04,
    kConditionOpcodeCounterIs =  0x05,
    kConditionOpcodeRandomIs =  0x06,
    kConditionOpcodeRandomIsNot =  0x07, // guessed
    kConditionOpcodeTileIs =  0x08,
    kConditionOpcodeEnterByPlane =  0x09,
    kConditionOpcodeTileAtIs =  0x0a,
    kConditionOpcodeTileAt =  0x0b,
    //  =  0x0c,
    kConditionOpcodeHasItem =  0x0d,
    kConditionOpcodeRequiredItem =  0x0e,
    kConditionOpcodeEnding =  0x0f,
    //  =  0x10,
    //  =  0x12,
    kConditionOpcodeHealthIsLessThan =  0x13,
    kConditionOpcodeHealthIsMoreThan =  0x14,
    //  =  0x16,
    kConditionOpcodePlaceItemIsNot =  0x17,
    kConditionOpcodeHeroIsAt =  0x18,
    //  =  0x19,
    //  =  0x1a,
    //  =  0x1b,
    kConditionOpcodeGamesWonIsExactly =  0x1c,
    //  =  0x1d,
    //  =  0x1e,
    kConditionOpcodeCounterIsNot =  0x1f,
    //  =  0x20,
    //  =  0x21,
    //  =  0x22,
    kConditionOpcodeGamesWonBiggerThan =  0x23,
};

enum {
    kInstructionOpcodePlaceTile = 0x00,
    kInstructionOpcodeRemoveTile = 0x01,
    kInstructionOpcodeMoveTile = 0x02,
    kInstructionOpcodeDrawTile = 0x03,
    kInstructionOpcodeSpeakHero = 0x04,
    kInstructionOpcodeSpeakNPC = 0x05,
    kInstructionOpcodeSetTileNeedsDisplay = 0x06,
    kInstructionOpcodeSetRectNeedsDisplay = 0x07,
    kInstructionOpcodeWait = 0x08,
    kInstructionOpcodeRedraw = 0x09,
    kInstructionOpcodePlaySound = 0x0a,
        //  = 0x0b, // update inventory?
    kInstructionOpcodeRollDice = 0x0c,
    kInstructionOpcodeSetCounter = 0x0d,
        //  = 0x0e, // AddToCounter
        //  = 0x0f,
    kInstructionOpcodeHideHero = 0x10,
    kInstructionOpcodeShowHero = 0x11,
    kInstructionOpcodeSetHero = 0x12,
        //  = 0x13,
    kInstructionOpcodeDisableAction = 0x14,
    kInstructionOpcodeDisableHotspot = 0x15,
    kInstructionOpcodeEnableHotspot = 0x16,
        //  = 0x17, // spawnEnemy(id, 0x00)
        //  = 0x18,
        //  = 0x19,
        //  = 0x1a,
    kInstructionOpcodeDropItem = 0x1b,
    kInstructionOpcodeAddItem = 0x1c,
    kInstructionOpcodeRemoveItem = 0x1d,
        //  = 0x1e, // mark zone as solved?
        //  = 0x1f,
    kInstructionOpcodeChangeZone = 0x21,
        //  = 0x22,
        //  = 0x23,
    kInstructionOpcodeSetRandom = 0x24,
    kInstructionOpcodeAddHealth = 0x25,
};

} // End of namespace Deskadv

#endif