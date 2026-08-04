#include "types.h"

/*
 * Resource 373: guarded camera-pan cutscene beat.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/sl saves and the
 * 12-byte local frame at 0x020010d8, through the single `add sp, #12`
 * epilogue at 0x020011b2..0x020011be, followed by its six-word literal pool
 * at 0x020011c0.  The guard's early exit reaches the same epilogue.
 *
 * All nineteen calls are placed.  None is an interworking `call_via rN`
 * site: r3 is never loaded with a helper address.
 *
 * The routine borrows the camera focus slot: it reads the pointer held in
 * the IWRAM word at 0x03001e70, saves the focus that slot currently names,
 * points it at a local copy of the reference position, animates that copy,
 * and restores the previous focus before returning.  The local frame is
 * therefore live for the whole body and must not be split.
 *
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image (shared 0x02000000 namespace, load-time fixups).
 * UNCERTAINTY 2: r0 is not deliberately set before the return; the routine
 * is spelled `void` for the same reason as Func_020011d8.
 */

struct Resource373Focus {
    s32 x;
    s32 y;
    s32 z;
};

struct Resource373Reference {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

/* IWRAM word naming the camera focus slot. */
#define RESOURCE_373_FOCUS_SLOT (*(struct Resource373Focus **volatile *)0x03001e70)

s32 Func_080770c0();
void Func_0808a018();
void Func_0808a090();
void Func_0808a100();
void Func_0808a010();
void Func_0808a170();
void Func_0808a188();
struct Resource373Reference *Func_0808a080();
void Func_08009128();
void Func_08015040();
void Func_0808a0d0();
void Func_0808a020();

void Func_020010d8(void)
{
    struct Resource373Focus focus;
    struct Resource373Focus **slot;
    struct Resource373Focus *savedFocus;
    struct Resource373Reference *reference;
    s32 step;

    if (Func_080770c0(0x00000808) != 0) {
        return;
    }

    slot = RESOURCE_373_FOCUS_SLOT;

    Func_0808a018();
    Func_0808a090(0, 0x10000, 0x8000);   /* 0x80 << 9, 0x80 << 8 */
    Func_0808a100(0, 1);
    Func_0808a010(2);
    Func_0808a170(0x00000f4d);
    Func_0808a188(15, 0, 2);
    Func_0808a188(16, 0, 2);

    reference = Func_0808a080(0);
    focus.x = reference->x;
    focus.y = reference->y;
    focus.z = reference->z;

    savedFocus = *slot;
    *slot = &focus;

    for (step = 0; step != 40; step++) {
        focus.z += 0x20000;          /* 0x80 << 10 */
        Func_0808a010(1);
        Func_08009128();
    }

    Func_0808a010(60);
    Func_08015040(0x00000f4f, 1);
    Func_0808a010(6);

    for (step = 0; step != 40; step++) {
        focus.z += (s32)0xfffe0000;  /* pool word */
        Func_0808a010(1);
        Func_08009128();
    }

    *slot = savedFocus;

    Func_0808a010(60);
    Func_0808a0d0(0, 70, 0x000002e5);
    Func_0808a020();
}
