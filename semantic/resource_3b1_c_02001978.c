#include "types.h"

/*
 * resource_3b1 owner at 0x02001978, 144 bytes.  It is the first ordinary
 * owner after the preceding 0x02001894 row: `push {lr}` starts the body,
 * `pop {r0} / bx r0` returns at 0x020019ea-0x020019ec, the halfword at
 * 0x020019ee is alignment, and six literal words at 0x020019f0-0x02001a07
 * carry the workspace base, offsets 0x928-0x92a, and the 0x6f/0x3e values.
 * The next owner's `push {lr}` is 0x02001a08, so the complete measured span
 * is 0x02001978-0x02001a07.
 *
 * The three nested tests preserve the machine's fall-through shape: clear
 * flag 0x8f0 gates the first arm, then flags 0x928, 0x929 and 0x92a each
 * select one setup pair.  The final call is shared by every path.
 */

extern u8 Data_02000240[];









extern void Func_02007e44(void);
extern void Func_02007fc8(void);
extern s32 Func_02007e42(s32 flag);
extern s32 Func_02007e38(s32 flag);
extern void Func_02007fcc(s32 group, s32 mode);
extern s32 Func_02007e54(s32 flag);
extern void Func_02007fe8(s32 group, s32 mode);
extern s32 Func_02007e70(s32 flag);
extern void Func_02008004(s32 group, s32 mode);
extern void Func_02007eb8(void);
void Func_02001978(void)
{
    Func_02007e44();
    Func_02007fc8();
    *(u8 *)(Data_02000240 + 0x22b) = 3;

    if (Func_02007e42(0x8f0) == 0) {
        if (Func_02007e38(0x928) == 0) {
            Func_02007fcc(0x6f, 16);
            Func_02007fcc(0x3e, 0);
        } else if (Func_02007e54(0x929) == 0) {
            Func_02007fe8(0x6f, 18);
            Func_02007fe8(0x3e, 1);
        } else if (Func_02007e70(0x92a) == 0) {
            Func_02008004(0x6f, 20);
            Func_02008004(0x3e, 2);
        }
    }

    Func_02007eb8();
}
