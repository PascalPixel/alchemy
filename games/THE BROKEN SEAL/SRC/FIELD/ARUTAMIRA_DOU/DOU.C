#include "ARUTAMIRA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SCENE_IDS.H"
#include "DMA.H"

extern const struct ScenePlacement gArutamiraDouPlacements2[];
extern const struct ScenePlacement gArutamiraDouPlacements4[];
extern const struct ScenePlacement gArutamiraDouPlacements6[];
extern const struct ScenePlacement gArutamiraDouPlacementsOther[];

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    volatile u16 snapshot[512][1];
    s32 words[256];
};

/* The game state read as rows of halfwords. */
#define gGameStateRows (*(union GameStateRows *)&gGameState)

extern u8 gEffectWork[];
void Animation_ApplyChildValues();
void Battle_WaitMode0();
void Engine_ActorFaceDirection();

struct Obj {
    u8 unknown_00[24];
    s32 a;
    s32 b;
    u8 unknown_20[68];
    s16 frame;
};

extern s32 ArutamiraDou_PulseScales[];

void ArutamiraDou_UpdateScalePulse();

void FieldScene_SetActor13Value41(void)
{
    BattleFx_SetPhaseRequest(13, 0x41);
}

/* Contiguous unnamed leaf-owner run for resource_3bd. */
u8 *SceneData_GetTablebf70(void)
{
    return ArutamiraDou_SceneTableA;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablec138(void)
{
    return ArutamiraDou_SceneTableB;
}

/* The actors placed in the scene. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutamiraDou2) {
        return gArutamiraDouPlacements2;
    }
    if (scene == (s32)&SceneId_ArutamiraDou4) {
        return gArutamiraDouPlacements4;
    }
    if (scene == (s32)&SceneId_ArutamiraDou6) {
        return gArutamiraDouPlacements6;
    }
    return gArutamiraDouPlacementsOther;
}

/* Set the blend mode, then the blend weights for the current fade level. */
void ArutamiraDou_ApplyFadeBlend(void)
{
    s8 level = ((s8 *)gSceneState)[4];

    /* FAKEMATCH: the do-while wrap keeps the level's sign extension after
     * the BLDCNT store. */
    do {
        s32 blend = 0x3f42;

        *(volatile u16 *)0x04000050 = blend;
    } while (0);
    if (level == 0) {
        s32 alpha = 0x1000;

        *(volatile u16 *)0x04000052 = alpha;
    } else if (level == 1) {
        s32 alpha = 0xe00;

        *(volatile u16 *)0x04000052 = alpha;
    } else if (level == 2) {
        s32 alpha = 0xc00;

        *(volatile u16 *)0x04000052 = alpha;
    } else if (level == 3) {
        s32 alpha = 0xa00;

        *(volatile u16 *)0x04000052 = alpha;
    } else if (level == 4) {
        s32 alpha = 0x800;

        *(volatile u16 *)0x04000052 = alpha;
    } else {
        s32 alpha = 0x600;

        *(volatile u16 *)0x04000052 = alpha;
    }
}

/* Record the fade level in the scene state; while the event work's word at
   +0xcb8 is clear, the blend takes it at once. */
void ArutamiraDou_SetFadeLevel(s32 level)
{
    u8 *work;
    u8 *fade = &gSceneState[4];

    work = (u8 *)gEventWork;
    *fade = level;
    if (*(s16 *)(work + 0xcb8) == 0) {
        ArutamiraDou_ApplyFadeBlend();
    }
}

void FieldScene_RunIndexedStep0(void)
{
    ArutamiraDou_SetFadeLevel(0);
}

void FieldScene_RunIndexedStep1(void)
{
    ArutamiraDou_SetFadeLevel(1);
}

void FieldScene_RunIndexedStep2(void)
{
    ArutamiraDou_SetFadeLevel(2);
}

void FieldScene_RunIndexedStep3(void)
{
    ArutamiraDou_SetFadeLevel(3);
}

void FieldScene_RunIndexedStep4(void)
{
    ArutamiraDou_SetFadeLevel(4);
}

void FieldScene_RunIndexedStep5(void)
{
    ArutamiraDou_SetFadeLevel(5);
}

void SceneEffect_SetupBlendByFlag201(void)
{
    u8 **base = (u8 **)&gEventWork;
    u8 *state;

    {
        u8 *tmp = *base;
        *(s32 *)(tmp + 0x1c0) = 0x100;
        *(s32 *)(tmp + 0x1c8) = 24;
    }
    Engine_TaskWait(1);
    DisplayTransition_InitializeBattleEffectState(0x4d);
    state = base[4];
    {
        u16 *slot = (u16 *)(state + 0x52a);
        s32 c = 5;
        *slot = c;
    }
    if (GameFlag_IsSet(0x201) != 0) {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x1d1d;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 0x3f;
            *slot = c;
        }
        ArutamiraDou_ApplyFadeBlend();
        return;
    } else {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x3f3f;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 31;
            *slot = c;
        }
    }
    {
        s32 a = 0x3f42;
        *(u16 *)0x4000050 = a;
    }
    {
        s32 b = 0xc04;
        *(u16 *)0x4000052 = b;
    }
}

