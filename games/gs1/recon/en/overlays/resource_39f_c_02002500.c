#include "types.h"

#define RunEventScript01 Func_02002500

/*
 * resource_39f owner at 0x02002500, 1700 reviewed bytes: the overlay's primary
 * event script.  It publishes the scene phase, then dispatches on two signed
 * halfwords of the shared scene-record block: the scene id at +448 selects one
 * of three arms (0x44, 0x45, 0x46) and the scene sub-state at +450 selects the
 * beat inside that arm.  The 0x44 and 0x45 arms use compare trees; the 0x46 arm
 * uses the 13-entry jump table that sits inline at 0x020028b0-0x020028e3, whose
 * base word 0x0200a8b0 is pooled at 0x02002b64 (runtime base 0x02008000, so
 * image offset 0x020028b0 - the table is inside this owner).  Every arm falls
 * into one shared epilogue that returns 0.
 *
 * The trailing literal pool ends at 0x02002ba4, immediately before the next
 * owner. Reference PC-relative loads through 0x02002b48 reach its last word
 * at 0x02002ba0. The reviewed extent includes this complete pool.
 *
 * Loader relocation.  The image holds BL-shaped word pairs that the resource
 * loader rewrites per site, so one pre-relocation call word can serve two sites
 * with different runtime targets.  Three of this owner's words do exactly that:
 *
 *   image word        site        runtime target   spelled here as
 *   Func_020053f6     0x020025d0  0x0200ae24       Func_02005434
 *   Func_020053f6     0x02002638  0x0200adbc       Func_02005394
 *   Func_020054ac     0x02002686  0x0200ae24       Func_020054a4
 *   Func_020054ac     0x020026ee  0x0200adbc       Func_0200544a
 *   Func_020055be     0x02002800  0x0200adbc       Func_0200558a
 *   Func_020055be     0x02002830  0x0200ad8c       Func_0200559e
 *
 * A colliding alias cannot be resolved from the reference alone, so each of the
 * six sites is spelled with another alias of this same owner that binds
 * uniquely to the runtime target the reference site actually reaches.  The
 * emitted call word is identical because a BL depends only on its site and its
 * target.  The honest Func_020053f6 / Func_020053f6_a spellings need an
 * absolute_symbols block in a translation unit; the exact JSON is in this
 * worker's integrator notes.
 *
 * Current scores and unresolved differences belong in the owner dossier.
 *
 * Uncertainties.  The engine roles below are named only where this repository
 * already binds the main-image address (GameFlag_IsSet 0x080770c0, GameFlag_Set
 * 0x080770c8, Scene_GetRecord 0x0808a080); every other veneer keeps its raw
 * alias and records its main-image target in a comment.  Record offsets +8,
 * +16, +35, +89 and +108 are named by position only.
 */

/* Shared cross-overlay scene-record block: +448 is the scene id, +450 the
 * scene sub-state.  Both are read as signed halfwords. */
extern u8 Data_02000240[];

/* The three scene ids this owner dispatches on.  The image pools the compared
 * word instead of building a Thumb immediate, so each is spelled as an
 * absolute-valued symbol, the convention this repository already uses in
 * games/gs1/src/overlays/state_update/dispatch_by_status_448.c. */
extern u8 Value_00000044;
extern u8 Value_00000045;
extern u8 Value_00000046;

