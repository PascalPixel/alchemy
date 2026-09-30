#include "TYPES.H"
#include "FIELD_EVENT.H"
extern s16 Data_02000240[];
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
#include "CALL.H"

enum {
    /* Message 0x182 + 243. */
    ITEM_RED_KEY = 243,
    /* Message 0x182 + 244. */
    ITEM_BLUE_KEY = 244
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

u8 *Object_GetById(s32);
void battle_owner_69();
void FieldEffect_UpdateGridPlacement();

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

s32 *Engine_GetTriggerActor(s32 slot);
s32 Engine_TestTriggerFlag(s32 flag);
void Engine_SetTriggerFlag(s32 flag);

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}

void SceneState_TriggerColumnNineteen(void)
{
    extern u8 *Data_03001ebc;
    s32 *pos = Engine_GetTriggerActor(0);
    s32 x = pos[2] / 0x100000;
    s32 z = pos[4] / 0x100000;
    u8 *work = Data_03001ebc;
    s16 *state = Data_02000240;

    if (state[0x24a / 2] != 12 && Value1(Engine_TestTriggerFlag, 0x220) == 0 &&
        state[0x24c / 2] == 0 && x == 19 && (u32)(z - 15) <= 1) {
        Call1(Engine_SetTriggerFlag, 0x220);
        SceneState_StoreStep((s16 *)(work + 386), 96);
    }
}

void FieldScene_RunScene3b4SequenceB(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
    Actor_SetSpeed(12, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(12);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
    Audio_PlayCue(188);
    Actor_SetDestinationOffset(12, 0, 16);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(12, 0x138, 232);
    Actor_WaitForMove(12);
    Event_End();
    GameFlag_Clear(0x220);
}

void SetBlendAlphaCoefficients(void)
{
    u32 coefficient = 208;

    coefficient <<= 4;
    *(u16 *)0x04000052 = coefficient;
}

/* Complete blend-alpha setter through return and its two pool words. */
void SceneEffect_SetBlendAlpha0607(void)
{
    u16 value = 0x0607;

    *(volatile u16 *)0x04000052 = value;
}

/*
 * Scene state interaction for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the scene flag test, clear and set imports. */

/*
 * Dispatch on slot 0's halfword at +6, the facing field, which takes the
 * values 0, 0x4000, 0x8000 and 0xc000.  The 160-byte owner includes its two
 * pool words, 0x206 and 0x207.  Two arms repaint one collision cell, but
 * only while scene flag 0x206 is set, and then move flag 0x207.  The 0x8000
 * arm does no flag work and branches on slot 0's height word at +12, read
 * from the record pointer already in hand rather than a fresh lookup.
 */
void FieldScene_DispatchBySlotZeroFacing(void)
{
    s32 *slot = Engine_GetTriggerActor(0);
    u16 facing = *(u16 *)((u8 *)slot + 6);

    if (facing == 0xc000) {
        if (GameFlag_IsSet(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Map_CopyCellAttributes(46, 43, 1, 1, fifth, last); }
        }
        GameFlag_Clear(0x207);
        battle_owner_69();
    } else if (facing == 0x4000) {
        FieldEffect_UpdateGridPlacement();
    } else if (facing == 0) {
        if (GameFlag_IsSet(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Map_CopyCellAttributes(58, 36, 1, 1, fifth, last); }
        }
        GameFlag_Set(0x207);
        Leader_CheckAhead();
    } else if (facing == 0x8000) {
        if (slot[3] == 0) {          /* +12 */
            SceneActor_BranchOnSlotZeroAtTile38();
        } else {
            Leader_CheckAhead();
        }
    }
}

/* Deliberate no-op callback. */
void FieldScene_NoOpCallback(void) {}

void SceneActor_MarkSlot13AndSetFlag200(void)
{
    u8 *slot = Engine_GetTriggerActor(13);
    s32 fifth = 40;
    s32 sixth = 55;

    Map_CopyCellAttributes(40, 54, 1, 1, fifth, sixth);
    if (slot != 0) {
        u8 *other = Object_GetById(13) + 85;
        u8 *flags = slot + 35;

        *other = 0;
        *flags = 2;
    }
    GameFlag_Set(512);
}

/*
 * Facing target scene for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the notification and step imports. */
void SceneState_BranchOnActorZeroFacing(void)
{
    struct Slot *slot = Engine_GetTriggerActor(0);

    if (slot->f06 == 0) {
        Leader_CheckAhead();
    } else {
        SceneActor_BranchOnSlotZeroAtTile38();
    }
}

void SceneState_ApplyFourRectsAndSetActor8Byte85(void)
{
    Map_CopyCellAttributes(57, 42, 1, 1, 40, 42);
    Map_CopyCellAttributes(57, 42, 1, 1, 41, 42);
    Map_CopyCellAttributes(58, 42, 1, 1, 42, 42);
    Map_CopyCellAttributes(62, 37, 3, 1, 37, 42);

    Object_GetById(8)[85] = 1;
}

void ActorPresentation_SetSceneCell58AndMarkActorEight(void)
{
    s32 extent = 42;
    u8 *entry;

    Map_CopyCellAttributes(58, 41, 1, 1, extent, extent);
    entry = Object_GetById(8) + 35;
    *entry = 2;
}

void SceneState_ApplyRectAndSetActor8Byte35(void)
{
    s32 w = 41;
    s32 h = 42;
    u8 *p;

    Map_CopyCellAttributes(44, 42, 1, 1, w, h);
    p = Object_GetById(8) + 35;
    *p = 2;
}

void SceneState_ApplyRectAndSetSlotEightByte35(void)
{
    s32 width = 40;
    s32 height = 42;
    u8 *entry;

    Map_CopyCellAttributes(39, 42, 1, 1, width, height);
    entry = Object_GetById(8) + 35;
    *entry = 2;
}
void SceneActor_BranchOnSlotZeroAtTile38(void);

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);

/* Repaints the lifted block's cell at a column and raises actor 8 to it. */
static __inline__ void LiftBlock(s32 column, u8 lowered)
{
    { s32 fifth = column; s32 last = 42; Map_CopyCellAttributes(61, 36, 1, 1, fifth, last); }
    Object_GetById(8)[85] = lowered;
    *(s32 *)(Object_GetById(8) + 12) = 0x200000;
}

/* Acts on the map column actor 8 stands in: marks it when it rests on the
 * ground, repaints the four gate cells, then opens the passage for columns
 * 40 to 42, or lifts the block in columns 37 to 39. Each of those three
 * columns lifts it on its own branch. */
void TakaraAshiba_DispatchByActorEightColumn(void)
{
    u8 *actor = Object_GetById(8);
    s32 x = *(s32 *)(actor + 8);
    s32 height = *(s32 *)(actor + 12);
    s32 column = x / 0x100000;
    u8 lowered;

    if (height == 0) {
        Object_GetById(8)[35] = 2;
    }
    SceneState_ApplyFourRectsAndSetActor8Byte85();
    lowered = 0;
    Object_GetById(8)[85] = 3;
    if (column == 40) {
        SceneState_ApplyRectAndSetSlotEightByte35();
    } else if (column == 42) {
        ActorPresentation_SetSceneCell58AndMarkActorEight();
    } else if (column == 41) {
        SceneState_ApplyRectAndSetActor8Byte35();
    } else if (column == 39) {
        LiftBlock(column, lowered);
    } else if (column == 38) {
        LiftBlock(column, lowered);
    } else if (column == 37) {
        LiftBlock(column, lowered);
    }
}