/* Restore the room's blend setting and the five actors' child poses. */
void ArutamiraDou_ApplyRoomVisuals(void)
{
    /* FAKEMATCH: the volatile view keeps two distinct scene reads; this
     * initialized narrow record keeps the snapshot unsigned until its later
     * signed comparison, without an extra halfword-to-word register copy. */
    struct { u32 value : 16; } map = { 0 };

    map.value = gGameStateRows.snapshot[224][0];

    if (gGameStateRows.halves[224][0] == (s32)&SceneId_ArutamiraDou1)
    {
        s32 alpha = 0x1000;

        *(volatile u16 *)0x04000052 = alpha;
    }
    if ((s16)map.value == (s32)&SceneId_ArutamiraDou6) {
        Engine_ActorSetChildValue(16, 1);
        Engine_ActorSetChildValue(17, 4);
        Engine_ActorSetChildValue(18, 11);
        Engine_ActorSetChildValue(19, 2);
        Engine_ActorSetChildValue(20, 3);
    }
}

void SceneState_RunFlag200SetupAndPlaceActors16To20(void)
{

    u8 *work = (*(u8 * *)gEffectWork);
    s16 *tbl;

    if (GameFlag_IsSet(0x200) != 0) {
        SceneEffect_SetupBlendByFlag201();
        work[0x34] = 1;
    }
    tbl = (s16 *)&gGameState;
    if (tbl[0xe0] == (s32)&SceneId_ArutamiraDou6) {
        Actor_SetChildValue(16, 6);
        Actor_SetChildValue(17, 6);
        Actor_SetChildValue(18, 6);
        Actor_SetChildValue(19, 6);
        Actor_SetChildValue(20, 6);
    }
}

void ArutamiraDou_RespawnActorObject(void)
{
    s32 rec6;
    s32 record;
    u8 *p6;
    u8 *p5;

    p6 = *(s32 *)gEffectWork;
    p5 = *(s32 *)((s32)p6 + 16);
    Engine_ActorFaceDirection(*(s16 *)((s32)p6 + 24), 0x4000, 0);
    Animation_ApplyChildValues((s32)p5, 0);
    Battle_WaitMode0(20);
    rec6 = (s32)Engine_ObjectCreate(0, *(s32 *)((s32)p5 + 8), *(s32 *)((s32)p5 + 12), *(s32 *)((s32)p5 + 16));
    if (rec6 != 0) {
        Dma_Set((const void *)((s32)p5), (void *)(rec6), -0x7bffffe4, (volatile u32 *)(0x40000d4));
        *(s32 *)((s32)p5 + 108) = 0;
        *(s32 *)((s32)p6 + 16) = rec6;
        p5[84] = 0;
    }
}

/* When actor 12 stands on cell (30, 20), settles it there, copies the cell's
 * attributes to (32, 20) and sets flag 0x212. */
void ArutamiraDou_SettleActorOnCell(void)
{
    struct FieldActor *actor = Object_GetById(12);

    if (actor->x.fixed >> 20 == 30 && actor->z.fixed >> 20 == 20) {
        actor->motion_flags = 2;
        *(s32 *)actor->unknown_14 = 0;
        actor->priority_flags = 2;
        Engine_MapCopyCellAttributes(30, 20, 1, 1, 32, 20);
        Engine_GameFlagSet(0x212);
    }
}

void FieldScene_RunTwoCallSequence(void)
{
    StagedActor_AdvancePair();
    ArutamiraDou_SettleActorOnCell();
}

/* Contiguous unnamed state-owner run for resource_3bd. */
void SceneState_MarkObjectWhenActorElevenAhead(void)
{
    u8 *obj = *(u8 **)gEffectWork;
    Ent_02000d58 *p = (Ent_02000d58 *)Object_GetById(11);
    Vec v;

    v.x = p->unk8;
    v.y = p->unkC;
    v.z = p->unk10;

    if (Object_CheckMovementCollision(p, &v) > 0) {
        obj[0x35] = 1;
    }
}

void FieldScene_RunActorElevenCellSetup(void)
{
    u8 *obj = *(u8 **)gEffectWork;
    u8 *p = (u8 *)Object_GetById(11);
    s32 t;

    obj += 0x35;
    t = *obj;
    t = (s8)t;
    if (t == 0) {
        s32 a = 0x49;
        s32 b = 0x11;
        Map_CopyCellAttributes(0x4c, 0x10, 1, 1, a, b);
        if (p != 0) {
            s32 c = 2;
            p[0x55] = c;
            p[0x23] = t;
        }
        GameFlag_Set(0x211);
    }
}

/* Hops the selected actor one step (two cells) the way it faces, snapping to
 * the facing's sixteenth, unless something blocks the landing cell. */
