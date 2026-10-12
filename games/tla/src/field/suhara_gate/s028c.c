#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "eventwrk.h"

extern char SceneId_SuharaGate1;
extern char SceneId_SuharaGate2;
extern char SceneId_SuharaGate3;

void Func_02000110(void);
void Func_02000168(void);
void Func_02000264(void);

s32 Func_0200028c(void)
{
    struct EventWork *work;
    s16 scene;

    work = Ram_HeapSlots->event_work;
    work->screen_effect = 256;
    scene = gPartyState.scene;
    if (scene == (s32)&SceneId_SuharaGate1)
        Func_02000110();
    else if (scene == (s32)&SceneId_SuharaGate2)
        Func_02000168();
    else if (scene == (s32)&SceneId_SuharaGate3)
        Func_02000264();
    return 0;
}
