/* Near miss, differ 4 of 124 (two reordered rows at the start): The Broken Seal's
 * Object_FindNearestFacingTarget with this game's object table (Ram_HeapSlots->scriptObjects).
 * The native starts the table's address, then the empty result (movs r3, #192; lsls r3, r3, #18;
 * movs r2, #0; ldr r5, [r3, #20]; mov sl, r2); this starts the result one row early on both.
 * The other five orders of the three initial statements measured 4 or 8, a barrier after the
 * best distance 12, and four minutes of alchemy permute found nothing better. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ObjectLayerAnimation {
    u8 unk00[9];
    u8 unused09 : 2;
    u8 layer : 2;
    u8 unk0a[27];
    u8 unused25 : 2;
    u8 shadowLayer : 2;
};

void SceneActor_UpdateFacingTowardTarget(void);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object, const void *script);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);

extern const u8 gData_080f34e4[];
extern const u8 gData_080f3428[];
extern const u8 gData_080f350c[];
extern const u8 gData_080f35c8[];
extern const u8 gData_080f373c[];
extern const u8 gData_080f3750[];
extern const u8 gData_080f34e8[];
extern const u8 gData_080f3614[];
extern const u8 gData_080f3764[];
extern const u8 gData_080f3500[];
extern const u8 gData_080f34f4[];

#include "iwram_call.h"

s32 ArcTan2(s32, s32);

struct ObjectAnimationEntry {
    s16 animId;
};

struct ObjectAnimation {
    u8 unk00[0x28];
    struct ObjectAnimationEntry *entries[1];
};

// 向いている先でいちばん近いオブジェクトを探す
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self, s32 id)
{
    struct ObjectRuntime *entry;
    struct ObjectRuntime *found;
    s32 cnt;
    s32 best;
    s32 dy;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 angle;
    s32 turn;

    found = NULL;
    best = 40;
    entry = Ram_HeapSlots->scriptObjects;
    for (cnt = 0; cnt < 64; cnt++, entry++) {
        if (entry->script == NULL)
            continue;
        if (entry == self)
            continue;
        if (entry->animationKind != 1)
            continue;
        dy = entry->y - self->y;
        if (dy >= 0) {
            if (dy > 0x2fffff)
                continue;
        } else {
            if (self->y - entry->y > 0x2fffff)
                continue;
        }
        dx = (entry->x - self->x) / 0x10000;
        dz = (entry->z - self->z) / 0x10000;
        dist = Iwram_Sqrt(dx * dx + dz * dz);
        if (dist >= best)
            continue;
        angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
        if (dist > 23) {
            turn = (s16)(angle - self->angle);
            if (turn < -0x2fff)
                continue;
            if (turn > 0x2fff)
                continue;
        }
        found = entry;
        best = dist;
    }
    if (found == NULL)
        return NULL;
    if (((struct ObjectAnimation *)found->animation)->entries[0]->animId != id)
        return NULL;
    return found;
}
