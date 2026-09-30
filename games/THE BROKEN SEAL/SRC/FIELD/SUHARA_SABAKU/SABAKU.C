/* The Suhara desert: the phase request and two object toggles. */
#include "SABAKU.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "CALL.H"
#include "FIELD_EFFECT.H"
#include "SCENE_IDS.H"

extern const struct SceneEntrance gSuharaSabakuEntrances1[];
extern const struct SceneEntrance gSuharaSabakuEntrances2[];
extern const struct SceneEntrance gSuharaSabakuEntrances3[];
extern const struct SceneEntrance gSuharaSabakuEntrancesOther[];

extern const struct ScenePlacement gSuharaSabakuPlacements1[];
extern const struct ScenePlacement gSuharaSabakuPlacements2[];
extern const struct ScenePlacement gSuharaSabakuPlacements3[];
extern const struct ScenePlacement gSuharaSabakuPlacementsOther[];

extern u8 MsgSuharaSandstorm[];

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

extern u8 MsgSuharaNowhereFound[];

void BattleFx_SetWeightedResult(s32 actor, s32 mode);

/* The game state seen as rows, as this handler addresses it. */
union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

void Event_SetPairWork1c0(s32 scene, s32 entrance);

extern const struct SceneEvent gSuharaSabakuEvents3[];
extern const struct SceneEvent gSuharaSabakuEventsOther[];

/* The IWRAM field globals: the map work first, the event work at +0x4c. */
struct FieldGlobals {
    u8 *map;
    u8 unknown_04[0x48];
    struct EventWork *event;
};

extern struct FieldGlobals gMapWork;

void QueueSceneSound(s32 cue);

/* QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C) written out in place, with
 * that function's one-pass loops around the IME read and the queue update. */
