#include "types.h"

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

void Func_02002232(s32, s32);
void Func_02002242(s32, s32, s32);
void Func_02002428(s32);
void Func_020024a6(void);
struct SceneActor *Func_020024b6(s32);
void Func_020024dc(void);
void Func_0200251c(s32, s32);
void Func_02002544(s32);

#define ActorPresentation_RunActorFifteenFollowupDialogue Func_02000acc

void ActorPresentation_RunActorFifteenFollowupDialogue(void)
{
    struct SceneActor *actor = Func_020024b6(15);
    s16 saved_state = actor->temporary_state;

    actor->presentation_flags |= 2;
    Func_020024a6();
    Func_02002544(0x133b);
    Func_0200251c(15, 0);
    Func_02002242(15, 0, 2);
    Func_02002232(15, 10);
    actor->temporary_state = saved_state;
    Func_02002428(1);
    Func_020024dc();
    actor->presentation_flags &= 1;
}
