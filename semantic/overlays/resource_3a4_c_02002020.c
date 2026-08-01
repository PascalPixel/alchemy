typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x02002020, 176 bytes: THE OVERLAY'S ENTRY
 * DRIVER, and the reason this overlay's closure was false -- every one
 * of the nine handlers it dispatches to was already drafted, but the
 * function that chooses between them was owned by nobody.
 *
 * It is the target of this overlay's header veneer (the word at image
 * offset 4; bun tools/overlay_driver.ts resource_3a4), so the loader
 * enters here and nothing inside the image calls it.
 *
 * Body: write 516 to the workspace's +448 s32, then read the s16 at
 * 0x02000240+448 and run exactly one handler for it --
 *   77 -> Func_020020d0   79 -> Func_02002310   80 -> Func_02002428
 *   81 -> Func_02002490   82 -> Func_020025c0   83 -> Func_020026c0
 *   85 -> Func_02002804   86 -> Func_02002934   87 -> Func_020029dc
 * -- then return 0.  The chain is a linear compare-and-branch, not a
 * table, and it is sparse: 78 and 84 fall through to the plain return
 * along with everything outside 77..87.
 *
 * Complete owner: `push {lr}` at 0x02002020 through `movs r0, #0 /
 * pop {r1} / bx r1` at 0x0200209e-0x020020a2, then the eleven-word
 * literal pool 0x020020a4-0x020020cf (0x03001ebc, 0x02000240 and the
 * nine selector constants); next owner (resource_3a4_c_020020d0.c,
 * already drafted) exactly at 0x020020d0.
 *
 * The field at 0x02000240+448 is NOT the selector the handlers
 * themselves test: those read +450 (`== 99`, `== 2`, `== 6` in the
 * drafted siblings).  Two adjacent s16 in the same global, read by
 * different layers -- +448 picks the handler, +450 sub-selects inside
 * it.  Both are recorded by offset.
 *
 * Uncertainty: the roles of the two s16 fields beyond that split, and
 * the meaning of the 516 written to workspace +448 (the neighbouring
 * overlays write 513, 514 and 515 to the same cell), are open.  The
 * gaps at 78 and 84 are transcribed as compiled, not smoothed.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_020020d0(void);
extern void Func_02002310(void);
extern void Func_02002428(void);
extern void Func_02002490(void);
extern void Func_020025c0(void);
extern void Func_020026c0(void);
extern void Func_02002804(void);
extern void Func_02002934(void);
extern void Func_020029dc(void);

s32 Func_02002020(void)
{
    s16 scene;

    *(s32 *)(Data_03001ebc + 448) = 516;
    scene = Data_02000240[224];
    if (scene == 77) {
        Func_020020d0();
    } else if (scene == 79) {
        Func_02002310();
    } else if (scene == 80) {
        Func_02002428();
    } else if (scene == 81) {
        Func_02002490();
    } else if (scene == 82) {
        Func_020025c0();
    } else if (scene == 83) {
        Func_020026c0();
    } else if (scene == 85) {
        Func_02002804();
    } else if (scene == 86) {
        Func_02002934();
    } else if (scene == 87) {
        Func_020029dc();
    }
    return 0;
}