#define QUEUE_IO_WRITE(address, value)                                      \
    q = &gIoWriteQueue;                                                     \
    do {                                                                    \
        do {                                                                \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        limit = 31;                                                         \
        count = q->count;                                                   \
        if (count <= limit) {                                               \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
                                                                            \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        *ime = saved;                                                       \
    } while (0)

void SceneState_SendRequest15With45(void)
{
    BattleFx_SetPhaseRequest(15, 45);
}

s32 OverlayObject_ApplyZeroAndClearByte89(u8 *obj)
{
    Actor_SetSpriteFlags(obj, 0);
    obj[89] = 0;
    return 0;
}

s32 OverlayObject_ToggleField84Bit0(u8 *obj)
{
    obj[84] ^= 1;
    return 1;
}

/* Where the party appears in each of the desert's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_SuharaSabaku1) {
        return gSuharaSabakuEntrances1;
    }
    if (selector == (s32)&SceneId_SuharaSabaku2) {
        return gSuharaSabakuEntrances2;
    }
    if (selector == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuEntrances3;
    }
    return gSuharaSabakuEntrancesOther;
}

/* The Suhara desert: the empty table slot and the exits. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SuharaSabaku_GetExits(void)
{
    return gSuharaSabakuExits;
}

/* The actors placed in each of the desert's three areas. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_SuharaSabaku1) {
        return gSuharaSabakuPlacements1;
    }
    if (selector == (s32)&SceneId_SuharaSabaku2) {
        return gSuharaSabakuPlacements2;
    }
    if (selector == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuPlacements3;
    }
    return gSuharaSabakuPlacementsOther;
}

/* The Suhara desert: the selected actor's progress and the middle steps. */
void SuharaSabaku_SyncSelectedActorProgress(void)
{
    struct Actor_02000400 *actor;
    struct SceneWork_02000400 *scene;
    s32 progress;

    actor = Actor_Get(((struct Selection_02000400 *)((s16 *)&gGameState))->actor_id);
    scene = *(struct SceneWork_02000400 **)((u8 *)&gEventWork);
    actor->presentation = (u16)(gFrameCount << 12);

    progress = GameFlag_GetByte(0x210);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (GameFlag_IsSet(0x106) == 0) {
            progress -= 1;
        }
    }
    GameFlag_SetByte(0x210, progress);
}

void FieldScene_RunMiddleAuxiliarySequence(s32 a0)
{
    s32 p10;
    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = (u8 *)((s16 *)&gGameState);
    p6 = *(u8 **)(base + 500);
    p10 = a0;
    rec7 = Object_GetById((s32)p6);
    Actor_Get(p10);
    rec2 = GameFlag_IsSet(0x20f);
    if (rec2 == 0) {
        Event_Begin();
        Actor_SetAttachedEffect((s32)p6, 0x101);
        Actor_SetAnimation((s32)p6, 9);
        record = Object_GetById(p10);
        if (record != 0) {
            Actor_SetDestination((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove((s32)p6);
        Audio_PlayCue(244);
        Engine_TaskAddCallback((s32)SuharaSabaku_SyncSelectedActorProgress, 0xc80);
        rec7[85] = rec2;
        Engine_ObjectSetPosition((s32)rec7, *(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12) + 0x200000, *(s32 *)(rec7 + 16));
        Actor_WaitForMove((s32)p6);
        *(s32 *)(rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        GameFlag_Set(0x20f);
        GameFlag_SetByte(0x218, p10);
        GameFlag_SetByte(0x210, 180);
        Event_End();
        *(u16 *)(*(u8 **)((u8 *)&gEventWork) + 0x17c) = rec2;
    }
}

void FieldScene_RunActor8Step(void) { FieldScene_RunMiddleAuxiliarySequence(8); }

void FieldScene_RunActor9Step(void) { FieldScene_RunMiddleAuxiliarySequence(9); }

void FieldScene_RunActor10Step(void) { FieldScene_RunMiddleAuxiliarySequence(10); }

void FieldScene_RunActor11Step(void) { FieldScene_RunMiddleAuxiliarySequence(11); }

void FieldScene_RunActor12Step(void) { FieldScene_RunMiddleAuxiliarySequence(12); }

/* FAKEMATCH: a one-pass IME wrapper and a signed queue limit preserve the
 * queued write order, as in VISIT_BLEND.C. */
void SuharaSabaku_RestoreActorScaleAndBlend(s32 a0)
{
    s32 i;
    s32 next_frame;
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    u16 count;
    s32 limit;
    s32 p10;
    s32 p10b;
    /* FAKEMATCH: x precedes z to break their equal allocation priority. */
    s32 p9;
    s32 p11;
    s32 p9b;
    struct FieldActor *rec;
    s32 rec2;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 v2;
    s32 queue_pos;
    s32 base6_4000208;
    s32 v0;
    s32 v6;

    p10 = a0;
    rec7 = Object_GetById(gGameState.selected_actor);
    rec = Object_GetById(p10);
    rec2 = Engine_GameFlagIsSet(0x340);
    p9 = rec7->x.part.pixel;
    p11 = rec7->z.part.pixel;
    ime = &REG_IME;
    Engine_EventBegin();
    Engine_AudioPlayCue(244);
    i = 0;
    do {
        rec->scale_x = ((i << 11) + 0x800);
        rec->scale_y = ((i << 12) + 0x1000);
        if (rec2 == 0) {
            q = &gIoWriteQueue;
            next_frame = i + 1;
            do {
                saved = *ime;
            } while (0);
            *ime = (u16)ime;
            limit = 31;
            count = q->count;
            if (count <= limit) {
                u32 *dst = (u32 *)((u8 *)q + count * 12 + 4);

                *(u16 *)&q->count = count + 1;
                *dst++ = ((16 - next_frame) << 8) | next_frame;
                *dst++ = 0x04000052;
                *dst = 0x20000;
            }
            *ime = saved;
        } else {
            next_frame = i + 1;
        }
        Engine_TaskWait(1);
        i = next_frame;
    } while (i <= 15);
    Engine_GameFlagSet((0x1fe + p10));
    Engine_GameFlagSet(0x340);
    if (Engine_GameFlagIsSet(0x9a0) == 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x9b6) != 0) {
        } else {
            Engine_GameFlagSet(0x9b6);
            Call3(Engine_ActorSetSpeed, 13, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            record = Object_GetById(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(13, record->x.fixed, record->z.fixed);
            }
            Call3(Engine_ActorFaceActor, 13, 0x4000, 0);
            Object_GetById(13)->unknown_5a &= 254;
            Engine_ActorWalkTo(13, p9, (p11 - 16));
            Object_GetById(0)->unknown_5a &= 254;
            Engine_ActorWalkToAndWait(0, (p9 + 8), (p11 - 40));
            Engine_EventWait(1);
            SetFlagBits(&Object_GetById(0)->unknown_5a, 1);
            Engine_ActorSetAnimation(13, 1);
            Engine_ActorFaceDirection(0, 0x4000, 0);
            Engine_EventSetMessage((s32)MsgSuharaSandstorm);
            Call3(Engine_ActorShowEmote, 13, 0x106, 60);
            Engine_EventShowMessage(13, 0);
            Call3(Engine_ActorShowEmote, 13, 0x102, 60);
            Engine_EventShowMessage(13, 0);
            Call3(Engine_ActorFaceDirection, 13, 0x2000, 0);
            Call3(Engine_ActorShowEmote, 13, 0x101, 60);
            Engine_EventShowMessage(13, 0);
            Engine_EventWait(10);
            Call3(Engine_ActorFaceDirection, 13, 0xc000, 30);
            Engine_ActorRunRepeatedMotion(13, 2);
            Engine_EventShowMessage(13, 0);
            Engine_ActorSetAnimationAndWait(13, 3);
            Engine_EventShowMessage(13, 0);
            Call3(Engine_ActorSetSpeed, 13, 0x10000, 0x8000);
            Engine_ActorSetAnimation(13, 2);
            record = Object_GetById(0);
            if ((s32)record != 0) {
                Engine_ActorSetDestination(13, record->x.part.pixel, record->z.part.pixel);
            }
            Engine_ActorWaitForMove(13);
            Engine_ActorSetPosition(13, 0, 0);
        }
    }
    Engine_EventEnd();
}

/* The Suhara desert: the late steps and actor 13's restoration. */
void FieldScene_RunLateActor8Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(8); }

void FieldScene_RunLateActor9Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(9); }

void FieldScene_RunLateActor10Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(10); }

void FieldScene_RunLateActor11Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(11); }

