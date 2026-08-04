#include "types.h"

typedef struct {
    u32 source;
    u32 destination;
    u32 control;
} DmaRegisters;

typedef struct {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
} State;

extern void *Func_080048b0(u32, u32);
extern void Func_08002dd8(u32);
typedef void (*FirstCallback)(u32, State *, void *);
typedef void (*SecondCallback)(State *, void *, u16, u16);

typedef struct {
    u8 filler0[0xc4];
    union {
        FirstCallback first;
        SecondCallback second;
    } callback;
} CallbackTable;

extern CallbackTable Data_03001e50;

void Func_0801a5a4(State *state, u32 alternate)
{
    void *buffer;
    u32 size;
    u32 source;

    size = 0x278;
    buffer = Func_080048b0(49, size);
    *(DmaRegisters *)0x040000d4 = (DmaRegisters) {
        0x08015afc,
        (u32)buffer,
        0x84000000 | (size >> 2)
    };
    Data_03001e50.callback.first(state->value, state, &state->value);
    Func_08002dd8(49);

    if (alternate != 0) {
        source = 0x08015d74;
        size = 0x9c;
        buffer = Func_080048b0(49, size);
        *(DmaRegisters *)0x040000d4 = (DmaRegisters) {
            source,
            (u32)buffer,
            0x84000000 | (size >> 2)
        };
    } else {
        source = 0x08015e10;
        size = 0x7c;
        buffer = Func_080048b0(49, size);
        *(DmaRegisters *)0x040000d4 = (DmaRegisters) {
            source,
            (u32)buffer,
            0x84000000 | (size >> 2)
        };
    }
    Data_03001e50.callback.second(
        state, (u8 *)state + 0x400, state->first, state->second);
    Func_08002dd8(49);
}
