/* NONMATCHING: 4708-byte owner 02002360..020035c4, candidate 4704.
 * 2026-09-27 volatile-handle H2, one authorized causal test: make the two
 * pointer slots volatile, leaving pointed-to image bytes ordinary. Feed the
 * derived assignment's value directly to Vram_Load because the complete ROM
 * trace has base-store/base-reload/derived-store and no derived-handle reload.
 * Prediction: two handle stores and one reload, 136-byte frame, origin-x
 * spills at sp+28/+20, preserved producer/copy/release arguments. Inspect
 * the full normalized diff and allocator dump. One compile only; any added
 * handle load, wrong frame, or failed admission closes H2. Preserve the
 * counterexample in a commit, then restore d449103a6's body and stop.
 * Adoption requires the complete 4708-byte owner/pools and all full gates;
 * volatile retention is FAKEMATCH, never original-source evidence or credit.
 * H2 result: 4724/4708 bytes, 1739 differing halfwords, 601 aligned edits.
 * Both handle stores survive at sp+32/+36, but the array address also spills
 * at sp+28 and reloads after Item_LoadIcon. The derived handle is reloaded
 * from [r4,+4] before Vram_Load even when its assignment value is the argument.
 * greg confirms volatile store insn 2964 and volatile reload insn 2966;
 * this adds reads absent from the complete ROM handle trace. Frame is 132,
 * options/velocity are sp+92/+40/+80, and neither origin-x spill survives.
 * Producer, copy and release arguments retain their values; memory trace
 * and frame admission fail. Commit 5229f1010 preserves this tagged witness;
 * the d449103a6 body is restored. H2 stops without another trial or spelling
 * change. Its canonical residual remains 4704/4708 bytes, 1704 differing
 * halfwords and 610 aligned edits; restoring it changes only this header.
 * No adoption, exact-function credit +0, alignment credit +0.
 *
 * 2026-09-27 two-buffer H1: buffers[0] receives the one slot-17 allocation;
 * Item_LoadIcon runs before buffers[1] = buffers[0] + 0x400, then Vram_Load
 * consumes buffers[1] and Heap_Release closes the same slot. ROM stores the
 * base at 020029d0/sp+40 and reloads at 020029da, then stores the derived
 * handle at 020029e6/sp+36; the whole owner has no later derived-handle read.
 * Prediction: both handle stores/reload, +8 frame bytes, unchanged calls.
 * Full normalized result: 4696/4708 bytes, 2100 differing halfwords,
 * 808 aligned halfword edits. Frame grows 120 -> 136 (+16, not +8), options
 * and velocity reach the ROM's sp+96/+44/+84, and origin x spills appear
 * at sp+28/+20 with low r7 counters. Neither origin call setup is exact.
 * RTL models buffers as DI pseudo 37; lreg deletes its second-word write
 * (initial insn 2948), keeping derived address pseudo 623 as Vram_Load arg.
 * greg spills only the first word at sp+36. No derived-handle store survives.
 * Calls and copy arguments are preserved, but the two-store admission gate
 * fails. Commit 1483b3e0e preserves the negative experiment; this body is
 * restored to the 4704/1704/610 canonical baseline. The buffer-owner axis
 * is closed. No origin-lifetime follow-up is admitted;
 * no index, record, declaration or scalar permutations. DONE +0; alignment +0.
 *
 * 2026-09-27 Sol Venus Summit H1: one FieldObject union owns both actor
 * and effect stores. Predict the reference's +48/+28/+44 store order from
 * one aliasing owner. The complete normalized diff is identical to baseline:
 * 4704 bytes / 1704 differing halfwords / 610 edits, still a 120-byte frame.
 * This union already had the same alias information through the old casts;
 * it repairs vocabulary, not emission. No counter/helper/zero permutation
 * follows. Remaining buffer spills and particle-loop lifetimes need a new
 * producer/consumer boundary; typed ownership alone is closed for this model.
 * 1704 differing halfwords, 610 aligned halfword edits (2026-09-27).
 * Complete return 020035a8; final pool 020035ac..020035c4. ROM frame is
 * 136 bytes: second options at sp+44, shared velocity at sp+84, first
 * options at sp+96. Candidate keeps the same record ordering but has a
 * 120-byte frame, fewer buffer/coordinate spills, and different loop roles.
 * Three bounded structural trials (candidate / differences / edits):
 * 1. Independent particle-loop counters: 4704 / 1704 / 612; frame remains
 *    120 and both counters still occupy high registers. Rejected.
 * 2. One aggregate containing second options, velocity, first options:
 *    4696 / 1726 / 609; changes field addressing, leaves frame 120 and
 *    loses eight more bytes. Rejected despite one fewer aligned edit.
 * 3. Halfword zero shared by early byte-field clears but not facing/later
 *    phases: 4688 / 2254 / 705; removes the early zero pools. Rejected.
 * These Sep-26 trials retained the 4704/1704/610 baseline. Reference spills the
 * icon buffer and buffer+0x400 at sp+40/+36, and each particle origin x at
 * sp+28/+20; candidate keeps them in registers. Audit those lifetimes and
 * call prototypes before another experiment. No credit for this draft.
 *
 * Sep-27 H1: one local inline SpawnRadialBurst owns the repeated seventeen-
 * particle loops; caller retains both options and shared velocity scratch.
 * Precedent: exact KARAGORU_DOU/SPINNING_LEAP.C's angle/vector/spawn loop,
 * with this owner's own 3*cos/2*sin ellipse and callback. The canonical
 * typed draft here supersedes stale resource_3c9_c_02002360.c (Sep 25);
 * do not reapply its missing arguments or task-pointer-as-byte fields.
 * Full normalized comparison: 4696/4708, 1733 halfwords, 582 edits, frame
 * still 120. The velocity pointer becomes r6 in both bursts (reference r6),
 * but it is rematerialized instead of retained through r9. Counter remains
 * high (sl), origin remains r8 rather than sl, both origin-x spills and
 * both icon-buffer spills remain absent. Call setup later in the scene
 * also changes; fewer edits do not establish the full ownership model.
 * H1 is preserved at 7364784c3, not accepted from its score alone.
 * H2 keeps both loops and shared scratch in the caller, extracting only
 * MakeBurstVelocity's cosine/zero/sine/scaling operations. Its entire 4704-
 * byte candidate and normalized diff are identical to the Sep-26 baseline:
 * 1704 differing halfwords, 610 edits; frame 120, both loop and icon-buffer
 * spill defects unchanged. This falsifies a narrow polar-helper explanation.
 * Exact entrance-dispatch owner 0200071c calls this no-argument scene in
 * case 2 (site 020007f8). Scene_RunSetupSequence35c4 is exact; callback
 * 020036d0 is a separate 238/244-byte draft, not an exact source precedent.
 * STOP helper-boundary axis after these two complete models. No counter,
 * aggregate, declaration or halfword-zero permutations. DONE +0. */
