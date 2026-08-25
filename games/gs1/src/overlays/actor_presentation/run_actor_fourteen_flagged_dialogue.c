#include "types.h"

extern u8 *Data_03001ebc;

struct SceneActor {
    u8 reserved_00[100];
    u16 presentation_flags;
};

void Func_0200168c(s32);
s32 Func_0200269e(s32);
s32 Func_020026c2(s32);
void Func_020026c8(void);
struct SceneActor *Func_020026e2(s32);
void Func_02002718(void);
struct SceneActor *Func_0200273e(s32);
void Func_02002770(s32);
void Func_02002784(s32);

#define ActorPresentation_RunActorFourteenFlaggedDialogue Func_02000cfc

void ActorPresentation_RunActorFourteenFlaggedDialogue(void)
{
    Func_020026e2(14)->presentation_flags |= 2;
    Func_020026c8();
    if (Func_0200269e(0x855) == 0) {
        Func_02002770(0x123c);
    } else {
        Func_02002784(0x1349);
        if (Func_020026c2(2) != 0)
            ++*(u16 *)(Data_03001ebc + 472);
    }
    Func_0200168c(14);
    Func_02002718();
    Func_0200273e(14)->presentation_flags &= 1;
}
