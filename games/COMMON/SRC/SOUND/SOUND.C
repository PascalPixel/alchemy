#include "AUDIO_ENGINE.H"
#include "RAM_BUFFER.H"

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

void MusicTrack_OperateWorkByte(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_ReleaseKey(struct SoundPlayer *player, struct SoundTrack *track);
void AudioEngine_SetPcmRate(u32 mode);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicTrack_CalcOutput(struct SoundPlayer *player, struct SoundTrack *track);
void Cgb_UpdateChannels(void);
void CgbChannel_Mute(u8 channel);
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine);
extern SoundCommand Sound_CommandTable[36];
extern u8 Sound_MaxLines;

void Audio_ResumePlayer(struct SoundPlayer *player)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->status &= 0x7fffffff;
        player->lock = SOUND_LOCK;
    }
}

void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->fade_counter = speed;
        player->fade_period = speed;
        player->fade_volume = 0x100;
        player->lock = SOUND_LOCK;
    }
}

void Audio_Initialize(void)
{
    u16 count;
    s32 i;

    Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), Ram_SoundMixer, 0x04000100);
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

void CgbAudio_Initialize(struct SoundNote *notes)
{
    struct SoundWork *work;
    u32 lock;
    u32 zero;

    *(u16 *)0x04000084 = 0x8F;
    *(u16 *)0x04000080 = 0;
    *(u8 *)0x04000063 = 8;
    *(u8 *)0x04000069 = 8;
    *(u8 *)0x04000079 = 8;
    *(u8 *)0x04000065 = 0x80;
    *(u8 *)0x0400006D = 0x80;
    *(u8 *)0x0400007D = 0x80;
    *(u8 *)0x04000070 = 0;
    *(u8 *)0x04000080 = 0x77;

    work = SOUND_WORK;
    lock = work->lock;
    if (lock != SOUND_LOCK)
        return;
    work->lock = lock + 1;

    Sound_CommandTable[8] = MusicTrack_OperateWorkByte;
    Sound_CommandTable[17] = MusicTrack_SetLfoSpeedFromCommand;
    Sound_CommandTable[19] = MusicTrack_SetModulationFromCommand;
    Sound_CommandTable[28] = MusicTrack_DispatchStreamCommand;
    Sound_CommandTable[29] = MusicTrack_ReleaseKey;
    Sound_CommandTable[30] = (SoundCommand)AudioEngine_SetPcmRate;
    Sound_CommandTable[31] = MusicTrack_Stop;
    Sound_CommandTable[32] = (SoundCommand)MusicPlayer_StepFade;
    Sound_CommandTable[33] = MusicTrack_CalcOutput;

    work->cgb_notes = notes;
    work->cgb_update = Cgb_UpdateChannels;
    work->cgb_mute = CgbChannel_Mute;
    work->cgb_frequency = Cgb_KeyToFrequency;
    work->max_lines = (u32)&Sound_MaxLines;

    zero = 0;
    Bios_CpuSet(&zero, notes, 0x05000040);
    notes[0].kind = 1;
    notes[0].channel_bits = 0x11;
    notes[1].kind = 2;
    notes[1].channel_bits = 0x22;
    notes[2].kind = 3;
    notes[2].channel_bits = 0x44;
    notes[3].kind = 4;
    notes[3].channel_bits = 0x88;

    work->lock = lock;
}
