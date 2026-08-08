#include "types.h"

/*
 * resource_391 owner at 0x02000d3c, 6700 bytes advertised.  The complete owner
 * is one prologue at 0x02000d3c and one epilogue at 0x0200273e-0x02002748; a
 * control-flow walk from the prologue reaches 6,382 bytes of code and leaves
 * eight literal pools totalling 318 bytes:
 *
 *   0x010d2-0x0111f (78)   0x01366-0x0138f (42)   0x0173e-0x01777 (58)
 *   0x01a78-0x01a83 (12)   0x01bfa-0x01bff  (6)   0x02002-0x02037 (54)
 *   0x02532-0x02557 (38)   0x0274a-0x02767 (30)
 *
 * 6,382 + 318 = 6,700, so the row is fully accounted for.  The pool map is
 * derived from the owner's own control flow (every one of those ranges is
 * jumped over by a `b.n`), NOT from overlay_show's "pool words referenced"
 * listing - that listing contains a spurious 0x2001424 = 0x78023023, which is
 * really the instruction pair `adds r0,#35 / ldrb r2,[r0,#0]` in live code.
 * Excluding it as pool would have dropped two real instructions.  Register
 * state is carried ACROSS every pool: several of them sit mid-call-setup.
 *
 * This is the scene script for the whole cutscene: it configures the camera and
 * the actor set, installs three per-frame effect tasks, and then plays out a
 * long fixed sequence of actor moves, waits, camera pushes and sound cues.
 *
 * CALLS.  Resolved with 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_391 0d3c'.
 * An overlay bl stores target_offset - 2, so overlay_show's annotations are
 * wrong at every site and were not used.  672 sites collapse to 43 distinct
 * targets: 41 import veneers resolving to main-image Func_08xxxxxx addresses,
 * plus the two in-overlay helpers Func_02002768 (63 sites) and Func_02002780
 * (68 sites), whose two- and three-argument signatures come from the byte-exact
 * assets/code/resource_391_c_02002768.c and _02002780.c.
 * The inventory says calls=616; that field is a lower bound, so completeness is
 * stated as placed(672) >= 616 and proved by the multiset comparison of bl
 * targets in the reconstruction assembly against the Func_ calls below.
 *
 * LINK BASE 0x02008000, confirmed here by three independent witnesses of the
 * cheapest kind - odd pool words that are a known function start plus the Thumb
 * bit, each handed to the task installers Func_080000d0/Func_080000d8:
 *   0x0200a7c9 = Func_020027c8 + 1   (semantic/overlays/resource_391_c_020027c8.c)
 *   0x0200a975 = Func_02002974 + 1   (semantic/overlays/resource_391_c_02002974.c)
 *   0x0200aad9 = Func_02002ad8 + 1   (assets/code/resource_391_c_02002ad8.c)
 * Under that base the even pool words 0x0200ae20/ae54/ae88/aebc/af48/af6c/afc8
 * and 0x0200b024 are in-image data at file offsets 0x2e20..0x3024, and the six
 * state words 0x0200b384..0x0200b398 are at 0x3384..0x3398, at and just past the
 * end of the 0x3384-byte assembled reconstruction.  The overlay image is
 * writable EWRAM, so those are live variables, not constants.
 *
 * This owner is the WRITER of the state the two installed tasks read, which
 * settles the open question in resource_391_c_020027c8.c and matches
 * resource_391_c_02002974.c exactly:
 *   Data_0200b384  height   (16.16), reset to 0x00800000 before each ramp
 *   Data_0200b388  spread,  reset to 0
 *   Data_0200b38c  phase    1 -> 2 -> 3; the task clears it to 0 when the ramp
 *                           finishes, and this owner spins on that
 *   Data_0200b390  cleared before the third task is installed
 *   Data_0200b394  a scene variant flag - Func_080770c0(3)'s result, gating the
 *                  fourth actor (slot 3) and actor 15 throughout
 *   Data_0200b398  the gate Func_020027c8 tests: set to 1 while that task runs,
 *                  cleared before it is stopped
 *
 * Epilogue 'pop {r3,r5} / mov r8,r3 / mov sl,r5 / pop {r5,r6,r7} / pop {r0} /
 * bx r0': r0 holds the popped return address, so the owner returns nothing, and
 * it reads no incoming argument register, so it takes none.
 *
 * The recurring five-instruction idiom
 *   ldr r3,[pc] ; ldr r2,[r3] ; movs r3,#236 ; lsls r3,#1 ; adds r2,r2,r3
 *   ldrh r3,[r2] ; adds r3,#1 ; strh r3,[r2]
 * bumps a u16 counter at workspace + 472.  It appears once on the "absent"
 * side of nearly every Data_0200b394 test, i.e. whenever a beat that would have
 * animated slot 3 is skipped - so it is a progress/step counter that must
 * advance the same number of times in both scene variants.  Spelled here as the
 * SKIP_BEAT macro; 0x03001ebc is the workspace pointer the rest of the overlay
 * loads directly.
 *
 * UNCERTAINTIES:
 *  - the numeric first arguments of the imports are actor/slot ids (0..3 are the
 *    four principals, 11..16 are secondary entities) and the second arguments are
 *    16.16 positions, durations in frames, or opaque animation ids depending on
 *    the import; no attempt is made to name them beyond what the call sites prove.
 *  - Func_0808a010 and Func_080000c0 both take a frame count and both appear as
 *    waits; they are kept distinct because the assembly does.
 *  - Func_0808a070(slot, 0) is tested three ways here - against 0, against
 *    non-zero, and (once, at 0x020015b2) against exactly 1 - so it returns a
 *    small code, not a boolean.  It is declared s32.
 *  - Data_0200ae20 etc. are opaque in-image blocks handed to Func_0808a098/
 *    Func_0808a0b0; their shape is not reconstructed.
 *  - r4 is never saved and never used; r8 and sl are saved and used only as
 *    long-lived constant holders, and both are reassigned mid-owner, so every
 *    constant below is taken per-use rather than as one variable.
 */

/* In-image data under the 0x02008000 link base. */
extern u8 Data_0200ae20[];
extern u8 Data_0200ae54[];
extern u8 Data_0200ae88[];
extern u8 Data_0200aebc[];
extern u8 Data_0200af48[];
extern u8 Data_0200af6c[];
extern u8 Data_0200afc8[];
extern u8 Data_0200b024[];
extern s32 Data_0200b384;
extern s32 Data_0200b388;
extern s32 Data_0200b38c;
extern s32 Data_0200b390;
extern s32 Data_0200b394;
extern s32 Data_0200b398;

/* IWRAM workspace pointer. */
#define WORKSPACE (*(u8 *volatile *)0x03001ebc)
#define SKIP_BEAT() do { \
    u8 *skip_workspace = WORKSPACE; \
    u16 *skip_counter = (u16 *)(skip_workspace + 472); \
    *skip_counter += 1; \
} while (0)

struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x23 - 0x14];
    u8 f23;
    u8 pad24[4];
    s32 f28;
};

