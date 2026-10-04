#include "TYPES.H"
#include "RAM_BUFFER.H"

s32 Func_080cc67c(s32);
s32 Func_080cc7c4(void);

/* Places the scene's entrance view with its first pass; in the scenes
   whose event mode is 2 the second view routine may handle it, and
   otherwise the default pass places it. */
void BattleMap_ApplySceneEntranceView(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    s32 done;

    /* FAKEMATCH: the native issues the event work load before the argument. */
    __asm__("");
    done = Func_080cc67c(1);
    if (*(s8 *)(event + 0x18a) == 2 && done == 0)
        done = Func_080cc7c4();
    if (done == 0)
        Func_080cc67c(0);
}
