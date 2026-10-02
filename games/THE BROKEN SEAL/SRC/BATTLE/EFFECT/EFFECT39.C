/*
 * Battle effect 7: two mode-3 item-break anchors fly apart, then
 * twenty-four shards scatter from the scene centre at random speeds and
 * angles.
 */
#include "TYPES.H"
#include "OBJDISP.H"
#include "SYSTEM.H"
#include "SCENE.H"
#include "SOUND_IDS.H"
#include "FIXED_MATH.H"

struct BattleEffectScene {
    u8 reserved_00[4];
    s32 x;
    s32 y;
    s32 z;
};

struct ScatterShard {
    u8 reserved_00[0x30];
    s32 speed;                      /* 0x30 */
    s32 lift;                       /* 0x34 */
    u8 reserved_38[0x55 - 0x38];
    u8 flag;                        /* 0x55 */
};

struct ScatterPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern struct BattleEffectScene *gEffectWork;
void BattleEffect_InitializeSharedScene(void);
void *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void Motion_SetTargetPositionFromMagnitudeAngle(
    void *object, s32 magnitude, s32 angle);
void Object_CommitPosition(void *object);
void Audio_PlayCue(s32 sound);
struct ScatterShard *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
extern const u8 BattleFx_FragmentScript[];
void Object_Destroy(void *object);
void BattleFx_PrepareBufferInterpolation(void);

static __inline__ struct ScatterShard *SpawnShard(
    struct BattleEffectScene *scene, struct ScatterPosition *position)
{
    /* FAKEMATCH: the inline interface preserves the separate shard cursor. */
    position->x = scene->x;
    position->y = scene->y + 0x100000;
    position->z = scene->z;
    return Object_Spawn(0x11d, position->x, position->y, position->z);
}

/* battle/effects/item_break/spawn_mode_3.c */
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);

void RunBattleEffect07(void)
{
    struct BattleEffectScene *scene;
    struct ScatterShard *obj;
    void *anchors[2];
    struct ScatterPosition position;
    void **walk;
    s32 index;
    s32 magnitude;

    scene = gEffectWork;
    BattleEffect_InitializeSharedScene();
    position.x = scene->x;
    position.y = scene->y + 0x100000;
    position.z = scene->z;
    anchors[0] = BattleFx_SpawnItemBreakMode3(
        position.x + 0x200000, position.y, position.z, 0x8000);
    anchors[1] = BattleFx_SpawnItemBreakMode3(
        position.x - 0x200000, position.y, position.z, 0);

    WaitFrames(15);
    walk = anchors;
    for (index = 0; index < 2; index++) {
        obj = *walk++;
        if (obj != 0)
            Motion_SetTargetPositionFromMagnitudeAngle(
                obj, 0x180000, *(u16 *)((u8 *)obj + 6));
    }

    Object_CommitPosition(anchors[0]);
    Audio_PlayCue(134);
    for (index = 0; index < 24; index++) {
        obj = SpawnShard(scene, &position);
        if (obj != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)obj, (u32)BattleFx_FragmentScript);
            obj->speed = Random16() + 0x20000;
            obj->lift = 0x20000;
            obj->flag = 0;
            magnitude = Random16() * 24 + 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(obj, magnitude, Random16());
        }
    }

    Object_Destroy(anchors[0]);
    Object_Destroy(anchors[1]);
    BattleFx_PrepareBufferInterpolation();
}

void *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle)
{
    u8 *obj;
    s32 v;

    Audio_PlayCue(SOUND_ITEM_BREAK);
    obj = Object_Spawn(215, x, y, z);
    if (obj != NULL) {
        *(s32 *)(obj + 0x18) = *(s32 *)(obj + 0x1C) = 0x4000;
        *(s32 *)(obj + 0x34) = *(s32 *)(obj + 0x30) = 0x30000;
        *(s8 *)(*(u8 **)(obj + 0x50) + 9) &= ~0xC;
        Object_SetMode(obj, 3);
        v = *(s32 *)(obj + 0x18);
        if (v < 0x10000) {
            do {
                v += 0x800;
                *(s32 *)(obj + 0x1C) = v;
                *(s32 *)(obj + 0x18) = v;
                *(u16 *)(obj + 6) += 0x2000;
                WaitFrames(1);
                v = *(s32 *)(obj + 0x18);
            } while (v <= 0xFFFF);
        }
        *(u16 *)(obj + 6) = (u16)angle;
    }
    return obj;
}
