#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000031
#define SecondaryRuntimeSelector Value_00000030
#define TertiaryRuntimeSelector Value_0000002f
#define PrimaryOverlayData Data_020089ec
#define SecondaryOverlayData Data_02008a64
#define TertiaryOverlayData Data_02008b24
#define DefaultOverlayData Data_020089bc

#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

extern s16 Data_02000240[];
extern unsigned char Value_00000031;
extern unsigned char Value_00000030;
extern unsigned char Value_0000002f;
extern s32 Data_02009064[]; /* Packed direction steps, 16 entries. */
extern u8 Data_02008c5c[];
extern u8 Data_02008cbc[];
extern u8 Data_02008c14[];
extern u8 Data_02008efc[];
extern u8 Data_02008f80[];
extern u8 Data_02008e9c[];

s32 Func_020011c0();
void Func_020011ca();
void Func_020011dc();
void Func_020011ea();

/*
 * Table getter at 0x02000098. The eight-byte owner includes its one pool word
 * at 0x0200009c, which holds 0x02008bcc; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/* 0x02000e9a serves two imports in sibling arms: the three-argument setter in
 * the first and the one-argument record accessor in the second. */

/*
 * Data_02009064 is indexed by heading >> 12, X step in the high halfword and Z
 * step in the low, promoted back to 16.16 by masking and shifting rather than
 * by multiply. The s16 at +0x0a and +0x12 are the integer parts of the 16.16
 * words at +0x08 and +0x10, so the tile lookup takes tile coordinates.
 */

/*
 * Push the actor one step ahead of the player. The 268-byte owner at
 * 0x020007f8 includes its alignment halfword and its three pool words.
 * `blocker` is zero wherever it is stored at +0x24 and +0x2c, but it is the
 * register the reference stores, so it stays spelled as itself.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */

/* Deliberate no-op callback. */

/* Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call instead precomputes a costly constant
 * into a pseudo shared with later uses in the block. A value-returning call
 * sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

#undef PrimaryOverlayData
#define PrimaryOverlayData Data_02008c2c
#undef SecondaryOverlayData
#define SecondaryOverlayData Data_02008c5c
#undef TertiaryOverlayData
#define TertiaryOverlayData Data_02008cbc
#undef DefaultOverlayData
#define DefaultOverlayData Data_02008c14

extern u8 Data_02008c2c[];

#undef PrimaryOverlayData
#define PrimaryOverlayData Data_02008ea8
#undef SecondaryOverlayData
#define SecondaryOverlayData Data_02008efc
#undef TertiaryOverlayData
#define TertiaryOverlayData Data_02008f80
#undef DefaultOverlayData
#define DefaultOverlayData Data_02008e9c

extern u8 Data_02008ea8[];

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{

    Actor_SetSpriteFlags(no, 0);
    return 0;
}

s32 SceneData_SelectByRuntimeSelector(void)
{

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)Data_020089ec;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)Data_02008a64;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)Data_02008b24;
    }
    return (s32)Data_020089bc;
}

s32 Func_02000094(void)
{

    return 0;
}

u8 *SceneData_GetTable8bcc(void)
{

    return (u8 *)0x02008bcc;
}

s32 SceneData_SelectSecondaryDataByRuntimeSelector(void)
{

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)Data_02008c2c;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)Data_02008c5c;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)Data_02008cbc;
    }
    return (s32)Data_02008c14;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)Data_02008ea8;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)Data_02008efc;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)Data_02008f80;
    }
    return (s32)Data_02008e9c;
}

void SceneState_ConfigureRegion1_0_21x14(void)
{

    s32 w = 21;
    s32 h = 14;

    Map_CopyCellAttributes(1, 0, 1, 1, w, h);
}

void SceneState_ConfigureRegion0_0_21x14(void)
{

    s32 w = 21;
    s32 h = 14;

    Map_CopyCellAttributes(0, 0, 1, 1, w, h);
}

void SceneState_ApplyTwoRects(void)
{

    {
        s32 a5 = 1;
        s32 a6 = 3;

        Map_CopyCellsTo(111, 37, 97, 21, a5, a6);
    }
    {
        s32 a5 = 32;
        s32 a6 = 24;

        Map_CopyCellAttributes(46, 38, 3, 2, a5, a6);
    }
}

void FieldScene_RunTwoLayoutSteps(void)
{

    {
        s32 fifth = 1;
        s32 sixth = 3;

        Map_CopyCellsTo(95, 21, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 25;

        Map_CopyCellAttributes(46, 38, 3, 1, fifth, sixth);
    }
}

void FieldScene_RunActor9Flag882Scene(void)
{

    Event_Begin();
    Actor_SetPosition(9, 0, 0);
    GameFlag_Set(0x882);
    Event_End();
}

void FieldScene_RunScene398SequenceA(void)
{
    Event_Begin();
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(0x883);
    Event_Wait(40);
    Actor_SetAnimationAndWait(15, 2);
    Actor_Get(15)->motion_flags = 0;
    Actor_Get(15)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpritePriority(15, 2);
    Map_CopyCellAttributes(0, 0, 1, 1, 18, 14);
    Event_End();
}

void FieldScene_RunActorFifteenScene(void)
{

    void Audio_PlayCue(s32);

    Event_Begin();
    Actor_SetChildValue(0xF, 0);
    Event_Wait(0x28);
    Audio_PlayCue(0xD2);
    Actor_SetAnimationAndWait(0xF, 6);
    Event_End();
}

void FieldScene_RunActorSixteenScene(void)
{

    void Event_End(void);
    void Audio_PlayCue(s32);

    Event_Begin();
    Actor_SetChildValue(0x10, 0);
    Event_Wait(0x28);
    Audio_PlayCue(0xD2);
    Actor_SetAnimationAndWait(0x10, 6);
    Event_End();
}

void FieldScene_RunActor17Steps28AndD2(void)
{

    void Event_End(void);

    Event_Begin();
    Actor_SetChildValue(0x11, 0);
    Event_Wait(0x28);
    Audio_PlayCue(0xD2);
    Actor_SetAnimationAndWait(0x11, 6);
    Event_End();
}

void FieldScene_RunScene398SequenceB(void)
{

    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v5;

    rec7 = Value1(Engine_ActorGet, 11);
    rec8 = Actor_Get(12);
    if ((*(s32 *)(rec7 + 8) >> 20) == 35) {
        if ((*(s32 *)(rec7 + 16) >> 20) != 23) {
            goto L_02000330;
        }
        GameFlag_Set(0x303);
    } else {
        L_02000330:;
        GameFlag_Clear(0x303);
    }
    if ((*(s32 *)(rec8 + 8) >> 20) == 35) {
        if ((*(s32 *)(rec8 + 16) >> 20) != 23) {
            goto L_02000350;
        }
        GameFlag_Set(0x304);
    } else {
        L_02000350:;
        GameFlag_Clear(0x304);
    }
    if (GameFlag_IsSet(0x303) == 0) {
        record = GameFlag_IsSet(0x304);
        if (record == 0) {
            goto L_020003c2;
        }
    }
    if (GameFlag_IsSet(0x302) == 0) {
        Event_Begin();
        Event_Wait(40);
        Audio_PlayCue(210);
        v5 = 36;
        Actor_SetAnimationAndWait(17, 6);
        Map_CopyCellAttributes(0, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(0, 2, 1, 1, v5, 24);
        Event_End();
    }
    GameFlag_Set(0x302);
    goto L_02000414;
    L_020003c2:;
    if (GameFlag_IsSet(0x302) != 0) {
        Event_Begin();
        Event_Wait(40);
        Audio_PlayCue(220);
        v5 = 36;
        Actor_SetAnimationAndWait(17, 2);
        Map_CopyCellAttributes(1, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(1, 2, 1, 1, v5, 24);
        Event_End();
    }
    GameFlag_Clear(0x302);
    L_02000414:;
}

void ActorPresentation_SetSceneCell31AndFlag305(void)
{

    s32 width = 8;
    s32 height = 13;

    Map_CopyCellAttributes(31, 0, 1, 1, width, height);
    GameFlag_Set(0x305);
}

void SceneState_SetGlobalByte17(void)
{

    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 1;
}

void SceneState_ClearRuntimeByte17(void)
{

    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 0;
}

/*
 * Map-variant selector, and the overlay's exported entry. The 72-byte owner at
 * 0x0200046c includes its alignment halfword and four pool words, and returns
 * 0. The selector is the signed halfword at byte offset 448 of Data_02000240;
 * the three compared constants are spelled as addresses of Value_ symbols,
 * which is what puts them in the literal pool.
 */
s32 FieldScene_DispatchByScenarioId(void)
{

    s16 variant = gGameState.scene;

    if (variant == (s32)&Value_00000031) {
        RunGuardedSceneSetup();
    } else if (variant == (s32)&Value_00000030) {
        SceneState_SetRuntimeWord448To516();
    } else if (variant == (s32)&Value_0000002f) {
        FieldScene_RunScene398SequenceC();
    }
    return 0;
}

void RunGuardedSceneSetup(void)
{

    if (GameFlag_IsSet(0x305) != 0) {
        s32 width = 8;
        s32 height = 13;

        Map_CopyCellAttributes(31, 0, 1, 1, width, height);
        Actor_SetAnimation(8, 0);
    }
}

void SceneState_SetRuntimeWord448To516(void)
{

    /* 448 is built as 224 << 1 and the stored 516 as that same register plus
     * 68; the two are not one running offset. */
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);

    Actor_SetAnimation(8, 1);
    Actor_SetAnimation(10, 2);

    if (GameFlag_IsSet(0x882) != 0) {
        Actor_SetPosition(9, 0, 0);
    } else {
        Actor_SetSpriteFlags(Actor_Get(9), 0);
    }
}

