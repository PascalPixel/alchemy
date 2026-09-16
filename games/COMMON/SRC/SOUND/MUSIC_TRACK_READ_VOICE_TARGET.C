#include "TYPES.H"

struct SoundPlayer;

struct SoundTrack {
    u8 unk00[0x28];
    u32 target;
    u8 unk2c[0x14];
    const u8 *cursor;
};

union PackedWord {
    u32 value;
    struct {
        u32 byte0:8;
        u32 byte1:8;
        u32 byte2:8;
        u32 byte3:8;
    } part;
};

void MusicTrack_ReadVoiceTarget(struct SoundPlayer *player, struct SoundTrack *track)
{
    const u8 *pos = track->cursor;
    union PackedWord word;

    word.part.byte0 = pos[0];
    word.part.byte1 = pos[1];
    word.part.byte2 = pos[2];
    word.part.byte3 = pos[3];
    track->target = word.value;
    track->cursor = pos + 4;
}
