typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_02009788[];
extern u8 *Data_03001ebc;

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
    workspace_offset -= 192;
    *(s32 *)workspace = workspace_offset;

    Func_02001714(0, 2);
    Func_02001708(0, 0, -16);
    Func_020016b6(16);
    Func_020017b4(2);
}
