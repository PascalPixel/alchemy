#include "types.h"

#define FieldScene_RunSecondEnsembleBeat Func_02001494

/*
 * resource_39e owner at 0x02001494, 2236 bytes.
 *
 * Second ensemble beat of this overlay's scene script: 233 calls into the
 * shared scene-script helper block, then a two-stage story branch.
 *
 *   - the opening call sets story flag 0x89a;
 *   - stage 1 (Func_0808a070(0, 0) == 0) joins with id 16 and sets flag 0x898
 *     -- the flag the dispatcher at 0x020012e0 tests and 0x02001dbc clears;
 *   - stage 1 otherwise bumps the step counter at +472 of the scene work
 *     record and runs stage 2: its ==0 arm joins with id 18 and also sets
 *     0x898, its else arm bumps that counter again and sets flag 0x899.
 *
 * Both join arms share the tail at 0x02001cb4: Func_0808a188(id, 0, 20) with
 * the id carried in r0, then Func_080770c8(0x898).
 *
 * Midway the beat clears then sets bit 0 of byte +90 of record 16 around a
 * Func_0808a0d0 reposition, plays sounds 158/159 with Func_08009178 text
 * calls, runs the slot-19/20 sequence (Func_0808a0f0(19/20, 232 << 16,
 * 168 << 16), record +12 = 0xc0000, +60 = 0x80000000, +24 = 0xcccc, +30 of
 * the +80 sub-object = 0x8000, sound 124) that also closes the owners at
 * 0x02001160 and 0x02001dbc, and ends with Func_0808a098(12, 0x0200c638) as
 * 0x02001dbc does.
 *
 * Complete owner: `push {r5, lr}` at 0x02001494 through `pop {r5} / pop {r0} /
 * bx r0` at 0x02001d42-0x02001d46, two inline pools plus two trailing pool
 * words ending 0x02001d4f; the next owner (0x02001d50, `push {lr}`) follows.
 * Called once from within this overlay.
 *
 * Call binding. This overlay reaches the shared helpers through its own veneer
 * bank, so every emitted call word names a veneer, not the main-image address.
 * The legacy alias a reference site produces is derived from that site's own
 * position, so one alias can name two different veneers and several aliases
 * can name one veneer. Each helper is therefore declared once, under an alias
 * whose reference sites all resolve to that helper's veneer, and is spelled in
 * the body under the main-image function it reaches. The bound veneer -- and
 * so the emitted call word -- depends only on the alias, never on where the
 * call lands in the candidate.
 *
 * Two call forms in the branch tail carry meaning beyond their arguments. The
 * three sites spelled through Value2 keep the callee's result register live
 * across the argument setup, which orders the r0 write after r1; the sites
 * spelled through Call2 discard it and order r0 first. Both forms appear in
 * the sibling beat at 0x02001dbc.
 *
 * The set of bit 0 in byte +90 reads the byte into its own local before the
 * or, as the staged-actor scene at resource_373:02003fb0 does at the same
 * offset; the paired clear needs no local because an and against 0xfe already
 * holds its mask in a word register.
 *
 * Uncertainty: helper roles and argument roles are read from call shape only;
 * ids, dialogue ids and constants are transcribed. 0x0200c77a, 0x0200c790 and
 * 0x0200c638 are overlay-image data pointers whose contents are unidentified.
 */

/* Veneer aliases this overlay binds; see Call binding above. */
void Func_02005934();
void Func_0200585e();
void Func_02005874();
s32 Func_02006026();
u8 *Func_020059c6();
void Func_02005e26();
void Func_02005976();
void Func_0200615c();
void Func_02005b04();
void Func_020059a0();
void Func_02005b30();
void Func_02005e60();
void Func_02006184();
void Func_02005c54();
void Func_02005ae8();
void Func_02005a1c();
void Func_02005926();
void Func_02005990();
s32 Func_020060ce();
void Func_020059b2();
void Func_02005a06();
void Func_02005c94();
void Func_0200599c();
void Func_02005a34();
void Func_02005c80();

