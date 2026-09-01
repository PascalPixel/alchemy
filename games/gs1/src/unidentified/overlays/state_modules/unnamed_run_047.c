/* Contiguous unnamed state-owner run for resource_3c1. */

#include "types.h"

extern u8 Value_000025b8;
extern s32 Func_02000332();
extern s32 Func_02000342(s32, s32);
extern void Func_02000334(s32);
extern void Func_02000352(s32);
extern void Func_0200035a(s32);
extern void Func_02000372(s32, s32);

void Func_02000068(s32 object)
{
    s32 cue = (s32) &Value_000025b8;
    Func_02000332(cue);
    Func_02000342(object, 0);
    if (Func_02000332(0, 0) == 0) {
        Func_02000334(10);
        Func_02000352(cue + 1);
    } else {
        Func_0200035a(cue + 2);
    }
    Func_02000372(object, 0);
}

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

#include "types.h"

extern s32 Func_0200039e(s32, s32, s32, s32, s32, s32);
extern void Func_020003b4(s32);

void Func_020000f8(void)
{
    s32 width = 4;
    s32 height = 9;

    Func_0200039e(25, 9, 1, 1, width, height);
    Func_020003b4(0x201);
}

