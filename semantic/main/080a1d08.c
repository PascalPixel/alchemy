typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Display_080a1d08 {
    u8 padding00[5];
    u8 status;
};

struct State_080a1d08 {
    u8 padding000[0x14];
    struct Display_080a1d08 *display;
    u8 padding018[0x14];
    void *fallback_handle;
    u8 padding030[0x0C];
    void *active_handle;
    u8 padding040[0x1E2];
    u16 finished;
};

typedef char Display_080a1d08_size[
    sizeof(struct Display_080a1d08) == 8 ? 1 : -1
];
typedef char State_080a1d08_size[
    sizeof(struct State_080a1d08) == 0x224 ? 1 : -1
];

void Func_080030f8(s32);
s32 Func_08015108(s32, s32 *, s32 *, s32 *, s32 *);
void Func_08015270(void *);
void Func_08015278(void *);
void Func_08015080(s32, void *, s32, s32);
void Func_08015078(s32, void *, s32, s32);
void Func_080770c8(s32);
s32 Func_080a10d0(void **, s32, s32, s32, s32, s32);
void Func_080a1114(void **);
void Func_080a23f4(void *, s32, s32, s32, s32);

void Func_080a1d08(s32 character, s32 wait_mode, s32 variant)
{
    struct State_080a1d08 *state;
    void *handle;
    s32 output20;
    s32 output16;
    s32 output12;
    s32 output8;

    state = *(struct State_080a1d08 **)0x03001F2C;
    state->display->status = 13;

    if (variant != -1) {
        Func_08015108(
            character,
            &output20,
            &output16,
            &output12,
            &output8);
        if (Func_080a10d0(
                &state->active_handle,
                wait_mode,
                variant,
                output12,
                output8,
                0x102) == 0) {
            Func_080a23f4(
                state->active_handle,
                wait_mode,
                variant,
                output12,
                output8);
        }
        handle = state->active_handle;
    } else {
        handle = state->fallback_handle;
    }

    Func_08015270(handle);
    Func_08015278(handle);
    if (variant == -1)
        Func_08015080(character, handle, 0, 0);
    else
        Func_08015078(character, handle, 0, 0);

    if (wait_mode != -1) {
        Func_080030f8(1);
        do {
            Func_080030f8(1);
        } while ((*(u32 *)0x03001C94 & 0xB) == 0);

        if (variant == -1)
            Func_08015270(handle);
        Func_08015278(handle);
    } else {
        Func_080770c8(0x151);
    }

    state->finished = 1;
    state->display->status = 1;
    if (variant != -1)
        Func_080a1114(&state->active_handle);
}
