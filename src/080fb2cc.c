typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

struct MusicTrackState {
    u8 status;
    u8 pad01[18];
    u8 priority;
    u8 pad14[60];
};

struct MusicPlayerState {
    u8 pad00[8];
    u8 track_count;
    u8 pad09[35];
    struct MusicTrackState *tracks;
    u8 pad30[4];
    s32 ident;
    u8 pad38[8];
};

typedef char MusicTrackState_size[
    sizeof(struct MusicTrackState) == 0x50 ? 1 : -1
];
typedef char MusicPlayerState_size[
    sizeof(struct MusicPlayerState) == 0x40 ? 1 : -1
];

void Func_080fb2cc(struct MusicPlayerState *player, u32 mask, u32 value)
{
    u8 *entry;
    s32 count;
    u32 bit;
    register u32 selected;
    register u32 shifted;

    selected = (mask << 16) >> 16;
    shifted = (u32)(unsigned short)value << 16;
    if (player->ident == 0x68736d53) {
        player->ident += 1;
        count = player->track_count;
        entry = (u8 *)player->tracks;
        bit = 1;
        while (count > 0) {
            if ((selected & bit) && (entry[0] & 0x80)) {
                entry[19] = shifted >> 18;
                entry[0] |= 3;
            }
            count--;
            entry += 0x50;
            bit <<= 1;
        }
        player->ident = 0x68736d53;
    }
}
