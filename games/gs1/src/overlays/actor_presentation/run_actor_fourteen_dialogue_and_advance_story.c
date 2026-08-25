#include "types.h"

extern u8 *Data_03001ebc;

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

void Func_020021d4(s32, s32);
void Func_020021e4(s32, s32, s32);
void Func_020023c8(s32);
s32 Func_020023fc(s32);
void Func_02002420(void);
struct SceneActor *Func_02002432(s32);
void Func_0200247c(void);
void Func_020024be();

#define ActorPresentation_RunActorFourteenDialogueAndAdvanceStory Func_02000a4c

void ActorPresentation_RunActorFourteenDialogueAndAdvanceStory(void)
{
    struct SceneActor *actor = Func_02002432(14);
    u16 *flags = &actor->presentation_flags;
    s16 saved_state = actor->temporary_state;
    /* Keeps the original compiler's flag-result register lifetime. */
    s32 flag_carrier;

    *flags = (flag_carrier = *flags | 2);
    Func_02002420();
    Func_020024be(0x1339);
    if (Func_020023fc(2) != 0)
        ++*(u16 *)(Data_03001ebc + 472);
    Func_020024be(14, 0);
    Func_020021e4(14, 0, 2);
    Func_020021d4(14, 10);
    actor->temporary_state = saved_state;
    Func_020023c8(1);
    Func_0200247c();
    *flags &= 1;
}