/* Old-style declarations are mandatory in overlay sources. */
void Func_02003952();
void Func_02003a44();
void Func_020038e2();
void Func_02003a5c();
s32 Func_0200396c();
struct Obj *Func_020039a4();
void Func_02003972();
struct Obj *Func_020039b0();
void Func_0200397e();
struct Obj *Func_020039bc();
void Func_0200398a();
struct Obj *Func_020039c8();
void Func_02003996();
struct Obj *Func_020039d4();
void Func_020039a2();
struct Obj *Func_020039e0();
void Func_020039ae();
struct Obj *Func_020039ec();
void Func_020039ba();
struct Obj *Func_020039f8();
void Func_020039c6();
struct Obj *Func_02003a04();
void Func_020039d2();
void Func_02003a24();
void Func_02003a2c();
void Func_02003a34();
void Func_02003a3c();
void Func_02003a98();
void Func_020039a6();
void Func_020039ea();
void Func_020039b0_b();
void Func_02003b34();
void Func_02003b40();
void Func_02003a6a();
void Func_02003aac();
void Func_02003a7e();
void Func_02003a88();
struct Obj *Func_02003a86();
void Func_02003aec();
struct Obj *Func_02003a9a();
void Func_02003b00();
void Func_02003ac0();
void Func_02003ac8();
void Func_02003ad0();
struct Obj *Func_02003ace();
void Func_02003af4();
void Func_02003b06();
void Func_02003654();
void Func_02003660();
void Func_02003670();
void Func_02003bd6();
void Func_02003bda();
void Func_02003698();
void Func_020036a2();
void Func_02003bfc();
void Func_020036ba();
void Func_02003c20();
void Func_02003c2a();
void Func_02003b58();
void Func_020036e8();
void Func_02003c14();
void Func_020036e6();
void Func_02003c38();
void Func_02003714();
void Func_02003c28();
void Func_0200370c();
void Func_0200372e();
void Func_02003738();
void Func_02003742();
void Func_0200374c();
void Func_02003756();
void Func_02003762();
void Func_02003c8e();
void Func_02003cd2();
void Func_02003c00();
void Func_02003790();
void Func_0200379a();
void Func_02003d44();
void Func_02003d4a();
void Func_02003d22();
void Func_02003d30();
void Func_02003bbe();
void Func_02003bd4();
void Func_02003d4c();
void Func_02003d56();
void Func_02003d64();
void Func_02003bf2();
void Func_02003cac();
void Func_02003cb4();
void Func_02003cbc();
void Func_02003cc4();
void Func_02003c9a();
void Func_02003806();
void Func_0200380e();
void Func_02003cb8();
void Func_02003d9c();
void Func_02003cca();
void Func_02003836();
void Func_02003d38();
struct Obj *Func_02003d66();
void Func_02003d52();
void Func_02003d84();
void Func_02003dd0();
void Func_02003da0();
struct Obj *Func_02003d96();
void Func_02003e04();
void Func_02003d8a();
struct Obj *Func_02003db0();
void Func_02003dce();
void Func_02003de0();
struct Obj *Func_02003dd6();
void Func_02003da4();
void Func_02003e44();
void Func_02003dca();
struct Obj *Func_02003df0();
void Func_02003dd8();
void Func_02003e0a();
void Func_02003e1a();
struct Obj *Func_02003e10();
void Func_02003dde();
void Func_02003e7e();
struct Obj *Func_02003e2a();
void Func_02003e12();
void Func_02003e4a();
struct Obj *Func_02003e40();
void Func_02003e0e();
void Func_02003eae();
void Func_02003e3a();
void Func_02003e46();
void Func_02003f3e();
void Func_02003f4c();
void Func_02003dda();
void Func_02003dfe();
void Func_02003e7c();
void Func_02003fa2();
void Func_020039ee();
void Func_020039f6();
void Func_02003f76();
void Func_02003f82();
void Func_02003f8e();
void Func_02003f9a();
void Func_02003ec8();
void Func_02003f74();
void Func_02003ee2();
void Func_02003ef0();
void Func_02003f90();
void Func_02003f98();
void Func_02003f06();
void Func_02003fa6();
void Func_02003f14();
void Func_02003fb4();
void Func_02003f22();
void Func_02003fc2();
void Func_02003f30();
void Func_02003fd0();
void Func_0200401c();
void Func_02003ab0();
void Func_02004036();
void Func_02003aca();
struct Obj *Func_02003fce();
struct Obj *Func_02003fe0();
struct Obj *Func_02003ff0();
struct Obj *Func_02004000();
void Func_020040c2();
void Func_020040ca();
void Func_020040d2();
void Func_020040da();
void Func_02004140();
struct Obj *Func_02004046();
void Func_02004108();
void Func_020040bc();
void Func_0200407e();
struct Obj *Func_02004074();
void Func_02004136();
void Func_020040ea();
void Func_020040aa();
struct Obj *Func_020040a8();
void Func_0200416a();
void Func_0200411e();
void Func_020040de();
struct Obj *Func_020040d4();
void Func_02004196();
void Func_0200414c();
void Func_0200410c();
void Func_0200406a();
void Func_020040f8();
void Func_0200408e();
void Func_02004104();
void Func_0200422a();
void Func_02004204();
void Func_02004212();
void Func_020040a0();
void Func_02004166();
void Func_0200416c();
void Func_0200417a();
void Func_02004180();
void Func_020040c6();
void Func_02004188();
void Func_020041b6();
void Func_02004174();
void Func_0200421c();
void Func_02004182();
void Func_02004242();
void Func_02004210();
void Func_0200421e();
void Func_0200412c();
s32 Func_020041cc();
void Func_02004270();
void Func_020041d6();
void Func_0200427e();
void Func_02004294();
void Func_020041fa();
void Func_020042b0();
void Func_02003d6c();
void Func_020042a8();
void Func_020042f2();
void Func_02004220();
void Func_020042c8();
void Func_020042de();
void Func_02003d9a();
void Func_02004244();
void Func_0200431c();
struct Obj *Func_02004272();
struct Obj *Func_02004286();
void Func_02004254();
void Func_02004306();
void Func_020042ea();
void Func_0200430a();
void Func_02003e02();
void Func_0200433e();
void Func_02003e14();
void Func_0200435a();
void Func_02003e40_b();
void Func_020043a8();
void Func_020042d8();
void Func_02003e5e();
void Func_02003e68();
void Func_02003e72();
void Func_0200439a();
void Func_02004310();
void Func_020043aa();
void Func_02004320();
void Func_020043ba();
void Func_02004330();
void Func_02003e9c();
void Func_0200445e();
void Func_0200438c();
void Func_02004434();
void Func_02004472();
struct Obj *Func_020043c8();
struct Obj *Func_020043d8();
void Func_020043a6();
void Func_02004458();
void Func_0200443c();
void Func_0200445c();
void Func_02003f64();
void Func_02004490();
void Func_020043f6();
void Func_02003f62();
void Func_020044ae();
void Func_020044bc();
void Func_02003fba();
void Func_02003faa();
void Func_020044d6();
void Func_02004454();
void Func_020044fc();
void Func_02004462();
void Func_0200450a();
void Func_02004470();
void Func_02004548();
struct Obj *Func_0200449e();
struct Obj *Func_020044ae_b();
void Func_0200447c();
void Func_0200452e();
void Func_02004526();
void Func_0200457a();
void Func_0200455a();
void Func_020044c0();
void Func_02004598();
struct Obj *Func_020044ee();
struct Obj *Func_020044fc_b();
void Func_020044ca();
void Func_0200457e();
void Func_02004578();
void Func_020045e4();
void Func_020045ee();
void Func_0200451c();
void Func_020040a4();
void Func_020045b8();
void Func_020045c0();
void Func_020040be();
void Func_020040c8();
void Func_02004620();
void Func_020045e8();
void Func_020040cc();
void Func_020045f0();
void Func_020045f8();
void Func_02004608();
void Func_02004586();
void Func_0200462e();
void Func_02004594();
void Func_02004654();
void Func_02004624();
void Func_02004678();
void Func_02004682();
s32 Func_020045da();
void Func_0200464e();
void Func_0200465e();
void Func_02004690();
void Func_02004176();
void Func_0200469a();
void Func_020046ca();
void Func_02004192();
void Func_020046a6();
void Func_0200418a();
void Func_020041ae();
void Func_020046c2();
void Func_020041a6();
void Func_020046f2();
void Func_020041d8();
void Func_020041e4();
void Func_020046f0();
void Func_020041dc();
void Func_0200476e();
void Func_0200422c();
void Func_02004738();
void Func_02004748();
void Func_020046c6();
void Func_0200479e();
void Func_0200425c();
void Func_02004770();
void Func_020046ee();
void Func_020047d2();
void Func_020047de();
void Func_0200470c();
void Func_020047b8();
void Func_0200428c();
void Func_020047fc();
void Func_020042ba();
void Func_02004810();
void Func_020042d0();
void Func_020042dc();
void Func_020047f0();
void Func_020042d4();
void Func_0200485a();
void Func_02004780();
void Func_020042ec();
void Func_02004818();
void Func_02004796();
void Func_0200487a();
void Func_020047a8();
void Func_02004314();
void Func_02004838();
void Func_02004324();
void Func_02004868();
void Func_02004350();
void Func_02004894();
void Func_020048ae();
s32 Func_02004806();
void Func_02004882();
void Func_02004808();
void Func_020048b0();
void Func_02004816();
void Func_020048ee();
void Func_02004954();
void Func_0200492c();
void Func_0200493a();
void Func_020047c8();
void Func_020047ee();
void Func_0200486c();
void Func_0200490c();
void Func_02004914();
void Func_0200491c();
void Func_0200492e();
void Func_02004896();
void Func_0200441c();
void Func_02004956();
void Func_02004412();
void Func_0200443e();
void Func_02004450();
struct Obj *Func_020048fa();
struct Obj *Func_0200490c_b();
struct Obj *Func_0200491c_b();
struct Obj *Func_0200492c_b();
void Func_020049ec();
void Func_020049f4();
void Func_020049fc();
void Func_02004a04();
void Func_02004a5e();
void Func_020049c2();
void Func_02004984();
void Func_020049d8();
void Func_02004998();
void Func_020049f2();
void Func_020049b2();
void Func_02004a08();
void Func_020049c8();
void Func_0200499e();
void Func_0200492a();
void Func_02004524();
void Func_02004aa2();
void Func_02004aac();
void Func_02004ab6();
void Func_02004ac0();
void Func_020049ee();
void Func_02004a7e();
void Func_02004a86();
void Func_02004a8e();
void Func_02004a9e();
void Func_02004582();
void Func_02004aa6();
void Func_02004aae();
void Func_02004ac6();
void Func_020045aa();
void Func_02004b2a();
void Func_02004b36();
void Func_02004b42();
void Func_02004b4e();
void Func_02004a7c();
void Func_02004b58();
void Func_02004b68();
void Func_02004b72();
void Func_02004630();
void Func_02004ba0();
void Func_02004bac();
void Func_02004bb8();
void Func_02004bc4();
void Func_02004bbe();
void Func_02004bc8();
void Func_02004bd2();
void Func_02004680();
void Func_02004bf0();
void Func_02004bfc();
void Func_02004c06();
void Func_020046c4();
void Func_020046b4();
void Func_02004c22();
void Func_02004c2c();
void Func_02004c36();
void Func_020046f4();
void Func_02004c00();
void Func_02004c08();
void Func_02004c10();
void Func_02004c20();
void Func_02004b9e();
void Func_0200470a();
void Func_02004c2e();
void Func_02004c3e();
void Func_02004c4e();
void Func_02004732();
void Func_02004ca2();
void Func_02004cae();
void Func_02004cb8();
void Func_02004776();
void Func_02004766();
void Func_02004cda();
void Func_02004ce2();
void Func_02004cea();
void Func_02004cfa();
void Func_02004d2c();
void Func_02004d36();
void Func_02004d40();
void Func_02004d4a();
void Func_02004d5e();
void Func_02004d6a();
void Func_02004d74();
void Func_02004832();
void Func_02004822();
void Func_02004da2();
void Func_02004dae();
void Func_02004dba();
void Func_02004dc6();
void Func_02004cf4();
void Func_02004860();
void Func_02004dce();
void Func_02004dd8();
void Func_02004de2();
void Func_020048a0();
void Func_02004890();
void Func_02004db4();
void Func_02004dbc();
void Func_02004dc4();
void Func_02004dd4();
void Func_02004d52();
void Func_02004e1a();
void Func_02004e22();
void Func_02004cf8();
void Func_02004d6e();
void Func_02004e68();
void Func_02004e76();
void Func_02004d04();
void Func_02004dca();
void Func_02004dd0();
void Func_02004dde();
void Func_02004d24();
void Func_02004de6();
void Func_02004df4();
void Func_02004e14();
void Func_02004dd2();
void Func_02004eaa();
void Func_02004eb2();
void Func_02004eba();
void Func_02004ec2();
struct Obj *Func_02004e18();
struct Obj *Func_02004e28();
struct Obj *Func_02004e36();
struct Obj *Func_02004e44();
void Func_02004ed8();
void Func_020049be();
void Func_02004f02();
void Func_02004f1e();
void Func_02004f28();
s32 Func_02004e80();
void Func_02004f16();
void Func_02004e7c();
void Func_02004f3c();
s32 Func_02004ea4();
void Func_02004a16();
void Func_02004f7c();
void Func_02004f86();
void Func_02004f90();
void Func_02004ebe();
void Func_02004a46();
void Func_02004a36();
void Func_02004a58();
void Func_02004a62();
void Func_02004f76();
void Func_02004a5a();
void Func_02004a8a();
void Func_02004ff2();
void Func_02004ffe();
void Func_0200500a();
void Func_02004f38();
void Func_02004ffa();
void Func_02004fe2();
void Func_02004fea();
void Func_02004f7a();
void Func_02004f88();
void Func_0200503e();
void Func_02004afa();
void Func_0200506a();
void Func_02005036();
void Func_02005046();
void Func_02004fc4();
void Func_02005054();
void Func_0200508c();
s32 Func_02004ff4();
void Func_02004fe8();
void Func_020050ca();
void Func_02004ff8();
void Func_02004b7e();
void Func_02004b6e();
void Func_02004b96();
void Func_020050ba();
void Func_02004b8e();
void Func_02005120();
void Func_0200512c();
void Func_0200505a();
void Func_02005102();
void Func_02004bce();
void Func_0200514c();
void Func_0200507a();
void Func_02004be6();
void Func_02004c12();
void Func_02005126();
void Func_02004c0a();
void Func_020050be();
void Func_02005166();
void Func_02004c56();
void Func_02004c62();
void Func_02004c52();
void Func_0200518e();
void Func_0200519e();
void Func_02005104();
void Func_02004c70();
void Func_0200519c();
void Func_020051a4();
void Func_02005122();
void Func_020051aa();
void Func_020051ba();
void Func_0200523c();
void Func_0200516a();
void Func_020051fa();
void Func_02005228();
void Func_02004ce4();
void Func_02004d0c();
void Func_02005230();
void Func_02005296();
void Func_020052a2();
void Func_020051d0();
void Func_02005278();
void Func_02004d44();
void Func_020052c2();
void Func_020051f0();
void Func_02004d5c();
void Func_02004d88();
void Func_02005294();
void Func_02004d80();
void Func_020052d6();
void Func_02004da8();
void Func_020052e4();
void Func_020052f4();
void Func_0200525a();
void Func_02005388();
void Func_0200529a();
void Func_020052a4();
void Func_020052ae();
void Func_0200530e();
struct Obj *Func_020052b4();
void Func_020052f6();
void Func_0200531c();
void Func_0200532e();
void Func_0200533e();
struct Obj *Func_020052e4_b();
void Func_02005326();
void Func_0200534c();
void Func_0200535e();
void Func_02005376();
struct Obj *Func_0200531c_b();
void Func_02005384();
void Func_02005396();
void Func_020053a6();
void Func_020053b0();
void Func_0200544c();
void Func_02005350();

