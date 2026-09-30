#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
void ActorPresentation_PlaceActorFourteenOnActorNine(void);

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

void Scheduler_RemoveCallbackFar();
u8 *Object_GetById(s32);
void battle_owner_69();
void FieldEffect_UpdateGridPlacement();
void TakaraAshiba_DispatchByActorEightColumn();

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
void SceneState_ApplyFourRectsAndSetActor8Byte85(void);

/*
 * Compare the X tiles of slots 0 and 8, each the word at +8 divided by
 * 0x100000.  The 100-byte owner includes its one pool word, 0x000fffff,
 * which that truncating signed division reads.  The special cases apply only
 * when slot 0 sits on tile 38 and slot 8 does not; then slot 0's halfword at
 * +6 selects one of two notifications.  Every other case, an unrecognised
 * halfword included, runs the three-step ordinary path.
 */
void SceneActor_BranchOnSlotZeroAtTile38(void)
{
    s32 *slot0 = Engine_GetTriggerActor(0);
    s32 *slot8 = Engine_GetTriggerActor(8);
    s32 x0 = slot0[2] / 0x100000;
    s32 x8 = slot8[2] / 0x100000;

    if (x0 == 38 && x8 != 38) {
        u16 facing = ((u16 *)slot0)[3];

        /*
         * The facing value is still in r0 at both branches, but whether
         * either callee reads it is unverified, so no argument is passed.
         */
        if (facing == 0xc000) {
            battle_owner_69();
            return;
        }
        if (facing == 0x4000) {
            FieldEffect_UpdateGridPlacement();
            return;
        }
    }

    SceneState_ApplyFourRectsAndSetActor8Byte85();
    StagedActor_AdvancePair();
    TakaraAshiba_DispatchByActorEightColumn();
}

void FieldScene_RunScene3b4_02001bc4(void)
{
    u32 i;
    s32 record;

    Scheduler_RemoveCallbackFar((s32)ActorPresentation_PlaceActorFourteenOnActorNine);
    Actor_SetPosition(14, 0, 0);
    if (GameFlag_IsSet(0x207) != 0) {
        Map_CopyCellAttributes(58, 36, 1, 1, 45, 43);
    } else {
        Map_CopyCellAttributes(46, 43, 1, 1, 45, 43);
    }
    SceneActor_PassActorNinePositionWithId107();
    GameFlag_Set(0x206);
}

void SceneActor_RunWhenActor9AtTile45x43(void)
{
    s32 *slot = Engine_GetTriggerActor(9);
    s32 x = slot[2] / 0x100000;
    s32 z = slot[4] / 0x100000;

    if (x == 45 && z == 43) {
        FieldScene_RunScene3b4_02001bc4();
    }
}

void FieldScene_RunTwoStepSequence(void)
{
    StagedActor_AdvancePair();
    SceneActor_RunWhenActor9AtTile45x43();
}

void SceneState_ApplyTwoRectsAtRow56(void)
{
    s32 base = 55;

    Map_CopyCellAttributes(38, 56, 1, 1, 38, base);
    Map_CopyCellAttributes(42, 56, 1, 1, 42, base);
}

void SceneState_ApplyRectAndClearSlotTenByte85(void)
{
    s32 width = 38;
    s32 height = 55;
    u8 *entry;

    Map_CopyCellAttributes(40, 54, 1, 1, width, height);
    entry = Object_GetById(10) + 85;
    *entry = 0;
}

void SceneState_ApplyRectAndClearActor10Byte85(void)
{
    s32 w = 42;
    s32 h = 55;
    u8 *p;

    Map_CopyCellAttributes(40, 54, 1, 1, w, h);
    p = Object_GetById(10) + 85;
    *p = 0;
}
void SceneActor_PassActorNinePositionWithId107(void);

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);
