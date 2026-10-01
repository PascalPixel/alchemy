/* The elemental rings of Mt. Aleph, linked into both overlays that show
 * them (380 and its twin 381). Engine_* bind at each overlay's runtime
 * import veneer; the ring tables are each overlay's own. */
#include "DMA.H"

struct SoruRingObject {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 scale_x;
    s32 scale_y;
    u8 pad20[24];
    s32 base_x;
    s32 base_y;
    s32 base_z;
    u8 pad44[12];
    u8 *anim;
    u8 pad54;
    u8 state;
};

struct SoruRingEntry {
    struct SoruRingObject *obj;
    s32 x;
    s32 y;
    s32 z;
    s32 speed_x;
    s32 speed_y;
    s32 speed_z;
    s32 scale;
    s32 scale_step;
    u8 timer;
    u8 hold;
    u8 pad26[2];
};

struct SoruRingList {
    struct SoruRingEntry entries[10];
    u16 count;
};

extern s32 Soru_RingOffsetX[];
extern s32 Soru_RingOffsetZ[];
extern u8 Soru_RingDrift[][3];
extern u8 Soru_RingSwing[][3];
extern s8 Soru_RingDirection[][3];

struct SoruRingList *Runtime_AllocateBlock(s32 slot, s32 size);
u32 Engine_RandomNext(void);
s32 Engine_MathSin(s32 angle);
s32 Engine_MathCos(s32 angle);
struct SoruRingObject *Object_GetById(s32 actor);
void Engine_ObjectSetBlendMode(struct SoruRingObject *obj, s32 mode);
s32 Engine_TaskAddCallback(void *callback, s32 priority);

/* Each frame: every ring breathes between its smallest size and its own
 * limit, and every third frame drifts by a random step along each axis its
 * direction table allows. A ring that has shrunk to its smallest size is
 * hidden at the origin for 24 steps while its saved position keeps
 * drifting, then reappears there. */
void Soru_UpdateRing(void)
{
    struct SoruRingList *list = Runtime_AllocateBlock(33, 404);
    struct SoruRingEntry *entry = list->entries;
    u32 i;

    i = 0;
    while (i != list->count) {
        struct SoruRingObject *obj;
        s32 speed_x;
        s32 speed_y;
        s32 speed_z;
        s32 scale;
        s32 step;
        u8 timer;
        s32 x;
        s32 y;
        s32 z;
        u8 hold;

        obj = entry->obj;
        speed_x = entry->speed_x;
        speed_y = entry->speed_y;
        speed_z = entry->speed_z;
        scale = entry->scale;
        step = entry->scale_step;
        hold = entry->hold;
        x = entry->x;
        y = entry->y;
        z = entry->z;
        timer = entry->timer;
        if (--timer == 0) {
            u32 rx;
            u32 ry;
            u32 rz;
            s32 dx;
            s32 dy;

            timer = 3;
            if (hold == 0) {
                scale += step;
                if (scale >= Soru_RingOffsetX[i]) {
                    step = -Soru_RingOffsetZ[i];
                } else if (scale <= 0x1999) {
                    step = Soru_RingOffsetZ[i];
                    scale = 0x1999;
                    hold = 24;
                    x = obj->x;
                    y = obj->y;
                    z = obj->z;
                    obj->x = 0;
                    obj->y = 0;
                    obj->z = 0;
                }
                obj->scale_x = scale;
                obj->scale_y = scale;
            }
            rx = (Soru_RingDrift[i][0] * Engine_RandomNext()) >> 16;
            ry = (Soru_RingDrift[i][1] * Engine_RandomNext()) >> 16;
            rz = (Soru_RingDrift[i][2] * Engine_RandomNext()) >> 16;
            rx = rx != 0 ? (rx << 16) / 1000 : 0;
            ry = ry != 0 ? (ry << 16) / 1000 : 0;
            rz = rz != 0 ? (rz << 16) / 1000 : 0;
            if (Soru_RingDirection[i][0] == 1)
                speed_x += rx;
            else if (Soru_RingDirection[i][0] == -1)
                speed_x -= rx;
            else
                speed_x = 0;
            if (Soru_RingDirection[i][1] == 1)
                speed_y += ry;
            else if (Soru_RingDirection[i][1] == -1)
                speed_y -= ry;
            else
                speed_y = 0;
            if (Soru_RingDirection[i][2] == 1)
                speed_z += rz;
            else if (Soru_RingDirection[i][2] == -1)
                speed_z -= rz;
            else
                speed_z = 0;
            dx = Engine_MathSin(speed_x * Soru_RingSwing[i][0]) * 2;
            dy = Engine_MathSin(speed_y * Soru_RingSwing[i][1]) * 2;
            {
                s32 dz = Engine_MathCos(speed_z * Soru_RingSwing[i][2]) * 2;

                if (hold != 0) {
                    x += dx;
                    y += dy;
                    z += dz;
                    if (--hold == 0) {
                        obj->x = x;
                        obj->base_x = x;
                        if (ry != 0) {
                            obj->y = y;
                            obj->base_y = y;
                        }
                        obj->z = z;
                        obj->base_z = z;
                    }
                } else {
                    obj->x += dx;
                    obj->base_x = obj->x;
                    if (ry != 0) {
                        obj->y += dy;
                        obj->base_y = obj->y;
                    }
                    obj->z += dz;
                    obj->base_z = obj->z;
                }
            }
        }
        entry->speed_x = speed_x;
        entry->speed_y = speed_y;
        entry->speed_z = speed_z;
        entry->scale = scale;
        entry->scale_step = step;
        entry->hold = hold;
        entry->x = x;
        entry->y = y;
        entry->z = z;
        entry->timer = timer;
        i++;
        entry++;
    }
}

/* Mt. Aleph (Sol Sanctum and the crater): claim heap block 33 for up to ten
 * ring entries, one per actor from first, each blended, placed at its fixed
 * offset, and schedule the ring's update. The same function sits in both
 * overlays. */
void SoruStar_SetupElementalRings(s32 first, u32 count)
{
    struct SoruRingList *list;
    struct SoruRingEntry *entry;
    struct SoruRingObject *obj;
    u32 i;
    s32 none;
    s32 cleared;
    volatile u32 zero;

    list = Runtime_AllocateBlock(33, 0x194);
    zero = 0;
    entry = list->entries;
    Dma_Set((const void *)&zero, list, 0x85000065, (volatile u32 *)0x040000d4);
    if (count > 10)
        count = 10;
    none = 0;
    i = none;
    if (count != 0) {
        cleared = none;
        do {
            obj = Object_GetById(first);
            {
                u8 *frame = &obj->anim[38];

                entry->obj = obj;
                *frame = cleared;
            }
            obj->state = cleared;
            Engine_ObjectSetBlendMode(Object_GetById(first), 1);
            entry->scale = Soru_RingOffsetX[i];
            entry->scale_step = -Soru_RingOffsetZ[i];
            entry->timer = 3;
            i++;
            entry++;
            first++;
        } while (i != count);
    }
    list->count = count;
    Engine_TaskAddCallback((void *)Soru_UpdateRing, 0xc80);
}
