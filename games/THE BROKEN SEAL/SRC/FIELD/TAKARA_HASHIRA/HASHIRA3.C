#include "EDITION.H"
#include "HASHIRA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"
#include "FIELD_SCENE.H"

void FieldScene_RunScene3b3_02001fd4(void);
s32 FieldScene_RunScene3b3SequenceD(void);
void FieldEffect_UpdateGridPlacement(void);

void Engine_ObjectSetPosition();
void Engine_ActorWaitForMove();
void Engine_MapCopyCellAttributes();

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventWait();
void Engine_MapCopyCellsTo();
s32 Engine_TaskAddCallback();
void Engine_ActorSetAnimation();
void ObjectMotion_WaitForAnimationChange();
void Engine_GameFlagClear();
void Engine_ActorSetChildValue();
void Engine_ActorEnableActionCallback();
void Engine_EventEnd();
void SceneEffect_SpawnRandomizedParticle();

s32 StagedActor_FillGridAttributeRectangle();
void Engine_AudioPlayCue();

struct Probe {
    s32 word[6];
};

void StagedActor_PlaceAtObjectTenCell();
void StagedActor_AdvancePair();
void TakaraHashira_DropActorTen();

extern const struct SceneEvent gTakaraHashiraEvents1[];
extern const struct SceneEvent gTakaraHashiraEvents2[];
extern const struct SceneEvent gTakaraHashiraEvents3[];
extern const struct SceneEvent gTakaraHashiraEvents4[];
extern const struct SceneEvent gTakaraHashiraEvents5[];
extern const struct SceneEvent gTakaraHashiraEventsOther[];

extern u8 TakaraHashira_PillarSlots[];
void SceneActor_ApplyPlacementQueryAndTag();
void BattleFx_StartFadeOverlay();
void SceneState_ClearWord24AndObjectByte62();
void OverlayObject_SetCallbackAndMode2();
void SceneActor_CheckActors8To11NearSlotZero(void);
void TakaraHashira_PrepLoweredActor();
void FieldScene_RunScene3b3_0200263c();

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

void FieldScene_RunScene3b3_02001fd4(void)
{
    Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    ((struct FieldActor *)Object_GetById(0))->y.fixed = Map_GetTerrainHeightFar(
        ((struct FieldActor *)Object_GetById(0))->unknown_22,
        ((struct FieldActor *)Object_GetById(0))->x.fixed,
        ((struct FieldActor *)Object_GetById(0))->z.fixed);
#endif
    if (FieldScene_RunScene3b3SequenceD() == 0) {
        *((u8 *)Object_GetById(0) + 85) &= 254;
        *((u8 *)Object_GetById(0) + 35) &= 254;
        StagedActor_AdvancePair();
        TakaraHashira_UpdatePillarActors();
        {
            u8 bits = 1;
            u8 *flags = (u8 *)Object_GetById(ACTOR_PARTY_LEADER) + 85;
            u8 value = *flags;

            value |= bits;
            *flags = value;
            flags = (u8 *)Object_GetById(ACTOR_PARTY_LEADER) + 35;
            bits |= *flags;
            *flags = bits;
        }
    }
    Engine_EventEnd();
}

/* Complete one-call wrapper through interworking return and alignment. */
void FieldScene_RunSingleStep(void)
{
    battle_owner_69();
}

/* Runs the scene when a staged actor stands in the cell a step further along z,
 * otherwise falls back to sequence D. */
void TakaraHashira_RunStagedCellScene(void)
{
    struct FieldActor *leader = Object_GetById(0);
    s32 pos[3];
    s32 *p = pos;

    p[0] = leader->x.fixed;
    p[1] = leader->y.fixed;
    p[2] = leader->z.fixed + 0x100000;
    if ((s32)StagedActor_FindAtTile(p, leader) != 0) {
        FieldScene_RunScene3b3_02001fd4();
    } else if (FieldScene_RunScene3b3SequenceD() == 0) {
        FieldEffect_UpdateGridPlacement();
    }
}