void FieldScene_RunScene398SequenceC(void)
{

    u32 i;
    u8 *record;
    s32 v5;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    record = Actor_Get(18);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(19);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(20);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(21);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(22);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(23);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(24);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(25);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(26);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(18, 5);
    Actor_SetAnimation(19, 5);
    Actor_SetAnimation(20, 5);
    Actor_SetAnimation(21, 5);
    Actor_SetAnimation(22, 5);
    Actor_SetAnimation(23, 3);
    Actor_SetAnimation(24, 3);
    Actor_SetAnimation(25, 3);
    Actor_SetAnimation(26, 3);
    Actor_SetAnimation(9, 2);
    Actor_SetAnimation(10, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetAnimation(12, 2);
    Actor_SetAnimation(13, 2);
    Actor_SetAnimation(14, 2);
    Resource398_ImportBankNoOp(18);
    Resource398_ImportBankNoOp(19);
    Resource398_ImportBankNoOp(20);
    Resource398_ImportBankNoOp(21);
    Resource398_ImportBankNoOp(22);
    Resource398_ImportBankNoOp(23);
    Resource398_ImportBankNoOp(24);
    Resource398_ImportBankNoOp(25);
    Resource398_ImportBankNoOp(26);
    Resource398_ImportBankNoOp(9);
    Resource398_ImportBankNoOp(10);
    Resource398_ImportBankNoOp(11);
    Resource398_ImportBankNoOp(12);
    Resource398_ImportBankNoOp(13);
    Resource398_ImportBankNoOp(14);
    if (GameFlag_IsSet(0x883) != 0) {
        Actor_SetPosition(8, 0, 0);
        Actor_SetAnimation(15, 5);
        Actor_Get(15)->motion_flags = 0;
        Actor_Get(15)->y.fixed = -0x40000;
        Actor_Get(15)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetSpritePriority(15, 2);
        Map_CopyCellAttributes(0, 0, 1, 1, 18, 14);
    } else {
        Actor_SetAnimation(8, 2);
        record = Actor_Get(8);
        Actor_SetSpriteFlags((s32)record, 0);
        Actor_SetAnimation(15, 1);
    }
    Actor_SetAnimation(16, 1);
    if (GameFlag_IsSet(0x302) != 0) {
        v5 = 36;
        Actor_SetAnimation(17, 1);
        Map_CopyCellAttributes(0, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(0, 2, 1, 1, v5, 24);
    } else {
        v5 = 36;
        Actor_SetAnimation(17, 5);
        Map_CopyCellAttributes(1, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(1, 2, 1, 1, v5, 24);
    }
    if (GameFlag_IsSet(0x303) != 0) {
        Actor_SetPosition(11, 0x23a0000, 0x1780000);
    }
    if (GameFlag_IsSet(0x304) != 0) {
        Actor_SetPosition(12, 0x23a0000, 0x1780000);
    }
}

s32 *SceneActor_FindSlotAtTile(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_PushActorAhead(void)
{

    u8 *player;
    u8 *target;
    u8 *blocker;
    s32 heading;
    s32 tx;
    s32 tz;
    s32 pos[3];

    player = Actor_Get(0);
    heading = *(u16 *)(player + 6) >> 12;

    tx = (*(s16 *)(player + 10)
        + (Data_02009064[heading] >> 16)) >> 4;
    tz = (*(s16 *)(player + 18)
        + ((Data_02009064[heading] << 16) >> 16)) >> 4;
    target = (u8 *)SceneActor_FindSlotAtTile(tx, tz);
    if (target == 0) return;

    tx = (*(s16 *)(target + 10)
        + (Data_02009064[heading] >> 16)) >> 4;
    tz = (*(s16 *)(target + 18)
        + ((Data_02009064[heading] << 16) >> 16)) >> 4;
    blocker = (u8 *)SceneActor_FindSlotAtTile(tx, tz);
    if (blocker != 0) return;

    target[0x22] = 2;

    pos[0] = *(s32 *)(target + 8)
        + (Data_02009064[heading] & (s32)0xffff0000);
    pos[1] = *(s32 *)(target + 12);
    pos[2] = *(s32 *)(target + 16) + (Data_02009064[heading] << 16);

    if (Func_020011c0(target, pos) > 0) return;

    Object_SetAnimation(player, 8);
    Task_Wait(15);
    Audio_PlayCue(185);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Func_020011ca(target, pos[0], pos[1], pos[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Func_020011dc(player, pos[0], pos[1], pos[2]);

    Func_020011ea(target);

    *(s32 *)(target + 8) = pos[0];
    *(s32 *)(target + 16) = pos[2];
    *(s32 *)(target + 36) = (s32)blocker;
    *(s32 *)(target + 44) = (s32)blocker;

    Object_SetAnimation(player, 1);
    FieldScene_RunScene398SequenceB();
}

void Resource398_ImportBankNoOp(void)
{
}
