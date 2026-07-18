typedef unsigned char u8;
typedef signed int s32;

struct State_080ad608 {
    u8 padding[0x224];
    void *objects[4];
};

extern struct State_080ad608 *Data_03001f2c;
extern s32 Data_080af304[];

void Func_08009038(void *);
void *Func_08009030(s32);
void Func_08009020(void *, s32);

s32 Func_080ad608(s32 index, s32 kind, s32 value)
{
    struct State_080ad608 *state = Data_03001f2c;

    if (state->objects[index] != 0) {
        Func_08009038(state->objects[index]);
        state->objects[index] = 0;
    }
    {
        void *object = Func_08009030(Data_080af304[kind]);

        if (object != 0) {
            Func_08009020(object, value);
        }
        state->objects[index] = object;
    }
    return 1;
}