/* Complete scene/entity linker through return and its sole pool word. */
void SceneState_LinkActorZeroToWork24(void)
{
    u8 *obj = Object_GetById(ACTOR_PARTY_LEADER);
    *(u8 **)(PILLAR_WORK + 24) = obj;
    obj[98] = 1;
}

/*
 * Clears PILLAR_WORK[+24] and one flag byte on the object returned by
 * Object_GetById. The 28-byte owner at 0x0200209c includes its one pool
 * word, the PILLAR_WORK pointer.
 */
void SceneState_ClearWord24AndObjectByte62(void)
{
    u8 *obj = Object_GetById(ACTOR_PARTY_LEADER);

    *(s32 *)(PILLAR_WORK + 24) = 0;
    obj[0x62] = 0;
}

/*
 * Place a staged actor at object ten's grid cell -- resource_3b3.
 */

/*
 * The Func_ aliases name the call words encoded in the overlay image. The
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * resource_3b3 @ 0x020020b8 (56 bytes including trailing alignment).
 *
 * Compares an actor with slot zero.  When it is farther right, bit 1 at +35
 * is cleared and then restored only if the actor is also above slot zero.
 * The function always returns zero.
 */
s32 SceneActor_UpdateBit1ByPositionToSlotZero(u8 *actor)
{
    u8 *ref = Object_GetById(ACTOR_PARTY_LEADER);

    if (*(s32 *)(actor + 16) > *(s32 *)(ref + 16)) {
        actor[35] = (u8)(actor[35] & 0xfd);
        if (*(s32 *)(actor + 12) < *(s32 *)(ref + 12))
            actor[35] = (u8)(actor[35] | 2);
    }

    return 0;
}

void TakaraHashira_RunActorAction(s32 a0)
{
    u32 i;
    struct FieldActor *rec7;
    s32 record;

    rec7 = Object_GetById(a0);
    Engine_EventBegin();
    rec7->update = (void (*)(union FieldObject *))SceneActor_UpdateBit1ByPositionToSlotZero;
    Engine_MapCopyCellAttributes(20, 14, 1, 1, (rec7->x.fixed >> 20), (rec7->z.fixed >> 20));
    Engine_GameFlagSet((a0 + 0x1f5));
    Engine_ActorEnableActionCallback(a0, (s32)TakaraHashira_ActionTable);
    Engine_EventEnd();
}

/* Contiguous unnamed leaf-owner run for resource_3b3. */

/* Complete 12-byte actor-11 wrapper before 0x02002150. */
void FieldScene_RunActor11Step(void)
{
    TakaraHashira_RunActorAction(11);
}

/* Complete 12-byte actor-12 wrapper before 0x0200215c. */
void FieldScene_RunActor12Step(void)
{
    TakaraHashira_RunActorAction(12);
}

void FieldScene_RunScene3b3_0200215c(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    u8 *p6;

    rec7 = Object_GetById(ACTOR_PARTY_LEADER);
    record = Object_GetById(13);
    p6 = *(s32 *)gEffectWork;
    if ((*(s32 *)(record + 8) >> 20) == (*(s32 *)(rec7 + 8) >> 20)) {
        if ((*(s32 *)(record + 16) >> 20) != (*(s32 *)(rec7 + 16) >> 20)) {
            goto L_02002198;
        }
        Engine_GameFlagSet(0x203);
        p6[53] = 1;
    } else {
        L_02002198:;
        Engine_GameFlagClear(0x203);
    }
}

/* Crossbone Isle: speed actor 13 up, switch the blend mode of actors 13 and
 * 14 and raise actor 14 before copying the map cells. */
