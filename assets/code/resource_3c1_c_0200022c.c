typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s16 Data_02000240[];

void Func_020004e2();
s32 Func_020004f6();
void Func_0200035e();
void Func_02000526();

s32 Func_0200022c(void)
{
    u8 *workspace;

    /* Record arrival on map 90, then publish the initial scene phase/timer. */
    if (Data_02000240[225] == 90) {
        Func_020004e2(0x96f);
    }

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 256;
    *(s32 *)(workspace + 456) = 24;

    /* The dressing sequence and cue are unlocked by the shared event flag. */
    if (Func_020004f6(0x201) != 0) {
        Func_0200035e();
        Func_02000526(16, 4);
    }
    return 0;
}