#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "FIELD_EVENT.H"

extern u8 LinkedValue_Zero;
extern u8 LinkedScene_VinasuChojo;
extern const u8 Data_0200e074[];
extern const u8 Data_0200e088[];
extern const u8 Data_0200e0ac[];
extern const u8 Data_0200e22c[];

void Scene_CallPairWith10(s32 actor, s32 angle);
void State_ApplyArgMode0AndSet10(s32 speaker);
void Scene_RunSetupSequence35c4(void);
void Actor_ParkRecord(struct FieldActor *object);
void Effect_MoveWithDrag(union FieldObject *object);
void SceneTask_020036d0(void);
void SceneTask_020037c4(void);

void Engine_ActorLaunch(s32 actor, s32 speed, s32 frames);
void Engine_ActorSetPositionAndReset(s32 actor, s32 x, s32 z);
void Engine_ActorSetPositionAndCommit(s32 actor, s32 x, s32 z);
void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void Engine_GameStateSetReturn(s32 scene, s32 entrance);
void Engine_GameStateSetWarp(s32 scene, s32 entrance);
void Engine_Import0808a250(s32 first, s32 second);
void Engine_Import08077268(void);

static inline void Actor_Launch(s32 actor, s32 speed, s32 frames)
{
    Engine_ActorLaunch(actor, speed, frames);
}

static inline void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetPositionAndReset(actor, x, z);
}

static inline void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetPositionAndCommit(actor, x, z);
}

static inline void Object_SetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y,
                                      s32 fixed_z)
{
    Engine_ObjectSetPosition(object, fixed_x, fixed_y, fixed_z);
}

static inline void Object_CommitPosition(struct FieldActor *object)
{
    Engine_ObjectCommitPosition(object);
}

/* Both bursts use one velocity scratch; only its polar construction is
 * shared, leaving the two option records and loop lifetimes in the scene. */
