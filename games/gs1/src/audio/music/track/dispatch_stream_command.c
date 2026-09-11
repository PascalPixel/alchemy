#include "types.h"
#include "scene.h"
#include "abi/audio/music/track/dispatch_stream_command.h"

/* Handler per stream command byte, indexed by the byte itself. */
extern s32 gRom[];

/*
 * Take the next byte of the track's command stream, advance the read
 * pointer past it, and hand the matching handler to the runtime caller.
 */
void MusicTrack_DispatchStreamCommand(s32 player, s32 track)
{
    s32 command;

    command = *(u8 *)(*(s32 *)(track + 64));
    *(s32 *)(track + 64) += 1;
    Audio_Run(player, track, gRom[command]);
}
