/* Near miss, differ 2 of 20 (one reordered row): after the first store the native loads the next object (ldr r0, [r0]) before it shifts the second tick; this draft shifts first. Tried 2026-10-12: a barrier after obj = obj->next (differ 4); a tick temporary read before or after the first store, with and without a barrier (the tick address moves to r2: differ 16 to 18); one inline applied to both objects (differ 34); three permuter minutes. */
#include "types.h"
#include "scene.h"

struct Object_080216b4 {
    struct Object_080216b4 *next;
    u8 filler4[4];
    u8 source;
    u8 filler9[11];
    u8 destination;
};

extern volatile u32 gFrameTick;
extern const u8 Ui_PairBobOffsets[];

void Ui_ApplyTableOffsetToPair(struct Object_080216b4 *obj)
{
    s32 value;

    value = obj->source + Ui_PairBobOffsets[(gFrameTick >> 2) & 7];
    obj->destination = value;
    obj = obj->next;
    value = obj->source + Ui_PairBobOffsets[(gFrameTick >> 2) & 7];
    obj->destination = value;
}
