#include "types.h"






/* Complete 44-byte actor-15 scene owner through its one-word pool. */
extern void Func_02001330(void);
extern void Func_020013de(s32 message);
extern void Func_020013d8(s32 actor, s32 target, s32 frames);
extern void Func_02001410(s32 actor, s32 mode);
extern void Func_02001354(void);
void Func_02000190(void)
{
    Func_02001330();
    Func_020013de(0xf6d);
    Func_020013d8(0, 15, 6);
    Func_02001410(15, 0);
    Func_02001354();
}
