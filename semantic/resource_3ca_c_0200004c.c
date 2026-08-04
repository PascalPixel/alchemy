typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/* Complete shared next-scene dispatch and backdrop-palette clear owner. */
struct SceneWork_0200004c {
    u8 reserved000[364];
    s16 next_scene;
};

extern struct SceneWork_0200004c *Data_03001ebc;
extern void Func_0808a248(s32 scene);

void Func_0200004c(void)
{
    Func_0808a248(Data_03001ebc->next_scene);
    *(volatile u16 *)0x05000000 = 0;
}
