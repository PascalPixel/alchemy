#include "types.h"

/*
 * Resource 3b1 scene owner at 0x02004254 (892 bytes).
 *
 * The two-stage saved-register prologue is at 0x02004254-0x02004258 and the
 * interworking epilogue is the pop/mov sequence at 0x02004592-0x0200459a.
 * Thirteen directly referenced literal pool words start at 0x0200459c and
 * occupy 0x0200459c-0x020045cf.  The next independent prologue begins at
 * 0x020045d0, so the complete owner is 0x37c (892) bytes.  The branch at
 * 0x020042dc selects the 0x300-flagged setup or the alternate 0x1eb7
 * sequence; the three nested 0x92b/0x92a/0x929 tests rejoin at 0x020044a2.
 * All 83 resolver-confirmed calls are represented in ordinary C.
 */

extern void Func_080000c0(s32 frames);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 y);
extern void Func_0808a0c8(s32 actor, s32 x, s32 y);
extern void Func_0808a0d0(s32 actor, s32 x, s32 y);
extern void Func_0808a0f0(s32 actor, s32 x, s32 y);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 mode);
extern void Func_0808a130(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a150(s32 actor, s32 target, s32 frames);
extern void Func_0808a170(s32 value);
extern void Func_0808a188(s32 actor, s32 value, s32 frames);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1e8(s32 actor, s32 animation, s32 frames);
extern void Func_0808a1f0(s32 actor, s32 animation);
extern void Func_0808a248(s32 actor);
extern void Func_0808a360(void);

extern void Func_020037d8(s32 value);
extern void Func_0200486c(s32 id);
extern void Func_02004880(s32 id, s32 value);
extern void Func_02004890(s32 id, s32 x, s32 y, s32 mode);
extern s32 Func_02004fa8(s32 kind, s32 flag);
extern void Func_020048e8(s32 selector, s32 arg1, s32 arg2);

void Func_02004254(s32 kind)
{
    s32 actor_id;
    s32 use_alt_position;
    s32 shared_y;
    u8 *record;

    Func_0808a018();
    Func_020048e8(24, 0, 0);
    Func_02004890(0, 0x1b0, 0x86, 0x8000);
    Func_020037d8(1);
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0d0(0, 0xcb << 1, 0x86);
    Func_0808a0d0(0, 0xcb << 1, 0x98);
    Func_0808a0d0(0, 0x1a5, 0x98);
    Func_0808a138(27, 1);
    Func_0808a010(20);
    Func_0808a150(27, 0, 10);

    if (Func_080770c0(0x300) != 0) {
        use_alt_position = 0;
        actor_id = Func_02004fa8(kind, 0);
        Func_0808a138(27, 1);
        Func_0808a010(20);
        Func_0808a150(27, 0, 10);
        Func_0808a170(0x1ebc);
        Func_0200486c(0xa01b);
        Func_0808a110(0, 3);
        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0d0(0, 0x1b0, 0xa8);
        Func_0808a1b8(0, 0xc000, 0);

        record = Func_0808a080(0);
        if (record != 0)
            Func_0808a0f0(actor_id, *(s32 *)(record + 8), *(s32 *)(record + 16));

        Func_0808a090(actor_id, 0x10000, 0x8000);
        Func_0808a0d0(actor_id, 0x1c0, 0xa8);
        Func_0808a1b8(actor_id, 0xb000, 20);
        Func_0808a1b8(27, 0xc000, 20);
        Func_0808a100(27, 3);
        Func_0200486c(27);
        Func_0808a1e8(actor_id, 0x102, 60);
        Func_0808a138(27, 1);
        Func_0808a100(27, 3);
        Func_0200486c(27);
        Func_0808a110(actor_id, 3);

        if (Func_080770c0(0x92b) != 0) {
            Func_0808a1b8(0, 0x2000, 0);
            Func_0808a1b8(27, 0x3000, 0);
            use_alt_position = 0;
            shared_y = 0xcc;
            goto shared_position;
        }

        if (Func_080770c0(0x92a) != 0) {
            use_alt_position = 1;
            Func_0808a0d0(0, 0xd3 << 1, 0x9a);
            Func_0808a1b8(0, 0xc000, 0);
            Func_0808a1b8(27, 0x5000, 0);
            Func_0808a0d0(actor_id, 0xcd << 1, 0xcc);
            Func_02004880(actor_id, 0xd000);
            goto scene_common;
        }

        if (Func_080770c0(0x929) != 0) {
            use_alt_position = 0;
            Func_0808a1b8(0, 0x2000, 0);
            Func_0808a1b8(27, 0x3000, 0);
            shared_y = 0xac;
            goto shared_position;
        }

        goto alternate_position;

shared_position:
        /* 0x92b and 0x929 share this text-position epilogue. */
        Func_0808a0d0(actor_id, 0xeb << 1, shared_y);
        Func_02004880(actor_id, 0xb000);
        goto scene_common;

alternate_position:
        use_alt_position = 1;
        Func_0808a0d0(0, 0xd3 << 1, 0x9a);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a1b8(27, 0x5000, 0);
        Func_0808a0d0(actor_id, 0xcd << 1, 0xac);
        Func_02004880(actor_id, 0xd000);
        goto scene_common;

scene_common:
        Func_0808a150(27, 0, 20);
        Func_0200486c(27);
        Func_0808a110(0, 3);
        Func_0808a110(27, 3);
        Func_0808a090(27, 0x10000, 0x8000);
        if (use_alt_position) {
            Func_0808a0d0(27, 0xd6 << 1, 0xa4);
            Func_0808a0d0(27, 0xcc << 1, 0xa4);
        }
        Func_0808a0d0(27, 0x198, 0x86);
        Func_0808a0c8(27, 0x1b8, 0x86);
        Func_0808a010(40);
        Func_020048e8(9, 10, 0);
    } else {
        Func_0808a170(0x1eb7);
        actor_id = 0xa01b;
        Func_0808a188(actor_id, 0, 40);
        Func_0808a1e8(27, 0x101, 60);
        Func_0200486c(actor_id);
        Func_0808a1f0(0, 0x102);
        Func_0808a010(60);
        Func_0808a1e8(27, 0x103, 40);
        Func_0808a130(27, 2);
        Func_0200486c(actor_id);
        Func_0808a1e8(27, 0x105, 40);
        Func_0200486c(actor_id);
        Func_0808a110(27, 4);
        Func_0200486c(actor_id);
        Func_0808a110(0, 3);
        Func_0808a010(20);
        Func_0808a248(4);
    }
}
