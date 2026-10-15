#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"

struct LightOam {
    u32 link;
    u32 attr01;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u16 unk0a;
};

extern s16 Data_0200275c;
extern s16 Data_0200275e;
extern struct FieldActor *Data_02002760[];
extern struct LightOam Data_020027a0[];
extern s16 Data_02002860;
extern s16 Data_02002862;

s32 Func_02001ee0(struct FieldActor *actor);
void Func_02001d80(struct LightOam *oam, s32 depth);

#define MAP_WORD(map, offset) (*(s32 *)((u8 *)(map) + (offset)))

void Func_02000e58(void)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;
    s32 *camera = (s32 *)((u8 *)map + 228);
    s32 camera_x = camera[0] & 0xffff0000;
    s32 camera_z = camera[1] & 0xffff0000;
    s32 tile = ResourceTableEntries[Data_0200275c].offset >> 5;
    struct LightOam *oam = Data_020027a0;
    s32 originY = map->origin[1];
    s32 i;

    for (i = 0; i < Data_0200275e; i++) {
        struct FieldActor *actor = Data_02002760[i];
        s32 x, y, z, depth, frame, sx, sy, ox, oy;
        u32 *word;

        if (actor == 0 || actor->x == 0)
            continue;
        x = actor->x - camera_x;
        y = actor->y - originY + 0x80000;
        z = actor->z - camera_z - originY;
        sy = z - y;
        depth = ((y + z) >> 16) + 58;
        sx = x >> 16;
        sy >>= 16;
        if (Data_02002860 == 0) {
            frame = (s16)actor->timer;
            ox = sx - 8;
            oy = sy - 16;
            if (ox <= -16 || ox >= 320 || oy <= -16 || oy >= 240)
                continue;
            word = (u32 *)oam;
            *word++ = 0;
            *word++ = (oy & 0xff) | ((ox & 0x1ff) << 16) | 0x40002000;
        } else {
            frame = (s16)actor->timer;
            ox = sx - 8;
            oy = sy - 64;
            if (ox <= -32 || ox >= 320 || oy <= -64 || oy >= 176)
                continue;
            word = (u32 *)oam;
            *word++ = 0;
            *word++ = (oy & 0xff) | ((ox & 0x1ff) << 16) | 0xc000a000;
        }
        *word = (tile + frame * 8) | 0x800;
        if (Data_02002862 == -1)
            oam->priority = Func_02001ee0(actor);
        else
            oam->priority = Data_02002862;
        Func_02001d80(oam, depth);
        oam++;
    }
}