void TakaraHashira_RaiseActorFourteen(void)
{
    s32 rec8;
    s32 record;
    s32 v5;

    rec8 = (s32)Object_GetById(14);
    v5 = 128;
    record = (s32)Object_GetById(13);
    *(s32 *)(record + 24) = (v5 << 9);
    record = (s32)Object_GetById(13);
    *(s32 *)(record + 28) = (v5 << 9);
    record = (s32)Object_GetById(13);
    { s8 *p = *(s8 **)(record + 80); p[9] = (p[9] & -13) | 8; }
    { s8 *p = *(s8 **)(rec8 + 80); p[9] = (p[9] & -13) | 8; }
    *(s32 *)(rec8 + 52) = 0x6666;
    *(s32 *)(rec8 + 48) = 0xcccc;
    Call4(Engine_ObjectSetPosition, rec8, *(s32 *)(rec8 + 8), 0x200000, *(s32 *)(rec8 + 16));
    Engine_ActorWaitForMove(14);
    Call6(Engine_MapCopyCellAttributes, 20, 14, 1, 1, 22, 16);
}

void SceneEffect_SpawnRandomizedParticle(void)
{
    u8 descriptor[40];
    u8 *d;
    s32 spread;
    s32 secondary;
    u32 draw;
    u32 mask;

    if ((*(volatile s32 *)&gFrameCount & 2) != 0)
        return;
    if ((*(volatile s32 *)&gFrameCount & 7) == 0)
        Engine_AudioPlayCue(136);

    d = descriptor;
    *(s32 *)(d + 4) = 10;
    *(s32 *)(d + 8) = 0x8000;
    *(s32 *)(d + 12) = 0x8000;
    *(s32 *)(d + 16) = 0x19999;
    *(s32 *)(d + 20) = 0x19999;
    draw = (u32)Engine_RandomNext();
    mask = 0x0ffff000;
    mask &= draw;
    *(u16 *)(d + 32) = (u16)mask;
    *(s32 *)(d + 36) = (s32)FieldScene_RunScene3b3SequenceE;

    draw = (u32)Engine_RandomNext();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    spread /= 2;
    draw = (u32)Engine_RandomNext();
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);

    Effect_Spawn(0x01440000, 0x00300000, 0x00e40000, spread,
                  secondary, 0, 0x014d0000, d);
}

s32 SceneEffect_SpawnRandomEffectEveryEightFrames(u8 *actor)
{
    u8 desc[40];
    u8 *p;
    u32 phase = (u32)*(volatile s32 *)&gFrameCount & 7;
    s32 x;
    s32 y;
    s32 z;
    s32 scale;

    if (phase != 0)
        return 0;

    p = desc;
    *(s32 *)(p + 4) = 7;
    *(s32 *)(p + 8) = 0xb333;
    *(s32 *)(p + 12) = 0xb333;

    x = *(s32 *)(actor + 8) + (((s32)(((u32)Engine_RandomNext() * 17) >> 16) - 8) << 16);
    y = *(s32 *)(actor + 12) + ((s32)(((u32)Engine_RandomNext() * 17) >> 16) << 16);
    z = *(s32 *)(actor + 16) + (((s32)(((u32)Engine_RandomNext() * 17) >> 16) - 8) << 16);
    scale = Math_Divide((s32)(((u32)Engine_RandomNext() * 5) >> 16) * 0x10000 + 0x30000, 10);

    Effect_Spawn(x, y, z, 0, scale, (s32)phase, 0x00090001, p);
    return 0;
}

/* Crossbone Isle: the first time (flag 0x203 clear) set flag 0x202, pan the
 * camera and shift the cells at column 73 while a task runs; with flag
 * 0x201, actor 12 plays its part and the cells at (17, 13) are copied. */
