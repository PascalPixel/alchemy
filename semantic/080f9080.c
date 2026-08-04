#include "layout_guard.h"
#include "types.h"

typedef struct AudioRuntime_080f9080 {
    u8 mode_00;
    u8 padding_01[7];
    u16 control_08;
    u8 padding_0a[6];
    u16 control_10;
    u8 padding_12[2];
    u8 latch_14;
    u8 padding_15[11];
    u16 active_sound_20[8];
    u8 padding_30[4];
    u16 control_34;
    u8 padding_36[6];
    u8 current_sound_3c;
} AudioRuntime_080f9080;

typedef struct SoundKey_080f9080 {
    s32 value_00;
    u16 player_index_04;
    u16 padding_06;
} SoundKey_080f9080;

typedef struct MusicPlayer_080f9080 {
    u8 padding_00[4];
    u8 busy_04;
} MusicPlayer_080f9080;

typedef struct PlayerSlot_080f9080 {
    MusicPlayer_080f9080 *player_00;
    void *field_04;
    void *field_08;
} PlayerSlot_080f9080;

LAYOUT_OFFSET_GUARD(
    AudioRuntime080f9080_Control08,
    AudioRuntime_080f9080,
    control_08,
    8);
LAYOUT_OFFSET_GUARD(
    AudioRuntime080f9080_Control10,
    AudioRuntime_080f9080,
    control_10,
    0x10);
LAYOUT_OFFSET_GUARD(
    AudioRuntime080f9080_Latch,
    AudioRuntime_080f9080,
    latch_14,
    0x14);
LAYOUT_OFFSET_GUARD(
    AudioRuntime080f9080_ActiveSound,
    AudioRuntime_080f9080,
    active_sound_20,
    0x20);
LAYOUT_OFFSET_GUARD(
    AudioRuntime080f9080_Control34,
    AudioRuntime_080f9080,
    control_34,
    0x34);
LAYOUT_OFFSET_GUARD(
    AudioRuntime080f9080_CurrentSound,
    AudioRuntime_080f9080,
    current_sound_3c,
    0x3c);
LAYOUT_SIZE_GUARD(SoundKey080f9080_Size, SoundKey_080f9080, 8);
LAYOUT_OFFSET_GUARD(
    SoundKey080f9080_PlayerIndex,
    SoundKey_080f9080,
    player_index_04,
    4);
LAYOUT_SIZE_GUARD(PlayerSlot080f9080_Size, PlayerSlot_080f9080, 12);
LAYOUT_OFFSET_GUARD(
    MusicPlayer080f9080_Busy,
    MusicPlayer_080f9080,
    busy_04,
    4);

extern PlayerSlot_080f9080 Data_080fc624[];
extern SoundKey_080f9080 Data_080fc684[];

u32 Func_080037d4(u32);
void Func_080fa324(u16);
void Func_080fa4bc(void *, u16);
void Func_080faa58(MusicPlayer_080f9080 *, s32);
void Func_080fb2cc(void *, u32, u32);

/*
 * Dispatch a packed sound request.  The low twelve bits select the sound and
 * bit 12 controls one RAM setting for the ordinary low-numbered requests.
 */
void Func_080f9080(s32 request)
{
    volatile AudioRuntime_080f9080 *runtime =
        (volatile AudioRuntime_080f9080 *)0x02003000;
    u32 flags = request & 0xf000;
    u32 sound = request & 0x0fff;

    if (sound == 0x11) {
        if (runtime->latch_14 != 0)
            return;

        Func_080fa4bc((void *)0x02004290, 7);
        runtime->latch_14++;
        runtime->current_sound_3c = 0x13;
        return;
    }

    if (sound == 0x121) {
        runtime->active_sound_20[3] = 0;
        Func_080fa4bc((void *)0x02004360, 3);
        return;
    }

    if (sound > 99) {
        SoundKey_080f9080 *key = &Data_080fc684[sound];
        u16 player_index = key->player_index_04;

        if (player_index == 7) {
            while (Data_080fc624[player_index].player_00->busy_04 != 0) {
                player_index--;
                if (player_index <= 3) {
                    player_index = 7;
                    break;
                }
            }
        }

        Func_080faa58(
            Data_080fc624[player_index].player_00,
            key->value_00);
        runtime->active_sound_20[player_index] = sound;
        return;
    }

    if (sound > 79) {
        Func_080fb2cc((void *)0x02004290, 0xff, 0);
        runtime->control_34 = 0;
        runtime->control_08 = 0;
        Func_080fa324(sound);
        runtime->mode_00 = 10;
        return;
    }

    if (sound == 0x12 || sound == runtime->current_sound_3c)
        return;

    runtime->current_sound_3c = sound;
    {
        u32 mode = 2;

        if (sound == 0x43 || sound == 0x46 || sound == 0x4b)
            mode = 3;
        Func_080037d4(mode);
    }

    Func_080fa324(sound);
    if ((flags & 0x1000) != 0)
        runtime->control_08 = 0;
    else
        runtime->control_08 = 0x100;
    runtime->control_34 = 0x100;
    runtime->control_10 = 4;
    runtime->latch_14 = 0;
}
