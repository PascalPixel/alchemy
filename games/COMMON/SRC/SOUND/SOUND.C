#include "AUDIO_ENGINE.H"

void Sound_Mixer(void);
void AudioEngine_Initialize(struct SoundWork *work);
void CgbAudio_Initialize(struct SoundNote *notes);
void AudioEngine_SetMode(u32 mode);
void AudioEngine_RunMixerTick(void);
void MusicPlayer_Initialize(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count);
void MusicPlayer_StartSong(struct SoundPlayer *player, const struct SequenceHeader *header);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed);
void Audio_ResumePlayer(struct SoundPlayer *player);
void AudioCommand_InvokeSlot35(void *block);

extern struct SoundWork Sound_Work;
extern struct SoundNote Sound_CgbNotes[4];
extern u8 Sound_WorkBytes[];
extern const struct PlayerSlot Sound_PlayerSlots[];
extern const struct SongEntry Sound_SongTable[];
extern u8 Sound_PlayerCount;

void Audio_Initialize(void)
{
    u16 count;
    s32 i;

    Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), AUDIO_MIXER_DESTINATION, 0x04000100);
    AudioEngine_Initialize(&Sound_Work);
    CgbAudio_Initialize(Sound_CgbNotes);
    AudioEngine_SetMode(AUDIO_INITIAL_MODE);
    count = (u32)&Sound_PlayerCount;
    for (i = 0; i < count; i++) {
        struct SoundPlayer *player = Sound_PlayerSlots[i].player;

        MusicPlayer_Initialize(player, Sound_PlayerSlots[i].tracks, Sound_PlayerSlots[i].track_count);
        player->check_priority = Sound_PlayerSlots[i].check_priority;
        player->work_bytes = Sound_WorkBytes;
    }
}

void AudioEngine_RunMixer(void)
{
    AudioEngine_RunMixerTick();
}

void Audio_PlaySound(u16 id)
{
    const struct PlayerSlot *slots = Sound_PlayerSlots;
    const struct SongEntry *songs = Sound_SongTable;
    const struct SongEntry *song = &songs[id];

    MusicPlayer_StartSong(slots[song->slot].player, song->header);
}

void Audio_PlaySoundIfInactive(u16 id)
{
    const struct PlayerSlot *slots = Sound_PlayerSlots;
    const struct SongEntry *songs = Sound_SongTable;
    const struct SongEntry *song = &songs[id];
    struct SoundPlayer *player = slots[song->slot].player;

    if (player->header != song->header)
        MusicPlayer_StartSong(player, song->header);
    else if ((player->status & 0xFFFF) == 0 || (player->status & 0x80000000))
        MusicPlayer_StartSong(player, song->header);
}

void Audio_PlayOrResumeSound(u16 id)
{
    const struct PlayerSlot *slots = Sound_PlayerSlots;
    const struct SongEntry *songs = Sound_SongTable;
    const struct SongEntry *song = &songs[id];
    struct SoundPlayer *player = slots[song->slot].player;

    if (player->header != song->header)
        MusicPlayer_StartSong(player, song->header);
    else if ((player->status & 0xFFFF) == 0)
        MusicPlayer_StartSong(player, song->header);
    else if (player->status & 0x80000000)
        Audio_ResumePlayer(player);
}

void Audio_StopSound(u16 id)
{
    const struct PlayerSlot *slots = Sound_PlayerSlots;
    const struct SongEntry *songs = Sound_SongTable;
    const struct SongEntry *song = &songs[id];
    struct SoundPlayer *player = slots[song->slot].player;

    if (player->header == song->header)
        MusicPlayer_Stop(player);
}

void Audio_ResumeSound(u16 id)
{
    const struct PlayerSlot *slots = Sound_PlayerSlots;
    const struct SongEntry *songs = Sound_SongTable;
    const struct SongEntry *song = &songs[id];
    struct SoundPlayer *player = slots[song->slot].player;

    if (player->header == song->header)
        Audio_ResumePlayer(player);
}

void Audio_StopAllPlayers(void)
{
    u16 count = (u32)&Sound_PlayerCount;
    s32 i;

    for (i = 0; i < count; i++)
        MusicPlayer_Stop(Sound_PlayerSlots[i].player);
}

void MusicPlayer_Resume(struct SoundPlayer *player)
{
    Audio_ResumePlayer(player);
}

void Audio_ResumeAllPlayers(void)
{
    u16 count = (u32)&Sound_PlayerCount;
    s32 i;

    for (i = 0; i < count; i++)
        Audio_ResumePlayer(Sound_PlayerSlots[i].player);
}

void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed)
{
    MusicPlayer_BeginFadeOut(player, speed);
}

void MusicPlayer_FadeOutPause(struct SoundPlayer *player, u16 speed)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->fade_counter = speed;
        player->fade_period = speed;
        player->fade_volume = 0x101;
        player->lock = SOUND_LOCK;
    }
}

void MusicPlayer_FadeIn(struct SoundPlayer *player, u16 speed)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->fade_counter = speed;
        player->fade_period = speed;
        player->fade_volume = 2;
        player->status &= 0x7FFFFFFF;
        player->lock = SOUND_LOCK;
    }
}

void MusicPlayer_ResetActiveTracks(struct SoundPlayer *player)
{
    s32 count = player->track_count;
    struct SoundTrack *track = player->tracks;

    while (count > 0) {
        if (track->flags & 0x80) {
            if (track->flags & 0x40) {
                AudioCommand_InvokeSlot35(track);
                track->flags = 0x80;
                track->bend_range = 2;
                track->volume_scale = 0x40;
                track->lfo_speed = 22;
                track->voice.kind = 1;
            }
        }
        count--;
        track++;
    }
}
