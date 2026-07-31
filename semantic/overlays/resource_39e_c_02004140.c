typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

/*
 * resource_39e owner at 0x02004140, 132 bytes: an eight-shot spawn loop
 * feeding this overlay's own eight-argument spawner
 * (resource_39e_c_0200013c.c). Fetches the slot-8 record
 * (Func_0808a080(8), the record-pointer idiom), builds a 32-byte
 * descriptor on the stack (sp+16: word 1, u16 0x0119 at +24, pointer
 * 0x0200d1d8 at +28, 224<<10 at +16, 192<<9 at +20), then loops eight
 * times: wait 10 frames (Func_0808a010), on odd iterations play sound
 * 0x82 (Func_080f9010), and spawn with
 * Func_0200013c(record[8], record[12], record[16] + 0xffe80000, 0,
 * 0x9999, 0, 0x00360001, descriptor). Closes with a 60-frame wait.
 *
 * Complete owner: `push {r5, r6, r7, lr} / sub sp, #56` at
 * 0x02004140-0x02004144 through `add sp, #56 / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x020041a6-0x020041ac, five pool words plus an
 * alignment halfword ending 0x020041c3; the next owner prologue
 * (`push {lr}`, the 36-byte gap function drafted as
 * resource_39e_c_020041c4.c in the same round) begins at 0x020041c4.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_39e 4140 41ac's
 * +2 rule; the loop's spawn site is an intra-overlay bl to
 * Func_0200013c).
 *
 * Uncertainty: callees identified by shape; the descriptor field
 * meanings and the constants (0x0119, 0x9999, 0x00360001, the
 * 0xffe80000 offset) are transcribed from the arithmetic, meaning
 * unknown. The loop is `bls` against 7 -- unsigned counter, per the
 * lever-playbook signedness rule.
 */

extern u8 *Func_0808a080(s32 id);
extern void Func_0808a010(s32 frames);
extern void Func_080f9010(s32 sound_id);
extern void Func_0200013c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);

void Func_02004140(void)
{
    u8 descriptor[32];
    u8 *record;
    u32 i;

    record = Func_0808a080(8);
    *(u32 *)(descriptor + 0) = 1;
    *(u16 *)(descriptor + 24) = 0x0119;
    *(u32 *)(descriptor + 28) = 0x0200d1d8;
    *(u32 *)(descriptor + 16) = 224 << 10;
    *(u32 *)(descriptor + 20) = 192 << 9;
    for (i = 0; i <= 7; i++) {
        Func_0808a010(10);
        if (i & 1) {
            Func_080f9010(0x82);
        }
        Func_0200013c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + 0xffe80000, 0,
                      0x9999, 0, 0x00360001, descriptor);
    }
    Func_0808a010(60);
}
