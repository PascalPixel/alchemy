typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3bc scene-staging owner at 0x02001a0c, 532 bytes including the
 * six-word pool.  It fans four supporting actors around the caller, plays
 * the scripted interaction, then collapses the support actors and returns
 * the caller to its original map coordinates.
 */

extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a0b8(s32 actor, s16 x, s16 z);
extern void Func_0808a0d0(s32 actor, s16 x, s16 z);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_0808a360(void);
extern void Func_0808a370(void);

void Func_02001a0c(s32 actor)
{
    u8 *record;
    s16 map_x;
    s16 map_z;
    s32 x;
    s32 z;

    record = Func_0808a080(actor);
    map_x = *(s16 *)(record + 10);
    map_z = *(s16 *)(record + 18);

    Func_0808a018();
    Func_0808a090(actor, 0x10000, 0x8000);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a090(3, 0x10000, 0x8000);

    x = (s32)map_x << 16;
    z = (s32)map_z << 16;
    Func_0808a0f0(0, x, z - 0x300000);
    Func_0808a0f0(1, x - 0x100000, z - 0x280000);
    Func_0808a0f0(2, x + 0x100000, z - 0x280000);
    Func_0808a0f0(3, x, z - 0x200000);
    Func_0808a0f0(actor, x, z - 0x500000);

    record = Func_0808a080(0);
    *(s16 *)(record + 6) = (s16)0xc000;
    Func_0808a200(0, 0);
    Func_0808a360();
    Func_0808a370();
    Func_0808a170(0x20ed);
    Func_0808a110(actor, 3);
    Func_0808a180(actor, 0);
    Func_0808a138(actor, 2);
    Func_0808a180(actor, 0);
    Func_0808a138(actor, 2);
    Func_0808a180(actor, 0);
    Func_0808a138(actor, 2);
    Func_0808a180(actor, 0);

    Func_0808a100(3, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(0, 3);
    Func_0808a010(6);

    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));

    Func_0808a100(2, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));

    Func_0808a100(3, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(3, *(s16 *)(record + 10), *(s16 *)(record + 18));

    Func_0808a0d0(actor, map_x - 16, map_z - 64);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0f0(3, 0, 0);
    Func_0808a0d0(actor, map_x - 16, map_z - 16);
    Func_0808a0d0(actor, map_x, map_z);
    Func_0808a1b8(actor, 0xc000, 10);
    Func_0808a020();
}
