typedef unsigned char u8;
typedef signed int s32;

struct PlacementResult_020015cc {
    s32 delta_index;
    s32 actor;
    s32 x;
    s32 y;
    s32 z;
    void (*callback)(void);
};

/* Placement-driven actor-11 scene, complete 208-byte owner to 0x0200169c. */

extern s32 Func_02000758(struct PlacementResult_020015cc *result);
extern void Func_020008ec(s32, s32, s32, s32, s32, void (*)(void));
extern void Func_02000528();
extern void Func_080091c0();
extern void Func_080770c8(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a0e0(s32 actor, s32 value, s32 mode);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_080f9010(s32 cue);

void Func_020015cc(void)
{
    struct PlacementResult_020015cc p;
    u8 *record;

    Func_0808a018();
    if (Func_02000758(&p) != 0) {
        Func_020008ec(p.delta_index, p.actor, p.x, p.y, p.z, p.callback);
        Func_0808a100(11, 3);
        Func_0808a090(11, 0x4000, 0x20000);
        Func_0808a0e0(11, 0, -16);
        Func_0808a010(45);
        Func_080f9010(0xf0);
        Func_0808a100(11, 8);
        record = Func_0808a080(11);
        record[0x23] = 2;
        Func_02000528(0, 13, (p.z >> 20) - 1, 4, 2, 0);

        if ((p.z >> 20) == 20) {
            Func_080770c8(0x205);
        } else {
            Func_080770c8(0x204);
            Func_080091c0(14, 17, 2, 1, 14, 16);
            Func_080091c0(14, 13, 1, 1, 14, 15);
        }
    }
    Func_0808a020();
}
