#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/script/gate_close_step.h"

extern u8 gWork[];

void Scene_RunGate(void)
{
    u32 i;
    s32 record;

    Script_Run6();
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Script_Run7();
    ((void (*)())Script_Check7)(0, 0);
    Script_Run8(4);
    Script_Run(-1, -1, -1, 0);
    Script_Run2(0x9999, 0x1333);
    Script_Run3(0x4c80000, -1, 0x880000, 1);
    record = Script_Check(0);
    if (record != 0) {
        Script_Run9(8, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Script_Check2(0);
    if (record != 0) {
        Script_Run10(5, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Script_Check3(0);
    if (record != 0) {
        Script_Run11(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Script_Place(8, 0x9999, 0x4ccc);
    Script_Place2(5, 0x9999, 0x4ccc);
    Script_Place3(1, 0x9999, 0x4ccc);
    Script_Run12(1, 2);
    Script_Run13(5, 2);
    Script_Run14(8, 2);
    Script_Place4(1, -16, 0);
    Script_Run15(5, 16, 0);
    Script_Place5(8, 0, -32);
    Script_Run16(1);
    Script_Run17(1, 0);
    Script_Run18(5, 0);
    Script_Place6(1, 0xc000, 0);
    Script_Place7(5, 0xc000, 0);
    Script_Run19(8);
    Script_Run20(8, 1);
    Script_Run21(40);
    Script_Run22(8, 2);
    Script_Run23(20);
    Script_Place8(8, 0x3000, 40);
    Script_Place9(8, 0x5000, 40);
    Script_Place10(8, 0x3000, 20);
    Script_Run24(8, 4, 20);
    Script_Do(0xfd3);
    Script_Run4(0x4008, 0);
    Script_Run25(20);
    Script_Run5(0x4c80000, -1, 0x940000, 1);
    Script_Run26(1, 2);
    record = Script_Check4(0);
    if (record != 0) {
        Script_Run27(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_Run28(5, 2);
    record = Script_Check5(0);
    if (record != 0) {
        Script_Run29(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_Run30(8, 2);
    record = Script_Check6(0);
    if (record != 0) {
        Script_Run31(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_Run32(1);
    Script_Run33(1, 0, 0);
    Script_Run34(5, 0, 0);
    Script_Run35(8);
    Script_Run36(8, 0, 0);
    Script_Run37(1, 1);
    Script_Run38(5, 1);
    Script_Run39(8, 1);
    Script_Do2(0x802);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    Script_Do3(0x12f);
    Script_Run40();
}
