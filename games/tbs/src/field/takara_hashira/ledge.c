#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "edition.h"
#include "types.h"
#include "field_event.h"
#include "hashira.h"
#include "field_scene.h"
#include "io_reg.h"

#define OverlayObject_PrepareObject ((s32 (*)(s32,s32,s32,s32))OverlayObject_PrepareObject)

extern s32 StagedActor_DirectionSteps[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);




extern u32 gBgScroll[];

void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *src)
{
    struct MapState *map = gMapWork[0];

    if (map != 0) {
        struct MapCell *cell = map->layers[layer].cells;

        cell += x + (y << 7);

        cell->layer = src->layer;
        /* FAKEMATCH: the kind is read as the top of the source byte, not through the bitfield, as the reference loads it. */
        cell->kind = ((u8 *)src)[1] >> 6;
        cell->height = src->height;
        cell->event = src->event;
    }
}

s32 TakaraHashira_LowerActorToLedge(s32 id, s32 far)
{
    struct FieldActor *actor = Object_GetById(id);
    s32 found;
    s32 object;
    s32 height;
    s32 level;
    s32 top;
    u32 row;
    u32 rows;
    s32 z;
    s32 y;
    s32 x;
    s32 kind;

    found = 0;
    object = found;
    Engine_ActorSetSpriteFlags(actor, found);
    height = Map_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed);
    level = height / 0x100000;
    rows = level;
    if (level < 0) {
        rows = -level;
    }
    rows++;
    for (row = 0; row <= rows; row++) {
        top = Map_GetTerrainHeight(actor->terrain_id, actor->x.fixed, (row << 20) + actor->z.fixed);
        top /= 0x100000;
        if (level < top) {
            x = ((actor->x.fixed >> 20) << 20) + 0x80000;
            if (far == 0) {
                row += 2;
                z = ((row + (actor->z.fixed >> 20)) << 20) + 0x20000;
                y = top << 20;
                kind = 223;
            } else {
                row += 3;
                z = ((row + (actor->z.fixed >> 20)) << 20) - 0x20000;
                y = top << 20;
                kind = 253;
            }
            /* FAKEMATCH: a do-while(0) around the object and height lets sched2 issue the height arithmetic first, as the reference does */
            do {
                object = OverlayObject_PrepareObject(x, y, z, kind);
                height = actor->z.fixed - z + y;
            } while (0);
            found = 1;
            break;
        }
    }
    SceneActor_WaitHeightBelowLimit(actor, height);
    actor->x.fixed = 0;
    actor->y.fixed = 0;
    actor->z.fixed = 0;
    if (object != 0) {
        Engine_ObjectDispatchRelease((void *)object);
    }
    return found;
}

void StagedActor_StepDownUntilClamp(s32 index)
{
    struct FieldActor *obj = Object_GetById(index);
    u32 cnt;

    obj->motion_flags = 0;

    cnt = 0;
    for (;;) {
        if (cnt > 31) return;
        Engine_TaskWait(1);
        obj->scale_y += -0x1999;
        obj->y.fixed += -0xcccc;
        cnt++;
        if (obj->scale_y <= 0x1998) {
            obj->scale_y = 0x1999;
            return;
        }
    }
}

