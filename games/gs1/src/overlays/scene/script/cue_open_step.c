#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/cue_open_step.h"

extern u8 gWork[];

void Scene_RunCue(void)
{
    u32 i;
    s32 record;

    if (Script_Check(0x84e) != 0) {
        Script_Run();
        Script_Run2(0, 19, 0);
        Script_Place(19, 0x9999, 0x4ccc);
        Script_Place2(19, 0x26e, 0x2fc);
        Script_Place3(19, 0xf000, 20);
        Script_Run3(19, 3);
        Script_Run4(17, 3);
        Script_Run5(20);
        Script_Run6(19, 0, 0);
        Script_Run7(20);
        Script_Run8(19, 3);
        Script_Do(0x1749);
        Script_Run9(19, 0, 10);
        Script_Place4(19, 0xcccc, 0x6666);
        Script_Place5(19, 0x23a, 0x2f6);
        Script_Run10(19, 0, 0);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x209;
        Script_Do2(0x85e);
        Script_Do3(0x333);
        Script_Run11();
    }
}
