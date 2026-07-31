typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern u8 *Data_03001ebc;

extern void Func_02001554(void);
extern u8 *Func_0200157a(s32);
extern void Func_0200167e(s32);
extern void Func_020015b2(s32, s32, s32);
extern void Func_02001668(s32);
extern void Func_02001674(void);
extern void Func_02001680(void);
extern void Func_02001594(void);

void Func_020006a4(void)
{
    u8 *workspace;
    u8 *record;

    workspace = Data_03001ebc;

    Func_02001554();

    record = Func_0200157a(0);
    record[0x55] = 0;

    Func_0200167e(123);
    Func_020015b2(0, 2, -16);

    Func_02001668((s32) *(s16 *) (workspace + 364));

    Func_02001674();
    Func_02001680();
    Func_02001594();
}
