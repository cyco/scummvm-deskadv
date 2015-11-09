//
//  Action.h
//  scummvm
//
//  Created by Christoph Leimbrock on 11/9/15.
//
//

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

    
} // End of namespace Deskadv

#endif