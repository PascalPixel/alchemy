typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_02009788[];
extern u8 *Data_03001ebc;

/*
 * The overlay's stored BL displacements use per-site linker identities rather
 * than the runtime veneer addresses.  These names reproduce those identities:
 * 0179e -> 1618 (room select), 01638 -> 14a8 (map placement), 0169e ->
 * 1508 (actor lookup), 016b6 -> 1510/14e8, 01714 -> 1558, 01708 -> 1540,
 * and 017b4 -> 15e0.
 */
void Func_0200179e();
void Func_02001638();
u8 *Func_0200169e();
void Func_020016b6();
void Func_02001714();
void Func_02001708();
void Func_020017b4();

void Func_02000180(void)
{
    u8 *workspace;
    s32 workspace_offset;

    Func_0200179e(188);
    Func_02001638(Data_02009788, 67, 6);

    Func_0200169e(0)[85] = 0;
    Func_020016b6(0, 0xcccc, 0x6666);

    workspace_offset = 448;
    workspace = Data_03001ebc;
    workspace += workspace_offset;
    /* The ROM stores 448 - 192 = 256, reusing the offset register. */
    workspace_offset -= 192;
    *(s32 *)workspace = workspace_offset;

    Func_02001714(0, 2);
    Func_02001708(0, 0, -16);
    Func_020016b6(16);
    Func_020017b4(2);
}
