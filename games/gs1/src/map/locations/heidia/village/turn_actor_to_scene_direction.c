#include "types.h"

extern s32 Data_0200dfa4;

void Func_020072be(s32, s32, s32);
void Func_020072c8(s32, s32, s32);
void Func_0200386a(s32);
void Func_020038ca(s32);
void Func_0200391e(s32);
void Func_02003996(s32);
void Func_020038e2(s32);

void TurnActorToSceneDirection(s32 actor_id)
{
    Func_020072be(actor_id, 0, 0);
    Func_020072c8(0, actor_id, 0);
    switch (Data_0200dfa4 & 3) {
    case 0:
        Func_0200386a(actor_id);
        break;
    case 1:
        Func_020038ca(actor_id);
        break;
    case 2:
        Func_0200391e(actor_id);
        break;
    case 3:
        Func_02003996(actor_id);
        break;
    default:
        Func_020038e2(actor_id);
        break;
    }
}
