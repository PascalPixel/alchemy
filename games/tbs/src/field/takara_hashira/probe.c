#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "hashira.h"
#include "staged_actor.h"
#include "types.h"
#include "dma.h"
#include "io_reg.h"

extern struct StagedActorFootprint StagedActor_FootprintBounds[];

struct MapLayer {
    u32 *cells;
    u8 pad04[44];
};

struct MapState {
    u8 pad000[0x130];
    struct MapLayer layers[1];
};

void OverlayObject_WaitUntilSettledAndReset(u8 *obj)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Engine_TaskWait(1);
        if (*(u32 *)(obj + 12) == *(u32 *)(obj + 20)) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
    *(u32 *)(obj + 12) = *(u32 *)(obj + 20);
}

void SceneActor_WaitHeightBelowLimit(struct FieldActor *obj, s32 limit)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Engine_TaskWait(1);
        if (obj->y.fixed <= obj->terrain_height) {
            break;
        }
        if (obj->y.fixed <= limit) {
            break;
        }
        cnt--;
    }

    obj->velocity_y = 0;
    obj->target_y = 0x80000000;
}

s32 FieldScene_QueryActorFootprint(s32 id, s32 *width, s32 *depth, struct StagedActorProbe *probe, s32 *left, s32 *top)
{
    u8 *map = gMapWork[0];
    struct StagedActor *actor = (struct StagedActor *)Object_GetById(id);
    u32 i;
    s32 a;
    s32 b;

    i = 0;
    if (actor->animation->entries[0]->anim_id != StagedActor_FootprintKinds[i]) {
    miss:
        probe->footprint_index = 7;
        if (++i > 5) {
            goto check;
        }
        if (actor->animation->entries[0]->anim_id != StagedActor_FootprintKinds[i]) {
            goto miss;
        }
    }
    probe->footprint_index = i;
check:
    if ((u32)probe->footprint_index > 6) {
        return 0;
    }
    probe->position_x = actor->x.value;
    probe->position_y = actor->y;
    probe->position_z = actor->z.value;
    a = StagedActor_FootprintBounds[probe->footprint_index].z0;
    if (a < 0) {
        a = -a;
    }
    b = StagedActor_FootprintBounds[probe->footprint_index].z1;
    if (b < 0) {
        b = -b;
    }
    *depth = (a + b) >> 4;
    a = StagedActor_FootprintBounds[probe->footprint_index].x0;
    if (a < 0) {
        a = -a;
    }
    b = StagedActor_FootprintBounds[probe->footprint_index].x1;
    if (b < 0) {
        b = -b;
    }
    *width = (a + b) >> 4;
    probe->position_x += StagedActor_FootprintBounds[probe->footprint_index].x0 << 16;
    {
        s32 step = StagedActor_FootprintBounds[probe->footprint_index].z0 << 16;

        a = probe->position_z;
        a += step;
        a >>= 20;
        probe->position_z = a;
    }
    probe->position_x >>= 20;
    *left = *(s32 *)(map + 0x13c) >> 20;
    *top = *(s32 *)(map + 0x140) >> 20;
    return 1;
}

s32 SceneActor_ApplyPlacementQueryAndTag(u8 *no)
{
    struct FieldActor *obj = Object_GetById(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (FieldScene_QueryActorFootprint(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Engine_MapCopyCellAttributes(2, 2, out20, out16, r2, r4);

    Object_SetMode(obj, 4);
    mask = 2;
    obj->priority_flags = obj->priority_flags | mask;

    if (out20 > out16) {
        Engine_MapCopyCellsTo(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Engine_MapCopyCellsTo(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}

s32 SceneActor_ApplyPlacementQuery(u8 *no)
{
    struct FieldActor *obj = Object_GetById(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (FieldScene_QueryActorFootprint(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Engine_MapCopyCellAttributes(x, z, out20, out16, rec[2], rec[4]);
        StagedActor_FillGridAttributeRectangle(0, rec[2], rec[4], out20, out16, 255);
    }

    Object_SetMode(obj, 1);
    obj->priority_flags &= 0xfd;

    return 1;
}

void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, u32 *dst)
{
    struct MapState *map = gMapWork[0];

    if (map != 0) {
        u32 *cell = map->layers[layer].cells;

        cell += x + (y << 7);
        Dma_Set(cell, dst, 0x84000001, REG_DMA3);
        {
            volatile u32 *dma = REG_DMA3;

            while (dma[2] & 0x80000000)
                ;
        }
    }
}
