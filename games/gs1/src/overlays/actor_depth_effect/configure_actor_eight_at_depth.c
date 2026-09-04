#include "types.h"

#define Overlay387_ConfigureActorEightAtDepth Func_02000d04
struct OverlayActorPosition {
    u8 pad00[8];
    s32 depth_fixed;
};

struct OverlayActorState {
    u8 pad00[35];
    u8 flags;
};

void Func_02001e36();
struct OverlayActorPosition *Func_02001e54();
void Func_02001af6();
struct OverlayActorState *Func_02001e68();
void Func_02001dfe();
void Func_02001e0e();
void Func_02001e66();
void Func_02001e92();
void Overlay387_ConfigureActorEightAtDepth(void)
{
    s32 depth;
    s32 span;
    struct OverlayActorState *state;

    Func_02001e36();
    depth = Func_02001e54(8)->depth_fixed >> 20;
    if (depth == 11) {
        Func_02001af6(8);
        state = Func_02001e68(8);
        state->flags |= 2;
        span = 12;
        Func_02001dfe(39, 12, 3, 1, 8, span);
        Func_02001e0e(43, 11, 3, 1, span, depth);
        Func_02001e66(2144);
    }
    Func_02001e92();
}
