typedef int s32;
typedef unsigned char u8;

/*
 * resource_39e owner at 0x02002778, 116 bytes: a scene-setup beat --
 * install a descriptor through Func_08009178, zero one record byte, aim
 * slot 0, write the shared workspace field, nudge the actor and wait.
 *
 * Complete owner: `push {lr}` at 0x02002778 through `pop {r0} / bx r0` at
 * 0x020027d6-0x020027d8, then the four-word literal pool
 * 0x020027dc-0x020027eb (0x0200c764, 0xcccc, 0x6666, 0x03001ebc). The
 * next owner's prologue is exactly at 0x020027ec -- the already-drafted
 * Func_020027ec. **116 bytes**, measured to the epilogue.
 *
 * Published population (sweep B). All 10 `bl` sites resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 2778 27ec`; sites=10
 * and ten bl lines were transcribed.
 *
 * THE WORKSPACE WRITE IS THE SUBTRACTIVE FORM AND THE VALUE IS 256, NOT
 * 513. The chain is `movs r2, #224 / lsls r2, #1` -> 448 (the
 * displacement, added to the workspace base), then `subs r2, #192` ->
 * 256, and 256 is what gets stored. This overlay's other rows and
 * resource_3af between them use 256, 513, 514, 521 and 0x202 at this same
 * displacement. Transcribe the arithmetic every time; there is no
 * canonical value.
 *
 * The 0xcccc/0x6666 pair handed to Func_0808a090 is the same pooled pair
 * that appears in the already-drafted 0x02002ad0 neighbourhood -- a
 * recurring aim/scale constant in this overlay, pooled rather than built.
 *
 * The descriptor 0x0200c764 is EVEN, so by the odd-word rule it is a data
 * address (image offset 0x4764), not a published Thumb function pointer.
 * The control in the same cluster is 0x020026d8's 0x0200a5b9, which is
 * ODD and really is a callback.
 *
 * Func_0808a0e0(0, 0, -16) takes its third argument through
 * `movs r2, #16 / negs r2, r2` -- a genuine negative, not a large
 * unsigned constant.
 *
 * SHARED IDIOMS, cited: scripted-scene bracket
 * Func_0808a018/Func_0808a020 (resource_39e_c_02001494.c);
 * wait-n-frames Func_0808a010(16).
 *
 * Uncertainty: record[85] is cleared here and record[91] is the byte the
 * neighbouring 0x02000afc gates on -- two different bytes in the same
 * record, both reached by `adds r0, #N` rather than a struct
 * displacement, and neither is identified. Func_080f9010(188),
 * Func_08009178(0x0200c764, 77, 8) and Func_0808a248(2) are transcribed,
 * not identified.
 */

extern void Func_02006b20(u8 *descriptor, s32 arg1, s32 arg2);
extern void Func_02006b9e(s32 frames);
extern void Func_02006b58(void);
extern void Func_02006bb8(void);
extern u8 *Func_02006b96(s32 slot);
extern void Func_02006bb6(s32 slot, s32 arg1, s32 arg2);
extern void Func_02006c00(s32 slot, s32 arg1, s32 arg2);
extern void Func_02006c0c(s32 slot, s32 arg1);
extern void Func_02006cd4(s32 arg0);
extern void Func_02006cfe(s32 arg0);

extern u8 *Data_03001ebc;

void Func_02002778(void)
{
    u8 *record;
    u8 *workspace;

    Func_02006b58();
    Func_02006cfe(188);
    Func_02006b20((u8 *)0x0200c764, 77, 8);

    record = Func_02006b96(0);
    record[85] = 0;

    Func_02006bb6(0, 0xcccc, 0x6666);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 448 - 192;

    Func_02006c0c(0, 2);
    Func_02006c00(0, 0, -16);
    Func_02006b9e(16);
    Func_02006cd4(2);
    Func_02006bb8();
}
