typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Complete actor-14 scene-selection owner through its trailing flag pool.
 * The actor is made active for the scene, then either receives the repeat
 * dialogue or runs the one-time movement sequence before flag 0x307 is set.
 */
struct Actor_02000804 {
    u8 reserved00[100];
    u16 state_flags;
};

extern struct Actor_02000804 *Func_0808a080(s32 actor);
extern void Func_0808a018(void);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a170(s32 dialogue);
extern void Func_020004b4(s32 actor);
extern void Func_0200058c(void);
extern void Func_080770c8(s32 flag);
extern void Func_0808a020(void);

void Func_02000804(void)
{
    struct Actor_02000804 *actor;

    actor = Func_0808a080(14);
    actor->state_flags |= 2;

    Func_0808a018();
    if (Func_080770c0(0x307) != 0) {
        Func_0808a170(0x1cc0);
        Func_020004b4(14);
    } else {
        Func_0200058c();
        Func_080770c8(0x307);
    }
    Func_0808a020();

    Func_0808a080(14)->state_flags = 1;
}
