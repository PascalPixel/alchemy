#include "types.h"

/*
 * resource_3c4 owner at 0x02001550, 420 bytes: code 0x02001550-0x020016e1, an
 * alignment halfword at 0x020016e2 and the four pool words 0x00009999,
 * 0xffff0000, 0x0000e666 and 0x00000971 at 0x020016e4-0x020016f3.
 *
 * A long cutscene beat, gated on slot 18 standing on whole-tile x 46.  It
 * spawns a sixteen-effect rain from a scattered band of positions while slot
 * 18 sinks, then plays out a fixed sequence of placements, camera moves and
 * two identical shake pairs, and finally publishes selector 0x971.
 *
 * Targets resolved with `tools/lib/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`): 31 sites, 17 distinct
 * callees, all 31 accounted for below.  The disassembler's annotations are
 * uniformly wrong here - the four Func_0808a080 sites, the four Func_0808a010
 * sites and the four Func_080091f0 sites each carry bit-identical `bl`
 * halfwords and each print four different targets.
 *
 * Resolved callees: veneers 0x020030f4 (-> Func_0808a018), 0x0200310c
 * (-> Func_0808a080, x4), 0x020030ec (-> Func_0808a010, x4), 0x020031ac
 * (-> Func_080f9010, x3), 0x02003024 (-> Func_080000c0), 0x02003034
 * (-> Func_080000f8, x2), 0x0200309c (-> Func_080091c0, x2), 0x02003144
 * (-> Func_0808a100), 0x0200306c (-> Func_080090d0), 0x0200313c
 * (-> Func_0808a0f0), 0x0200308c (-> Func_08009180, x2), 0x020030bc
 * (-> Func_080091f0, x4), 0x020030c4 (-> Func_080091f8), 0x020030dc
 * (-> Func_080770c8) and 0x020030fc (-> Func_0808a020); plus this overlay's
 * prologue at 0x02000ae8 and its routine at 0x020009f4.
 *
 * 0x020009f4 is reported `unknown` by the tool only because it is not an
 * inventory row of its own - it sits inside a neighbouring row - but it is a
 * real `push {r5, r6, lr}` prologue that permutes its four arguments and
 * returns a handle, and the handle is what Func_080090d0 is given below.  It
 * is also one of the owners that uses r4 as call-clobbered scratch without
 * saving it, the idiom HANDOVER records for resource_371/resource_372.
 *
 * Previously skipped because 0x02001f74 (the 0x020009f4 site) and 0x020020de
 * (the 0x02000ae8 site) both looked like in-image addresses that are not
 * function starts.  Both were decoding artefacts.
 *
 * flags = 0x00090000 for the rain selects exactly the two optional fields the
 * options record initialises: 0x00010000 -> options+4 (7) and 0x00080000 ->
 * options+8/options+12 (both 0x9999).  The remaining 28 bytes of the 40-byte
 * record are never written and never read.
 *
 * The epilogue is `add sp,#56 / pop {r3} / mov r8,r3 / pop {r5, r6, r7} /
 * pop {r0} / bx r0`, so the owner is void.  The gated-out path branches to
 * the same physical Func_0808a020 close site used by the full sequence.
 */

typedef struct Slot_02001550 {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_02001550;

void Func_0808a018();      /* scripted-action bracket: open */
Slot_02001550 *Func_0808a080();
void Func_0808a010();
void Func_080f9010();
void Func_080000c0();      /* ROM dispatch[0] -> Func_080030f8 */
u32 Func_080000f8();       /* ROM dispatch[7] -> Func_08004458 */
void Func_080091c0();      /* four register arguments plus two stack words */
void Func_0808a100();
void Func_080090d0();
void Func_0808a0f0();
void Func_08009180();      /* four register arguments plus two stack words */
void Func_080091f0();
void Func_080091f8();
void Func_080770c8();
void Func_0808a020();      /* scripted-action bracket: close */

/* This overlay's routine at 0x020009f4; returns a handle. */
s32 Func_020009f4();

/* This overlay's effect spawner: four register arguments plus four stack
 * words - (x, y, z, vx, vy, vz, flags, options). */
void Func_02000ae8();

void Func_02001550(void)
{
    u8 options[40];        /* sp+16 */
    s32 handle;
    u32 step;

    /* No argument register is set. */
    Func_0808a018();

    if ((Func_0808a080(18)->x >> 20) != 46) {
        goto close_sequence;
    }

    Func_0808a010(30);

    /* 186 << 18 and 184 << 16. */
    handle = Func_020009f4(0x02e80000, 0, 0x00b80000, 253);

    *(s32 *)&options[12] = 0x9999;
    *(s32 *)&options[4] = 7;
    *(s32 *)&options[8] = 0x9999;

    ((u8 *)Func_0808a080(18))[85] = 0;

    Func_080f9010(185);

    for (step = 0; step <= 15; step++) {
        Slot_02001550 *slot;
        s32 x;
        s32 z;

        Func_080000c0(3);

        /* 0xffff0000 is -0x00010000: the slot sinks by one sixteenth of a
         * unit per tick. */
        slot = Func_0808a080(18);
        slot->y = slot->y + (s32)0xffff0000;

        /* Both reductions use `lsrs`, so they are unsigned.  184 << 18 and
         * 128 << 16 are the band's origins. */
        x = (s32)(((Func_080000f8() << 4) >> 16) << 16) + 0x02e00000;
        z = (s32)(((Func_080000f8() * 18u) >> 16) << 16) + 0x00800000;

        Func_02000ae8(x, 0, z, 0, 0, 0, 0x00090000, options);
    }

    Func_080091c0(51, 8, 1, 1, 49, 8);
    Func_0808a010(30);

    ((u8 *)Func_0808a080(18))[35] |= 2;

    Func_0808a100(18, 3);
    Func_080090d0(handle);

    Func_080091c0(45, 4, 1, 1, 46, 8);

    /* 186 << 18, 136 << 16. */
    Func_0808a0f0(20, 0x02e80000, 0x00880000);
    Func_080f9010(188);

    /* 160 << 11 = 0x00050000, 128 << 9 = 0x00010000. */
    Func_08009180(58, 8, 49, 8, 1, 3);
    Func_080091f0(0, 0x00050000, 0x00010000);
    Func_080091f0(-1, -1, 0xe666);

    Func_0808a010(20);
    Func_080f9010(188);

    Func_08009180(59, 8, 49, 8, 1, 3);
    Func_080091f0(0, 0x00050000, 0x00010000);
    Func_080091f0(-1, -1, 0xe666);

    /* No argument register is set. */
    Func_080091f8();

    Func_0808a010(10);
    Func_080770c8(0x971);

close_sequence:
    Func_0808a020();
}
