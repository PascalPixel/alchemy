typedef unsigned char u8;
typedef signed int s32;

struct MusicPlayerState;

struct MusicTrackState {
    u8 unknown_00[0x40];
    u8 *command;
};

void Func_080072ec(
    struct MusicPlayerState *player,
    struct MusicTrackState *track,
    s32 handler);

void Func_080fb670(
    struct MusicPlayerState *player,
    struct MusicTrackState *track)
{
    u8 *cursor;
    s32 value;
    s32 address;

    cursor = track->command;
    value = *cursor;
    track->command = cursor + 1;
    address = 0x080fba48 + (value << 2);
    Func_080072ec(player, track, *(s32 *)address);
}
