#ifndef SHIAN_JIIN_TEMPLE_H
#define SHIAN_JIIN_TEMPLE_H
extern struct EventWork *gEventWork;


#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define Scene_GetRecord_2(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_3(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_4(a0) Value1(Engine_ActorGet, a0)
#define Object_SetModeById_1(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) ((void (*)())Engine_ActorSetAnimationAndWait)(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ACTOR_FIELD_108(rec) (*(s32 *)((rec) + 108))

struct EffectRec {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
};

struct EffectWork {
    u8 pad[0x50];
    struct EffectRec *rec;
};

struct SceneRecordHeading {
    u8 pad[6];
    u16 heading;
};

struct SceneActor_02001334 {
    u8 unknown_00[6];
    u16 facing;
};

struct Params {
    s32 unused0;
    s32 field1;
    s32 field2;
    s32 field3;
    u8 pad[24];
};

struct Descriptor_02000484 {
    u32 field0;
    u8 unused[12];
    u32 field16;
    u32 field20;
    u16 field24;
    u16 unused26;
    u32 field28;
    u8 unused32[8];
};

struct Descriptor_020041ec {
    u32 field0;
    u32 field4;
    u8 unused8[16];
    u16 field24;
    u8 unused26[6];
    u8 unused32[8];
};

extern u8 ShianJiin_XianMessages[];

s16 ArcTan2Far(s32, s32);
void Engine_ScheduleCallbackFar();
void Scheduler_RemoveCallbackFar();
void FieldScene_SpawnRandomizedParticle();
void Effect_Spawn();
void Effect_Spawn();
void Effect_Spawn();
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void ShianJiin_SpinAway(void);
void ShianJiin_RunGatheringScene(void);
void ShianJiin_RunMasterScene(s32 arg0);
void ShianJiin_SpinAway(void);
void ShianJiin_RunMasterScene(s32 variant);
void ShianJiin_SpinAway(void);
void ShianJiin_RunMasterScene();
void ShianJiin_RunGatheringScene(void);
void ShianJiin_RunMasterScene();
void FieldScene_RunOpeningAuxiliarySequence(s32 arg0, s32 arg1);
void FieldScene_RunOpeningAuxiliarySequence(s32 arg0, s32 arg1);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void UiText_DrawQuantityPairWithCueFar();

/*
 * resource_39e owner at 0x02001494, 2236 bytes.
 *
 * Second ensemble beat of this overlay's scene script: 233 calls into the
 * shared scene-script helper block, then a two-stage story branch.
 *
 *   - the opening call sets story flag 0x89a;
 *   - stage 1 (Engine_EventChooseYesNo(0, 0) == 0) joins with id 16 and sets flag 0x898
 *     -- the flag the dispatcher at 0x020012e0 tests and 0x02001dbc clears;
 *   - stage 1 otherwise bumps the step counter at +472 of the scene work
 *     record and runs stage 2: its ==0 arm joins with id 18 and also sets
 *     0x898, its else arm bumps that counter again and sets flag 0x899.
 *
 * Both join arms share the tail at 0x02001cb4: Engine_EventShowMessageAndWait(id, 0, 20) with
 * the id carried in r0, then Engine_GameFlagSet(0x898).
 *
 * Midway the beat clears then sets bit 0 of byte +90 of record 16 around a
 * Engine_ActorWalkToAndWait reposition, plays sounds 158/159 with Engine_MapAnimateCells text
 * calls, runs the slot-19/20 sequence (Engine_ActorSetPosition(19/20, 232 << 16,
 * 168 << 16), record +12 = 0xc0000, +60 = 0x80000000, +24 = 0xcccc, +30 of
 * the +80 sub-object = 0x8000, sound 124) that also closes the owners at
 * 0x02001160 and 0x02001dbc, and ends with Engine_ActorEnableActionCallback(12, 0x0200c638) as
 * 0x02001dbc does.
 *
 * Complete owner: `push {r5, lr}` at 0x02001494 through `pop {r5} / pop {r0} /
 * bx r0` at 0x02001d42-0x02001d46, two inline pools plus two trailing pool
 * words ending 0x02001d4f; the next owner (0x02001d50, `push {lr}`) follows.
 * Called once from within this overlay.
 *
 * Call binding. This overlay reaches the shared helpers through its own veneer
 * bank, so every emitted call word names a veneer, not the main-image address.
 * The legacy alias a reference site produces is derived from that site's own
 * position, so one alias can name two different veneers and several aliases
 * can name one veneer. Each helper is therefore declared once, under an alias
 * whose reference sites all resolve to that helper's veneer, and is spelled in
 * the body under the main-image function it reaches. The bound veneer -- and
 * so the emitted call word -- depends only on the alias, never on where the
 * call lands in the candidate.
 *
 * Two call forms in the branch tail carry meaning beyond their arguments. The
 * three sites spelled through Value2 keep the callee's result register live
 * across the argument setup, which orders the r0 write after r1; the sites
 * spelled through Call2 discard it and order r0 first. Both forms appear in
 * the sibling beat at 0x02001dbc.
 *
 * The set of bit 0 in byte +90 reads the byte into its own local before the
 * or, as the staged-actor scene at resource_373:02003fb0 does at the same
 * offset; the paired clear needs no local because an and against 0xfe already
 * holds its mask in a word register.
 *
 * Uncertainty: helper roles and argument roles are read from call shape only;
 * ids, dialogue ids and constants are transcribed. 0x0200c77a, 0x0200c790 and
 * 0x0200c638 are overlay-image data pointers whose contents are unidentified.
 */

/* Veneer aliases this overlay binds; see Call binding above. */

/* The main-image helper each veneer above reaches. */
void FieldScene_RunForwardArcBurst(void);

void FieldScene_RunDescentBurst(void);

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_SetSpeed(actor, horizontal, vertical);
}

static __inline__ void SetPosition(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_WalkToAndWait(actor, horizontal, vertical);
}

/* Field cleared to 0 on each actor record below. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* Newly named raw call sites: each engine function below was reached only
 * through an unresolved veneer island until its own source was read; the
 * per-site macro keeps the site's original calling form. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ u8 *Record1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at +472 of the shared scene work record. */
static __inline__ void Scene_BumpStep(s32 amount)
{
    u8 *work = *(u8 **)&gEventWork;

    *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + amount);
}

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a0, s32 a1, s32 a2)
{
    func(a0, a1, a2);
}

static __inline__ void Scene_Call6(void (*func)(s32, s32, s32, s32, s32, s32),
                                  s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    func(a0, a1, a2, a3, a4, a5);
}

void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                          u32 flags, u8 *extra);

#endif
