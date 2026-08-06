#include "types.h"

/*
 * resource_3ba current-actor scene owner at 0x02001b5c, complete 288-byte
 * span through its six-word pool. This is the independently address-mapped
 * sibling of resource_3bc:288c and resource_3bb:1df4: it retains this
 * overlay's own workspace cells, messages, flag and continuation address.
 */

extern u8 Data_02000240[];
extern u8 *Data_03001ebc;
















extern u8 * Func_0200582e(s32 selector);
extern u8 * Func_02005834(s32 selector);
extern u8 * Func_02005844(s32 selector);
extern void Func_02005832(void);
extern void Func_020058cc(s32 message);
extern void Func_020058dc(s32 selector, s32 mode);
extern s32 Func_0200586e(s32 selector, s32 mode);
extern void Func_020058fc(s32 message);
extern void Func_02005916(s32 selector, s32 mode);
extern void Func_020059a6(void);
extern void Func_020059b2(void);
extern void Func_02005874(s32 id, s32 value);
extern void Func_02005886(s32 id, s32 value);
extern void Func_020059a8(s32 request);
extern void Func_02005888(s32 flagId);
extern void Func_02003540(s32 selector);
extern void Func_020059e4(void);
extern void Func_020059f8(void);
extern void Func_02005976(s32 message);
extern void Func_0200598e(s32 selector, s32 mode);
extern void Func_020058fa(void);
void Func_02001b5c(s32 actorId)
{
    u8 *workspace = Data_03001ebc;
    s32 selector;
    u8 *record;
    s32 predicate;
    s32 idBase;

    selector = *(s32 *)(Data_02000240 + 500);
    Func_0200582e(actorId);
    Func_02005834(actorId);
    record = Func_02005844(selector);

    Func_02005832();
    Func_020058cc(0x2086);
    Func_020058dc(actorId, 0);

    *(u16 *)(workspace + 0x0cc2) = 0x2089;
    *(u16 *)(workspace + 0x0cc4) = 4;

    predicate = Func_0200586e(selector, 0);
    if (predicate == 0) {
        Func_020058fc(0x2087);
        Func_02005916(actorId, 0);

        Func_020059a6();
        *(s32 *)(workspace + 448) = 512;
        *(s32 *)(workspace + 456) = 15;
        Func_020059b2();

        selector++;
        idBase = selector << 4;
        Func_02005874(idBase + 0x370, *(s32 *)(record + 8) >> 20);
        Func_02005886(idBase + 0x378, *(s32 *)(record + 16) >> 20);

        if (selector > 3) {
            Func_020059a8(10);
            Func_02005888(282);
        } else {
            *(s32 *)(workspace + 448) = predicate;
            Func_02003540(selector);
            Func_020059e4();
            Func_020059f8();
        }
    } else {
        Func_02005976(0x2088);
        Func_0200598e(actorId, 0);
    }

    Func_020058fa();
}
