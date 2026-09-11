#include "types.h"
#include "scene.h"

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Talk_Place(actor, axis, offset);
}

void RunSceneEffectSetup(void)
{
    Talk_Run();
    Talk_unk2_4(0, 32768, 16384);
    Talk_unk3_4(158);
    Talk_unk4_4(33604944, 36, 10);
    SetOffset(0, 2, -16);
    Talk_unk5_4(16);
    Talk_unk6_4(2);
    Talk_unk7_4();
}
