/* Draft of overlay 68a's Func_02000110, differ 2 of 273: the garden statue's rise. Written from
 * the listing (the ☀️ twin's shape was 0.79 but its code is another scene). The scene layer's
 * inlines (MapCopy, Flag_IsSet/Flag_Set, the placement call) are needed: called directly the stack
 * arguments, the 0x96c flag and the placement's arguments come out in another order (differ 32).
 * In the loop the barrier after the angle store keeps the 0xffff limit from starting before it
 * (differ 11 without), and the memory barrier keeps the native's reload of scaleX at the join,
 * which GCSE otherwise takes from the branch (differ 20 without). Left: the native loads scaleX
 * (ldr r3, [r5, #24]) between movs r2, #255 and lsls r2, r2, #8; here the load and the shift tie
 * at priority 3 in sched2 and the shift wins on its two dependents. Tried: the limit as < 0x10000
 * or 0xffff >= scale, scaleX read into a local, an empty asm with scale as operand (its cost is 0,
 * the load's priority drops to 2), braces, a one-pass block round the clamp (11), the loop as a
 * do-while (2) or a goto loop (21); permute 4 minutes found nothing. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 a, s32 b);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void Battle_WaitMode0(s32 frames);
void WaitFrames(s32 frames);
void Func_02000048(void);
void Func_0200195c(s32 id, s32 x, s32 z);
void Func_02001a24(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY, s32 layer);

static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY, s32 layer)
{
    Func_02001a24(srcX, srcY, width, height, destX, destY, layer);
}

static inline void PlaceActor(s32 id, s32 x, s32 z)
{
    Func_0200195c(id, x, z);
}

static inline void Hop(s32 dx, s32 dz)
{
    ObjectMotion_Launch(12, 5, 0);
    ObjectMotion_CommitPositionAndActivate(12, dx, dz);
    Battle_WaitMode0(3);
}

void Func_02000110(void)
{
    struct ObjectRuntime *object;
    s32 i;

    MapCopy(7, 13, 1, 1, 5, 13, 0);
    if (Flag_IsSet(0x96c))
        return;
    object = Object_GetById(12);
    Flag_Set(0x96c);
    Flag_Set(0x96e);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(12), 0);
    PlaceActor(12, 176 << 15, 216 << 16);
    ObjectMotion_Launch(12, 9, 0);
    object->scaleY = 0x8000;
    object->scaleX = 0x8000;
    for (i = 29; i >= 0; i--) {
        object->angle += 0x1000;
        /* FAKEMATCH: the barrier keeps the 0xffff limit from starting before the angle store (differ 11 without). */
        asm("");
        if (object->scaleX <= 0xffff)
            object->scaleX += 0xa00;
        /* FAKEMATCH: the memory barrier keeps the native reload of scaleX at the join (differ 20 without). */
        asm("" ::: "memory");
        object->scaleY = object->scaleX;
        WaitFrames(1);
    }
    object->angle = 0x3000;
    object->scaleY = 0x10000;
    object->scaleX = 0x10000;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(12), 1);
    ObjectMotion_SetActionVariant(12, 1);
    Battle_WaitMode0(8);
    ObjectMotion_Launch(12, 9, 0);
    ObjectMotion_SetPositionAndReset(12, 120, 200);
    Object_GetById(12)->priorityFlags |= 1;
    ObjectMotion_SetPositionAndReset(12, 136, 200);
    Battle_WaitMode0(3);
    Object_SetModeById(gPartyState.currentOwner, 1);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xe000, 0);
    Hop(0, -16);
    Hop(16, 0);
    Hop(16, 0);
    Hop(0, -16);
    Hop(16, 0);
    Hop(0, -16);
    Hop(16, 0);
    Hop(16, 0);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0, 0);
    Hop(0, 16);
    Hop(16, 0);
    Hop(16, 0);
    Hop(16, 0);
    Hop(16, 0);
    Func_0200195c(12, 0, 0);
    Object_GetById(13)->update = Func_02000048;
    Object_GetById(13)->action = 0;
}
