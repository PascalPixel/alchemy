#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"

/* main:0800c62c ObjectSystem_UpdateCamera - hand-written draft, 144 of 296
   halfwords differ (588 of 592 bytes).

   Each frame the field camera places every live object on screen: objects
   inside the view are projected through their tile's layer bits, and objects
   that leave it (or sit at the origin) fall back to their resource entry.

   Residual: the first view test is emitted as "bls .+4; b far" in the ROM.
   GCC picks that long form from its length estimate, which counts each
   inline asm line as four bytes; the ROM needs one more estimated line per
   Iwram_MulQ16 call than IWRAM_CALL.H has (with one extra line the branch
   flips and 90 halfwords differ). Left beside that: the loop-constant registers (63 and the MulQ16
   routine swap r3/r4), and the tail's flag test (ROM keeps kind in a copy:
   "adds r3, r6, #0; ands r3, r2"). */

struct CameraTile {
    u32 unk_00 : 12;
    u32 layer : 2;
    u32 priority : 2;
    u32 unk_10 : 16;
};

struct CameraSprite {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 flip_x : 1;
    u16 flip_y : 1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[0x0a];
    u16 second_tile : 10;
    u16 second_priority : 2;
    u16 second_palette : 4;
    u8 unknown_16[2];
    s32 scale;
    u8 resource;
    u8 flags_1d;
    u8 unk_1e[7];
    u8 activated;
};

struct CameraObject {
    u32 active;
    u16 unk_04;
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[2];
    u8 layer;
    u8 flags;
    u8 unk_24[0x2c];
    struct CameraSprite *sprite;
    u8 kind;
    u8 unk_55[7];
    u8 held;
    u8 unk_5d[0x13];
};

struct CameraLayer {
    struct CameraTile *tiles;
    u8 unk_04[44];
};

struct CameraState {
    u8 unk_000[0xe4];
    s32 x;
    s32 z;
    u8 unk_0ec[0x44];
    struct CameraLayer layers[1];
};

struct CameraSync {
    s16 count;
    s16 unk_02;
    s16 frozen;
};

extern u8 Data_03001e64_a[];
#define FIELD_RUNTIME ((u32)Data_03001e64_a)
extern u8 Data_08009bb8[];
extern u8 Value_000002c4[];

u8 *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Resource_ActivateEntry(u32 resource_index);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale, u16 angle);

void ObjectSystem_UpdateCamera(void)
{
    s32 cnt;
    struct CameraState *state;
    s32 cam_x;
    s32 cam_z;
    struct CameraSync *sync;
    struct CameraObject *obj;
    struct CameraSprite *sprite;
    s32 *cam;
    u32 size;
    u32 kind;
    s32 dx;
    s32 dz;
    s32 top;
    struct CameraTile *tile;
    u32 bits;
    s32 unused[9]; /* the ROM frame is 80 bytes */
    s32 scale[2];
    s32 pos[4];
    s32 y;
    s32 height;
    u32 bits_val;

    state = *(struct CameraState **)(FIELD_RUNTIME + 12);
    cam = &state->x;
    cam_x = cam[0] & 0xffff0000;
    cam_z = cam[1] & 0xffff0000;
    sync = *(struct CameraSync **)(FIELD_RUNTIME + 4);
    /* FAKEMATCH: the do-while keeps the size load after the runtime loads. */
    do { size = (u32)Value_000002c4; } while (0);
    Dma_Set(Data_08009bb8, Runtime_AllocateHeapBlock(52, size), 0x84000000 | (size >> 2),
        (volatile u32 *)0x040000d4);
    obj = *(struct CameraObject **)FIELD_RUNTIME;
    sync->count = 0;
    for (cnt = 63; cnt >= 0; cnt--, obj++) {
        if (obj->active == 0)
            continue;
        if (obj->x != 0 || obj->z != 0) {
            kind = obj->kind & 15;
            if (kind == 0)
                continue;
            if (kind != 1)
                continue;
            if (sync->frozen != 0 && obj->held == 0) {
                struct CameraSprite *frozen = obj->sprite;

                Resource_ActivateEntry(frozen->resource);
                frozen->activated = kind;
                continue;
            }
            dx = obj->x - cam_x;
            dz = obj->z - cam_z;
            top = dz - obj->y;
            sprite = obj->sprite;
            if (dx > -0x200000 && dx < 0x1100000 && top > -0x200000 && top < 0xe00000) {
                tile = &state->layers[obj->layer].tiles[(obj->x >> 20) + ((obj->z >> 20) << 7)];
                if (obj->flags & 1) {
                    bits = tile->priority;
                    if (bits != 0) {
                        sprite->priority = bits;
                        sprite->second_priority = bits;
                    }
                }
                bits = tile->layer;
                if (bits != 0)
                    obj->layer = bits - 1;
                scale[0] = Iwram_MulQ16(obj->scale_x, sprite->scale);
                scale[1] = Iwram_MulQ16(obj->scale_y, sprite->scale);
                pos[0] = dx;
                pos[1] = obj->y;
                pos[2] = dz;
                pos[3] = obj->height;
                if (obj->flags & 2) {
                    pos[1] += 0xfec00000;
                    pos[2] += 0xfec00000;
                    pos[3] += 0xfec00000;
                }
                if (obj->flags & 4) {
                    pos[1] += 0x1400000;
                    pos[2] += 0x1400000;
                    pos[3] += 0x1400000;
                }
                Render_ApplyProjectedPlacement(sprite, pos, scale, obj->angle);
                continue;
            }
            if (obj->held == 0) {
                kind = 1;
                if (!(sprite->flags_1d & kind)) {
                    Resource_ActivateEntry(sprite->resource);
                    sprite->activated = kind;
                }
            }
        } else {
            kind = obj->kind & 15;
            if (kind == 1) {
                sprite = obj->sprite;
                if (obj->held == 0 && !(sprite->flags_1d & kind)) {
                    Resource_ActivateEntry(sprite->resource);
                    sprite->activated = kind;
                }
            }
        }
    }
    Runtime_ReleaseHeapBlock(52);
}
