#include "types.h"

/* Complete actor-16 conditional-counter dialogue scene through its pool. */
struct SceneWork_020006b4 {
    u8 reserved000[472];
    u16 branch_counter;
};

extern struct SceneWork_020006b4 *Data_03001ebc;









extern void Func_020017a0(void);
extern void Func_02001826(s32 dialogue);
extern void Func_02001820(s32 actor, s32 mode, s32 value);
/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */
extern s32 Func_02001840(s32 actor, s32 mode);
extern s32 Func_020017d0(s32 actor, s32 mode);
extern void Func_0200186c(s32 actor, s32 mode);
extern void Func_020017c4(s32 flag);
extern void Func_020017f0(void);
void Func_020006b4(void)
{
    Func_020017a0();
    Func_02001826(0x1cb5);
    Func_02001820(16, 0, 2);
    Func_02001840(16, 0);
    if (Func_020017d0(0, 0) != 0) {
        Data_03001ebc->branch_counter += 1;
    }
    Func_0200186c(16, 0);
    Func_020017c4(0x308);
    Func_020017f0();
}
