#include "types.h"
#include "system.h"
#include "object_runtime.h"
#include "fx_scene.h"
#include "ram_buffer.h"

struct ResourceEntry
{
    u8 unk00[0x10];
    u8 vramBlock;
};

void                  Func_080de9f8(void);
void                  EffectRuntime_StopCurrentObject();
void                  BattleEffect_InitializeSharedScene(void);
void                  BattleFx_PrepareBufferInterpolation(void);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
struct ResourceEntry *Object_ReplaceResourceEntry(void *sprite, struct ResourceEntry *last);
void                  Animation_ApplyChildValuesFar(struct ObjectRuntime *object, u32 value);
void                  Motion_SetTargetPositionFromMagnitudeAngle(
    struct ObjectRuntime *object, s32 magnitude, s32 angle);
void Object_Destroy(struct ObjectRuntime *object);
s32  Resource_ResetEntry(u32 entry);
void BattleFx_UpdateShrinkingOrbitObject(struct ObjectRuntime *particle);
void Func_080de8d0(struct ObjectRuntime *object);

void Func_080de9ec(void)
{
    Func_080de9f8();
    EffectRuntime_StopCurrentObject();
}

// Eight particles spiral in on the main object, one a frame; then a centre
// object flashes for 60 frames, emits sparks and flies off.
void Func_080de9f8(void)
{
    struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
    struct ObjectRuntime *target = scene->mainObject;
    struct ObjectRuntime *object;
    struct ResourceEntry *last;
    u8                    marker;
    s32                   i;

    BattleEffect_InitializeSharedScene();

    last = 0;
    for (i = 0; i < 8; i++)
    {
        object = Object_Spawn(296, target->x, target->y + 0x200000, target->z);
        if (object != 0)
        {
            object->scaleY = 0xb333;
            object->scaleX = 0xb333;
            object->update = (void (*)(void))BattleFx_UpdateShrinkingOrbitObject;
            object->action = 120;
            object->unk66 = i << 13;
            object->motionFlags = 4;
            last = Object_ReplaceResourceEntry(object->animation, last);
        }
        WaitFrames(1);
    }

    marker = last->vramBlock;
    Audio_PlayCue(130);
    WaitFrames(110);

    target = Object_Spawn(296, 0, 0, 0);
    object = target;
    if (target != 0)
    {
        target->scaleY = 0xb333;
        target->scaleX = 0xb333;
        target->x = scene->x;
        target->y = scene->y + 0x100000;
        target->z = scene->z;
        target->motionFlags = 4;
        Animation_ApplyChildValuesFar(target, 7);
    }

    Audio_PlayCue(131);
    WaitFrames(12);
    if (target != 0)
    {
        for (i = 0; i < 30; i++)
        {
            /* FAKEMATCH: the native keeps object as a second register copy of the centre object (r5 beside r6); this redundant reassignment stops gcse copy-propagating object = target away, as in ☀️'s RunBattleEffect03. */
            target = object;
            if (i & 3)
                Animation_ApplyChildValuesFar(object, 9);
            else
                Animation_ApplyChildValuesFar(object, 10);
            WaitFrames(2);
        }
    }

    Animation_ApplyChildValuesFar(object, 0);
    Audio_PlayCue(84);
    if (object != 0)
    {
        target->update = (void (*)(void))Func_080de8d0;
        target->action = 0;
        if (scene->enabled != 0)
            WaitFrames(128);
        else
            WaitFrames(192);
    }

    if (target != 0)
    {
        target->action = -1;
        target->speedLimit = 0x50000;
        target->acceleration = 0x6666;
        target->actionFlags = 0;
        Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
        Object_CommitPosition(target);
        Object_Destroy(target);
    }
    if (marker != 0x60)
        Resource_ResetEntry(marker);
    if (scene->finishCallback != 0)
        scene->finishCallback();
    BattleFx_PrepareBufferInterpolation();
}
