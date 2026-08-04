#include "types.h"

struct CgbChannel;
struct MusicPlayerState;
struct MusicTrackState;
struct AudioEngineState;

typedef void (*PlayerMainCallback)(struct MusicPlayerState *);

struct MusicPlayerState {
    u32 unknown00;
    u32 status;
    u8 track_count;
    u8 priority;
    u8 unknown0a;
    u8 config;
    u8 unknown0c[0x0c];
    u8 *memory_area;
    u8 unknown1c[0x10];
    struct MusicTrackState *tracks;
    void *unknown30;
    u32 ident;
    PlayerMainCallback next_callback;
    struct MusicPlayerState *next_player;
};

struct PlayerBootstrapRecord {
    struct MusicPlayerState *player;
    struct MusicTrackState *tracks;
    u8 max_tracks;
    u8 unknown09;
    u16 config;
};

void Func_08006864(s32, s32, s32);
void Func_080fa55c(struct CgbChannel *);
void Func_080fa6a0(struct AudioEngineState *);
void Func_080fa83c(u32 value);
void Func_080fa9e0(
    struct MusicPlayerState *,
    struct MusicTrackState *,
    u32);
extern u8 Data_080f9675;
extern u32 Value_00000008;
extern u8 Data_02004350;

void Func_080fa2a0(void)
{
    u16 count;

    Func_08006864((s32)&Data_080f9675 & ~1, 0x03007000, 0x04000100);
    Func_080fa6a0((struct AudioEngineState *)0x02003050);
    Func_080fa55c((struct CgbChannel *)0x02004090);
    Func_080fa83c(0x0097F800);

    count = (u32)&Value_00000008;
    if (count != 0) {
        struct PlayerBootstrapRecord *entry =
            (struct PlayerBootstrapRecord *)0x080FC624;
        u32 remaining = count;
        do {
            struct MusicPlayerState *player = entry->player;
            Func_080fa9e0(player, entry->tracks, entry->max_tracks);
            player->config = entry->config;
            player->memory_area = &Data_02004350;
            entry++;
            remaining--;
        } while (remaining != 0);
    }
}