void FieldScene_RunLateActor12Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(12); }

void FieldScene_RunActorThirteenRestoration(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet(0x9a0) == 0) {
    } else {
        if (GameFlag_IsSet(0x1b7) != 0) {
        } else {
            if (GameFlag_IsSet(0x9b0) == 0) {
            } else {
                GameFlag_Set(0x9b5);
                Event_Begin();
                Event_SetMessage((s32)MsgSuharaNowhereFound);
                /* Record layout observed here: s32 at +8, s32 at +16. */
                record = Actor_Get(ACTOR_PARTY_LEADER);
                if (record != 0) {
                    Actor_SetPosition(ACTOR_ID, *(s32 *)(record + 8), *(s32 *)(record + 16));
                }
                Actor_FaceActor(ACTOR_ID, 0xc000, 0);
                Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 0x4e8);
                Actor_FaceDirection(ACTOR_ID, 0x4000, 0);
                Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1bc, 0x4d8);
                Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 40);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
                Actor_SetAnimationAndWait(ACTOR_ID, 4);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
                Actor_ShowEmote(ACTOR_ID, 0x105, 60);
                Event_ShowMessage(ACTOR_ID, 0);
                Event_Wait(30);
                Actor_RunRepeatedMotion(ACTOR_ID, 2);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_FaceDirection(ACTOR_ID, 0xc000, 30);
                Event_AskYesNo(ACTOR_ID, 0);
                Event_Wait(30);
                Actor_ShowEmote(ACTOR_ID, 0x106, 60);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetAnimationAndWait(ACTOR_ID, 3);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetSpeed(ACTOR_ID, 0xb333, 0x5999);
                Actor_WalkToAndWait(ACTOR_ID, 0x1b8, 0x4e8);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
                Actor_SetAnimation(ACTOR_ID, 2);
                /* Record layout observed here: s16 at +10, s16 at +18. */
                record = Actor_Get(ACTOR_PARTY_LEADER);
                if (record != 0) {
                    Actor_SetDestination(ACTOR_ID, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Actor_WaitForMove(ACTOR_ID);
                Actor_SetPosition(ACTOR_ID, 0, 0);
                Event_End();
            }
        }
    }
}

