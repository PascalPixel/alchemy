typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3bf scene step at 0x02000e80.
 *
 * r5 initially holds the shared workspace pointer read from the IWRAM slot at
 * 0x03001ebc; after the guarded branch is taken it is reused to hold the
 * SIGNED halfword at workspace offset 364 (`ldrsh`), which is then used as a
 * small identifier, not as a pointer.  r6 holds the constant -1 used both as
 * the failure code of the first call and as the two leading arguments of
 * Func_020064b4.
 *
 * The callees are other entry points of this same overlay module, named by the
 * tree's offset convention (see the note on the module's link base below);
 * none of them is reconstructed here.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints, so the module is linked at 0x02008000: the
 * dispatcher at 0x02004638 loads its jump-table base as 0x0200c64c while the
 * table itself is embedded at offset 0x464c, and every entry in it likewise
 * points 0x8000 past the case body it selects.  Both the existing byte-exact
 * sources under assets/code and this file keep the printed-offset spelling for
 * symbol names and the raw pool value for absolute data addresses, which is
 * self-consistent; nothing here depends on which of the two bases is real.
 *
 * The routine returns whatever r0 happens to hold at the epilogue
 * (`pop {r5,r6}; pop {r0}; bx r0`), which is a call result on both paths, so
 * it is written as a void routine here.
 */
void Func_02000e30();
void Func_080091f0();
s32 Func_08077040();
void Func_080770c8();
void Func_080f9010();
s32 Func_02006492(s32 request);
void Func_02001cd4(s32 id);
void Func_02006652(s32 id);
void Func_020064aa(s32 x, s32 y, s32 z);
void Func_020064b4(s32 a, s32 b, s32 c);
void Func_020064e6(s32 id);

void Func_02000e80(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s16 id;

    if (Func_08077040(234) == -1) {
        return;
    }

    id = *(s16 *)(workspace + 364);
    Func_02000e30(id - 40);
    Func_080f9010(157);
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_080091f0(-1, -1, 0xe666);
    Func_080770c8(id + 816);
}
