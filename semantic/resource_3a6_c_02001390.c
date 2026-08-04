typedef unsigned char u8;
typedef signed int s32;

struct PlacementResult_02001390 {
    s32 delta_index;
    s32 actor;
    s32 x;
    s32 y;
    s32 z;
    void (*callback)(void);
};

/* Placement-driven actor-8/10 scene, complete 284-byte owner to 0x020014ac. */

extern s32 Func_02000758(struct PlacementResult_02001390 *result);
extern void Func_020008ec(s32, s32, s32, s32, s32, void (*)(void));
extern void Func_02000cd0(void *record);
extern void Func_02000528();
extern void Func_080770c8(s32 flag);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a0e0(s32 actor, s32 value, s32 mode);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a1e0(s32 actor, s32 mode);
extern void Func_080f9010(s32 cue);

void Func_02001390(void)
{
    struct PlacementResult_02001390 p;
    u8 *record;

    Func_0808a018();
    if (Func_02000758(&p) != 0) {
        Func_020008ec(p.delta_index, p.actor, p.x, p.y, p.z, p.callback);
        if ((p.x >> 20) == 17) {
            Func_0808a100(p.actor, 3);
            record = Func_0808a080(p.actor);
            record[0x55] = 0;
            record = Func_0808a080(p.actor);
            *(s32 *)(record + 0x44) = 0;
            Func_0808a0e0(p.actor, -12, 0);
            Func_0808a0e8(p.actor);
            Func_0808a100(p.actor, 3);
            Func_0808a1e0(10, 3);
            record = Func_0808a080(p.actor);
            record[0x55] = 3;
            Func_0808a0e0(p.actor, -6, 0);
            record = Func_0808a080(p.actor);
            Func_02000cd0(record);
            Func_0808a100(p.actor, 8);
            record = Func_0808a080(p.actor);
            record[0x23] = 2;

            Func_02000528(0, p.x >> 20, (p.z >> 20) - 2, 1, 4, 0);
            Func_02000528(2, p.x >> 20, (p.z >> 20) - 2, 1, 4, 0);
            Func_02000528(2, 16, 18, 1, 2, 0);
            Func_02000528(0, 16, 16, 1, 4, 0);
            Func_080770c8(0x203);
            Func_080f9010(0xf0);
        }
    }
    Func_0808a020();
}
