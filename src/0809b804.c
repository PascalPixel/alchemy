typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

struct State_0809b804;
typedef void (*Callback_0809b804)(struct State_0809b804 *);

struct State_0809b804 {
    u8 filler0[0x34];
    Callback_0809b804 callback;
    u16 count;
    s16 timer;
    u8 filler60[7];
    s8 update43;
    s8 update44;
    s8 active;
};

void Func_0809b86c(struct State_0809b804 *);
void Func_0809b8f4(struct State_0809b804 *);

void Func_0809b804(struct State_0809b804 *state)
{
    if (state->active != 0) {
        state->count++;
        if (state->timer != 0)
            state->timer--;
        else if (state->callback != 0)
            state->callback(state);
        if (state->active != 0) {
            if (state->update43 != 0)
                Func_0809b8f4(state);
            if (state->update44 != 0)
                Func_0809b86c(state);
        }
    }
}
