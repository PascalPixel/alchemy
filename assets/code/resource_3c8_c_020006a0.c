typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

struct Object_020006a0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams_020006a0 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

extern u32 Data_03001e40;

s32 Func_02005470();
s32 Func_020054cc();
s32 Func_0200081a();
s32 Func_02000852();

s32 Func_020006a0(struct Object_020006a0 *object)
{
    struct EffectParams_020006a0 params;
    s32 speed;
    s32 phase;

    params.color1 = 0x0000cccc;
    params.color2 = 0x0000cccc;
    params.unk00 = 0;

    speed = (s32)((u32)(Func_02005470() * 8) >> 16) * 0x3333;
    phase = Data_03001e40 & 15;

    Func_0200081a(
        object->x + ((8 - phase) << 16),
        object->y + 0x001a0000,
        object->z,
        0,
        -speed,
        0,
        0x000a0000,
        &params);

    phase = Data_03001e40 & 15;
    if (phase == 0) {
        params.color1 = 0x00008000;
        params.color2 = 0x00008000;
        Func_02000852(
            object->x
                + (((s32)((u32)(Func_020054cc() * 9) >> 16) - 4) << 16),
            object->y,
            object->z,
            0,
            0,
            0,
            0x000a0000,
            &params);
    }

    return 0;
}
