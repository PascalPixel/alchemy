#include "types.h"

#define Scene_RunPartySequence Func_02001494
#define SceneData_SelectTable9bd4ByState Func_02000030
#define SceneData_ReturnZero Func_02000060
#define SceneData_GetTable9d9c Func_02000064
#define SceneData_SelectTable9ddcByStateWithInit Func_0200006c
#define SceneData_SelectTable9f2cByState Func_020000a4
#define SceneDialogue_RunActor13Message1b83 Func_020000d4
#define SceneDialogue_RunActor16Message1b88 Func_020000f4
#define FieldScene_RunActorEightTurnDialogue Func_02000114
#define FieldScene_RunScene3aa_02000184 Func_02000184
#define SceneState_SetWord448To209AndRun Func_02000230
#define FieldScene_DispatchSceneByIndex Func_02000264
#define RunEventScript01 Func_02000360
#define ConfigureFourSceneChannelsAndHandoff Func_0200140c

extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009c04[];
extern u8 Data_02009bd4[];
extern u8 Data_02009df4[];
extern u8 Data_02009ddc[];
extern u8 Data_02009f38[];
extern u8 Data_02009f2c[];
extern u8 Data_02009b94[];

void Func_02001b1a();
void Func_02001b5c(void);
void Func_02001bea(s32);
s32 Func_02001c12(s32, s32);
void Func_02001b76(void);
void Func_02001b7c(void);
void Func_02001c0a(s32);
s32 Func_02001c32(s32, s32);
void Func_02001b96(void);
void Func_02001b9c(void);
s32 Func_02001c60(s32, s32, s32);
void Func_02001c36(s32);
void Func_02001c58(s32, s32, s32);
void Func_02001c40(s32, s32);
void Func_02001c6a(s32, s32, s32);
void Func_02001c42(s32, s32);
void Func_02001c7c(s32, s32, s32);
void Func_02001c54(s32, s32);
s32 Func_02001c8e(s32, s32, s32);
void Func_02001bdc(s32);
void Func_02001c06();
void Func_02001c0e();
void Func_02001c10();
void Func_02001c3c();
void Func_02001c6c();
void Func_02001c72();
void Func_02001c7a();
void Func_02001c80();
void Func_02001cae();
void Func_02001cc2();
void Func_02001ccc();
void Func_02001cd2();
void Func_02001d32();
void Func_02001d3a();
void Func_02001d6e();
void Func_020004ae(void);
void Func_02001cc0();
void Func_02001cc6();
s32 Func_02001cfc();
u8 *Func_02001d46();
s32 Func_02001d12();
void Func_0200174a();
void Func_02001da6();
s32 Func_02001d2c();
void Func_02001dbc(s32, s32, s32);
u8 *Func_02001d82();
s32 Func_02001d50();
void Func_02001d48(s32, s32, s32, s32, s32, s32);
void Func_02001d74(s32, s32, s32, s32, s32, s32);
void Func_02001d58();
void Func_02001d58_handoff(s32);
void Func_02001d56();
void Func_02001e12();
s32 Func_02001d98();
void Func_0200069c();
void Func_0200191e();
void Func_02001b74();
void Func_02001bbe();
void Func_02001db0();
void Func_02001dcc();
void Func_02001de8();
void Func_02001e5e();
void Func_02001e60();
void Func_02001ea2();
void Func_02001eb0();
void Func_02001eb4();
void Func_02001ebe();
void Func_02001eca();
void Func_02001eca_a();
void Func_02001ecc();
void Func_02001eda();
void Func_02001ee0();
void Func_02001ee6();
void Func_02001ef0();
void Func_02001ef0_a();
void Func_02001ef2();
void Func_02001f06();
void Func_02001f3c();
void Func_02001f44();
void Func_02001f44_a();
void Func_02001f4e();
void Func_02001f52();
void Func_02001f5a();
void Func_02001f62();
void Func_02001f74();
void Func_02001f7c();
void Func_02001f7e();
void Func_02001f8e();
void Func_02001f90();
void Func_02001f9e();
void Func_02001fa4();
void Func_02001fa6();
void Func_02001faa();
void Func_02001fb2();
void Func_02001fb6();
void Func_02001fb6_a();
void Func_02001fb6_b();
void Func_02001fd6();
void Func_02001fda();
void Func_02001fda_a();
void Func_02001fdc();
void Func_02001fe6();
void Func_02001fee();
void Func_02001ffc();
void Func_0200200a();
void Func_0200201c();
void Func_02002026();
void Func_0200202c();
void Func_02002042();
void Func_02002048();
void Func_02002054();
void Func_02002066();
void Func_02002068();
void Func_02002074();
void Func_0200207c();
void Func_02002080();
void Func_02002084();
void Func_02002084_a();
void Func_020020a2();
void Func_020020b4();
void Func_020020c2();
void Func_020020cc();
void Func_020020ce();
void Func_020020e4();
void Func_020020ec();
void Func_020020f6();
void Func_020020fe();
void Func_02002100();
void Func_02002106();
void Func_02002108();
void Func_02002114();
void Func_0200211e();
void Func_02002124();
void Func_02002130();
void Func_0200213e();
void Func_02002142();
void Func_02002154();
void Func_02002156();
void Func_02002158();
void Func_0200215e();
void Func_02002162();
void Func_0200216c();
void Func_02002170();
void Func_0200217a();
void Func_0200217e();
void Func_02002188();
void Func_02002192();
void Func_0200219a();
void Func_0200219c();
void Func_020021b8();
void Func_020021ba();
void Func_020021c8();
void Func_020021d8();
s32 Func_020021de();
s32 Func_020021de_a();
void Func_020021e2();
void Func_020021e4();
void Func_020021f0();
void Func_020021f0_a();
void Func_02002200();
void Func_0200220c();
void Func_02002212();
void Func_0200221a();
void Func_0200222a();
void Func_0200222a_a();
void Func_02002246();
void Func_0200224c();
void Func_02002258();
void Func_02002262();
void Func_02002266();
void Func_02002274();
void Func_0200228c();
void Func_0200229e();
void Func_020022c2();
void Func_020022ee();
void Func_02002300();
void Func_02002300_a();
void Func_02002302();
void Func_02002304();
void Func_02002310();
void Func_0200231a();
void Func_0200231c();
void Func_02002324();
void Func_02002328();
void Func_0200232a();
void Func_02002336();
void Func_0200233c();
void Func_0200234e();
void Func_02002372();
void Func_0200237e();
void Func_0200238a();
void Func_02002392();
void Func_02002396();
void Func_0200239c();
void Func_020023a2();
void Func_020023a6();
void Func_020023ac();
void Func_020023ae();
void Func_020023b4();
void Func_020023ba();
void Func_020023bc();
void Func_020023c4();
void Func_020023c6();
void Func_020023cc();
void Func_020023de();
void Func_020023e0();
void Func_020023fc();
void Func_020023fc_a();
void Func_02002410();
void Func_02002414();
void Func_02002418();
void Func_02002424();
void Func_02002426();
s32 Func_02002428();
void Func_02002436();
void Func_0200243c();
void Func_02002444();
void Func_02002454();
void Func_0200245e();
void Func_02002466();
void Func_0200246e();
void Func_0200247e();
s32 Func_0200248a();
void Func_020024a2();
void Func_020024b6();
void Func_020024ce();
void Func_020024fc();
void Func_02002516();
void Func_0200251c();
void Func_0200251c_a();
void Func_0200251e();
void Func_02002522();
void Func_0200252e();
s32 Func_02002532();
void Func_02002534();
void Func_0200253a();
void Func_02002554();
void Func_02002556();
void Func_02002558();
void Func_02002564();
void Func_0200256a();
void Func_02002570();
s32 Func_0200257e();
s32 Func_0200257e_a();
void Func_0200258c();
void Func_0200259c();
void Func_020025aa();
void Func_020025b6();
void Func_020025c2();
void Func_020025d4();
void Func_020025d6();
void Func_020025e8();
void Func_0200261c();
void Func_02002622();
void Func_0200263e();
void Func_02002646();
void Func_0200264a();
void Func_02002650();
void Func_02002656();
void Func_02002658();
void Func_0200265c();
void Func_02002662();
void Func_0200266a();
void Func_02002670();
void Func_0200267e();
void Func_02002682();
void Func_0200268a();
void Func_020026ae();
void Func_020026b2();
void Func_020026b8();
void Func_020026c4();
void Func_020026cc();
void Func_020026de();
void Func_020026e4();
void Func_020026ec();
void Func_020026f2();
void Func_020026f6();
void Func_02002714();
void Func_0200271a();
void Func_02002720();
void Func_02002730();
void Func_02002732();
s32 Func_02002736();
void Func_02002738();
void Func_02002742();
void Func_0200275a();
void Func_0200275c();
void Func_02002762();
void Func_02002766();
void Func_02002768();
s32 Func_0200276a();
s32 Func_0200276a_a();
void Func_02002770();
void Func_02002772();
void Func_0200277a();
void Func_0200279a();
void Func_020027ae();
void Func_020027ba();
void Func_020027c6();
void Func_020027e2();
void Func_020027f2();
void Func_020027f8();
void Func_0200281a();
void Func_02002832();
void Func_0200284c();
void Func_0200285a();
void Func_0200285c();
void Func_02002862();
void Func_0200286e();
void Func_02002872();
void Func_02002886();
void Func_02002892();
void Func_02002894();
void Func_020028be();
void Func_020028c6();
void Func_020028c8();
void Func_020028c8_a();
void Func_020028cc();
void Func_020028d2();
void Func_020028da();
void Func_020028dc();
void Func_020028e8();
void Func_020028f4();
void Func_020028fc();
void Func_02002900();
void Func_02002902();
void Func_02002904();
void Func_0200290e();
void Func_02002914();
void Func_02002916();
void Func_0200291e();
void Func_02002926();
void Func_0200292e();
void Func_0200293e();
void Func_02002956();
void Func_02002978();
void Func_0200297e();
void Func_02002980();
void Func_02002988();
void Func_02002990();
void Func_020029a8();
void Func_020029ba();
void Func_020029bc();
void Func_020029ce();
void Func_020029ce_a();
void Func_020029dc();
void Func_020029e4();
void Func_020029f2();
void Func_020029f6();
void Func_02002a00();
void Func_02002a08();
void Func_02002a1a();
void Func_02002a1c();
void Func_02002a22();
void Func_02002a2a();
void Func_02002a38();
void Func_02002a3a();
void Func_02002a44();
void Func_02002a54();
void Func_02002a68();
void Func_02002a6e();
void Func_02002a76();
void Func_02002a84();
void Func_02002a86();
void Func_02002a8c();
void Func_02002aa4();
void Func_02002aaa();
void Func_02002ab6();
void Func_02002abe();
void Func_02002acc();
void Func_02002ace();
void Func_02002ad8();
void Func_02002ad8_a();
void Func_02002ada();
void Func_02002ade();
void Func_02002ae6();
void Func_02002aea();
void Func_02002af2();
void Func_02002af6();
void Func_02002afa();
void Func_02002b08();
void Func_02002b0a();
void Func_02002b1a();
void Func_02002b22();
void Func_02002b3e();
void Func_02002b4c();
void Func_02002b58();
void Func_02002b68();
void Func_02002b70();
void Func_02002b76();
void Func_02002b76_a();
void Func_02002b78();
void Func_02002b80();
void Func_02002ba0();
void Func_02002bb2();
void Func_02002bb4();
void Func_02002bbc();
void Func_02002bc6();
void Func_02002bc8();
void Func_02002bd4();
void Func_02002bdc();
void Func_02002be2();
void Func_02002bf6();
void Func_02002c08();
void Func_02002c0a();
void Func_02002c24();
void Func_02002c30();
void Func_02002c32();
void Func_02002c4c();
void Func_02002c4c_a();
void Func_02002c56();
void Func_02002c5a();
void Func_02002c60();
void Func_02002c68();
void Func_02002c6c();
void Func_02002c6c_a();
void Func_02002c7a();
void Func_02002c7a_a();
void Func_02002c7e();
s32 Func_02002c82();
void Func_02002c90();
void Func_02002c90_a();
void Func_02002c94();
void Func_02002ca2();
void Func_02002cb2();
void Func_02002cb6();
void Func_02002cbc();
s32 Func_02002cce();
s32 Func_02002cce_a();
void Func_02002cea();
void Func_02002cf2();
void Func_02002cf8();
void Func_02002cfa();
void Func_02002d02();
void Func_02002d06();
void Func_02002d12();
void Func_02002d2e();
void Func_02002d3c();
void Func_02002d42();
void Func_02002d42_a();
void Func_02002d52();
void Func_02002d56();
void Func_02002d5a();
void Func_02002d64();
void Func_02002d6a();
void Func_02002d7c();
void Func_02002d82();
void Func_02002daa();
void Func_02002db0();
void Func_02002dbc();
void Func_02002dbe();
void Func_02002dc4();
void Func_02002dd2();
void Func_02002dd8();
void Func_02002dd8_a();
void Func_02002de4();
void Func_02002de6();
void Func_02002dee();
void Func_02002df8();
void Func_02002dfe();
void Func_02002e00();
void Func_02002e00_a();
void Func_02002e0e();
void Func_02002e1e();
void Func_02002e30();
void Func_02002e3a();
void Func_02002e3e();
void Func_02002e4c();
void Func_02002e52();
void Func_02002e52_a();
void Func_02002e58();
void Func_02002e5a();
void Func_02002e5a_a();
void Func_02002e5a_b();
void Func_02002e62();
void Func_02002e6c();
void Func_02002e72();
s32 Func_02002e74();
void Func_02002e7c();
s32 Func_02002e84();
void Func_02002f4e(s32, s32, s32);
void Func_02002f5a(s32, s32, s32);
void Func_02002f66(s32, s32, s32);
void Func_02002f72(s32, s32, s32);
void Func_02002ec4(s32);
void Func_02002f92(s32, s32, s32);
void Func_02002f9e(s32, s32, s32);
void Func_02002faa(s32, s32, s32);
void Func_02002fb6(s32, s32, s32);
void Func_02002f08(s32);
void Func_02002a2e();
void Func_02002d34();
void Func_02002ee4();
void Func_02002f00();
void Func_02002f02();
void Func_02002f1c();
void Func_02002fbc();
void Func_02002fc6();
void Func_02002fd0();
void Func_02002fda();
void Func_02002fe4();
s32 Func_02002ff0();
void Func_02002ffe();
s32 Func_02003004();
void Func_02003012();
void Func_02003016();
s32 Func_02003018();
void Func_0200303e();
void Func_0200304a();
void Func_02003052();
void Func_02003054();
void Func_02003062();
void Func_02003064();
void Func_02003066();
void Func_02003070();
void Func_02003072();
void Func_02003086();
void Func_020030ae();
void Func_020030b6();
void Func_020030be();
void Func_020030e2();
void Func_02003106();
void Func_02003124();
void Func_02003128();
void Func_02003148();
void Func_0200314a();
void Func_0200314e();
void Func_0200315c();
void Func_02003160();
void Func_0200316a();
void Func_02003170();
s32 Func_02003176();
void Func_02003186();
void Func_02003190();
void Func_0200319a();
void Func_020031a4();
void Func_020031ac();
void Func_020031ae();
void Func_020031b2();
void Func_020031b6();
void Func_020031be();
void Func_020031c8();
void Func_020031ce();
void Func_020031d8();
void Func_020031dc();
void Func_020031e0();
s32 Func_020031e4();
void Func_020031f6();
void Func_020031fc();
void Func_0200322e();
void Func_0200323a();
void Func_0200326e();
void Func_02003270();
void Func_02003280();
void Func_0200328c();
void Func_02003298();
void Func_0200329e();
void Func_020032b8();
void Func_020032ba();
void Func_020032be();
void Func_020032c0();
void Func_020032c2();
void Func_020032ca();
void Func_020032ce();
void Func_020032e0();
void Func_020032e6();
void Func_020032ea();
void Func_020032f2();
void Func_020032f4();
void Func_020032fc();
void Func_02003310();
void Func_02003312();
void Func_02003324();
void Func_0200333a();
void Func_02003340();
void Func_02003342();
void Func_02003356();
void Func_0200335a();
void Func_02003368();
s32 Func_02003382();
void Func_02003392();
void Func_020033c2();
void Func_020033d2();
void Func_020033f0();
void Func_020033f2();
void Func_02003402();
void Func_02003410();
void Func_02003418();
void Func_0200341a();
void Func_0200342c();
void Func_02003446();
void Func_02003448();
void Func_0200344a();
void Func_0200345e();
void Func_02003464();
void Func_02003466();
void Func_0200346c();
void Func_02003476();
void Func_02003478();
void Func_0200347c();
void Func_02003482();
void Func_02003488();
void Func_0200348a();
void Func_0200348e();
s32 Func_02003492();
void Func_0200349a();
void Func_020034a0();
void Func_020034a2();
void Func_020034aa();
void Func_020034b2();
void Func_020034ba();
void Func_020034ca();
void Func_02003054_a();
void Func_02003128_a();
void Func_02003298_a();
s32 Func_02003312_a();
void Func_02003340_a();
void Func_02003340_b();
void Func_0200335a_a();
void Func_02003448_a();
void Func_0200349a_a();
void Func_0200349a_b();
s32 Func_020034a2_a();

