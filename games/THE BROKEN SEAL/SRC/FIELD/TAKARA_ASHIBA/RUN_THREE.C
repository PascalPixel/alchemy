#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

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

void TakaraAshiba_UpdateBlockRects(void);
void BattleFx_RunRisingObjectSequence(s32, s32, s32);

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
void SceneState_ApplyTwoRectsAtRow56(void);

void FieldScene_RunThreeCallSequenceB(void)
{
    SceneState_ApplyTwoRectsAtRow56();
    StagedActor_AdvancePair();
    TakaraAshiba_UpdateBlockRects();
}

void FieldScene_DispatchByActorZeroFacing(void)
{
    struct Slot *slot = Engine_GetTriggerActor(0);

    if (slot->f06 == 0x4000) {
        BattleFx_RunRisingObjectSequence(0, 6, 0);
    } else {
        FieldScene_RunThreeCallSequenceB();
    }
}

void SceneState_ApplyFourRectsAt48_55(void)
{
    s32 base = 55;

    Map_CopyCellAttributes(48, 55, 1, 1, 49, base);
    Map_CopyCellAttributes(48, 55, 1, 1, 50, base);
    Map_CopyCellAttributes(48, 55, 1, 1, 51, base);
    Map_CopyCellAttributes(48, 55, 1, 1, 52, base);
}

/*
 * Repaint the four cells, then one cell for each of slots 11 and 12 at that
 * slot's own X tile.  The 92-byte owner at 0x02001df8 includes two bytes of
 * alignment and the pool word 0x000fffff.  The tile divisions must stay
 * spelled `/ 0x100000': the reference biases a negative value before the
 * arithmetic shift, which is exactly this truncating signed division.
 */
void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void)
{
    s32 *slot;
    s32 tile;

    SceneState_ApplyFourRectsAt48_55();

    slot = Engine_GetTriggerActor(11);
    tile = slot[2] / 0x100000;
    Map_CopyCellAttributes(53, 55, 1, 1, tile, 55);
    slot = Engine_GetTriggerActor(12);
    tile = slot[2] / 0x100000;
    Map_CopyCellAttributes(53, 55, 1, 1, tile, 55);
}

void FieldScene_RunSingleStep(void)
{
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

void FieldScene_RunThreeCallSequence(void)
{
    SceneState_ApplyFourRectsAt48_55();
    StagedActor_AdvancePair();
    FieldScene_RunSingleStep();
}

void FieldScene_CallHelper3c70(void)
{
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

void FieldScene_RunThreeStepSequence(void)
{
    SceneState_ApplyFourRectsAt48_55();
    StagedActor_AdvancePair();
    FieldScene_CallHelper3c70();
}

void ActorPresentation_PlaceActorFourteenOnActorNine(void)
{
    struct Actor *target = Engine_GetTriggerActor(14);
    struct Actor *source = Engine_GetTriggerActor(9);

    target->f0c = 0x200000;
    target->f08 = source->f08;
    target->f10 = source->f10 + 0x10000;
}

void SceneActor_PassActorNinePositionWithId107(void)
{
    struct Frame *frame = Engine_GetTriggerActor(9);

    MapObject_SetPosition(107, frame->f08, frame->f10 + 0x10000);
}

/*
 * Publish one marker byte at +35 to slots 8, 10, 11 and 12 according to slot
 * 0's height word at +12.  The 156-byte owner includes its one pool word,
 * 0x000fffff, read by the tile division.  The marker local is what carries
 * the value 2 across the high path, which branches over the clear to 0.
 * Slot 11's record is fetched once on each path rather than once before
 * them, and that duplication is what reproduces the reference.
 */
void SceneActor_PublishMarkerBySlotZeroHeight(void)
{
    s32 *slot0 = Engine_GetTriggerActor(0);
    u8 marker;

    if (slot0[3] > 0x100000) {                 /* +12 */
        marker = 2;
        ((u8 *)Engine_GetTriggerActor(8))[35] = marker;
        if (Engine_GetTriggerActor(10)[3] == 0) {
            ((u8 *)Engine_GetTriggerActor(10))[35] = marker;
        }
        ((u8 *)Engine_GetTriggerActor(11))[35] = marker;
    } else {
        if (Engine_GetTriggerActor(10)[3] == 0 &&
            Engine_GetTriggerActor(0)[4] / 0x100000 > 56) {   /* +16 */
            Actor_SetSpritePriority(10, 3);
        } else {
            Actor_SetSpritePriority(10, 1);
            ((u8 *)Engine_GetTriggerActor(10))[35] = 1;
        }
        marker = 0;
        ((u8 *)Engine_GetTriggerActor(11))[35] = marker;
    }

    ((u8 *)Engine_GetTriggerActor(12))[35] = marker;
}

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);
