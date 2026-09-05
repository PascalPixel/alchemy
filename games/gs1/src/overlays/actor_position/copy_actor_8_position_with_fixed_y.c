#include "types.h"

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Frame *Func_02003c38(s32);

s32 SceneActor_CopyActor8PositionWithFixedY(struct Frame *dst)
{
    struct Frame *src = Func_02003c38(8);

    dst->f08 = src->f08;
    dst->f0c = 0xFFF40000;
    dst->f10 = src->f10;
    return 0;
}
