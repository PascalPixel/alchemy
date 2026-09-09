#include "types.h"

/*
 * resource_371 owner at 0x02001ca4, 1,488 bytes.
 *
 * A field scene step for actor 8.  It reads the party-leader record (slot 0),
 * halves the distance between that record's x/y and the fixed anchor
 * (0x15d00000, 0x05300000) to get the position the camera and the actor are
 * driven to, then branches on game flag 0x16e:
 *
 *   - flag clear: the long first-visit presentation.  It sets 0x16e, plays
 *     the approach animation, walks actor 8 to the anchor, runs a 60-frame
 *     alternating-mode idle driven by the shared frame counter at 0x03001e40,
 *     shows message 0xc4f, then pages messages 0xc5c..0xc62 while the reader
 *     keeps confirming (message 0xc63 when the reader cancels), draws a
 *     quantity, and finishes with message 0xc64/0xc65.
 *   - flag set: the short repeat-visit branch.  It shows message 0xc68/0xc6a,
 *     sets 0x16f and clears 0x171, and asks two yes/no questions.  Both
 *     accepted answers fall through to the shared tail; a refusal runs the
 *     decline block, which sets 0x16f and 0x171, plays cue 42 and clears
 *     0x16e/0x16f/0x171 again.
 *
 * Both accepted paths end by calling the neighbouring owner at 0x02001c08 and
 * the 0x0808a3e0 finish helper.
 *
 * Uncertain: the argument roles of the 0x0808axxx scene-script helpers, the
 * meaning of the actor-record fields at +6, +20, +40, +48, +52, +72, +85,
 * +102 and +108, and whether 0x100000 in the second position call is a speed
 * or a flag word.  The cue/message numbers are reproduced, not interpreted.
 *
 * Residual against the reference: 1,488 of 1,488 bytes, topology equal, 128
 * differing halfwords, 42 wrong instructions.  What is left is scheduling and
 * allocation noise - the counter decrement drifting one store earlier in each
 * of the four wait loops, the argument-setup order of a handful of otherwise
 * identical calls, the register chosen for the +85 mode pointer and for the
 * two pool constants, and the one interworking branch described below.  No
 * branch, loop, call, argument or store is missing.
 *
 * Read that figure with the caveat below: the call-shape helpers are a source
 * spelling device, not recovered structure.  The same statements written as
 * plain direct calls measure 1,496 bytes and 317 differing halfwords, so
 * roughly three fifths of the apparent agreement comes from the helpers and
 * from the block-scoped constant temporaries, not from recovered source.  The
 * control flow, calls, arguments and stores below are evidence; the residual
 * number is not a claim about how close the original spelling is.
 */

#define FieldScene_RunScene371_02001ca4 Func_02001ca4

/*
 * Loader-relocated overlay calls.  Each symbol names a pre-relocation call
 * word the image holds; every site that reaches the same runtime veneer is
 * spelled through one symbol, and the comment records the main-image function
 * the veneer forwards to.
 */
void Func_02003db0();  /* resource_371 owner 0x02001c08, FieldScene_RunScene371_02001c08 */
void Func_02005eb6();  /* resource_371 owner 0x02004058 */
void Func_02005eea();  /* main:080000c0  wait N frames */
void Func_02005fd4();  /* main:08009150  Object_SetPosition */
void Func_02006050();  /* main:08009240 */
void Func_020062c2();  /* main:08015040  UiText_DrawMessage */
void Func_0200614c();  /* main:08015120  UiText_DrawQuantity */
void Func_02006392();  /* main:080153e8 */
s32 Func_02005f7c();   /* main:080770c0  GameFlag_IsSet */
void Func_02005f98();  /* main:080770c8  GameFlag_Set */
void Func_020064ca();  /* main:080770d0  GameFlag_Clear */
s32 Func_02005fda();   /* main:080771a8 */
s32 Func_02005fec();   /* main:080771c8 */
void Func_02005fc0();  /* main:08077260 */
void Func_02006018();  /* main:0808a010 */
void Func_02005fe4();  /* main:0808a018 */
void Func_0200650a();  /* main:0808a020 */
s32 Func_02006454();   /* main:0808a070  reader confirmed */
s32 Func_02005fb2();   /* main:0808a080  Scene_GetRecord */
void Func_020061f6();  /* main:0808a0d0 */
void Func_02006078();  /* main:0808a0f0 */
void Func_0200621e();  /* main:0808a100 */
void Func_0200624a();  /* main:0808a128 */
void Func_02006202();  /* main:0808a138 */
void Func_020060ca();  /* main:0808a148 */
void Func_02006110();  /* main:0808a170  show message */
s32 Func_0200637a();   /* main:0808a178 */
void Func_02006128();  /* main:0808a180 */
void Func_02006122();  /* main:0808a1e8 */
void Func_02006190();  /* main:0808a3d8 */
void Func_020061dc();  /* main:0808a3e0 */
void Func_02006284();  /* main:0808a428 */
void Func_020062ea();  /* main:0808a440 */
void Func_020066a0();  /* main:0808a4f8 */
void Func_0200620c();  /* main:0808a5c0 */
void Func_02006218();  /* main:0808a5c8 */
void Func_020065ba();  /* main:080a1040 */
void Func_020062ac();  /* main:080f9010  Audio_PlayCue */

