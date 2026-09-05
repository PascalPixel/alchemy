#include "types.h"

#define OverlayObject_AdvanceXWhenCounterExpires Func_020000ac

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[94];
    s16 cnt;
};

extern s32 Func_02003bec(void);
extern s32 Func_02003bfa(void);

s32 OverlayObject_AdvanceXWhenCounterExpires(struct Object *obj)
{
    s32 cnt = *(u16 *)&obj->cnt;
    s32 loaded = obj->cnt;
    s32 amount;

    if (loaded == 0) {
        obj->x += (u32)(Func_02003bec() << 15) >> 16;
        amount = (u32)(Func_02003bfa() * 80) >> 16;
        obj->cnt = amount;
        if (amount == 0) {
            goto done;
        }
        cnt = amount;
    }
    obj->cnt = cnt - 1;
done:
    return 1;
}
