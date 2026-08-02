typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3b1 selector dispatcher at 0x020048e8.
 *
 * The saved-register prologue is at 0x020048e8 and the interworking return is
 * the pop {r5,r6,r7}/pop {r0}/bx r0 sequence at 0x02004f64-0x02004f68.
 * The complete owner continues through the alignment halfword at 0x02004f6a
 * and the fourteen directly referenced pool words at 0x02004f6c-0x02004fa4;
 * the next independent push {r5,r6,r7,lr} begins at 0x02004fa8.  The exact
 * owner span is therefore 0x6c0 (1728) bytes.
 *
 * A range check at 0x020048f6 accepts selector values 0..25 and dispatches
 * through the 26-entry table at 0x02004904-0x02004967.  The table words are
 * linked with the overlay's 0x02008000 base and select the ordinary C cases
 * below.  The pool at 0x02004cb8 holds the table base (0x0200c904), while the
 * pools at 0x02004cbc, 0x02004cc0 and 0x02004f6c-0x02004fa4 provide the
 * workspace, numeric and scene-data constants used by the arms.
 *
 * A few arms share a final instruction or re-enter another table arm.  The
 * labels and gotos preserve those shared call sites without duplicating a
 * callee in the source.  Loop arms retain their single assembly call site as
 * a C loop; no inline assembly, fixed-register construct or compiler escape
 * is needed.
 */

extern u8 *Data_03001ebc;

extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a118(s32 actor);
extern void Func_0808a1f0(s32 actor, s32 animation);
extern void Func_0808a010(s32 frames);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a108(s32 actor, s32 mode);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a368(void);
extern void Func_0808a248(s32 actor);
extern void Func_080000c0(s32 frames);
extern void Func_080770d0(s32 value);
extern void Func_0808a098(s32 actor, const void *table);
extern void Func_0808a088(s32 actor);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 flags);
extern void Func_0808a228(void);
extern void Func_0808a030(const void *resource);

extern void Func_020037d8(s32 value);
extern void Func_020045d0(void);
extern void Func_020047cc(s32 actor);
extern void Func_02004880(s32 actor, s32 value);
extern void Func_02004890(s32 actor, s32 x, s32 y, s32 z);
extern void Func_020048ac(s32 x, s32 y, s32 z, u32 flags);
extern void Func_020048e8(s32 selector, s32 arg1, s32 arg2);

