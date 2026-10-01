/* NONMATCHING: localized scene source, 2026-10-01.
 * The complete 316-byte probe caches each packed direction word.
 * Four instructions still use r2 rather than r1 for the z-component.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "HASHIRA.H"
#include "FIELD_SCENE.H"

extern u8 *gMapWork;
extern s32 StagedActor_DirectionSteps[];

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
s32 FieldScene_RunScene3b3SequenceD(void)
{
    s32 FieldScene_RunScene3b3SequenceD();

    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
#if defined(TBS_EDITION_JA)
    s32 step;
#endif
    s32 *p;
    s32 buf[3];

    rec = Actor_Get(ACTOR_PARTY_LEADER);
    pflag = rec + 85;
    saved = *pflag;
#if !defined(TBS_EDITION_JA)
    mode = (*(u16 *)(rec + 6) + 0x2000) & 0xc000;
#endif
    if (gCell[249][0] != 0) {
        return 0;
    }
    p = buf;
#if defined(TBS_EDITION_JA)
    mode = *(u16 *)(rec + 6) >> 12;
    step = StagedActor_DirectionSteps[mode];
    p[0] = *(s32 *)(rec + 8) + (step & 0xffff0000);
    p[1] = *(s32 *)(rec + 12);
    p[2] = *(s32 *)(rec + 16) + (step << 16);
#else
    p[0] = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(s32 *)(rec + 12);
    p[2] = (*(s32 *)(rec + 16) & -0x100000) + 0x80000;
    Vector_AddPolarOffset(0x100000, mode, (s32)p);
#endif
    if (Object_CheckMovementCollision((s32)rec, (s32)p) == 1) {
        goto reject;
    }
    if (StagedActor_FindAtTile((s32)p, (s32)rec) != 0) {
        goto reject;
    }
#if defined(TBS_EDITION_JA)
    step = StagedActor_DirectionSteps[mode];
    p[0] = *(s32 *)(rec + 8) + ((step & 0xffff0000) * 2);
    p[1] = *(s32 *)(rec + 12);
    p[2] = *(s32 *)(rec + 16) + (step << 17);
#else
    p[0] = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(s32 *)(rec + 12);
    p[2] = (*(s32 *)(rec + 16) & -0x100000) + 0x80000;
    Vector_AddPolarOffset(0x200000, mode, (s32)p);
#endif
    if (StagedActor_FindAtTile((s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Object_CheckMovementCollision((s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Engine_EventBegin();
    Object_SetMode((s32)rec, 6);
    WaitFrames(6);
    Audio_PlayCue(152);
    Object_SetMode((s32)rec, 7);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    *(s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Engine_ActorSetSpriteFlags((s32)rec, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Object_SetMode((s32)rec, 6);
    Engine_ActorSetSpriteFlags((s32)rec, 1);
    *pflag = saved;
    Engine_EventEnd();
    return 1;
reject:
    return 0;
}
