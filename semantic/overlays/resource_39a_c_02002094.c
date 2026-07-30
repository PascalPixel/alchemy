typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
#define NULL ((void *)0)

/*
 * Resource 39a overlay pathing step at 0x02002094.
 *
 * Complete owner: the two-stage `push {r5, r6, r7, lr}` / high-register
 * `push` prologue at 0x02002094 with `sub sp, #20`, and the matching
 * `add sp, #20 / pop {r3, r5, r6, r7} / ... / pop {r0} / bx r0` at
 * 0x0200222a.  Nothing is returned.  Six pool words follow the return and are
 * data.  All 19 distinct branch targets in the row are placed.
 *
 * Frame map: sp+0 holds the goal marker, sp+4 the heading, and sp+8..sp+19
 * the three-word probe position handed to the stepping imports by address.
 *
 * 0x0200a015 is 0x02008000 + 0x2015, i.e. the in-image routine 0x02002014
 * plus the Thumb bit - a fourth witness for this overlay's 0x02008000 link
 * base.  It is installed as the subject's per-frame callback, and 0x02002014
 * is exactly the routine that consumes 0x0200a424 the way this one consumes
 * 0x0200a464.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address is an import identity, not a place to disassemble.
 * That is the convention the byte-exact sources in this overlay already use
 * (`assets/code/resource_39a_c_02000030.c` declares `Func_02002442`), so
 * imports are named by the address their call site computes and their
 * interfaces are left open.  Declarations are old-style because one name is
 * reached with different argument counts.
 */

/* In-image tables at 0x02008000 + 0x2464; 0x02000240 is below the link base
 * and is a resident table whose word at byte offset 500 selects the subject. */
extern s16 Data_0200a464[];
extern s16 Data_02000240[];
extern u32 Data_03001ae8;

/* The installed callback, named by its in-image address. */
extern void Func_02002014();

/* Imports; those used for their return value are typed. 0x02004406 is reached
 * both as a three-argument probe and as a two-argument command. */
u8 *Func_02004498();
void Func_0200440a();
s32 Func_020043ec();
void Func_02004392();
s32 Func_02004406();
s32 Func_02004410();
void Func_02004426();
void Func_02004416();
void Func_02004444();
void Func_02004450();
s32 Func_0200446e();
void Func_02004480();
void Func_0200448e();
s32 Func_020044c4();
void Func_020044c2();
void Func_020044d0();
void Func_02004466();
void Func_02004484();
void Func_02004568();

void Func_02002094(void)
{
    u8 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 x;
    s32 z;

    subject = Func_02004498(*(s32 *)((u8 *)Data_02000240 + 500));

    for (;;) {
        heading = Data_0200a464[(Data_03001ae8 >> 4) & 15];
        /* The test is on heading << 16 against 0xffff0000, i.e. the signed
         * halfword -1 meaning "no heading". */
        if ((s16)heading == -1) {
            return;
        }
        /* No argument register is written before this branch. */
        Func_0200440a();

        /* movs r3,#0x80 / lsls r3,#12 builds the 0x80000 bias kept in fp. */
        x = (*(s32 *)(subject + 8) & (s32)0xfff00000) + 0x80000;
        probe[0] = x;
        probe[1] = *(s32 *)(subject + 12);
        z = (*(s32 *)(subject + 16) & (s32)0xfff00000) + 0x80000;
        probe[2] = z;

        goal = Func_020043ec((s32)subject[34], x, z);
        /* movs r0,#0x80 / lsls r0,#13 builds 0x100000.  The probe block is
         * passed by address and is advanced by the callee. */
        Func_02004392((s32)0x100000, heading, probe);

        marker = Func_02004406((s32)subject[34], probe[0], probe[2]);
        if (marker == 255) {
            *(u16 *)(subject + 6) = (u16)heading;
            goto tail;
        }
        if (Func_02004410((s32)subject[34], probe[0], probe[2])
                - *(s32 *)(subject + 12) > 0x80000) {
            *(u16 *)(subject + 6) = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before 0x02004392. */
        probe[0] = x;
        probe[2] = z;
        *(s32 *)(subject + 48) = 0x20000;
        *(s32 *)(subject + 52) = 0x1999;
        *(u16 *)(subject + 100) = 0;
        Func_02004426(subject, x, *(s32 *)(subject + 12), z);
        /* Same import as the probe above, two arguments here. */
        Func_02004406(subject, 2);
        Func_02004416(subject, 48);
        Func_02004444(subject);
        *(void **)(subject + 108) = (void *)Func_02002014;

        for (;;) {
            Func_02004450((s32)0x100000, heading, probe);
            marker = Func_020044c4((s32)subject[34], probe[0], probe[2]);
            if (marker == 255) {
                break;
            }
            if (Func_0200446e((s32)subject[34], probe[0], probe[2])
                    - *(s32 *)(subject + 12) > 0x80000) {
                break;
            }
            *(s32 *)(subject + 48) = 0x20000;
            *(s32 *)(subject + 52) = 0x1999;
            x = probe[0];
            z = probe[2];
            Func_02004480(subject, probe[0], probe[1], probe[2]);
            Func_0200448e(subject);
            if (marker != goal) {
                goto blocked;
            }
        }

        *(s32 *)(subject + 48) = 0x20000;
        *(s32 *)(subject + 52) = 0x10000;
        Func_020044c2(subject, x, *(s32 *)(subject + 12), z);
        Func_020044d0(subject);
        Func_02004466(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    *(void **)(subject + 108) = NULL;
    subject[90] |= 1;
    /* movs r3,#0x80 / lsls r3,#7 builds 0x4000. */
    *(s32 *)(subject + 52) = 0x4000;

tail:
    Func_02004484(10);
    Func_02004568();
}
