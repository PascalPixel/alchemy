#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers instead of sharing them in saved registers. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Opens the lane beside whichever villager stands in it: the cells copied depend on the villager's column, and the matching flag is set. */
void HaidiaMura_Func02005950(void)
{
    struct FieldActor *actor;
    s32 column;

    Call6((void (*)())Engine_MapCopyCellAttributes, 17, 0, 3, 1, 22, 36);
    if (Engine_GameFlagIsSet(0x87a))
        actor = Engine_ActorGet(21);
    else
        actor = Engine_ActorGet(20);
    if (actor == NULL)
        return;
    Call1((void (*)())Engine_GameFlagClear, 0x314);
    Call1((void (*)())Engine_GameFlagClear, 0x315);
    Call1((void (*)())Engine_GameFlagClear, 0x316);
    column = actor->x.fixed >> 20;
    if (column == 22) {
        Call6((void (*)())Engine_MapCopyCellAttributes, 17, 1, 1, 1, column, 36);
        Call1((void (*)())Engine_GameFlagSet, 0x314);
    } else if (column == 23) {
        Call6((void (*)())Engine_MapCopyCellAttributes, 17, 1, 1, 1, column, 36);
        Call1((void (*)())Engine_GameFlagSet, 0x315);
    } else {
        Call6((void (*)())Engine_MapCopyCellAttributes, 17, 1, 1, 1, 24, 36);
        Call1((void (*)())Engine_GameFlagSet, 0x316);
    }
}
