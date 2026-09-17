#include "AUDIO_ENGINE.H"

void AudioCommand_InvokeSlot35(void *block);
void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void AudioEngine_SetMode(u32 mode);

void MusicPlayer_Initialize(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count)
{
    struct SoundWork *work;

    if (count == 0)
        return;
    if (count > 16)
        count = 16;
    work = SOUND_WORK;
    if (work->lock != SOUND_LOCK)
        return;
    work->lock++;

    AudioCommand_InvokeSlot35(player);
    player->tracks = tracks;
    player->track_count = count;
    player->status = 0x80000000;
    while (count != 0) {
        tracks->flags = 0;
        count--;
        tracks++;
    }
    if (work->tick != NULL) {
        player->next_tick = work->tick;
        player->next_tick_player = work->tick_player;
        work->tick = NULL;
    }
    work->tick_player = player;
    work->tick = MusicPlayer_Tick;
    work->lock = SOUND_LOCK;
    player->lock = SOUND_LOCK;
}

void MusicPlayer_StartSong(struct SoundPlayer *player, const struct SequenceHeader *header)
{
    s32 i;
    struct SoundTrack *track;

    if (player->lock != SOUND_LOCK)
        return;

    if (player->check_priority != 0) {
        if (player->header == NULL || !(player->tracks[0].flags & 0x40)) {
            if ((player->status & 0xFFFF) == 0 || (player->status & 0x80000000))
                goto start;
        }
        if (player->priority > header->priority)
            return;
    }
start:
    player->lock++;
    player->status = 0;
    player->header = header;
    player->voices = header->voices;
    player->priority = header->priority;
    player->ticks = 0;
    player->tempo = 150;
    player->tempo_step = 150;
    player->tempo_scale = 256;
    player->tempo_count = 0;
    player->fade_period = 0;

    i = 0;
    track = player->tracks;
    while (i < header->track_count && i < player->track_count) {
        MusicTrack_Stop(player, track);
        track->flags = 0xC0;
        track->notes = NULL;
        track->cursor = header->tracks[i];
        i++;
        track++;
    }
    while (i < player->track_count) {
        MusicTrack_Stop(player, track);
        track->flags = 0;
        i++;
        track++;
    }
    if (header->mode & 0x80)
        AudioEngine_SetMode(header->mode);
    player->lock = SOUND_LOCK;
}

void MusicPlayer_Stop(struct SoundPlayer *player)
{
    s32 i;
    struct SoundTrack *track;

    if (player->lock != SOUND_LOCK)
        return;
    player->lock++;
    player->status |= 0x80000000;
    i = player->track_count;
    track = player->tracks;
    while (i > 0) {
        MusicTrack_Stop(player, track);
        i--;
        track++;
    }
    player->lock = SOUND_LOCK;
}