void Func_020027c8(void);
void Func_02002974(void);
void Func_02002ad8(void);

                     

                    

                     

                    

                     

                     

                     

                     

                     

                     

                     

                     

                     

void Func_02000d3c(void)
{
    struct Obj *p;
    s32 movement_2000;
    s32 movement_4000;
    s32 movement_6000;
    s32 movement_a000;
    s32 movement_c000;
    s32 actor1_2000;
    s32 actor1_6000;

    Func_02003952();
    Func_02003a44(-1, -1, -1, 0);
    Func_020038e2(1);
    Func_02003a5c(246 << 16, -1, 151 << 18, 0);

    /* The scene-variant flag: with it clear, slot 3 and actor 15 are absent. */
    Data_0200b394 = Func_0200396c(3);

    p = Func_020039a4(13);
    Func_02003972(p, 0);
    p = Func_020039b0(14);
    Func_0200397e(p, 0);
    p = Func_020039bc(15);
    Func_0200398a(p, 0);
    p = Func_020039c8(16);
    Func_02003996(p, 0);
    p = Func_020039d4(17);
    Func_020039a2(p, 0);
    p = Func_020039e0(18);
    Func_020039ae(p, 0);
    p = Func_020039ec(19);
    Func_020039ba(p, 0);
    p = Func_020039f8(20);
    Func_020039c6(p, 0);
    p = Func_02003a04(21);
    Func_020039d2(p, 0);

    Func_02003a24(17, Data_0200b024);
    Func_02003a2c(18, Data_0200b024);
    Func_02003a34(19, Data_0200b024);
    Func_02003a3c(20, Data_0200b024);
    Func_02003a44(21, Data_0200b024);

    Func_02003a98(0, 232 << 15, 0x025a0000);
    Func_020039a6(1);
    Func_020039ea();
    Func_020039b0_b(1);
    Func_02003b34();
    Func_02003b40();

    Func_02003a6a(0, 0xcccc, 0x6666);
    Func_02003aac(0, 254, 0x251);
    Func_02003a7e(1, 0x9999, 0x4ccc);
    Func_02003a88(2, 0x9999, 0x4ccc);

    p = Func_02003a86(0);
    if (p != 0) {
        Func_02003aec(1, p->f08, p->f10);
    }
    p = Func_02003a9a(0);
    if (p != 0) {
        Func_02003b00(2, p->f08, p->f10);
    }
    Func_02003ac0(1, Data_0200ae20);
    Func_02003ac8(2, Data_0200ae54);

    if (Data_0200b394 != 0) {
        Func_02003ad0(3, 0x9999, 0x4ccc);
        p = Func_02003ace(0);
        if (p != 0) {
            Func_02003b34(3, p->f08, p->f10);
        }
        Func_02003af4(3, Data_0200ae88);
    }

    movement_2000 = 0x2000;
    Func_02003b06(2);
    movement_4000 = 0x4000;
    Func_02003654(2, movement_2000, 40);
    Func_02003660(2, 0x8000, 20);
    Func_02003670(2, movement_4000, 40);
    Func_02003bd6(2, 0x101, 0);
    Func_02003b06(60);
    movement_6000 = 0x6000;
    Func_02003bda(1, movement_4000, 0);
    Func_02003698(0, movement_6000, 60);
    Func_020036a2(3, movement_2000, 10);
    Func_02003bfc(1, movement_2000, 0);
    movement_a000 = 0xa000;
    Func_020036ba(0, movement_a000, 10);
    Func_02003c20(1, 0x101, 0);
    Func_02003c2a(0, 0x101, 0);
    Func_02003b58(40);
    Func_02003c2a(1, movement_4000, 0);
    Func_020036e8(0, movement_6000, 10);
    Func_02003c14(1, 2);
    Func_02003c2a(0x1474);
    Func_020036e6(1, 10);
    Func_02003c38(0x147c);
    Func_02003714(2, movement_c000, 20);
    movement_c000 = 0xc000;
    Func_02003c28(2, 3);
    Func_0200370c(2, 20);
    Func_0200372e(1, 0, 20);
    Func_02003738(0, movement_a000, 40);
    Func_02003742(1, movement_4000, 20);
    Func_0200374c(0, movement_6000, 30);
    Func_02003756(1, movement_6000, 20);
    Func_02003762(0, 0xe000, 30);
    Func_02003c8e(2, 2);
    Func_02003cd2(2, 256, 0);
    Func_02003c00(40);
    Func_02003cd2(1, movement_4000, 0);
    Func_02003790(0, movement_6000, 20);
    Func_0200379a(2, movement_c000, 10);
    Func_02003d44(17);
    Func_02003d4a(206);
    Func_02003d22(0x7fff, 0);
    Func_02003d30(1);
    Func_02003bbe(1);

    /* First effect task: the 0x27c8 spawner, gated by Data_0200b398. */
    Data_0200b398 = 1;
    Func_02003bd6((void *)((u32)Func_020027c8 + 1), 3200);

    Func_02003bd4(20);
    Func_02003d4c(0x00405210, 1);
    Func_02003d56(0x10000, 2);
    Func_02003d64(120);
    Func_02003bf2(60);
    Func_02003cac(0, Data_0200aebc);
    Func_02003cb4(1, Data_0200aebc);
    Func_02003cbc(2, Data_0200aebc);
    Func_02003cc4(3, Data_0200aebc);
    Func_02003c9a(100);
    Func_02003806(1, 20);
    Func_0200380e(2, 40);

    if (Data_0200b394 != 0) {
        Func_02003cb8(40);
        Func_02003d9c(3, 258, 0);
        Func_02003cca(40);
        Func_02003836(3, 40);
    } else {
        SKIP_BEAT();
    }

    Func_02003d38(20);

    {
        u8 *actor_data;
        s32 placement_20000;

        if (Data_0200b394 != 0) {
            s32 conditional_placement;

            p = Func_02003d66(3);
            conditional_placement = 0x20000;
            p->f28 = conditional_placement;
            Func_02003d52(10);
            Func_02003d84(3, conditional_placement, conditional_placement);
            Func_02003dd0(3, -2, 0);
            Func_02003da0(3, Data_0200af48);
            p = Func_02003d96(3);
            Func_02003d64(p, 0);
            Func_02003e04(3, 19);
            Func_02003d8a(10);
        }

        p = Func_02003db0(0);
        placement_20000 = 0x20000;
        p->f28 = placement_20000;
        Func_02003d9c(10);
        Func_02003dce(0, placement_20000, placement_20000);
        actor_data = Data_0200af48;
        Func_02003de0(0, actor_data);
        p = Func_02003dd6(0);
        Func_02003da4(p, 0);
        Func_02003e44(0, 19);
        Func_02003dca(20);

        p = Func_02003df0(1);
        p->f28 = placement_20000;
        Func_02003dd8(10);
        Func_02003e0a(1, placement_20000, placement_20000);
        Func_02003e1a(1, actor_data);
        p = Func_02003e10(1);
        Func_02003dde(p, 0);
        Func_02003e7e(1, 19);
        Func_02003e04(40);

        p = Func_02003e2a(2);
        p->f28 = placement_20000;
        Func_02003e12(10);
        /* Slot 2 gets no Func_0808a090 - that asymmetry is in the original. */
        Func_02003e4a(2, actor_data);
        p = Func_02003e40(2);
        Func_02003e0e(p, 0);
        Func_02003eae(2, 19);
    }

    /* Stop the 0x27c8 spawner, then uninstall it. */
    Func_02003e3a(160);
    Data_0200b398 = 0;
    Func_02003dd0((void *)((u32)Func_020027c8 + 1));

    Func_02003e46(120);
    Func_02003f3e(0x00406218, 1);
    Func_02003f4c(60);
    Func_02003dda(60);

    /* Second effect task: the 0x2974 three-phase emitter. */
    Data_0200b388 = 0;
    Data_0200b384 = 0x00800000;
    Data_0200b38c = 1;
    Func_02003dfe((void *)((u32)Func_02002974 + 1), 3200);

    Func_02003e7c(180);
    Func_02003fa2(21);
    Func_020039ee(1, 80);
    Func_020039f6(2, 40);
    Func_02003f76(0, 258, 0);
    Func_02003f82(1, 258, 0);
    Func_02003f8e(2, 258, 0);
    Func_02003f9a(3, 258, 0);
    Func_02003ec8(60);
    Func_02003a34(2, 20);

    Func_02003f74(2, 2);
    Func_02003ee2(20);
    Func_02003f82(1, 1);
    Func_02003ef0(40);
    Func_02003f90(0, 2);
    Func_02003f98(3, 1);
    Func_02003f06(20);
    Func_02003fa6(2, 3);
    Func_02003f14(40);
    Func_02003fb4(0, 1);
    Func_02003f22(20);
    Func_02003fc2(1, 2);
    Func_02003f30(20);
    Func_02003fd0(3, 2);
    Data_0200b38c = 2;
    Func_0200401c(1, 258, 0);
    Func_02003ab0(1, 20);

    if (Data_0200b394 != 0) {
        Func_02004036(3, 258, 0);
        Func_02003aca(3, 10);
    } else {
        SKIP_BEAT();
    }

    Data_0200b38c = 3;

    Func_02003fce(0)->f23 &= 0xfe;
    Func_02003fe0(1)->f23 &= 0xfe;
    Func_02003ff0(2)->f23 &= 0xfe;
    Func_02004000(3)->f23 &= 0xfe;
    Func_020040c2(0, 3);
    Func_020040ca(1, 3);
    Func_020040d2(2, 3);
    Func_020040da(3, 3);

    /* Third effect task: 0x2ad8. */
    Data_0200b390 = 0;
    Func_02003fa2((void *)((u32)Func_02002ad8 + 1), 3200);

    Func_02004140(220);

    Func_02004046(13)->f23 &= 0xfe;
    Func_02004108(13, 2);
    Func_020040bc(13, 253 << 16, 0x025b0000);
    Func_0200407e(13, Data_0200af6c);

    Func_02004074(14)->f23 &= 0xfe;
    Func_02004136(14, 2);
    Func_020040ea(14, 233 << 16, 0x02750000);
    Func_020040aa(14, Data_0200af6c);

    if (Data_0200b394 != 0) {
        Func_020040a8(15)->f23 &= 0xfe;
        Func_0200416a(15, 2);
        Func_0200411e(15, 207 << 16, 0x02610000);
        Func_020040de(15, Data_0200af6c);
    }

    Func_020040d4(16)->f23 &= 0xfe;
    Func_02004196(16, 2);
    Func_0200414c(16, 227 << 16, 145 << 18);
    Func_0200410c(16, Data_0200af6c);

    /* Wait for the 0x2974 emitter to finish its ramp and clear the phase. */
    if (Data_0200b38c != 0) {
        do {
            Func_0200406a(1);
        } while (Data_0200b38c != 0);
    }

    Func_020040f8(300);
    Func_0200408e((void *)((u32)Func_02002974 + 1));
    Func_02004104(120);
    Func_0200422a(17);
    Func_02004204(0x10000, 1);
    Func_02004212(60);
    Func_020040a0(60);

    Func_02004166(13);
    Func_0200416c(14);
    if (Data_0200b394 != 0) {
        Func_0200417a(15);
    }
    Func_02004180(16);
    Func_020040c6(1);

    Func_02004180(13, Data_0200afc8);
    Func_02004188(14, Data_0200afc8);
    if (Data_0200b394 != 0) {
        Func_02004196(15, Data_0200afc8);
    }
    Func_020041b6(16, Data_0200afc8);

    Func_02004174(80);
    Func_0200421c(1, 2);
    Func_02004182(40);
    Func_02004242(1, 0);
    Func_02004210(11, 220 << 16, 247 << 17);
    Func_0200421e(12, 220 << 16, 247 << 17);
    Func_0200412c(1);

    if (Func_020041cc(11, 0) == 1) {
        SKIP_BEAT();
    }

    Func_02004270(0, 1);
    Func_020041d6(20);
    Func_0200427e(2, 2);
    Func_02003d4a(2, 20);

    if (Data_0200b394 != 0) {
        Func_02004294(3, 2);
        Func_020041fa(10);
        Func_020042b0(0x1488);
        Func_02003d6c(3, 40);
    }

    Func_020042a8(1, 1);
    Func_020042f2(1, 0x101, 0);
    Func_02004220(80);
    Func_020042c8(2, 2);
    Func_020042de(0x1489);
    Func_02003d9a(2, 40);
    Func_020042de(1, 3);
    Func_02004244(40);
    Func_0200431c(1, 2);

    Func_02004272(1)->f23 |= 1;
    p = Func_02004286(1);
    Func_02004254(p, 1);
    Func_02004306(1, 6, 0);
    Func_020042ea(1, -3, 0);
    Func_0200430a(1, 1);
    Func_02003e12(1, 0x4000, 60);
    Func_02003e02(1, 20);
    Func_0200433e(1, 2);
    actor1_2000 = 0x2000;
    Func_02003e14(1, 10);
    Func_0200435a(0, 3);
    Func_02003e40_b(1, actor1_2000, 20);
    Func_020043a8(1, 0x101, 0);
    actor1_6000 = 0x6000;
    Func_020042d8(40);
    Func_02003e5e(1, actor1_6000, 40);
    Func_02003e68(1, actor1_2000, 20);
    Func_02003e72(1, actor1_6000, 20);
    Func_02003e7c(1, actor1_2000, 10);
    Func_0200439a(1, 2, 0);
    Func_02004310(40);
    Func_020043aa(1, 2, 0);
    Func_02004320(10);
    Func_020043ba(1, 4, 0);
    Func_02004330(20);
    Func_02003e9c(1, 20);

    if (Data_0200b394 != 0) {
        Func_0200445e(3, 256, 0);
        Func_0200438c(60);
        Func_02004434(3, 2);
        Func_0200439a(80);
        Func_02004472(3, 2);
        Func_020043c8(3)->f23 |= 1;
        p = Func_020043d8(3);
        Func_020043a6(p, 1);
        Func_02004458(3, 4, 0);
        Func_0200443c(3, -2, 0);
        Func_0200445c(3, 1);
        Func_02003f64(3, 0xe000, 60);
        Func_02004490(3, 2);
        Func_020043f6(20);
        Func_02003f62(3, 20);
    } else {
        SKIP_BEAT();
    }

    Func_020044ae(1, 2, 0);
    Func_02003fa6(1, 0x4000, 20);
    Func_020044bc(1, 3);
    Func_02003fba(1, 0x2000, 10);
    Func_02003faa(1, 20);
    Func_020044d6(1, 3);
    Func_02004454(10);
    Func_020044fc(2, 1);
    Func_02004462(40);
    Func_0200450a(2, 2);
    Func_02004470(20);
    Func_02004548(2, 2);
    Func_0200449e(2)->f23 |= 1;
    p = Func_020044ae_b(2);
    Func_0200447c(p, 1);
    Func_0200452e(2, 4, 0);
    Func_02004526(2, 1);
    Func_0200457a(2, 0xc000, 0);
    Func_0200455a(0, 2);
    Func_020044c0(10);
    Func_02004598(0, 2);
    Func_020044ee(0)->f23 |= 1;
    p = Func_020044fc_b(0);
    Func_020044ca(p, 1);
    Func_0200457e(0, 4, 0);
    Func_02004578(0, 1);
    Func_0200407e(0, 0x6000, 60);
    Func_020045e4(0, 0x105, 0);
    Func_020045ee(2, 0x105, 0);
    Func_0200451c(60);
    Func_020040a4(0, 0xa000, 20);
    Func_020045b8(1, 3);
    Func_020045c0(0, 3);
    Func_020040be(0, 0x6000, 10);
    Func_020040c8(1, 0x4000, 10);
    Func_02004620(2, 0xc000, 0);
    Func_020045e8(2, 3);
    Func_020040cc(2, 20);
    Func_020045f0(0, 3);
    Func_020045f8(3, 3);
    Func_02004608(1, 3);
    Func_02004586(20);
    Func_0200462e(2, 1);
    Func_02004594(20);
    Func_02004654(2, 0);
    Func_02004624(2, 3);
    Func_02004678(2, 0xe000, 0);
    Func_02004682(1, 0x2000, 0);

    if (Func_020045da(0, 0) == 0) {
        Func_0200464e(2, 3);
        Func_0200465e(1, 3);
        SKIP_BEAT();
    } else {
        Func_02004690(1, 2);
        Func_02004176(1, 0x2000, 10);
        Func_0200469a(1, 2);
        Func_020046ca(1, 0);
    }

    Func_02004192(1, 0x4000, 10);
    Func_020046a6(1, 4);
    Func_0200418a(1, 20);
    Func_020041ae(2, 0xc000, 10);
    Func_020046c2(2, 3);
    Func_020041a6(2, 10);

    if (Data_0200b394 != 0) {
        Func_020046f2(3, 2);
        Func_020041d8(3, 0, 20);
        Func_020041e4(3, 0x2000, 10);
        Func_020046f0(3, 4);
        Func_020041dc(3, 10);
    } else {
        SKIP_BEAT();
    }

    Func_0200476e(1, 0x2000, 0);
    Func_0200422c(0, 0xa000, 10);
    Func_02004738(0, 3);
    Func_02004748(1, 3);
    Func_020046c6(20);
    Func_0200479e(0, 0x6000, 0);
    Func_0200425c(1, 0x4000, 10);
    Func_02004770(2, 4);
    Func_020046ee(20);
    Func_020047d2(0, 258, 0);
    Func_020047de(1, 258, 0);
    Func_0200470c(80);
    Func_02004294(2, 0xe000, 10);
    Func_020047b8(2, 2);
    Func_0200428c(2, 20);
    Func_020047fc(1, 0x2000, 0);
    Func_020042ba(0, 0xa000, 40);
    Func_02004810(1, 0x4000, 0);
    Func_020042d0(0, 0x6000, 10);
    Func_020042dc(2, 0xc000, 10);
    Func_020047f0(2, 3);
    Func_020042d4(2, 10);
    Func_0200485a(1, 258);
    Func_02004780(40);
    Func_020042ec(1, 20);
    Func_02004818(2, 3);
    Func_02004796(20);
    Func_0200487a(1, 258, 0);
    Func_020047a8(40);
    Func_02004314(1, 20);
    Func_02004838(2, 3);
    Func_02004324(2, 10);
    Func_02004868(1, 2);
    Func_02004350(1, 0x2000, 10);
    Func_02004894(1, 0);
    Func_020048ae(0, 0xa000, 0);

    if (Func_02004806(0, 0) == 0) {
        Func_02004882(1, 3);
    } else {
        Func_02004808(20);
        Func_020048b0(1, 2);
        Func_02004816(40);
        SKIP_BEAT();
    }

    Func_020048ee(1, 0);
    Func_02004954(21);
    Func_0200492c(0x00406218, 1);
    Func_0200493a(60);
    Func_020047c8(60);

    /* Re-arm and reinstall the 0x2974 emitter for the second ramp. */
    Data_0200b388 = 0;
    Data_0200b384 = 0x00800000;
    Data_0200b38c = 1;
    Func_020047ee((void *)((u32)Func_02002974 + 1), 3200);

    Func_0200486c(80);
    Func_0200490c(0, 2);
    Func_02004914(1, 2);
    Func_0200491c(3, 2);
    Func_0200492e(2, 2);
    Func_02004896(60);
    Func_0200441c(2, 0xc000, 10);
    Func_02004956(0x149d);
    Func_02004412(2, 10);
    Func_02004434(1, 0xc000, 10);
    Func_0200443e(0, 0xc000, 10);
    if (Data_0200b394 != 0) {
        Func_02004450(3, 0xc000, 10);
    }

    Func_020048fa(0)->f23 &= 0xfe;
    Func_0200490c_b(1)->f23 &= 0xfe;
    Func_0200491c_b(2)->f23 &= 0xfe;
    Func_0200492c_b(3)->f23 &= 0xfe;
    Func_020049ec(0, 3);
    Func_020049f4(1, 3);
    Func_020049fc(2, 3);
    Func_02004a04(3, 3);

    Data_0200b38c = 2;
    Func_02004a5e(220);
    Func_020049c2(13, 253 << 16, 0x025b0000);
    Func_02004984(13, Data_0200af6c);
    Func_020049d8(14, 233 << 16, 0x02750000);
    Func_02004998(14, Data_0200af6c);
    if (Data_0200b394 != 0) {
        Func_020049f2(15, 207 << 16, 0x02610000);
        Func_020049b2(15, Data_0200af6c);
    }
    Func_02004a08(16, 227 << 16, 145 << 18);
    Func_020049c8(16, Data_0200af6c);
    Func_0200499e(120);

    Data_0200b38c = 3;
    do {
        Func_0200492a(1);
    } while (Data_0200b38c != 0);

    Func_0200451c(11, 80);
    Func_02004524(12, 20);
    Func_02004aa2(0, 0x101, 0);
    Func_02004aac(1, 0x101, 0);
    Func_02004ab6(2, 0x101, 0);
    Func_02004ac0(3, 0x101, 0);
    Func_020049ee(60);
    Func_0200455a(12, 20);
    Func_02004a7e(0, 3);
    Func_02004a86(1, 3);
    Func_02004a8e(3, 3);
    Func_02004a9e(2, 3);
    Func_02004582(12, 10);
    Func_02004aa6(0, 3);
    Func_02004aae(1, 3);
    Func_02004ab6(3, 3);
    Func_02004ac6(2, 3);
    Func_020045aa(12, 10);
    Func_02004b2a(0, 256, 0);
    Func_02004b36(1, 256, 0);
    Func_02004b42(2, 256, 0);
    Func_02004b4e(3, 256, 0);
    Func_02004a7c(40);
    Func_020045e8(11, 10);
    Func_02004b58(0, 0x8000, 0);
    Func_02004b68(1, 0x4000, 0);
    Func_02004b72(3, 0, 0);
    Func_02004630(2, 0xc000, 40);
    Func_02004620(12, 10);
    Func_02004ba0(0, 258, 0);
    Func_02004bac(1, 258, 0);
    Func_02004bb8(2, 258, 0);
    Func_02004bc4(3, 258, 0);
    Func_02004bbe(0, 0xc000, 0);
    Func_02004bc8(1, 0xc000, 0);
    Func_02004bd2(2, 0xc000, 0);
    Func_02004690(3, 0xc000, 80);
    Func_02004680(12, 10);
    Func_02004bf0(0, 0x8000, 0);
    Func_02004bfc(1, 0x4000, 0);
    Func_02004c06(2, 0xc000, 0);
    Func_020046c4(3, 0, 40);
    Func_020046b4(11, 10);
    Func_02004c22(0, 0xc000, 0);
    Func_02004c2c(1, 0xc000, 0);
    Func_02004c36(2, 0xc000, 0);
    Func_020046f4(3, 0xc000, 10);
    Func_02004c00(0, 4);
    Func_02004c08(1, 4);
    Func_02004c10(3, 4);
    Func_02004c20(2, 4);
    Func_02004b9e(60);
    Func_0200470a(12, 10);
    Func_02004c2e(0, 3);
    Func_02004c36(1, 3);
    Func_02004c3e(3, 3);
    Func_02004c4e(2, 3);
    Func_02004732(12, 20);
    Func_02004ca2(0, 0x8000, 0);
    Func_02004cae(1, 0x4000, 0);
    Func_02004cb8(2, 0xc000, 0);
    Func_02004776(3, 0, 20);
    Func_02004766(12, 10);

    Func_02004cda(0, 2);
    Func_02004ce2(1, 2);
    Func_02004cea(3, 2);
    Func_02004cfa(2, 2);
    Func_02004d2c(0, 0xc000, 0);
    Func_02004d36(1, 0xc000, 0);
    Func_02004d40(2, 0xc000, 0);
    Func_02004d4a(3, 0xc000, 0);
    Func_020047ee(12, 20);
    Func_02004d5e(0, 0x8000, 0);
    Func_02004d6a(1, 0x4000, 0);
    Func_02004d74(2, 0xc000, 0);
    Func_02004832(3, 0, 20);
    Func_02004822(11, 20);
    Func_02004da2(0, 258, 0);
    Func_02004dae(1, 258, 0);
    Func_02004dba(3, 258, 0);
    Func_02004dc6(2, 258, 0);
    Func_02004cf4(40);
    Func_02004860(12, 10);
    Func_02004dce(0, 0xc000, 0);
    Func_02004dd8(1, 0xc000, 0);
    Func_02004de2(2, 0xc000, 0);
    Func_020048a0(3, 0xc000, 10);
    Func_02004890(12, 10);
    Func_02004db4(0, 3);
    Func_02004dbc(1, 3);
    Func_02004dc4(3, 3);
    Func_02004dd4(2, 3);
    Func_02004d52(60);
    Func_02004e1a(12, 0);
    Func_02004e22(11, 0);

    Func_02004cf8((void *)((u32)Func_02002974 + 1));
    Func_02004d6e(80);
    Func_02004e68(0x10000, 1);
    Func_02004e76(60);
    Func_02004d04(80);

    Func_02004dca(13);
    Func_02004dd0(14);
    /* Unlike the first pass, slot 15 is faded unconditionally here. */
    Func_02004dd8(15);
    Func_02004dde(16);
    Func_02004d24(1);
    Func_02004dde(13, Data_0200afc8);
    Func_02004de6(14, Data_0200afc8);
    if (Data_0200b394 != 0) {
        Func_02004df4(15, Data_0200afc8);
    }
    Func_02004e14(16, Data_0200afc8);

    Func_02004dd2(20);
    Func_02004eaa(0, 2);
    Func_02004eb2(1, 2);
    Func_02004eba(2, 2);
    Func_02004ec2(3, 2);
    Func_02004e18(0)->f23 |= 1;
    Func_02004e28(1)->f23 |= 1;
    Func_02004e36(2)->f23 |= 1;
    Func_02004e44(3)->f23 |= 1;

    Func_02004ed8(2, 2);
    Func_020049be(2, 0xe000, 10);
    Func_02004f02(2, 0);
    Func_02004f1e(1, 0x2000, 0);
    Func_02004f28(3, 0, 0);

    if (Func_02004e80(0, 0) == 0) {
        Func_02004f16(1, 2);
        Func_02004e7c(10);
        Func_02004f3c(1, 0);

        if (Func_02004ea4(0, 0) == 0) {
            Func_02004a16(3, 0, 20);
            Func_02004f7c(1, 0x101, 0);
            Func_02004f86(2, 0x101, 0);
            Func_02004f90(3, 0x101, 0);
            Func_02004ebe(40);
            Func_02004a46(1, 0x4000, 20);
            Func_02004a36(1, 10);
            Func_02004a58(2, 0xc000, 20);
            Func_02004a62(2, 0xe000, 20);
            Func_02004f76(2, 3);
            Func_02004a5a(2, 20);
            Func_02004a7e(1, 0x2000, 20);
        } else {
            Func_02004a8a(3, 0, 20);
            Func_02004ff2(1, 258, 0);
            Func_02004ffe(2, 258, 0);
            Func_0200500a(3, 258, 0);
            Func_02004f38(40);
            Func_02004ac0(1, 0x4000, 20);
            Func_02004ffa(0x14b4);
            Func_02004ab6(1, 20);
            Func_02004fe2(2, 3);
            Func_02004ac6(2, 20);
        }

        Func_02004fea(3, 3);
        Func_02004ffa(1, 3);
    } else {
        Func_02004f7a(20);
        Func_0200500a(1, 3);
        Func_02004f88(10);
        Func_0200503e(0x14b6);
        Func_02004afa(1, 10);
        Func_0200506a(1, 0x4000, 0);
        Func_02004b2a(0, 0x6000, 20);
        Func_02005036(1, 3);
        Func_02005046(0, 3);
        Func_02004fc4(10);
        Func_02005054(2, 4);
        Func_0200508c(2, 0);

        if (Func_02004ff4(0, 0) == 0) {
            Func_02004fe8(20);
            Func_020050ca(2, 0x103, 0);
            Func_02004ff8(40);
            Func_02004b7e(2, 0xe000, 10);
            Func_02004b6e(2, 10);
            if (Data_0200b394 != 0) {
                Func_02004b96(3, 0, 10);
                Func_020050ba(3, 3);
                Func_02004b8e(3, 20);
            } else {
                SKIP_BEAT();
            }
            Func_02005120(0, 258, 0);
            Func_0200512c(1, 258, 0);
            Func_0200505a(40);
            Func_02005102(1, 2);
            Func_02004bce(1, 20);
            Func_0200514c(1, 0x105, 0);
            Func_0200507a(120);
            Func_02004be6(2, 40);
            if (Data_0200b394 != 0) {
                Func_02004c12(3, 0x2000, 10);
                Func_02005126(3, 4);
                Func_02004c0a(3, 10);
            } else {
                SKIP_BEAT();
            }
            Func_020050be(60);
            Func_02005166(2, 2);
            if (Data_0200b394 != 0) {
                Func_02004c56(2, 0xa000, 40);
                Func_02004c62(2, 0xe000, 20);
            }
            Func_02004c52(2, 10);
            Func_0200518e(0, 2);
            Func_0200519e(1, 2);
            Func_02005104(40);
            Func_02004c70(2, 20);
            Func_0200519c(0, 3);
            Func_020051a4(1, 3);
            Func_02005122(20);
            Func_020051aa(3, 3);
            goto shared_scene_tail;
        }

        goto alternate_scene_tail;

shared_scene_tail:
        Func_020051ba(2, 3);
        goto scene_close;

alternate_scene_tail:
        {
            Func_0200523c(2, 0x105, 0);
            Func_0200516a(40);
            Func_020051fa(2, 3);
            Func_02005228(0x14bf);
            Func_02004ce4(2, 20);
            if (Data_0200b394 != 0) {
                Func_02004d0c(3, 0, 10);
                Func_02005230(3, 1);
                Func_02004d04(3, 20);
            } else {
                SKIP_BEAT();
            }
            Func_02005296(1, 258, 0);
            Func_020052a2(0, 258, 0);
            Func_020051d0(40);
            Func_02005278(1, 2);
            Func_02004d44(1, 20);
            Func_020052c2(2, 0x105, 0);
            Func_020051f0(80);
            Func_02004d5c(2, 40);
            if (Data_0200b394 != 0) {
                Func_02004d88(3, 0x2000, 20);
                Func_02005294(3, 4);
                Func_02004d80(3, 40);
            } else {
                SKIP_BEAT();
            }
            Func_020052d6(2, 2);
            Func_0200523c(20);
            Func_02004da8(2, 20);
            Func_020052e4(1, 2);
            Func_020052f4(0, 2);
            Func_0200525a(40);
            Func_02004dc6(2, 20);
            goto scene_close;
        }
    }

scene_close:
    Func_02005388(17);
    Func_0200529a(1, 0x00013333, 0x9999);
    Func_020052a4(2, 0x00013333, 0x9999);
    Func_020052ae(3, 0x00013333, 0x9999);

    Func_0200530e(1, 2);
    p = Func_020052b4(0);
    if (p != 0) {
        Func_020052f6(1, *(s16 *)((u8 *)p + 10), *(s16 *)((u8 *)p + 18));
    }
    Func_0200531c(1);
    Func_0200532e(1, 0, 0);

    Func_0200533e(2, 2);
    p = Func_020052e4_b(0);
    if (p != 0) {
        Func_02005326(2, *(s16 *)((u8 *)p + 10), *(s16 *)((u8 *)p + 18));
    }
    Func_0200534c(2);
    Func_0200535e(2, 0, 0);

    if (Data_0200b394 != 0) {
        Func_02005376(3, 2);
        p = Func_0200531c_b(0);
        if (p != 0) {
            Func_0200535e(3, *(s16 *)((u8 *)p + 10), *(s16 *)((u8 *)p + 18));
        }
        Func_02005384(3);
        Func_02005396(3, 0, 0);
    }

    Func_0200531c(0x843);
    Func_020053a6(11, 0, 0);
    Func_020053b0(12, 0, 0);
    Func_0200544c();
    Func_02005350();
}
