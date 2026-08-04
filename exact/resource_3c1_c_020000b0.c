#include "types.h"

extern u8 Value_000025dc;
extern s32 Func_0200037a();
extern s32 Func_0200038a(s32, s32);
extern void Func_0200037c(s32);
extern void Func_0200039a(s32);
extern void Func_020003a2(s32);
extern void Func_020003ba(s32, s32);

void Func_020000b0(s32 object)
{
    s32 cue = (s32) &Value_000025dc;
    Func_0200037a(cue);
    Func_0200038a(object, 0);
    if (Func_0200037a(0, 0) == 0) {
        Func_0200037c(10);
        Func_0200039a(cue + 1);
    } else {
        Func_020003a2(cue + 2);
    }
    Func_020003ba(object, 0);
}
