#include "types.h"

/*
 * Region 0x080e4e0c .. 0x080e53f4 (1512 bytes), classified mixed_region.
 *
 * This owner is NOT a whole function. It is the per-kind scene setup slice of
 * a much larger routine: it begins with no prologue (`ldr r1,[sp,#96]` is its
 * first instruction), inherits a live pointer in r7, reads and writes fourteen
 * distinct slots of the enclosing function's stack frame, and ends with the
 * enclosing function's own dense Thumb switch dispatch
 * (`cmp #33 / ldr table / lsls #2 / ldr / mov pc,r3`) whose 34-entry jump
 * table occupies the 136 data bytes immediately after this owner at
 * 0x080e53f4..0x080e547c, and whose out-of-range target 0x080e551a lies in the
 * next region. Reading all 34 table entries gives 14 distinct targets spanning
 * 0x080e547c..0x080e551a, so what follows the table is not one pool block: the
 * ten eight-byte case stubs 0x080e547c..0x080e54cc come first, then the
 * fourteen-word literal pool 0x080e54cc..0x080e5504 that supplies this slice's
 * four out-of-extent words, then three more stubs at 0x080e5504/0x080e550c/
 * 0x080e5514 and the out-of-range arm at 0x080e551a. All of it belongs to the
 * enclosing routine, not to this owner.
 *
 * Because the slice cannot be compiled as part of its parent, every enclosing
 * frame slot it touches is modelled as an explicit parameter. Slots that the
 * slice only reads become value parameters; slots that it writes and that
 * outlive the slice (the two camera words, the interpolation output triple,
 * the effect duration and the frame counter) become pointer parameters, so
 * that the stores are preserved and the reloads that the reference performs
 * across intervening calls remain justified. Nothing here is a recovered
 * historical spelling; the parameter list is a reading aid for one slice.
 *
 * Frame ledger (offsets are the reference's own `[sp,#N]` immediates):
 *   sp+0, sp+4   compiler-managed outgoing argument slots for the six-argument
 *                rectangle-blit call; not source variables.
 *   sp+8         `origin`, pointer to a three-word world origin (x, y, z).
 *   sp+16, sp+20 written once each with `kind - 4` and `kind - 2` immediately
 *                before the two unsigned range tests, and never read again
 *                inside this extent. Modelled as spills of those subexpressions
 *                and therefore not represented below. UNPROVEN: because the
 *                stores are dead within the owner but the owner ends four
 *                instructions later, the values may instead be live-out into
 *                the 34 case bodies. Deciding that needs the enclosing routine,
 *                so these are the two reference stores the draft omits.
 *   sp+24        CSE'd address of the sp+136 output triple (see `out`).
 *   sp+28        `record`, the resolved party/effect record pointer.
 *   sp+32..sp+48 five words copied verbatim into `unit` fields 72, 52, 44, 40
 *                and 36 (`field72` .. `field36` below).
 *   sp+56        `step`, pointer to a two-word (x, y) step record.
 *   sp+60        `scroll`, pointer to the scalar subtracted from the BG2X seed.
 *   sp+64        `dur`, the effect duration selected by the cascade.
 *   sp+68, sp+72 the two camera words (`camera[0]`, `camera[1]`).
 *   sp+84        `frame`, the enclosing per-frame loop counter. This slice
 *                writes its zero initialiser and then reads it three times;
 *                the loop's back edge is outside the owner.
 *   sp+88        `canvas`, the blit destination.
 *   sp+92        `work`, the 0x03001eec battle-work base (family offsets
 *                0x6980, 0x7080, 0x7098, 0x7780, 0x7784, 0x7828 all match
 *                games/gs1/recon/en/main/080dd9c0.c and 080e6eac.c).
 *   sp+96        `kind`, the scene selector, 0..33 plus the out-of-range 35
 *                and 100 cases the cascades test.
 *   sp+100       `object`, the effect object.
 *   sp+104       the rectangle-blit callback reached through the
 *                _call_via_r4 trampoline at 0x080072f4 (`draw`).
 *   sp+136       the three-word interpolation output triple (`out`).
 *
 * Calls through 0x080072f8 are the _call_via_r5 trampoline; at every one of
 * those sites r5 holds the constant 0x03000164, the IWRAM clear routine the
 * family already spells as a raw ClearFn cast (see 080cd594.c, 080e6eac.c).
 * The single 0x080072f4 site is _call_via_r4 with the sp+104 callback.
 *
 * Literal pool words read out of the owner's own bytes: 0x00000100 (BG2PA),
 * 0x04000020/0x04000028/0x04000050/0x04000052, 0x00007828/0x00007784/
 * 0x00007098, 0x03000164, 0x06004000, 0xff800000, 0xfff40000, 0x080cd4b5,
 * 0x080cd261, 0x000000ab/ac/ad/ae, 0x02010000, 0x00000e10; and out of the
 * following pool block: 0x02014000, 0x080dbb9d, 0x00007828, 0x080e53f4.
 *
 * KNOWN RESIDUALS (measured, not resolved):
 *   - Two reference stores are not represented: `str [sp,#20]` of `kind - 2`
 *     and `str [sp,#16]` of `kind - 4` (see the sp+16/sp+20 ledger entry).
 *     Every other store, branch, loop and call in the extent is present.
 *   - The slice necessarily gains a prologue/epilogue the reference does not
 *     have, and loses the reference's `mov r0,sp / adds r0,#136 / str r0,[sp,#24]`
 *     address materialisation because `out` arrives as a pointer.
 *   - The terminal switch is modelled as an indirect call through the real
 *     jump table at 0x080e53f4 rather than as a C `switch`: reproducing the
 *     dispatch as a switch would require the 34 case bodies, which lie outside
 *     this owner and would overshoot its extent by hundreds of bytes.
 *   - The `kind == 33` early exit that the reference folds into the draw-guard
 *     chain is written as an ordinary member of that chain.
 *   - Register residency, not structure: the reference reloads `kind`, `work`
 *     and `record` from their frame slots at nearly every use, because in the
 *     whole routine those slots compete with far more live values than this
 *     slice contains. Here the same three end up resident in sl / fp / r9, so
 *     the slice pays a four-register high-save prologue and epilogue that the
 *     reference does not have, and loses one `ldr [sp,#N]` per use. This is a
 *     coloring tie-break inside a slice of a larger frame; it was not chased
 *     with artificial competing locals.
 *   - `(camera[1] >> 16) << 8` narrows to `ldrsh r3,[r1,#6]` here, where the
 *     reference reloads the whole word and shifts (`ldr` + `asrs #16`).
 *
 * Measured with `alchemy score ... --owner 080e4e0c`: candidate 1556 bytes
 * against reference 1512 (102.9 %), differing_halfwords 708, wrong_instructions
 * 776. Every reference call site is represented: `alchemy inspect 080e4e0c`
 * reports 52 resolved call sites over 19 distinct targets, and the body below
 * contains all 52 with the same per-target multiplicities. The one call the
 * draft adds is Func_080e551a, which the reference reaches with a plain branch
 * (see the residuals above).
 */

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);
typedef void (*ClearFn)(void *dest, s32 size);

