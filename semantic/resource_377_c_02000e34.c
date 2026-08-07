#include "types.h"

/*
 * resource_377 owner at 0x02000e34, 196 bytes.  Complete owner: 'push {r5, lr}'
 * prologue and the interworking return 'pop {r5} ; pop {r0} ; bx r0'.  r0 holds
 * the popped return address, so the owner returns nothing, and nothing reads r0
 * on entry (the first instruction sets it to 0), so it takes no argument.
 *
 * A scene beat on actor 13, guarded by a range test on the scene record and then
 * split by two progress flags.
 *
 * Guard: Func_0808a080(0) is the scene-record accessor (the identity settled in
 * HANDOVER by a third owner using it as Func_0808a080(0)); its u16 at +6 is
 * biased by 0xffffe000 and compared UNSIGNED ('bls') against 0x9000, so the beat
 * runs only while that field is within [0x2000, 0xb000].  Outside the window the
 * owner reports through Func_080b0018(0, 13) and returns without the scene
 * bracket -- the early exit branches straight to the epilogue, past
 * Func_0808a020.
 *
 * Register carry across the guard: 'movs r5,#144 ; lsls r5,r5,#8' builds 0x9000
 * for that comparison and r5 is still live much later, supplying the second
 * argument of Func_0808a1b8.  It is one constant with two uses, not two.
 *
 * Literal pool: owner offsets 0xa8..0xc3, preceded by an alignment 'movs r0,r0'
 * at 0x02000eda.  The control-flow walk from the prologue ends at the epilogue
 * (0x02000ed4), so the tail is entirely pool -- and it is pool that decodes as
 * plausible instructions (0xffffe000 prints as a 'b.n'/NEON pair), which is why
 * the map is taken from the walk and not from a listing.  Words: 0xffffe000
 * (the bias), progress-flag ids 0x87a/0x815/0x300 (0x300 is built inline) and
 * cue ids 0x1c14, 0x1c15, 0x11a9, 0x0f58.
 *
 * All eighteen calls resolved with
 * 'bun tools/lib/overlay_call_targets.ts resource_377 0e34 --json' (18 sites,
 * 12 distinct veneer targets).  Per-target multiset, matched exactly below:
 * Func_0808a170 x4, Func_080770c0 x3, Func_0808a180 x2, one each of
 * Func_0808a080, Func_080b0018, Func_0808a018, Func_0808a138, Func_0808a148,
 * Func_080770c8, Func_0808a190, Func_0808a1b8, Func_0808a020.  In order:
 *   0x0e38 -> 0x192c -> Func_0808a080(0)
 *   0x0e4e -> 0x1aa4 -> Func_080b0018(0, 13)
 *   0x0e54 -> 0x190c -> Func_0808a018()
 *   0x0e5a -> 0x18ec -> Func_080770c0(0x87a)   result tested
 *   0x0e66 -> 0x199c -> Func_0808a138(13, 2)
 *   0x0e70 -> 0x19a4 -> Func_0808a148(13, 0, 10)
 *   0x0e78 -> 0x18ec -> Func_080770c0(0x300)   result tested
 *   0x0e82 -> 0x19c4 -> Func_0808a170(0x1c14)
 *   0x0e8a -> 0x19d4 -> Func_0808a180(13, 0)
 *   0x0e92 -> 0x18f4 -> Func_080770c8(0x300)   the setter for that flag
 *   0x0e98 -> 0x19c4 -> Func_0808a170(0x1c15)
 *   0x0ea0 -> 0x19e4 -> Func_0808a190(13, 0)
 *   0x0eaa -> 0x19ec -> Func_0808a1b8(13, 0x9000, 10)
 *   0x0eb2 -> 0x18ec -> Func_080770c0(0x815)   result tested
 *   0x0ebc -> 0x19c4 -> Func_0808a170(0x11a9)
 *   0x0ec4 -> 0x19c4 -> Func_0808a170(0xf58)
 *   0x0ecc -> 0x19d4 -> Func_0808a180(13, 0)
 *   0x0ed0 -> 0x1914 -> Func_0808a020()
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 */

u8 *Func_02002766();
void Func_020028f4();
void Func_02002762();
s32 Func_02002748();
void Func_02002804();
void Func_02002816();
s32 Func_02002766_b();
void Func_02002848();
void Func_02002860();
void Func_02002788();
void Func_0200285e();
void Func_02002886();
void Func_02002898();
s32 Func_020027a0();
void Func_02002882();
void Func_0200288a();
void Func_020028a2();
void Func_020027e6();

                     

                     

                     

                     

void Func_02000e34(void)
{
    u8 *record;
    u32 limit = 0x9000;

    record = Func_02002766(0);
    if ((u32)(*(u16 *)(record + 6) - 0x2000) > limit) {
        Func_020028f4(0, 13);
        return;
    }

    Func_02002762();

    if (Func_02002748(0x87A) != 0) {
        Func_02002804(13, 2);
        Func_02002816(13, 0, 10);

        if (Func_02002766_b(0x300) == 0) {
            Func_02002848(0x1C14);
            Func_02002860(13, 0);
            Func_02002788(0x300);
        }

        Func_0200285e(0x1C15);
        Func_02002886(13, 0);
        Func_02002898(13, limit, 10);
    } else {
        if (Func_020027a0(0x815) != 0) {
            Func_02002882(0x11A9);
        } else {
            Func_0200288a(0xF58);
        }
        Func_020028a2(13, 0);
    }

    Func_020027e6();
}
