typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/* Complete actor-15 facing-preserving dialogue scene through its two-word pool. */
struct Actor_02000640 {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

extern struct Actor_02000640 *Func_0808a080(s32 actor);
extern void Func_0808a018(void);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a150(s32 actor, s32 mode, s32 value);
extern void Func_0808a188(s32 actor, s32 mode, s32 frames);
extern void Func_080000c0(s32 frames);
extern void Func_0808a020(void);

void Func_02000640(void)
{
    struct Actor_02000640 *actor;
    s16 original_facing;

    actor = Func_0808a080(15);
    original_facing = (s16)actor->facing;
    actor->state_flags |= 2;
    Func_0808a018();
    Func_0808a170(0x1cb4);
    Func_0808a100(15, 0);
    Func_0808a150(15, 0, 2);
    Func_0808a188(15, 0, 10);
    actor->facing = (u16)original_facing;
    Func_080000c0(1);
    Func_0808a020();
    actor->state_flags = 0;
}