void FieldScene_RunPrimarySequence(void)
{
    s32 FieldScene_RunScene3b3SequenceD();

    struct FieldActor *rec;
    s32 flag;
    s32 p6;
    s32 p5;
    s32 record;
    s32 v1;
    s32 v2;
    s32 v3;
    u8 *base;
    u8 slot16[40];

    rec = Object_GetById(ACTOR_PARTY_LEADER);
    flag = gFrameCount & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Engine_RandomNext();
        p6 = rec->x.fixed + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Engine_RandomNext();
        p5 = rec->z.fixed + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Engine_RandomNext();
        record = Math_Divide((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Effect_Spawn(p6, rec->y.fixed, p5, 0, record, flag, 0x90001, (s32)base);
    }
}

s32 FieldScene_RunScene3b3SequenceD(void)
{
    struct FieldActor *leader;
    u8 *flags;
    s32 saved;
    s32 mode;
#if !EDITION_INTERNATIONAL
    s32 step;
#endif
    s32 *p;
    s32 buf[3];

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    flags = &leader->motion_flags;
    saved = *flags;
#if EDITION_INTERNATIONAL
    mode = (leader->facing + 0x2000) & 0xc000;
#endif
    if (gGameState.movement_mode != 0) {
        return 0;
    }
    p = buf;
#if EDITION_INTERNATIONAL
    p[0] = (leader->x.fixed & -0x100000) + 0x80000;
    p[1] = leader->y.fixed;
    p[2] = (leader->z.fixed & -0x100000) + 0x80000;
    Vector_AddPolarOffset(0x100000, mode, p);
#else
    mode = leader->facing >> 12;
    step = StagedActor_DirectionSteps[mode];
    p[0] = leader->x.fixed + (step & 0xffff0000);
    p[1] = leader->y.fixed;
    step <<= 16;
    p[2] = leader->z.fixed + step;
#endif
    if (Object_CheckMovementCollision(leader, p) == 1) {
        goto reject;
    }
    if (StagedActor_FindAtTile(p, (struct StagedActor *)leader) != 0) {
        goto reject;
    }
#if EDITION_INTERNATIONAL
    p[0] = (leader->x.fixed & -0x100000) + 0x80000;
    p[1] = leader->y.fixed;
    p[2] = (leader->z.fixed & -0x100000) + 0x80000;
    Vector_AddPolarOffset(0x200000, mode, p);
#else
    step = StagedActor_DirectionSteps[mode];
    p[0] = leader->x.fixed + ((step & 0xffff0000) * 2);
    p[1] = leader->y.fixed;
    step <<= 17;
    p[2] = leader->z.fixed + step;
#endif
    if (StagedActor_FindAtTile(p, (struct StagedActor *)leader) != 0) {
        goto reject;
    }
    if (Object_CheckMovementCollision(leader, p) != 0) {
        goto reject;
    }
    Engine_EventBegin();
    Object_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(152);
    Object_SetMode(leader, 7);
    leader->speed = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocity_y = 0x40000;
    *flags &= 126;
    Engine_ActorSetSpriteFlags(leader, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, ((s16 *)p)[1], ((s16 *)p)[5]);
    Object_SetMode(leader, 6);
    Engine_ActorSetSpriteFlags(leader, 1);
    *flags = saved;
    Engine_EventEnd();
    return 1;
reject:
    return 0;
}

void FieldScene_RunScene3b3SequenceE(union FieldObject *object)
{
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;

    object->effect.x += object->effect.velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    velocity_x = object->effect.velocity_x;
    velocity_y = object->effect.velocity_y;
    velocity_z = object->effect.velocity_z;
    object->effect.velocity_x = velocity_x - Math_Divide(velocity_x, 10);
    object->effect.velocity_y = velocity_y - Math_Divide(velocity_y, 3);
    object->effect.velocity_z = velocity_z - Math_Divide(velocity_z, 10);
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}

s32 SceneActor_ApplyCounterLowBitsAsMode(u8 *actor)
{
    ObjectGroup_SetChildValue(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraHashira1) {
        return gTakaraHashiraEntrances1;
    }
    if (scene == (s32)&SceneId_TakaraHashira2) {
        return gTakaraHashiraEntrances2;
    }
    if (scene == (s32)&SceneId_TakaraHashira3) {
        return gTakaraHashiraEntrances3;
    }
    if (scene == (s32)&SceneId_TakaraHashira4) {
        return gTakaraHashiraEntrances4;
    }
    if (scene == (s32)&SceneId_TakaraHashira5) {
        return gTakaraHashiraEntrances5;
    }
    return gTakaraHashiraEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gTakaraHashiraExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraHashira1) {
        return gTakaraHashiraPlacements1;
    }
    if (scene == (s32)&SceneId_TakaraHashira2) {
        return gTakaraHashiraPlacements2;
    }
    if (scene == (s32)&SceneId_TakaraHashira3) {
        return gTakaraHashiraPlacements3;
    }
    if (scene == (s32)&SceneId_TakaraHashira5) {
        return gTakaraHashiraPlacements5;
    }
    return gTakaraHashiraPlacementsOther;
}

void FieldScene_RunTransitionOrFallback(void)
{
    Engine_EventBegin();
    if (FieldScene_RunScene3b3SequenceD() == 0)
        StagedActor_AdvancePair();
    Engine_EventEnd();
}

void SceneState_ApplyPlacementResult(void)
{
    struct StagedActorProbe out;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&out) != 0)
        SceneActor_MoveAndRedraw(out);
    Engine_EventEnd();
}

void TakaraHashira_JitterBackgroundScroll(void)
{
    u32 line = REG_VCOUNT;
    u32 *src = &gBgScroll[1];
    volatile u32 *reg = (volatile u32 *)0x04000014;

    if (line == 227 || line <= 46) {
        if ((u32)(Engine_RandomNext() * 100) >> 16 < TakaraHashira_ShakeChance) {
            src = ((u32 *)TakaraHashira_ShakenScroll);
        }
    }
    *reg = *src++;
    reg = (volatile u32 *)0x04000018;
    *reg++ = *src++;
    *reg = *src;
}