/*
 * The site at 0x020021d6 reaches main:0808a110 through the runtime veneer at
 * 0x0200c36c.  Its pre-relocation call word decodes to the same legacy name
 * as the neighbouring main:0808a128 site, so the overlay call table cannot
 * bind it by name without a translation-unit alias the integrator owns.  It
 * is spelled here through the untagged runtime address instead, which keeps
 * the call, its arguments and the owner extent honest at the cost of one
 * differing halfword (the linker emits the interworking form of the branch).
 */
void Data_0200c36c();  /* main:0808a110 */

/* Readable spellings for the veneers whose engine target the project names. */
#define Scene_GetRecord_Veneer Func_02005fb2
#define Object_SetPosition_Veneer Func_02005fd4
#define GameFlag_IsSet_Veneer Func_02005f7c
#define GameFlag_Set_Veneer Func_02005f98
#define GameFlag_Clear_Veneer Func_020064ca
#define UiText_DrawMessage_Veneer Func_020062c2
#define UiText_DrawQuantity_Veneer Func_0200614c
#define Audio_PlayCue_Veneer Func_020062ac
#define ShowMessage_Veneer Func_02006110
#define WaitFrames_Veneer Func_02005eea

/* The shared frame counter the idle loop samples. */
#define FRAME_COUNTER (*(s32 *)0x03001e40)

/* The actor this scene step drives, and the party-leader record slot. */
#define ACTOR 8
#define LEADER 0

/* The anchor the actor and camera are driven to, in 16.16 field units. */
#define ANCHOR_X 0x15d00000
#define ANCHOR_Y 0x05300000

