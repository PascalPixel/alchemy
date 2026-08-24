#include "types.h"

typedef struct {
    u8 pad_to_angle[6];
    u16 angle;
} ActorState;

#define GetActorState Func_020068be
#define IsActorFacingInward Func_020032a8

extern ActorState *GetActorState(s32 actor_id);

s32 IsActorFacingInward(void)
{
    ActorState *actor = GetActorState(0);

    if ((u32)((actor->angle + 0x5fff) << 16) <= 0x3ffe0000) {
        return 1;
    }
    return 0;
}
