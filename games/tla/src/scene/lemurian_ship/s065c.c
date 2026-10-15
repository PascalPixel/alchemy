#include "lemurian_ship.h"

// A switch that toggles flag 0x207 and its map edits once flag 0x983 is set; before that,
// "There's no reaction... It doesn't seem to be running."
void Func_0200065c(void)
{
    Func_02002c68();
    Func_02002da8(0);
    if (LemurianShip_FlagCheck(0x983))
    {
        if (LemurianShip_FlagCheck(0x207))
        {
            LemurianShip_SePlay(232);
            LemurianShip_ObjSetPos(Object_GetById(15), 0x3380000, 0xffe00000, 0x480000);
            Object_GetById(15)->height = 0xffe00000;
            LemurianShip_MapCopy(72, 68, 50, 64, 3, 3);
            LemurianShip_BlendStart(106, 16, 3, 2, 110, 4);
            LemurianShip_BlendStart(50, 1, 3, 1, 50, 4);
            GameFlag_ClearBit(0x207);
        }
        else if (LemurianShip_FlagCheck(0x212))
        {
            LemurianShip_SePlay(117);
        }
        else
        {
            LemurianShip_SePlay(232);
            LemurianShip_ObjPlace(15, 0, 0);
            LemurianShip_MapCopy(68, 68, 50, 64, 3, 3);
            LemurianShip_BlendStart(102, 16, 3, 2, 110, 4);
            LemurianShip_BlendStart(50, 0, 3, 1, 50, 4);
            LemurianShip_FlagSet(0x207);
        }
    }
    else
    {
        UiText_ShowPositionedMessageAndWait((s32)MsgScene656No02, 1);
    }
    Func_02002c70();
}
