typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 373: long scripted cutscene beat driven entirely by service calls.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020012bc through the single
 * `pop {r5, r6} ; pop {r0} ; bx r0` at 0x02001424..0x02001428, followed by
 * its ten-word literal pool at 0x0200142c.  Straight-line code, no branches,
 * nothing live past the return.
 *
 * All thirty-five calls are placed below in order.  None is an interworking
 * `call_via rN` site: r3 is never loaded with a helper address in this
 * routine.
 *
 * Immediates are spelled as the values the reference materialises with a
 * `movs`/`lsls` pair, with the pair shown in a comment where it is not
 * obvious.
 *
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image; overlay-local code and resident services share the
 * 0x02000000 namespace and the module's `bl` displacements are fixed up on
 * load, so an encoded address is an identity rather than a location.
 * UNCERTAINTY 2: the two sites that encode Func_0200737e disagree about
 * arity — one passes three registers, the other passes two with r2 left
 * clobbered by the preceding call.  Rather than invent a third argument the
 * declaration is left prototype-less; the pair may well be two different
 * services that the load-time fixup separates.
 * UNCERTAINTY 3: 0x0200d72d is an odd (Thumb) address published into the
 * actor's 0x6c behaviour slot; its signature is not established here.
 */

struct Resource373Actor {
    u8 unknown_00[0x5a];
    u8 flags5a;
};

struct Resource373Published {
    u8 unknown_00[0x6c];
    void (*behaviour)(void);
};

void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
struct Resource373Published *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a128();
void Func_0808a130();
void Func_0808a170();
void Func_0808a190();
void Func_0808a1b8();
void Func_0808a1e8();

void Func_020012bc(void)
{
    struct Resource373Actor *actor = Func_0808a080(22);
    u8 *flags = &actor->flags5a;
    struct Resource373Published *published;

    Func_0808a018();
    Func_0808a090(0, 0x20000, 0x20000);     /* 0x80 << 10 twice */
    Func_0808a128(0, 5, 0);
    Func_0808a0c8(0, 215, 0x193);

    *flags = (u8)(*flags | 1);

    Func_0808a0f0(22, 0x00a60000, 0x01770000);
    Func_0808a1b8(22, 0x2000, 20);          /* 0x80 << 6 */

    *flags = (u8)(*flags ^ 1);

    Func_0808a090(22, 0x28000, 0x28000);    /* 0xa0 << 10 twice */
    Func_0808a128(22, 4, 0);
    Func_0808a0d0(22, 202, 0x18b);
    Func_0808a100(0, 1);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xb000, 0);            /* 0xb0 << 8 */
    Func_0808a1b8(22, 0x3000, 24);          /* 0xc0 << 6 */
    Func_0808a1e8(0, 0x100, 0);             /* 0x80 << 1 */
    Func_0808a130(0, 2);
    Func_0808a010(20);
    Func_0808a090(22, 0x18000, 0x10000);    /* 0xc0 << 9, 0x80 << 9 */
    Func_0808a098(0, (const void *)0x0200f59c);
    Func_0808a010(10);
    Func_0808a1e8(22, 0x103, 0);
    Func_0808a098(22, (const void *)0x0200f5ec);
    Func_0808a0a0(0);
    Func_0808a0d0(0, 0x100, 0x1da);         /* 0x80 << 1, 0xed << 1 */
    Func_0808a1b8(0, 0xc000, 0);            /* 0xc0 << 8 */
    Func_0808a0a0(22);
    Func_0808a0d0(22, 0x100, 0x1c8);        /* 0x80 << 1, 0xe4 << 1 */
    Func_0808a100(0, 1);
    Func_0808a1b8(22, 0x4000, 20);          /* 0x80 << 7 */
    Func_0808a130(22, 2);
    Func_0808a010(20);
    Func_0808a170(0xfce);
    Func_0808a190(22, 0);

    published = Func_0808a080(22);
    published->behaviour = (void (*)(void))0x0200d72d;

    Func_0808a098(22, (const void *)0x0200e248);
    Func_080770c8(0x823);
    Func_0808a020();
}
