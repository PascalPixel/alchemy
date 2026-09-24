#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define Scene_GetRecord_1(a0) Call1(Func_0200349c, a0)
#define TARGET_ID 9
#define GATE_CODE 2059

struct Ent {
    s32 f00;
    u8 f04;
    u8 f05;
    u16 f06:9;
    u16 f07:7;
    s32 f08;
};

struct Cam {
    u8 pad[228];
    s32 x;
    s32 y;
};

extern u8 SceneEventRuntime_ScriptData[];
extern u8 SceneEventRuntime_MessageData[];
extern u8 SceneEventRuntime_ActorData[];
extern u8 SceneEventRuntime_EffectData[];
extern struct Cam *Data_03001e70;
extern s32 Data_0200a974;
extern u16 Data_02000240_t[][1];
extern s32 Data_0200a980[];

void Func_0200181c(void);
void Func_02003318(struct Ent *, s32);
void Func_02003358(struct Ent *, s32);
void Func_0200339c(struct Ent *, s32);
u16 *Func_02003456(s32);
u16 *Func_02003472(s32);
void Func_0200349c();
void Func_0200252c();
s32 Func_020034ee();
u8 *Func_02003506();
s32 Func_02003512();
s32 Func_02003890();
void Func_020038aa();
s32 Func_020038be();
s32 Func_020038e6();
s32 Func_02003910();
void Func_02003920();
void Func_020039d4();
void Func_020039e0();
void Func_02003a78();
void Func_02003a88();
s32 Func_02003a96();
s32 Func_02003aa0();
s32 Func_02003dce();
s32 Func_02003dfe();
s32 Func_02003e16();
void Func_0200353a(s32, s32, s32, s32, s32);
void Func_0200360e(void);
void Func_0200355a(s32, s32, s32, s32, s32);
void Func_0200372e_a(void);
void Func_0200359a(s32, s32, s32, s32, s32);
void Func_02003966(void);
void Func_02003a08(s32, s32, s32, s32);
void Func_02003a30();
void Func_0200357a_a(s32, s32, s32, s32, s32);
void Func_0200384a(void);
void Func_020035ba_a(s32, s32, s32, s32, s32);
void Func_02003cbe(void);
void Func_0200362a(s32, s32, s32, s32, s32);
void Func_02003dde(void);
u8 *Func_02003ef8(s32 index);
void Func_020035e6();
s32 *Func_02003f68(s32);
void Func_02003656(s32, s32, s32, s32, s32);
s32 *Func_0200405c(s32);
s32 *Func_02003f20(s32);
void Func_0200360e_a(s32, s32, s32, s32, s32);
s32 *Func_02003f90(s32);
void Func_0200367e(s32, s32, s32, s32, s32);
void Func_02003cec(void);
void Func_02003de6(void);
void Func_02003ee4(void);
void Func_02003fcc(void);

/* Scene-record accessor. */

/* Old-style: the interface is not established, and the call site passes five
 * arguments. */

/* Argument shared by every configuration call below. */