void ArutamiraDou_HopSelectedActor(void)
{
    struct FieldActor *actor = Object_GetById(gGameState.selected_actor);
    s32 flags = actor->motion_flags;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p = pos;

    p[0].fixed = actor->x.fixed;
    p[1].fixed = actor->y.fixed;
    p[2].fixed = actor->z.fixed;
    {
        s32 angle = actor->facing & 0xf000;

        Vector_AddPolarOffset(0x200000, angle, p);
    }
    if (Object_CheckMovementCollision(actor, p) == 0) {
        Engine_EventBegin();
        Object_SetMode(actor, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Object_SetMode(actor, 7);
        actor->speed = 0x30000;
        actor->acceleration = 0x20000;
        actor->velocity_y = 0x40000;
        actor->motion_flags &= 126;
        Engine_ActorSetSpriteFlags(actor, 0);
        Engine_ObjectMotionSetPositionAndCommit(0, p[0].part.pixel, p[2].part.pixel);
        Object_SetMode(actor, 6);
        Engine_ActorSetSpriteFlags(actor, 1);
        actor->motion_flags = flags;
        Engine_EventEnd();
    }
}

void FieldScene_RunGuardedSixWordStep(void)
{
    struct StagedActorProbe s;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&s) != 0) {
        SceneActor_MoveAndRedraw(s);
    }
    Engine_EventEnd();
}

void ArutamiraDou_UpdateScalePulse(struct Obj *obj)
{
    s32 v = ArutamiraDou_PulseScales[(u16)(obj->frame >> 2) & 3];

    obj->a = v;
    obj->b = v;
    obj->frame++;
    obj->frame &= 15;
}

void SceneActor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c)
{
    s32 k1 = 0x1f80000;
    s32 k2 = 0x180000;
    s32 k3 = 0x900000;
    u8 *obj = (u8 *)Object_GetById(a);
    s32 buf[3];
    s32 *bp = buf;

    bp[0] = k1;
    bp[2] = k2;
    Vector_AddPolarOffset(b, c, bp);
    *(s32 *)(obj + 8) = bp[0];
    *(s32 *)(obj + 12) = bp[2];
    *(s32 *)(obj + 16) = k3;
}

/* Contiguous unnamed state-owner run for resource_3bd. */
void SceneActor_PlaceFiveActorsInRow(u8 *p)
{
    s32 i = 0;

    do {
        SceneActor_SetPositionFromTransformedBase(i + 11, 0x180000, p);
        p -= 13107;
        i++;
    } while (i <= 4);
}

/* Five actors turn in a ring: the wheel speeds up, runs, slows, and stops
   when the actor the scene state's third byte chooses comes round; that
   actor then pulses. A cue plays at every 0x3000 of travel.
   FAKEMATCH: forced temporaries; the block-local zero and state, the
   reloaded work pointers and the stored-sum temporaries keep the game's
   register and pool order, which the plain statements do not. */
void ArutamiraDou_SpinActorWheel(void)
{
    u16 *p = ArutamiraDou_ClearTarget;
    s32 flag = 1;
    s32 state = *(s16 *)p;

    if (state == 0) {
        s32 t = p[4] + 16;
        p[4] = t;
        if ((u16)t > 0xbff) {
            p[0] = p[0] + 1;
            p[1] = state;
        }
    } else if (state == 1) {
        if ((s16)p[1] == 30) {
            p[0] = p[0] + 1;
        }
    } else if (state == 2) {
        s32 t = p[4] + 0xfff8;
        p[4] = t;
        if ((u16)t <= 0x2ff) {
            p[0] = p[0] + 1;
        }
    } else if (state == 3) {
        s32 v = ((s8 *)gSceneState)[2];
        s32 r = Math_Divide(v << 16, 5);
        if ((unsigned int)(((p[3] - r) << 16) + 0xc2ff0000) <= 0x5fe0000) {
            u8 *o;
            s32 nv = r + 0x4000;
            p[3] = nv;
            {
                s32 z = 0;
                s32 k = 0x63;
                p[0] = k;
                p[4] = z;
            }
            o = (u8 *)Object_GetById(v + 11);
            *(s32 *)(o + 0x6c) = (s32)ArutamiraDou_UpdateScalePulse;
        }
    } else if (state == 0x63) {
        flag = 0;
    }
    if (flag != 0) {
        u16 *q2;
        ArutamiraDou_ClearTarget[3] += ArutamiraDou_ClearTarget[4];
        SceneActor_PlaceFiveActorsInRow(ArutamiraDou_ClearTarget[3]);
        q2 = ArutamiraDou_ClearTarget;
        {
            s32 t2 = q2[5] + q2[4];
            q2[5] = t2;
            if ((u16)t2 > 0x3000) {
                s32 z2 = 0;
                q2[5] = z2;
                Audio_PlayCue(0x87);
            }
        }
    }
    {
        u16 *q = ArutamiraDou_ClearTarget;
        q[1]++;
    }
}
