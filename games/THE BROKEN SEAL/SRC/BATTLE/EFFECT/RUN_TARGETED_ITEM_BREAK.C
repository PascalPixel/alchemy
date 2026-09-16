#include "TYPES.H"

#define BattleEffect_RunTargetedItemBreak Func_08098848

struct BattleEffectScene {
    u8 reserved_00[4];
    s32 x;
    s32 y;
    s32 z;
    void *main_object;
    void *child;
};

struct EffectChild {
    u8 reserved_00[12];
    s32 y;
    u8 reserved_10[0x55 - 0x10];
    u8 flag;
    u8 reserved_56[0x6c - 0x56];
    void (*callback)(void);
};

extern struct BattleEffectScene *Data_03001f30;

void Func_08097384(void);
void *SpawnItemBreakEffectMode3(s32 x, s32 y, s32 z, s32 angle);
void Object_SetCallback(void *object, const void *callback);
void WaitFrames(s32 frames);
void Motion_SetTargetPositionFromMagnitudeAngle(
    void *object, s32 magnitude, s32 angle);
void Object_CommitPosition(void *object);
void Audio_PlayCue(s32 sound);
void ObjectGroup_ApplyRandomChildValues(void);
void UpdateRisingParticleBurst(void *effect);
void BattleFx_PrepareBufferInterpolation(void);

/* Links the scene child to the main object, spawns two mode-3 item-break
 * anchors on either side of the scene position, sends them outward, then
 * raises both anchors and the child together until the child has risen
 * 0x200000 before finishing both bursts. */
void BattleEffect_RunTargetedItemBreak(void)
{
    struct BattleEffectScene *scene;
    void *main_object;
    void *child;
    void *anchors[2];
    s32 position[3];
    s32 x, y, z;
    s32 index;
    s32 start_y;

    scene = Data_03001f30;
    child = scene->child;
    main_object = scene->main_object;
    if (child == 0)
        return;

    Func_08097384();
    *(void **)((u8 *)main_object + 0x68) = child;
    Object_SetCallback(main_object, (const void *)0x0809f0bc);

    x = scene->x;
    position[0] = x;
    y = scene->y + 0x100000;
    position[1] = y;
    z = scene->z;
    position[2] = z;
    anchors[0] = SpawnItemBreakEffectMode3(x + 0x200000, y, z, 0x8000);
    anchors[1] = SpawnItemBreakEffectMode3(
        position[0] - 0x200000, position[1], position[2], 0);

    WaitFrames(15);
    for (index = 0; index < 2; index++) {
        void *anchor = anchors[index];
        if (anchor != 0)
            Motion_SetTargetPositionFromMagnitudeAngle(
                anchor, 0xe0000, *(u16 *)((u8 *)anchor + 6));
    }

    Object_CommitPosition(anchors[0]);
    ((struct EffectChild *)child)->callback = ObjectGroup_ApplyRandomChildValues;
    Audio_PlayCue(130);
    ((struct EffectChild *)child)->flag = 4;

    start_y = ((struct EffectChild *)child)->y;
    if (anchors[0] != 0 && anchors[1] != 0 &&
        start_y <= start_y + 0x200000) {
        do {
            *(s32 *)((u8 *)anchors[0] + 0xc) += 0x4000;
            *(s32 *)((u8 *)anchors[1] + 0xc) += 0x4000;
            ((struct EffectChild *)child)->y += 0x4000;
            WaitFrames(1);
        } while (((struct EffectChild *)child)->y <= start_y + 0x200000);
    }

    UpdateRisingParticleBurst(anchors[0]);
    UpdateRisingParticleBurst(anchors[1]);
    BattleFx_PrepareBufferInterpolation();
}
