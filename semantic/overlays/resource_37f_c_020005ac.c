typedef signed int s32;

/*
 * Resource 37f, owner at 0x020005ac (60 bytes of code + a 4-byte literal pool
 * at 0x020005e8).  Sibling of 0x0200056c; see that file for the family notes.
 *
 * `push {r5, lr}` at 0x020005ac, interworking return `pop {r5} / pop {r0} /
 * bx r0` at 0x020005e2 — r0 is the popped return address, so `void`.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all five sites
 * are import veneers.  The cue id 0x300 is built as `192 << 2` rather than
 * loaded from the pool, which is why only one pool word is present.
 */

s32 Func_0808a080();
void Func_080770d0();
void Func_080770c8();

void Func_020005ac(void)
{
    s32 *record;
    s32 heading;

    record = (s32 *)Func_0808a080(10);
    if (record == 0) {
        return;
    }

    heading = record[2] >> 20;

    Func_080770d0(0x300);
    Func_080770d0(0x301);

    if (heading == 115) {
        Func_080770c8(0x300);
    } else if (heading == 113) {
        Func_080770c8(0x301);
    }
}
