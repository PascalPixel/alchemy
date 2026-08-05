#include "types.h"

/* resource_3bc owner at 0x020029ac, 164 bytes, independently matched to
 * resource_3bb:1f14. Prologue through the sole return at
 * 0x02002a3c-0x02002a44, then alignment and two pool words through 0x02002a4f.
 * The party-count-selected interaction accounts for all seventeen calls.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including the repeated Func_0808a090 and
 * Func_0808a0d0 calls) -- declared/named as the literal per-site targets,
 * not the shared ultimate-destination symbol. */
u8 *Func_020073ca(s32 selector);              /* Func_0808a080 veneer */
void Func_020073be(void);                     /* Func_0808a018 veneer */
s32 Func_0200739a(void);                      /* Func_08077148 veneer */
void Func_0200747c(s32 message);               /* Func_0808a170 veneer #1 */
s32 Func_0200749c(s32 selector, s32 mode);     /* Func_0808a190 veneer */
void Func_0200740e(s32 actor, s32 x, s32 z);   /* Func_0808a090 veneer #1 */
void Func_0200741c(s32 actor, s32 x, s32 z);   /* Func_0808a090 veneer #2 */
void Func_02007448(s32 selector, s32 x, s32 z); /* Func_0808a0c8 veneer */
void Func_020073fe(s32 frames);                /* Func_0808a010 veneer */
void Func_02007460(s32 actor, s32 x, s32 z);   /* Func_0808a0d0 veneer #1 */
void Func_0200746c(s32 actor, s32 x, s32 z);   /* Func_0808a0d0 veneer #2 */
void Func_02007578(void);                      /* Func_0808a368 veneer */
void Func_02007584(void);                      /* Func_0808a370 veneer */
void Func_0200753a(s32 cue);                    /* Func_0808a248 veneer */
void Func_020074e2(s32 message);                /* Func_0808a170 veneer #2 */
void Func_020074fa(s32 selector, s32 mode);     /* Func_0808a180 veneer */
void Func_02007446(void);                       /* Func_0808a020 veneer */

void Func_020029ac(s32 selector)
{
    u8 *actor = Func_020073ca(selector);
    s32 x = *(s16 *)(actor + 10);
    s32 z = *(s16 *)(actor + 18);
    Func_020073be();
    if (Func_0200739a() <= 1) {
        Func_0200747c(0x20e5);
        if (Func_0200749c(selector, 0) == 0) {
            Func_0200740e(0, 0x10000, 0x8000);
            Func_0200741c(selector, 0x10000, 0x8000);
            Func_02007448(selector, x, z + 64);
            Func_020073fe(15);
            Func_02007460(0, x, z);
            Func_0200746c(0, x, z + 32);
            Func_02007578(); Func_02007584(); Func_0200753a(11);
        }
    } else {
        Func_020074e2(0x20e8); Func_020074fa(selector, 0);
    }
    Func_02007446();
}
