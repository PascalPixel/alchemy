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
