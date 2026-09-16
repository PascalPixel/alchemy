#include "AUDIO_ENGINE.H"

void MusicTrack_OperateWorkByte(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_ReleaseKey(struct SoundPlayer *player, struct SoundTrack *track);
void AudioEngine_SetPcmRate(u32 mode);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicTrack_CalcOutput(struct SoundPlayer *player, struct SoundTrack *track);
void CgbAudio_Update(void);
void CgbChannel_Mute(u8 channel);
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine);

extern SoundCommand Sound_CommandTable[36];
extern u8 Value_00000000;

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
    work->cgb_update = CgbAudio_Update;
    work->cgb_mute = CgbChannel_Mute;
    work->cgb_frequency = Cgb_KeyToFrequency;
    work->unk0c = (u32)&Value_00000000;

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