/* Pointer to the shared scene work record; +448 is the scene phase word. */
extern u8 Data_03001ebc[];

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_020052d0(); /* main:080770c0  GameFlag_IsSet */
void Func_020052f0(); /* main:0808a018 */
void Func_0200523e(); /* main:080000c0 */
void Func_0200537e(); /* main:0808a158 */
void Func_0200535c(); /* main:0808a0f0 */
void Func_020053a8(); /* main:0808a1b8 */
void Func_02005342(); /* main:0808a090 */
void Func_02005364(); /* main:0808a0c8 */
void Func_02005420(); /* main:0808a360 */
void Func_0200542c(); /* main:0808a370 */
void Func_02005382(); /* main:0808a0e8 */
void Func_02005338(); /* main:0808a010 */
void Func_020053ec(); /* main:0808a1e8 */
void Func_020053bc(); /* main:0808a138 */
void Func_02005352(); /* main:0808a010 */
void Func_02003354(); /* FieldScene_RunScene39f_02000d90 */
void Func_020033e2(); /* overlay 0x02000e18 */
u8 *Func_02005394(); /* main:0808a080  Scene_GetRecord */
void Func_0200533a(); /* main:080091e0 */
void Func_02005378(); /* main:080770c8  GameFlag_Set */
void Func_02005386(); /* main:0808a010 */
void Func_0200539a(); /* main:0808a020 */
s32 Func_02005380(); /* main:080770c0  GameFlag_IsSet */
s32 Func_0200538e(); /* main:080770c0  GameFlag_IsSet */
void Func_02005434(); /* main:0808a158 */
void Func_02005412(); /* main:0808a0f0 */
u8 *Func_020053e2(); /* main:0808a080  Scene_GetRecord */
void Func_02005428(); /* main:0808a0f0 */
s32 Func_020053d0(); /* main:080770c0  GameFlag_IsSet */
u8 *Func_0200540a(); /* main:0808a080  Scene_GetRecord */
void Func_02005346(); /* main:080000c0 */
void Func_0200545c(); /* main:0808a0f0 */
s32 Func_020053fa(); /* main:080770c0  GameFlag_IsSet */
void Func_02005284(); /* InitializeOrbitingEffect */
void Func_020054a4(); /* main:0808a158 */
u8 *Func_0200544a(); /* main:0808a080  Scene_GetRecord */
void Func_020053f0(); /* main:080091e0 */
u8 *Func_02005456(); /* main:0808a080  Scene_GetRecord */
void Func_020053fc(); /* main:080091e0 */
void Func_02002f66(); /* overlay 0x020008c0 */
s32 Func_0200543a(); /* main:080770c0  GameFlag_IsSet */
void Func_02002f78(); /* overlay 0x020008c0 */
void Func_020053ac(); /* main:080000c0 */
void Func_020054c2(); /* main:0808a0f0 */
void Func_020054d2(); /* main:0808a100 */
void Func_02005436(); /* main:080091c0 */
s32 Func_020054b4(); /* main:080770c0  GameFlag_IsSet */
void Func_02005558(); /* main:0808a158 */
u8 *Func_020054f6(); /* main:0808a080  Scene_GetRecord */
void Func_0200549c(); /* main:080091e0 */
s32 Func_020054d4(); /* main:080770c0  GameFlag_IsSet */
void Func_0200557a(); /* main:0808a158 */
s32 Func_020054ec(); /* main:080770c0  GameFlag_IsSet */
void Func_0200302a(); /* overlay 0x020008c0 */
void Func_0200545e(); /* main:080000c0 */
void Func_02005574(); /* main:0808a0f0 */
void Func_02005584(); /* main:0808a100 */
u8 *Func_0200554a(); /* main:0808a080  Scene_GetRecord */
void Func_020054f4(); /* main:080091c0 */
s32 Func_02002a00(u32, s32, s32, u32, u32, s32);
void Func_02003082(); /* overlay 0x020008c0 */
void Func_02003088(); /* overlay 0x020008c0 */
u8 *Func_0200558a(); /* main:0808a080  Scene_GetRecord */
u8 *Func_02005592(); /* main:0808a080  Scene_GetRecord */
s32 Func_02002a32(u32, s32, s32, u32, u32, s32);
void Func_020054e0(); /* main:080000c0 */
void Func_02005620(); /* main:0808a158 */
s32 Func_0200559e(); /* main:080770c0  GameFlag_IsSet */
void Func_02005644(); /* main:0808a158 */
u8 *Func_020055e2(); /* main:0808a080  Scene_GetRecord */
void Func_02005588(); /* main:080091e0 */
void Func_0200563a(); /* main:0808a0f0 */
void Func_02005648(); /* main:0808a0f0 */
void Func_020055d4(); /* main:080000c0 */
s32 Func_0200567a(); /* main:080770c0  GameFlag_IsSet */
void Func_0200571e(); /* main:0808a158 */
u8 *Func_020056bc(); /* main:0808a080  Scene_GetRecord */
void Func_02005662(); /* main:080091e0 */
u8 *Func_020056c8(); /* main:0808a080  Scene_GetRecord */
void Func_0200566e(); /* main:080091e0 */
s32 Func_020056a6(); /* main:080770c0  GameFlag_IsSet */
void Func_0200574a(); /* main:0808a158 */
u8 *Func_020056e8(); /* main:0808a080  Scene_GetRecord */
void Func_0200568e(); /* main:080091e0 */
u8 *Func_020056f4(); /* main:0808a080  Scene_GetRecord */
void Func_0200569a(); /* main:080091e0 */
s32 Func_020056d0(); /* main:080770c0  GameFlag_IsSet */
void Func_02005776(); /* main:0808a158 */
u8 *Func_02005714(); /* main:0808a080  Scene_GetRecord */
void Func_020056ba(); /* main:080091e0 */
u8 *Func_02005720(); /* main:0808a080  Scene_GetRecord */
void Func_020056c6(); /* main:080091e0 */
u8 *Func_0200572e(); /* main:0808a080  Scene_GetRecord */
u8 *Func_02005736(); /* main:0808a080  Scene_GetRecord */
s32 Func_02002bd6(u32, s32, s32, u32, u32, s32);
void Func_020057be(); /* main:0808a158 */
void Func_0200568c(); /* main:080000c0 */
u8 *Func_02005762(); /* main:0808a080  Scene_GetRecord */
void Func_02003276(); /* overlay 0x020008c0 */
u8 *Func_02003414(s32, s32, s32, s32);
u8 *Func_02003420(s32, s32, s32, s32);
void Func_0200329a(); /* overlay 0x020008c0 */
void Func_020032a0(); /* overlay 0x020008c0 */
s32 Func_02005772(); /* main:080770c0  GameFlag_IsSet */
void Func_020032b0(); /* overlay 0x020008c0 */
void Func_020056e4(); /* main:080000c0 */
void Func_020057fc(); /* main:0808a100 */
void Func_02005800(); /* main:0808a0f0 */
u8 *Func_020057ce(); /* main:0808a080  Scene_GetRecord */
void Func_0200577c(); /* main:080091c0 */
void Func_020057cc(); /* main:080770c8  GameFlag_Set */
void Func_0200583a(); /* main:0808a0f0 */
u8 *Func_02005808(); /* main:0808a080  Scene_GetRecord */
void Func_020057ae(); /* main:080091e0 */
s32 Func_020057e4(); /* main:080770c0  GameFlag_IsSet */
void Func_02003322(); /* overlay 0x020008c0 */
void Func_02005756(); /* main:080000c0 */
void Func_0200586e(); /* main:0808a100 */
void Func_02005870(); /* main:0808a0f0 */
u8 *Func_0200583e(); /* main:0808a080  Scene_GetRecord */
void Func_020057e8(); /* main:080091c0 */
u8 *Func_0200585e(); /* main:0808a080  Scene_GetRecord */
u8 *Func_02005866(); /* main:0808a080  Scene_GetRecord */
s32 Func_02002d06(u32, s32, s32, u32, u32, s32);
void Func_020058ee(); /* main:0808a158 */
void Func_020057bc(); /* main:080000c0 */
u8 *Func_02005892(); /* main:0808a080  Scene_GetRecord */
s32 Func_02005872(); /* main:080770c0  GameFlag_IsSet */
void Func_02005916(); /* main:0808a158 */
u8 *Func_020058b4(); /* main:0808a080  Scene_GetRecord */
void Func_0200585a(); /* main:080091e0 */
s32 Func_02005890(); /* main:080770c0  GameFlag_IsSet */
void Func_0200590a(); /* main:0808a0f0 */
void Func_02005918(); /* main:0808a0f0 */
void Func_0200475c(); /* Scene_RunSupplementalSequenceOne */
void Func_020058fe(); /* main:? */
s32 Func_020058c4(); /* main:080770c0  GameFlag_IsSet */
s32 Func_020058ce(); /* main:080770c0  GameFlag_IsSet */
void Func_020058e0(); /* main:080770c8  GameFlag_Set */
void Func_020058e8(); /* main:080770c8  GameFlag_Set */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Publishes the scene phase, then runs the beat selected by the scene id and
 * sub-state.  Returns 0 for every path. */
