#include "types.h"

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

void Func_02001fc2(s32, s32);
void Func_02001fd2(s32, s32, s32);
void Func_020021b8(s32);
void Func_02002236(void);
struct SceneActor *Func_02002246(s32);
void Func_0200226c(void);
void Func_020022ac(s32, s32);
void Func_020022d4(s32);

#define ActorPresentation_RunActorFifteenDialogue Func_0200085c

void ActorPresentation_RunActorFifteenDialogue(void)
{
    struct SceneActor *actor = Func_02002246(15);
    s16 saved_state = actor->temporary_state;

    actor->presentation_flags |= 2;
    Func_02002236();
    Func_020022d4(0x122d);
    Func_020022ac(15, 0);
    Func_02001fd2(15, 0, 2);
    Func_02001fc2(15, 10);
    actor->temporary_state = saved_state;
    Func_020021b8(1);
    Func_0200226c();
    actor->presentation_flags &= 1;
}
