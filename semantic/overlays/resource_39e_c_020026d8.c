typedef int s32;
typedef unsigned char u8;

/*
 * resource_39e owner at 0x020026d8, 140 bytes: the scene that INSTALLS
 * this overlay's per-frame callback, plays a short beat, and uninstalls
 * it again.
 *
 * Complete owner: `push {r5, lr}` at 0x020026d8 through `pop {r5} /
 * pop {r0} / bx r0` at 0x0200274e-0x02002752, then the four-word literal
 * pool 0x02002754-0x02002763 (0x0200a5b9, 0x3333, 0x1999, 0x03001ebc).
 * The next owner's prologue is exactly at 0x02002764. **140 bytes**,
 * measured to the epilogue.
 *
 * Not found by the structural inventory walk (unindexed): reached only as
 * a published pointer. All 15 `bl` targets resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 26d8 2764` under the
 * `+2` rule. No branches.
 *
 * THIS ROW IS THE PUBLISHER OF ITS OWN NEIGHBOUR, AND THAT IS THE FIND.
 * The pool word at 0x02002754 is **0x0200a5b9** -- ODD, so a Thumb
 * pointer, not a data address. Resolving it the way the published sweep
 * does, `(0x0200a5b9 & ~1) - 0x8000`, gives **0x020025b8**: the 164-byte
 * owner two rows up, which the sweep reports as published-with-no-caller.
 * It is handed to Func_080000d0 with 200<<4 at 0x020026e6 and to
 * Func_080000d8 at 0x0200273c, a register/unregister pair bracketing the
 * beat between them. So the pairing is install-here, run-per-frame-there,
 * and nothing in either function's call graph shows the link -- the
 * address only ever appears as an argument.
 *
 * The practical rule this earns: when a pool word in an overlay row is
 * ODD, resolve it as a published pointer BEFORE assuming it is a data
 * descriptor. Every other pointer-shaped word in this cluster
 * (0x0200c77a in 0x02002484) is EVEN and genuinely is data. One bit is
 * the whole difference, and a row that installs a callback reads exactly
 * like a row that passes a table.
 *
 * The workspace write is the ordinary single-field form: `movs r2,#228 /
 * lsls r2,#1` gives the displacement 456 and the value 60 follows. NOT
 * the displacement-from-value chain -- only one field is written, so
 * there is no previous value to derive the next displacement from.
 *
 * Note Func_0808a080(0) is called at 0x02002730 purely to obtain the
 * record for Func_080091e0; its result is not otherwise used, and it is a
 * fresh fetch rather than anything carried from earlier in the row.
 *
 * 0x3333 and 0x1999 arrive as pool words here. 0x3333 is 13107, the same
 * value 0x020025b8 builds with a shift chain (`*3`, `*17`, `*257`) rather
 * than pooling -- one constant, two spellings, in two functions that talk
 * to each other. The compiler picks per value and per context, never per
 * meaning.
 *
 * SHARED IDIOM: the Func_0808a018/Func_0808a020 scripted-scene bracket
 * and the Func_0808a368/Func_0808a370 close, cited from
 * resource_39e_c_02001494.c.
 *
 * Uncertainty: none of the fifteen callees are identified beyond call
 * shape; every id and constant is recorded as a passed value.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 arg0);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080000d0(void *callback, s32 arg1);
extern void Func_080000d8(void *callback);
extern void Func_080091e0(u8 *record, s32 arg1);
extern void Func_080f9010(s32 arg0);

extern u8 *Data_03001ebc;

void Func_020026d8(void)
{
    void *frame_callback = (void *)0x0200a5b9;
    u8 *record;
    u8 *workspace;

    Func_0808a018();
    Func_080000d0(frame_callback, 200 << 4);

    Func_0808a090(0, 0x3333, 0x1999);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 456) = 60;

    Func_0808a368();
    Func_080f9010(154);
    Func_0808a100(0, 2);
    Func_0808a0e0(0, 0, -6);
    Func_0808a0e8(0);
    Func_0808a158(0, 15);

    record = Func_0808a080(0);
    Func_080091e0(record, 0);

    Func_080000d8(frame_callback);
    Func_0808a370();
    Func_0808a248(3);
    Func_0808a020();
}
