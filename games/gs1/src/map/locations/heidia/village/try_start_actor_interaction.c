#include "types.h"

void Func_02006678(void);
s32 Func_020067d2(s32, s32, s32);
s32 Func_0200669c(s32, s32);
void Func_02006714(s32, s32);
void Func_02006820(s32);
void Func_02006728(s32, s32);
void Func_02006636(s32);
void Func_020066c2(void);

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
    s32 started = 0;
    s32 interaction;

    Func_02006678();
    interaction = Func_020067d2(0, actor_id, interaction_id);
    if (Func_0200669c(interaction_id, 0) != -1) {
        Func_02006714(actor_id, 2);
        started = 1;
    } else {
        Func_02006820(0x7d);
        Func_02006728(actor_id, 5);
    }
    Func_02006636(interaction);
    Func_020066c2();
    return started;
}
