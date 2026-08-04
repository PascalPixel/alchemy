typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Complete actor-8 staged presentation setup.  It freezes actors 0 and 8,
 * installs actor 8's descriptor, publishes the two scene timers, runs the
 * long presentation sequence, then restores the palette and hands control to
 * scene 111.
 */
struct Actor_0200357c {
    u8 reserved00[24];
    s32 width;
    s32 height;
    u8 reserved20[68];
    u16 state_flags;
};

struct SceneWork_0200357c {
    u8 reserved000[448];
    s32 scene_value;
    u8 reserved1c4[4];
    s32 scene_timer;
};

extern const u8 Data_0200d218[];
extern void Func_020034a0(void);
extern struct SceneWork_0200357c *Data_03001ebc;
extern struct Actor_0200357c *Func_0808a080(s32 actor);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a5c0(s32 value, s32 mode);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_080000c0(s32 frames);
extern void Func_0808a158(s32 actor, s32 mode);
extern void Func_080091e0(struct Actor_0200357c *actor, s32 enabled);
extern void Func_0808a090(s32 actor, s32 width, s32 height);
extern void Func_0808a098(s32 actor, const void *descriptor);
extern void Func_080000d0(void (*task)(void), s32 priority);
extern void Func_0808a330(s32 value, s32 mode);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a248(s32 scene);

void Func_0200357c(void)
{
    struct Actor_0200357c *actor0;
    struct Actor_0200357c *actor8;

    actor8 = Func_0808a080(8);
    Func_0808a010(60);
    Func_0808a018();
    Func_0808a5c0(0x9999, 1);
    actor8->width = 0x13333;
    actor8->height = 0x13333;
    Func_0808a200(8, 1);
    Func_080000c0(1);
    Func_0808a158(0, 15);
    actor0 = Func_0808a080(0);
    Func_080091e0(actor0, 0);
    actor8 = Func_0808a080(8);
    Func_080091e0(actor8, 0);
    Func_0808a090(8, 0x6666, 0x3333);
    actor8->state_flags = 0;
    Func_0808a098(8, Data_0200d218);
    Func_080000d0(Func_020034a0, 3200);

    Data_03001ebc->scene_value = 256;
    Func_0808a330(0x10003, 1);
    Data_03001ebc->scene_timer = 32;
    Func_0808a360();
    Func_0808a010(120);
    Func_0808a5c0(0x16666, 300);
    Func_0808a010(270);
    Data_03001ebc->scene_timer = 16;
    *(volatile u16 *)0x05000000 = 0x7fff;
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(111);
}