void Func_020048e8(s32 selector, s32 arg1, s32 arg2)
{
    u8 *record;
    const void *resource;
    s32 i;

    switch (selector) {
    case 0:
        Func_0808a1b8(0, 0, 0);
        Func_0808a1b8(1, 0x8000, 0);
        Func_0808a1b8(2, 0, 0);
        goto selector_01_shared;

    case 1:
        Func_0808a1b8(0, arg1, 0);
        Func_0808a1b8(1, arg1, 0);
        Func_0808a1b8(2, arg1, 0);
selector_01_shared:
        Func_0808a1b8(3, 0x8000, arg2);
        break;

    case 2:
        Func_0808a100(0, 3);
        Func_0808a100(1, 3);
        Func_0808a100(2, 3);
        Func_0808a100(3, 3);
        if (arg1 != 0)
            Func_0808a118(3);
        if (arg2 == 0)
            break;
        goto selector_03_after_pose;

    case 3:
        Func_0808a1f0(0, 0x102);
        Func_0808a1f0(1, 0x102);
        Func_0808a1f0(2, 0x102);
        Func_0808a1f0(3, 0x102);
selector_03_after_pose:
        Func_0808a010(arg2);
        break;

    case 4:
        for (i = 0; i < arg1; i++)
            Func_0808a0f0(i + 10, 0, 0);
        break;

    case 5:
        record = Func_0808a080(arg1);
        *(u16 *)(record + 6) = 0x5000;
        goto selector_56_shared;

    case 6:
        record = Func_0808a080(arg1);
        *(u16 *)(record + 6) = 0x5000;
        *(s32 *)(record + 24) = (s32)0xffff0000;
selector_56_shared:
        Func_0808a100(arg1, 5);
        Func_0808a108(arg1, arg2);
        break;

    case 7:
        record = Func_0808a080(arg1);
        *(u16 *)(record + 6) = 0x5000;
        Func_020047cc(arg1);
        if (arg2 == 0)
            Func_0808a108(arg1, 0);
        break;

    case 8:
        *(s32 *)(Data_03001ebc + 448) = 514;
        Func_0808a360();
        if (arg1 != 0)
            Func_0808a370();
        Func_0808a010(0);
        break;

    case 9:
        Func_0808a368();
        Func_0808a370();
        if (arg1 != 0)
            Func_0808a248(arg1);
        break;

    case 10:
        Func_020048e8(24, 1, 0);
        Func_020048e8(25, 0, 0);
        Func_020037d8(0);
        Func_02004890(0, 0x1b0, 0xa8, 0x4000);
        Func_02004890(1, 0x1c0, 0xa8, 0x4000);
        Func_02004890(2, 0x1a8, 0x98, 0x4000);
        Func_02004890(3, 0x1ca, 0x98, 0x4000);
        break;

    case 11:
        if (arg1 != 0) {
            Func_0808a100(13, 1);
            record = Func_0808a080(13);
            *(u16 *)(record + 6) = 0x3000;
            record = Func_0808a080(13);
            *(s32 *)(record + 24) = 0x10000;
        }
        Func_0808a100(14, 1);
        record = Func_0808a080(14);
        *(u16 *)(record + 6) = 0x5000;
        Func_0808a100(15, 1);
        record = Func_0808a080(15);
        *(u16 *)(record + 6) = 0x3000;
        record = Func_0808a080(15);
        *(s32 *)(record + 24) = 0x10000;
        Func_0808a100(16, 1);
        record = Func_0808a080(16);
        *(u16 *)(record + 6) = 0x5000;
        Func_0808a100(17, 1);
        record = Func_0808a080(17);
        *(u16 *)(record + 6) = 0x3000;
        record = Func_0808a080(17);
        *(s32 *)(record + 24) = 0x10000;
        Func_0808a0f0(28, 0x19a0000, 0xae0000);
        Func_0808a0f0(29, 0x1d60000, 0xae0000);
        Func_0808a0f0(30, 0x19a0000, 0xce0000);
        Func_0808a0f0(31, 0x1d60000, 0xce0000);
        Func_0808a0f0(32, 0x19a0000, 0x11e0000);
        Func_0808a0f0(33, 0x1d60000, 0x11e0000);
        Func_0808a0f0(34, 0x19a0000, 0x13c0000);
        Func_0808a0f0(35, 0x1d60000, 0x13c0000);
        Func_080000c0(1);
        if (arg1 != 0) {
            Func_0808a1b8(13, 0xb000, 0);
            Func_0808a1b8(14, 0xd000, 0);
            Func_0808a1b8(15, 0xb000, 0);
            Func_0808a1b8(16, 0xd000, 0);
            Func_02004880(17, 0xb000);
        }
        break;

    case 12:
        record = Func_0808a080(arg1);
        Func_0808a100(arg1, 1);
        if (arg2 != 0)
            *(u16 *)(record + 6) = 0x3000;
        else
            *(u16 *)(record + 6) = 0x5000;
        *(s32 *)(record + 24) = 0x10000;
        break;

    case 13:
        Func_0808a0f0(9, 0, 0);
        Func_0808a0f0(12, 0, 0);
        Func_0808a0f0(11, 0, 0);
        Func_0808a0f0(13, 0, 0);
        goto selector_14_tail;

    case 14:
selector_14_reentry:
        Func_0808a0f0(14, 0, 0);
selector_14_tail:
        Func_0808a0f0(13, 0, 0);
        break;

    case 15:
        Func_020048e8(24, 1, 0);
        Func_0808a0f0(9, 0, 0);
        Func_0808a0f0(10, 0, 0);
        Func_02004890(8, 0x1bc, 0x266, 0xd000);
        Func_0808a0f0(0, 0, 0);
        if (arg1 != 0)
            Func_020045d0();
        Func_020048ac(0x1c00000, 0x200000, 0x2700000, 0x01000001);
        if (arg2 != 0) {
            *(s32 *)(Data_03001ebc + 448) = 514;
            Func_0808a360();
            Func_0808a370();
            Func_0808a010(20);
        }
        break;

    case 16:
        Func_0808a0f0(8, 0, 0);
        Func_0808a0f0(9, 0, 0);
        Func_0808a0f0(27, 0xdb0000, 0x980000);
        break;

    case 17:
        for (i = 0; i <= 7; i++)
            Func_0808a0f0(i + 28, 0, 0);
        break;

    case 18:
        Func_02004890(12, 152, 0x214, 0xb000);
        Func_02004890(8, 134, 490, 0xc000);
        Func_02004890(9, 166, 490, 0x5000);
        Func_02004890(10, 182, 504, 0x5000);
        Func_02004890(11, 118, 504, 0xc000);
        /* The arm clears its working registers and re-enters selector 14. */
        goto selector_14_reentry;

    case 19:
        Func_02004890(8, 0x1a0, 0x148, 0);
        Func_02004890(9, 0x1c0, 0x160, 0xd000);
        Func_02004890(10, 0x1c6, 0xf8, 0xc000);
        Func_02004890(0, 408, 290, 0);
        Func_02004890(1, 408, 342, 0);
        Func_02004890(13, 420, 356, 0xd000);
        Func_02004890(14, 408, 304, 0);
        Func_02004890(15, 418, 378, 0xd000);
        Func_02004890(16, 440, 262, 0xc000);
        Func_02004890(17, 448, 378, 0xd000);
        break;

    case 20:
        for (i = arg1; i <= arg2; i++)
            Func_080770d0(i);
        break;

    case 21:
        Func_020048e8(20, 0x92c, 0x93d);
        Func_020048e8(20, 0x917, 0x91f);
        Func_020048e8(20, 0x990, 0x998);
        Func_080770d0(0x300);
        Func_080770d0(0x301);
        Func_080770d0(0x302);
        break;

    case 22:
        Func_080000c0(1);
        Func_020048e8(23, 0, 0);
        Func_0808a098(12, (const void *)0x0200e840);
        break;

    case 23:
        Func_0808a088(1);
        Func_0808a088(2);
        Func_0808a088(3);
        break;

    case 24:
        Func_0808a210(-1, -1, -1, 0);
        Func_080000c0(1);
        if (arg1 != 0) {
            Func_0808a228();
            *(u8 *)((u8 *)arg1 + 0x55) = 0;
        }
        break;

    case 25:
        Func_0808a030((const void *)0x0200f2a0);
        Func_080000c0(1);
        if (arg1 == 1 || arg1 == 2) {
            resource = (arg1 == 1) ? (const void *)0x0200f300 : (const void *)0x0200f360;
            Func_0808a030(resource);
            Func_080000c0(1);
        } else if (arg1 == 3) {
            Func_0808a030((const void *)0x0200f3c0);
            Func_080000c0(1);
        }
        break;

    default:
        break;
    }
}
