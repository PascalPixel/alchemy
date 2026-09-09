#include "types.h"

/* Message ids handed to the Func_0808a170 service; see the header note. */
extern u8 Data_00000fe3[];
extern u8 Data_00000ff0[];
extern u8 Data_00000ff1[];
extern u8 Data_00000ff2[];

#define FieldScene_RunScene37aSequenceF Func_02000488

/*
 * resource_37a owner 0x02000488..0x020009f3 (0x56c = 1388 bytes).
 *
 * Reviewed scene script for the resource_37a sequence guarded by story flag
 * 0x809.  One straight command list: it opens the scene, places and drives
 * four actor slots (0, 1, 5 and 0x10) through a long beat-by-beat schedule of
 * moves and waits, plays two audio cues, branches once on a runtime query
 * near the end to choose between two message ids, then hands the three
 * principal slots back to their walking records, sets story flags 0x144 and
 * 0x809 and closes the scene.
 *
 * Complete owner.  Prologue `push {lr}` at 0x02000488; the single epilogue
 * `pop {r0} / bx r0` at 0x020009da pops the return address, so the owner is
 * void and takes no argument.  Code runs 0x02000488..0x020009d9 with two
 * interior literal pools (0x0200081c..0x0200084f and 0x020009de..0x020009f3);
 * the next prologue is at 0x020009f4.  All 140 `bl` sites are transcribed.
 *
 * Calls are spelled as the pre-relocation call word the image holds, the
 * convention the adopted sibling
 * games/gs1/src/overlays/scene_primary_script/run_scene_37a_sequence_a.c uses,
 * so one target reached from several sites appears under several spellings.
 *
 * RESIDUAL (all 32 remaining differing halfwords).  Eight of those spellings
 * collide: two sites whose different call words happen to decode to the same
 * address (Func_0200316c, Func_02003234, Func_0200326c, Func_02003330,
 * Func_02003352, Func_02003382, Func_020033b2 and Func_02003390).
 * Separating them needs an `absolute_symbols` entry in the translation-unit
 * manifest, which a single-file draft may not add, so all sixteen of those
 * sites are spelled with the main-image name of the service they reach.
 * Those calls are semantically right; they emit the direct long call instead
 * of this overlay's veneer word, and nothing else about them differs.
 *
 * The four ids handed to the Func_0808a170 message service are spelled as
 * `Data_00000<id>` externs, the convention the adopted overlay scene scripts
 * already use for this family (see
 * games/gs1/src/overlays/scene_primary_script/run_scene_372_sequence_a.c,
 * which declares `extern u8 Data_00000e67[]` and casts it to s32 at the call).
 * The reference forces that reading for 0xff0: every other pooled constant in
 * this owner (0x101, 0x105, 0x2005, 0x6666 ...) is one GCC cannot synthesize,
 * while 0xff0 is 0xff << 4 and from a plain integer literal would come out as
 * `movs r0,#255 / lsls r0,r0,#4`.  The reference loads it from the pool
 * instead, so the operand was address-shaped in the original source.  The
 * other three ids are byte-identical either way; they are spelled the same
 * way because they reach the same service.
 *
 * Uncertain: none of the main-image services is established beyond its
 * argument count and access widths, so all are declared old-style and their
 * constants are transcribed rather than interpreted.  The record returned by
 * the Func_0808a080 service is read at +8/+16 as words early in the scene and
 * at +10/+18 as signed halfwords at the end, so both stay raw offsets; no
 * shared type in games/gs1/include covers it.
 */

/* Each declaration's comment names the owner or main-image service the call
 * word reaches. */
