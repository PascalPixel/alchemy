typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 373: configure one dialogue/portrait actor and start it.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl/fp saves at
 * 0x02003380 through the single epilogue at 0x02003436..0x02003444, followed
 * by its five-word literal pool at 0x02003448.  Straight-line code with one
 * `if`; no state escapes the return.
 *
 * All twelve calls are placed.  None is an interworking `call_via rN` site:
 * r3 holds the fourth incoming argument throughout and is never loaded with
 * a helper address.
 *
 * UNCERTAINTY: the called service addresses are the ones encoded in the
 * overlay image; overlay-local code and resident services share the
 * 0x02000000 namespace and the module's `bl` displacements are fixed up on
 * load, so the encoded address is an identity rather than a location.
 */

struct Resource373Handle {
    u8 unknown_00[0x26];
    u8 field26;
};

struct Resource373Actor {
    u8 unknown_00[0x28];
    s32 field28;
    u8 unknown_2c[0x24];
    struct Resource373Handle *handle;   /* 0x50 */
    u8 unknown_54[1];
    u8 field55;
};

void Func_0808a010();
struct Resource373Actor *Func_0808a080();
void Func_0808a090();
void Func_0808a0c0();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a1b8();

void Func_02003380(s32 actorId, s32 firstArgument, s32 secondArgument,
                   s32 emphasise)
{
    struct Resource373Actor *actor = Func_0808a080(actorId);
    struct Resource373Handle *handle = actor->handle;

    /* 0x80 << 9 and 0x80 << 8; 196 << 1 is the shared 0x188 duration. */
    Func_0808a090(actorId, 0x10000, 0x8000);
    Func_0808a0d0(actorId, 0x188, 0x35b);
    Func_0808a1b8(actorId, 0xc000, 10);

    actor->field55 = 0;
    handle->field26 = 0;

    Func_0808a100(actorId, firstArgument);
    Func_0808a090(actorId, 0x10000, 0x8000);
    Func_0808a0c0(actorId, 0x188, 0x36b);
    Func_0808a010(10);
    Func_0808a090(actorId, 0x4ccc, 0x2666);
    Func_0808a100(actorId, secondArgument);
    Func_0808a0c0(actorId, 0x188, 0x37a);

    actor->field28 = 0x20000;       /* 0x80 << 10 */
    handle->field26 = 1;
    if (emphasise != 0) {
        actor->field55 = 3;
    }

    Func_0808a100(actorId, 1);
}