/* The main-image helper each veneer above reaches. */
#define Func_08009178 Func_02005934
#define Func_080770c8 Func_0200585e
#define Func_0808a010 Func_02005874
#define Func_0808a070 Func_02006026
#define Func_0808a080 Func_020059c6
#define Func_0808a088 Func_02005e26
#define Func_0808a090 Func_02005976
#define Func_0808a098 Func_0200615c
#define Func_0808a0c8 Func_02005b04
#define Func_0808a0d0 Func_020059a0
#define Func_0808a0e8 Func_02005b30
#define Func_0808a0f0 Func_02005e60
#define Func_0808a100 Func_02006184
#define Func_0808a110 Func_02005c54
#define Func_0808a130 Func_02005ae8
#define Func_0808a138 Func_02005a1c
#define Func_0808a148 Func_02005926
#define Func_0808a170 Func_02005990
#define Func_0808a178 Func_020060ce
#define Func_0808a188 Func_020059b2
#define Func_0808a190 Func_02005a06
#define Func_0808a1b8 Func_02005c94
#define Func_0808a1e8 Func_0200599c
#define Func_0808a200 Func_02005a34
#define Func_080f9010 Func_02005c80

/* Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo that the compiler then shares with later uses in the block. A
 * value-returning site also writes r0 last of its arguments. The same wrappers
 * carry the sibling beat at 0x02001dbc. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ u8 *Record1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at +472 of the shared scene work record. */
static __inline__ void Scene_BumpStep(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + amount);
}

