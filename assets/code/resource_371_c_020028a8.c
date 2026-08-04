typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct SceneWork_020028a8 {
    u8 reserved000[370];
    u16 story_result;
};

extern struct SceneWork_020028a8 *Data_03001ebc;
extern void Func_02006b88(void);
extern void Func_02006bbe(void);
extern void Func_02006d0c(void);
extern void Func_02006d3a(void);
extern void Func_02006b1c(s32 dialogue, s32 mode);
extern s32 Func_02006b4c(s32 flag);

/* Dialogue 0x264c conditionally publishes its story result. */
void Func_020028a8(void)
{
    Func_02006b88();
    Func_02006d0c();
    Func_02006b1c(0x264c, 1);
    if (Func_02006b4c(0x234) != 0) {
        Data_03001ebc->story_result = 1;
    }
    Func_02006d3a();
    Func_02006bbe();
}