s32 SuharaSabaku_FollowLeaderWithSparks(struct FieldActor *actor)
{
    struct EffectOptions options;
    struct FieldActor *leader;
    s32 dx;
    s32 dz;
    s32 phase;

    leader = Object_GetById(gGameState.selected_actor);
    actor->acceleration = 0x4000;
    actor->speed = 0x18000;
    actor->motion_flags = 0;
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->active ^= 1;
    if (Engine_GameFlagIsSet(0x104)) {
        actor->target_x = ACTOR_NO_TARGET;
        actor->target_y = ACTOR_NO_TARGET;
        actor->target_z = ACTOR_NO_TARGET;
    } else {
        actor->target_x = leader->x.fixed;
        actor->target_y = *(s32 *)leader->unknown_14;
        actor->target_z = leader->z.fixed;
        dx = actor->x.fixed - leader->x.fixed;
        if (dx < 0) {
            dx = leader->x.fixed - actor->x.fixed;
        }
        dz = actor->z.fixed - leader->z.fixed;
        if (dx + (dz >= 0 ? actor->z.fixed - leader->z.fixed : leader->z.fixed - actor->z.fixed) < 0x80000) {
            struct EventWork *work = gEventWork;

            if (leader->motion_flags != 0) {
                work->raised_trigger = 55;
            }
            actor->motion_flags = 3;
            actor->target_x = leader->x.fixed;
            actor->target_y = leader->y.fixed;
            actor->target_z = leader->z.fixed;
        }
    }
    phase = gFrameCount & 7;
    if (phase == 0) {
        options.start_scale_x = 0xcccc;
        options.start_scale_y = 0xcccc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase, phase, 0x880001, &options);
    }
    return 1;
}

/* The Suhara desert: actor 12's place and the effect state byte. */
void PlaceActorTwelveWhenFlagClear(void)
{
    if (GameFlag_IsSet(2487) == 0) {
        GameFlag_Set(526);
        PlaceActor(12, 240 << 15, 206 << 18);
        Engine_ActorEnableActionCallback(12, gSuharaSabakuActor12Action);
    }
}

void SceneState_SetStateByte52(void)
{
    u8 *state = *(u8 **)gEffectWork;
    state[52] = 1;
}

/* The Suhara desert: meeting one of the stranded actors 8 to 12. */
void SuharaSabaku_MeetActor(s32 a0, s32 actor)
{
    register s32 a1 asm("r6") = actor; /* FAKEMATCH: pins the actor id to r6 */
    union GameStateRows *state;

    if (gEventWork->raised_trigger == 99) {
        {
            u16 *target = (u16 *)&gEventWork->raised_trigger;
            s32 shown = 0;

            *target = shown;
        }
    }
    Engine_GameFlagClear(0x20f);
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1) {
        Engine_GameFlagSet((a1 + 0x2f9));
    } else {
        if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
            Engine_GameFlagSet((a1 + 0x309));
        }
    }
    GameFlag_SetByte(0x210, 0);
    BattleFx_SetWeightedResult(98, 5);
    /* FAKEMATCH: publish the byte before retaining the scene pointer. */
    do {
        ((union GameStateRows *)&gGameState)->bytes[277][1] = 3;
    } while (0);
    state = (union GameStateRows *)&gGameState;
    if (state->halves[224][0] == (s32)&SceneId_SuharaSabaku2) {
        if (a1 == 11) {
            BattleFx_SetWeightedResult(98, 7);
        } else {
            if (a1 == 12) {
                BattleFx_SetWeightedResult(98, 6);
                Engine_ActorStop(12);
                Engine_ActorSetPosition(12, 0, 0);
            }
        }
    }
    Object_GetById(state->words[125])->motion_flags = 3;
}

