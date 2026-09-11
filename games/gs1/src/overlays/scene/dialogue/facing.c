#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/facing.h"

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

T *Talk_Run2(s32);

T *Talk_Run3(s32);

T *Talk_Run4(s32);

T *Talk_Run5(s32);

T *Talk_Run6(s32);

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
        if (Talk_Check2(0x941) != 0) {
            Talk_Apply(8, 17);
            return;
        }
    }
    Talk_Run7();
    if (Talk_Check3(0x941) != 0) {
        Talk_Do(0x24fb);
        Talk_Apply2(17, 0);
    } else {
        Talk_Do2(0x1bd0);
        Talk_Run8(17, 0);
    }
    Talk_Run9();
}

void Dialogue_RunActor20DialogueA(void)
{
    Talk_Run10();
    Talk_Do3(0x1bd5);
    Talk_Apply3(20, 0);
    Talk_Do4(0x940);
    Talk_Run11();
}

void Dialogue_RunActor20DialogueB(void)
{
    Talk_Run12();
    Talk_Do5(0x1bdb);
    Talk_Apply4(20, 0);
    Talk_Do6(0x940);
    Talk_Run13();
}

void Dialogue_RunActor18Dialogue(void)
{
    Talk_Run14();
    Talk_Do7(0x24fe);
    Talk_Apply5(18, 0);
    Talk_Run15();
}

void Dialogue_RunActor21FlaggedDialogue(void)
{
    T *p;

    p = Talk_Run2(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Talk_Do8(21);
    } else if (Talk_Check4(0x941) != 0) {
        Talk_Run16();
        Talk_Do9(0x2507);
        Talk_Apply6(21, 0);
        Talk_Run17();
    } else {
        Talk_Run18();
        Talk_Do10(0x1bdc);
        Talk_Apply7(21, 0);
        Talk_Check5();
    }
}

void Dialogue_RunActor16FlaggedDialogue(void)
{
    if (Talk_Check6(0x941) != 0) {
        Talk_Run19();
        Talk_Do11(0x24fa);
        Talk_Apply8(16, 0);
        Talk_Run20();
    } else {
        Talk_Run21();
        Talk_Do12((s32)&Value_00001be0);
        Talk_Apply9(16, 0);
        Talk_Run22();
    }
}

void Dialogue_RunActor16FacingFlaggedDialogue(void)
{
    T *p;

    p = Talk_Run3(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Talk_Apply10(25, 16);
    } else if (Talk_Check7(0x941) != 0) {
        Talk_Run23();
        Talk_Do13(0x24f9);
        Talk_Apply11(16, 0);
        Talk_Run24();
    } else {
        Talk_Run25();
        Talk_Do14(0x1bcf);
        Talk_Apply12(16, 0);
        Talk_Run26();
    }
}

void Dialogue_RunActor14FlaggedDialogue(void)
{
    if (Talk_Check8(0x941) != 0) {
        Talk_Do15(0x24f6);
        Talk_Run27(14, 0);
    } else {
        Talk_Do16(0x1bde);
        Talk_Apply13(14, 0);
    }
}

void Dialogue_RunActor14FacingFlaggedDialogue(void)
{
    T *p;
    s32 v;

    p = Talk_Run4(0);
    v = p->unk6;
    if (Talk_Check9(0x941) != 0) {
        if ((u32)(v - 0xa001) <= 0x3ffe) {
            Talk_Apply14(29, 14);
        } else {
            Talk_Run28();
            Talk_Do17(0x24f5);
            Talk_Apply15(14, 0);
            Talk_Run29();
        }
    } else {
        Talk_Do18(0x1bcd);
        Talk_Apply16(14, 0);
    }
}

void Dialogue_RunActor15FlaggedDialogue(void)
{
    if (Talk_Check10(0x941) != 0) {
        Talk_Do19(0x24f8);
        Talk_Run30(15, 0);
    } else {
        Talk_Do20(0x1bdf);
        Talk_Apply17(15, 0);
    }
}

void Dialogue_RunActor15FacingFlaggedDialogue(void)
{
    T *p;
    s32 v;

    p = Talk_Run5(0);
    v = p->unk6;
    if (Talk_Check11(0x941) != 0) {
        if ((u32)(v - 0xa001) <= 0x3ffe) {
            Talk_Apply18(30, 15);
        } else {
            Talk_Run31();
            Talk_Do21(0x24f7);
            Talk_Apply19(15, 0);
            Talk_Run32();
        }
    } else {
        Talk_Do22(0x1bce);
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
        Talk_Do23(0x12f);
        n = (s32)&Value_00000069;
        *(u16 *)(tbl + 0x1c4) = n;
        *(u16 *)(tbl + 0x1c6) = v;
    }
    Talk_Apply21(Talk_Check12(23), 0);
    Talk_Apply22(Talk_Check13(24), 0);
    Talk_Apply23(Talk_Check14(25), 0);
    return 0;
}

void Dialogue_RunActor22FacingDialogue(void)
{
    T *p;

    p = Talk_Run6(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Talk_Do24(21);
    } else {
        Talk_Do25(0x266b);
        Talk_Apply24(22, 0);
    }
}
