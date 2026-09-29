/* NONMATCHING: 488 of 484 bytes, 195 differing halfwords, 105 halfword
 * edits (2026-09-25). Reusing Sparkles map-cell derivation restores the
 * reference opening loads (previously492 bytes,204 differing halfwords).
 * Remaining: tile-mask lifetime, loop/spawn stack slots and screen bounds
 * ordering. This owner renders timed frames and delayed particle bursts.
 * 2026-09-29: alchemy permute took the score from 2922 to 1764 in six
 * minutes; rerun and minimized, 10 of its 12 changed regions give 1703
 * (the dropped ones were worse). The kept regions chain many word
 * temporaries through the particle update (tagged below): they record the
 * scheduling and allocation the search found, not code to adopt.
 */

#include "TYPES.H"
#include "SYSTEM.H"

s32 GameFlag_TestFar(s32 flag);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Runtime_PushSlotEntry(void *entry, s32 slot);

struct Particle {
    void *next;
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
    u16 unknown_0a;
    s32 pos_x;
    s32 pos_y;
    s32 pos_z;
    s32 fall;
    u16 timer;
    u16 unknown_1e;
};

struct ParticleWork {
    u32 unknown_00;
    s32 tile;
    struct Particle particles[32];
};

struct MapPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct CameraWork {
    struct MapPosition *leader;
    u8 unknown_04[0xe0];
    s32 camera_x;
    s32 camera_z;
};

extern struct ParticleWork *gParticleWork;
/* FAKEMATCH: derive the adjacent map cell from the particle-work cell,
 * as in the exact Sparkles routine, to preserve the reference loads. */
#define gCamera (*(struct CameraWork **)((u8 *)&gParticleWork - 84))
extern unsigned long gFrameCount;

void Unnamed_08094bbc(void)
{
    /* FAKEMATCH: the tmp locals are the permuter's, kept for their schedule. */
    register struct CameraWork *camera;
    register struct ParticleWork *work;
    u32 i;
    s32 spawned;
    struct Particle *p;
    s32 cam_x;
    s32 cam_y;
    s32 sx;
    s16 lift;
    s32 sy;
    s32 tmp9;

    work = gParticleWork;
    spawned = 0;
    p = (struct Particle *)work->particles;
    i = 0;
    tmp9 = i < 32;
    if (tmp9) {
        do {
            if (p[0].timer--) {
                s32 tmp;
                s32 tmp2;
                register s32 tmp5;
                s32 tmp10;
                u32 tmp8;
                s32 tmp11;
                s32 tmp13;
                s16 tmp15;
                s32 tmp16;
                s32 tmp17;
                cam_x = camera->camera_x;
                tmp = camera->camera_z;
                tmp15 = (s16)p->timer;
                tmp11 = GameFlag_TestFar(0x166);
                lift = tmp15;
                tmp2 = tmp11;
                if (tmp2 != 0) {
                    p->timer++;
                    p[0].fall--;
                }
                tmp8 = Random16();
                sx = ((p->pos_x - cam_x) >> 16) + (((1 & tmp8) + (Random16() & 1)) >> 1);
                cam_y = tmp;
                tmp10 = 15 + sx;
                tmp16 = p->pos_z;
                tmp13 = tmp16;
                tmp5 = tmp13;
                tmp = tmp5;
                tmp17 = p[0].pos_y;
                tmp16 = tmp17;
                tmp17 = tmp - tmp16;
                tmp5 = tmp17;
                tmp13 = (tmp5 - cam_y) >> 16;
                tmp8 = tmp10;
                tmp2 = tmp13;
                tmp11 = tmp2 - (u16)lift;
                sy = tmp11;
                if (255 >= tmp8 && sy >= -32 && sy <= 159) {
                    s32 tmp7;
                    s32 tmp4;
                    s32 tmp14;
                    s32 tmp6;
                    if (p->timer < 60) {
                        p->tile = work->tile + 16;
                        p->fall += 3;
                    } else if (90 > p->timer) {
                        p->tile = work[0].tile + 8;
                        p->fall++;
                    } else {
                        s32 tmp3;
                        tmp3 = work->tile;
                        p->tile = tmp3;
                    }
                    tmp4 = sx - 1;
                    if (tmp7 = tmp14 = 0 != (1 & (gFrameCount >> 3)))
                        p->tile += 4;
                    p->x = tmp4;
                    tmp6 = p->fall >> 2;
                    tmp4 = tmp6;
                    tmp6 = sy - tmp4;
                    p->y = tmp6;
                    p->shape = 0;
                    p->size = 1;
                    Runtime_PushSlotEntry(p, 240);
                } else {
                    p->timer = 0;
                }
            }
            if (8 > spawned && !p->timer) {
                s32 x;
                struct MapPosition *focus = camera->leader;
                s32 z;
                u32 tmp12;
                tmp12 = (u32)0x800000;
                x = focus->x + (Random16() << 8) - tmp12;
                z = focus->z + (Random16() << (s32)8) - 0x800000;
                p->pos_z = z;
                p->pos_x = x;
                p->pos_y = Map_GetTerrainHeightFar((s32)0, x >> 16, z >> 16) << 16;
                p->timer = 120;
                spawned = 1 + spawned;
                p->fall = 0;
            }
            ++i, p += 1;
        } while (32 > i);
    }
}