void Func_020004ea();  /* FieldScene_RunScene37aSequenceA (0x02000054) */
void Func_02002b36();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002bf8();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002c0c();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002c5a();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002c76();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002c9a();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002d22();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002d34();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002d44();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002d78();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002dbc();  /* SetSolShindenActorStep (0x020025fc) */
s32  Func_02002e42();  /* Func_080770c0 */
s32  Func_02002e50();  /* Func_080770c0 */
void Func_02002e7a();  /* Func_0808a018 */
void Func_02002e90();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002ea6();  /* Func_0808a010 */
void Func_02002eac();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002eb4();  /* Func_0808a090 */
void Func_02002ebc();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002ed8();  /* Func_0808a0d0 */
void Func_02002eda();  /* SetSolShindenActorStep (0x020025fc) */
s32  Func_02002ede();  /* Func_0808a080 */
void Func_02002ee8();  /* Func_0808a010 */
void Func_02002ef8();  /* Func_0808a100 */
void Func_02002efe();  /* Func_0808a090 */
void Func_02002f00();  /* Func_0808a170 */
void Func_02002f08();  /* SetSolShindenActorStep (0x020025fc) */
void Func_02002f1c();  /* Func_0808a0f0 */
void Func_02002f22();  /* Func_0808a0d0 */
s32  Func_02002f2c();  /* Func_0808a080 */
s32  Func_02002f40();  /* Func_0808a080 */
void Func_02002f60();  /* Func_0808a1b8 */
void Func_02002f64();  /* Func_0808a090 */
void Func_02002f66();  /* Func_0808a128 */
void Func_02002f6a();  /* Func_0808a0f0 */
void Func_02002f72();  /* Func_0808a090 */
void Func_02002f7e();  /* Func_0808a0f0 */
void Func_02002f8e();  /* Func_0808a0c8 */
void Func_02002fa2();  /* Func_0808a0d0 */
void Func_02002fa4();  /* Func_0808a1e8 */
void Func_02002fa6();  /* Func_080f9010 */
void Func_02002fb8();  /* Func_0808a010 */
void Func_02002fc2();  /* Func_0808a100 */
void Func_02002fce();  /* Func_0808a090 */
void Func_02002fd4();  /* Func_080f9010 */
void Func_02002ff2();  /* Func_0808a0d0 */
void Func_02002ff4();  /* Func_0808a010 */
void Func_0200300e();  /* Func_0808a010 */
void Func_0200301e();  /* Func_0808a1b8 */
void Func_0200302a();  /* Func_0808a1b8 */
void Func_02003030();  /* Func_0808a138 */
void Func_0200304e();  /* Func_0808a110 */
void Func_02003052();  /* Func_0808a208 */
void Func_02003056();  /* Func_0808a010 */
void Func_02003068();  /* Func_0808a110 */
void Func_0200306c();  /* Func_0808a210 */
void Func_02003078();  /* Func_0808a010 */
void Func_0200307c();  /* Func_0808a1b8 */
void Func_02003086();  /* Func_0808a138 */
void Func_0200308e();  /* Func_0808a1b8 */
void Func_02003094();  /* Func_0808a010 */
void Func_020030a2();  /* Func_0808a138 */
void Func_020030a8();  /* Func_0808a1b8 */
void Func_020030ca();  /* Func_0808a1b8 */
void Func_020030dc();  /* Func_0808a010 */
void Func_020030e2();  /* Func_0808a138 */
void Func_020030e6();  /* Func_0808a1b8 */
void Func_020030ea();  /* Func_0808a010 */
void Func_02003108();  /* Func_0808a1f0 */
void Func_0200310a();  /* Func_0808a1b8 */
void Func_0200312c();  /* Func_0808a1b8 */
void Func_02003140();  /* Func_0808a010 */
void Func_02003144();  /* Func_0808a110 */
void Func_02003146();  /* Func_0808a1f0 */
void Func_02003148();  /* Func_0808a1b8 */
void Func_02003154();  /* Func_0808a1b8 */
void Func_02003156();  /* Func_0808a010 */
void Func_02003160();  /* Func_0808a1b8 */
void Func_0200316e();  /* Func_0808a180 */
void Func_0200317e();  /* Func_0808a1e8 */
void Func_020031aa();  /* Func_0808a138 */
void Func_020031ac();  /* Func_0808a1e8 */
void Func_020031b0();  /* Func_0808a110 */
void Func_020031ca();  /* Func_0808a010 */
void Func_020031ce();  /* Func_0808a1e8 */
void Func_020031d8();  /* Func_0808a1e8 */
void Func_020031e2();  /* Func_0808a1e8 */
s32  Func_020031f2();  /* Func_0808a070 */
void Func_02003212();  /* Func_0808a1e8 */
void Func_0200321e();  /* Func_0808a1e8 */
void Func_0200322a();  /* Func_0808a1e8 */
void Func_02003240();  /* Func_0808a1b8 */
void Func_0200324c();  /* Func_0808a1b8 */
void Func_02003258();  /* Func_0808a1b8 */
s32  Func_02003262();  /* Func_0808a178 */
void Func_02003264();  /* Func_0808a1b8 */
void Func_0200329c();  /* Func_0808a128 */
void Func_020032a8();  /* Func_0808a170 */
void Func_020032c8();  /* Func_0808a010 */
void Func_020032d4();  /* Func_0808a110 */
void Func_020032da();  /* Func_0808a1e8 */
void Func_020032e4();  /* Func_0808a110 */
void Func_020032e8();  /* Func_0808a1b8 */
void Func_020032ee();  /* Func_0808a170 */
void Func_020032f4();  /* Func_0808a1b8 */
void Func_020032fe();  /* Func_0808a128 */
void Func_02003300();  /* Func_0808a1b8 */
void Func_0200330a();  /* Func_0808a100 */
void Func_0200330e();  /* Func_0808a090 */
void Func_02003312();  /* Func_0808a100 */
void Func_0200331c();  /* Func_0808a090 */
void Func_02003322();  /* Func_0808a110 */
void Func_0200332a();  /* Func_0808a090 */
s32  Func_02003360();  /* Func_0808a080 */
void Func_02003362();  /* Func_0808a100 */
void Func_02003370();  /* Func_0808a0e8 */
void Func_0200338a();  /* Func_080770c8 */
void Func_02003392();  /* Func_0808a100 */
void Func_020033a0();  /* Func_0808a0e8 */
void Func_020033b4();  /* Func_0808a020 */
void Func_020033c2();  /* Func_0808a100 */
void Func_020033d0();  /* Func_0808a0e8 */
void Func_020033e2();  /* Func_0808a0f0 */

