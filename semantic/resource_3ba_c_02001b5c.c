#include "types.h"

/*
 * resource_3ba current-actor scene owner at 0x02001b5c, complete 288-byte
 * span through its six-word pool. This is the independently address-mapped
 * sibling of resource_3bc:288c and resource_3bb:1df4: it retains this
 * overlay's own workspace cells, messages, flag and continuation address.
 */

extern u8 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_02001910(s32 selector);
extern void Func_080770c8(s32 flagId);
extern void Func_080770e8(s32 id, s32 value);
extern s32 Func_0808a070(s32 selector, s32 mode);
extern u8 *Func_0808a080(s32 selector);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 message);
extern void Func_0808a178(s32 selector, s32 mode);
extern void Func_0808a180(s32 selector, s32 mode);
extern void Func_0808a248(s32 request);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);

void Func_02001b5c(s32 actorId)
{
    u8 *workspace = Data_03001ebc;
    s32 selector;
    u8 *record;
    s32 predicate;
    s32 idBase;

    selector = *(s32 *)(Data_02000240 + 500);
    Func_0808a080(actorId);
    Func_0808a080(actorId);
    record = Func_0808a080(selector);

    Func_0808a018();
    Func_0808a170(0x2086);
    Func_0808a178(actorId, 0);

    *(u16 *)(workspace + 0x0cc2) = 0x2089;
    *(u16 *)(workspace + 0x0cc4) = 4;

    predicate = Func_0808a070(selector, 0);
    if (predicate == 0) {
        Func_0808a170(0x2087);
        Func_0808a180(actorId, 0);

        Func_0808a368();
        *(s32 *)(workspace + 448) = 512;
        *(s32 *)(workspace + 456) = 15;
        Func_0808a370();

        selector++;
        idBase = selector << 4;
        Func_080770e8(idBase + 0x370, *(s32 *)(record + 8) >> 20);
        Func_080770e8(idBase + 0x378, *(s32 *)(record + 16) >> 20);

        if (selector > 3) {
            Func_0808a248(10);
            Func_080770c8(282);
        } else {
            *(s32 *)(workspace + 448) = predicate;
            Func_02001910(selector);
            Func_0808a360();
            Func_0808a370();
        }
    } else {
        Func_0808a170(0x2088);
        Func_0808a180(actorId, 0);
    }

    Func_0808a020();
}
