typedef unsigned char u8;
typedef signed int s32;

struct State_080ad318 {
    u8 padding[0x224];
    void *objects[4];
};

extern struct State_080ad318 *Data_03001f2c;

void Func_08009038(void *);
void Func_08004278(s32);

void Func_080ad318(void)
{
    struct State_080ad318 *state = Data_03001f2c;
    s32 index = 0;

    do {
        void *object = state->objects[index];

        if (object != 0) {
            Func_08009038(object);
            state->objects[index] = 0;
        }
        index++;
    } while (index < 4);
    Func_08004278(0x080ad35d);
}
