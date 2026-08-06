#include "types.h"

/*
 * resource_3ab owner at 0x02001668, 368 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_3ab), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Two scene blocks on the id at 0x02000240+448, both opening by
 * writing 256 to the workspace's +448 s32:
 *
 * - scene 104: run Func_020005f0; unless flag 0xfd1 is set, run
 *   Func_02001840(20); seat record 8 with Func_080091e0(record, 0) if
 *   it exists; raise flag 513.
 * - scene 159: store 10 into the u16 at 0x02000240+578, then two
 *   sub-selector arms guarded by the same flag 0x109 -- selector 4
 *   runs Func_0200131c, selector 3 runs Func_020015bc.  Then, if flag
 *   0x941 is set and 0x94d is clear, install callback 0x02008ac5 with
 *   3200; always install callback 0x02009241 with 3200; and finally
 *   raise ten flags in a fixed order: 0x944, 0x945, 0x946, 0x947,
 *   0x948, 0x943, 0x949, 0x94a, 0x94b, 0x94c.  The order is
 *   transcribed as compiled -- 0x943 sits sixth, not first.
 *
 * Returns 0.  The two scene tests are separate `if`s over the same
 * cell rather than an else-chain, which is how the reference reads it.
 *
 * Complete owner: `push {r5, lr}` at 0x02001668 through `movs r0, #0 /
 * pop {r5} / pop {r1} / bx r1` at 0x02001778-0x0200177e, then the
 * twenty-two-word literal pool 0x02001780-0x020017d7; the next
 * function's `push {r5, r6, r7, lr}` follows at 0x020017d8.  Returns
 * a literal 0.
 *
 * Cross-checked per the 2026-08-01 ruling: tools/m2c_guard.ts clears
 * this row of the call-through-register hazard and reports full call
 * coverage against this draft.
 *
 * Uncertainty: every flag id and scene id is literal; 0x02008ac5 and
 * 0x02009241 are in-image code pointers (image offsets 0xac4 | 1 and
 * 0x1240 | 1) installed as callbacks, not called here; the u16 at
 * 0x02000240+578 is recorded by offset, role open.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;













extern void Func_02001c7c(void);
extern s32 Func_02003052(s32 flagId);
extern void Func_02002edc(s32 arg0);
extern u8 * Func_020030a2(s32 id);
extern void Func_02003044(void *record, s32 arg1);
extern void Func_0200307a(s32 flagId);
extern s32 Func_020030ac(s32 flagId);
extern void Func_02002a10(void);
extern s32 Func_020030ca(s32 flagId);
extern void Func_02002cce(void);
extern s32 Func_020030d8(s32 flagId);
extern s32 Func_020030e2(s32 flagId);
extern void Func_02003058(s32 callback, s32 arg1);
extern void Func_02003062(s32 callback, s32 arg1);
extern void Func_02003110(s32 flagId);
extern void Func_02003116(s32 flagId);
extern void Func_0200311c(s32 flagId);
extern void Func_02003122(s32 flagId);
extern void Func_02003128(s32 flagId);
extern void Func_0200312e(s32 flagId);
extern void Func_02003134(s32 flagId);
extern void Func_0200313a(s32 flagId);
extern void Func_02003140(s32 flagId);
extern void Func_02003146(s32 flagId);
s32 Func_02001668(void)
{
    u8 *record;

    if (Data_02000240[224] == 104) {
        Func_02001c7c();
        if (Func_02003052(0xfd1) == 0) {
            Func_02002edc(20);
        }
        record = Func_020030a2(8);
        if (record != 0) {
            Func_02003044(record, 0);
        }
        Func_0200307a(513);
        *(s32 *)(Data_03001ebc + 448) = 256;
    }
    if (Data_02000240[224] == 159) {
        Data_02000240[289] = 10;
        if (Data_02000240[225] == 4 && Func_020030ac(0x109) == 0) {
            Func_02002a10();
        }
        *(s32 *)(Data_03001ebc + 448) = 256;
        if (Data_02000240[225] == 3 && Func_020030ca(0x109) == 0) {
            Func_02002cce();
        }
        if (Func_020030d8(0x941) != 0 && Func_020030e2(0x94d) == 0) {
            Func_02003058(0x02008ac5, 3200);
        }
        Func_02003062(0x02009241, 3200);
        Func_02003110(0x944);
        Func_02003116(0x945);
        Func_0200311c(0x946);
        Func_02003122(0x947);
        Func_02003128(0x948);
        Func_0200312e(0x943);
        Func_02003134(0x949);
        Func_0200313a(0x94a);
        Func_02003140(0x94b);
        Func_02003146(0x94c);
    }
    return 0;
}