/* Condition code passed to the gating check; the body below runs only when
 * that check reports 0. */

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z);

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200105c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1_020014b8(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02001624(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1_0200195c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1_020019e4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    Func_02003a08(actor, x, y, mode);
}

static __inline__ void ConfigureScene_02003a30(s32 actor, s32 x, s32 y, s32 mode)
{
    Func_02003a30(actor, x, y, mode);
}

u8 *SceneEventRuntime_GetScriptData(void)
{
    return SceneEventRuntime_ScriptData;
}

s32 SceneEventRuntime_ReturnZero(void)
{
    return 0;
}

u8 *SceneEventRuntime_GetMessageData(void)
{
    return SceneEventRuntime_MessageData;
}

u8 *SceneEventRuntime_GetActorData(void)
{
    return SceneEventRuntime_ActorData;
}

u8 *SceneEventRuntime_GetEffectData(void)
{
    return SceneEventRuntime_EffectData;
}

s32 SceneEventRuntime_SelectInitialSceneByFlags(void)
{
    s32 no;

    if (GameFlag_IsSet(0x818) != 0) {
        if (GameFlag_IsSet(0x813) == 0) {
            no = 3;
            goto apply;
        }
        goto fail;
    }
    if (GameFlag_IsSet(0x812) == 0) {
        no = 4;
apply:
        Event_RequestExit(no);
        return 1;
    }
fail:
    return -1;
}

void FieldScene_RunLoopedLayoutSequence(void)
{
    s32 i;

    { s32 k5 = 2, k6 = 1; Map_CopyCellsTo(0, 28, 17, 8, k5, k6); }
    Audio_PlayCue(200);
    for (i = 0; i != 22; i++) {
        Map_CopyCellsTo(10, 61, 17, 40, 2, 1);
        Event_Wait(4);
        Map_CopyCellsTo(8, 61, 17, 40, 2, 1);
        Event_Wait(4);
    }
    { s32 k5 = 4, k6 = 3;
      Map_CopyCellsTo(0, 59, 15, 38, k5, k6);
      Map_CopyCellsTo(4, 59, 17, 38, k5, k6); }
    Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
    { s32 k5 = 17, k6 = 8; Map_CopyCellAttributes(0, 0, 2, 1, k5, k6); }
    GameFlag_Set(0x207);
    Func_0200181c();
}

void SceneEffect_UpdateScrollingSpriteRows(void)
{
    extern struct Ent Data_0200aa50[];

    s32 *cp = &Data_03001e70->x;
    struct Ent *e = Data_0200aa50;
    s32 sx = cp[0] / 65536;
    s32 sy = 80 - cp[1] / 65536;
    s32 v;
    u32 i;

    if ((u32)(sy + 16) <= 175) {
        v = (Data_0200a974 >> 10) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Func_02003318(e, 0);
                v += 32;
                e++;
            }
        }
        v = (Data_0200a974 >> 9) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Func_02003358(e, 0);
                v += 32;
                e++;
            }
        }
        v = (Data_0200a974 >> 8) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy + 8;
                Func_0200339c(e, 0);
                v += 32;
                e++;
            }
        }
    }
    Data_0200a974 += 0x80;
}

void SceneState_RunWhenSlotZeroFacingC000(void)
{
    u16 *p = Func_02003456(0);
    if (p[3] == 0xc000) {
        Leader_CheckAhead();
    }
}

void SceneState_RunWhenActorZeroFacing4000(void)
{
    u16 *p = Func_02003472(0);
    if (p[3] == 0x4000) {
        Leader_CheckAhead();
    }
}

/* If the code-2059 check passes, runs a short setup/configuration sequence
 * for id 9: two no-argument calls bracket a select call and two calls each
 * taking a pair of numeric arguments. */
void FieldScene_RunPrimarySequenceHead(void)
{
    if (GameFlag_IsSet(GATE_CODE) == 0) {
        Event_Begin();
        Scene_GetRecord_1(TARGET_ID);
        Actor_SetSpeed(TARGET_ID, 13107, 0x00001999); /* object_id, speed_limit, acceleration */
        Actor_WalkToAndWait(TARGET_ID, 504, 152); /* object_id, x=504, z=152 */
        Event_End();
    }
}