#define REG_BG2PA (*(volatile u16 *)0x04000020)
#define REG_BG2X (*(volatile s32 *)0x04000028)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

#define IWRAM_CLEAR ((ClearFn)0x03000164)
#define SceneBody_080e53f4 ((void (*const *)(void))0x080E53F4)

/*
 * Names in comments are the project names `alchemy inspect 080e4e0c` prints
 * for these targets; the declarations keep the Func_<address> spelling the
 * owner register binds, exactly as the family sibling
 * games/gs1/recon/en/main/080e99c0.c already annotates the same helpers.
 * Targets with no comment are ones the project has not yet named.
 */
/* Runtime_ReleaseHeapBlock */
void Func_08002dd8(s32 id);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_08002322(s32 angle);
/* Scheduler_AddOrUpdateCallback */
void Func_080041d8(void *callback, s32 interval);
/* Scheduler_RemoveCallback */
void Func_08004278(void *callback);
/* random_16 */
u32 Func_08004458(void);
void Func_08009080(void *unit, s32 mode);
void Func_08009088(void *unit, s32 mode);
s32 Func_080b5070(s32 id);
void **Func_080b5098(s32 id);
void Func_080d4604(void *object, s32 mode);
/* BattleFx_RenderMode5 */
void Func_080d52a4(void *object);
void Func_080dea70(void *object, s32 mode);
/* load_and_decompress */
void Func_080e0524(s32 resource_id, void *destination, s32 a, s32 b);
/* apply_step_and_y_offset */
void Func_080e396c(s32 value, s32 *step);

/*
 * The four resource identifiers below reach r0 through a pc-relative literal
 * (`ldr r0,[pc,#N]`), never through the one-instruction `movs r0,#imm8` that
 * an ordinary 0..255 integer constant would use, so each one is a relocated
 * symbol address. Spelled the way the family's exact sibling
 * games/gs1/recon/en/main/080dd9c0.c already spells the same argument.
 */
extern u8 Value_000000ab;
extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 Value_000000ae;
void Func_080e551a(void);
void Func_080e657c(void);
void Func_080e65f8(void);

