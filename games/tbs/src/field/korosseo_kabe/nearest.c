#include "task.h"
#include "text/msg_ids.h"

void KorosseoKabe_MarkSceneProgress(void)
{
    struct EventWork *work = gEventWork;
    s32 v = gGameState.selected_actor;

    if (v != 0 && (work->psynergy_request >> 10) == v
        && Engine_GameFlagIsSet(321) != 0) {
        u16 *p = (u16 *)&work->raised_trigger;
        s32 t = 99;

        *p = t;
    }
}

void KorosseoKabe_SelectNearestActor(void)
{
    struct EventWork *state = gEventWork;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gGameState.selected_actor;
    Obj *p = (Obj *)Object_GetById(n);
    s32 i;
    s32 *q;
    s32 base;

    Engine_EventBegin();
    for (i = 8; i <= 66; i++) {
        Obj *o = (Obj *)Object_GetById(i);

        if (o != 0 && o->f54 == 1 && *o->f50->f28 == 165) {
            s32 dx = (p->f08 - o->f08) / 65536;
            s32 dy = (p->f10 - o->f10) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    Engine_EventSetMessage((s32)MsgKorosseoMatchAboutBeginPleaseTake);
    Engine_EventShowMessage(best, 0);
    q = (s32 *)&state->start_transition;
    *q = 512;
    state->transition_frames = 15;
    Engine_EventWait(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    base = n << 4;
    GameFlag_SetByte(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        GameFlag_SetByte(base + 888, v);
    }
    n++;
    if (n > 3) {
        Engine_EventRequestExit(10);
        Engine_GameFlagSet(282);
    } else {
        Korosseo_SelectSoloCompetitor(n);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        *q = 0;
    }
    Engine_EventEnd();
}
