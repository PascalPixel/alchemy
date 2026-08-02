typedef unsigned char u8;
typedef signed int s32;

/* Create one scene handle and bind its three adjacent resource records. */
extern u8 Data_02008920;
extern u8 Data_0200893c;
extern u8 Data_02008958;

extern s32 Func_08015010(s32, s32, s32, s32, s32);
extern void Func_08015098(const void *, s32, s32, s32);

void Func_02000768(void)
{
    s32 handle = Func_08015010(0, 13, 30, 6, 2);

    Func_08015098(&Data_02008920, handle, 0, 0);
    Func_08015098(&Data_0200893c, handle, 0, 8);
    Func_08015098(&Data_02008958, handle, 0, 16);
}
