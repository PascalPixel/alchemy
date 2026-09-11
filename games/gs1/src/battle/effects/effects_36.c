#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* battle/effects/scene_transition/finish_and_release_heap_block.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 ScheduleCallback(s32);

struct Outer0808e0b0 *Object_GetById(u32);
void BattleFx_PrepareBufferInterpolation(void);

extern u8 gRom;

void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
    void *work;

    work = *(void **)ADDR_03001EA8;
    Battle_Run();
    ScheduleCallback((s32)&gRom);
    Battle_Apply(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1);
    BattleFx_PrepareBufferInterpolation();
    Battle_Check(0x16);
}

/* battle/effects/color/advance_hue_cycle.c */
extern u8 *gIw;

void BattleFx_AdvanceHueCycle(void)
{
    u8 *base = gIw;
    s32 out1 = 0;
    s32 out2 = 0;
    s32 out3 = 0;
    s32 offset;
    u8 *p0;
    u8 *p1;
    u8 *p2;

    Battle_SetMode((s32)(*(u16 *)(base + 0x28E)) << 16, &out1, &out2, &out3);

    offset = 0x28B;
    p0 = base + offset;
    *p0 = (u8)((out1 >> 18) + 4);
    offset += 1;
    p1 = base + offset;
    *p1 = (u8)((out2 >> 18) + 4);
    offset += 1;
    p2 = base + offset;
    *p2 = (u8)((out3 >> 18) + 4);

    *(u16 *)(base + 0x28E) += 4;

    *p0 &= 0x1F;
    *p1 &= 0x1F;
    *p2 &= 0x1F;

    if (*(u16 *)(base + 0x28E) >= 360) {
        *(u16 *)(base + 0x28E) = 0;
    }
}

/* battle/effects/color/compute_hue_channels.c */
void BattleFx_ComputeHueChannels(s32 value, s32 *maximum, s32 *center, s32 *minimum)
{
    *maximum = Battle_Place(value + 0x780000, 0, 0x1F0000);
    *center = Battle_Place(value, 0, 0x1F0000);
    *minimum = Battle_Place(value + 0xFF880000, 0, 0x1F0000);
}

/* battle/effects/object_control/set_callback_when_target_unset.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetCallback(void *, s32);
extern const u8 gRom[];

struct Target_08097a54 {
    u8 unknown_00[0x38];
    s32 x;
    s32 y;
    s32 z;
};

void BattleFx_SetCallbackWhenTargetUnset(struct Target_08097a54 *target)
{
    s32 ty;
    s32 tx;

    tx = target->x;
    if (tx == 0x80000000) {
        ty = target->y;
        if ((ty == tx) && (target->z == ty)) {
            Object_SetCallback(target, (s32)gRom);
        }
    }
}