/* The sixteen colliding sites reach these main-image services directly. */
void Func_080770c8();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_0808a010();  /* main-image name; collision fallback, see the RESIDUAL note */
s32  Func_0808a080();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_0808a0b8();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_0808a0f0();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_0808a110();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_0808a138();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_0808a170();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_0808a1b8();  /* main-image name; collision fallback, see the RESIDUAL note */
void Func_0808a1e8();  /* main-image name; collision fallback, see the RESIDUAL note */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block,
 * which the reference's bare `push {lr}` frame shows did not happen.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void FieldScene_RunScene37aSequenceF(void)
{
    u8 *rec;

    if (Value1(Func_02002e42, 0x814) != 0) {
        Func_020004ea();
    }
    if (Value1(Func_02002e50, 0x809) == 0) {
        Func_02002e7a();
        Call1(Func_02002f00, (s32)Data_00000fe3);
        Func_02002fa6(17);
        Call3(Func_02002eb4, 0, 0x10000, 0x8000);
        Call3(Func_02002ed8, 0, 0x120, 232);
        Func_02002ef8(0, 0);
        Func_02002ea6(20);
        Func_02002fd4(21);
        Call3(Func_02002f60, 0, 0xc000, 0);
        rec = (u8 *)Value1(Func_02002ede, 0);
        if (rec != 0) {
            Func_02002f1c(16, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Call3(Func_02002efe, 16, 0x16666, 0xb333);
        Call3(Func_02002f22, 16, 0x120, 206);
        Func_02002ee8(40);
        Call3(Func_02002fa4, 16, 0x100, 0);
        Func_02002f66(16, 4, 60);
        Func_02002b36(16, 20);
        rec = (u8 *)Value1(Func_02002f2c, 0);
        if (rec != 0) {
            Func_02002f6a(1, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        rec = (u8 *)Value1(Func_02002f40, 0);
        if (rec != 0) {
            Func_02002f7e(5, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Call3(Func_02002f64, 1, 0x8000, 0x4000);
        Call3(Func_02002f72, 5, 0x8000, 0x4000);
        Call3(Func_02002f8e, 1, 0x118, 248);
        Call3(Func_02002fa2, 5, 0x128, 248);
        Func_02002fc2(1, 1);
        Call3(Func_0200301e, 1, 0xd000, 0);
        Call3(Func_0200302a, 5, 0xb000, 30);
        Call2(Func_02003052, 0x9999, 0x1333);
        Call4(Func_0200306c, 0x1200000, -1, 0xd50000, 1);
        Call3(Func_02002fce, 16, 0x6666, 0x3333);
        Call3(Func_02002ff2, 16, 0x120, 176);
        Func_02002fb8(40);
        Func_02003030(16, 2);
        Func_02002bf8(16, 6);
        Call3(Func_0200307c, 16, 0x4000, 60);
        Func_02002c0c(16, 20);
        Func_0200308e(16, 0, 40);
        Func_0200304e(16, 3);
        Func_02002ff4(10);
        Call3(Func_020030a8, 16, 0x8000, 40);
        Func_02003068(16, 3);
        Func_0200300e(20);
        Func_02003086(5, 2);
        Call3(Func_020030ca, 5, 0x9000, 10);
        Func_02002c5a(5, 10);
        Func_020030a2(1, 2);
        Call3(Func_020030e6, 1, 0xf000, 10);
        Func_02002c76(1, 6);
        Call2(Func_02003108, 5, 0x102);
        Func_02003056(40);
        Call3(Func_0200310a, 5, 0xa000, 10);
        Call2(Func_02002c9a, 0x2005, 10);
        Func_020030e2(16, 2);
        Func_02003078(10);
        Call3(Func_0200312c, 16, 0xa000, 20);
        Call2(Func_02003146, 16, 0x102);
        Func_02003094(20);
        Call3(Func_02003148, 0, 0x5000, 40);
        Call3(Func_02003154, 0, 0xc000, 0);
        Call3(Func_02003160, 1, 0xe000, 0);
        Call3(Func_0808a1b8, 5, 0xa000, 40);
        Call3(Func_0200317e, 1, 0x101, 20);
        Func_0200316e(1, 0);
        Func_020030dc(60);
        Func_02003144(16, 4);
        Func_020030ea(40);
        Func_02002d22(16, 20);
        Call3(Func_020031ac, 5, 0x101, 40);
        Func_02002d34(5, 60);
        Func_0808a110(16, 3);
        Func_02002d44(16, 10);
        Call3(Func_020031ce, 0, 0x105, 0);
        Call3(Func_020031d8, 1, 0x105, 0);
        Call3(Func_020031e2, 5, 0x105, 60);
        Func_020031aa(1, 2);
        Func_02003140(20);
        Func_02002d78(1, 10);
        Func_020031b0(16, 3);
        Func_02003156(20);
        Call3(Func_02003212, 0, 0x102, 0);
        Call3(Func_0200321e, 1, 0x102, 0);
        Call3(Func_0200322a, 5, 0x102, 80);
        Call3(Func_0808a1e8, 16, 0x105, 80);
        Func_02002dbc(16, 6);
        Call3(Func_02003240, 0, 0x4000, 0);
        Call3(Func_0200324c, 1, 0xf000, 0);
        Call3(Func_02003258, 5, 0x9000, 60);
        Call3(Func_02003264, 16, 0x4000, 10);
        Func_0808a138(16, 3);
        Func_020031ca(6);
        Value2(Func_02003262, 16, 0);
        if (Value2(Func_020031f2, 0, 0) == 0) {
            Func_0808a170((s32)Data_00000ff0);
        } else {
            Call1(Func_020032a8, (s32)Data_00000ff1);
            Call3(Func_020032da, 16, 0x107, 20);
        }
        Func_0200329c(16, 4, 20);
        Call3(Func_020032e8, 0, 0xc000, 0);
        Call3(Func_020032f4, 1, 0xe000, 0);
        Call3(Func_02003300, 5, 0xa000, 0);
        Func_02002e90(16, 6);
        Call1(Func_020032ee, (s32)Data_00000ff2);
        Func_0808a010(30);
        Func_020032d4(5, 4);
        Call2(Func_02002eac, 0x2005, 6);
        Func_020032e4(1, 3);
        Func_02002ebc(1, 20);
        Func_020032fe(16, 6, 20);
        Call3(Func_0808a1e8, 16, 0x104, 20);
        Func_02002eda(16, 30);
        Func_0200330a(0, 3);
        Func_02003312(1, 3);
        Func_02003322(5, 3);
        Func_020032c8(20);
        Func_0808a110(16, 3);
        Func_02002f08(16, 6);
        Call3(Func_0200330e, 1, 0x10000, 0x8000);
        Call3(Func_0200331c, 5, 0x10000, 0x8000);
        Call3(Func_0200332a, 16, 0x20000, 0x10000);
        Func_02003362(16, 2);
        rec = (u8 *)Value1(Func_0808a080, 0);
        if (rec != 0) {
            Func_0808a0b8(16, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_02003370(16);
        Func_0808a0f0(16, 0, 0);
        Func_02003392(1, 2);
        rec = (u8 *)Value1(Func_02003360, 0);
        if (rec != 0) {
            Func_0808a0b8(1, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_020033a0(1);
        Func_0808a0f0(1, 0, 0);
        Func_020033c2(5, 2);
        rec = (u8 *)Value1(Func_0808a080, 0);
        if (rec != 0) {
            Func_0808a0b8(5, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Func_020033d0(5);
        Func_020033e2(5, 0, 0);
        Call1(Func_0200338a, 0x144);
        Call1(Func_080770c8, 0x809);
        Func_020033b4();
    }
}
