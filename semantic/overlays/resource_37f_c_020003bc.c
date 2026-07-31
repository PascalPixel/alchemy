typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 37f, owner at 0x020003bc (76 bytes of code + a five-word literal
 * pool at 0x0200040c-0x0200041f).  `push {r5, lr}` at 0x020003bc, interworking
 * return `pop {r5} / pop {r0} / bx r0` at 0x02000404 — r0 is the popped return
 * address, so the owner is `void`.
 *
 * All seven call sites were resolved with `tools/overlay_call_targets.ts`; all
 * seven are import veneers.  Func_080770c0(flag) is used only as a predicate.
 *
 * Near-identical sibling of 0x02000200: the same Func_0808a018 /
 * Func_080770c0 / Func_08015040 / Func_0808a020 skeleton with flag ids
 * 0x821 / 0xf02 instead of 0x81a / 0xf01, and with the fall-through arm
 * spelled out separately rather than shared.
 *
 * r5 is loaded with the workspace pointer *before* the Func_08015040 call and
 * used after it; that is why the owner saves r5 at all.
 */

extern u8 *Data_03001ebc;

void Func_0808a018();
s32 Func_080770c0();
void Func_08015040();
void Func_0808a020();

void Func_020003bc(void)
{
    u8 *workspace;

    Func_0808a018();

    if (Func_080770c0(0x821) != 0) {
        Func_08015040(0x1034, 1);
    } else if (Func_080770c0(0xf02) != 0) {
        workspace = Data_03001ebc;
        Func_08015040(0x1031, 1);
        *(u16 *)(workspace + 370) = 1;
    } else {
        Func_08015040(0x1031, 1);
    }

    Func_0808a020();
}
