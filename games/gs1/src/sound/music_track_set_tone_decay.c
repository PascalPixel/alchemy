#include "audio_engine.h"

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
