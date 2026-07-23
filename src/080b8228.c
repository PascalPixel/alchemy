typedef unsigned char u8;
typedef signed int s32;

struct Wrapper080b8228 {
    u8 *object;
    u8 padding04[8];
    s32 scale;
    s32 z;
};

struct Wrapper080b8228 *Func_080b7dd0(s32);
u8 *Func_08077008(s32);
void Func_08009140(void *);
void Func_08009150(void *, s32, s32, s32);
void Func_08009080(void *, s32);
s32 Func_080022ec(s32, s32);

extern s32 Data_080c59a4[];
extern s32 Data_080c59c4[];
extern s32 Data_080c59e4[];
extern s32 Data_080c5a04[];

void Func_080b8228(s32 id, s32 variant)
{
    struct Wrapper080b8228 *wrapper;
    u8 *object;
    s32 index;
    s32 x;
    s32 *table;
    s32 scale;

    wrapper = Func_080b7dd0(id);
    object = wrapper->object;
    if (Func_08077008(id)[0x128] != 0x94) {
        table = Data_080c59a4;
        index = variant * 4;
        *(s32 *)(object + 0x34) = *(s32 *)((u8 *)table + index);
        table = Data_080c59c4;
        *(s32 *)(object + 0x30) = *(s32 *)((u8 *)table + index);
        if (*(s32 *)(object + 0x0c) == 0 || variant > 4) {
            table = Data_080c59e4;
            *(s32 *)(object + 0x28) = *(s32 *)((u8 *)table + index);
        }
        *(s32 *)(object + 0x48) = 0x9999;
        *(s32 *)(object + 0x44) = 0;
        object[0x5a] = 0;
        Func_08009140(object);
        scale = wrapper->scale;
        table = Data_080c5a04;
        x = Func_080022ec(scale * *(s32 *)((u8 *)table + index), 100);
        Func_08009150(object, x, 0, wrapper->z);
    }
    Func_08009080(object, 5);
}
