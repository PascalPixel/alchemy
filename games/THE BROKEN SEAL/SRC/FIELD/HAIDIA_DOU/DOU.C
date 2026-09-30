#include "HAIDIA.H"
#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
#include "CALL.H"

extern const struct SceneEntrance gHaidiaDouEntrances1[];
extern const struct SceneEntrance gHaidiaDouEntrances2[];
extern const struct SceneEntrance gHaidiaDouEntrances3[];
extern const struct SceneEntrance gHaidiaDouEntrancesOther[];

extern const u32 HaidiaDou_Exits[];

extern const struct ScenePlacement gHaidiaDouPlacements1[];
extern const struct ScenePlacement gHaidiaDouPlacements2[];
extern const struct ScenePlacement gHaidiaDouPlacements3[];

struct FieldActor *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 kind);
void Map_CopyCellAttributeRect();

void SceneState_SetValues8_3_4(void)
{
    BattleFx_RunPageEffectForSlot(8, 3, 4);
}

/* Give the actor at most sixty frames to descend to its target height, then
 * clamp the live height to the target so the following scene starts exact. */
void SceneActor_WaitActorDescent(u8 *obj)
{
    s32 cnt = 60;

    while (cnt != 0) {
        WaitFrames(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20))
            break;
    }
    *(s32 *)(obj + 12) = *(s32 *)(obj + 20);
}

/* Point an object toward actor zero using their fixed-point X/Z delta. */
s32 SceneActor_FaceActorZero(u8 *obj)
{
    u8 *target = Actor_Get(ACTOR_PARTY_LEADER);
    s32 dz = *(s32 *)(target + 16) - *(s32 *)(obj + 16);
    s32 dx = *(s32 *)(target + 8) - *(s32 *)(obj + 8);

    *(s16 *)(obj + 6) = (s16)ArcTan2(dz, dx);
    return 0;
}

/* Where the party appears in each of the sanctum's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouEntrances1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouEntrances2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouEntrances3;
    }
    return gHaidiaDouEntrancesOther;
}

/* The sanctum's regions and exits, between its scene-dependent getters. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return HaidiaDou_Exits;
}

/* The actors placed in each of the sanctum's three areas; any other scene
   takes the first area's. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouPlacements1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouPlacements2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouPlacements3;
    }
    return gHaidiaDouPlacements1;
}

void FieldScene_ConfigureRegionAtRow15(void)
{
    Map_CopyCells(16, 15, 1, 1, 15, 15);
}

/* Configure the matching 16x15 scene rectangle at row 17. */
void FieldScene_ConfigureRegionAtRow17(void)
{
    Map_CopyCells(16, 17, 1, 1, 15, 15);
}

/* When the pillar (actor 9) stands in column 23, steps the leader aside and
 * lowers the pillar into the floor amid a spray of dust between two
 * markers, opens the cell it blocked and sets flag 0x200. */
void HaidiaDou_SinkPillarColumn23(void)
{
    struct EffectOptions options;
    struct EffectOptions *o;
    struct FieldActor *left;
    struct FieldActor *right;
    u32 i;

    Engine_EventBegin();
    if (Object_GetById(9)->x.fixed >> 20 == 23) {
        Call3((void (*)())Engine_ActorMoveToAndWait, 0, 360, 664);
        Engine_ActorFaceDirection(0, 0xe000, 10);
        Object_GetById(9)->x.fixed += 0x20000;
        left = OverlayObject_CreateConfigured(Object_GetById(9)->x.fixed, 0,
                                              Object_GetById(9)->z.fixed + 0x340000, 241);
        right = OverlayObject_CreateConfigured(Object_GetById(9)->x.fixed + 0x100000, 0,
                                               Object_GetById(9)->z.fixed + 0x340000, 241);
        Object_GetById(9)->motion_flags = 0;
        o = &options;
        o->start_scale_x = 0x9999;
        o->start_scale_y = 0x9999;
        o->palette = 7;
        Audio_PlayCue(216);
        for (i = 0; i < 68; i++) {
            s32 x = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + 0x1700000;
            s32 z = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + 0x2700000;

            Effect_Spawn(x, 0, z, 0, 0, 0, 0x90000, o);
            Object_GetById(9)->y.fixed -= 0x8000;
            Battle_WaitMode0(1);
        }
        Call6((void (*)())Map_CopyCellAttributeRect, 23, 41, 1, 1, 23, 39);
        Object_GetById(9)->priority_flags |= 2;
        Engine_GameFlagSet(0x200);
        Object_GetById(9)->y.fixed = -0x80000;
        Object_SetModeById(9, 2);
        Engine_ObjectDispatchRelease(left);
        Engine_ObjectDispatchRelease(right);
        Battle_WaitMode0(30);
    }
    Engine_EventEnd();
}

/* When the pillar (actor 10) stands in column 27, lowers it into the floor
 * amid a spray of dust between two markers, opens the cells it blocked,
 * sets flag 0x201 and settles it below the floor. */