s32 Func_0200105c(void)
{
    u8 *record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Func_0200252c();
    GameFlag_Set(0x144);
    record = (u8 *)Value1(Func_020034ee, 18);
    record[89] = 0;
    {
        u8 flags = record[35] | 2;

        record[35] = flags;
    }
    Actor_SetSpriteFlags((s32)Func_02003506(18), 0);
    *(u8 *)(Func_02003512(18) + 35) &= 254;
    Actor_SetSpritePriority(18, 1);
    if ((u32)((Data_02000240_t[225][0] - 3) << 16) > 0x10000) {
        Actor_SetPosition(5, 0, 0);
        Actor_SetPosition(1, 0, 0);
    }
    if (GameFlag_IsSet(0x818) != 0) {
        Actor_SetPosition(18, 0x1200000, 0xb20000);
        Actor_SetPosition(17, 0x6480000, 0x6480000);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
        Map_CopyCellAttributes(0, 1, 2, 1, 17, 7);
    } else if (GameFlag_IsSet(0x816) != 0
                && GameFlag_IsSet(0x817) != 0) {
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(0, 28, 17, 8, 2, 1);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
        Map_CopyCellAttributes(0, 0, 2, 1, 17, 8);
    } else {
        if (GameFlag_IsSet(0x816) != 0) {
            Actor_SetPosition(10, 0xe80000, 0x780000);
            Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        }
        if (GameFlag_IsSet(0x817) != 0) {
            Actor_SetPosition(12, 0x1580000, 0x780000);
            Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        }
    }
    if (GameFlag_IsSet(0x80b) != 0) {
        Actor_SetPosition(9, 0x1f80000, 0x980000);
        Map_CopyCellsTo(2, 28, 34, 10, 2, 1);
        Map_CopyCellsTo(2, 30, 16, 10, 2, 1);
        Map_CopyCellsTo(0, 55, 32, 40, 4, 3);
    }
    if (GameFlag_IsSet(0x80c) != 0) {
        Actor_SetPosition(11, 0x2880000, 0x980000);
        Map_CopyCellsTo(4, 28, 36, 10, 2, 1);
        Map_CopyCellsTo(4, 30, 18, 10, 2, 1);
        Map_CopyCellsTo(4, 55, 36, 40, 4, 3);
    }
    if (GameFlag_IsSet(0x80d) != 0) {
        Actor_SetPosition(13, 0x1f80000, 0xc80000);
        Map_CopyCellsTo(2, 29, 34, 11, 2, 1);
        Map_CopyCellsTo(2, 31, 16, 11, 2, 1);
        Map_CopyCellsTo(0, 58, 32, 43, 4, 1);
    }
    if (GameFlag_IsSet(0x80e) != 0) {
        Actor_SetPosition(15, 0x2880000, 0xc80000);
        Map_CopyCellsTo(4, 29, 36, 11, 2, 1);
        Map_CopyCellsTo(4, 31, 18, 11, 2, 1);
        Map_CopyCellsTo(4, 58, 36, 43, 4, 1);
    }
    {
    s16 *state = (s16 *)Data_02000240_t;

    if (state[225] == 3) {
        if (GameFlag_IsSet(0x30a) != 0) {
            Actor_SetPosition(1, 0, 0);
            Actor_SetPosition(5, 0, 0);
        } else if (GameFlag_IsSet(0x109) == 0) {
            FieldScene_SetupStagedActors();
            GameFlag_Set(0x30a);
        }
    }
    if (state[225] == 4) {
        if (GameFlag_IsSet(0x30b) != 0) {
            Actor_SetPosition(1, 0, 0);
            Actor_SetPosition(5, 0, 0);
        } else if (GameFlag_IsSet(0x109) == 0) {
            FieldScene_RunStagedActorScene();
            GameFlag_Set(0x30b);
        }
    }
    }
    if (GameFlag_IsSet(0x814) != 0) {
        Func_020039e0(141);
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        Func_020039d4();
    }
    return 0;
}

void Func_020014b8(void)
{
    extern u8 Data_0200aa50[];

    u32 i;
    s32 value;
    s32 *p;
    s32 buf;

    p = (s32 *)Data_0200aa50;
    buf = Value2(Func_02003890, 14, 0x400);
    Call2(Func_020038aa, 0x200a56c, buf);
    value = Vram_Load(Func_020038be(), 128, buf);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xac00;
    }
    value = Vram_Load(Func_020038e6(), 128, buf + 128);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xdc00;
    }
    value = Vram_Load(Func_02003910(), 128, buf + 0x100);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xc00;
    }
    Heap_Release(14);
    {
        s32 size = 0xc80;

        Func_02003920(0x2008eb1, size);
    }
}

void FieldScene_CallWhenCheck9_31_9(void)
{
    if (SceneActor_IsActorAtTile(9, 31, 9) != 0) {
        SceneData_InitTableA980();
    }
}

void FieldScene_RunGuardedStep11(void)
{
    if (SceneActor_IsActorAtTile(11, 40, 9) != 0) {
        SceneData_FillTableA980();
    }
}

void FieldScene_RunGuardedStep13(void)
{
    if (SceneActor_IsActorAtTile(13, 31, 12) != 0) {
        SceneData_InitTableA980AndRunB();
    }
}

void FieldScene_RunGuardedStep15(void)
{
    if (SceneActor_IsActorAtTile(15, 40, 12) != 0) {
        SceneData_BuildTableA980();
    }
}