void FieldScene_RunSecondEnsembleBeat(void)
{
    s32 id;
    u8 *rec;

    Call1(Func_080770c8, 0x89a);
    Call1(Func_0808a010, 30);
    Call3(Func_0808a148, 13, 0, 0);
    Call3(Func_0808a148, 15, 0, 0);
    Call3(Func_0808a148, 16, 0, 0);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a1e8, 13, 128 << 1, 0);
    Call3(Func_0808a1e8, 15, 128 << 1, 0);
    Call3(Func_0808a1e8, 16, 128 << 1, 0);
    Call1(Func_0808a010, 60);
    Call1(Func_0808a170, 0x183b);
    Call3(Func_0808a188, 13, 0, 20);
    Call3(Func_0808a148, 0, 13, 0);
    Call2(Func_0808a138, 15, 1);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 15, 0, 20);
    Call3(Func_0808a148, 0, 15, 0);
    Call2(Func_0808a138, 16, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a148, 0, 16, 0);
    Call2(Func_0808a190, 16, 0);
    Call1(Func_0808a010, 50);
    Call2(Func_0808a200, 16, 1);
    Call3(Func_0808a090, 16, 0xcccc, 0x6666);
    Call3(Func_0808a0d0, 16, 176, 248);
    Call3(Func_0808a0d0, 16, 154 << 1, 248);
    Call3(Func_0808a1b8, 0, 128 << 6, 0);
    Call3(Func_0808a1b8, 16, 192 << 8, 20);
    Call1(Func_080f9010, 158);
    Call3(Func_08009178, 0x0200c77a, 78, 13);
    Call2(Func_0808a138, 16, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a090, 16, 192 << 9, 192 << 8);
    rec = Record1(Func_0808a080, 16);
    rec[90] &= 0xfe;
    Call3(Func_0808a0d0, 16, 154 << 1, 136 << 1);
    Call1(Func_0808a010, 1);
    {
        u8 *record = Record1(Func_0808a080, 16);
        u8 value = *(volatile u8 *)&record[90]; /* keeps the byte in its own register */

        record[90] = (u8)(value | 1);
    }
    Call3(Func_0808a188, 16, 0, 50);
    Call3(Func_0808a0f0, 17, 152 << 17, 216 << 16);
    Call3(Func_0808a0d0, 17, 152 << 1, 248);
    Call3(Func_0808a148, 9, 17, 0);
    Call3(Func_0808a148, 10, 17, 0);
    Call3(Func_0808a148, 11, 17, 0);
    Call3(Func_0808a148, 12, 17, 0);
    Call3(Func_0808a148, 13, 17, 0);
    Call3(Func_0808a148, 14, 17, 0);
    Call3(Func_0808a148, 15, 17, 0);
    Call3(Func_0808a148, 16, 17, 0);
    Call3(Func_0808a148, 0, 17, 0);
    Call1(Func_0808a010, 10);
    Call2(Func_0808a130, 9, 2);
    Call2(Func_0808a130, 10, 2);
    Call2(Func_0808a130, 11, 2);
    Call2(Func_0808a130, 12, 2);
    Call2(Func_0808a130, 13, 2);
    Call2(Func_0808a130, 14, 2);
    Call2(Func_0808a130, 15, 2);
    Call2(Func_0808a138, 16, 2);
    Call3(Func_0808a1e8, 17, 0x103, 60);
    Call3(Func_0808a0f0, 18, 152 << 17, 216 << 16);
    Call3(Func_0808a0c8, 18, 152 << 1, 248);
    Call3(Func_0808a0c8, 17, 140 << 1, 132 << 1);
    Call1(Func_0808a0e8, 18);
    Call3(Func_0808a1b8, 18, 160 << 7, 0);
    Call1(Func_0808a0e8, 17);
    Call1(Func_080f9010, 159);
    Call3(Func_08009178, 0x0200c790, 78, 13);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a148, 9, 17, 0);
    Call3(Func_0808a148, 10, 17, 0);
    Call3(Func_0808a148, 11, 17, 0);
    Call3(Func_0808a148, 12, 17, 0);
    Call3(Func_0808a148, 13, 17, 0);
    Call3(Func_0808a148, 14, 17, 0);
    Call3(Func_0808a148, 15, 17, 0);
    Call3(Func_0808a148, 16, 17, 0);
    Call3(Func_0808a148, 0, 17, 0);
    Call1(Func_0808a010, 10);
    Call2(Func_0808a138, 17, 2);
    Call1(Func_0808a010, 20);
    Call2(Func_0808a110, 18, 4);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 17, 3);
    Call1(Func_0808a010, 20);
    Call2(Func_0808a138, 18, 1);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a1b8, 17, 208 << 8, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call3(Func_0808a1e8, 18, 0x102, 60);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 17, 3);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 18, 3);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 17, 3);
    Call1(Func_0808a010, 20);
    Call2(Func_0808a110, 18, 4);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a138, 17, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a1b8, 17, 0, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call3(Func_0808a1b8, 16, 128 << 8, 20);
    Call2(Func_0808a110, 16, 3);
    Call1(Func_0808a010, 20);
    Call2(Func_0808a110, 17, 3);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 16, 3);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 16, 0, 20);
    Call3(Func_0808a1b8, 17, 128 << 8, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 9, 3);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 9, 0, 20);
    Call3(Func_0808a1b8, 17, 208 << 8, 20);
    Call2(Func_0808a138, 17, 1);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call3(Func_0808a1e8, 18, 0x102, 60);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a1e8, 17, 0x101, 60);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 18, 3);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a1e8, 17, 0x100, 60);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a110, 18, 4);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a1e8, 17, 0x103, 60);
    Call3(Func_0808a188, 17, 0, 20);
    Call3(Func_0808a1e8, 18, 0x100, 60);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 17, 4);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 17, 0, 20);
    Call2(Func_0808a138, 18, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a138, 17, 2);
    Call1(Func_0808a010, 10);
    Call3(Func_0808a0d0, 17, 128 << 1, 140 << 1);
    Call3(Func_0808a1b8, 17, 128 << 7, 20);
    Call3(Func_0808a0f0, 17, 0, 0);
    Call1(Func_0808a088, 17);
    Call1(Func_0808a010, 30);
    Call2(Func_0808a138, 9, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 9, 0, 20);
    Call2(Func_0808a138, 15, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 15, 0, 20);
    Call3(Func_0808a148, 16, 18, 0);
    Call1(Func_0808a010, 20);
    Call2(Func_0808a138, 16, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 16, 0, 20);
    Call3(Func_0808a148, 18, 16, 0);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 18, 4);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call2(Func_0808a110, 18, 3);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a090, 18, 0xcccc, 0x6666);
    Call3(Func_0808a0d0, 18, 128 << 1, 248);
    Call3(Func_0808a1b8, 18, 192 << 8, 20);
    Call2(Func_0808a130, 18, 1);
    Call3(Func_0808a1e8, 18, 0x100, 60);
    Call3(Func_0808a0d0, 18, 240, 184);
    Call2(Func_0808a138, 18, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a0f0, 19, 232 << 16, 168 << 16);
    Call3(Func_0808a0f0, 20, 232 << 16, 168 << 16);
    rec = Record1(Func_0808a080, 19);
    *(s32 *)(rec + 12) = 0xc0000;
    rec = Record1(Func_0808a080, 19);
    *(s32 *)(rec + 60) = -0x80000000;
    rec = Record1(Func_0808a080, 19);
    *(s32 *)(rec + 24) = 0xcccc;
    rec = Record1(Func_0808a080, 19);
    {
        u8 *target = *(u8 **)(rec + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Call1(Func_080f9010, 124);
    Call3(Func_0808a188, 18, 0, 20);
    Call3(Func_0808a1b8, 0, 192 << 8, 20);
    Call3(Func_0808a0d0, 16, 128 << 1, 240);
    Call3(Func_0808a1b8, 16, 176 << 8, 20);
    Call2(Func_0808a138, 16, 1);
    Call3(Func_0808a188, 16, 0, 20);
    Call3(Func_0808a148, 9, 0, 0);
    Call3(Func_0808a148, 10, 0, 0);
    Call3(Func_0808a148, 11, 0, 0);
    Call3(Func_0808a148, 12, 0, 0);
    Call3(Func_0808a148, 13, 0, 0);
    Call3(Func_0808a148, 14, 0, 0);
    Call3(Func_0808a148, 15, 0, 0);
    Call3(Func_0808a148, 16, 0, 0);
    Call2(Func_0808a138, 18, 2);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a1b8, 18, 160 << 7, 20);
    Call3(Func_0808a0d0, 18, 248, 208);
    Call3(Func_0808a1b8, 18, 160 << 7, 20);
    Value2(Func_0808a178, 18, 0);
    if (Value2(Func_0808a070, 0, 0) == 0) {
        Value2((s32 (*)())Func_0808a138, 16, 1);
        Call1(Func_0808a010, 20);
        id = 16;
        goto joinBeat;
    }

    /* Skipped once: bump the workspace skip counter and offer the beat again. */
    Scene_BumpStep(1);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a1e8, 18, 0x105, 60);
    Call3(Func_0808a1b8, 18, 128 << 7, 20);
    Call2(Func_0808a138, 16, 2);
    Call1(Func_0808a010, 20);
    Value2(Func_0808a178, 16, 0);
    if (Value2(Func_0808a070, 0, 0) != 0) {
        goto skipTwice;
    }
    Call2(Func_0808a110, 16, 3);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a1b8, 18, 176 << 8, 20);
    id = 18;

joinBeat:
    Call3(Func_0808a188, id, 0, 20);
    Call1(Func_080770c8, 0x898);
    goto finish;

skipTwice:
    Scene_BumpStep(1);
    Call1(Func_0808a010, 20);
    Call2(Func_0808a110, 18, 4);
    Call1(Func_0808a010, 20);
    Call3(Func_0808a188, 18, 0, 20);
    Call1(Func_080770c8, 0x899);

finish:
    Call3(Func_0808a1b8, 10, 128 << 8, 0);
    Call3(Func_0808a1b8, 11, 128 << 8, 20);
    Call2(Func_0808a100, 10, 5);
    Call2(Func_0808a100, 11, 5);
    Func_0808a098(12, 0x0200c638);
}
