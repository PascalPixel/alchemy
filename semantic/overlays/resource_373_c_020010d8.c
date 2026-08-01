typedef unsigned char u8;
typedef signed int s32;

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

s32 Func_02007082();
void Func_020070b0();
void Func_020070ee();
void Func_0200714e();
void Func_020070c4();
void Func_0200719a();
void Func_020071bc();
void Func_020071c6();
struct Resource373Reference *Func_02007114();
void Func_02007112();
void Func_02007076();
void Func_02007120();
void Func_020070f0();
void Func_0200712e();
void Func_02007142();
void Func_020070a6();
void Func_02007156();
void Func_020071d0();
void Func_02007174();

void Func_020010d8(void)
{
    struct Resource373Focus focus;
    struct Resource373Focus **slot;
    struct Resource373Focus *savedFocus;
    struct Resource373Reference *reference;
    s32 step;

    if (Func_02007082(0x00000808) != 0) {
        return;
    }

    slot = RESOURCE_373_FOCUS_SLOT;

    Func_020070b0();
    Func_020070ee(0, 0x10000, 0x8000);   /* 0x80 << 9, 0x80 << 8 */
    Func_0200714e(0, 1);
    Func_020070c4(2);
    Func_0200719a(0x00000f4d);
    Func_020071bc(15, 0, 2);
    Func_020071c6(16, 0, 2);

    reference = Func_02007114(0);
    focus.x = reference->x;
    focus.y = reference->y;
    focus.z = reference->z;

    savedFocus = *slot;
    *slot = &focus;

    for (step = 0; step != 40; step++) {
        focus.z += 0x20000;          /* 0x80 << 10 */
        Func_02007112(1);
        Func_02007076();
    }

    Func_02007120(60);
    Func_020070f0(0x00000f4f, 1);
    Func_0200712e(6);

    for (step = 0; step != 40; step++) {
        focus.z += (s32)0xfffe0000;  /* pool word */
        Func_02007142(1);
        Func_020070a6();
    }

    *slot = savedFocus;

    Func_02007156(60);
    Func_020071d0(0, 70, 0x000002e5);
    Func_02007174();
}
