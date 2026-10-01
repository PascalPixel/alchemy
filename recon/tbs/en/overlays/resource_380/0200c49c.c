/* NONMATCHING: resource_380 .text.x0200c49c Soru_UpdateRing, 752 bytes with
 * its pool, and its twin resource_381 .text.x0200b01c (the same code with
 * that overlay's veneers and tables). Both sit just before RING_SETUP.o, so
 * once exact the function belongs at the head of
 * FIELD/SORU_STAR/RING_SETUP.C (two overlays, 1,504 bytes); it needs
 * the overlays' __udivsi3 import stubs.
 * 2026-10-01 (☀️ matcher 1): first draft, written from the listing; 752 of
 * 752 bytes, permuter score 1630 (permute runs reached 1347 with temporary
 * shuffles that are not kept). Every block, call, table access (the drift
 * and swing tables as pointer walks, the direction table by i * 3), the
 * stack slots and the pool line up. Remaining:
 * - the hi registers rotate: the reference keeps the divided y drift in r9,
 *   the entry + 8 walk in sl and the hold counter in fp; here the walk takes
 *   r9, hold sl and the drift fp (global allocation order);
 * - a few schedule differences around the timer load, the scale step and
 *   the reset branch's two 0x1999 loads (the reference loads it twice).
 * The two block temporaries (limit, and the cosine through tmp3) only move
 * allocation ties; without them the score is 4298. */
#include "TYPES.H"

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
        x = entry->x;
        y = entry->y;
        z = entry->z;
        hold = entry->hold;
        timer = entry->timer;
        if (--timer == 0) {
            u32 rx;
            u32 ry;
            u32 rz;
            s32 dx;
            s32 dy;
            s32 dz;

            timer = 3;
            if (hold == 0) {
                s32 limit = Soru_RingOffsetX[i];

                scale += step;
                if (scale >= limit) {
                    step = -Soru_RingOffsetZ[i];
                } else if (scale <= 0x1999) {
                    scale = 0x1999;
                    step = Soru_RingOffsetZ[i];
                    x = obj->x;
                    y = obj->y;
                    z = obj->z;
                    obj->x = hold;
                    obj->y = hold;
                    obj->z = hold;
                    hold = 24;
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
                s32 tmp3 = Engine_MathCos(speed_z * Soru_RingSwing[i][2]) * 2;
                dz = tmp3;
            }
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
