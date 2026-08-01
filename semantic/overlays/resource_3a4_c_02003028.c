typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x02003028, 1000 bytes: a four-actor staged scene
 * -- place actors 1, 2 and 3 from actor 0's position, load three sprite
 * sets, walk them through a long series of moves and poses with two
 * sound cues, briefly install this overlay's own 0x02002ffc as a
 * per-frame callback while a fourth sprite set is swapped in, then close
 * two workspace fields, clear story flag 0x12f and set 0x909.
 *
 * A SWEEP-A ROW: a caller exists somewhere in the image, so it is not a
 * published callback. No arguments, void: r0 is written before it is
 * read at 0x02003032.
 *
 * ITS SPAN IS 1000, NOT THE 1024 THE RECORDED-OWNER BOUND GIVES, and the
 * difference is not padding. Complete owner: `push {r5, r6, lr}` at
 * 0x02003028 through `pop {r5, r6} / pop {r0} / bx r0` at
 * 0x020033cc-0x020033d0, one alignment halfword, and the fifteen-word
 * literal pool 0x020033d4-0x0200340f. measureSpan reports 938,
 * undersizing by the pool.
 *
 * WHAT SITS IN THE REMAINING 24 BYTES IS A FUNCTION NO SWEEP REPORTS.
 * 0x02003410-0x02003427 is a complete 24-byte leaf --
 *
 *     ldr r3, =0x03001ebc / movs r1, #191 / ldr r3, [r3] / lsls r1, #1 /
 *     adds r2, r3, r1 / ldr r3, =0x1018 / strh r3, [r2] / bx lr
 *
 * -- writing 0x1018 into the halfword at workspace + 382, with its own
 * two-word pool at 0x02003420-0x02003427 and the next recorded owner at
 * 0x02003428. It is invisible to all three of this overlay's sweeps at
 * once: sweep A cannot see it because NO `bl` anywhere in the image
 * targets 0x3410 (checked by resolving every BL-shaped halfword pair in
 * the whole 0x5238-byte image with the same `+2` rule this row's sites
 * used); sweep B cannot see it because no word anywhere in the image
 * holds 0x0200b411 or 0x0200b410, its published spelling; and sweep C
 * cannot see it because it has NO PUSH PROLOGUE -- being a leaf, it
 * returns with `bx lr` and never saves a register, so the shaped scan,
 * which keys on `b5xx`/`b4xx`, has nothing to key on. It is written up
 * here rather than drafted because it is not this row and not in my
 * contract; resource_3a4 must NOT be certified closed while it stands.
 *
 * Ninety-six call sites, every one resolved with the `+2` rule via
 * `bun tools/overlay_call_targets.ts resource_3a4 0x3028 0x3410` and
 * none read from the listing's arrows. Site count cross-checked: the
 * resolver reports sites=96 and the listing carries 96 `bl` lines. They
 * collapse onto twenty-six targets, all import veneers -- seventeen
 * sites share Func_0808a1b8 and nine share Func_0808a1e8 -- with no
 * in-image prologue among them.
 *
 * A FOURTH PUBLISHED-POINTER INSTANCE, and the first that also REMOVES
 * the callback it installs. The pool word at 0x020033fc is 0x0200affd:
 * image offset 0x2ffc with the Thumb bit under the base + 0x8000
 * in-image spelling, i.e. the already-drafted 0x02002ffc. This row hands
 * it to Func_080000d0 at the established 200 << 4 rate at 0x020032b2,
 * swaps a sprite set underneath it, and hands the same word to
 * Func_080000d8 at 0x020032ca. So on this overlay the mechanism has now
 * been seen installing (0x02002b58, 0x02002f10, 0x02001838, 0x02001d0c)
 * and, here, uninstalling.
 *
 * THE DISPLACEMENT-FROM-VALUE CHAIN APPEARS TWICE, both times through
 * the workspace pointer at 0x03001ebc, and reading either subtraction
 * against the base writes the wrong field:
 *   - 0x02003054: 224 << 1 makes the POINTER 448, `subs r3, #192` turns
 *     448 into the VALUE 256 stored at +448, and `adds r3, #200` turns
 *     256 into the DISPLACEMENT 456 for the next store of 40;
 *   - 0x020033a4: 224 << 1 makes the pointer 448 again and `adds r2, #68`
 *     turns 448 into the VALUE 516 stored at +448.
 * That is the same habit Isaac recorded twice on resource_3b9, now with
 * two more sightings on a different overlay -- five in all, so it is
 * this compiler's normal shape for a workspace write and not a curiosity.
 *
 * The only conditional in the body, at 0x02003160, is not two
 * independent blocks: the zero arm runs Func_0808a110(2, 3) and BRANCHES
 * to 0x02003184, which is exactly where the non-zero arm falls through
 * after Func_0808a110(2, 4) and its halfword increment. The three
 * identical guards at the head are each a plain null check on the actor-0
 * handle with no shared tail.
 *
 * r5 is reused four times -- the workspace address 0x03001ebc at
 * 0x02003052, Data_0200c0e4 at 0x020032b6, Data_0200c12c at 0x02003382,
 * and 0x03001ebc again at 0x020033a2 -- so no site below reads it as a
 * value carried from an earlier one.
 *
 * Constants are written in the shifted-literal form the encodings build
 * -- 164 << 17, 176 << 15, 224 << 1, 236 << 1, 228 << 1, 192 << 8,
 * 224 << 8, 160 << 8, 128 << 6, 128 << 7, 192 << 7, 129 << 1, 128 << 1,
 * 200 << 4 -- because that shape, not the flattened value, is what the
 * row shows. Pool constants 0x9999, 0x4ccc, 0x190c, 0x1910, 0x101,
 * 0x121, 0x12f and 0x909 are transcribed as words.
 */

