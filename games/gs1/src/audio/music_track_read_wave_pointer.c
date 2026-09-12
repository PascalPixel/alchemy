#include "types.h"
#include "scene.h"
#include "audio_engine.h"

/* audio/music/track/read_wave_pointer.c */
union Value_080fb6a4 {
    u32 word;
    struct {
        u32 byte0 : 8;
        u32 byte1 : 8;
        u32 byte2 : 8;
        u32 byte3 : 8;
    } bits;
};

void MusicTrack_ReadWavePointer(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    union Value_080fb6a4 value;
    u8 *source = track->command;

    value.bits.byte0 = source[0];
    value.bits.byte1 = source[1];
    value.bits.byte2 = source[2];
    value.bits.byte3 = source[3];
    track->wave_pointer = value.word;
    track->command = source + 4;
}

/* audio/music/track/set_tone_decay.c */
/* 戻り値は未使用だが、更新後位置を返す形でレジスタ順が一致する。 */
u8 *MusicTrack_SetToneDecay(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u32 cursor;

    cursor = (u32)track->command;
    cursor = *(u8 *)cursor;
    track->tone_decay = cursor;
    cursor = (u32)track->command;
    cursor++;
    track->command = (u8 *)cursor;
    return (u8 *)cursor;
}

/* audio/music/track/set_tone_sustain.c */
/* 戻り値は未使用だが、更新後位置を返す形でレジスタ順が一致する。 */
u8 *MusicTrack_SetToneSustain(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u32 cursor;

    cursor = (u32)track->command;
    cursor = *(u8 *)cursor;
    track->tone_sustain = cursor;
    cursor = (u32)track->command;
    cursor++;
    track->command = (u8 *)cursor;
    return (u8 *)cursor;
}

/* audio/music/track/set_tone_release.c */
/* 戻り値は未使用だが、更新後位置を返す形でレジスタ順が一致する。 */
u8 *MusicTrack_SetToneRelease(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u32 cursor;

    cursor = (u32)track->command;
    cursor = *(u8 *)cursor;
    track->tone_release = cursor;
    cursor = (u32)track->command;
    cursor++;
    track->command = (u8 *)cursor;
    return (u8 *)cursor;
}

/* audio/music/track/set_pseudo_echo_volume.c */
/* 戻り値は未使用だが、更新後位置を返す形でレジスタ順が一致する。 */
u8 *MusicTrack_SetPseudoEchoVolume(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u8 *cursor;
    u8 value;

    cursor = (u8 *)track->command;
    value = *cursor;
    track->pseudo_echo_volume = value;
    cursor++;
    track->command = cursor;
    return cursor;
}

/* audio/music/track/set_pseudo_echo_length.c */
const u8 *MusicTrack_SetPseudoEchoLength(
    struct MusicPlayerState *player, struct MusicTrackState *track)
{
    const u8 *cursor;
    u8 value;

    cursor = track->command;
    value = *cursor;
    track->pseudo_echo_length = value;
    cursor++;
    track->command = cursor;
    return cursor;
}

/* audio/misc/dummy_callback.c */
void Audio_DummyCallback(void)
{
}
