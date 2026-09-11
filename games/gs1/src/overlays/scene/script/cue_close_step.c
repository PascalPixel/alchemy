#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/cue_close_step.h"

extern u8 gWork[];

void Scene_RunCue(void)
{
    u32 i;
    s32 record;

    Script_Run3();
    Script_Run4();
    Script_Run5();
    Script_Place(19, 0x3000, 0);
    Script_Place2(0, 0x9999, 0x4ccc);
    Script_Place3(0, 0x100, 0x294);
    Script_Run6(20);
    Script_Run(-1, -1, -1, 0);
    Script_Do(0x200);
    Script_Run7(188);
    Script_Run8(1);
    Script_Run9(2);
    Script_Place4(19, 0x1000000, 0x2780000);
    Script_Run10(1);
    Script_Place5(19, 0x9999, 0x4ccc);
    Script_Place6(19, 0x100, 0x284);
    Script_Run11(1);
    Script_Run12(2);
    Script_Run13(20);
    Script_Run14(19, 2);
    Script_Do2(0x145e);
    Script_Run15(19, 0, 10);
    Script_Place7(0, 0x100, 40);
    Script_Place8(0, 0x108, 0x294);
    Script_Place9(0, 0x8000, 0);
    Script_Place10(19, 248, 0x294);
    Script_Place11(19, 0x1000, 40);
    Script_Run16(19, 4);
    Script_Run17(19, 0);
    Script_Run18(19, 3);
    Script_Check(19, 0);
    Script_Run19(19, 2);
    Script_Run20(19, 0, 10);
    Script_Place12(0, 0x101, 60);
    Script_Run2(19, 0x102);
    Script_Run21(60);
    Script_Run22(19, 1);
    Script_Run23(19, 0, 10);
    Script_Run24(19, 3);
    Script_Run25(19, 0);
    Script_Place13(19, 0xcccc, 0x6666);
    Script_Place14(19, 248, 0x304);
    Script_Run26(19, 0, 0);
    Script_Do3(0x12f);
    Script_Do4(0x84f);
    Script_Run27();
}
