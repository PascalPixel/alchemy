typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/* Complete actor-15 active-state dialogue wrapper through alignment. */
struct Actor_02000864 {
    u8 reserved00[100];
    u16 state_flags;
};

extern struct Actor_02000864 *Func_0808a080(s32 actor);
extern void Func_0808a018(void);
extern void Func_0808a170(s32 dialogue);
extern void Func_020004b4(s32 actor);
extern void Func_0808a020(void);

void Func_02000864(void)
{
    struct Actor_02000864 *actor;

    actor = Func_0808a080(15);
    actor->state_flags |= 2;

    Func_0808a018();
    Func_0808a170(0x1cc1);
    Func_020004b4(15);
    Func_0808a020();

    Func_0808a080(15)->state_flags = 0;
}
