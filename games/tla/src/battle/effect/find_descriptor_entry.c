#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

struct MenuFlagWork {
    u8 unk000[0x506];
    u8 busy;
};

struct FxGroup {
    u8 unk00[32];
    struct ObjectRuntime *objects[8];
    u16 values[8];
    s32 count;
};

struct DescriptorEntry {
    s16 id;
    u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleFx_BeginTiledCanvas(s32 mode);
void Func_08143bb8(void);
void Func_0814ea58(void *effect, s32 mode);
void UiWindow_CreateWithLayoutBounds(s32 flags);
void WaitFrames(s32 frames);
s32 Func_08020310(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Func_08108148(void);
s32 Func_0810b378(s32 value);
void Func_0810824c(void);
s32 ArcTan2(s32 y, s32 x);

/* The Spanish and Italian far-call tables reach this function under another
   slot's name; those two editions keep it on their scaffold. */
#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)
// Picks the object nearest object id, under 32 pixels away and 48 pixels in
// height. Distances to ids above 63 count double and to objects with
// collision flag 4 ten thirteenths; beyond 11 pixels the object must also lie
// within 0x2fff of id's facing. Returns -1 when none qualifies.
s32 Func_080cd91c(s32 id)
{
    struct ObjectRuntime *src;
    struct ObjectRuntime *obj;
    s32 best;
    s32 best_dist;
    s32 i;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 xx;
    s32 yy;
    s32 zz;
    s32 sq;
    s32 dist;
    s32 angle;

    best = -1;
    best_dist = 32;
    src = ObjectTable_Get(id);
    if (src != 0) {
        for (i = 0; i <= 80; i++) {
            if (i == id)
                continue;

            obj = ObjectTable_Get(i);
            if (obj == 0 || (obj->collisionFlags & 8) != 0)
                continue;

            if (obj->y - src->y >= 0) {
                if (obj->y - src->y >= 48 << 16)
                    continue;
            } else if (src->y - obj->y >= 48 << 16) {
                continue;
            }

            dx = (obj->x - src->x) / 0x10000;
            dy = (obj->y - src->y) / 0x10000;
            dz = (obj->z - src->z) / 0x10000;

            xx = dx * dx;
            yy = dy * dy;
            zz = dz * dz;
            sq = xx + yy + zz;
            dist = Iwram_Sqrt(sq);
            if (i > 63)
                dist <<= 1;
            if ((obj->collisionFlags & 4) != 0)
                dist = dist * 10 / 13;
            if (dist >= best_dist)
                continue;

            angle = (u16)ArcTan2(obj->z - src->z, obj->x - src->x);
            if (dist > 11) {
                s32 diff = (s16)(angle - src->angle);

                if (diff < -0x2fff)
                    continue;
                if (diff > 0x2fff)
                    continue;
            }

            best = i;
            best_dist = dist;
        }
    }
    return best;
}
#endif

struct DescriptorEntry *Func_080cda58(s32 id)
{
    struct DescriptorEntry *entry = gData_080eff28;

    while (entry->id != -1 && id != entry->id)
        entry++;
    return entry;
}
