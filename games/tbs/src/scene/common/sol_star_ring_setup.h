#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"

struct SolRingObject
{
    u8  pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8  pad14[4];
    s32 scaleX;
    s32 scaleY;
    u8  pad20[24];
    s32 baseX;
    s32 baseY;
    s32 baseZ;
    u8  pad44[12];
    u8 *anim;
    u8  pad54;
    u8  state;
};

struct SolRingEntry
{
    struct SolRingObject *obj;
    s32                   x;
    s32                   y;
    s32                   z;
    s32                   speedX;
    s32                   speedY;
    s32                   speedZ;
    s32                   scale;
    s32                   scaleStep;
    u8                    timer;
    u8                    hold;
    u8                    pad26[2];
};

struct SolRingList
{
    struct SolRingEntry entries[10];
    u16                 count;
};

static void Sol_UpdateRing(void)
{
    struct SolRingList  *list = Runtime_AllocateBlock(33, 404);
    struct SolRingEntry *entry = list->entries;
    u32                  i;

    i = 0;
    while (i != list->count)
    {
        struct SolRingObject *obj;
        s32                   speedX;
        s32                   speedY;
        s32                   speedZ;
        s32                   scale;
        s32                   step;
        u8                    timer;
        s32                   x;
        s32                   y;
        s32                   z;
        u8                    hold;

        obj = entry->obj;
        speedX = entry->speedX;
        speedY = entry->speedY;
        speedZ = entry->speedZ;
        scale = entry->scale;
        step = entry->scaleStep;
        hold = entry->hold;
        x = entry->x;
        y = entry->y;
        z = entry->z;
        timer = entry->timer;
        if (--timer == 0)
        {
            u32 rx;
            u32 ry;
            u32 rz;
            s32 dx;
            s32 dy;

            timer = 3;
            if (hold == 0)
            {
                scale += step;
                if (scale >= gSol_RingOffsetX[i])
                {
                    step = -gSol_RingOffsetZ[i];
                }
                else if (scale <= 0x1999)
                {
                    step = gSol_RingOffsetZ[i];
                    scale = 0x1999;
                    hold = 24;
                    x = obj->x;
                    y = obj->y;
                    z = obj->z;
                    obj->x = 0;
                    obj->y = 0;
                    obj->z = 0;
                }
                obj->scaleX = scale;
                obj->scaleY = scale;
            }
            rx = (gSol_RingDrift[i][0] * (u32)Engine_RandomNext()) >> 16;
            ry = (gSol_RingDrift[i][1] * (u32)Engine_RandomNext()) >> 16;
            rz = (gSol_RingDrift[i][2] * (u32)Engine_RandomNext()) >> 16;
            rx = rx != 0 ? (rx << 16) / 1000 : 0;
            ry = ry != 0 ? (ry << 16) / 1000 : 0;
            rz = rz != 0 ? (rz << 16) / 1000 : 0;
            if (gSol_RingDirection[i][0] == 1)
                speedX += rx;
            else if (gSol_RingDirection[i][0] == -1)
                speedX -= rx;
            else
                speedX = 0;
            if (gSol_RingDirection[i][1] == 1)
                speedY += ry;
            else if (gSol_RingDirection[i][1] == -1)
                speedY -= ry;
            else
                speedY = 0;
            if (gSol_RingDirection[i][2] == 1)
                speedZ += rz;
            else if (gSol_RingDirection[i][2] == -1)
                speedZ -= rz;
            else
                speedZ = 0;
            dx = Engine_MathSin(speedX * gSol_RingSwing[i][0]) * 2;
            dy = Engine_MathSin(speedY * gSol_RingSwing[i][1]) * 2;
            {
                s32 dz = Engine_MathCos(speedZ * gSol_RingSwing[i][2]) * 2;

                if (hold != 0)
                {
                    x += dx;
                    y += dy;
                    z += dz;
                    if (--hold == 0)
                    {
                        obj->x = x;
                        obj->baseX = x;
                        if (ry != 0)
                        {
                            obj->y = y;
                            obj->baseY = y;
                        }
                        obj->z = z;
                        obj->baseZ = z;
                    }
                }
                else
                {
                    obj->x += dx;
                    obj->baseX = obj->x;
                    if (ry != 0)
                    {
                        obj->y += dy;
                        obj->baseY = obj->y;
                    }
                    obj->z += dz;
                    obj->baseZ = obj->z;
                }
            }
        }
        entry->speedX = speedX;
        entry->speedY = speedY;
        entry->speedZ = speedZ;
        entry->scale = scale;
        entry->scaleStep = step;
        entry->hold = hold;
        entry->x = x;
        entry->y = y;
        entry->z = z;
        entry->timer = timer;
        i++;
        entry++;
    }
}

void SolStar_SetupElementalRings(s32 first, u32 count)
{
    struct SolRingList   *list;
    struct SolRingEntry  *entry;
    struct SolRingObject *obj;
    u32                   i;
    s32                   none;
    s32                   cleared;

    list = Runtime_AllocateBlock(33, 0x194);
    entry = list->entries;
    Dma_Fill32(0, list, 0x194);
    if (count > 10)
        count = 10;
    none = 0;
    i = none;
    if (count != 0)
    {
        cleared = none;
        do
        {
            obj = (struct SolRingObject *)Object_GetById(first);
            {
                u8 *frame = &obj->anim[38];

                entry->obj = obj;
                *frame = cleared;
            }
            obj->state = cleared;
            Engine_ObjectSetBlendMode(Object_GetById(first), 1);
            entry->scale = gSol_RingOffsetX[i];
            entry->scaleStep = -gSol_RingOffsetZ[i];
            entry->timer = 3;
            i++;
            entry++;
            first++;
        } while (i != count);
    }
    list->count = count;
    FieldScheduler_AddCallback(Sol_UpdateRing, 0xc80);
}
