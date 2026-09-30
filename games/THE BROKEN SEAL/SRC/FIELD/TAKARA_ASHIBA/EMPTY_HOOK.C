#include "TYPES.H"
#include "FIELD_EVENT.H"
extern s16 Data_02000240[];
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

void BattleFx_RunPageEffectForSlot(s32 actor, s32 mode, s32 frames);
s32 ArcTan2();
void ObjectDispatch_ApplyValueToChildren();
s32 Object_GetById();

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

/* Complete two-byte empty hook plus its alignment halfword. */
void Resource3b4_EmptyHookB(void)
{
}

/*
 * Complete 40-byte heading update: face the supplied entity towards entity 0,
 * store the resulting angle in its +6 halfword and report zero.
 */
s32 SceneActor_FaceTowardActorZero(u8 *obj)
{
    u8 *p = Engine_GetTriggerActor(0);
    *(u16 *)(obj + 6) = (u16)ArcTan2(
        *(s32 *)(p + 16) - *(s32 *)(obj + 16),
        *(s32 *)(p + 8) - *(s32 *)(obj + 8));
    return 0;
}

void FieldScene_RunScene3b4_02000ad0(void)
{
    s32 record;

    if (GameFlag_IsSet(0x9c8) == 0) {
        GameFlag_Set(0x9c8);
        Event_Begin();
        Camera_SetSpeed(0x20000, 0x4000);
        Camera_MoveToActor(15, 1);
        Camera_WaitForMove();
        Actor_FaceDirection(15, 0, 20);
        Actor_SetAttachedEffect(15, 0x102);
        Actor_RunRepeatedMotion(15, 2);
        Event_Wait(20);
        Actor_SetSpeed(15, 0x10000, 0x8000);
        Audio_PlayCue(152);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x80000;
        Actor_WalkToAndWait(15, 0x248, 0x2a8);
        Actor_FaceDirection(15, 0x4000, 20);
        Event_End();
    }
}

void FieldScene_RunScene3b4_02000b68(void)
{
    s32 rec7;
    s32 rec8;
    s32 record;

    if (GameFlag_IsSet(0x9c8) != 0) {
        rec8 = GameFlag_IsSet(0x9c9);
        if (rec8 == 0) {
            GameFlag_Set(0x9c9);
            Event_Begin();
            Camera_SetSpeed(0x20000, 0x4000);
            Camera_MoveToActor(15, 1);
            Camera_WaitForMove();
            Actor_FaceDirection(15, 0x4000, 20);
            Actor_SetAttachedEffect(15, 0x102);
            Actor_RunRepeatedMotion(15, 2);
            Event_Wait(20);
            Actor_SetSpeed(15, 0x10000, 0x8000);
            Audio_PlayCue(152);
            record = Engine_GetTriggerActor(15);
            *(s32 *)(record + 40) = 0xa0000;
            Actor_WalkToAndWait(15, 0x248, 0x298);
            Actor_FaceDirection(15, 0x4000, 20);
            Actor_SetAttachedEffect(15, 0x102);
            Event_Wait(30);
            Actor_SetSpeed(15, 0x80000, 0x4000);
            Actor_WalkToAndWait(15, 0x298, 0x298);
            Actor_WalkToAndWait(15, 0x2e8, 0x298);
            Actor_WalkToAndWait(15, 0x338, 0x298);
            Event_Wait(10);
            Audio_PlayCue(208);
            Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
            Event_Wait(20);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(30);
            Actor_SetPosition(15, 0x3780000, 0x2980000);
            rec7 = Engine_GetTriggerActor(15);
            {
                s32 target = *(s32 *)(rec7 + 80);
                s32 shown = 0xf800;

                *(u16 *)(target + 30) = shown;
            }
            *(u16 *)(rec7 + 6) = 0;
            ObjectDispatch_ApplyValueToChildren(rec7, 0);
            Engine_ObjectSetScript(rec7, 0x200a6fc);
            Event_End();
        }
    }
}

void FieldScene_RunScene3b4_02000ccc(void)
{
    s32 record;

    if (GameFlag_IsSet(0x9c9) != 0 && GameFlag_IsSet(0x9ca) == 0) {
        GameFlag_Set(0x9ca);
        Event_Begin();
        record = Object_GetById(15);
        *(u16 *)(*(s32 *)(record + 80) + 30) = 0;
        ObjectDispatch_ApplyValueToChildren(record, 16);
        Audio_PlayCue(152);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x80000;
        Actor_FaceDirection(15, 0x8000, 30);
        Actor_SetAttachedEffect(15, 0x102);
        Actor_RunRepeatedMotion(15, 2);
        Event_Wait(20);
        Actor_SetSpeed(15, 0x10000, 0x8000);
        Audio_PlayCue(152);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x40000;
        Actor_WalkToAndWait(15, 0x370, 0x2a8);
        Event_Wait(10);
        Actor_SetAttachedEffect(15, 0x101);
        Actor_SetSpeed(15, 0x20000, 0x10000);
        Actor_WalkToAndWait(15, 0x370, 0x2b8);
        Actor_WalkToAndWait(15, 0x372, 0x2c0);
        Actor_WalkToAndWait(15, 0x370, 0x2c8);
        Actor_WalkToAndWait(15, 0x36e, 0x2d0);
        Actor_WalkToAndWait(15, 0x370, 0x2d8);
        Actor_WalkToAndWait(15, 0x372, 0x2e0);
        Actor_WalkToAndWait(15, 0x370, 0x2e8);
        Actor_WalkToAndWait(15, 0x36e, 0x2f0);
        Actor_WalkToAndWait(15, 0x370, 0x2f8);
        Actor_SetPosition(15, 0x3580000, 0x3380000);
        Event_Wait(10);
        Actor_FaceDirection(15, 0xc000, 20);
        Actor_SetAttachedEffect(15, 0x100);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 108) = (s32)SceneActor_FaceTowardActorZero;
        Event_End();
    }
}

void FieldScene_CopyActorPosition(void)
{
    s32 dst;
    s32 src;
    s32 idx;
    s32 tbl;
    s32 idx4;
    struct EventWork *work;

    work = gEventWork;
    if (GameFlag_IsSet(0x9ca) != 0) {
        if (Data_02000240[293] != 15) {
            idx = work->touched_trigger;
            dst = Object_GetById(15);
            src = Object_GetById(0);
            *(s32 *)(dst + 48) = *(s32 *)(src + 48);
            dst = Object_GetById(15);
            src = Object_GetById(0);
            *(s32 *)(dst + 52) = *(s32 *)(src + 48);
            idx -= 30;
            tbl = 0x0200a808;
            idx <<= 3;
            idx4 = idx + 4;
            Actor_WalkTo(15, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

/* Contiguous unnamed leaf-owner run for resource_3b4. */
void FieldScene_RunActor15ZeroStep(void)
{
    Event_Begin();
    Actor_SetAnimation(15, 0);
    Event_End();
}

s32 *Engine_GetTriggerActor(s32 slot);
s32 Engine_TestTriggerFlag(s32 flag);
void Engine_SetTriggerFlag(s32 flag);

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);
