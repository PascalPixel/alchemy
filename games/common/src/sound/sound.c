#include "types.h"
#include "audio_engine.h"
#include "ram_buffer.h"

s32 Audio_UmulHigh32(s32, s32);
extern u8 Sound_PcmPitchCodes[];
extern u32 Sound_PcmFrequencySteps[];

s32 Pcm_KeyToFrequency(void *wave, u8 key, u8 pitch)
{
    u32 key_index;
    u32 pitch_scale;
    u32 lower_frequency;
    u32 upper_frequency;

    key_index = key;
    pitch_scale = pitch << 24;
    if (key_index > 178) {
        key_index = 178;
        pitch_scale = 255 << 24;
    }

    lower_frequency = Sound_PcmPitchCodes[key_index];
    lower_frequency =
        Sound_PcmFrequencySteps[lower_frequency & 15] >> (lower_frequency >> 4);
    upper_frequency = Sound_PcmPitchCodes[key_index + 1];
    upper_frequency =
        Sound_PcmFrequencySteps[upper_frequency & 15] >> (upper_frequency >> 4);

    return Audio_UmulHigh32(*(s32 *)((u8 *)wave + 4),
        lower_frequency
            + Audio_UmulHigh32(
                upper_frequency - lower_frequency,
                pitch_scale));
}

void Audio_NoopStub(void)
{
}

// プレイヤー数と最大ライン数はサウンドデータ側のシンボルの値
extern char Sound_PlayerCount[];
extern char Sound_MaxLines[];
#define SOUND_PLAYER_COUNT ((u16)(u32)Sound_PlayerCount)
#define SOUND_MAX_LINES ((u32)Sound_MaxLines)

void Sound_Mixer(void);
void AudioEngine_Initialize(struct SoundWork *work);
void AudioEngine_SetMode(u32 mode);
void AudioEngine_RunMixerTick(void);
void MusicPlayer_Initialize(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count);
void MusicPlayer_StartSong(struct SoundPlayer *player, const struct SequenceHeader *header);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed);
void Audio_ResumePlayer(struct SoundPlayer *player);
extern struct SoundWork Sound_Work;
extern struct CgbNote Sound_CgbNotes[4];
extern u8 Sound_WorkBytes[];
extern const struct PlayerSlot Sound_PlayerSlots[];
extern const struct SongEntry Sound_SongTable[];

void MusicTrack_OperateWorkByte(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_ReleaseKey(struct SoundPlayer *player, struct SoundTrack *track);
void AudioEngine_SetPcmRate(u32 mode);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicTrack_CalcOutput(struct SoundPlayer *player, struct SoundTrack *track);
void Cgb_UpdateChannels(void);
void CgbChannel_Mute(u8 channel);
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine);
extern SoundCommand Sound_CommandTable[36];

void Audio_ResumePlayer(struct SoundPlayer *player)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->status &= ~SOUND_PLAYER_PAUSED;
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
    s32 i;

    Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), Ram_SoundMixer, 0x04000100);
    AudioEngine_Initialize(&Sound_Work);
    CgbAudio_Initialize(Sound_CgbNotes);
    AudioEngine_SetMode(AUDIO_INITIAL_MODE);
    for (i = 0; i < SOUND_PLAYER_COUNT; i++) {
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
    else if ((player->status & 0xFFFF) == 0 || (player->status & SOUND_PLAYER_PAUSED))
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
    else if (player->status & SOUND_PLAYER_PAUSED)
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
    s32 i;

    for (i = 0; i < SOUND_PLAYER_COUNT; i++)
        MusicPlayer_Stop(Sound_PlayerSlots[i].player);
}

void MusicPlayer_Resume(struct SoundPlayer *player)
{
    Audio_ResumePlayer(player);
}

void Audio_ResumeAllPlayers(void)
{
    s32 i;

    for (i = 0; i < SOUND_PLAYER_COUNT; i++)
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
        player->status &= ~SOUND_PLAYER_PAUSED;
        player->lock = SOUND_LOCK;
    }
}

void MusicPlayer_ResetActiveTracks(struct SoundPlayer *player)
{
    s32 count = player->track_count;
    struct SoundTrack *track = player->tracks;

    while (count > 0) {
        if (track->flags & SOUND_TRACK_ACTIVE) {
            if (track->flags & SOUND_TRACK_RESET) {
                AudioCommand_InvokeSlot35(track);
                track->flags = SOUND_TRACK_ACTIVE;
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

void CgbAudio_Initialize(struct CgbNote *notes)
{
    struct SoundWork *work;
    u32 lock;

    REG_SOUNDCNT_X = SOUNDCNT_X_ENABLE | SOUNDCNT_X_ALL_TONES_ON;
    REG_SOUNDCNT_L = 0;
    REG_NR12 = 8;
    REG_NR22 = 8;
    REG_NR42 = 8;
    REG_NR14 = 0x80;
    REG_NR24 = 0x80;
    REG_NR44 = 0x80;
    REG_NR30 = 0;
    REG_NR50 = 0x77;

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
    work->max_lines = SOUND_MAX_LINES;

    {
        volatile u32 zero = 0;

        Bios_CpuSet((void *)&zero, notes, 0x05000000 | (sizeof(struct CgbNote) * 4 / 4));
    }
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