/* The sand swallows the leader and the actor flag byte 0x218 names: cue 219,
 * both sink for sixty frames, the screen closes, flag 0x122 is set and the
 * party leaves for the world map, at entrance 77 from the second area when
 * the flag byte names actor 11 and at entrance 27 otherwise. */
void SuharaSabaku_DropAndLeave(void)
{
    s32 other;
    struct FieldActor *leader;
    struct FieldActor *partner;
    s32 i;

    other = GameFlag_GetByte(0x218);
    leader = Object_GetById(gGameState.selected_actor);
    partner = Object_GetById(other);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_AudioPlayCue(219);
    Engine_ActorSetSpriteFlags((struct FieldActor *)gGameState.selected_actor, 0);
    partner->motion_flags = 0;
    leader->motion_flags = 0;
    leader->velocity_y = 0;
    leader->unknown_5d[4] = 1;
    partner->unknown_5d[4] = 1;
    for (i = 0; i < 60; i++) {
        leader->velocity_y += 0x3333;
        partner->velocity_y += 0x3333;
        Engine_TaskWait(1);
    }
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
    Engine_GameFlagSet(0x122);
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2 && GameFlag_GetByte(0x218) == 11) {
        Event_SetPairWork1c0((s32)&SceneId_WorldMap, 77);
    } else {
        Event_SetPairWork1c0((s32)&SceneId_WorldMap, 27);
    }
}

/* The Suhara desert: the encounter palette's pulse. */
void EncounterPalette_Pulse(void)
{
    u16 phase = gFrameCount & 63;
    s32 level;

    if (phase > 31)
        phase = 64 - phase;
    level = (phase >> 1) + 7;
    level |= (level << 10) | (level << 5);
    EncounterPalette = ((u32)level << 16) >> 16;
}

/* The second desert area's encounter: actor 14 and map object 100 stand
 * aside while the encounter palette pulses. */
s32 FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 record;

    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        Object_GetById(14)->priority_flags = 2;
        Object_GetById(14)->motion_flags = 3;
        Actor_SetPosition(14, 0, 0);
        Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
        MapObject_SetPosition(100, 0, 0);
        Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
        Value6(Engine_MapCopyCellAttributes, 11, 71, 1, 1, 12, 71);
        /* FAKEMATCH: an empty scheduling barrier after the third cell copy
         * gives the game's argument order r1, r2, r3, r0. */
        do {
        } while (0);
        record = Task_RemoveCallback(EncounterPalette_Pulse);
        {
            /* FAKEMATCH: a forced temporary gives the game's load order. */
            s32 shown = gSuharaSabakuShownLevel;

            EncounterPalette = shown;
        }
        return record;
    }
}

s32 FieldScene_RunScene3c0SequenceA(void)
{
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        Object_GetById(14)->priority_flags = 2;
        Object_GetById(14)->motion_flags = 0;
        Actor_SetPosition(14, 0xf80000, 0x2c80000);
        Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
        MapObject_SetPosition(100, -1, -1);
        BattleFx_EmitRandomParticle();
        Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
        return Task_AddCallback(EncounterPalette_Pulse, 0xc80);
    }
}

/* The third area answers its own events; the others share one table. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuEvents3;
    }
    return gSuharaSabakuEventsOther;
}

/* The desert's scene script: it opens the screen, starts the leader's
 * progress task when flag byte 0x210 is set, keeps the encounter palette's
 * level in the first two areas and blends the visit or altar flags there;
 * the third area plays cue 0x120. */
s32 SuharaSabaku_RunSceneScript(void)
{
    u8 *map;

    map = gMapWork.map;
    gMapWork.event->start_transition = 0x201;
    if (GameFlag_GetByte(0x210) != 0) {
        gGameState.movement_mode = 2;
        Task_AddCallback(SuharaSabaku_SyncSelectedActorProgress, 0xc80);
    }
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1
        || gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        gSuharaSabakuShownLevel = EncounterPalette;
        FieldScene_RunOpeningAuxiliarySequence();
    }
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1) {
        SuharaSabaku_ApplyVisitFlagBlend();
    } else if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        SuharaSabaku_ApplyAltarFlagBlend();
    } else {
        Audio_PlayCue(0x120);
    }
    if (gGameState.entrance == 0) {
        *(u16 *)(map + 20) &= ~0x200;
    }
    return 0;
}

