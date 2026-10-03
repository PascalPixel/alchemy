#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "FIXED_POINT_POSITION.H"
#include "IWRAM_CALL.H"

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_0200113c {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 continuation);
void PartyInventory_Discard();

/* Map cell steps played as the leader arrives in scene twelve. */
extern const u16 KuupuappuMura_Scene12Cells[];

s32 ActorPresentation_UpdateEntityFromLeader();
s32 SceneActor_UpdateProximityToLeader();
extern u8 MsgKuupuappuWelcomeBackTwo[];
s32 Engine_GameFlagIsSet();
void Engine_EventRequestExit();
void Engine_MapCopyCellAttributes();
void OverlayObject_CreateConfiguredObject();
void Engine_ActorSetPosition();
void Engine_EventBegin();
void Engine_ActorFaceEachOther();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventWait();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Event_PrepareObjectAndApplyValue();

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[0x47];
    u8 active;
};

extern s32 ArcTan2(s32, s32);

/* The exact damping callback owns +30/+34 as drift rates and +64 as its
 * plane selector. Its scalar coordinates share the engine actor record. */
struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};

LAYOUT_OFFSET_GUARD(Drift_HorizontalRate, struct OverlayEffectMotion, horizontal_rate, 0x30);
LAYOUT_OFFSET_GUARD(Drift_VerticalRate, struct OverlayEffectMotion, vertical_rate, 0x34);
LAYOUT_OFFSET_GUARD(Drift_Mode, struct OverlayEffectMotion, mode, 0x64);

union DriftingObject {
    struct FieldActor actor;
    struct OverlayEffectMotion motion;
};

void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect);

/* The two animation scripts a drifting effect may play. */
extern const s32 KuupuappuMura_DriftScriptA[];
extern const s32 KuupuappuMura_DriftScriptB[];

/*
 * Later scene steps: the leader's arrival for scene twelve and thirteen, and
 * the sequences of actors 23 and 19.
 */