/* Call-shape helpers, following the convention already used by most of the
 * drafted overlay scene files in this directory.  A site spelled through one
 * of these passes its constants straight into the argument registers, while a
 * direct call precomputes a costly constant into a pseudo the compiler then
 * shares with later uses in the block; a value-returning helper also sets r0
 * last of its arguments.
 *
 * These are a reading and spelling aid, not a recovered interface: the
 * original almost certainly wrote every site as a plain direct call.  Which
 * helper a site uses is chosen per call site to follow the reference's
 * argument-setup order, so the mixture below carries no meaning.  Every
 * function keeps one declared type across the whole file; no site casts a
 * veneer to a different signature. */

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunScene371_02001ca4(void)
{
    s32 rec8;
    s32 record;
    s32 mid_x;
    s32 mid_y;
    s32 cnt;
    s32 page;
    s32 text;
    u8 *mode;

    rec8 = Value1(Scene_GetRecord_Veneer, ACTOR);
    record = Scene_GetRecord_Veneer(LEADER);
    mid_x = (*(s32 *)(record + 8) - ANCHOR_X) / 2 + ANCHOR_X;
    mid_y = (*(s32 *)(record + 16) - ANCHOR_Y) / 2 + ANCHOR_Y;
    if (Value1(GameFlag_IsSet_Veneer, 0x16e) == 0) {
        /* First visit: claim the flag and play the long presentation. */
        Func_02005fc0(1);
        Call1(GameFlag_Set_Veneer, 0x16e);
        Func_02005fe4();
        record = Value1(Scene_GetRecord_Veneer, LEADER);
        if (record != 0) {
            Func_02006078(ACTOR, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Value3(Func_02005fda, 0, 0, 0);
        Value3(Func_02005fec, 0, 0, 0);
        Func_02006190();
        Func_020060ca(0, ACTOR, 0);
        Func_02006018(10);
        Call3(Func_02006122, 0, 0x101, 60);
        {
            s32 shown = 1;

            *(u16 *)(rec8 + 102) = shown;
        }
        Func_020060ca(ACTOR, 0, 0);
        WaitFrames_Veneer(16);
        Call1(ShowMessage_Veneer, 0xc4f);
        Func_02006128(ACTOR, 0);
        Func_020061dc();
        Call2(Func_0200620c, 0x13333, 6);
        Func_02006218();
        Func_02006190();
        mode = (u8 *)(rec8 + 85);
        *mode = 2;
        *(s32 *)(rec8 + 72) = 0x4000;
        *(s32 *)(rec8 + 48) = 0x10000;
        *(s32 *)(rec8 + 52) = 0x10000;
        *(s32 *)(rec8 + 40) = 0;
        *(s32 *)(rec8 + 20) = 0;
        Object_SetPosition_Veneer(rec8, ANCHOR_X, 0, ANCHOR_Y);
        cnt = 15;
        do {
            *(s32 *)(rec8 + 24) += 0x800;
            *(s32 *)(rec8 + 28) += 0x800;
            cnt = cnt - 1;
            WaitFrames_Veneer(1);
        } while (cnt >= 0);
        Func_020060ca(ACTOR, 0, 0);
        Func_020060ca(0, ACTOR, 0);
        WaitFrames_Veneer(16);
        *(s32 *)(rec8 + 108) = 0;
        Func_02006050(rec8, 0);
        *(s32 *)(rec8 + 72) = 0x10000;
        Call2(Func_02006128, ACTOR, 0);
        Audio_PlayCue_Veneer(131);
        Func_02006284(140, 0);
        /* Sixty frames of the alternating idle mode, refreshed every 16. */
        cnt = 59;
        do {
            if ((FRAME_COUNTER & 2) != 0) {
                Func_02006050(rec8, 7);
            } else {
                Func_02006050(rec8, 0);
            }
            if ((FRAME_COUNTER & 15) == 0) {
                Func_02005eb6(rec8);
            }
            cnt = cnt - 1;
            WaitFrames_Veneer(1);
        } while (cnt >= 0);
        Func_020062ea();
        Func_02006050(rec8, 0);
        Func_02006202(ACTOR, 2);
        Func_02006128(ACTOR, 0);
        Call3(Func_02006122, 0, 0x102, 30);
        Func_02006128(ACTOR, 0);
        Call3(Func_02006122, 0, 0x101, 30);
        Func_020061f6(ACTOR, mid_x >> 16, mid_y >> 16);
        Func_0200621e(0, 22);
        Func_02006128(ACTOR, 0);
        Call3(Func_02006122, 0, 0x101, 40);
        Func_0200624a(ACTOR, 4, 30);
        Call2(UiText_DrawQuantity_Veneer, 0x12c, 4);
        Func_02006128(ACTOR, 0);
        Call3(Func_02006122, 0, 0x100, 30);
        Func_02006128(ACTOR, 0);
        Func_02006202(0, 2);
        Func_02006128(ACTOR, 0);
        Func_0200624a(ACTOR, 2, 30);
        page = 0;
        Func_02006128(ACTOR, 0);
        *mode = page;
        Call4(Object_SetPosition_Veneer, rec8, mid_x, 0x100000, mid_y);
        cnt = 15;
        do {
            *(u16 *)(rec8 + 6) += 0x1000;
            cnt = cnt - 1;
            WaitFrames_Veneer(1);
        } while (cnt >= 0);
        Func_0200621e(0, 1);
        Call2(Func_02006128, ACTOR, 0);
        *mode = 2;
        *(s32 *)(rec8 + 40) = 0;
        *(s32 *)(rec8 + 20) = 0;
        cnt = 7;
        do {
            *(u16 *)(rec8 + 6) += 0x1000;
            cnt = cnt - 1;
            WaitFrames_Veneer(1);
        } while (cnt >= 0);
        Func_0200621e(0, 22);
        Func_02006128(ACTOR, 0);
        Call3(Func_02006122, ACTOR, 0x102, 30);
        Func_020060ca(ACTOR, 0, 0);
        Func_02006202(ACTOR, 2);
        Func_02006128(ACTOR, 0);
        Func_0200624a(ACTOR, 2, 30);
        Func_0200637a(ACTOR, 0);
        /* Page through 0xc5c..0xc62 while the reader keeps confirming. */
        page = 0;
    page_loop:
        if (Value2(Func_02006454, 0, 0) == 1) {
            Func_0200624a(ACTOR, 2, 20);
            Func_0200624a(ACTOR, 2, 20);
            if (page == 6) {
                Call1(ShowMessage_Veneer, 0xc62);
                Func_02006128(ACTOR, 0);
                goto paged;
            }
            Func_02006110(page + 0xc5c);
            Func_0200637a(ACTOR, 0);
            page = page + 1;
            goto page_loop;
        }
        /* The reader cancelled before the last page. */
        Func_0200621e(0, 22);
        Func_0200624a(ACTOR, 2, 20);
        Func_0200624a(ACTOR, 4, 20);
        Call1(ShowMessage_Veneer, 0xc63);
        Func_02006128(ACTOR, 0);
    paged:
        Call2(UiText_DrawQuantity_Veneer, 0x12c, 4);
        Audio_PlayCue_Veneer(81);
        text = 0xc64;
        Call2(UiText_DrawMessage_Veneer, text, 3);
        text = text + 1;
        Call1(ShowMessage_Veneer, text);
        Func_0200624a(ACTOR, 2, 20);
        Call2(Func_02006128, ACTOR, 0);
        Audio_PlayCue_Veneer(9);
        goto play;
    }
    /* Repeat visit: the short branch with two confirmations. */
    Func_02005fe4();
    record = Value1(Scene_GetRecord_Veneer, LEADER);
    if (record != 0) {
        Func_02006078(ACTOR, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    *(s32 *)(rec8 + 40) = 0xa0000;
    Object_SetPosition_Veneer(rec8, mid_x, 0, mid_y);
    Func_02006018(30);
    Func_02006190();
    Func_020060ca(ACTOR, 0, 0);
    Func_020060ca(0, ACTOR, 0);
    Func_0200621e(0, 22);
    Call1(ShowMessage_Veneer, 0xc68);
    Func_0200624a(ACTOR, 2, 20);
    Func_0200624a(ACTOR, 2, 20);
    Func_02006128(ACTOR, 0);
    Func_02006202(ACTOR, 2);
    Call2(Func_02006128, ACTOR, 0);
    Audio_PlayCue_Veneer(111);
    Func_02006392(0, 2);
    Call1(GameFlag_Set_Veneer, 0x16f);
    Call1(GameFlag_Clear_Veneer, 0x171);
    Func_020065ba();
    Call1(ShowMessage_Veneer, 0xc6a);
    Object_SetPosition_Veneer(rec8, ANCHOR_X, 0, ANCHOR_Y);
    Func_02006018(30);
    Func_02006128(ACTOR, 0);
    Func_020060ca(ACTOR, 0, 0);
    Func_02006128(ACTOR, 0);
    Value2(Func_0200637a, ACTOR, 0);
    if (Value2(Func_02006454, 0, 0) == 1) {
        Func_0200621e(0, 22);
        Func_02006202(ACTOR, 2);
        Call1(ShowMessage_Veneer, 0xc6d);
        Value2(Func_0200637a, ACTOR, 0);
        if (Value2(Func_02006454, 0, 0) != 1) {
            Func_02006128(ACTOR, 0);
            Func_020061f6(ACTOR, mid_x >> 16, mid_y >> 16);
        play:
            Func_02003db0();
            Func_020061dc();
            return;
        }
    }
    /* Declined: undo the branch flags and leave. */
    Func_0200621e(0, 22);
    Call1(ShowMessage_Veneer, 0xc6f);
    Func_0200624a(ACTOR, 2, 20);
    Func_0200624a(ACTOR, 2, 20);
    Data_0200c36c(0, 3);
    Call3(Func_02006122, ACTOR, 0x100, 30);
    Call2(Func_02006128, ACTOR, 0);
    Call1(GameFlag_Set_Veneer, 0x16f);
    Call1(GameFlag_Set_Veneer, 0x171);
    Func_020065ba();
    Func_0200624a(ACTOR, 2, 20);
    Func_02006128(ACTOR, 0);
    Func_020061dc();
    Func_020066a0(ACTOR, 0, 0);
    Audio_PlayCue_Veneer(42);
    Func_0200650a();
    Call1(GameFlag_Clear_Veneer, 0x16e);
    Call1(GameFlag_Clear_Veneer, 0x16f);
    Call1(GameFlag_Clear_Veneer, 0x171);
}