void ConfigureSceneAndCheckActors(void)
{
    ConfigureScene(2, 0x00d00000, 0x00700000, 0);
    if (SceneActor_IsActorAtTile(10, 14, 7) != 0) {
        FieldScene_RunScene37b_02002244();
    }
}

void Func_020015fc(void)
{
    ConfigureScene_02003a30(2, 23068672, 7340032, 0);
    if (SceneActor_IsActorAtTile(12, 21, 7) != 0) {
        FieldScene_RunScene37b_020022f4();
    }
}

void FieldScene_RunClosingSequence(void)
{
    s32 first;
    s32 kind;
    s32 second;

    first = Value1_02001624(Func_02003a96, 0);
    kind = *(s32 *)(first + 8) >> 20;
    second = Value1_02001624(Func_02003aa0, 0);
    if ((*(s32 *)(second + 16) >> 20) == 8) {
        if ((u32)(kind - 17) <= 1) {
            Call4(Func_02003a78, 2, 0x1100000, 0x800000, 255);
            Call4(Func_02003a88, 2, 0x1200000, 0x800000, 255);
        }
    }
}

void FieldScene_RunScene37bSequenceA(void)
{
    u32 i;
    s32 record;

    record = Value1_0200195c(Func_02003dce, 17);
    if (record != 0) {
        if ((*(s32 *)(record + 16) >> 20) == 8) {
            Event_Begin();
            Audio_PlayCue(185);
            Actor_SetSpeed(17, 0x3333, 0x1999);
            Actor_SetSpeed(0, 0x3333, 0x1999);
            *(u8 *)(Func_02003dfe(17) + 90) &= 254;
            Actor_SetAnimation(0, 8);
            record = Func_02003e16(0);
            Actor_SetDestination(0, *(s16 *)(record + 10), 136);
            Actor_SetDestination(17, 0x120, 120);
            Actor_WaitForMove(17);
            Actor_SetAnimation(0, 1);
            Event_End();
        }
    }
}

void FieldScene_RunFiveValueStep9(void)
{
    Func_0200353a(9, 31, 9, 30, 9);
    Func_0200360e();
}

void FieldScene_RunFiveValueStep11(void)
{
    Func_0200355a(11, 40, 9, 41, 9);
    Func_0200372e_a();
}

void FieldScene_ApplyRect13_31_12_30_12(void)
{
    Func_0200357a_a(13, 31, 12, 30, 12);
    Func_0200384a();
}

void FieldScene_RunFiveValueStep15(void)
{
    Func_0200359a(15, 40, 12, 41, 12);
    Func_02003966();
}

void FieldScene_ApplyRect10_14_7_13_7(void)
{
    Func_020035ba_a(10, 14, 7, 13, 7);
    Func_02003cbe();
}

/*
 * Fetches scene record 10 and, when it exists, hands a coarse coordinate
 * derived from it to a five-argument routine, which receives both the
 * coordinate and the coordinate plus one; the fifth argument travels on the
 * stack. The `>> 20` reduction to a cell index is by analogy with the rest of
 * the tree and is not verified, and the repeated 13 is as written.
 */
void SceneActor_UseActorTenCellAndNext(void)
{
    u8 *record = Func_02003ef8(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = *(s32 *)(record + 16) >> 20;
    Func_020035e6(10, 13, cell + 1, 13, cell);
}

void SceneActor_MoveActor10ByRow(void)
{
    s32 *p = Func_02003f20(10);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_0200360e_a(10, 13, v - 1, 13, v);
    }
}

void FieldScene_ApplyRect12_21_7_22_7(void)
{
    Func_0200362a(12, 21, 7, 22, 7);
    Func_02003dde();
}

void SceneActor_ApplyActorTwelveZCellPair(void)
{
    s32 *p = Func_02003f68(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_02003656(12, 22, v + 1, 22, v);
    }
}

void SceneActor_RunSlot12ColumnStep(void)
{
    s32 *p = Func_02003f90(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_0200367e(12, 22, v - 1, 22, v);
    }
}

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z)
{
    s32 *p = Func_0200405c(no);
    if (p == NULL || (p[2] >> 20) != x) {
        return 0;
    }
    if ((p[4] >> 20) != z) {
        return 0;
    }
    return 1;
}