extern void Func_080000d0(s32 callback, s32 rate);
extern void Func_080000d8(s32 callback);
extern void Func_080770c8(s32 flag_id);
extern void Func_080770d0(s32 flag_id);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, u8 *set);
extern void Func_0808a0b0(s32 id, u8 *set);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a170(s32 arg0);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 sound_id);

extern u8 *Data_03001ebc;    /* the scene workspace pointer */

extern u8 Data_0200c054[];   /* image offset 0x4054: sprite set */
extern u8 Data_0200c084[];   /* image offset 0x4084: sprite set */
extern u8 Data_0200c0b4[];   /* image offset 0x40b4: sprite set */
extern u8 Data_0200c0e4[];   /* image offset 0x40e4: sprite set */
extern u8 Data_0200c12c[];   /* image offset 0x412c: sprite set */

/* image offset 0x2ffc with the Thumb bit: this overlay's own 0x02002ffc */
#define PUBLISHED_02002ffc 0x0200affd

void Func_02003028(void)
{
    u8 *workspace;
    u8 *handle;

    Func_0808a018();
    Func_0808a0f0(8, 164 << 17, 176 << 15);
    Func_0808a0f0(9, 164 << 17, 176 << 15);
    Func_0808a100(8, 0);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + (224 << 1)) = 256;
    *(s32 *)(workspace + 456) = 40;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    handle = Func_0808a080(0);
    if (handle != 0) {
        Func_0808a0f0(1, *(s32 *)(handle + 8), *(s32 *)(handle + 16));
    }

    handle = Func_0808a080(0);
    if (handle != 0) {
        Func_0808a0f0(2, *(s32 *)(handle + 8), *(s32 *)(handle + 16));
    }

    handle = Func_0808a080(0);
    if (handle != 0) {
        Func_0808a0f0(3, *(s32 *)(handle + 8), *(s32 *)(handle + 16));
    }

    Func_0808a090(1, 0x9999, 0x4ccc);
    Func_0808a090(2, 0x9999, 0x4ccc);
    Func_0808a090(3, 0x9999, 0x4ccc);

    Func_0808a098(1, Data_0200c054);
    Func_0808a098(2, Data_0200c084);
    Func_0808a0b0(3, Data_0200c0b4);

    Func_0808a1b8(1, 192 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 192 << 8, 40);

    Func_0808a138(1, 1);
    Func_0808a1b8(1, 224 << 8, 10);
    Func_0808a170(0x190c);
    Func_0808a188(1, 0, 10);

    Func_0808a138(2, 1);
    Func_0808a1b8(2, 160 << 8, 10);
    Func_0808a1b8(0, 128 << 6, 0);
    Func_0808a178(2, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(2, 3);
    } else {
        Func_0808a110(2, 4);
        workspace = Data_03001ebc;
        *(u16 *)(workspace + (236 << 1)) = (u16)(*(u16 *)(workspace + (236 << 1)) + 1);
    }

    Func_0808a188(2, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(0, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);

    Func_0808a138(3, 2);
    Func_0808a170(0x1910);
    Func_0808a180(3, 0);
    Func_0808a1b8(0, 128 << 7, 10);
    Func_0808a1e8(1, 129 << 1, 60);
    Func_0808a1b8(1, 128 << 6, 40);
    Func_0808a180(1, 0);
    Func_0808a1b8(0, 192 << 7, 10);

    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a1e8(3, 0x101, 40);

    Func_080f9010(190);
    Func_0808a158(8, 7);
    Func_0808a010(10);
    Func_080f9010(0x121);

    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 192 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 192 << 8, 0);

    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a1e8(2, 128 << 1, 0);
    Func_0808a1e8(3, 128 << 1, 40);

    Func_080f9010(103);

    Func_080000d0(PUBLISHED_02002ffc, 200 << 4);
    Func_0808a098(9, Data_0200c0e4);
    Func_0808a0b0(8, Data_0200c0e4);
    Func_080000d8(PUBLISHED_02002ffc);

    Func_0808a010(60);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a1b8(1, 224 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 20);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);

    Func_0808a138(3, 1);
    Func_0808a188(3, 0, 20);
    Func_0808a1b8(1, 224 << 8, 0);
    Func_0808a1b8(0, 192 << 7, 40);
    Func_0808a110(1, 3);
    Func_0808a188(1, 0, 10);
    Func_0808a110(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);

    Func_0808a098(1, Data_0200c12c);
    Func_0808a098(2, Data_0200c12c);
    Func_0808a0b0(3, Data_0200c12c);
    Func_0808a010(20);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + (224 << 1)) = 516;
    Func_080770d0(0x12f);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + (228 << 1)) = 16;
    Func_080770c8(0x909);

    Func_0808a020();
}