void ActorPresentation_SetupActorZeroForSceneTwelveAt72_160(void)
{

    struct SceneActor_0200113c *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(KuupuappuMura_Scene12Cells, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(72, 160, 12);
}

void FieldScene_SetupScene13At152_264(void) { Engine_AudioPlayCue(123); SceneActor_PlaceAndSetSceneDelay(152, 264, 13); }

void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
    void Engine_TaskWait(int);
    void Actor_SetPosition(int, int, int);

    u8 *record = Object_GetById(actor); int frames;
    Engine_ActorSetSpeed(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorMoveToAndWait(actor, x, z); Engine_ActorSetPosition(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Engine_TaskWait(1); if (*(s16 *)(record + 42) == 0) break; }
    Engine_ActorSetSpriteFlags(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void FieldScene_RunActor23SequenceOnceByFlag867(void)
{
    void Engine_EventWait();

    u32 i;
    s32 record;

    Engine_EventBegin();
    Audio_PlayCue(100);
    Engine_EventWait(40);
    if (GameFlag_IsSet(0x867) == 0) {
        Actor_SetAttachedEffect(23, 0x102);
        Engine_ActorJump(23, 4, 0);
        Engine_EventWait(12);
        Engine_ActorJump(23, 4, 0);
        Engine_EventWait(20);
        ActorPresentation_MoveActorToPositionAndWait(23, 0x188, 104, 0x70000);
        Engine_EventWait(20);
        Actor_WalkToAndWait(23, 0x198, 104);
        Actor_WalkToAndWait(23, 0x198, 120);
        GameFlag_Set(0x867);
    }
    Engine_EventEnd();
}

void FieldScene_RunActor19MotionSequence(void)
{
    u32 i;
    s32 record;

    PartyInventory_Discard(231);
    Engine_EventBegin();
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(19, 2);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_WalkToAndWait(19, 216, 0x198);
    Engine_EventWait(10);
    Actor_FaceDirection(19, 0x4000, 20);
    Engine_ActorJump(19, 6, 0);
    Engine_EventWait(30);
    Engine_ActorJump(19, 6, 0);
    Engine_EventWait(30);
    Engine_ActorJump(19, 6, 0);
    Engine_EventWait(30);
    Actor_WalkToAndWait(19, 216, 0x188);
    Engine_EventWait(10);
    Actor_FaceDirection(19, 0x4000, 20);
    GameFlag_Set(0x858);
    Engine_EventEnd();
}

/* The saved game state; the entrance the party came in by is at +450. */
s32 KuupuappuMura_RestoreEntryState(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v7;
    s32 zero;

    if (Engine_GameFlagIsSet(0x87a) != 0) {
        Engine_EventRequestExit(14);
    }
    if (Engine_GameFlagIsSet(0x200) != 0) {
        Call6(Engine_MapCopyCellAttributes, 55, 26, 4, 2, 23, 26);
    }
    OverlayObject_CreateConfiguredObject(0x800000, 0, 0x1a40000, 223);
    v7 = 1;
    zero = 0;
    record = Object_GetById(14);
    *(u16 *)(record + 100) = v7;
    *(s32 *)(record + 108) = (s32)SceneActor_UpdateProximityToLeader;
    record = Object_GetById(15);
    *(u16 *)(record + 100) = zero;
    /* FAKEMATCH: the empty loop keeps the halfword store ahead of the
     * script-pointer store, as in the reference schedule. */
    do { } while (0);
    *(s32 *)(record + 108) = (s32)SceneActor_UpdateProximityToLeader;
    if (Engine_GameFlagIsSet(0x858) != 0) {
        Call3(Engine_ActorSetPosition, 19, 0xd80000, 0x1880000);
    }
    rec7 = Engine_GameFlagIsSet(0x853);
    if (Engine_GameFlagIsSet(0x855) == 0) {
        if ((v7 & rec7) != 0) {
            record = Object_GetById(21);
            *(s32 *)(record + 108) = (s32)ActorPresentation_UpdateEntityFromLeader;
        }
    }
    if (gGameState.entrance <= 2) {
        if (Engine_GameFlagIsSet(0x109) == 0) {
            Engine_GameFlagClear(0x867);
            if (Engine_GameFlagIsSet(0x855) == 0) {
                if (Engine_GameFlagIsSet(0x856) != 0) {
                    Engine_EventBegin();
                    record = Object_GetById(0);
                    if (record != 0) {
                        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
                    }
                    if (gGameState.entrance == 1) {
                        Call3(Engine_ActorSetPosition, 2, 0x1900000, 0x1c00000);
                    } else {
                        Call3(Engine_ActorSetPosition, 2, 0xe00000, 0xa20000);
                    }
                    Engine_ActorFaceEachOther(2, 0, 0);
                    Engine_EventOpenScreen();
                    Engine_EventWaitForScreen();
                    Engine_EventWait(30);
                    Engine_ActorRunRepeatedMotion(2, 2);
                    Engine_EventSetMessage((s32)MsgKuupuappuWelcomeBackTwo);
                    Engine_EventShowMessageAndWait(2, 0, 20);
                    Engine_ActorSetAnimationAndWait(0, 3);
                    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
                    Engine_ActorSetAnimation(2, 2);
                    record = Object_GetById(0);
                    if (record != 0) {
                        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
                    }
                    Engine_ActorWaitForMove(2);
                    Engine_ActorSetPosition(2, 0, 0);
                    Event_PrepareObjectAndApplyValue(2, 0);
                    Engine_EventEnd();
                }
            }
        }
    }
    if (Engine_GameFlagIsSet(0x867) != 0) {
        Call3(Engine_ActorSetPosition, 23, 0x1980000, 0x780000);
    }
    if (gGameState.entrance == 11) {
        if (Engine_GameFlagIsSet(0x855) == 0) {
            if (Engine_GameFlagIsSet(0x856) != 0) {
                if (Engine_GameFlagIsSet(2) == 0) {
                    Engine_EventBegin();
                    Call3(Engine_ActorSetPosition, 2, 0x280000, 0x1360000);
                    Engine_ActorFaceEachOther(2, 0, 0);
                    Engine_EventOpenScreen();
                    Engine_EventWaitForScreen();
                    Engine_EventWait(30);
                    Engine_ActorRunRepeatedMotion(2, 2);
                    Engine_EventSetMessage((s32)MsgKuupuappuWelcomeBackTwo);
                    Engine_EventShowMessageAndWait(2, 0, 20);
                    Engine_ActorSetAnimationAndWait(0, 3);
                    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
                    Engine_ActorSetAnimation(2, 2);
                    record = Object_GetById(0);
                    if (record != 0) {
                        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
                    }
                    Engine_ActorWaitForMove(2);
                    Engine_ActorSetPosition(2, 0, 0);
                    Event_PrepareObjectAndApplyValue(2, 0);
                    Engine_EventEnd();
                }
            }
        }
        Engine_GameFlagClear(0x12f);
    } else {
        if (gGameState.entrance == 13) {
            if (Engine_GameFlagIsSet(0x855) != 0) {
                Engine_ActorSetPosition(20, 0, 0);
            }
        }
    }
    return 0;
}

/*
 * Whether an actor should turn toward a target: the target is within range
 * (or a forced check), and it lies in the actor's facing half of the full
 * circle. A hit activates the actor and queues the face-toward motion; a miss
 * deactivates it and queues the fall-back motion.
 *
 * The local SceneActor layout is this overlay's own copy of the field actor
 * record (facing at 0x08, x/y/z words at 0x0C, active byte at 0x5B). It is
 * not the shared STAGED_ACTOR.H type: the offsets here are what the reference
 * reads, so the spelling is kept as-is.
 */
s32 SceneActor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return Iwram_Sqrt(dxsq + dysq + dzsq);
}

s32 SceneActor_CheckFacingAndRange(struct SceneActor *actor, struct SceneActor *target,
                  s32 range, s32 force)
{
    s32 result = 0;
    s32 *target_pos = &target->x;
    s32 *actor_pos = &actor->x;
    if (SceneActor_GetPositionDistance(target_pos, actor_pos) < range || force != 0) {
        u32 angle = (u16)ArcTan2(target->z - actor->z,
                                       *target_pos - *actor_pos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;
        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Object_SetMode(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Object_SetMode(actor, 2);
    }
    return result;
}

/* Whole owner 020017ac..02001900: exact 340 bytes, including six pool words.
 * Coordinate inputs overlap through x/z loads; y begins after the x sum.
 * Explicit flag-store addresses recover the zero-copy order. Preparing the
 * callback value before the tagged address scope recovers literal/address/
 * publication order. Verified with the approved compiler on 2026-09-27.
 */
void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay)
{
    Engine_EventShowMessage(actor, 0);
    Engine_EventWait(delay);
}

void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay)
{
    Engine_ActorFaceEachOther(actor, cue, 0);
    Engine_EventWait(delay);
}

/*
 * Per-frame step for a projectile. Advance x by its rate and mirror it into
 * the shadow copy, then either follow the vertical rate or fall at a fixed
 * rate depending on the mode word, and finally decay both rates.
 */
void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect)
{
    s32 horizontal_rate;
    s32 vertical_rate;

    effect->x += effect->horizontal_rate;
    effect->shadow_x = effect->x;

    if (effect->mode != 0) {
        effect->y += effect->vertical_rate;
        effect->shadow_y = effect->y;
    } else {
        effect->z += effect->vertical_rate;
        effect->shadow_z = effect->z;
        effect->y += 1024;
        effect->shadow_y = effect->y;
    }

    horizontal_rate = effect->horizontal_rate;
    effect->horizontal_rate = horizontal_rate - Math_Divide(horizontal_rate, 28);
    vertical_rate = effect->vertical_rate;
    effect->vertical_rate = vertical_rate - Math_Divide(vertical_rate, 28);
}

/* Spawn a drifting effect near actor 19: bit 1 of flags picks its plane,
 * bit 0 its direction. */
void KuupuappuMura_SpawnDriftingEffect(s32 flags)
{
    union DriftingObject *leader;
    union DriftingObject *leaf;
    struct FieldSprite *sprite;
    s32 x;
    s32 z;
    s32 zero;

    leader = (union DriftingObject *)Object_GetById(19);
    if (leader == NULL)
        return;
    x = Engine_RandomNext();
    z = Engine_RandomNext();
    x <<= 3;
    x = (u32)x >> 16;
    z <<= 3;
    x -= 4;
    z = (u32)z >> 16;
    z -= 4;
    x <<= 16;
    z <<= 16;
    {
        s32 base_x = leader->motion.x;
        s32 base_z = leader->motion.z;
        s32 base_y;

        x += base_x;
        base_y = leader->motion.y;
        z += base_z;
        leaf = (union DriftingObject *)Engine_ObjectCreate(0xac, x, base_y, z);
    }
    if (leaf == NULL)
        return;
    sprite = leaf->actor.sprite;
    /* The exact burst-particle sibling dispatches the animation/script
     * pair as one random variant, rather than as a boolean flag. */
    switch (Engine_RandomNext() & 1) {
    case 1:
        Object_SetMode(&leaf->actor, 3);
        Engine_ObjectSetScript(&leaf->actor, KuupuappuMura_DriftScriptA);
        break;
    default:
        Object_SetMode(&leaf->actor, 2);
        Engine_ObjectSetScript(&leaf->actor, KuupuappuMura_DriftScriptB);
        break;
    }
    {
        u8 *motion_flags = &leaf->actor.motion_flags;

        zero = 0;
        *motion_flags = zero;
    }
    if (flags & 2) {
        s32 cnt;
        s32 bias;

        cnt = (u32)Engine_RandomNext() % 10 + 5;
        /* FAKEMATCH: reuse the coordinate local for the direction mask. */
        x = 1;
        flags &= x;
        bias = flags ^ x;
        bias <<= 2;
        cnt += bias;
        leaf->motion.vertical_rate = (0x3332 * flags - 0x1999) * cnt;
        cnt = (u32)Engine_RandomNext() % 15 - 7;
        leaf->motion.horizontal_rate = 0x1999 * cnt;
        leaf->motion.mode = zero;
    } else {
        s32 cnt;

        cnt = (u32)Engine_RandomNext() % 10 + 8;
        leaf->motion.horizontal_rate = (0x3332 * flags - 0x1999) * cnt;
        cnt = (u32)Engine_RandomNext() % 14 + 1;
        leaf->motion.vertical_rate = 0x1999 * cnt;
        leaf->motion.mode = 1;
    }
    {
        void (*update)(union FieldObject *) =
            (void (*)(union FieldObject *))SceneEffect_UpdateMotionWithDamping;
        u8 *sprite_flags;

        /* FAKEMATCH: preserve the sprite-flags address scheduling boundary. */
        do {
            sprite_flags = &sprite->flags;
        } while (0);
        leaf->actor.update = update;
        *sprite_flags = 0;
    }
    sprite->priority = leader->actor.sprite->priority;
}
