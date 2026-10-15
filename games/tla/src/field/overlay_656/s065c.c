#include "ov656.h"

// A switch that toggles flag 0x207 and its map edits once flag 0x983 is set; before that,
// "There's no reaction... It doesn't seem to be running."
void Func_0200065c(void)
{
    Func_02002c68();
    Func_02002da8(0);
    if (Ov656_FlagChk(0x983))
    {
        if (Ov656_FlagChk(0x207))
        {
            Ov656_SePlay(232);
            Ov656_ObjSetPos(Object_GetById(15), 0x3380000, 0xffe00000, 0x480000);
            Object_GetById(15)->height = 0xffe00000;
            Ov656_MapCopy(72, 68, 50, 64, 3, 3);
            Ov656_BlendStart(106, 16, 3, 2, 110, 4);
            Ov656_BlendStart(50, 1, 3, 1, 50, 4);
            GameFlag_ClearBit(0x207);
        }
        else if (Ov656_FlagChk(0x212))
        {
            Ov656_SePlay(117);
        }
        else
        {
            Ov656_SePlay(232);
            Ov656_ObjPlace(15, 0, 0);
            Ov656_MapCopy(68, 68, 50, 64, 3, 3);
            Ov656_BlendStart(102, 16, 3, 2, 110, 4);
            Ov656_BlendStart(50, 0, 3, 1, 50, 4);
            Ov656_FlagSet(0x207);
        }
    }
    else
    {
        UiText_ShowPositionedMessageAndWait((s32)MsgScene656No02, 1);
    }
    Func_02002c70();
}
