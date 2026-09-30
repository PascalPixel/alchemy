#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "HASHIRA.H"
#include "FIELD_SCENE.H"

extern u8 *gMapWork;

struct MapCell {
    u32 tile : 12;
    u32 layer : 2;
    u32 kind : 2;
    u32 height : 8;
    u32 event : 8;
};

struct MapLayer {
    struct MapCell *cells;
    u8 pad04[44];
};

struct MapState {
    u8 pad000[0x130];
    struct MapLayer layers[1];
};

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
void SceneActor_WaitHeightBelowLimit(struct FieldActor *actor, s32 limit);

extern const struct SceneEntrance gTakaraHashiraEntrances1[];
extern const struct SceneEntrance gTakaraHashiraEntrances2[];
extern const struct SceneEntrance gTakaraHashiraEntrances3[];
extern const struct SceneEntrance gTakaraHashiraEntrances4[];
extern const struct SceneEntrance gTakaraHashiraEntrances5[];
extern const struct SceneEntrance gTakaraHashiraEntrancesOther[];

extern const u32 gTakaraHashiraExits[];

extern const struct ScenePlacement gTakaraHashiraPlacements1[];
extern const struct ScenePlacement gTakaraHashiraPlacements2[];
extern const struct ScenePlacement gTakaraHashiraPlacements3[];
extern const struct ScenePlacement gTakaraHashiraPlacements5[];
extern const struct ScenePlacement gTakaraHashiraPlacementsOther[];

s32 Engine_RandomNext();
extern u32 gBgScroll[];

/* Copy one map cell's attribute fields (all but the tile) from src into the
 * cell at x, y of a map layer. */
void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *src)
{
    struct MapState *map = *(struct MapState **)&gMapWork;

    if (map != 0) {
        struct MapCell *cell = map->layers[layer].cells;

        cell += x + (y << 7);

        cell->layer = src->layer;
        /* FAKEMATCH: the kind is read as the top of the source byte, not
         * through the bitfield, as the reference loads it. */
        cell->kind = ((u8 *)src)[1] >> 6;
        cell->height = src->height;
        cell->event = src->event;
    }
}

/* Steps the actor's column row by row until the terrain rises above its own
 * level, then prepares object 223 or 253 at that step and waits for the actor
 * to fall to it. */
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
    height = Map_GetTerrainHeightFar(2, actor->x.fixed, actor->z.fixed);
    level = height / 0x100000;
    rows = level;
    if (level < 0) {
        rows = -level;
    }
    rows++;
    for (row = 0; row <= rows; row++) {
        top = Map_GetTerrainHeightFar(actor->unknown_22, actor->x.fixed, (row << 20) + actor->z.fixed);
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
            /* FAKEMATCH: a do-while(0) around the object and height lets sched2
               issue the height arithmetic first, as the reference does */
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

/*
 * Each Func_ name labels the call word of one call site rather than a
 * runtime address.  The first call is made before r0 is disturbed, so the
 * index is passed straight through instead of being materialised again.  The
 * two exits differ: the thirty-two frame cap returns without pinning, while
 * the clamp path pins the record to exactly 0x1999.
 */
void StagedActor_StepDownUntilClamp(s32 index)
{
    u8 *obj = Actor_Get(index);
    u32 cnt;

    obj[0x55] = 0;

    cnt = 0;
    for (;;) {
        if (cnt > 31) return;
        Stage_Wait(1);
        *(s32 *)(obj + 28) += -0x1999;
        *(s32 *)(obj + 12) += -0xcccc;
        cnt++;
        if (*(s32 *)(obj + 28) <= 0x1998) {
            *(s32 *)(obj + 28) = 0x1999;
            return;
        }
    }
}

void FieldScene_RunPrimarySequence(void)
{
    s32 FieldScene_RunScene3b3SequenceD();

    s32 rec;
    s32 flag;
    s32 p6;
    s32 p5;
    s32 record;
    s32 v1;
    s32 v2;
    s32 v3;
    u8 *base;
    u8 slot16[40];

    rec = Actor_Get(ACTOR_PARTY_LEADER);
    flag = gFrameCount & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Random_Next();
        p6 = *(s32 *)(rec + 8) + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Random_Next();
        p5 = *(s32 *)(rec + 16) + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Random_Next();
        record = Math_Divide((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Effect_Spawn(p6, *(s32 *)(rec + 12), p5, 0, record, flag, 0x90001, (s32)base);
    }
}

s32 FieldScene_RunScene3b3SequenceD(void)
{
    s32 FieldScene_RunScene3b3SequenceD();

    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
    s32 *p;
    s32 buf[3];

    rec = Actor_Get(ACTOR_PARTY_LEADER);
    pflag = rec + 85;
    saved = *pflag;
    mode = (*(u16 *)(rec + 6) + 0x2000) & 0xc000;
    if (gCell[249][0] != 0) {
        return 0;
    }
    p = buf;
    p[0] = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(s32 *)(rec + 12);
    p[2] = (*(s32 *)(rec + 16) & -0x100000) + 0x80000;
    Vector_AddPolarOffset(0x100000, mode, (s32)p);
    if (Object_CheckMovementCollision((s32)rec, (s32)p) == 1) {
        goto reject;
    }
    if (StagedActor_FindAtTile((s32)p, (s32)rec) != 0) {
        goto reject;
    }
    p[0] = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(s32 *)(rec + 12);
    p[2] = (*(s32 *)(rec + 16) & -0x100000) + 0x80000;
    Vector_AddPolarOffset(0x200000, mode, (s32)p);
    if (StagedActor_FindAtTile((s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Object_CheckMovementCollision((s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Event_Begin();
    Stage_SetMode((s32)rec, 6);
    Stage_Wait(6);
    Audio_PlayCue(152);
    Stage_SetMode((s32)rec, 7);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    *(s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Actor_SetSpriteFlags((s32)rec, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Stage_SetMode((s32)rec, 6);
    Actor_SetSpriteFlags((s32)rec, 1);
    *pflag = saved;
    Event_End();
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
    Object_SetPalette(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}

/* Where the party appears in each of the pillar rooms. */
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

/* The actors placed in the pillar rooms; the fourth room takes the others'. */
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
    Event_Begin();
    if (FieldScene_RunScene3b3SequenceD() == 0)
        StagedActor_AdvancePair();
    Event_End();
}

/*
 * Scene state reset for overlay resource_3b3. The callee name refers to its
 * own call word rather than to a shared runtime address.
 */
void SceneState_ApplyPlacementResult(void)
{
    StagedActorMovementRequest out;

    Event_Begin();
    if (StagedActor_FindClearPosition((struct StagedActorProbe *)&out) != 0)
        SceneActor_MoveAndRedraw(out);
    Event_End();
}

/* Copy the background scroll offsets for this frame, occasionally from the
 * shaken set while the display is outside the visible lines. */
void TakaraHashira_JitterBackgroundScroll(void)
{
    u32 line = *(volatile u16 *)0x04000006;
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
