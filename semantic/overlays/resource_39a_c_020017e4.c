typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 39a overlay chapter dispatcher at 0x020017e4.
 *
 * Complete owner: `push {r5, r6, r7, lr}` and `sub sp, #8` at 0x020017e4,
 * and the matching `add sp, #8 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x0200191e; every early exit branches to that same epilogue, so nothing is
 * returned.  Nine pool words follow the return and are data.  All 16 distinct
 * branch targets in the row are placed.
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

/* 0x02000240 is below the 0x02008000 link base, so it is a resident table.
 * Entry 224 selects the chapter and entry 225 the step; entry 225 is read
 * both signed and unsigned by this owner, so both spellings are declared. */
extern s16 Data_02000240[];

/* 0x0200b328 is one word past the end of the 0x3328-byte image, so it is a
 * loader/RAM word - the same one the byte-exact 0x02001730 tests.  Here it is
 * a 0/1 flag. */
extern s32 Data_0200b328;
extern u8 *Data_03001ebc;
extern u8 *Data_03001ee0;

/* Imports; the queried ones are typed for their return value. */
void Func_02000ed8();
void Func_02001948();
void Func_02001b1c();
void Func_080000c0();
void Func_08009180();
s32 Func_080770c0();
s32 Func_0808a080();
void Func_0808a158();
void Func_0808a1e0();
void Func_0808a330();
void Func_0808a338();
void Func_0808a348();
void Func_0808a408();
void Func_02002718();
void Func_02002726();
void Func_02003132();
void Func_02003334();
s32 Func_02003b1a();
void Func_02003b7c();
u8 *Func_02003bac();
void Func_02003bb6();
void Func_02003bc8();
s32 Func_02003be0();
void Func_02003c02();
void Func_02003c0a();
void Func_02003c88();
void Func_02003c94();
void Func_02003cc0();
void Func_02003cd6();

void Func_020017e4(void)
{
    s16 chapter;
    s16 step;

    Func_02001948();

    /* movs r1,#0xe0 / lsls r1,#1 gives the byte offset 448, i.e. entry 224. */
    chapter = Data_02000240[224];

    if (chapter == 0x40) {
        if (Func_080770c0((s32)0xf13) == 0 && Data_02000240[225] == 1) {
            Func_02001b1c();
        }
        /* Unsigned test on ((entry225 - 2) << 16) against 0xc0 << 10 =
         * 0x30000, which selects steps 2..5. */
        if ((u32)((u32)(((u16 *)Data_02000240)[225] - 2) << 16) >
            (u32)0x30000) {
            return;
        }
        /* movs/lsls build 0x9c0000, 0xbc0000 and the shared 0x1c40000. */
        Func_02000ed8((s32)0x9c0000, 0, (s32)0x1c40000, 223);
        Func_02000ed8((s32)0xbc0000, 0, (s32)0x1c40000, 223);
        return;
    }

    if (chapter != 0x43) {
        return;
    }

    {
        u8 *actor = Func_0808a080(8);

        Data_0200b328 = 0;
        actor[85] = 0;
        *(s32 *)(actor + 12) = 0;
    }
    Func_0808a1e0(8, 1);
    Func_0808a158(8, 15);

    step = Data_02000240[225];
    if (step >= 1) {
        if (step <= 2) {
            Func_0808a408(0);
            Data_0200b328 = 1;
        } else if (step == 5) {
            Func_0808a408(0);
            Data_0200b328 = 1;
            *(s32 *)(Data_03001ee0 + 24) = 0;
        }
    }

    if (Data_02000240[225] > 6) {
        return;
    }

    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (Func_080770c0((s32)0x820) != 0) {
        Func_08009180(30, 57, 19, 57, 1, 1);
        Func_08009180(30, 8, 12, 8, 8, 7);
    } else {
        /* The offset register is reused: 448 for the slot, then 448 - 192 =
         * 256 for the value stored into it. */
        *(s32 *)(Data_03001ebc + 448) = 256;
        Func_0808a338((s32)0x203108, 1);
        Func_0808a330((s32)0x203108, 1);
        Func_0808a348(1);
        Func_080000c0(1);
    }
}
