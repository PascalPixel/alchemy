#include "types.h"
#include "scene.h"

extern u8 gOv[];
extern s32 *gWork;

void Scene_RunActorTwentyDialogueSequence(void)
{
    Actor_Run();
    Actor_Do((s32)gOv);
    Actor_unk2_2(1);
    Actor_Apply(0, 15);
    Actor_Apply2(Actor_Check(0), 0);
    gWork[0x70] = 0x202;
    Actor_unk2_4();
    Actor_unk3_4();
    Actor_unk3_2(20);
    Actor_Apply3(20, 1);
    Actor_unk4_2(0x1e41);
    Actor_Place(20, 0, 10);
    Actor_Apply4(22, 0x5000);
    Actor_unk2_3(22, 4, 20);
    Actor_Apply5(22, 2);
    Actor_unk3_3(0x6016, 0, 20);
    Actor_unk4_4();
    Actor_unk5_4();
    Actor_unk5_2(11);
}
