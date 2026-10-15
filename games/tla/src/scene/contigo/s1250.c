#include "contigo.h"
#include "scene_ids.h"

void Func_020000f4(void);
void Func_02000158(void);
void Func_0200428c(void);
void Func_020041ac(s32 scene, s32 entrance);

void Func_02001250(void)
{
    Func_020000f4();
    Func_0200428c();
    /* FAKEMATCH: the loop note ends the scheduling block: the native loads the party state address before it builds the field offset (2 differ without it; an inline accessor and an inline setter the same or worse). */
    do
    {
    } while (0);
    gPartyState.unk2c2 = 2;
    Func_020041ac((s32)&SceneId_Ov6B0Room1, 99);
    Battle_WaitMode0(40);
    Func_02000158();
}
