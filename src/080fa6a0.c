typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef void (*NoArgumentCallback_080fa6a0)(void);
typedef void (*PointerCallback_080fa6a0)(void *);
typedef void (*ByteCallback_080fa6a0)(u8);
typedef s32 (*ThreeWordCallback_080fa6a0)(s32, s32, s32);
typedef void (*NoteCallback_080fa6a0)(u32, void *, void *);

union ByteCallbackSlot_080fa6a0 {
    NoArgumentCallback_080fa6a0 placeholder;
    ByteCallback_080fa6a0 handler;
};

union ThreeWordCallbackSlot_080fa6a0 {
    NoArgumentCallback_080fa6a0 placeholder;
    ThreeWordCallback_080fa6a0 handler;
};

struct SoundState_080fa6a0 {
    u32 ident;
    u8 pcm_dma_counter;
    u8 reverb;
    u8 max_pcm_channels;
    u8 master_volume;
    u8 pcm_rate;
    u8 unknown09[2];
    u8 pcm_dma_period;
    u8 unknown0c[4];
    u32 samples_per_vblank;
    u32 pcm_frequency;
    u32 frequency_scale;
    void *channel_state;
    PointerCallback_080fa6a0 callback20;
    void *callback_context;
    NoArgumentCallback_080fa6a0 callback28;
    union ByteCallbackSlot_080fa6a0 callback2c;
    union ThreeWordCallbackSlot_080fa6a0 callback30;
    void *command_table;
    NoteCallback_080fa6a0 note_handler;
    NoArgumentCallback_080fa6a0 callback3c;
    u8 reserved40[0x10];
    u8 channels[12][0x40];
    u8 pcm_buffers[2][0x630];
};

void Func_08006864(const void *source, void *destination, u32 control);
void Func_080f9a80(void *destination);
void Func_080f9f6c(u32, void *, void *);
void Func_080fb790(void);
void Func_080fa798(u32 value);

void Func_080fa6a0(struct SoundState_080fa6a0 *state)
{
    u32 zero;
    void *command_table;

    state->ident = 0;

    if ((*(volatile u32 *)0x040000c4 & 0x02000000) != 0)
        *(volatile u32 *)0x040000c4 = 0x84400004;
    if ((*(volatile u32 *)0x040000d0 & 0x02000000) != 0)
        *(volatile u32 *)0x040000d0 = 0x84400004;

    *(volatile u16 *)0x040000c6 = 0x400;
    *(volatile u16 *)0x040000d2 = 0x400;
    *(volatile u16 *)0x04000084 = 143;
    *(volatile u16 *)0x04000082 = 0xa90e;
    *(volatile u8 *)0x04000089 =
        (*(volatile u8 *)0x04000089 & 0x3f) | 0x40;

    *(volatile u32 *)0x040000bc = (u32)state->pcm_buffers[0];
    *(volatile u32 *)0x040000c0 = 0x040000a0;
    *(volatile u32 *)0x040000c8 = (u32)state->pcm_buffers[1];
    *(volatile u32 *)0x040000cc = 0x040000a4;

    *(struct SoundState_080fa6a0 * volatile *)0x03007ff0 = state;
    zero = 0;
    Func_08006864(
        &zero,
        state,
        0x01000000 | 0x04000000 | (sizeof(*state) / sizeof(u32)));

    state->max_pcm_channels = 8;
    state->master_volume = 15;
    state->note_handler = Func_080f9f6c;
    state->callback28 = Func_080fb790;
    state->callback2c.placeholder = Func_080fb790;
    state->callback30.placeholder = Func_080fb790;
    state->callback3c = Func_080fb790;

    command_table = (void *)0x02004000;
    Func_080f9a80(command_table);
    state->command_table = command_table;

    Func_080fa798(0x40000);
    state->ident = 0x68736d53;
}
