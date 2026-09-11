#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/task/run_scene_late_sequence.h"

extern u8 gVal[];

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};
extern struct ModeRecord gOv;
extern struct ModeRecord gOv2;
extern u8 gCell[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gWork[];
extern u8 gCell2[][2];

u8 *State_Run2();

u8 *State_Run3();

void Scene_RunLateSequence(s32 a0)
{
    s32 kind;

    State_Run4(247);
    State_Run5();
    State_Run6();
    gOv.span = a0 * 60;
    gOv2.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        State_Run7(30);
        State_Run8(86);
        State_Run9(8);
        State_Check(3, 1);
        State_Run10(-a0 * 60 + 60);
        kind = 0;
    } else {
        State_Run11(30);
        State_Run12(a0 + 90);
        State_Run13(4);
        State_Check2(3, 0);
        State_Run14(a0 * 60 + 60);
        kind = 8;
    }
    State_Place(kind, 0x105, 0);
    while (State_Run()!= 0) {
        State_Run15(1);
    }
    State_Run16(19);
    State_Run17(30);
    State_Do(0x121);
    State_Run18();
    State_Run19();
}

void State_Run20(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p8b;
    u8 *p9;
    s32 p9b;
    u8 *rec;
    s32 record;
    s32 v5;
    s32 base6_4000208;
    s32 v1;
    s32 base4_2002090;
    s32 v4;
    s32 v0;
    s32 slot0;
    u8 *p6;

    p6 = *(volatile s32 *)0x03001e68;
    p11 = a0;
    p8 = a1;
    p10 = a2;
    rec = State_Run2();
    p6[6] = 1;
    p6[7] = 4;
    *(volatile s32 *)gOv6 = *(volatile s32 *)((s32)rec + 8);
    *(volatile s32 *)gOv4 = *(volatile s32 *)((s32)rec + 16);
    p9 = *(volatile s32 *)((s32)rec + 80);
    *(volatile s32 *)gOv5 = *(volatile u16 *)((s32)rec + 6);
    v5 = 1;
    State_Run21(p11, 2);
    {
        u8 value = *(volatile u8 *)&rec[35];

        rec[35] = (u8)(value | v5);
    }
    {
        s32 shown = 0x4000;

        *(volatile u16 *)((s32)rec + 6) = shown;
    }
    State_Run22((s32)rec, 3);
    State_Run23((s32)rec, 0);
    State_Run24((s32)rec, 1);
    p10b = ((s32)p10 << 16);
    p8b = ((s32)p8 << 16);
    State_Run25(p11, p8b, p10b);
    State_Run26(0, 0x4000, 0);
    base6_4000208 = 0x4000208;
    base4_2002090 = (s32)gOv3;
    v1 = *(volatile u16 *)base6_4000208;
    *(volatile u16 *)base6_4000208 = base6_4000208;
    if (*(volatile u16 *)base4_2002090 <= 31) {
        *(volatile u16 *)base4_2002090 += 1;
        *(volatile s32 *)((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4)) = 0xf00;
        *(volatile s32 *)(((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4) + 4)) = 0x4000050;
        *(volatile s32 *)(((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4) + 4) + 4) = 0x20000;
    }
    *(volatile u16 *)base6_4000208 = v1;
    p9[5] = ((s32)(-13 & p9[5]) | 4);
    p9[17] = ((s32)(-13 & p9[17]) | 4);
    slot0 = base4_2002090;
    v5 = 0;
    State_Run27(252);
    v4 = slot0;
    do {
        *(volatile s32 *)((s32)rec + 24) = ((v5 << 12) + 0x1000);
        *(volatile s32 *)((s32)rec + 28) = (0x1f000 - (v5 << 12));
        v0 = *(volatile u16 *)base6_4000208;
        *(volatile u16 *)base6_4000208 = base6_4000208;
        if (*(volatile u16 *)(v4) <= 31) {
            *(volatile u16 *)(v4) += 1;
            *(volatile s32 *)(((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4)) = (((15 - v5) << 8) | (v5 + 1));
            *(volatile s32 *)((((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4) + 4)) = 0x4000052;
            *(volatile s32 *)((((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4) + 4) + 4) = 0x20000;
        }
        *(volatile u16 *)base6_4000208 = v0;
        slot0 = v4;
        State_Run28(1);
        v5 = (v5 + 2);
        v4 = slot0;
    } while (v5 <= 15);
    v4 = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0x4000208;
    if (*(volatile u16 *)gOv3 <= 31) {
        *(volatile u16 *)gOv3 += 1;
        *(volatile s32 *)((((((*(volatile u16 *)gOv3 << 1) + *(volatile u16 *)gOv3) << 2) + 0x2002090) + 4)) = 16;
        *(volatile s32 *)(((((((*(volatile u16 *)gOv3 << 1) + *(volatile u16 *)gOv3) << 2) + 0x2002090) + 4) + 4)) = 0x4000052;
        *(volatile s32 *)(((((((*(volatile u16 *)gOv3 << 1) + *(volatile u16 *)gOv3) << 2) + 0x2002090) + 4) + 4) + 4) = 0x20000;
    }
    *(volatile u16 *)0x04000208 = v4;
    *(volatile s32 *)((s32)rec + 24) = 0x11000;
    *(volatile s32 *)((s32)rec + 28) = 0xf000;
    State_Run29(1);
    *(volatile s32 *)((s32)rec + 24) = 0x10000;
    *(volatile s32 *)((s32)rec + 28) = 0x10000;
    State_Run30(13);
    p9[5] &= -13;
    p9[17] &= -13;
    State_Run31(p11, 3);
    State_Run32(20);
    p9b = v5;
}

void State_Run33(s32 a0)
{
    u32 i;
    u8 *rec7;
    s32 record;
    u8 *p7;

    p7 = *(volatile s32 *)0x03001e68;
    rec7 = State_Run3();
    if (gCell2[249][0] == 1) {
        gCell2[249][0] = 0;
        State_Run34(a0, 1);
    } else {
        State_Place2(a0, 0x4000, 30);
        State_Run35(a0, 3);
        State_Run36(30);
    }
    p7[7] = 0;
    p7[6] = 15;
    *(volatile s32 *)((s32)rec7 + 8) = *(volatile s32 *)gOv6;
    *(volatile s32 *)((s32)rec7 + 16) = *(volatile s32 *)gOv4;
    *(volatile u16 *)((s32)rec7 + 6) = *(volatile s32 *)gOv5;
    *(volatile s32 *)((s32)rec7 + 56) = -0x80000000;
    *(volatile s32 *)((s32)rec7 + 64) = -0x80000000;
    *(volatile s32 *)((s32)rec7 + 36) = 0;
    *(volatile s32 *)((s32)rec7 + 44) = 0;
    rec7[85] = 3;
    rec7[34] = (s32)gVal;
    *(volatile s32 *)((s32)rec7 + 12) = 0;
    *(volatile s32 *)((s32)rec7 + 20) = 0;
    State_Run37((s32)rec7, 1);
    State_Run38((s32)rec7, 0);
    State_Run39((s32)rec7, 1);
    State_Run8(1);
}
