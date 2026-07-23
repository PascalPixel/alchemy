typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned z : 16;
    unsigned unused : 16;
};

struct Object {
    u8 filler0[6];
    u16 source6;
    u8 source8;
    u8 filler9[6];
    u8 output15;
    u8 filler16[4];
    u8 output20;
    u8 field21_0 : 2;
    u8 field21_2 : 6;
    u16 field22_0 : 9;
    u16 field22_9 : 5;
    u16 field22_14 : 2;
};

extern volatile u32 Data_03001800;
extern s32 Data_08037230[];
s32 Func_08003d28(struct Effect *effect);

void Func_080217a4(struct Object *object)
{
    s32 value = Data_08037230[(Data_03001800 >> 1) & 7];
    struct Effect effect;

    if (value < 0)
        value += 255;
    value >>= 8;

    if (object != 0) {
        effect.x = value;
        effect.y = value;
        effect.z = 0;
        object->field22_9 = Func_08003d28(&effect);
        object->field21_0 = 3;
        object->field22_0 = object->source6 + 0xfff0;
        object->output20 = object->source8 + 0xf0;
        object->output15 = 0xfc;
    }
}
