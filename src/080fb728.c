#include "audio_engine.h"

/* 戻り値は未使用だが、更新後位置を返す形でレジスタ順が一致する。 */
u8 *Func_080fb728(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u32 cursor;

    cursor = (u32)track->command;
    cursor = *(u8 *)cursor;
    track->byte_2e = cursor;
    cursor = (u32)track->command;
    cursor++;
    track->command = (u8 *)cursor;
    return (u8 *)cursor;
}
