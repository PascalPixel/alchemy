#include "types.h"

/* Complete actor-13 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_020007d4 {
    u8 reserved00[91];
    u8 accepted;
};

extern void Func_0808a170(s32 dialogue);
extern struct Actor_020007d4 *Func_0808a080(s32 actor);
extern void Func_020004b4(s32 actor);

void Func_020007d4(void)
{
    Func_0808a170(0x1cbf);
    Func_0808a080(13)->accepted = 1;
    Func_020004b4(13);
    Func_0808a080(13)->accepted = 0;
}