void HaidiaDou_SinkPillarColumn27(void)
{
    struct EffectOptions options;
    struct EffectOptions *o;
    struct FieldActor *left;
    struct FieldActor *right;
    u32 i;

    Engine_EventBegin();
    if (Object_GetById(10)->x.fixed >> 20 == 27) {
        left = OverlayObject_CreateConfigured(Object_GetById(10)->x.fixed - 0x80000, 0,
                                              Object_GetById(10)->z.fixed + 0x340000, 241);
        right = OverlayObject_CreateConfigured(Object_GetById(10)->x.fixed + 0x80000, 0,
                                               Object_GetById(10)->z.fixed + 0x340000, 241);
        Object_GetById(9)->motion_flags = 0;
        o = &options;
        o->start_scale_x = 0x9999;
        o->start_scale_y = 0x9999;
        o->palette = 7;
        Audio_PlayCue(216);
        for (i = 0; i < 68; i++) {
            s32 x = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + 0x1b00000;
            s32 z = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + 0x2900000;

            Effect_Spawn(x, 0, z, 0, 0, 0, 0x90000, o);
            Object_GetById(10)->y.fixed -= 0x8000;
            Battle_WaitMode0(1);
        }
        Call6((void (*)())Map_CopyCellAttributeRect, 31, 39, 2, 1, 27, 41);
        Object_GetById(10)->priority_flags |= 2;
        Engine_GameFlagSet(0x201);
        Object_GetById(10)->y.fixed = -0x80000;
        Object_SetModeById(10, 2);
        Engine_ObjectDispatchRelease(left);
        Engine_ObjectDispatchRelease(right);
        Battle_WaitMode0(30);
    }
    Engine_EventEnd();
}

void FieldScene_RunInitBracketThenSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    HaidiaDou_SinkPillarColumn27();
}

s32 FieldScene_RunPrimarySequence(s32 a0)
{

    s32 box[3];
    u8 *rec;
    u8 *flag;
    u8 *slot;
    s32 saved;

    rec = (u8 *)Object_GetById(ACTOR_PARTY_LEADER);
    flag = rec + 85;
    saved = *flag;
    slot = (u8 *)box;
    *(s32 *)(slot + 0) = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Object_CheckMovementCollision((s32)rec, (s32)slot) == 0) {
        Event_Begin();
        Object_SetAnimation((s32)rec, 6);
        WaitFrames(6);
        Audio_PlayCue(152);
        Object_SetAnimation((s32)rec, 7);
        *(s32 *)(rec + 48) = 0x30000;
        *(s32 *)(rec + 52) = 0x20000;
        *(s32 *)(rec + 40) = 0x40000;
        *flag = *flag & 126;
        Actor_SetSpriteFlags((s32)rec, 0);
        Engine_ActorMoveToAndWait(0, *(s16 *)(slot + 2), *(s16 *)(slot + 10));
        Object_SetAnimation((s32)rec, 6);
        Actor_SetSpriteFlags((s32)rec, 1);
        *flag = (u8)saved;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}

void FieldScene_RunScene3a6SequenceA(void)
{

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) == 0) {
        GameFlag_Set(0x200);
        Event_Begin();
        Camera_SetSpeed(0x10000, 0x2000);
        Camera_FollowActor(8, 1);
        Camera_WaitForMove();
        Battle_WaitMode0(60);
        Actor_FaceDirection(8, 0xc000, 20);
        Actor_SetAttachedEffect(8, 0x102);
        Actor_RunRepeatedMotion(8, 2);
        Battle_WaitMode0(20);
        Actor_SetMotionSpeed(8, 0x10000, 0x8000);
        Actor_WalkToAndWait(8, 0x318, 248);
        Audio_PlayCue(152);
        record = Actor_Get(8);
        *(s32 *)(record + 40) = 0x80000;
        Actor_WalkToAndWait(8, 0x318, 0x118);
        Battle_WaitMode0(20);
        Actor_FaceDirection(8, 0xc000, 20);
        Battle_WaitMode0(30);
        Event_End();
    }
}

void FieldScene_RunScene3a6SequenceB(void)
{

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) != 0) {
        if (GameFlag_IsSet(0x201) == 0) {
            GameFlag_Set(0x201);
            GameFlag_Set(0x302);
            Event_Begin();
            Actor_SetAttachedEffect(8, 0x102);
            Actor_RunRepeatedMotion(8, 2);
            Battle_WaitMode0(20);
            Actor_SetMotionSpeed(8, 0x20000, 0x10000);
            Actor_WalkToAndWait(8, 0x2f8, 0x118);
            Actor_WalkToAndWait(8, 0x2f8, 0x138);
            Actor_WalkToAndWait(8, 0x318, 0x138);
            Battle_WaitMode0(10);
            Actor_FaceDirection(8, 0xc000, 20);
            record = Actor_Get(8);
            *(s32 *)(record + 108) = (s32)SceneActor_FaceActorZero;
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunScene3a6SequenceC(void)
{

    s32 rec8;
    s32 record;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    struct EventWork *p5;

    p5 = gEventWork;
    if (GameFlag_IsSet(0x302) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)&gGameState + off24a) != 8) {
            idx = p5->touched_trigger;
            rec8 = Object_GetById(8);
            record = Object_GetById(ACTOR_PARTY_LEADER);
            *(s32 *)(rec8 + 48) = *(s32 *)(record + 48);
            rec8 = Object_GetById(8);
            record = Object_GetById(ACTOR_PARTY_LEADER);
            *(s32 *)(rec8 + 52) = *(s32 *)(record + 52);
            idx -= 45;
            tbl = (s32)HaidiaDou_WalkTargets;
            idx <<= 3;
            idx4 = idx + 4;
            Actor_WalkTo(8, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

void FieldScene_RunActor8ZeroStep(void)
{
    Event_Begin();
    Object_SetModeById(8, 0);
    Event_End();
}
