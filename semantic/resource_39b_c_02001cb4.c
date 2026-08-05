#include "types.h"

/*
 * Complete 324-byte movement owner, including the five-word pool through
 * 0x02001df7.  The four apparent returns in the body are calls through the
 * relocated fixed-point helper bank, not owner boundaries.  This is the
 * resource_39b instance of the independently reviewed resource_39c:5244
 * owner; its object fields, thresholds, helper order and display update were
 * checked against this image rather than inferred from the homolog.
 */
typedef s32 (*BinaryFixedHelper)(s32, s32);





extern s32 Func_02004344(s32);
extern s32 Func_02004162(s32);
extern s32 Func_020043cc(s32, s32);
extern s32 Func_020043e2(s32, s32);
void Func_02001cb4(u8 *object)
{
    u8 *target = *(u8 **)(object + 0x68);
    s32 target_x = *(s32 *)(target + 8);
    BinaryFixedHelper multiply = (BinaryFixedHelper)0x03000118;
    s32 target_z = *(s32 *)(target + 16);
    s32 dx = target_x - *(s32 *)(object + 8);
    s32 dz = target_z - *(s32 *)(object + 16);
    s32 tile_x = (dx < 0 ? dx + 0xffff : dx) >> 16;
    s32 tile_z = (dz < 0 ? dz + 0xffff : dz) >> 16;
    s32 distance;
    s32 step;
    u8 *display;
    u8 *metadata;
    u32 frame;

    *(s32 *)(object + 0x30) = 0x20000;
    *(s32 *)(object + 0x34) = 0x10000;
    *(s32 *)(object + 0x38) = (s32)0x80000000;
    *(s32 *)(object + 0x3c) = (s32)0x80000000;
    *(s32 *)(object + 0x40) = (s32)0x80000000;

    distance = Func_02004344(tile_x * tile_x + tile_z * tile_z) << 16;
    dx = target_x - *(s32 *)(object + 8);
    dz = target_z - *(s32 *)(object + 16);
    if (distance < 0x400000)
        distance = Func_02004162(multiply(dx, dx) + multiply(dz, dz));

    step = (distance < 0 ? distance + 7 : distance) >> 3;
    if (step > *(s32 *)(object + 0x30))
        step = *(s32 *)(object + 0x30);

    if (distance < 0x4000) {
        *(s32 *)(object + 8) = target_x;
        *(s32 *)(object + 16) = target_z;
    } else {
        if (distance > step) {
            dx = multiply(Func_020043cc(distance, dx), step);
            dz = multiply(Func_020043e2(distance, dz), step);
        }
        *(s32 *)(object + 8) += dx;
        *(s32 *)(object + 16) += dz;
    }

    frame = *(volatile u32 *)0x03001e40;
    display = *(u8 **)(object + 0x50);
    metadata = *(u8 **)(display + 0x28);
    metadata[5] = (u8)(((frame >> 1) & 1) * 7);
    display[0x25] = 1;
}
