typedef unsigned char u8;
typedef signed int s32;

extern void Func_0808a018(void);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_02000dd8(s32 actorId);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_080770c8(s32 flag);
extern void Func_0808a020(void);

/* Run the actor-8 scene only while it occupies column eleven. */
void Func_02000d04(void)
{
    u8 *actor;
    s32 column;

    Func_0808a018();
    actor = Func_0808a080(8);
    column = *(s32 *)(actor + 8) >> 20;
    if (column == 11) {
        Func_02000dd8(8);
        actor = Func_0808a080(8);
        actor[35] |= 2;
        Func_080091c0(39, 12, 3, 1, 8, 12);
        Func_080091c0(43, 11, 3, 1, 12, column);
        Func_080770c8(0x860);
    }
    Func_0808a020();
}
