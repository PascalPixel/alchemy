typedef signed short s16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

struct Output {
    s32 x;
    s32 y;
    s32 z;
};

struct Temp {
    s32 x;
    s32 y;
    s32 z;
};

extern s16 * volatile Data_03001ebc;

void Func_08005268(void *, struct Temp *);

void Func_080974d8(struct Output *output)
{
    struct Temp temp;
    s32 z;

    if (Data_03001ebc[207] == 3) {
        Func_08005268(output, &temp);
        output->x = temp.x << 16;
        z = temp.y << 16;
    } else {
        u8 *state;
        u32 stateZ;
        u32 stateX;
        state = *((u8 **)&Data_03001ebc - 19);
        stateX = *(u32 *)(state + 228) & 0xffff0000;
        stateZ = *(u32 *)(state + 232) & 0xffff0000;
        output->x -= stateX;
        z = output->z - output->y - stateZ;
    }

    output->z = z;
    output->y = 0;
}