/* Contiguous unnamed leaf-owner run for resource_3aa. */

/*
 * resource_3aa owner at 0x02000064, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000068 holding 0x2009d9c.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000064 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009d9c is image offset
 * 0x1d9c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Contiguous unnamed state-owner run for resource_3aa. */

/*
 * resource_3aa owner at 0x02000264, 252 bytes: code 0x02000264-0x02000343 and
 * the seven-word literal pool at 0x02000344-0x0200035f.  The next inventory
 * row starts at 0x02000360, so the owner is exactly its advertised span; the
 * 8-byte outgoing-argument frame it opens with `sub sp, #8` is released by
 * `add sp, #8` before the return, so nothing live escapes.
 *
 * This is the overlay's scene dispatcher.  It re-enables two channels, then
 * switches on the signed halfword `Data_02000240[225]` - the neighbour of the
 * edition selector at index 224 that the byte-exact siblings
 * `games/gs1/asm/overlays/resource_3aa_c_02000030.c` and `_020000a4.c` test - and runs
 * one of three scene bodies.  Values other than 9, 10, 11 and 20 do nothing.
 *
 * The comparison chain is a compiled `switch`: `cmp #11 / bgt`, then
 * `cmp #10 / bge` for the 10-11 pair, then `cmp #9 / beq`, with 20 tested on
 * the greater-than side.  10 and 11 share one body.
 *
 * Return type, by the interworking-epilogue rule: `pop {r0} / bx r0` pops the
 * return address into r0, so the owner is void.  r0 is written with 1 before
 * the first branch, so the owner takes no arguments either.
 *
 * Call accounting: 18 `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3aa 0264` - 16 to import
 * veneers (10 distinct) and 2 to in-overlay prologues, at file offsets 0x1494
 * and 0x0360.  Every site appears below exactly once.  The disassembler's own
 * annotations are wrong in the usual overlay way and were not used.
 *
 * Import shapes, consistent with the already-converted overlays:
 *   GameFlag_IsSet(id)              -> queried predicate; its result is tested
 *                                     against zero at all five sites, so it is
 *                                     used purely as a condition here.
 *   Scene_GetRecord(selector)        -> rec pointer; the halfword at +6 of
 *                                     the returned rec is written straight
 *                                     after both calls (resource_373 and
 *                                     resource_39f fix the same shape).
 *   Func_0808a0f0(selector, x, z)  -> 16.16 position setter.
 *   Func_080091c0(a, b, c, d, e, f)-> the six-argument service whose spelling
 *                                     games/gs1/semantic/overlays/resource_39a already
 *                                     carries; the two stack words this owner
 *                                     stores at [sp,#0] and [sp,#4] before the
 *                                     0x02000302 and 0x02000316 branches are
 *                                     arguments five and six, so Func_08009180
 *                                     is called with the same six-argument
 *                                     shape.
 *
 * Uncertainties, recorded rather than guessed:
 *  - The pooled ids 0x941, 0x914, 0x321, 0x915 and 0x109 passed to
 *    GameFlag_IsSet are opaque here; only the sense of each test is recovered.
 *  - The halfword written at +6 of the Scene_GetRecord rec is 0x1000 in one
 *    arm and 0xd000 in the other, both built by shifting 0x80 / 0xd0.  Whether
 *    that field is an angle or a flag word is not established.
 *  - Func_0808a0f0(8, 0x038a0000, 0x01a60000) uses a pooled first coordinate
 *    and a shifted second (211 << 17); as 16.16 these are 906.0 and 211.0.
 *  - Both in-overlay branches (0x02001494 and 0x02000360) are taken with r0
 *    holding the zero result of the preceding GameFlag_IsSet, i.e. no argument
 *    is deliberately set, so they are called with none here.
 */

