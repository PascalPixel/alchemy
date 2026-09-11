#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/task/run_scene_scene_three_coordinator.h"

extern u8 gCell[];
extern u8 gWork[];
extern s16 gCell2[][1];

void Scene_RunSceneThreeCoordinator(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 record;

    if (gCell2[225][0] == 2) {
        State_Run6();
    } else {
        State_Run7();
        rec2 = State_Check(a0, 3);
        if (rec2 != 0) {
        } else {
            State_Do(0x20a6);
            State_Run(0x30000, 0x6000);
            State_Run2(0x2f00000, -1, 0xc00000, 1);
            State_Run8();
            State_Run9(60);
            State_Run3(0x10000, 0x2000);
            State_Run4(0x2f00000, -1, 0xe00000, 1);
            State_Run10();
            State_Run11(a0, 0);
            State_Run12(0, 0x358, 0x108);
            State_Run13(10);
            State_Place(0, 0x18000, 0xc000);
            State_Place2(0, 0x358, 0x108);
            State_Place3(0, 0x358, 232);
            State_Run14(a0, 0);
            State_Place4(0, 0x348, 232);
            State_Run15(10);
            State_Place5(33, -64, 0);
            State_Run5(0x2f00000, -1, 0xd80000, 1);
            State_Run16(0, 1);
            State_Run17(10);
            State_Place6(0, 0x10000, 0x8000);
            State_Place7(0, 0x2f8, 232);
            State_Run18(10);
            State_Place8(0, 0x4000, 30);
            State_Run19(a0, 0);
            State_Run20(0);
            State_Run21(0, 0);
            State_Place9(33, 0x3480000, 0xe80000);
            State_Run22(a0, 3);
            goto L_020016b0;
        }
        if (rec2 == 1) {
            State_Do2(0x20a5);
            State_Run23(a0, 0);
        }
        L_020016b0:;
        State_Place10(rec2, a0, 3);
        State_Run24();
    }
}
