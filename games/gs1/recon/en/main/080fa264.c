#include "audio_engine.h"

#define MUSIC_PLAYER_SIGNATURE 0x68736d53
#define Audio_ResumePlayer Func_080fa264

void Audio_ResumePlayer(struct MusicPlayerState *player)
{
    if (player->ident == MUSIC_PLAYER_SIGNATURE)
        player->status &= 0x7fffffff;
}
