/* Near miss of Func_02001250 (17 instructions, 2 differ): the native loads the pool address of
 * gPartyState before it starts the three-instruction offset 0x2c2, the draft after the first of
 * them. sched2 gives the load priority 69 (cost 2) against 70 for the constant; an inline
 * accessor and an inline setter measured the same or worse. The same early pool load keeps
 * 6B9 Func_02000104 and 6BA Func_02000688 drafts. */
#include "ov694.h"
#include "scene_ids.h"

void Func_020000f4(void);
void Func_02000158(void);
void Func_0200428c(void);
void Func_020041ac(s32 scene, s32 entrance);

void Func_02001250(void)
{
    Func_020000f4();
    Func_0200428c();
    gPartyState.unknown_2c2 = 2;
    Func_020041ac((s32)&SceneId_Ov6B0Room1, 99);
    Battle_WaitMode0(40);
    Func_02000158();
}