s32 RunEventScript01(void)
{
    u8 *rec;
    s32 mode;
    s32 step;
    s32 pos;
    s32 tmp;

    {
        u8 *work = *(u8 **)Data_03001ebc;
        s32 off = 448;

        *(s32 *)(work + off) = 0x204;
        mode = *(s16 *)(Data_02000240 + off);
    }
    if (mode == (s32)&Value_00000044) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (Value1(Func_020052d0, 0x89c) == 0) {
                Func_020052f0();
                Func_0200523e(1);
                Func_0200537e(10, 1);
                Call3(Func_0200535c, 10, 0x5c0000, 0x780000);
                Call3(Func_020053a8, 10, 0xd000, 0);
                Call3(Func_02005342, 0, 0x6666, 0x3333);
                Call3(Func_02005364, 0, 136, 64);
                Func_02005420();
                Func_0200542c();
                Func_02005382(0);
                Func_02005338(30);
                Call3(Func_020053ec, 10, 256, 0);
                Func_020053bc(10, 2);
                Func_02005352(30);
                Func_02003354(10, 136, 116, 0x70000);
                Func_020033e2(10);
                /* image word Func_020053f6 at 0x020025d0 */
                Call2(Func_02005434, 10, 15);
                Func_0200533a(Func_02005394(10), 0);
                Call1(Func_02005378, 0x89c);
                Func_02005386(60);
                Func_0200539a();
            }
            if (Func_02005380(0x109) == 0) {
                break;
            }
            if (Func_0200538e(768) != 0) {
                break;
            }
            Func_02005434(10, 15);
            Call3(Func_02005412, 10, 0x880000, 0x740000);
            break;

        case 7:
        case 8:
        case 9:
            rec = Func_020053e2(0);
            if (rec != 0) {
                Func_02005428(16, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
            }
            /* image word Func_020053f6 at 0x02002638 */
            rec = Func_02005394(16);
            *(s32 *)(rec + 108) = 0;
            if (Func_020053d0(0x109) != 0) {
                rec = Func_0200540a(16);
                *(s32 *)(rec + 12) = 0x200000;
            }
            Func_02005346(1);
            Call3(Func_0200545c, 16, 0x2780000, 0x1b80000);
            if (Func_020053fa(0xfd4) == 0) {
                Func_02005284(16);
            }
            Func_020054a4(11, 15);
            /* image word Func_020054ac at 0x02002686 */
            Call2(Func_020054a4, 12, 15);
            Func_020053f0(Func_0200544a(11), 0);
            Func_020053fc(Func_02005456(12), 0);
            Func_02002f66(8);
            if (Func_0200543a(784) == 0) {
                Func_02002f78(9);
                break;
            }
            Func_020053ac(1);
            Call3(Func_020054c2, 9, 0x2100000, 0x1980000);
            Func_020054d2(9, 4);
            Call6(Func_02005436, 38, 27, 4, 2, 31, 25);
            /* image word Func_020054ac at 0x020026ee */
            *(u8 *)(Func_0200544a(9) + 35) = 2;
            break;
        }
    } else if (mode == (s32)&Value_00000045) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (Func_020054b4(0x303) == 0) {
                Call2(Func_02005558, 12, 15);
                Func_0200549c(Func_020054f6(12), 0);
            }
            if (Func_020054d4(772) != 0) {
                break;
            }
            Call2(Func_0200557a, 13, 15);
            Func_020056c6(Func_02005720(13), 0);
            break;

        case 10:
        case 11:
        case 12:
            if (Func_020054ec(0x311) == 0) {
                Func_0200302a(10);
            } else {
                Func_0200545e(1);
                Call3(Func_02005574, 10, 0x2280000, 0x1fe0000);
                Call2(Func_02005584, 10, 4);
                *(u8 *)(Func_0200554a(10) + 35) = 2;
                Call6(Func_020054f4, 44, 30, 2, 4, 34, 30);
                Func_02002a00(0, 35, 29, 1, 4, 0);
            }
            Func_02003082(8);
            Func_02003088(9);
            pos = *(s32 *)(Func_0200558a(11) + 8);
            tmp = *(s32 *)(Func_02005592(11) + 16);
            pos >>= 20;
            Func_02002a32(2, pos, tmp >> 20, 1, 1, 255);
            Func_020054e0(1);
            Func_02005620(11, 6);
            /* image word Func_020055be at 0x02002800 */
            {
                u8 *obj = Func_0200558a(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Func_0200559e(0x306) != 0) {
                break;
            }
            Call2(Func_02005644, 14, 15);
            Func_02005588(Func_020055e2(14), 0);
            /* image word Func_020055be at 0x02002830 */
            if (Func_0200559e(0x305) == 0) {
                break;
            }
            Call3(Func_0200563a, 14, 0x1a80000, 0x1e00000);
            Call3(Func_02005648, 17, 0x1a80000, 0x1e00000);
            break;
        }
    } else if (mode == (s32)&Value_00000046) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            Func_020055d4(1);
            if (Func_0200567a(0x307) == 0) {
                Call2(Func_0200571e, 15, 15);
                Func_02005662(Func_020056bc(15), 0);
                Func_0200566e(Func_020056c8(19), 0);
            }
            if (Func_020056a6(776) == 0) {
                Call2(Func_0200574a, 16, 15);
                Func_0200568e(Func_020056e8(16), 0);
                Func_0200569a(Func_020056f4(20), 0);
            }
            if (Func_020056d0(0x309) != 0) {
                break;
            }
            Call2(Func_02005776, 17, 15);
            Func_020056ba(Func_02005714(17), 0);
            Func_020056c6(Func_02005720(21), 0);
            break;

        case 7:
            pos = *(s32 *)(Func_0200572e(13) + 8);
            tmp = *(s32 *)(Func_02005736(13) + 16);
            pos >>= 20;
            Func_02002bd6(2, pos, tmp >> 20, 1, 1, 255);
            Call2(Func_020057be, 13, 6);
            Func_0200568c(1);
            {
                u8 *obj = Func_02005762(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            Func_02003276(8);
            break;

        case 8:
        case 9:
        case 10:
        case 11:
            Func_02003414(0x2de0000, 0, 0x1720000, 223);
            Func_02003420(0x2f20000, 0, 0x1720000, 223);
            Func_0200329a(10);
            Func_020032a0(12);
            if (Func_02005772(0x312) == 0) {
                Func_020032b0(9);
            } else {
                Func_020056e4(1);
                Func_020057fc(9, 4);
                Call3(Func_02005800, 9, 0x2ba0000, 0x18e0000);
                {
                    u8 *obj = Func_020057ce(9);
                    u32 mask = 2;
                    mask = mask | obj[35];
                    obj[35] = mask;
                }
                Call6(Func_0200577c, 26, 20, 2, 4, 42, 23);
                Func_020057cc(532);
                Call3(Func_0200583a, 14, 0x2780000, 0x1b80000);
                Func_020057ae(Func_02005808(14), 0);
            }
            if (Func_020057e4(0x313) == 0) {
                Func_02003322(11);
            } else {
                Func_02005756(1);
                Func_0200586e(11, 4);
                Call3(Func_02005870, 11, 0x29a0000, 0x2260000);
                *(u8 *)(Func_0200583e(11) + 35) = 2;
                Call6(Func_020057e8, 26, 20, 2, 4, 40, 32);
            }
            pos = *(s32 *)(Func_0200585e(14) + 8);
            tmp = *(s32 *)(Func_02005866(14) + 16);
            pos >>= 20;
            Func_02002d06(2, pos, tmp >> 20, 1, 1, 255);
            Call2(Func_020058ee, 14, 6);
            Func_020057bc(1);
            {
                u8 *obj = Func_02005892(9);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Func_02005872(0x30b) == 0) {
                Call2(Func_02005916, 18, 15);
                Func_0200585a(Func_020058b4(18), 0);
                if (Func_02005890(0x30a) != 0) {
                    Call3(Func_0200590a, 22, 0x2e80000, 0x1f80000);
                    Call3(Func_02005918, 18, 0x2e80000, 0x1f80000);
                }
            }
            Func_0200475c();
            break;

        case 12:
        case 13:
            Func_020058fe(18, 0x200b084);
            if (Func_020058c4(0x893) == 0) {
                break;
            }
            if (Func_020058ce(0x89e) == 0) {
                break;
            }
            Func_020058e0(0x88f);
            break;

        case 15:
            Func_020058e8(0x89e);
            break;
        }
    }
    return 0;
}
