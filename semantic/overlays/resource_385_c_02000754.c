typedef unsigned char u8;
typedef signed int s32;

/* Complete actor-11 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_02000754 {
    u8 reserved00[91];
    u8 accepted;
};

extern void Func_0808a170(s32 dialogue);
extern struct Actor_02000754 *Func_0808a080(s32 actor);
extern void Func_020004b4(s32 actor);

void Func_02000754(void)
{
    Func_0808a170(0x1cbd);
    Func_0808a080(11)->accepted = 1;
    Func_020004b4(11);
    Func_0808a080(11)->accepted = 0;
}
