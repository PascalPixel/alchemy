/* Draft of Func_02001940 (overlay 679, 185 instructions), differ 90: the ground
 * particles' update, ported from ☀️'s FieldFx_UpdateGroundParticles (⚓️ has 64
 * particles in heap slot 29, a leader pointer at 0x808 that may be NULL, and new
 * positions leader + Random16Far() * 240 - 0x780000 and * 160 - 0x500000, with the
 * terrain height unshifted). As ☀️'s for loop, loop.c adds a count-down register
 * (differ 105); as this goto loop the counter stays in r10 as the native's. Left,
 * mostly allocation: the frame pointer takes r6 and the screen y r4, where the native
 * has r4 and r6 (y then shares r6 with the leader); the bitfield stores interleave
 * differently. Measured: the declaration order of frame, x and y (no change); new
 * variables for the new position (91, 161). */
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

struct GroundParticle {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[2];
    s32 pos_x;
    s32 pos_y;
    s32 pos_z;
    s32 delay;
    u16 timer;
    u8 unknown_1e[2];
};

struct GroundParticleWork {
    s32 entry;
    s32 tile_base;
    struct GroundParticle particles[64];
    s32 *leader;
};

extern s16 Data_02001f18[];

s32 GameFlag_Test(s32 flag);
s32 Random16Far(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02001c3c(void *entry, s32 slot);

void Func_02001940(void)
{
    struct MapWork *view = Ram_HeapSlots->map_work;
    struct GroundParticleWork *work = Ram_HeapSlots->unknown_70[1];
    s32 *camera = &view->view_x;
    s32 camera_x = camera[0];
    s32 camera_z = camera[1];
    struct GroundParticle *particle = work->particles;
    u32 i;

    i = 0;
loop:
    {
        s16 *frame;
        s32 x;
        s32 y;

        if (--particle->timer == 0xffff)
            goto next;
        if (GameFlag_Test(0x166))
            particle->timer++;
        frame = &Data_02001f18[5 * particle->timer];
        x = (particle->pos_x - camera_x) / 0x10000 + *frame++;
        y = (particle->pos_z - particle->pos_y - camera_z) / 0x10000 + *frame++;
        if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
            particle->priority = 1;
            particle->x = x;
            particle->y = y;
            particle->tile = work->tile_base + *(u16 *)frame;
            frame++;
            particle->shape = *(u8 *)frame;
            particle->size = *(u8 *)(frame + 1);
            Func_02001c3c(particle, 240);
        }
        if (particle->timer == 0) {
            s32 *leader = work->leader;

            if (leader != NULL) {
                x = leader[0] + Random16Far() * 240 - 0x780000;
                y = leader[2] + Random16Far() * 160 - 0x500000;
                particle->pos_x = x;
                particle->pos_z = y;
                particle->pos_y = Map_GetTerrainHeight(0, x, y);
            } else {
                particle->pos_x = 0;
                particle->pos_z = 0;
                particle->pos_y = 0;
            }
            particle->timer = 16;
        }
    }
next:
    i++;
    particle++;
    if (i < 64)
        goto loop;
}
