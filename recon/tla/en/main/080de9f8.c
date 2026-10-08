#include "resource.h"
#include "types.h"
#include "fx_scene.h"
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void RunBattleEffect03(void)
{
    struct BattleFxScene *state = gEffectWork;
    struct BattleEffect03Object *target = state->main_object;
    struct BattleEffect03Object *object;
    struct BattleEffect03Link *last;
    u8 link_marker;
    s32 i;

    BattleEffect_InitializeSharedScene();
    last = 0;
    for (i = 0; i < 8; i++) {
        object = Object_Spawn(
            0xe9, target->x, target->y + 0x200000, target->z);
        if (object != 0) {
            object->scale_y = 0xb333;
            object->scale_x = 0xb333;
            object->callback = BattleFx_UpdateShrinkingOrbitObject;
            object->angle = 0x78;
            object->phase = i << 13;
            object->mode = 4;
            last = Object_ReplaceResourceEntry(object->visual, last);
        }
        WaitFrames(1);
    }

    link_marker = last->marker;
    Audio_PlayCue(0x82);
    WaitFrames(110);
    target = Object_Spawn(0xe9, 0, 0, 0);
    /* FAKEMATCH: the ROM keeps object as a second copy of the new spark (r5 beside r6); a plain object = target is copy-propagated away by gcse. */
    asm("" : "=r"(object) : "0"(target));
    if (target != 0) {
        target->scale_y = 0xb333;
        target->scale_x = 0xb333;
        target->x = state->x;
        target->y = state->y + 0x100000;
        target->z = state->z;
        target->mode = 4;
        Animation_ApplyChildValuesFar(target, 7);
    }

    Audio_PlayCue(0x83);
    WaitFrames(12);
    if (target != 0) {
        for (i = 0; i < 30; i++) {
            if (i & 3)
                Animation_ApplyChildValuesFar(object, 9);
            else
                Animation_ApplyChildValuesFar(object, 10);
            WaitFrames(2);
        }
    }

    Animation_ApplyChildValuesFar(object, 0);
    Audio_PlayCue(0x54);
    if (object != 0) {
        target->callback = BattleFx_RunSparkEmitter;
        target->angle = 0;
        if (state->enabled != 0)
            WaitFrames(128);
        else
            WaitFrames(192);
    }
    if (target != 0) {
        target->angle = -1;
        target->velocity_x = 0x50000;
        target->velocity_y = 0x6666;
        target->unknown_5a = 0;
        Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
        Object_CommitPosition(target);
        Object_Destroy(target);
    }
    if (link_marker != 0x60)
        Resource_ResetEntry(link_marker);
    if (state->finish_callback != 0)
        state->finish_callback();
    BattleFx_PrepareBufferInterpolation();
}
