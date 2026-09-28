#include "TASK.H"

void KorosseoKabe_MarkSceneProgress(void)
{
    u8 *state = (u8 *)gEventWork;
    s32 v = gGameState.selected_actor;

    if (v != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == v
        && GameFlag_IsSet(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 t = 99;

        *p = t;
    }
}

void KorosseoKabe_SelectNearestActor(void)
{
    u8 *state = (u8 *)gEventWork;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gGameState.selected_actor;
    Obj *p = (Obj *)Engine_ActorGet(n);
    s32 i;
    s32 *q;
    s32 base;

    Event_Begin();
    for (i = 8; i <= 66; i++) {
        Obj *o = (Obj *)Engine_ActorGet(i);

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
    Event_SetMessage(MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE);
    Event_ShowMessage(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Event_Wait(20);
    Event_CloseScreen();
    Event_WaitForScreen();
    base = n << 4;
    GameFlag_SetByte(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        GameFlag_SetByte(base + 888, v);
    }
    n++;
    if (n > 3) {
        Event_RequestExit(10);
        GameFlag_Set(282);
    } else {
        Korosseo_SelectSoloCompetitor(n);
        Event_OpenScreen();
        Event_WaitForScreen();
        *q = 0;
    }
    Event_End();
}
