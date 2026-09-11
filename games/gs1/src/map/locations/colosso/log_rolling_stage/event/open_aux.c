#include "types.h"
#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/open_aux.h"

extern u8 gCell[];
extern u8 gOv[];
extern u8 gWork[];
extern s16 gCell2[][1];

void Scene_RunOpeningAuxiliarySequence(s32 a0)
{
    s32 i;
    s32 rec2;
    s32 rec7;
    s32 record;

    if (gCell2[225][0] == 2) {
        Map_Run5();
    } else {
        Map_Run6();
        rec2 = Map_Check(a0, 5);
        if (rec2 != 0) {
        } else {
            Map_Do(0x20c3);
            Map_Run(0x30000, 0x6000);
            Map_Run2(0x4380000, -1, 0xa80000, 1);
            Map_Run7();
            Map_Run8(30);
            Map_Run9(a0, 0);
            Map_Run10(a0, 0);
            Map_Place(0, 0x3d8, 184);
            Map_Place2(0, 0x18000, 0xc000);
            Map_Run11(0, 0x3e0, 184);
            Map_Place3(0, 0x4ccc, 0x2666);
            Map_Place4(0, 0x460, 184);
            Map_Run12(120);
            Map_Run3(0, 0x101);
            Map_Run13(120);
            Map_Run14(0);
            Map_Run15(0, 1);
            Map_Run4(0, 0x100);
            Map_Place5(0, 0x105, 0);
            rec7 = Map_Check2(0);
            for (i = 119; i >= 0; i--) {
                if (*(s32 *)(rec7 + 8) > 0x3e00000) {
                    *(s32 *)(rec7 + 8) += -0x13333;
                }
                Map_Run16(1);
            }
            Map_Place6(0, 0x103, 60);
            Map_Place7(0, 0x460, 184);
            Map_Run17(a0, 0);
            Map_Run18(0);
            {
                u8 *flag = (u8 *)gCell2;

                flag[498] = 1;
            }
            Map_Run19(0);
            Map_Run20(0, 0);
            Map_Run21(a0, 5);
            goto L_02002494;
        }
        if (rec2 == 1) {
            Map_Do2(0x20c2);
            Map_Run22(a0, 0);
        }
        L_02002494:;
        Map_Place8(rec2, a0, 5);
        Map_Run23();
    }
}
