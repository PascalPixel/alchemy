#include "types.h"

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

void Func_02001f52(s32, s32);
void Func_02001f62(s32, s32, s32);
void Func_02002148(s32);
void Func_020021c6(void);
struct SceneActor *Func_020021d6(s32);
void Func_020021fc(void);
void Func_0200223c(s32, s32);
void Func_02002264(s32);

#define ActorPresentation_RunActorFourteenDialogue Func_020007ec

void ActorPresentation_RunActorFourteenDialogue(void)
{
    struct SceneActor *actor = Func_020021d6(14);
    s16 saved_state = actor->temporary_state;

    actor->presentation_flags |= 2;
    Func_020021c6();
    Func_02002264(0x122c);
    Func_0200223c(14, 0);
    Func_02001f62(14, 0, 2);
    Func_02001f52(14, 10);
    actor->temporary_state = saved_state;
    Func_02002148(1);
    Func_020021fc();
    actor->presentation_flags &= 1;
}