void TakaraHashira_RunMapShiftScene(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Engine_GameFlagIsSet(0x203);
    if (rec7 == 0) {
        Engine_GameFlagSet(0x202);
        Engine_EventBegin();
        Engine_CameraSetSpeed(0x9999, 0x1333);
        Engine_CameraMoveTo(0x1380000, -1, 0xb80000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(20);
        Call6(Engine_MapCopyCellsTo, 73, 10, 60, 10, 1, 2);
        Engine_EventWait(20);
        Engine_TaskAddCallback((s32)SceneEffect_SpawnRandomizedParticle, 0xc80);
        Engine_EventWait(40);
        if (Value1(Engine_GameFlagIsSet, 0x201) != 0) {
            record = (s32)Object_GetById(12);
            *(s32 *)(record + 108) = (s32)SceneEffect_SpawnRandomEffectEveryEightFrames;
            Engine_ActorSetAnimation(12, 6);
            ObjectMotion_WaitForAnimationChange(12);
            record = (s32)Object_GetById(12);
            *(s32 *)(record + 108) = rec7;
            Call6(Engine_MapCopyCellAttributes, 17, 13, 1, 1, 18, 13);
            Engine_GameFlagClear(0x201);
            Engine_ActorSetChildValue(12, 0);
            Engine_ActorEnableActionCallback(12, 1);
        } else {
            Engine_EventWait(60);
        }
        ((void (*)())Engine_TaskRemoveCallback)((s32)SceneEffect_SpawnRandomizedParticle);
        Engine_EventWait(20);
        Call6(Engine_MapCopyCellsTo, 72, 10, 60, 10, 1, 2);
        Engine_EventWait(20);
        Engine_EventEnd();
    }
}

/*
 * Fetch object ten, shift its +8 and +16 fixed-point fields down to grid
 * coordinates, and place there. The last two literal arguments go on the
 * stack.
 */
void StagedActor_PlaceAtObjectTenCell(void)
{
    u8 *obj = Object_GetById(10);
    s32 x;
    s32 z;

    Engine_EventBegin();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0);
    Engine_EventEnd();
}

/* Crossbone Isle: block actor 10's cell and, the first time it reaches
 * column 16 (flag 0x204), drop it into the floor with cue 159. */
void TakaraHashira_DropActorTen(void)
{
    u8 *rec7;
    s32 rec8;

    rec7 = (s32)Object_GetById(10);
    Engine_EventBegin();
    StagedActor_FillGridAttributeRectangle(2, (*(s32 *)((s32)rec7 + 8) >> 20), (*(s32 *)((s32)rec7 + 16) >> 20), 1, 1, 255);
    if ((*(s32 *)((s32)rec7 + 8) >> 20) == 16) {
        rec8 = Value1(Engine_GameFlagIsSet, 0x204);
        if (rec8 == 0) {
            Engine_EventWait(10);
            Engine_AudioPlayCue(159);
            rec7[85] = rec8;
            *(s32 *)((s32)rec7 + 20) = -0x20000;
            *(s32 *)((s32)rec7 + 12) = -0x20000;
            Engine_GameFlagSet(0x204);
        }
    }
    Engine_EventEnd();
}

/* Crossbone Isle: move the pushed actor when the probe finds room, otherwise
 * put it back on its cell and step the staged pair. */
void TakaraHashira_RunPushScene(void)
{
    struct Probe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        ((void (*)(struct Probe))SceneActor_MoveAndRedraw)(probe);
    } else {
        StagedActor_PlaceAtObjectTenCell();
        StagedActor_AdvancePair();
        TakaraHashira_DropActorTen();
    }
    Engine_EventEnd();
}

