/* Near miss, differ 4 of 147: sets up actors 8, 5 and 6 unless
 * flags 0x863, 0x864 and 0x865 are set, then runs Func_02002c64 unless flag
 * 0x844 is set. Each block's actor is its own block-local variable: one
 * actor for all three makes it a global pseudo, and the +89 address is then
 * a fresh register rather than the actor's (differ 18). Left: in the first
 * two blocks the pooled zero (the angle store's mask, reused by both byte
 * stores) comes after strh where the listing loads it between the halves of
 * 0x4000, the same leftover as Func_02002f88's draft: sched2 ranks the second
 * half 3 above the store chain and the zero 2. Tried 2026-10-15: every order
 * of the four stores (12 to 42), FieldActor for ObjectRuntime, an inline
 * helper for the four stores, an empty asm or a one-pass block after each
 * statement (4 or 22); permute 3 minutes twice found nothing below 4. */
#include "types.h"
#include "field_event.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32 GameFlag_Test(s32 flag);
void Func_02003190();
void Func_02003228();
void Func_02002c64(void);

void Func_02002fd4(void)
{
    s32 flag;

    if (!GameFlag_Test(0x863)) {
        struct ObjectRuntime *actor;

        Call3(Func_02003228, 8, 0x1a00000, 0x2a80000);
        Call6(Func_02003190, 38, 32, 2, 1, 25, 42);
        actor = Object_GetById(8);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
        actor->angle = 0x4000;
        actor->motionFlags = 0;
        actor->y = 0xc0000;
        actor->collisionFlags = 0;
        Engine_ActorSetAnimation(8, 5);
    }
    if (!GameFlag_Test(0x864)) {
        struct ObjectRuntime *actor;

        Call3(Func_02003228, 5, 0x1200000, 0x2e80000);
        Call6(Func_02003190, 38, 33, 2, 1, 17, 46);
        actor = Object_GetById(5);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(5), 0);
        actor->angle = 0x4000;
        actor->motionFlags = 0;
        actor->y = 0xc0000;
        actor->collisionFlags = 0;
        Engine_ActorSetAnimation(5, 18);
    }
    flag = GameFlag_Test(0x865);
    if (flag == 0) {
        struct ObjectRuntime *actor;

        actor = Object_GetById(6);
        Call3(Func_02003228, 6, 0xf00000, 0x2880000);
        Call6(Func_02003190, 38, 34, 2, 1, 14, 40);
        Engine_ActorSetAnimation(6, 19);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(6), 0);
        actor->motionFlags = 0;
        actor->y = 0xc0000;
    }
    Engine_MapCopyCellsTo(72, 42, 75, 40, 1, 1);
    Task_Wait(1);
    if (!GameFlag_Test(0x844))
        Func_02002c64();
}
