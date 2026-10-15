/* Near miss (12 instructions differ): the native loads the update
 * callback and the pooled zero before the first timer store, and keeps the
 * second actor in r2 with the timer zero in r1; the draft loads both after
 * the store and swaps r1 and r2. */
#include "types.h"

struct YallamActor {
    u8 unk00[98];
    u8 state;
    u8 kind;
    s16 timer;
    s16 count;
    u8 unk68[4];
    void (*update)(struct YallamActor *actor);
};

struct YallamActor *Object_GetById(s32 id);
void Func_0200014c(struct YallamActor *actor);

void Func_020023c8(void)
{
    struct YallamActor *actor = Object_GetById(9);

    actor->timer = 1;
    actor->kind = 0;
    actor->update = Func_0200014c;
    actor = Object_GetById(10);
    actor->timer = 0;
    actor->kind = 0;
    actor->update = Func_0200014c;
}