/* What each of the pillar rooms answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraHashira1) {
        return gTakaraHashiraEvents1;
    }
    if (scene == (s32)&SceneId_TakaraHashira2) {
        return gTakaraHashiraEvents2;
    }
    if (scene == (s32)&SceneId_TakaraHashira3) {
        return gTakaraHashiraEvents3;
    }
    if (scene == (s32)&SceneId_TakaraHashira4) {
        return gTakaraHashiraEvents4;
    }
    if (scene == (s32)&SceneId_TakaraHashira5) {
        return gTakaraHashiraEvents5;
    }
    return gTakaraHashiraEventsOther;
}

void TakaraHashira_PrepLoweredActor(s32 id)
{
    struct FieldActor *actor = Object_GetById(id);

    actor->collision_flags &= 254;
    actor->priority_flags |= 2;
    actor->motion_flags = 0;
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->sprite->priority = 2;
}

void FieldScene_RunScene3b3_0200263c(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = (s32)Object_GetById(a0);
    if (Engine_GameFlagIsSet((a0 + 0x1f5)) != 0) {
        Object_SetMode(rec7, 5);
        *(s32 *)(rec7 + 108) = (s32)SceneActor_UpdateBit1ByPositionToSlotZero;
        Engine_MapCopyCellAttributes(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
        Engine_ActorEnableActionCallback(a0, TakaraHashira_ActionTable);
    }
}

void OverlayObject_SetCallbackAndMode2(s32 id)
{
    u8 *obj = (u8 *)Object_GetById(id);
    u8 *base = obj;
    u8 zero = 0;

    obj += 0x22;
    *obj = 2;
    base[0x55] = zero;
    *(u32 *)(base + 0x6c) = (u32)TakaraHashira_UpdateActorPriority;
}

void SceneActor_CheckActors8To11NearSlotZero(void)
{
    u8 *hero = Object_GetById(ACTOR_PARTY_LEADER);
    u32 selector = 8;
    u8 *actor;

loop:
    actor = Object_GetById(selector);

    if (*(s32 *)(hero + 12) / 0x10000 != *(s32 *)(actor + 12) / 0x10000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 16) > *(s32 *)(actor + 16) - 0x80000
        || *(s32 *)(hero + 16) <= *(s32 *)(actor + 16) - 0x180000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 8) - 0x100000 > *(s32 *)(actor + 8)
        || *(s32 *)(actor + 8) >= *(s32 *)(hero + 8) + 0x100000)
        goto continue_loop;

    {
        Handle *handle = *(Handle **)(actor + 80);
        Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, handle->mode);
    }
    goto done;

mark_and_continue:
    {
        u8 *mark = (u8 *)Object_GetById(0) + 35;
        u8 bit = 1;
        bit |= *mark;
        *mark = bit;
    }

continue_loop:
    selector++;
    if (selector <= 11)
        goto loop;

done:
    return;
}

/* The pillar rooms' setup: redraw the pillars' footprints, restore the map
 * the switches changed, and in the fourth room raise the pillars and watch
 * the actors near them. */
