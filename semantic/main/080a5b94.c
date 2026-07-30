typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080048b0(s32, s32);
void Func_08015408(s32, s32, s32, s32);
void Func_080030f8(u32);
void Func_080a1090(s32);
s8 Func_08077158(s32);
void Func_080a3354(s32, s32, s32, s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_080a2144(s32);
void Func_08015418(s32);
void Func_080a2474(void);
s32 Func_080a5cc0(s32 *, s32 *, s32 *);
void Func_080a2490(void);
u8 *Func_08077080(s32);
void Func_08015278(s32);
void Func_080a34c0(void);
void Func_08002dd8(s32);
s32 Func_08015410(s32, s32, s32, s32);
void Func_0808a548(s32);

s32 Func_080a5b94(void)
{
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 temp_r0;
    s32 temp_r6;
    s32 temp_r7;
    void *temp_r5;
    void *g;

    temp_r6 = Func_080048b0(0x37, 0xA70);
    g = (void *)0x03001E68;
    FIELD(FIELD(g, void **, 0), s16 *, 4) = 1;
    Func_08015408(0, 0, 0x1E, 0x14);
    Func_080030f8(1U);
    Func_080a1090(0);
    FIELD(temp_r6, s8 *, 0x219) = Func_08077158(temp_r6 + 0x208);
    Func_080a3354(0, 3, 0, 7);
    FIELD(temp_r6, s32 *, 0x10C) = Func_08015010(0xD, 0, 0x11, 3, 2);
    Func_080a2144(0xE);
    Func_08015418(0x06002500);
    Func_080a2474();
    temp_r7 = Func_080a5cc0(&spC, &sp8, &sp4);
    Func_080a2490();
    if (temp_r7 == 1) {
        temp_r5 = FIELD(g, void **, 0x54);
        Func_08077080(0x3FFF & FIELD(temp_r6, u16 *, 0x178));
        FIELD(temp_r5, s16 *, 0x17E) = (s16)(sp4 | (spC << 0xA));
    }
    Func_08015278(FIELD(temp_r6, s32 *, 0x24));
    FIELD(FIELD(g, void **, 0x24), s8 *, 0xEA6) = 1;
    Func_080a34c0();
    Func_08015408(0, 0, 0x1E, 0x14);
    Func_08002dd8(0x37);
    FIELD(FIELD(g, void **, 0), s16 *, 4) = 0;
    Func_080030f8(1U);
    temp_r0 = Func_08015410(0, 0, 0x1E, 0x14);
    FIELD(FIELD(g, void **, 0x24), s8 *, 0xEA6) = 0;
    Func_0808a548(temp_r0);
    return temp_r7;
}
