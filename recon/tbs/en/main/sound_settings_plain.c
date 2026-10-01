/* 2026-10-01: plain numeric sound-driver settings attempt.
   Compile with the existing whole SOUND.C library route (old_agbcc -O2),
   unchanged options. Relative to that source, Audio_Initialize is 108
   rather than 120 bytes, Audio_StopAllPlayers and Audio_ResumeAllPlayers
   are 32 rather than 44 each, and CgbAudio_Initialize is 276 rather than
   280. The three loops lose their count load, u16 extension and zero
   guard; the mixer limit loses its literal load and changes allocation.
   The matching SOUND.C keeps these C bodies with tagged local literal
   loads and owns the numeric settings in source. No linker settings or
   whole-routine assembly are needed; this plain spelling stays a draft. */
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

enum { SOUND_PLAYER_COUNT = 8, SOUND_MAX_LINES = 0 };

void Audio_Initialize(void)
{
    u16 count;
    s32 i;

    Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), Ram_SoundMixer, 0x04000100);
    AudioEngine_Initialize(&Sound_Work);
    CgbAudio_Initialize(Sound_CgbNotes);
    AudioEngine_SetMode(AUDIO_INITIAL_MODE);
    count = SOUND_PLAYER_COUNT;
    for (i = 0; i < count; i++) {
        struct SoundPlayer *player = Sound_PlayerSlots[i].player;

        MusicPlayer_Initialize(player, Sound_PlayerSlots[i].tracks, Sound_PlayerSlots[i].track_count);
        player->check_priority = Sound_PlayerSlots[i].check_priority;
        player->work_bytes = Sound_WorkBytes;
    }
}

void Audio_StopAllPlayers(void)
{
    u16 count = SOUND_PLAYER_COUNT;
    s32 i;

    for (i = 0; i < count; i++)
        MusicPlayer_Stop(Sound_PlayerSlots[i].player);
}

void Audio_ResumeAllPlayers(void)
{
    u16 count = SOUND_PLAYER_COUNT;
    s32 i;

    for (i = 0; i < count; i++)
        Audio_ResumePlayer(Sound_PlayerSlots[i].player);
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
    work->max_lines = SOUND_MAX_LINES;

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
