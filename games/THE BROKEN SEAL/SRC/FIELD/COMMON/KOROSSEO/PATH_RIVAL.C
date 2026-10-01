#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"
#include "CALL.H"
#include "VRAM_BLOCK.H"

u16 Math_Atan2(s32 dy, s32 dx);

struct PathRecorder {
    s16 mode;
    s16 mirror;
    s16 actor;
    u16 pos;
    s16 still;
};

struct PathWork {
    u8 unknown_00[224];
    u16 actor;
    u16 finish_facing_a;
    u16 finish_facing_b;
    u16 unknown_e6;
    s32 finish_x;
    s32 finish_z;
    s16 path[0x3840];
};

#define PATH_RECORDER ((struct PathRecorder *)gSceneState)
#define PATH_WORK ((struct PathWork *)gKorosseoWork)

extern const s32 Korosseo_RivalFinishScript[];

/* The rival's recorded course: mode 2 records the actor's pixel position each
 * frame; mode 1 replays it, turning at most 0x1000 a frame, and at the end
 * walks the rival beside the finish and hands it to the finish script. */
void Korosseo_UpdatePathRival(void)
{
    struct PathWork *work = PATH_WORK;
    struct PathRecorder *rec = PATH_RECORDER;
    struct FieldActor *obj;
    s32 x, z, d;
    u16 *facing;

    obj = Object_GetById(rec->actor);
    if (obj == NULL) {
        return;
    }
    if (rec->mode == 1) {
        x = work->path[(s16)rec->pos++];
        z = work->path[(s16)rec->pos++];
        if (x == 0 && z == 0) {
            s32 fz;
            rec->mode = 9;
            Object_SetMode(obj, 1);
            if (work->finish_x < obj->x.fixed) {
                x = work->finish_x + 0xc0000;
            } else {
                x = work->finish_x - 0xc0000;
            }
            if (Engine_GameFlagIsSet(0x211)) {
                fz = work->finish_z + 0x100000;
                facing = &work->finish_facing_b;
            } else {
                fz = work->finish_z - 0x100000;
                facing = &work->finish_facing_a;
            }
            obj->unknown_64 = *facing;
            obj->acceleration = 0x4000;
            obj->speed = 0x10000;
            Engine_ObjectSetPosition(obj, x, 0, fz);
            Engine_GameFlagSet(0x211);
            Engine_ObjectSetScript(obj, Korosseo_RivalFinishScript);
            return;
        }
        x <<= 16;
        z <<= 16;
        if (rec->mirror != 0) {
            x = work->finish_x * 2 - x;
        }
        if (obj->x.fixed != x || obj->z.fixed != z) {
            {
            u16 angle = Math_Atan2(z - obj->z.fixed, x - obj->x.fixed);
            d = (s16)(angle - obj->facing);
            }
            if (d > 0x1000) {
                d = 0x1000;
            }
            if (d < -0x1000) {
                d = -0x1000;
            }
            obj->facing += d;
            obj->x.fixed = x;
            obj->z.fixed = z;
            rec->still = 0;
        } else {
            rec->still++;
        }
        if (rec->still > 2) {
            Object_SetMode(obj, 1);
        } else {
            Object_SetMode(obj, 5);
        }
    } else if (rec->mode == 2) {
        x = obj->x.part.pixel;
        z = obj->z.part.pixel;
        work->path[(s16)rec->pos++] = x;
        work->path[(s16)rec->pos++] = z;
        if ((s16)rec->pos == 0x383e) {
            work->path[(s16)rec->pos++] = 0;
            work->path[(s16)rec->pos] = 0;
            rec->actor = work->actor;
            rec->pos = 0;
            rec->mode = 1;
        }
    }
}

/* The race gauge and its sprites: the work begins with their eighteen
 * records, and the stage scenes keep their own state after them. */
struct GaugeSprite {
    s32 link;
    s32 attr;
    s32 tile;
};

struct GaugeWork {
    struct GaugeSprite sprites[18];
    s16 vram_block;
    /* How far the gauge has slid in from the top, 0 to 2. */
    s16 level;
    s16 hold;
    s16 rival;
    s16 player;
    u16 finish_facing_a;
    u16 finish_facing_b;
    /* Middle pieces on each side of the gauge. */
    s16 segments;
    s32 finish_x;
    s32 finish_z;
};

extern u16 Korosseo_GaugePalette[];
extern u8 Korosseo_GaugeGraphics[];