void Func_080e4e0c(
    void *unit, void *canvas, void *object, s32 *origin, void *work, u32 kind,
    s32 *step, s32 *scroll, s32 *camera, s32 *out, s32 *dur, s32 *frame,
    DrawRectangleFn draw, s32 field36, s32 field40, s32 field44, s32 field52,
    s32 field72)
{
    void *record;
    void *cur;
    s32 angle;
    s32 x0;
    s32 half;
    s32 y0;
    s32 i;

    if (kind == 32) {
        REG_BG2PA = 0x0100;
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 0) {
            camera[1] = -0x800000;
            camera[0] = 0xC0000;
        } else {
            camera[1] = 0x80000;
            camera[0] = -0xC0000;
        }
        REG_BG2X = (camera[1] >> 16) << 8;
    }

    if (kind == 8) {
        REG_BG2PA = 0x0100;
        REG_BG2X = (64 - *scroll) << 8;
        M2C_FIELD(work, s32 *, 0x7780) = 1;
        M2C_FIELD(work, s32 *, 0x7784) = 0;
        IWRAM_CLEAR((void *)0x06004000, 0x4000);
        IWRAM_CLEAR(canvas, 0x4000);
        REG_BLDCNT = 0;
    }

    if (kind == 31) {
        REG_BG2PA = 0x0100;
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 0) {
            x0 = 32;
        } else {
            x0 = 96;
        }
        REG_BG2X = (x0 - *scroll) << 8;
    }

    if (kind == 15 || kind == 17 || kind == 24 || kind == 26) {
        IWRAM_CLEAR((void *)0x06004000, 0x4000);
        IWRAM_CLEAR(canvas, 0x4000);
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 28) = 0;
        Func_08004278((void *)0x080CD4B5);
        Func_08004278((void *)0x080CD261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        Func_08009080(unit, 3);
        if (kind == 15) {
            Func_080dea70(object, 9);
        }
        if (kind == 24) {
            Func_080d52a4(object);
        }
        if (kind != 26) {
            Func_080e65f8();
        }
        Func_080dea70(object, 8);
        Func_080e65f8();
    }

    Func_08009088(unit, 16);
    M2C_FIELD(unit, s32 *, 36) = field36;
    M2C_FIELD(unit, s32 *, 40) = field40;
    M2C_FIELD(unit, s32 *, 44) = field44;
    M2C_FIELD(unit, s32 *, 52) = field52;
    M2C_FIELD(unit, s32 *, 72) = field72;

    if (kind == 35) {
        IWRAM_CLEAR((void *)0x06004000, 0x4000);
        IWRAM_CLEAR(canvas, 0x4000);
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 28) = 0;
        Func_08004278((void *)0x080CD4B5);
        Func_08004278((void *)0x080CD261);
        Func_08002dd8(47);
        Func_08002dd8(46);
        M2C_FIELD(object, s32 *, 24) = 3;
        Func_080d4604(object, 2);
        Func_080e65f8();
    }

    record = *Func_080b5098(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36));

    out[0] = Func_080022ec(M2C_FIELD(record, s32 *, 8) - origin[0], 6);
    out[1] = Func_080022ec(
        (M2C_FIELD(record, s32 *, 12) - origin[1]) + 0x1E0000, 6);
    out[2] = Func_080022ec(M2C_FIELD(record, s32 *, 16) - origin[2], 6);

    cur = (s8 *)work + 0x7098;
    for (i = 0; i != 64; i++) {
        M2C_FIELD(cur, s32 *, 0) = 0;
        cur = (s8 *)cur + 28;
    }

    if (kind != 14) {
        half = Func_080b5070(
            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36));
        half = half / 2;
        cur = (s8 *)work + 0x7080;
        for (i = 0; i != 32; i++) {
            M2C_FIELD(cur, s32 *, 4) = half;
            M2C_FIELD(cur, s32 *, 0) = M2C_FIELD(record, s32 *, 8);
            M2C_FIELD(cur, s32 *, 8) = M2C_FIELD(record, s32 *, 16);
            if (kind == 31) {
                M2C_FIELD(cur, s32 *, 12) =
                    ((s32)(Func_08004458() & 255) - 127) << 12;
                M2C_FIELD(cur, s32 *, 16) =
                    ((s32)(Func_08004458() & 255) - 64) << 10;
            } else {
                M2C_FIELD(cur, s32 *, 12) =
                    ((s32)(Func_08004458() & 255) - 127) << 12;
                M2C_FIELD(cur, s32 *, 16) =
                    ((s32)(Func_08004458() & 255) - 64) << 12;
            }
            M2C_FIELD(cur, s32 *, 20) =
                ((s32)(Func_08004458() & 255) - 127) << 12;
            M2C_FIELD(cur, s32 *, 24) = i / 2 + 32;
            cur = (s8 *)cur + 28;
        }
    }

    if (kind == 11) {
        Func_080e0524((s32)&Value_000000ab, work, 1, 1);
        Func_080e0524((s32)&Value_000000ac, (void *)0x02010000, 1, 0);
        REG_BLDALPHA = 0x0E10;
    }

    if (kind == 32) {
        Func_080e0524((s32)&Value_000000ad, work, 1, 1);
        Func_080e0524((s32)&Value_000000ae, (void *)0x02010000, 1, 0);
        REG_BLDALPHA = 0x0E10;
    }

    if (kind != 7 && kind != 13 && kind != 18 && kind != 11 && kind != 32 &&
        kind != 19) {
        if (kind == 12) {
            y0 = 0;
        } else {
            y0 = 0x140000;
        }
        cur = (void *)0x02014000;
        for (i = 0; i != 64; i++) {
            M2C_FIELD(cur, s32 *, 4) = y0;
            M2C_FIELD(cur, s32 *, 0) = M2C_FIELD(record, s32 *, 8);
            M2C_FIELD(cur, s32 *, 8) = M2C_FIELD(record, s32 *, 16);
            if (kind == 5 || kind == 23) {
                M2C_FIELD(cur, s32 *, 12) =
                    ((s32)(Func_08004458() & 255) - 127) << 11;
                M2C_FIELD(cur, s32 *, 16) =
                    (s32)(Func_08004458() & 255) << 11;
                M2C_FIELD(cur, s32 *, 20) =
                    ((s32)(Func_08004458() & 255) - 127) << 11;
            } else if (kind == 25) {
                M2C_FIELD(cur, s32 *, 12) =
                    ((s32)(Func_08004458() & 255) - 127) << 11;
                M2C_FIELD(cur, s32 *, 16) =
                    (s32)(Func_08004458() & 127) << 10;
                M2C_FIELD(cur, s32 *, 20) =
                    ((s32)(Func_08004458() & 255) - 127) << 11;
            } else {
                M2C_FIELD(cur, s32 *, 12) =
                    ((s32)(Func_08004458() & 255) - 127) << 10;
                M2C_FIELD(cur, s32 *, 16) =
                    (s32)(Func_08004458() & 127) << 10;
                M2C_FIELD(cur, s32 *, 20) =
                    ((s32)(Func_08004458() & 255) - 127) << 10;
            }
            M2C_FIELD(cur, s32 *, 24) = 0;
            cur = (s8 *)cur + 28;
        }
    }

    if (kind - 2 <= 1 || kind == 12 || kind == 22 || kind == 29 ||
        kind == 28) {
        Func_080041d8((void *)0x080DBB9D, 144 << 3);
    }

    if (kind - 4 <= 2 || kind == 23 || kind == 30 || kind == 27 ||
        kind == 33 || kind == 34 || kind == 100) {
        *dur = 32;
    } else if (kind <= 3 || kind == 8 || kind == 9 || kind == 10 ||
               kind == 22 || kind == 25 || kind == 29 || kind == 31 ||
               kind == 14) {
        *dur = 48;
    } else {
        *dur = 20;
        if (kind != 21) {
            if (kind == 11 || kind == 32 || kind == 20) {
                *dur = 40;
            } else if (kind == 28 || kind == 12) {
                *dur = 64;
            } else {
                *dur = 80;
            }
        }
    }

    *frame = 0;
    if (*dur == 0) {
        Func_080e657c();
    }

    if (kind != 11 && kind != 32) {
        cur = (s8 *)work + 0x6980;
        angle = *frame << 12;
        for (i = 0; i != 160; i++) {
            M2C_FIELD(cur, s32 *, 0) = (0x40000 - (Func_08002322(angle) << 2))
                >> 10;
            cur = (s8 *)cur + 4;
            angle += 0x800;
        }
    }

    if (*frame <= 2) {
        Func_080e396c(M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8),
            step);
        step[0] = step[0] / 2;
        step[1] += 16;
    }

    if (kind != 11 && kind != 8 && kind != 32 && kind != 33 && kind != 34 &&
        *frame <= 11) {
        /*
         * The row offset is written out in both arms because the reference
         * recomputes the whole `27 * (frame / 2) << 7` chain, and reloads
         * `frame`, separately inside each arm rather than sharing one copy.
         */
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 0) {
            draw(canvas, (s8 *)work + (*frame / 2) * 27 * 128, step[0] - 32,
                step[1] - 40, 48, 72);
        } else {
            draw(canvas, (s8 *)work + (*frame / 2) * 27 * 128, step[0],
                step[1] - 40, 48, 72);
        }
    }

    if (kind > 33) {
        Func_080e551a();
        return;
    }
    SceneBody_080e53f4[kind]();
}
