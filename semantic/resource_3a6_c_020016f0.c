typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a6 owner at 0x020016f0, 88 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_3a6), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Body: write 516 to the workspace's +448 s32, then read the scene id
 * s16 at 0x02000240+448.  For scene 93 only, OVERWRITE that same +448
 * cell with 256, refresh through Func_080000c0(1), put records 11 and
 * 12 into mode 3 and raise flag 0x12f.  Either way, finish by running
 * Func_02001984 and returning 0.
 *
 * The double write to workspace +448 is the notable detail and is
 * transcribed as compiled: 516 is stored unconditionally and 256
 * replaces it inside the scene-93 arm, so the first value is only
 * observable to something that runs between them -- nothing here
 * does. The first write is therefore kept.
 *
 * Complete owner: `push {lr}` at 0x020016f0 through `movs r0, #0 /
 * pop {r1} / bx r1` at 0x02001732-0x02001736, alignment halfword,
 * then the four-word literal pool 0x02001738-0x02001747 (0x03001ebc,
 * 0x02000240, 0x5d, 0x12f).  Returns a literal 0.
 *
 * The scene id is read from 0x02000240+448, the same cell and the
 * same +448/+450 global that resource_39e's and resource_3a4's
 * drivers use; +448 selects the scene.
 *
 * Uncertainty: Func_02001984 is not yet owned and is declared here
 * only; the flag id and scene id are literal.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_02001984(void);

extern void Func_080000c0(s32 arg0);
extern void Func_080770d0(s32 flagId);
extern void Func_0808a1e0(s32 id, s32 mode);

s32 Func_020016f0(void)
{
    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Data_02000240[224] == 93) {
        *(s32 *)(Data_03001ebc + 448) = 256;
        Func_080000c0(1);
        Func_0808a1e0(11, 3);
        Func_0808a1e0(12, 3);
        Func_080770d0(0x12f);
    }
    Func_02001984();
    return 0;
}
