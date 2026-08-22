#include "audio_engine.h"

/* 戻り値は未使用だが、更新後位置を返す形でレジスタ順が一致する。 */
u8 *Func_080fb750(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u8 *cursor;
    u8 value;

    cursor = (u8 *)track->command;
    value = *cursor;
    track->byte_1e = value;
    cursor++;
    track->command = cursor;
    return cursor;
}
