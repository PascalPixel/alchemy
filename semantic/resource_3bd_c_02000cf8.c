typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3bd owner at 0x02000cf8 (80 bytes).  The single scene record is
 * selected by id 12.  When its x/z cells are (30, 20), the original code
 * marks two byte fields, clears the record's word at +20, draws the six-arg
 * panel, and raises story flag 0x212.  The owner has no null guard because the
 * machine code dereferences the accessor result immediately.
 *
 * The literal pool contains only 0x212 at 0x02000d44; the next owner begins at
 * 0x02000d48.  The two stack words are the fifth and sixth arguments of
 * Func_080091c0, so the cell derived from +16 is retained and passed exactly
 * as loaded.
 */

extern u8 *Func_0808a080(s32 id);
extern void Func_080091c0(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080770c8(s32 flag_id);

void Func_02000cf8(void)
{
    u8 *record = Func_0808a080(12);
    s32 xCell = *(s32 *)(record + 8) >> 20;
    s32 zCell = *(s32 *)(record + 16) >> 20;

    if (xCell != 30 || zCell != 20)
        return;

    record[0x55] = 2;
    *(s32 *)(record + 20) = 0;
    record[0x23] = 2;
    Func_080091c0(30, 20, 1, 1, 32, zCell);
    Func_080770c8(0x212);
}
