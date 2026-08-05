#include "types.h"






/* Complete 44-byte actor-19 scene owner through its one-word pool. */
extern void Func_0200135c(void);
extern void Func_0200140a(s32 message);
extern void Func_02001404(s32 actor, s32 target, s32 frames);
extern void Func_0200143c(s32 actor, s32 mode);
extern void Func_02001380(void);
void Func_020001bc(void)
{
    Func_0200135c();
    Func_0200140a(0xf73);
    Func_02001404(0, 19, 6);
    Func_0200143c(19, 0);
    Func_02001380();
}
