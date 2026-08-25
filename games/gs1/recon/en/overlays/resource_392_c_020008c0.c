#include "types.h"

extern u8 *Data_03001e70;
extern s32 Data_02008f08[];
extern s32 Data_02008f20[];
extern u8 *Func_02001706(s32 actor);
extern void Func_0200177c(s32, s32, s32, s32, s32, s32);
extern s32 Func_02000bf0(u32, s32, s32, u32, u32, s32);
extern s32 Func_02000c02(u32, s32, s32, u32, u32, s32);

#define StagedActorProbeValues Data_02008f08
#define StagedActorProbeBounds Data_02008f20
#define GetStagedActor Func_02001706
#define DrawProbeRectangle Func_0200177c
#define FillLowerGridProbeRectangle Func_02000bf0
#define FillUpperGridProbeRectangle Func_02000c02
#define RedrawStagedActorProbeRectangle Func_020008c0

struct StagedActorProbeRectangle {
    s32 probe;
    s32 unused;
    s32 x;
    s32 y;
    s32 z;
};

s32 RedrawStagedActorProbeRectangle(s32 actor_index)
{
    u8 *runtime = Data_03001e70;
    u8 *actor = GetStagedActor(actor_index);
    volatile struct StagedActorProbeRectangle rectangle;
    s32 probe = 0;
    s32 probe_value;
    s32 *probe_values = StagedActorProbeValues;
    s32 *bounds;
    s32 width;
    s32 height;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 world_x;
    s32 world_z;

    probe_value =
        **(s16 **)(*(u8 * volatile *)(actor + 0x50) + 0x28);
    if (probe_value == *probe_values) goto matched_probe;
    do {
        rectangle.probe = 7;
        probe++;
        if (probe > 5) goto finished_probe_search;
        probe_value =
            **(s16 **)(*(u8 * volatile *)(actor + 0x50) + 0x28);
        probe_values++;
    } while (probe_value != *probe_values);
matched_probe:
    rectangle.probe = probe;
finished_probe_search:
    if ((u32)rectangle.probe > 6) return 0;

    rectangle.x = *(s32 *)(actor + 8);
    rectangle.y = *(s32 *)(actor + 12);
    rectangle.z = *(s32 *)(actor + 16);
    bounds = StagedActorProbeBounds + rectangle.probe * 4;

    top = bounds[1];
    if (top < 0) top = -top;
    bottom = bounds[3];
    if (bottom < 0) bottom = -bottom;
    height = (top + bottom) >> 4;

    left = bounds[0];
    if (left < 0) left = -left;
    right = bounds[2];
    if (right < 0) right = -right;
    width = (left + right) >> 4;

    rectangle.x = (rectangle.x + (bounds[0] << 16)) >> 20;
    rectangle.z = (rectangle.z + (bounds[1] << 16)) >> 20;
    world_x = rectangle.x + (*(s32 *)(runtime + 0x13c) >> 20);
    world_z = rectangle.z + (*(s32 *)(runtime + 0x140) >> 20);

    DrawProbeRectangle(
        rectangle.x, rectangle.z, width, height, world_x, world_z);
    FillLowerGridProbeRectangle(
        0, rectangle.x, rectangle.z, width, height, 255);
    FillUpperGridProbeRectangle(
        2, rectangle.x, rectangle.z, width, height, 255);
    return 1;
}
