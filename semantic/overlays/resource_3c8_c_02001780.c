typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02001780: eleven-call cutscene step that first flips two
 * workspace flags.
 *
 * The owner is complete: `push {lr}` at 0x02001780 and the matching
 * `pop {r0} ; bx r0` at 0x020017f4..0x020017f6, followed by a four-word
 * literal pool (0x020017f8-0x02001807) that is data, not code.  No live frame
 * or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * 0x03001ebc is the package workspace pointer already proven by the
 * byte-exact siblings (0x02000754 loads it as `*(u8 **)0x03001ebc`), read
 * here once and offset twice with pooled displacements 0xcba and 0xcb6.  Both
 * stores are halfword-wide.
 *
 * Constants are materialised as `movs #imm ; lsls #shift`, so the values are
 * exact: 224<<8 = 0xe000, 128<<9 = 0x10000, 128<<6 = 0x2000, 224<<17 =
 * 0x01c00000 and 216<<17 = 0x01b00000.  The -1 at 0x020017de is `negs r1, r1`
 * on 1.
 *
 * All eleven `bl` sites are accounted for: 0x0200662c, 0x020066e2,
 * 0x020066cc, 0x0200663a, 0x0200670c, 0x02006720, 0x0200674c, 0x02006766,
 * 0x02006772, 0x0200673a, 0x0200668e.  The three reached with no argument
 * register written (0x0200662c, 0x02006772, 0x0200668e) are called with no
 * argument asserted.  Import naming follows the note in
 * resource_3c8_c_020002f0.c.
 */

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_0200662c();
void Func_020066e2();
void Func_020066cc();
void Func_0200663a();
void Func_0200670c();
void Func_02006720();
void Func_0200674c();
void Func_02006766();
void Func_02006772();
void Func_0200673a();
void Func_0200668e();

void Func_02001780(void)
{
    u8 *workspace;

    workspace = *(u8 **)0x03001ebc;
    *(u16 *)(workspace + 0xcba) = 0;
    *(u16 *)(workspace + 0xcb6) = 1;

    Func_0200662c();
    Func_020066e2(0x267d);
    Func_020066cc(10, 0, 0);
    Func_0200663a(10);
    Func_0200670c(10, 0, 20);
    Func_02006720(10, 0xe000, 0);
    Func_0200674c(0x10000, 0x2000);
    Func_02006766(0x01c00000, -1, 0x01b00000, 1);
    Func_02006772();
    Func_0200673a(10, 0);
    Func_0200668e();
}
