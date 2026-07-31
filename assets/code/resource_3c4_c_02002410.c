/*
 * resource_3c4 @ 0x02002410 (112 bytes).
 *
 * Sibling of 0x020023a0 for slots 10 and 11 with its own service set.
 * Same shape: one six-argument placement, then two grid-cell pins built from
 * each slot's +8 and +16 words shifted right by 20 (signed).
 *
 * Exact-lane transcription: each `bl` site keeps its own raw overlay_show
 * target name (HANDOVER section 2), even though overlay_call_targets.ts
 * resolves all eight sites to the same three real veneers
 * (Func_080091c0 x3, Func_0808a080 x4, Func_02000f10 x1) -- the raw address
 * IS the assembler's encoding, verbatim.
 *
 * `add sp,#8 ; pop {r5} ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

typedef struct Slot_02002410 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02002410;

void Func_020054c2();
void Func_0200333e();
Slot_02002410 *Func_02005540();
Slot_02002410 *Func_02005548();
void Func_020054ee();
Slot_02002410 *Func_02005564();
Slot_02002410 *Func_0200556c();
void Func_02005512();

void Func_02002410(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Func_020054c2(93, 30, 6, 5, k5, k6);
    }
    Func_0200333e(11, 10);

    {
        s32 col20 = Func_02005540(10)->column >> 20;
        row = Func_02005548(10)->row >> 20;
        Func_020054ee(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Func_02005564(11)->column >> 20;
        row = Func_0200556c(11)->row >> 20;
        Func_02005512(2, 36, 1, 1, col20, row);
    }
}
