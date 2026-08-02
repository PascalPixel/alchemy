typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_02009e2c[];
extern u8 Data_02009db4[];
extern u8 Data_02009ddc[];
extern u8 Data_02009e04[];
extern u8 Data_02000240[];

extern s32 Func_080770c0(s32 flag);
extern void Func_0808a018(void);
extern void Func_080f9010(s32 cue);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a188(s32 actor, s32 value, s32 frames);
extern void Func_0808a090(s32 actor, s32 x, s32 y);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a1e0(s32 actor, s32 mode);
extern void Func_0808a0f0(s32 actor, s32 x, s32 y);
extern void Func_0808a098(s32 actor, u8 *descriptor);
extern void Func_0808a0b0(s32 actor, u8 *descriptor);
extern void Func_0808a010(s32 frames);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a100(s32 actor, s32 pose);
extern void Func_02001424(s32 sequence);
extern void Func_0808a180(s32 actor, s32 phase);
extern void Func_0808a130(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1f0(s32 actor, s32 pose);
extern void Func_0808a260(s32 actor, s32 value);
extern void Func_0808a250(s32 actor, s32 value);
extern void Func_0808a020(void);

/* Establish the four-actor tableau and publish its presentation state. */
void Func_0200069c(void)
{
    s32 hasFourth = Func_080770c0(3);
    u8 *actor;

    Func_0808a018();
    Func_080f9010(17);
    Func_0808a170(0x14ce);
    Func_0808a188(0x8009, 0, 20);
    Func_080f9010(29);

    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a090(3, 0x10000, 0x8000);
    actor = Func_0808a080(3);
    actor[35] &= 0xfe;
    Func_0808a1e0(3, 2);
    actor = Func_0808a080(0);
    actor[35] &= 0xfe;
    Func_0808a1e0(0, 2);

    actor = Func_0808a080(0);
    if (actor != 0)
        Func_0808a0f0(1, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    actor = Func_0808a080(0);
    if (actor != 0)
        Func_0808a0f0(2, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    if (hasFourth != 0) {
        actor = Func_0808a080(0);
        if (actor != 0)
            Func_0808a0f0(3, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
        Func_0808a098(3, Data_02009e2c);
    }
    Func_0808a098(0, Data_02009db4);
    Func_0808a098(1, Data_02009ddc);
    Func_0808a0b0(2, Data_02009e04);

    Func_0808a010(10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 40);
    Func_0808a100(8, 11);
    Func_0808a010(10);
    Func_0808a100(8, 8);
    Func_0808a010(20);
    Func_02001424(8);
    Func_0808a180(0x8008, 0);

    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(3, 2);
    Func_0808a130(2, 2);
    Func_0808a1e8(0, 0x100, 0);
    Func_0808a1e8(1, 0x100, 0);
    Func_0808a1e8(3, 0x100, 0);
    Func_0808a1e8(2, 0x100, 60);
    Func_02001424(11);
    Func_0808a188(0x8008, 0, 10);

    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a130(3, 1);
    Func_0808a138(2, 1);
    Func_0808a180(0x8008, 0);
    Func_0808a1f0(0, 0x102);
    Func_0808a1f0(1, 0x102);
    Func_0808a1f0(3, 0x102);
    Func_0808a1f0(2, 0x102);
    Func_0808a010(40);
    Func_02001424(11);
    Func_0808a180(0x8008, 0);

    *(s32 *)(*(u8 **)0x03001ebc + 448) = 512;
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 64;
    Data_02000240[0x22b] = 3;
    Func_0808a260(45, 19);
    Func_0808a250(36, 0);
    Func_0808a020();
}