/* Raises flags 0x206-0x20a for each of flags 0x301-0x305 already set,
 * resets the scale of actors 8 to 12 unless flag 0x109 is set, then queues
 * the blend setup: full fade-in weights when flag 0x340 is set.
 *
 * FAKEMATCH: the queue bound is a variable, so the comparison stays signed
 * on the u16 count as the ROM's bgt has it, and the queue writes keep the
 * one-pass loops of QueueIoWriteDelay2. */
void SuharaSabaku_ApplyVisitFlagBlend(void)
{
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    u16 count;
    s32 limit;
    struct FieldActor *actor;
    s32 id;
    s32 alpha = 0;

    if (Engine_GameFlagIsSet(0x301)) {
        Engine_GameFlagSet(0x206);
    }
    if (Engine_GameFlagIsSet(0x302)) {
        Engine_GameFlagSet(0x207);
    }
    if (Engine_GameFlagIsSet(0x303)) {
        Engine_GameFlagSet(0x208);
    }
    if (Engine_GameFlagIsSet(0x304)) {
        Engine_GameFlagSet(0x209);
    }
    if (Engine_GameFlagIsSet(0x305)) {
        Engine_GameFlagSet(0x20a);
    }
    for (id = 8; id <= 12; id++) {
        actor = Object_GetById(id);
        if (actor != 0) {
            if (!Engine_GameFlagIsSet(0x109)) {
                actor->scale_x = 0x800;
                actor->scale_y = 0x800;
            }
            actor->sprite->flags = 0;
        }
    }
    QUEUE_IO_WRITE(0x04000050, 0x3f42);
    if (Engine_GameFlagIsSet(0x340)) {
        alpha = 16;
        QueueSceneSound(244);
    }
    QUEUE_IO_WRITE(0x04000052, ((16 - alpha) << 8) | alpha);
}

/* Raises flags 0x206-0x208 for each of flags 0x311-0x313 already set and
 * 0x9b7 for 0x315, resets the scale of actors 8 to 10 unless flag 0x109 is
 * set, shows actor 11's second part, then queues the blend setup as
 * SuharaSabaku_ApplyVisitFlagBlend does.
 *
 * FAKEMATCH: the queue bound is a variable, so the comparison stays signed
 * on the u16 count as the ROM's bgt has it, and the queue writes keep the
 * one-pass loops of QueueIoWriteDelay2. */
void SuharaSabaku_ApplyAltarFlagBlend(void)
{
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    u16 count;
    s32 limit;
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 id;
    s32 alpha = 0;

    if (Engine_GameFlagIsSet(0x311)) {
        Engine_GameFlagSet(0x206);
    }
    if (Engine_GameFlagIsSet(0x312)) {
        Engine_GameFlagSet(0x207);
    }
    if (Engine_GameFlagIsSet(0x313)) {
        Engine_GameFlagSet(0x208);
    }
    for (id = 8; id <= 10; id++) {
        actor = Object_GetById(id);
        if (actor != 0) {
            if (!Engine_GameFlagIsSet(0x109)) {
                actor->scale_x = 0x800;
                actor->scale_y = 0x800;
            }
            sprite = actor->sprite;
            sprite->flags = 0;
        }
    }
    actor = Object_GetById(11);
    if (actor != 0) {
        u8 *part;

        sprite = actor->sprite;
        part = *(u8 **)((u8 *)sprite + 40);
        if (part != 0) {
            part[5] = 10;
        }
        sprite->unknown_20[5] = 1;
        sprite->flags = 0;
    }
    if (Engine_GameFlagIsSet(0x315)) {
        Engine_GameFlagSet(0x9b7);
    }
    QUEUE_IO_WRITE(0x04000050, 0x3f42);
    if (Engine_GameFlagIsSet(0x340)) {
        alpha = 16;
        QueueSceneSound(244);
    }
    QUEUE_IO_WRITE(0x04000052, ((16 - alpha) << 8) | alpha);
}
