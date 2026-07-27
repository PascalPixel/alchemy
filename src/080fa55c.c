typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned int u32;

typedef void (*NoArgumentCallback_080fa55c)(void);
typedef void (*PointerCallback_080fa55c)(void *);
typedef void (*ByteCallback_080fa55c)(u8);
typedef s32 (*ThreeWordCallback_080fa55c)(s32, s32, s32);
typedef void (*TwoArgumentCommand_080fa55c)(void *, void *);
typedef void (*WordPointerCommand_080fa55c)(s32, void *);
typedef void (*WordCommand_080fa55c)(u32);
typedef void (*PointerCommand_080fa55c)(void *);

union ByteCallbackSlot_080fa55c {
    NoArgumentCallback_080fa55c placeholder;
    ByteCallback_080fa55c handler;
};

union ThreeWordCallbackSlot_080fa55c {
    NoArgumentCallback_080fa55c placeholder;
    ThreeWordCallback_080fa55c handler;
};

union CommandSlot_080fa55c {
    TwoArgumentCommand_080fa55c two_arguments;
    WordPointerCommand_080fa55c word_pointer;
    WordCommand_080fa55c word;
    PointerCommand_080fa55c pointer;
};

struct CgbChannel_080fa55c {
    u8 active;
    u8 channel_id;
    u8 right_volume;
    u8 left_volume;
    u8 unknown04[2];
    u8 sustain;
    u8 unknown07[3];
    u8 envelope;
    u8 unknown0b[14];
    s8 stage;
    u8 unknown1a;
    u8 pan;
    u8 pan_mask;
    u8 reserved1d[0x23];
};

struct SoundState_080fa55c {
    u32 ident;
    u8 pcm_dma_counter;
    u8 reverb;
    u8 max_pcm_channels;
    u8 master_volume;
    u8 pcm_rate;
    u8 unknown09[2];
    u8 pcm_dma_period;
    u8 field0c;
    u8 unknown0d[3];
    u32 samples_per_vblank;
    u32 pcm_frequency;
    u32 frequency_scale;
    struct CgbChannel_080fa55c *channel_state;
    PointerCallback_080fa55c callback20;
    void *callback_context;
    NoArgumentCallback_080fa55c callback28;
    union ByteCallbackSlot_080fa55c callback2c;
    union ThreeWordCallbackSlot_080fa55c callback30;
};

void Func_08006864(const void *source, void *destination, u32 control);
void Func_080fb518(void *, void *);
void Func_080fa1d4(void *, void *);
void Func_080fa1e8(void *, void *);
void Func_080fb670(s32, void *);
void Func_080fa16c(void *, void *);
void Func_080fa798(u32);
void Func_080f9ef8(void *, void *);
void Func_080fab7c(void *);
void Func_080fac44(void *, void *);
void Func_080fae58(void);
void Func_080fada0(u8);
s32 Func_080facf8(s32, s32, s32);
extern u8 Value_00000000;

void Func_080fa55c(struct CgbChannel_080fa55c *channels)
{
    u32 zero;
    struct SoundState_080fa55c *state;
    union CommandSlot_080fa55c *command_table;
    u32 ident;

    *(volatile u16 *)0x04000084 = 143;
    *(volatile u16 *)0x04000080 = 0;
    *(volatile u8 *)0x04000063 = 8;
    *(volatile u8 *)0x04000069 = 8;
    *(volatile u8 *)0x04000079 = 8;
    *(volatile u8 *)0x04000065 = 128;
    *(volatile u8 *)0x0400006d = 128;
    *(volatile u8 *)0x0400007d = 128;
    *(volatile u8 *)0x04000070 = 0;
    *(volatile u8 *)0x04000080 = 119;

    state = *(struct SoundState_080fa55c * volatile *)0x03007ff0;
    ident = state->ident;
    if (ident != 0x68736d53)
        return;

    state->ident = ident + 1;

    command_table = (union CommandSlot_080fa55c *)0x02004000;
    command_table[8].two_arguments = Func_080fb518;
    command_table[17].two_arguments = Func_080fa1d4;
    command_table[19].two_arguments = Func_080fa1e8;
    command_table[28].word_pointer = Func_080fb670;
    command_table[29].two_arguments = Func_080fa16c;
    command_table[30].word = Func_080fa798;
    command_table[31].two_arguments = Func_080f9ef8;
    command_table[32].pointer = Func_080fab7c;
    command_table[33].two_arguments = Func_080fac44;

    state->channel_state = channels;
    state->callback28 = Func_080fae58;
    state->callback2c.handler = Func_080fada0;
    state->callback30.handler = Func_080facf8;
    state->field0c = (u32)&Value_00000000;

    zero = 0;
    Func_08006864(
        &zero,
        channels,
        0x01000000 | 0x04000000 |
            (sizeof(struct CgbChannel_080fa55c) * 4 / sizeof(u32)));

    channels[0].channel_id = 1;
    channels[0].pan_mask = 0x11;
    channels[1].channel_id = 2;
    channels[1].pan_mask = 0x22;
    channels[2].channel_id = 3;
    channels[2].pan_mask = 0x44;
    channels[3].channel_id = 4;
    channels[3].pan_mask = 0x88;

    state->ident = ident;
}
