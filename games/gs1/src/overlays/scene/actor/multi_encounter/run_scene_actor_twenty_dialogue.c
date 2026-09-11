#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/multi_encounter/run_scene_actor_twenty_dialogue.h"

extern u8 gOv[];
extern s32 *gWork;

void Scene_RunActorTwentyDialogueSequence(void)
{
    Actor_Run();
    Actor_Do((s32)gOv);
    Actor_Do2(1);
    Actor_Apply(0, 15);
    Actor_Apply2(Actor_Check(0), 0);
    gWork[0x70] = 0x202;
    Actor_Run2();
    Actor_Run3();
    Actor_Do3(20);
    Actor_Apply3(20, 1);
    Actor_Do4(0x1e41);
    Actor_Place(20, 0, 10);
    Actor_Apply4(22, 0x5000);
    Actor_Place2(22, 4, 20);
    Actor_Apply5(22, 2);
    Actor_Place3(0x6016, 0, 20);
    Actor_Run4();
    Actor_Run5();
    Actor_Do5(11);
}
