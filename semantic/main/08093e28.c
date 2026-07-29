#include "object_runtime.h"
#include "types.h"

#define TRANSITION_ACTIVE (*(volatile u8 *)0x02000432)
#define CURRENT_OBJECT_ID (*(volatile u32 *)0x02000434)

struct ObjectRuntime *Func_08092054(u32 objectId);
void Func_080916b0(void);
void Func_08091750(void);
s8 Func_08009220(const s32 *position);
void Func_08092158(u32 objectId, s32 x, s32 z);
void Func_08092adc(u32 objectId, s32 value, s32 effect);
void Func_080920e8(u32 objectId);
void Func_080091e0(struct ObjectRuntime *object, u32 mode);
void Func_08009080(struct ObjectRuntime *object, s32 mode);
void Func_08009150(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_080923c4(u32 objectId);
void Func_0809163c(s32 effect);

/*
 * Toggle a two-phase object transition.  The opening phase validates the
 * destination tile and probes a stack-backed 3D position before moving the
 * current object.  The closing phase restores its motion/display state.
 */
s32 Func_08093e28(void)
{
    struct ObjectRuntime *object = Func_08092054(CURRENT_OBJECT_ID);
    s32 alignedX = ((s16)((u32)object->x >> 16) & 0xfff0);
    s32 alignedZ = ((s16)((u32)object->z >> 16) & 0xfff0);
    s32 centerX = alignedX + 8;
    s32 centerZ = alignedZ + 8;

    Func_080916b0();
    if (TRANSITION_ACTIVE == 0) {
        s32 tileX = centerX < 0 ? alignedX + 23 : centerX;
        s32 tileZ = centerZ < 0 ? alignedZ + 23 : centerZ;
        u32 tileOffset =
            ((u32)(tileX >> 4) + ((u32)(tileZ >> 4) << 7)) << 2;

        if (*(u8 *)(0x02010002 + tileOffset) ==
            *(u8 *)(0x02010202 + tileOffset)) {
            s32 position[3];
            s8 probeResult;

            position[0] = object->x;
            position[1] = (s32)((u32)object->y - 0x100000);
            position[2] = object->z;
            probeResult = Func_08009220(position);
            if (probeResult == 0) {
                Func_08092158(CURRENT_OBJECT_ID, centerX, centerZ);
                object->speed_limit = 0x10000;
                Func_08092adc(CURRENT_OBJECT_ID, 0xc000, 0);
                Func_080920e8(CURRENT_OBJECT_ID);
                object->action_flags = 1;
                object->flags = probeResult;
                Func_080091e0(object, 0);
                Func_08009080(object, 13);
                Func_08009150(
                    object,
                    (s32)((u32)centerX << 16),
                    (s32)((u32)object->y - 0x100000),
                    (s32)(((u32)centerZ << 16) + 0x100000));
                Func_080923c4(CURRENT_OBJECT_ID);
                TRANSITION_ACTIVE = 1;
                goto success;
            }
        }

        Func_08091750();
        return -1;
    }

    Func_08009080(object, 10);
    object->flags = 3;
    object->velocity_y = 0x40000;
    object->terrain_height = object->y;
    Func_080091e0(object, 1);
    Func_0809163c(6);
    TRANSITION_ACTIVE = 0;
    object->action_flags = 1;
    *(u16 *)((u8 *)object + 6) = 0xc000;
success:
    Func_08091750();
    return 0;
}