/* Imports, named by the main-image address in the trailing word of each
 * overlay veneer. Old-style declarations are mandatory in this overlay. */

/* Signed halfword table in RAM; index 225 selects the scene. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];
    void Func_02001c00();

    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];
    void Func_02001c00();

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Func_02002f5a(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Func_02002f66(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Func_02002f72(channel, value, zero);
}

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Func_02002f92(channel, value, zero);
}

static __inline__ void ConfigureUniformSecond(s32 channel, s32 value, s32 zero)
{
    Func_02002f9e(channel, value, zero);
}

static __inline__ void ConfigureUniformThird(s32 channel, s32 value, s32 zero)
{
    Func_02002faa(channel, value, zero);
}

static __inline__ void ConfigureUniformFourth(s32 channel, s32 value, s32 zero)
{
    Func_02002fb6(channel, value, zero);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 Data_03001ebc[];

    *(u16 *)(*(u8 **)Data_03001ebc + 0x1d8) += amount;
}

s32 SceneData_SelectTable9bd4ByState(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        return (s32)Data_02009c04;
    }
    return (s32)Data_02009bd4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable9d9c(void)
{
    return (u8 *)0x02009d9c;
}

s32 SceneData_SelectTable9ddcByStateWithInit(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        Func_02001b1a(Data_02009df4);
        return (s32)Data_02009df4;
    }
    return (s32)Data_02009ddc;
}

s32 SceneData_SelectTable9f2cByState(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        return (s32)Data_02009f38;
    }
    return (s32)Data_02009f2c;
}

void SceneDialogue_RunActor13Message1b83(void)
{
    Func_02001b5c();
    Func_02001bea(0x1B83);
    Func_02001c12(13, 0);
    Func_02001b76();
}

void SceneDialogue_RunActor16Message1b88(void)
{
    Func_02001b7c();
    Func_02001c0a(0x1B88);
    Func_02001c32(16, 0);
    Func_02001b96();
}

void FieldScene_RunActorEightTurnDialogue(void)
{
    void Func_02001c00_end(void);

    Func_02001b9c();
    Func_02001c60(8, 0x100, 0x3C);
    Func_02001c36(0x1B91);
    Func_02001c58(8, 0, 0xA);
    Func_02001c40(8, 2);
    Func_02001c6a(8, 0, 0xA);
    Func_02001c42(8, 4);
    Func_02001c7c(8, 0, 0xA);
    Func_02001c54(8, 3);
    Func_02001c8e(8, 0, 0xA);
    Func_02001bdc(0x913);
    Func_02001c00_end();
}

void FieldScene_RunScene3aa_02000184(void)
{
    extern u8 Data_03001ebc[];
    void Func_02001c00();

    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02001c10();
    Func_02001c0e(10);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 4) {
        Func_02001d32(188);
    } else {
        Func_02001d3a(158);
    }
    Func_02001c00(1);
    Func_02001c06(2);
    Func_02001c3c(10);
    Call3(Func_02001c7a, 0, 0x8000, 0x4000);
    Func_02001cc2(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 4) {
        Call3(Func_02001ccc, 0, 0, -16);
    } else {
        Call3(Func_02001cd2, 0, 3, -16);
    }
    Func_02001c80(16);
    Func_02001d6e(*(s16 *)(((s32)p5 + 0x16c)));
    Func_02001c6c(1);
    Func_02001c72(2);
    Func_02001cae();
}

s32 SceneState_SetWord448To209AndRun(void)
{
    extern u8 *Data_03001ebc;

    *(s32 *)(Data_03001ebc + 448) = 0x209;
    if (Data_02000240[224] == (s32) (u32) &Value_00000067) {
        Func_020004ae();
    }
    return 0;
}

void FieldScene_DispatchSceneByIndex(void)
{
    extern u8 *Data_03001ebc;

    u8 *rec;
    s32 h;
    s32 x1 = 0x038a0000;
    s32 z1 = 0x01a60000;

    Func_02001cc0(1);
    Func_02001cc6(2);

    switch (Data_02000240[225]) {
    case 9:
        if (Func_02001cfc(0x941) != 0) {
            rec = Func_02001d46(8);
            h = 0x1000;
            *(u16 *)(rec + 6) = h;

            if (Func_02001d12(0x914) == 0) {
                Func_0200174a();
            }
        } else {
            Func_02001da6(9, 0, 0);
            if (Func_02001d2c(0x321) != 0) {
                Func_02001dbc(8, x1, z1);
                rec = Func_02001d82(8);
                h = 0xd000;
                *(u16 *)(rec + 6) = h;
            }
        }
        break;

    case 10:
    case 11:
        if (Func_02001d50(0x915) != 0) {
            s32 a5 = 4;
            s32 a6 = 3;
            Func_02001d48(58, 70, 54, 70, a5, a6);
            {
                s32 b5 = 55;
                s32 b6 = 8;
                Func_02001d74(55, 9, 2, 1, b5, b6);
            }
            Func_02001d58();
            Func_02001d56(1);
        }
        break;

    case 20:
        Func_02001e12(9, 0, 0);
        if (Func_02001d98(0x109) == 0) {
            Func_0200069c();
        }
        break;

    default:
        break;
    }
}

void RunEventScript01(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Func_02001de8();
    Call4(Func_02001eca, -1, -1, -1, 0);
    Func_02001db0(1);
    Call2(Func_02001ef0, 0x10002, 0);
    Call2(Func_02001ef0_a, 0x10002, 0);
    Func_02001f06(1);
    Func_02001dcc(1);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 24;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x201;
    Call3(Func_02001ea2, 8, 0x3580000, 0x1b80000);
    Call3(Func_02001eb0, 0, 0x3580000, 0x1e60000);
    Call3(Func_02001ebe, 1, 0x3500000, 0x1f60000);
    Call3(Func_02001ecc, 2, 0x3680000, 0x1e60000);
    Call3(Func_02001eda, 3, 0x3700000, 0x1f60000);
    Call3(Func_02001ee6, 10, 0x3480000, 0x2060000);
    Call3(Func_02001ef2, 11, 0x3780000, 0x2060000);
    Call4(Func_02001f74, 0x3600000, -1, 0x1d80000, 0);
    Func_02001e60();
    Func_02001e5e(1);
    Func_02001faa();
    Func_02001fb6();
    Func_02001eb4(40);
    Func_02001f44(8, 1);
    Func_02001f3c(8, 3);
    Func_02001eca_a(10);
    Func_02001f5a(2, 1);
    Func_02001f52(2, 4);
    Func_02001ee0(20);
    Call3(Func_02001fa4, 2, 0x8000, 20);
    Func_02001f7c(0, 2);
    Func_02001fb6_a(0, 0, 10);
    Func_02001f7e(0, 3);
    Func_02001f8e(1, 1);
    Func_02001f9e(3, 1);
    Call3(Func_02001fda, 1, 0xe000, 0);
    Call3(Func_02001fe6, 3, 0xa000, 10);
    Func_02001fa6(1, 3);
    Func_02001fb6_b(3, 3);
    Func_02001f44_a(20);
    Func_0200191e(20);
    Call2(Func_0200201c, 8, 0x102);
    Func_02001fdc(8, 2);
    Func_02001f62(60);
    Call3(Func_02002026, 8, 0x5000, 10);
    Func_02001fee(8, 3);
    Call3(Func_02002042, 10, 0x100, 0);
    Func_0200200a(10, 2);
    Func_02001f90(60);
    Call3(Func_02002054, 10, 0xf000, 20);
    Func_0200202c(11, 2);
    Call3(Func_02002068, 11, 0x9000, 40);
    Call3(Func_02002074, 10, 0xd000, 0);
    Call3(Func_02002080, 11, 0xb000, 20);
    Func_02002048(11, 3);
    Func_02001fd6(10);
    Func_02002066(8, 2);
    Call3(Func_020020a2, 8, 0x3000, 10);
    Call3(Func_020020b4, 8, 0x105, 60);
    Func_02002084(11, 2);
    Func_0200207c(11, 4);
    Func_02002084_a(11, 4);
    Call2(Func_020020fe, 0x10000, 0);
    Func_02002114(40);
    Func_02001fda_a(60);
    Call3(Func_020020ec, 8, 0x105, 60);
    Call1(Func_020020c2, 0x1b21);
    Func_020020e4(8, 0, 10);
    Func_020020cc(10, 2);
    Func_020020f6(10, 0, 10);
    Func_020020ce(11, 4);
    Func_02002108(11, 0, 10);
    Call3(Func_02002124, 2, 0x6000, 10);
    Call3(Func_0200211e, 0x6002, 0, 10);
    Func_02002106(11, 2);
    Func_02002130(11, 0, 10);
    Func_02002100(10, 4);
    Func_02002142(10, 0, 10);
    Call3(Func_0200215e, 2, 0xc000, 10);
    Call3(Func_02002158, 0x2002, 0, 10);
    Call3(Func_0200217a, 8, 0x101, 60);
    Func_0200216c(8, 0, 10);
    Func_02002154(11, 2);
    Func_0200217e(11, 0, 10);
    Call3(Func_0200219a, 2, 0x2000, 10);
    Func_02002162(10, 3);
    Func_0200219c(10, 0, 10);
    Call3(Func_020021b8, 2, 0x6000, 10);
    Func_02002188(2, 1);
    Call3(Func_020021ba, 0x6002, 0, 10);
    Func_02002192(10, 3);
    Call3(Func_020021de, 2, 0x8000, 10);
    Value2(Func_020021de_a, 0x6002, 0);
    Call2(Func_02002200, 8, 0x102);
    Func_0200213e(60);
    Func_020021f0(8, 0, 10);
    Func_020021d8(2, 2);
    Func_02002156(10);
    Call3(Func_0200221a, 2, 0xc000, 20);
    Func_020021e2(2, 3);
    Func_02002170(20);
    Func_020021f0_a(1, 3);
    Func_0200222a(1, 0, 10);
    Call3(Func_0200224c, 8, 0x105, 60);
    Func_0200220c(3, 4);
    Func_02002246(3, 0, 40);
    Call3(Func_02002262, 8, 0x1000, 40);
    Call3(Func_020021e4, 8, 0x6666, 0x3333);
    Call3(Func_02002212, 8, 0x37c, 0x1b8);
    Func_020021c8(40);
    Call3(Func_0200228c, 8, 0xd000, 20);
    Call3(Func_0200229e, 8, 0x105, 60);
    Func_02001b74(60);
    Func_02001bbe(40);
    Call3(Func_02002274, 8, 0x358, 0x1b8);
    Func_0200222a_a(40);
    Call3(Func_020022ee, 8, 0x9000, 20);
    Call3(Func_02002300, 8, 0x105, 60);
    Call3(Func_02002304, 10, 0xf000, 0);
    Call3(Func_02002310, 11, 0x9000, 40);
    Call3(Func_0200231c, 10, 0xd000, 0);
    Call3(Func_02002328, 11, 0xb000, 10);
    Func_02002300_a(8, 1);
    Func_0200232a(8, 0, 20);
    Func_02002302(8, 4);
    Func_0200233c(8, 0, 20);
    Func_02002324(10, 2);
    Func_0200234e(10, 0, 10);
    Func_02002336(8, 2);
    Call3(Func_02002372, 8, 0x5000, 10);
    Call3(Func_0200237e, 0, 0x4000, 0);
    Call3(Func_0200238a, 1, 0x4000, 0);
    Call3(Func_02002396, 2, 0x6000, 0);
    Call3(Func_020023a2, 3, 0x6000, 0);
    Call3(Func_020023ae, 11, 0x9000, 40);
    Call3(Func_020023ba, 10, 0xf000, 20);
    Func_020023b4(10, 0, 20);
    Func_0200239c(11, 2);
    Func_0200231a(20);
    Call3(Func_020023de, 11, 0xb000, 20);
    Func_020023a6(11, 3);
    Func_020023e0(11, 0, 10);
    Call3(Func_020023fc, 11, 0x9000, 40);
    Func_020023bc(10, 3);
    Func_020023cc(11, 3);
    Call3(Func_02002418, 10, 0xd000, 0);
    Call3(Func_02002424, 11, 0xb000, 10);
    Func_020023fc_a(10, 2);
    Func_02002426(10, 0, 20);
    Func_02001d58_handoff(20);
    Func_02002414(8, 2);
    Func_02002392(40);
    Func_02002444(8, 0, 10);
    Call2(Func_0200246e, 8, 0x102);
    Func_020023ac(60);
    Func_0200245e(8, 0, 20);
    Func_02002436(8, 4);
    Func_020023c4(20);
    Func_0200243c(8, 4);
    Func_0200247e(8, 0, 20);
    Func_02002466(1, 2);
    Call3(Func_020024a2, 1, 0xe000, 20);
    Value2(Func_0200248a, 1, 0);
    Call3(Func_020024b6, 0, 0x6000, 0);
    v5 = 1;
    if (Value2(Func_02002428, 0, 0) == 1) {
        bump_step(1);
        v5 = 0;
    }
    Func_020024ce(1, 0, 10);
    if (v5 != 0) {
        bump_step(1);
    }
    Call3(Func_020024fc, 2, 0x8000, 10);
    Call2(Func_02002516, 2, 0x102);
    Func_02002454(60);
    Call3(Func_0200251e, 8, 0x101, 0);
    Call3(Func_02002522, 8, 0x3000, 40);
    Call3(Func_0200252e, 8, 0x5000, 20);
    Call3(Func_0200253a, 8, 0x3000, 20);
    Func_02002534(8, 0, 10);
    Func_0200251c(2, 1);
    Call3(Func_02002558, 2, 0xc000, 10);
    Call3(Func_02002564, 0, 0xc000, 0);
    Call3(Func_02002570, 1, 0xc000, 10);
    Call3(Func_0200256a, 0x2002, 0, 10);
    Call3(Func_0200258c, 8, 0x107, 60);
    ((void (*)())Func_0200257e)(8, 0, 10);
    Func_02002556(8, 4);
    Value2(Func_0200257e_a, 8, 0);
    Call3(Func_020025aa, 2, 0x8000, 0);
    Call3(Func_020025b6, 1, 0xe000, 0);
    Call3(Func_020025c2, 3, 0xa000, 0);
    if (Value2(Func_02002532, 0, 0) == 0) {
        Func_0200251c_a(20);
        Func_0200259c(8, 3);
        Func_020025d6(8, 0, 10);
        bump_step(1);
    } else {
        Func_02002554(20);
        Func_020025d4(8, 4);
        bump_step(1);
        Func_0200261c(8, 0, 10);
    }
    Func_02001f4e(20);
    Call3(Func_0200263e, 2, 0x2000, 0);
    Call3(Func_0200264a, 3, 0xa000, 40);
    Call3(Func_02002656, 2, 0xc000, 0);
    Call3(Func_02002662, 3, 0xc000, 10);
    Func_0200265c(3, 0, 20);
    Call3(Func_0200267e, 8, 0x105, 60);
    Func_02002670(8, 0, 40);
    Func_02002658(8, 2);
    Func_02002682(8, 0, 40);
    Func_0200266a(2, 1);
    Func_020025e8(20);
    Call3(Func_02002622, 2, 0xcccc, 0x6666);
    Call3(Func_02002650, 2, 0x364, 0x1d8);
    Call3(Func_020026b2, 0x2002, 0, 10);
    Func_0200268a(8, 4);
    Func_020026c4(8, 0, 10);
    Func_02001fb2(40);
    Func_02001ffc(20);
    Call3(Func_020026f2, 8, 0x105, 60);
    Func_020026e4(8, 0, 10);
    Func_020026cc(1, 2);
    Func_020026f6(1, 0, 10);
    Func_020026de(8, 1);
    Call3(Func_0200271a, 8, 0x5000, 10);
    Func_02002714(8, 0, 10);
    Func_020026ec(8, 3);
    Call3(Func_02002738, 8, 0x3000, 10);
    Func_02002732(8, 0, 10);
    Call2(Func_0200275c, 0, 0x102);
    Call2(Func_02002766, 1, 0x102);
    Call2(Func_02002770, 2, 0x102);
    Call2(Func_0200277a, 3, 0x102);
    Func_020026b8(40);
    Call3(Func_0200276a, 0x2002, 0, 10);
    Func_02002742(8, 3);
    Value2(Func_0200276a_a, 8, 0);
    Func_0200275a(1, 1);
    Func_02002762(2, 1);
    Func_02002772(3, 1);
    Call3(Func_020027ae, 1, 0xe000, 0);
    Call3(Func_020027ba, 2, 0x6000, 0);
    Call3(Func_020027c6, 3, 0xa000, 0);
    if (Value2(Func_02002736, 0, 0) == 0) {
        Func_02002720(20);
        Call2(Func_020027f2, 8, 0x102);
        Func_02002730(40);
        Func_020027e2(8, 0, 10);
        bump_step(2);
    } else {
        Func_02002768(20);
        Func_020027f8(1, 2);
        bump_step(1);
        Func_02002832(1, 0, 10);
        Call2(Func_0200285c, 8, 0x102);
        Func_0200279a(40);
        Func_0200284c(8, 0, 10);
    }
    Call3(Func_0200286e, 3, 0x101, 40);
    Call3(Func_02002872, 3, 0xc000, 10);
    Func_02002862(3, 0);
    Call3(Func_02002886, 1, 0xc000, 0);
    Call3(Func_02002892, 2, 0xc000, 20);
    Func_0200285a(8, 3);
    Func_02002894(8, 0, 10);
    Call2(Func_020028be, 0, 0x102);
    Call2(Func_020028c8, 1, 0x102);
    Call2(Func_020028d2, 2, 0x102);
    Call2(Func_020028dc, 3, 0x102);
    Func_0200281a(40);
    Func_020028cc(8, 0, 10);
    Call3(Func_020028e8, 0, 0xe000, 0);
    Call3(Func_020028f4, 1, 0xe000, 0);
    Call3(Func_02002900, 3, 0xa000, 10);
    Func_020028c8_a(2, 4);
    Call3(Func_02002902, 0x2002, 0, 10);
    Func_020028da(8, 3);
    Func_02002914(8, 0, 10);
    Func_020028fc(8, 1);
    Func_02002926(8, 0, 10);
    Func_02002258(10);
    Func_02002904(8, 4);
    Func_0200293e(8, 0, 20);
    Func_0200290e(0, 4);
    Func_02002916(1, 4);
    Func_0200291e(2, 4);
    Func_0200292e(3, 4);
    Call2(Func_02002988, 8, 0x102);
    Func_020028c6(60);
    Func_02002978(8, 0, 10);
    Func_02002266(40);
    Func_02002956(8, 4);
    Func_02002990(8, 0, 10);
    Func_020022c2(20);
    Func_0200297e(2, 2);
    Call3(Func_020029a8, 0x2002, 0, 10);
    Func_02002980(8, 3);
    Func_020029ba(8, 0, 10);
    Call3(Func_020029dc, 3, 0x101, 60);
    Func_020029ce(3, 0, 10);
    Call3(Func_020029f2, 8, 0x108, 60);
    Func_020029e4(8, 0, 10);
    Func_020029bc(3, 3);
    Func_020029f6(3, 0, 10);
    Func_020029ce_a(8, 3);
    Func_02002a08(8, 0, 10);
    Call3(Func_02002a2a, 1, 0x101, 40);
    Func_02002a1c(1, 0, 10);
    Call3(Func_02002a38, 8, 0x5000, 10);
    Func_02002a00(8, 3);
    Func_02002a3a(8, 0, 10);
    Func_02002a22(10, 2);
    Func_02002a1a(10, 4);
    Func_02002a54(10, 0, 10);
    Call3(Func_02002a76, 8, 0x105, 40);
    Func_02002a68(8, 0, 10);
    Call3(Func_02002a84, 8, 0x3000, 10);
    Func_02002a44(8, 4);
    Func_02002a86(8, 0, 10);
    Func_02002a6e(8, 1);
    Call3(Func_02002aaa, 8, 0x5000, 10);
    Func_02002aa4(8, 0, 10);
    Func_02002a8c(2, 1);
    Call3(Func_02002ab6, 0x2002, 0, 10);
    Call3(Func_02002ada, 8, 0x100, 40);
    Call3(Func_02002ade, 8, 0x3000, 10);
    Func_02002ad8(8, 0, 10);
    Func_020023c6(40);
    Func_02002410(20);
    Func_02002acc(1, 1);
    Func_02002af6(1, 0, 10);
    Func_02002ace(8, 3);
    Func_02002b08(8, 0, 10);
    Func_02002ad8_a(8, 4);
    Func_02002b1a(8, 0, 10);
    Func_02002aea(0, 3);
    Func_02002af2(1, 3);
    Func_02002afa(2, 3);
    Func_02002b0a(3, 3);
    Func_02002b22(8, 1);
    Func_02002b4c(8, 0, 10);
    Call2(Func_02002b76, 0, 0x102);
    Call2(Func_02002b80, 1, 0x102);
    Func_02002abe(60);
    Func_02002b3e(8, 4);
    Func_02002b78(8, 0, 10);
    Func_02002b58(2, 1);
    Func_02002b68(3, 1);
    Func_02002ae6(20);
    Func_02002b76_a(8, 1);
    Func_02002ba0(8, 0, 10);
    Func_02002b70(8, 3);
    Func_02002bb2(8, 0, 10);
    Call3(Func_02002bd4, 2, 0x105, 60);
    Call3(Func_02002bc6, 0x2002, 0, 10);
    Call3(Func_02002be2, 3, 0xa000, 10);
    Func_02002bdc(3, 0, 10);
    Func_02002bb4(2, 4);
    Func_02002bbc(2, 4);
    Call3(Func_02002c08, 2, 0x2000, 10);
    Func_02002bc8(2, 4);
    Call3(Func_02002c0a, 0x2002, 0, 10);
    Func_02002bf6(8, 4);
    Func_02002c30(8, 0, 10);
    Call3(Func_02002c4c, 2, 0xc000, 0);
    Func_02002c24(3, 2);
    Call3(Func_02002c60, 3, 0xc000, 10);
    Func_02002c5a(3, 0, 10);
    Func_02002c32(8, 4);
    Func_02002c6c(8, 0, 10);
    Func_02002c4c_a(10, 2);
    Func_02002c7e(10, 0, 10);
    Func_02002c56(11, 3);
    Func_02002c90(11, 0, 20);
    Func_02002c68(8, 3);
    Func_02002ca2(8, 0, 10);
    Func_02002c7a(2, 4);
    Func_02002c7a_a(2, 4);
    Call3(Func_02002cbc, 0x2002, 0, 10);
    Func_02002c94(1, 3);
    ((void (*)())Func_02002cce)(1, 0, 10);
    Func_02002cb6(8, 1);
    Value2(Func_02002cce_a, 8, 0);
    Call3(Func_02002cfa, 1, 0xe000, 0);
    Call3(Func_02002d06, 2, 0x6000, 0);
    Call3(Func_02002d12, 3, 0xa000, 0);
    if (Value2(Func_02002c82, 0, 0) == 0) {
        Func_02002c6c_a(20);
        Func_02002646(10);
        Func_02002cea(1, 3);
        Func_02002cf2(2, 3);
        Func_02002d02(3, 3);
        Func_02002c90_a(10);
        Func_02002d42(1, 0, 10);
        bump_step(2);
    } else {
        Func_02002cb2(10);
        Func_02002d42_a(1, 2);
        bump_step(1);
        Func_02002d7c(1, 0, 10);
        Func_020026ae(10);
        Func_02002d52(1, 3);
        Func_02002d5a(2, 3);
        Func_02002d6a(3, 3);
        Func_02002cf8(10);
        Func_02002daa(1, 0, 10);
    }
    Func_02002d82(8, 3);
    Func_02002dbc(8, 0, 40);
    Call3(Func_02002dd8, 8, 0x5000, 10);
    Func_02002db0(8, 1);
    Func_02002d2e(10);
    Func_02002dbe(10, 1);
    Func_02002d3c(20);
    Call3(Func_02002e00, 8, 0x3000, 10);
    Func_02002dd8_a(8, 1);
    Func_02002d56(10);
    Func_02002de6(11, 1);
    Func_02002d64(20);
    Func_02002de4(8, 3);
    Func_02002e1e(8, 0, 10);
    Func_02002dee(10, 3);
    Func_02002dfe(11, 3);
    Call3(Func_02002dc4, 10, 0x10000, 0x8000);
    Call3(Func_02002dd2, 11, 0x10000, 0x8000);
    Call3(Func_02002df8, 10, 0x350, 0x21c);
    Call3(Func_02002e0e, 11, 0x370, 0x21c);
    Func_02002e30(10, 0, 0);
    Func_02002e3a(11, 0, 0);
    Func_02002e52(8, 3);
    Func_02002e52_a(0, 3);
    Func_02002e5a(1, 3);
    Func_02002e62(2, 3);
    Func_02002e72(3, 3);
    Func_02002e00_a(20);
    Call3(Func_02002e3e, 1, 0x10000, 0x8000);
    Call3(Func_02002e4c, 2, 0x10000, 0x8000);
    Call3(Func_02002e5a_a, 3, 0x10000, 0x8000);
    tbl = (s32)Data_02009b94;
    Func_02002e6c(1, tbl);
    Value2(Func_02002e74, 2, tbl);
    Value2(Func_02002e84, 3, tbl);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 16;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x209;
    Call1(Func_02002e5a_b, 0x12f);
    Call1(Func_02002e58, 0x912);
    Func_02002e7c();
}

void ConfigureFourSceneChannelsAndHandoff(s32 handoff)
{
    Func_02002f4e(0, 0x6000, 0);
    ConfigureSecond(1, 0xe000, 0);
    ConfigureThird(2, 0x2000, 0);
    ConfigureFourth(3, 0xa000, 0);
    if (handoff != 0) {
        Func_02002ec4(handoff);
    }
}

void Func_02001450(s32 handoff)
{
    ConfigureFirst(0, 0xc000, 0);
    ConfigureUniformSecond(1, 0xc000, 0);
    ConfigureUniformThird(2, 0xc000, 0);
    ConfigureUniformFourth(3, 0xc000, 0);
    if (handoff != 0) {
        Func_02002f08(handoff);
    }
}

void Scene_RunPartySequence(void)
{
    extern u8 Data_03001ebc[];

    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Func_02002f1c();
    Call4(Func_02002ffe, -1, -1, -1, 0);
    Func_02002ee4(1);
    Call4(Func_02003016, 0x3600000, -1, 0x2180000, 0);
    Func_02002f02();
    Func_02002f00(1);
    Call3(Func_02002fbc, 0, 0x3600000, 0x2760000);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = v5 = 0x100;
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 40;
    Func_02003072();
    Call2(Func_0200304a, 0x6666, 0xccc);
    Call4(Func_02003064, 0x3600000, -1, 0x1d80000, 1);
    Call3(Func_02002fc6, 0, 0xcccc, 0x6666);
    Call3(Func_02002fd0, 1, 0xcccc, 0x6666);
    Call3(Func_02002fda, 2, 0xcccc, 0x6666);
    Call3(Func_02002fe4, 3, 0xcccc, 0x6666);
    Call3(Func_02003012, 0, 0x360, 0x1f2);
    record = Value1(Func_02002ff0, 0);
    if (record != 0) {
        Func_0200303e(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003004, 0);
    if (record != 0) {
        Func_02003052(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003018, 0);
    if (record != 0) {
        Func_02003066(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02003054, 0, 0x358, 0x1e6);
    Call3(Func_02003062, 1, 0x350, 0x1f6);
    Call3(Func_02003070, 2, 0x368, 0x1e6);
    Call3(Func_02003086, 3, 0x370, 0x1f6);
    Func_020030ae(0, 1);
    Func_020030b6(1, 1);
    Func_020030be(2, 1);
    Func_02003054_a(10);
    Func_02002a2e(10);
    Func_02003124(9, v5, 20);
    Call3(Func_02003128, 9, 0x5000, 20);
    Call1(Func_02003106, 0x2588);
    Call3(Func_02003128_a, 0x2009, 0, 10);
    Func_0200314a(8, v5, 20);
    Call3(Func_0200314e, 8, 0x3000, 20);
    Func_02003148(8, 0, 20);
    Call3(Func_0200316a, 8, 0x107, 60);
    Func_0200315c(8, 0, 10);
    Call2(Func_02003186, 0, 0x102);
    Call2(Func_02003190, 1, 0x102);
    Call2(Func_0200319a, 2, 0x102);
    Call2(Func_020031a4, 3, 0x102);
    Func_020030e2(60);
    Call3(Func_020031ae, 9, 0x102, 60);
    Call3(Func_020031b2, 9, 0x7000, 10);
    Call3(Func_020031ac, 0x2009, 0, 10);
    Call3(Func_020031c8, 8, 0x1000, 10);
    Call3(Func_020031dc, 8, 0x108, 20);
    Func_020031ce(8, 0, 40);
    Func_020031b6(8, 2);
    Func_020031e0(8, 0, 20);
    Call3(Func_020031fc, 8, 0x3000, 10);
    Value2(Func_020031e4, 8, 0);
    v5 = 1;
    if (Value2(Func_02003176, 0, 0) == 0) {
        Func_02003160(10);
        Func_020031d8(8, 3);
    } else {
        Func_02003170(10);
        Scene_AdvanceStep(1);
        Func_020031f6(8, 4);
        v5 = 0;
    }
    Func_0200323a(8, 0, 10);
    if (v5 != 0) {
        Scene_AdvanceStep(1);
    }
    Func_0200322e(9, 2);
    Call2(Func_02003280, 9, 0x102);
    Func_020031be(80);
    Call3(Func_02003270, 0x2009, 0, 10);
    Call3(Func_0200328c, 8, 0x1000, 10);
    Call3(Func_0200329e, 8, 0x107, 40);
    Func_0200326e(2, 3);
    Call3(Func_02003298, 0x2002, 0, 20);
    Call3(Func_020032ba, 8, 0x105, 60);
    Call3(Func_020032be, 8, 0x3000, 60);
    Func_020032b8(8, 0, 10);
    Func_02003298_a(1, 2);
    Func_020032ca(1, 0, 10);
    Call3(Func_020032e6, 9, 0x5000, 0);
    Call3(Func_020032f2, 2, 0x6000, 10);
    Func_020032c2(2, 2);
    Call3(Func_020032f4, 0x6002, 0, 10);
    Call3(Func_02003310, 2, 0xc000, 10);
    Func_020032e0(2, 2);
    Call3(Func_02003312, 0x2002, 0, 20);
    Func_020032ea(8, 3);
    Func_02003324(8, 0, 20);
    Call3(Func_02003340, 2, 0x8000, 20);
    Call3(Func_0200333a, 0x6002, 0, 10);
    Call3(Func_02003356, 3, 0xa000, 10);
    Call3(Func_02003368, 3, 0x105, 40);
    Func_0200335a(3, 0, 10);
    Func_02003342(2, 2);
    Func_020032c0(80);
    Func_02003340_a(2, 3);
    Func_020032ce(20);
    Call3(Func_02003392, 1, 0xe000, 10);
    Func_0200335a_a(1, 4);
    Value2(Func_02003382, 1, 0);
    if (Value2(Func_02003312_a, 0, 0) == 0) {
        Func_020032fc(20);
        Scene_AdvanceStep(1);
    } else {
        Func_02003340_b(20);
        Func_020033f2(1, 0, 10);
    }
    Func_020033c2(2, 3);
    Func_020033d2(3, 3);
    Func_02002d34(20);
    Func_020033f0(9, 2);
    Call3(Func_0200341a, 0x2009, 0, 10);
    Func_02003402(3, 2);
    Func_0200342c(3, 0, 10);
    Call3(Func_02003448, 9, 0x3000, 10);
    Func_02003410(9, 3);
    Call3(Func_0200344a, 0x2009, 0, 10);
    Call3(Func_0200346c, 2, 0x101, 60);
    Call3(Func_0200345e, 0x2002, 0, 10);
    Func_02003446(9, 1);
    Call3(Func_02003482, 9, 0x5000, 10);
    Call3(Func_0200347c, 0x2009, 0, 10);
    Func_02003464(1, 2);
    Func_0200348e(1, 0, 10);
    Func_02003466(9, 4);
    Call3(Func_020034a0, 0x2009, 0, 10);
    Func_02003488(3, 1);
    Func_020034b2(3, 0, 10);
    Func_0200349a(8, 1);
    Func_02003418(20);
    Func_020034ca(8, 0, 10);
    Func_0200349a_a(0, 3);
    Func_020034a2(1, 3);
    Func_020034aa(2, 3);
    Func_020034ba(3, 3);
    Func_02003448_a(20);
    tbl = (s32)Data_02009b94;
    Func_0200348a(1, tbl);
    Value2(Func_02003492, 2, tbl);
    Value2(Func_020034a2_a, 3, tbl);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(((s32)work + 0x1c8)) = 16;
    *(s32 *)(((s32)work + 0x1c0)) = 0x209;
    Call1(Func_02003478, 0x12f);
    Call1(Func_02003476, 0x914);
    Func_0200349a_b();
}
