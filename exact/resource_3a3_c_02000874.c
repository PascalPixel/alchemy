#include "types.h"

/*
 * resource_3a3 flag-sync owner 0x02000874-0x02000903 (144 bytes).
 *
 * Three progress flags are mirrored into three scene flags, then the scene id
 * held at Data_02000240[224] selects one of two continuations.  Every call
 * leaves through its own import veneer; the sites are kept separate because
 * the byte-level call multiset is part of the completeness proof.
 */

s32 Func_02001702();           /* site 0x878, was Func_080770c0(0x8fd) */
void Func_02001716();          /* site 0x884, was Func_080770c8(0x240) */
s32 Func_02001714();           /* site 0x88a, was Func_080770c0(0x8fe) [1st] */
s32 Func_0200171e();           /* site 0x894, was Func_080770c0(0x907) [1st] */
void Func_02001730();          /* site 0x89e, was Func_080770c8(0x241) */
s32 Func_0200172e();           /* site 0x8a4, was Func_080770c0(0x8fe) [2nd] */
s32 Func_02001738();           /* site 0x8ae, was Func_080770c0(0x907) [2nd] */
void Func_0200174a();          /* site 0x8b8, was Func_080770c8(0x242) */
void Func_020011d4(void);      /* site 0x8ce, was Func_02000904 */
void Func_02001408(void);      /* site 0x8da, was Func_02000b2c */

extern s16 Data_02000240[];
extern u8 Value_0000004b;
extern u8 Value_0000004c;

s32 Func_02000874(void)
{
    s16 scene;

    if (Func_02001702(0x8fd) != 0) {
        Func_02001716(0x240);
    }

    if (Func_02001714(0x8fe) != 0 || Func_0200171e(0x907) != 0) {
        Func_02001730(0x241);
    }

    if (Func_0200172e(0x8fe) != 0 && Func_02001738(0x907) != 0) {
        Func_0200174a(0x242);
    }

    scene = Data_02000240[224];
    if (scene == (s32)&Value_0000004b) {
        Func_020011d4();
    } else if (scene == (s32)&Value_0000004c) {
        Func_02001408();
    }

    return 0;
}
