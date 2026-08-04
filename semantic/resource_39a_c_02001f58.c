typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x02001f58.
 *
 * Complete owner: `push {lr}` at 0x02001f58 and `pop {r0} / bx r0` at
 * 0x02001f9c, so nothing is returned.  Three pool words follow the return.
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

/* 0x03001ebc is the overlay's workspace pointer, loaded directly by several of
 * its routines. */
extern u8 *Data_03001ebc;

/* Imports. Those used for their return value are typed. */
void Func_08015040();
s32 Func_08077040();
s32 Func_080770c0();
void Func_0808a018();
void Func_0808a020();

void Func_02001f58(void)
{
    Func_0808a018();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (Func_080770c0((s32)0x820) != 0) {
        Func_08015040((s32)0x17e5, 1);
    } else {
        Func_08015040((s32)0x17e4, 1);
        if (Func_08077040((s32)0xe6) != -1) {
            u8 *workspace = Data_03001ebc;

            /* movs r1,#0xb9 / lsls r1,#1 gives the byte offset 370. */
            *(u16 *)(workspace + 370) = 1;
        }
    }
    Func_0808a020();
}
