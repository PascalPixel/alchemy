typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct SceneWork_020028a8 {
    u8 reserved000[370];
    u16 story_result;
};

extern struct SceneWork_020028a8 *Data_03001ebc;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a3d8(void);
extern void Func_0808a3e0(void);
extern void Func_08015040(s32 dialogue, s32 mode);
extern s32 Func_080770c0(s32 flag);

/* Dialogue 0x264c conditionally publishes its story result. */
void Func_020028a8(void)
{
    Func_0808a018();
    Func_0808a3d8();
    Func_08015040(0x264c, 1);
    if (Func_080770c0(0x234) != 0) {
        Data_03001ebc->story_result = 1;
    }
    Func_0808a3e0();
    Func_0808a020();
}
