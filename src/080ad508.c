#include "types.h"

struct State_080ad508 {
    u8 padding_000[0x224];
    void *objects[4];
    s16 positions_x[4];
    s16 positions_y[4];
    s32 phases[4];
};

extern struct State_080ad508 *Data_03001f2c;
extern s32 Data_080af304[];

void Func_08009038(void *);
void *Func_08009030(s32);
void Func_08009020(void *, s32);
void Func_080ad40c(void);
s32 Func_080041d8(void (*)(void), s32);

void Func_080ad508(void)
{
    struct State_080ad508 *state = Data_03001f2c;
    s32 index;

    for (index = 0; index < 4; index++) {
        void *object = state->objects[index];

        if (object != NULL) {
            Func_08009038(object);
            state->objects[index] = NULL;
        }
    }

    for (index = 0; index < 4; index++) {
        void *object = Func_08009030(Data_080af304[index]);

        if (object != NULL) {
            Func_08009020(object, 2);
        }
        state->objects[index] = object;
        state->phases[index] = 0x10000;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0xc8;
    }
    Func_080041d8(Func_080ad40c, 0xc80);
}