u8 *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(u8 *block);
void Resource_DecodeType01(u8 *source, u8 *destination);
void VramBlock_LoadCached(s32 block, s32 size, u8 *source);
s32 Resource_ActivateEntry(u32 block);
void Runtime_PushSlotEntry(struct GaugeSprite *sprite, s32 priority);

/* Draws the race gauge each frame: it slides in while flag 0x106 is clear
 * and out once it is set, loading its palette and tiles as it first shows.
 * The gauge is two end caps and a middle joint around segments pieces on
 * each side, the right half mirrored; on eleven frames of every sixteen
 * the player and the rival are marked on it by how far each stands from
 * the finish. */
void Korosseo_DrawGauge(void)
{
    struct GaugeWork *gauge = (struct GaugeWork *)gKorosseoWork;
    s32 *dst = (s32 *)gauge;
    struct GaugeSprite *entry = gauge->sprites;
    s32 tile;
    u32 segments;
    u32 i;
    s32 y;
    s32 x;
    s32 width;
    u8 *buf;
    struct FieldActor *actor;
    u32 shape;

    tile = gVramBlockCache[gauge->vram_block].offset >> 5;
    segments = gauge->segments;
    if (gauge->hold != 0) {
        gauge->level = 2;
    } else if (Engine_GameFlagIsSet(0x106)) {
        if (gauge->level > 0) {
            gauge->level--;
        }
    } else if (gauge->level <= 1) {
        if (++gauge->level == 1) {
            Dma_Set(Korosseo_GaugePalette, (void *)0x050003c0, 0x80000010,
                    (volatile u32 *)0x040000d4);
            buf = (u8 *)Value1((s32 (*)())Runtime_BumpAllocateAlternatePool, 512);
            Resource_DecodeType01(Korosseo_GaugeGraphics, buf);
            Call3((void (*)())VramBlock_LoadCached, gauge->vram_block, 512, (s32)buf);
            Runtime_BumpFree(buf);
        }
    }
    if (gauge->level == 0) {
        Resource_ActivateEntry(gauge->vram_block);
        return;
    }

    y = gauge->level * 6 - 8;
    y &= 255;
    width = segments * 16;
    x = 104 - width;
    shape = 0x8000;
    *dst++ = 0;
    *dst++ = (x << 16) | y | shape;
    *dst++ = tile | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);
    for (i = 0; i < segments; i++) {
        x = 96 - i * 16;
        shape = 0x40000000;
        *dst++ = 0;
        *dst++ = (x << 16) | y | shape;
        *dst++ = (tile + 2) | 0xe400;
        Runtime_PushSlotEntry(entry++, 255);
    }
    x = 112;
    shape = 0x8000;
    *dst++ = 0;
    *dst++ = (x << 16) | y | shape;
    *dst++ = (tile + 6) | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);
    x = 120;
    *dst++ = 0;
    *dst++ = (x << 16) | y | shape | 0x10000000;
    *dst++ = (tile + 6) | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);
    for (i = 0; i < segments; i++) {
        x = 128 + i * 16;
        shape = 0x40000000;
        *dst++ = 0;
        *dst++ = y | (x << 16) | shape | 0x10000000;
        *dst++ = (tile + 2) | 0xe400;
        Runtime_PushSlotEntry(entry++, 255);
    }
    x = width + 128;
    shape = 0x8000;
    *dst++ = 0;
    y |= x << 16;
    y |= shape;
    y |= 0x10000000;
    *dst++ = y;
    *dst++ = tile | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);

    if ((gFrameCount & 15) > 4) {
        shape = 0x40000000;
        actor = Engine_ActorLookup(gauge->player);
        if (actor != 0) {
            x = __divsi3(actor->x.fixed - gauge->finish_x, 0xe0000) + 112;
            y = __divsi3(actor->z.fixed - gauge->finish_z, 0xe0000) + gauge->level * 6 - 4;
            y &= 255;
            *dst++ = 0;
            y |= x << 16;
            y |= shape;
            *dst++ = y;
            *dst++ = (tile + 12) | 0xe400;
            Runtime_PushSlotEntry(entry++, 255);
        }
        actor = Engine_ActorLookup(gauge->rival);
        if (actor != 0) {
            x = __divsi3(actor->x.fixed - gauge->finish_x, 0xe0000) + 112;
            y = __divsi3(actor->z.fixed - gauge->finish_z, 0xe0000) + gauge->level * 6 - 4;
            y &= 255;
            *dst++ = 0;
            y |= x << 16;
            y |= shape;
            *dst++ = y;
            *dst++ = (tile + 8) | 0xe400;
            Runtime_PushSlotEntry(entry++, 255);
        }
    }
}
