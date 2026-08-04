#include "types.h"

s32 Func_080022ec(s32 value, s32 divisor);
s32 Func_080022fc(s32 value, s32 divisor);
void Func_08009008(s32 object, s32 *position, s32 *projection, s32 flags);

/*
 * Place one of four predefined object groups around a caller-provided origin.
 * The first group forms a 3-by-3 grid; the others use compact ROM coordinate
 * tables. Modes zero through two share one projection, while mode three uses
 * the alternate projection pair.
 */
void Func_080e6d3c(s32 mode, s32 origin_x, s32 origin_z)
{
    u32 *objects = (u32 *)(*(u8 **)0x03001EEC + 0x77D8);
    const s32 *primary_projection = (const s32 *)0x080EDAB8;
    const s32 *alternate_projection = (const s32 *)0x080EDAC0;
    s32 projection[2];
    s32 position[4];
    s32 i;

    position[1] = 0xFF0000;
    position[3] = 0;

    switch (mode) {
    case 0:
        projection[0] = primary_projection[0];
        projection[1] = primary_projection[1];
        for (i = 0; i < 9; i++) {
            position[0] =
                origin_x + (Func_080022fc(i, 3) << 21);
            position[2] =
                origin_z + (Func_080022ec(i, 3) << 21);
            Func_08009008(objects[i], position, projection, 0);
        }
        break;

    case 1: {
        const u8 *x_offsets = (const u8 *)0x080EEE1E;
        const u8 *z_offsets = (const u8 *)0x080EEE2A;

        projection[0] = primary_projection[0];
        projection[1] = primary_projection[1];
        for (i = 0; i < 12; i++) {
            position[0] =
                origin_x + ((s32)x_offsets[i] << 16) - 0x100000;
            position[2] =
                origin_z + ((s32)z_offsets[i] << 16) - 0x200000;
            Func_08009008(objects[i], position, projection, 0);
        }
        break;
    }

    case 2: {
        const u8 *x_offsets = (const u8 *)0x080EEE36;
        const u8 *z_offsets = (const u8 *)0x080EEE3E;

        projection[0] = primary_projection[0];
        projection[1] = primary_projection[1];
        for (i = 0; i < 8; i++) {
            position[0] =
                origin_x + ((s32)x_offsets[i] << 16) + 0x100000;
            position[2] = origin_z + ((s32)z_offsets[i] << 16);
            Func_08009008(objects[i], position, projection, 0);
        }
        break;
    }

    case 3: {
        const u8 *x_offsets = (const u8 *)0x080EEE46;
        const u8 *z_offsets = (const u8 *)0x080EEE4E;

        projection[0] = alternate_projection[0];
        projection[1] = alternate_projection[1];
        for (i = 0; i < 8; i++) {
            position[0] = origin_x + ((s32)x_offsets[i] << 16);
            position[2] = origin_z + ((s32)z_offsets[i] << 16);
            Func_08009008(objects[i], position, projection, 0);
        }
        break;
    }
    }
}
