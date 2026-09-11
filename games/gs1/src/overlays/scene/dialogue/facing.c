#include "types.h"
#include "scene.h"

#include "resource_3ac.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern u8 gCell[];
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 Value_00001be0;
extern u8 Value_00000069;

T *Talk_Run(s32);

T *Talk_unk2_4(s32);

T *Talk_unk3_4(s32);

T *Talk_unk4_4(s32);

T *Talk_unk5_4(s32);

T *Talk_unk6_4(s32);

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned table address 0x020084f8.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x0200865c.
 */
u8 *SceneData_GetTable84f8(void)
{
    return (u8 *)0x020084f8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x02008630.
 */
u8 *SceneData_GetTable8630(void)
{
    return (u8 *)0x02008630;
}

u8 *SceneData_GetTable865c(void)
{
    return (u8 *)0x0200865c;
}

u8 *SceneData_SelectTableBySubStateAndFlag941(void)
{
    s16 *tbl = ((s16 *)gCell);

    if (tbl[225] == 10) {
        return gOv;
    }
    if (Talk_Check(0x941) != 0) {
        return gOv2;
    }
    return gOv3;
}

void Dialogue_RunActor17FlaggedDialogue(void)
{
    T *p;

    p = Talk_Run(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        if (Talk_unk2(0x941) != 0) {
            Talk_Apply(8, 17);
            return;
        }
    }
    Talk_unk7_4();
    if (Talk_unk3(0x941) != 0) {
        Talk_Do(0x24fb);
        Talk_Apply2(17, 0);
    } else {
        Talk_unk2_2(0x1bd0);
        Talk_unk8_4(17, 0);
    }
    Talk_unk9_4();
}

void Dialogue_RunActor20DialogueA(void)
{
    Talk_unk10_4();
    Talk_unk3_2(0x1bd5);
    Talk_Apply3(20, 0);
    Talk_unk4_2(0x940);
    Talk_unk11_4();
}

void Dialogue_RunActor20DialogueB(void)
{
    Talk_unk12_4();
    Talk_unk5_2(0x1bdb);
    Talk_Apply4(20, 0);
    Talk_unk6_2(0x940);
    Talk_unk13_4();
}

void Dialogue_RunActor18Dialogue(void)
{
    Talk_unk14_4();
    Talk_unk7_2(0x24fe);
    Talk_Apply5(18, 0);
    Talk_unk15_4();
}

void Dialogue_RunActor21FlaggedDialogue(void)
{
    T *p;

    p = Talk_unk2_4(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Talk_unk8_2(21);
    } else if (Talk_unk4(0x941) != 0) {
        Talk_unk16_4();
        Talk_unk9_2(0x2507);
        Talk_Apply6(21, 0);
        Talk_unk17_4();
    } else {
        Talk_unk18_4();
        Talk_unk10_2(0x1bdc);
        Talk_Apply7(21, 0);
        Talk_unk5();
    }
}

void Dialogue_RunActor16FlaggedDialogue(void)
{
    if (Talk_unk6(0x941) != 0) {
        Talk_unk19_4();
        Talk_unk11_2(0x24fa);
        Talk_Apply8(16, 0);
        Talk_unk20_4();
    } else {
        Talk_unk21_4();
        Talk_unk12_2((s32)&Value_00001be0);
        Talk_Apply9(16, 0);
        Talk_unk22_4();
    }
}

void Dialogue_RunActor16FacingFlaggedDialogue(void)
{
    T *p;

    p = Talk_unk3_4(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Talk_Apply10(25, 16);
    } else if (Talk_unk7(0x941) != 0) {
        Talk_unk23_4();
        Talk_unk13_2(0x24f9);
        Talk_Apply11(16, 0);
        Talk_unk24_4();
    } else {
        Talk_unk25_4();
        Talk_unk14_2(0x1bcf);
        Talk_Apply12(16, 0);
        Talk_unk26_4();
    }
}

void Dialogue_RunActor14FlaggedDialogue(void)
{
    if (Talk_unk8(0x941) != 0) {
        Talk_unk15_2(0x24f6);
        Talk_unk27_4(14, 0);
    } else {
        Talk_unk16_2(0x1bde);
        Talk_Apply13(14, 0);
    }
}

void Dialogue_RunActor14FacingFlaggedDialogue(void)
{
    T *p;
    s32 v;

    p = Talk_unk4_4(0);
    v = p->unk6;
    if (Talk_unk9(0x941) != 0) {
        if ((u32)(v - 0xa001) <= 0x3ffe) {
            Talk_Apply14(29, 14);
        } else {
            Talk_unk28_4();
            Talk_unk17_2(0x24f5);
            Talk_Apply15(14, 0);
            Talk_unk29_4();
        }
    } else {
        Talk_unk18_2(0x1bcd);
        Talk_Apply16(14, 0);
    }
}

void Dialogue_RunActor15FlaggedDialogue(void)
{
    if (Talk_unk10(0x941) != 0) {
        Talk_unk19_2(0x24f8);
        Talk_unk30_4(15, 0);
    } else {
        Talk_unk20_2(0x1bdf);
        Talk_Apply17(15, 0);
    }
}

void Dialogue_RunActor15FacingFlaggedDialogue(void)
{
    T *p;
    s32 v;

    p = Talk_unk5_4(0);
    v = p->unk6;
    if (Talk_unk11(0x941) != 0) {
        if ((u32)(v - 0xa001) <= 0x3ffe) {
            Talk_Apply18(30, 15);
        } else {
            Talk_unk31_4();
            Talk_unk21_2(0x24f7);
            Talk_Apply19(15, 0);
            Talk_unk32_4();
        }
    } else {
        Talk_unk22_2(0x1bce);
        Talk_Apply20(15, 0);
    }
}

s32 State_SetRuntimeWord448To521(void)
{
    u8 *work;
    s32 *dst;
    u8 *tbl;
    s32 off;
    s32 v;
    s32 n;

    work = *(u8 **)0x03001ebc;
    dst = (s32 *)(work + 0x1c0);
    off = 0x209;
    *dst = off;
    tbl = gCell;
    v = *(s16 *)(tbl + off - 71);
    if (v == 10) {
        Talk_unk23_2(0x12f);
        n = (s32)&Value_00000069;
        *(u16 *)(tbl + 0x1c4) = n;
        *(u16 *)(tbl + 0x1c6) = v;
    }
    Talk_Apply21(Talk_unk12(23), 0);
    Talk_Apply22(Talk_unk13(24), 0);
    Talk_Apply23(Talk_unk14(25), 0);
    return 0;
}

void Dialogue_RunActor22FacingDialogue(void)
{
    T *p;

    p = Talk_unk6_4(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Talk_unk24_2(21);
    } else {
        Talk_unk25_2(0x266b);
        Talk_Apply24(22, 0);
    }
}