s32 TakaraHashira_SetupArea(void)
{
    u32 i;
    u8 *record;
    s32 v1;
    s32 v2;
    s32 base5_8;
    s32 v5;
    s32 v0;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_TakaraHashira2) {
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        FieldScene_RedrawActorFootprint(11);
        FieldScene_RedrawActorFootprint(12);
    } else {
        if (gGameState.scene == (s32)&SceneId_TakaraHashira3) {
            Call6(Engine_MapCopyCells, 32, 0, 64, 32, 0, 64);
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(11);
            FieldScene_RedrawActorFootprint(12);
            FieldScene_RedrawActorFootprint(13);
            FieldScene_RedrawActorFootprint(14);
            FieldScene_RedrawActorFootprint(15);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                goto L_020029fa;
            }
            if (Engine_GameFlagIsSet(0x200) == 0) {
                goto L_020029fa;
            }
            Call6(Engine_MapCopyCellsTo, 79, 34, 84, 24, 1, 2);
            Engine_MapCopyCellsTo(0, 32, 32, 0, 32, 32);
            Engine_MapCopyCellsTo(32, 32, 64, 0, 32, 32);
            SceneActor_ApplyPlacementQueryAndTag(9);
            SceneActor_ApplyPlacementQueryAndTag(10);
            SceneActor_ApplyPlacementQueryAndTag(11);
            SceneActor_ApplyPlacementQueryAndTag(12);
            SceneActor_ApplyPlacementQueryAndTag(13);
            SceneActor_ApplyPlacementQueryAndTag(14);
            SceneActor_ApplyPlacementQueryAndTag(15);
            Call6(Engine_MapCopyCellAttributes, 24, 3, 1, 1, 24, 8);
            goto L_020029fa;
        } else {
            if (gGameState.scene != (s32)&SceneId_TakaraHashira4) {
                goto L_02002922;
            }
            OverlayObject_CreateConfiguredObject(0x2480000, 0, 0xc80000, 223);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                *((u8 *)Object_GetById(0) + 98) = 1;
            }
            BattleFx_StartFadeOverlay(0);
            if (*((u8 *)Object_GetById(0) + 98) == 0) {
                SceneState_ClearWord24AndObjectByte62();
            }
            OverlayObject_SetCallbackAndMode2(8);
            OverlayObject_SetCallbackAndMode2(9);
            OverlayObject_SetCallbackAndMode2(10);
            OverlayObject_SetCallbackAndMode2(11);
            {
                s32 *entry = (s32 *)TakaraHashira_PillarSlots;

                for (v1 = 0; (u32)v1 <= 3; v1++) {
                    entry[0] = 0;
                    entry[1] = 0;
                    entry[2] = 0;
                    entry[4] = v1 + 0x200;
                    entry += 5;
                }
            }
            TakaraHashira_UpdatePillarActors();
            Engine_EventWait(1);
            ((void (*)())Engine_TaskAddCallback)((s32)SceneActor_CheckActors8To11NearSlotZero, 0xc80);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                goto L_020029fa;
            }
            for (base5_8 = 8; (u32)base5_8 <= 11; base5_8++) {
                record = (u8 *)Object_GetById(base5_8);
                v2 = *(s32 *)(record + 8) >> 20;
                if (v2 == 37) {
                    v0 = *(s32 *)(record + 16) >> 20;
                    if (v0 == 9) {
                        Engine_MapCopyCellAttributes(27, 8, 1, 1, v2, v0);
                        break;
                    }
                }
            }
        }
        goto L_020029fa;
        L_02002922:;
        if (gGameState.scene == (s32)&SceneId_TakaraHashira5) {
            Engine_ActorSetAnimation(10, 2);
            Engine_ActorSetChildValue(10, 6);
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
#if EDITION_INTERNATIONAL
            v5 = 0;
            *((u8 *)Object_GetById(8) + 85) = v5;
            *((u8 *)Object_GetById(9) + 85) = v5;
#endif
            TakaraHashira_DropActorTen();
            TakaraHashira_PrepLoweredActor(11);
            TakaraHashira_PrepLoweredActor(12);
            TakaraHashira_PrepLoweredActor(13);
            FieldScene_RunScene3b3_0200263c(11);
            FieldScene_RunScene3b3_0200263c(12);
            FieldScene_RunScene3b3_0200263c(13);
            record = (u8 *)Object_GetById(13);
#if EDITION_INTERNATIONAL
            *(s32 *)((s32)record + 108) = v5;
#else
            *(s32 *)((s32)record + 108) = 0;
#endif
            TakaraHashira_PrepLoweredActor(14);
            {
                u8 *record = (u8 *)Object_GetById(14);
                /* FAKEMATCH: retain the flag read before its merge. */
                u8 value = *(volatile u8 *)&record[89];

                record[89] = (u8)(value | 8);
            }
            if (Engine_GameFlagIsSet(0x202) == 0) {
                v5 = 192;
                record = (u8 *)Object_GetById(13);
                *(s32 *)((s32)record + 24) = (v5 << 9);
                record = (u8 *)Object_GetById(13);
                *(s32 *)((s32)record + 28) = (v5 << 9);
                record = (u8 *)Object_GetById(13);
                *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
                record = (u8 *)Object_GetById(14);
                *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
                Call6(Engine_MapCopyCellAttributes, 26, 12, 1, 1, 22, 16);
            }
        }
    }
    L_020029fa:;
    return 0;
}