static __inline__ void MakeBurstVelocity(s32 velocity[3], s32 angle)
{
    velocity[0] = Math_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle) * 2;
    velocity[0] *= 3;
}

void Scene_RunPairedActorEffectSequence(void)
{
    struct EffectOptions options_a;
    s32 velocity[3];
    struct EffectOptions options_b;
    struct FieldActor *actor;
    struct FieldActor *origin;
    union FieldObject *object;
    struct FieldActor *bird;
    struct FieldSprite *sprite;
    u8 *buffer;
    s32 yes;
    u32 i;

    Event_Begin();
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Actor_Get(1)->facing = 0x8000;
    Actor_SetPosition(1, 0x1480000, 0xa80000);
    Actor_Get(2)->facing = 0x8000;
    Actor_SetPosition(2, 0x1540000, 0xc40000);
    Actor_Get(3)->facing = 0x8000;
    Actor_SetPosition(3, 0x1460000, 0xcc0000);
    Actor_Get(6)->facing = 0x3000;
    Actor_SetPosition(6, 0x10c0000, 0x9a0000);
    Actor_Get(21)->facing = 0x3000;
    Actor_SetPosition(21, 0x10c0000, 0xa40000);
    actor = Actor_Get(20);
    actor->unknown_64 = 10;
    actor->facing = 0xd000;
    Actor_SetPosition(20, 0x1260000, 0xd40000);
    Actor_SetAnimation(20, 9);
    Actor_EnableActionCallback(20, Data_0200e074);
    Actor_SetSpriteFlags(Actor_Get(20), 0);
    actor = Actor_Get(19);
    actor->unknown_64 = 10;
    actor->facing = 0;
    Actor_SetPosition(19, 0x11e0000, 0xc00000);
    Actor_SetAnimation(19, 7);
    Actor_EnableActionCallback(19, Data_0200e074);
    Actor_SetSpriteFlags(Actor_Get(19), 0);
    Event_GetViewCenter()->motion_flags = 0;
    Camera_MoveTo(0x1300000, 0x200000, 0xb40000, 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(80);

    Actor_Launch(1, 2, 20);
    Scene_CallPairWith10(1, 0x2000);
    Event_SetMessage(0x27cf);
    State_ApplyArgMode0AndSet10(0x1001);
    Scene_CallPairWith10(3, 0xa000);
    State_ApplyArgMode0AndSet10(3);
    Actor_TurnToAngle(1, 0x8000, 0);
    Scene_CallPairWith10(2, 0xa000);
    Actor_RunRepeatedMotion(21, 2);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_Get(21)->unknown_5a &= ~1;
    Actor_SetPositionAndReset(21, 0x118, 164);
    Event_Wait(1);
    Actor_Get(21)->unknown_5a |= 1;
    Event_Wait(20);
    Actor_ShowEmote(2, 0x102, 40);
    State_ApplyArgMode0AndSet10(2);
    Actor_SetAnimationAndWait(21, 4);
    State_ApplyArgMode0AndSet10(21);
    Actor_ShowEmote(1, 0x103, 20);
    Actor_RunRepeatedMotion(1, 1);
    State_ApplyArgMode0AndSet10(1);
    Actor_RunRepeatedMotion(21, 1);
    Scene_CallPairWith10(21, 0);
    State_ApplyArgMode0AndSet10(21);
    Actor_StartRepeatedMotion(3, 2);
    State_ApplyArgMode0AndSet10(3);
    Actor_SetAnimation(3, 4);
    State_ApplyArgMode0AndSet10(3);
    Actor_ShowEmote(21, 0x105, 40);
    Scene_CallPairWith10(21, 0x3000);
    State_ApplyArgMode0AndSet10(21);
    Actor_Launch(2, 2, 20);
    State_ApplyArgMode0AndSet10(2);
    Actor_SetAnimationAndWait(21, 3);
    State_ApplyArgMode0AndSet10(21);
    Actor_RunRepeatedMotion(1, 1);
    Scene_CallPairWith10(1, 0x2000);
    Event_OpenMessage(1, 0);
    Actor_TurnToAngle(2, 0xe000, 0);
    Actor_TurnToAngle(3, 0xe000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        yes = TRUE;
    } else {
        gEventWork->message++;
        yes = FALSE;
    }
    Task_Wait(20);
    Actor_TurnToAngle(1, 0x8000, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Scene_CallPairWith10(3, 0xa000);
    Scene_CallPairWith10(21, 0);
    Event_ShowMessage(21, 0);
    Audio_PlayCue(17);
    Event_Wait(40);
    if (yes) {
        gEventWork->message++;
    }

    Actor_Stop(20);
    Actor_Stop(19);
    Task_Wait(1);
    actor = Actor_Get(20);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    actor = Actor_Get(19);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    Task_Wait(1);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(21, 0x3000, 0);
    Actor_TurnToAngle(0, 0x6000, 0);
    Actor_TurnToAngle(1, 0x6000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 40);
    Actor_SetAnimation(20, 1);
    Actor_SetSpriteFlags(Actor_Get(20), 1);
    Event_Wait(20);
    Actor_SetSpeed(20, 0x3333, 0x1999);
    Actor_SetPositionAndReset(20, 0x12c, 206);
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_Get(20)->scale_x = -0x10000;
    Audio_PlayCue(161);
    Actor_SetAnimation(20, 8);
    Event_Wait(20);
    Actor_SetAnimation(19, 1);
    Actor_SetSpriteFlags(Actor_Get(19), 1);
    Actor_Launch(19, 4, 40);
    Actor_SetSpeed(19, 0x3333, 0x1999);
    Actor_Get(19)->unknown_5a &= ~1;
    Actor_SetPositionAndReset(19, 0x128, 186);
    Actor_SetSpeed(19, 0x1999, 0xccc);
    Actor_SetPositionAndReset(19, 0x124, 186);
    Actor_Get(19)->unknown_5a |= 1;
    Actor_Get(19)->scale_x = -0x10000;
    Audio_PlayCue(161);
    Actor_SetAnimation(19, 5);
    Event_Wait(20);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 1);
    Event_Wait(80);
    Actor_SetAnimationAndWait(3, 4);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(40);
    State_ApplyArgMode0AndSet10(19);
    Actor_TurnToAngle(0, 0xa000, 0);
    Actor_TurnToAngle(1, 0x2000, 0);
    Actor_TurnToAngle(2, 0x6000, 0);
    Actor_TurnToAngle(3, 0xe000, 40);
    Actor_TurnToAngle(0, 0x6000, 0);
    Actor_TurnToAngle(1, 0x6000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 20);

    actor = Actor_Get(20);
    actor->facing = 0x3000;
    actor->scale_x = 0x10000;
    Actor_SetAnimation(20, 1);
    Event_Wait(10);
    Scene_CallPairWith10(20, 0xd000);
    Actor_Launch(20, 6, 0);
    Event_Wait(10);
    Actor_TurnToAngle(0, 0xa000, 0);
    Actor_TurnToAngle(1, 0xa000, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Actor_TurnToAngle(3, 0xa000, 0);
    Actor_TurnToAngle(21, 0xd000, 0);
    Actor_TurnToAngle(6, 0, 0);
    object = (union FieldObject *)Object_Create(22, actor->x.fixed, actor->y.fixed + 0x80000, actor->z.fixed);
    if (object != NULL) {
        sprite = object->actor.sprite;
        sprite->part_count = 0;
        sprite->full_color = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        object->actor.priority_flags &= ~1;
        object->actor.motion_flags = 0;
        object->actor.unknown_5c = 1;
        object->actor.speed = 0x19999;
        object->actor.acceleration = 0xcccc;
        buffer = Heap_Allocate(17, 0x608);
        Item_LoadIcon(220);
        Vram_Load(sprite->vram_block, 128, buffer + 0x400);
        Heap_Release(17);
    }
    Actor_SetSpritePriority(22, 1);
    bird = Actor_Get(22);
    bird->x.fixed = actor->x.fixed;
    bird->y.fixed = 0x200000;
    bird->z.fixed = actor->z.fixed;
    bird->motion_flags = 3;
    bird->speed = 0x19999;
    bird->acceleration = 0xcccc;
    bird->scale_x = 0xc000;
    bird->scale_y = 0xc000;
    if (object != NULL) {
        object->actor.motion_flags = 3;
        object->effect.velocity_y = 0x9999;
        object->effect.velocity_x = 0xcccc;
        object->actor.velocity_y = 0x80000;
        Object_SetPosition(&object->actor, 0x1340000, 0x200000, 0xa40000);
    }
    Actor_SetPositionAndCommit(22, 0x134, 164);
    Actor_SetPosition(22, 0, 0);
    Actor_SetSpritePriority(21, 0);
    if (object != NULL) {
        Audio_PlayCue(0x135);
        Actor_SetSpriteFlags(&object->actor, 0);
        object->actor.velocity_y = 0x40000;
        Object_SetPosition(&object->actor, 0x13a0000, 0x200000, 0x890000);
        Object_CommitPosition(&object->actor);
        Audio_PlayCue(0x135);
        object->actor.sprite->priority = 1;
        object->actor.velocity_y = 0x60000;
        Object_SetPosition(&object->actor, 0x11d0000, 0x200000, 0x920000);
        Object_CommitPosition(&object->actor);
        Audio_PlayCue(0x135);
        object->actor.velocity_y = 0x50000;
        Object_SetPosition(&object->actor, 0x12c0000, 0x200000, 0x9a0000);
        Object_CommitPosition(&object->actor);
        Task_Wait(6);
        object->actor.x.fixed = 0;
        object->actor.y.fixed = 0;
        object->actor.z.fixed = 0;
        Actor_ParkRecord(&object->actor);
    }
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 0);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 30);
    Actor_SetSpritePriority(21, 1);
    Actor_Get(21)->priority_flags |= 1;
    Actor_StartRepeatedMotion(2, 2);
    State_ApplyArgMode0AndSet10(2);
    Actor_RunRepeatedMotion(3, 1);
    Event_ShowMessageAndWait(3, 0, 40);
    Actor_RunRepeatedMotion(20, 1);
    Event_Wait(20);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(0, 0x5000, 0);
    Actor_TurnToAngle(1, 0x5000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 0);
    Actor_TurnToAngle(6, 0x3000, 0);
    Scene_CallPairWith10(21, 0x3000);
    Actor_ShowEmote(21, 0x101, 0);
    Actor_ShowEmote(6, 0x101, 0);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 60);
    Actor_ShowEmote(20, 0x108, 40);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(0, 0xa000, 0);
    Actor_TurnToAngle(1, 0x2000, 40);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(1, ANIM_NOD);
        yes = TRUE;
    } else {
        gEventWork->message++;
        Actor_SetAnimation(1, ANIM_SHAKE_HEAD);
        yes = FALSE;
    }
    State_ApplyArgMode0AndSet10(1);
    if (yes) {
        gEventWork->message++;
    }
    Event_Wait(20);

    Actor_SetSpriteFlags(Actor_Get(24), 0);
    Actor_SetChildValue(24, 7);
    actor = Actor_Get(24);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x1999;
    actor->motion_flags = 0;
    actor->y.fixed = 0x400000;
    actor->z.fixed = 0x9e0000;
    actor->x.fixed = 0x1300000;
    Actor_SetSpriteFlags(Actor_Get(25), 0);
    Actor_SetChildValue(25, 7);
    actor = Actor_Get(25);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x1999;
    actor->motion_flags = 0;
    actor->y.fixed = 0x600000;
    actor->x.fixed = 0x1300000;
    actor->z.fixed = 0x9e0000;
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 0);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 0);
    Actor_TurnToAngle(1, 0xa000, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Actor_TurnToAngle(3, 0xa000, 0);
    Actor_TurnToAngle(21, 0xd000, 0);
    Actor_TurnToAngle(6, 0, 0);
    Actor_EnableActionCallback(24, Data_0200e088);
    Actor_EnableActionCallback(25, Data_0200e088);
    Audio_PlayCue(145);
    MapRender_SetValues(0x60000, 0x60000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    ColorBuffer_Interpolate(16);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(24);
    Task_Wait(60);
    Task_AddCallback(SceneTask_020036d0, TASK_PRIORITY_SCENE);
    Audio_PlayCue(141);
    MapRender_SetValues(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    ColorBuffer_Interpolate(120);
    Actor_EnableActionCallback(24, Data_0200e0ac);
    Actor_EnableActionCallback(25, Data_0200e0ac);
    Task_Wait(120);
    MapRender_SetValues(0x30000, 0x30000, 0x10000);
    ColorBuffer_ApplyTarget(0x203210, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    Audio_PlayCue(63);
    MapRender_SetValues(0x20000, 0x20000, 0x10000);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    MapRender_SetValues(0x10000, 0x10000, 0x10000);

    Actor_SetAnimation(19, 1);
    Actor_TurnToAngle(19, 0, 0);
    Actor_Get(19)->scale_x = 0x10000;
    Actor_Launch(19, 4, 40);
    Actor_RunRepeatedMotion(19, 1);
    Event_ShowMessageAndWait(19, 0, 20);
    Actor_TurnToAngle(0, 0x6000, 0);
    Actor_TurnToAngle(1, 0x6000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 0);
    Actor_TurnToAngle(21, 0x3000, 0);
    Actor_TurnToAngle(6, 0x3000, 20);
    Actor_RunRepeatedMotion(20, 1);
    State_ApplyArgMode0AndSet10(20);
    Actor_RunRepeatedMotion(3, 1);
    Scene_CallPairWith10(3, 0x8000);
    State_ApplyArgMode0AndSet10(3);
    Scene_CallPairWith10(20, 0);
    Actor_SetAnimationAndWait(20, 4);
    State_ApplyArgMode0AndSet10(20);
    Actor_ShowEmote(19, 0x103, 20);
    Actor_StartRepeatedMotion(19, 2);
    State_ApplyArgMode0AndSet10(19);
    Actor_ShowEmote(1, 0x100, 20);
    Scene_CallPairWith10(1, 0x6000);
    State_ApplyArgMode0AndSet10(1);
    Scene_CallPairWith10(20, 0xd000);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 20);
    Actor_RunRepeatedMotion(1, 1);
    State_ApplyArgMode0AndSet10(1);
    Actor_TurnToAngle(19, 0xb000, 20);
    Actor_RunRepeatedMotion(19, 1);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(2, 1);
    Actor_RunRepeatedMotion(3, 1);
    Actor_TurnToAngle(0, 0x8000, 0);
    Actor_TurnToAngle(1, 0x8000, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Scene_CallPairWith10(3, 0xa000);
    Actor_ShowEmote(21, 0x101, 80);
    State_ApplyArgMode0AndSet10(21);
    Actor_TurnToAngle(0, 0xa000, 0);
    Actor_TurnToAngle(1, 0x2000, 0);
    Actor_TurnToAngle(2, 0x6000, 0);
    Actor_TurnToAngle(3, 0xe000, 40);
    Actor_RunRepeatedMotion(20, 1);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(0, 0x8000, 0);
    Actor_TurnToAngle(1, 0x8000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 20);
    Scene_CallPairWith10(20, 0xb000);
    State_ApplyArgMode0AndSet10(0x2014);
    Actor_SetAnimationAndWait(21, 3);
    Actor_TurnToAngle(21, 0xb000, 60);
    Actor_TurnToAngle(21, 0x3000, 40);
    Actor_RunRepeatedMotion(19, 1);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_TurnToAngle(21, 0, 40);
    State_ApplyArgMode0AndSet10(21);
    Actor_TurnToAngle(20, 0xd000, 40);
    Scene_CallPairWith10(20, 0xb000);
    State_ApplyArgMode0AndSet10(0x2014);
    Actor_TurnToAngle(21, 0x3000, 40);
    Actor_TurnToAngle(21, 0x3000, 20);
    Actor_SetAnimationAndWait(21, 3);
    Actor_TurnToAngle(19, 0, 40);
    Actor_TurnToAngle(19, 0xb000, 20);
    Actor_ShowEmote(19, 0x101, 40);
    State_ApplyArgMode0AndSet10(0x2013);
    Scene_CallPairWith10(21, 0x3000);
    Actor_ShowEmote(21, 0x101, 20);
    State_ApplyArgMode0AndSet10(21);
    Actor_TurnToAngle(20, 0xb000, 20);
    Actor_SetAnimation(19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_ShowEmote(21, 0x103, 80);
    State_ApplyArgMode0AndSet10(21);
    Actor_SetAnimationAndWait(19, 3);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    Actor_StartRepeatedMotion(21, 2);
    State_ApplyArgMode0AndSet10(21);
    Actor_SetAnimation(19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    Actor_ShowEmote(21, 0x102, 60);
    Actor_RunRepeatedMotion(21, 1);
    State_ApplyArgMode0AndSet10(21);
    Actor_ShowEmote(20, 0x108, 40);
    Actor_SetAnimation(20, 4);
    Event_ShowMessageAndWait(0x2014, 0, 40);
    Actor_RunRepeatedMotion(21, 1);
    Event_Wait(20);
    State_ApplyArgMode0AndSet10(21);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 0);
    Actor_TurnToAngle(2, 0xa000, 0);
    Actor_TurnToAngle(3, 0xa000, 20);
    Actor_TurnToAngle(19, 0x3000, 20);
    Actor_SetAnimation(19, 3);
    Actor_SetAnimationAndWait(20, 3);
    Actor_TurnToAngle(21, 0xb000, 20);
    State_ApplyArgMode0AndSet10(0xa015);
    Actor_SetSpeed(6, 0x10000, 0x8000);
    Actor_SetSpeed(21, 0x10000, 0x8000);
    Actor_EnableActionCallback(21, Data_0200e22c);
    Event_Wait(20);
    Actor_EnableActionCallback(6, Data_0200e22c);
    Scene_CallPairWith10(1, 0x6000);
    Actor_StartRepeatedMotion(1, 2);
    State_ApplyArgMode0AndSet10(1);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x40250d, 1);
    ColorBuffer_Interpolate(40);
    Scene_CallPairWith10(20, 0xd000);
    State_ApplyArgMode0AndSet10(20);
    Actor_TurnToAngle(0, 0x6000, 0);
    Actor_TurnToAngle(2, 0x8000, 0);
    Actor_TurnToAngle(3, 0x8000, 0);
    Scene_RunSetupSequence35c4();
    Actor_SetChildValue(20, 7);
    Actor_SetChildValue(19, 7);
    Event_Wait(20);
    Actor_SetChildValue(20, 0x100);
    Actor_SetChildValue(19, 0x100);
    Event_Wait(20);
    Scene_RunSetupSequence35c4();
    Actor_Launch(3, 2, 20);
    State_ApplyArgMode0AndSet10(3);
    Scene_CallPairWith10(19, 0);
    State_ApplyArgMode0AndSet10(19);
    Scene_RunSetupSequence35c4();

    origin = Actor_Get(19);
    options_a.palette = 7;
    options_a.update = Effect_MoveWithDrag;
    options_a.start_scale_x = 0x10000;
    options_a.start_scale_y = 0x10000;
    for (i = 0; i <= 16; i++) {
        MakeBurstVelocity(velocity, i << 12);
        Effect_Spawn(origin->x.fixed, origin->y.fixed, origin->z.fixed, velocity[0], velocity[1],
                     velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                     &options_a);
    }
    Audio_PlayCue(212);
    Task_Wait(6);
    Scene_RunSetupSequence35c4();
    origin = Actor_Get(20);
    options_b.palette = 7;
    options_b.update = Effect_MoveWithDrag;
    options_b.start_scale_x = 0x10000;
    options_b.start_scale_y = 0x10000;
    for (i = 0; i <= 16; i++) {
        MakeBurstVelocity(velocity, i << 12);
        Effect_Spawn(origin->x.fixed, origin->y.fixed, origin->z.fixed, velocity[0], velocity[1],
                     velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                     &options_b);
    }
    Audio_PlayCue(212);
    Actor_Launch(2, 6, 20);
    Audio_PlayCue(54);
    State_ApplyArgMode0AndSet10(2);
    Actor_SetAnimation(20, 4);
    State_ApplyArgMode0AndSet10(20);
    Scene_RunSetupSequence35c4();
    Actor_SetSpeed(20, 0x3333, 0x1999);
    Actor_SetSpeed(19, 0x3333, 0x1999);
    Actor_Get(20)->unknown_5a &= ~1;
    Actor_Get(19)->unknown_5a &= ~1;
    Actor_SetDestination(20, 0x126, 196);
    Actor_SetDestination(19, 0x126, 196);
    Task_AddCallback(SceneTask_020037c4, TASK_PRIORITY_SCENE);
    Actor_StartRepeatedMotion(1, 2);
    Event_ShowMessage(1, 0);
    GameFlag_Set(0x234);
    Event_ShowMessage(2, 0);
    GameFlag_Set(0x235);
    Scene_RunSetupSequence35c4();
    Event_Wait(20);
    Scene_RunSetupSequence35c4();
    Event_Wait(20);
    ((u8 *)&gGameState)[0x22b] = 3;
    Engine_GameStateSetReturn((s32)&LinkedScene_VinasuChojo, 3);
    Engine_GameStateSetWarp((s32)&LinkedScene_VinasuChojo, 9);
    Engine_Import0808a250(98, 0);
    Engine_Import08077268();
    GameFlag_Set(0x351);
}