void SceneData_InitTableA980(void)
{
    s32 *p = Data_0200a980;
    p[0] = 0;
    p[1] = 55;
    p[2] = 32;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 2;
    p[7] = 30;
    p[8] = 34;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 28;
    p[14] = 34;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 30;
    p[20] = 16;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80b;
    p[25] = 0x4000;
    p[26] = 500;
    p[27] = 132;
    p[28] = 8;
    p[29] = 55;
    p[30] = 32;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 2;
    p[35] = 30;
    p[36] = 34;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 28;
    p[42] = 16;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 9;
    p[47] = 488;
    p[48] = 152;
    Func_02003cec();
}

void SceneData_FillTableA980(void)
{
    s32 *p = Data_0200a980;
    p[0] = 4;
    p[1] = 55;
    p[2] = 36;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 4;
    p[7] = 30;
    p[8] = 36;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 28;
    p[14] = 36;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 30;
    p[20] = 18;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80c;
    p[25] = 0x4000;
    p[26] = 654;
    p[27] = 132;
    p[28] = 12;
    p[29] = 55;
    p[30] = 36;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 4;
    p[35] = 30;
    p[36] = 36;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 28;
    p[42] = 18;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 11;
    p[47] = 664;
    p[48] = 152;
    Func_02003de6();
}

void SceneData_InitTableA980AndRunB(void)
{
    s32 *p = Data_0200a980;
    p[0] = 0;
    p[1] = 58;
    p[2] = 32;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 2;
    p[7] = 31;
    p[8] = 34;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 29;
    p[14] = 34;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 31;
    p[20] = 16;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80d;
    p[25] = 0xc000;
    p[26] = 500;
    p[27] = 216;
    p[28] = 8;
    p[29] = 58;
    p[30] = 32;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 2;
    p[35] = 31;
    p[36] = 34;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 29;
    p[42] = 16;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 13;
    p[47] = 488;
    p[48] = 200;
    Func_02003ee4();
}

void SceneData_BuildTableA980(void)
{
    s32 *p = Data_0200a980;
    p[0] = 4;
    p[1] = 58;
    p[2] = 36;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 4;
    p[7] = 31;
    p[8] = 36;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 29;
    p[14] = 36;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 31;
    p[20] = 18;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80e;
    p[25] = 0xc000;
    p[26] = 0x28e;
    p[27] = 216;
    p[28] = 12;
    p[29] = 58;
    p[30] = 36;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 4;
    p[35] = 31;
    p[36] = 36;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 29;
    p[42] = 18;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 15;
    p[47] = 664;
    p[48] = 200;
    Func_02003fcc();
}

void FieldScene_RunScene37b_02002244(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x818) == 0) {
        if (GameFlag_IsSet(0x816) == 0) {
            Camera_SetSpeed(0x20000, 0x4000);
            Camera_MoveTo(0x11e0000, -1, 0x920000, 1);
            Camera_WaitForMove();
            Audio_PlayCue(186);
            Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
            if (GameFlag_IsSet(0x817) != 0) {
                Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
            }
            Actor_FaceDirection(0, 0, 0);
            Event_Wait(30);
            GameFlag_Set(0x816);
            if (GameFlag_IsSet(0x817) != 0) {
                FieldScene_RunLoopedLayoutSequence();
            }
        }
    }
    Event_End();
}

void FieldScene_RunScene37b_020022f4(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x818) == 0) {
        if (GameFlag_IsSet(0x817) == 0) {
            Camera_SetSpeed(0x20000, 0x4000);
            Camera_MoveTo(0x11e0000, -1, 0x920000, 1);
            Camera_WaitForMove();
            Audio_PlayCue(186);
            Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
            if (GameFlag_IsSet(0x816) != 0) {
                Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
            }
            Actor_FaceDirection(0, 0x8000, 0);
            Event_Wait(30);
            GameFlag_Set(0x817);
            if (GameFlag_IsSet(0x816) != 0) {
                FieldScene_RunLoopedLayoutSequence();
            }
        }
    }
    Event_End();
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Event_Wait(frames);
}
