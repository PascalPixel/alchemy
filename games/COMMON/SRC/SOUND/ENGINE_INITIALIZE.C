#include "AUDIO_ENGINE.H"

s32 Math_Div(s32 numerator, s32 denominator);
void Sound_LoadCommandTable(SoundCommand *table);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player, struct SoundTrack *track);
void Audio_EmptyCallback(void);
void AudioEngine_SetPcmRate(u32 mode);
void AudioEngine_SuspendDirectSound(void);
void AudioEngine_ResumeDirectSound(void);

extern const u16 Sound_FrameLengths[];
extern SoundCommand Sound_CommandTable[36];

void AudioEngine_Initialize(struct SoundWork *work)
{
    u32 zero;

    work->lock = 0;
    if (*(u32 *)0x040000C4 & 0x02000000)
        *(u32 *)0x040000C4 = 0x84400004;
    if (*(u32 *)0x040000D0 & 0x02000000)
        *(u32 *)0x040000D0 = 0x84400004;
    *(u16 *)0x040000C6 = 0x400;
    *(u16 *)0x040000D2 = 0x400;
    *(u16 *)0x04000084 = 0x8F;
    *(u16 *)0x04000082 = 0xA90E;
    *(volatile u8 *)0x04000089 = (*(volatile u8 *)0x04000089 & 0x3F) | 0x40;
    *(u32 *)0x040000BC = (u32)work->buffers[0];
    *(u32 *)0x040000C0 = 0x040000A0;
    *(u32 *)0x040000C8 = (u32)work->buffers[1];
    *(u32 *)0x040000CC = 0x040000A4;
    SOUND_WORK = work;
    zero = 0;
    Bios_CpuSet(&zero, work, 0x050003EC);
    work->pcm_note_count = 8;
    work->volume = 15;
    work->note_on = MusicTrack_HandleNote;
    work->cgb_update = Audio_EmptyCallback;
    work->cgb_mute = (void (*)(u8))Audio_EmptyCallback;
    work->cgb_frequency = (s32 (*)(u8, u8, u8))Audio_EmptyCallback;
    work->unk3c = Audio_EmptyCallback;
    Sound_LoadCommandTable(Sound_CommandTable);
    work->commands = Sound_CommandTable;
    AudioEngine_SetPcmRate(0x40000);
    work->lock = SOUND_LOCK;
}

void AudioEngine_SetPcmRate(u32 mode)
{
    struct SoundWork *work = SOUND_WORK;
    u32 frame;
    u16 *timer;

    mode = (mode & 0xF0000) >> 16;
    work->rate = mode;
    frame = Sound_FrameLengths[mode - 1];
    work->frame_samples = frame;
    work->transfer_period = Math_Div(0x630, frame);
    work->output_rate = Math_Div(0x91D1B * frame + 5000, 10000);
    work->rate_step = (Math_Div(0x1000000, work->output_rate) + 1) >> 1;
    *(u16 *)0x04000102 = 0;
    timer = (u16 *)0x04000100;
    *timer = -Math_Div(0x44940, frame);
    AudioEngine_ResumeDirectSound();
    while (*(volatile u8 *)0x04000006 == 159)
        ;
    while (*(volatile u8 *)0x04000006 != 159)
        ;
    *(u16 *)0x04000102 = 0x80;
}

void AudioEngine_SetMode(u32 mode)
{
    struct SoundWork *work = SOUND_WORK;
    u32 value;
    struct SoundNote *note;

    if (work->lock != SOUND_LOCK)
        return;
    work->lock++;

    value = mode & 0xFF;
    if (value != 0)
        work->reverb = value & 0x7F;
    value = mode & 0xF00;
    if (value != 0) {
        work->pcm_note_count = value >> 8;
        value = 12;
        note = work->pcm_notes;
        do {
            note->state = 0;
            value--;
            note++;
        } while (value != 0);
    }
    value = mode & 0xF000;
    if (value != 0)
        work->volume = value >> 12;
    value = mode & 0xB00000;
    if (value != 0) {
        value = (value & 0x300000) >> 14;
        *(volatile u8 *)0x04000089 = (*(volatile u8 *)0x04000089 & 0x3F) | value;
    }
    value = mode & 0xF0000;
    if (value != 0) {
        AudioEngine_SuspendDirectSound();
        AudioEngine_SetPcmRate(value);
    }
    work->lock = SOUND_LOCK;
}

void AudioEngine_StopAllChannels(void)
{
    struct SoundWork *work = SOUND_WORK;
    struct SoundNote *note;
    s32 i;

    if (work->lock != SOUND_LOCK)
        return;
    work->lock++;

    i = 12;
    note = work->pcm_notes;
    while (i > 0) {
        note->state = 0;
        i--;
        note++;
    }
    note = work->cgb_notes;
    if (note != NULL) {
        for (i = 1; i <= 4; i++, note++) {
            work->cgb_mute(i);
            note->state = 0;
        }
    }
    work->lock = SOUND_LOCK;
}

void AudioEngine_SuspendDirectSound(void)
{
    struct SoundWork *work = SOUND_WORK;
    u32 zero;

    if (work->lock - SOUND_LOCK > 1)
        return;
    work->lock += 10;
    if (*(u32 *)0x040000C4 & 0x02000000)
        *(u32 *)0x040000C4 = 0x84400004;
    if (*(u32 *)0x040000D0 & 0x02000000)
        *(u32 *)0x040000D0 = 0x84400004;
    *(u16 *)0x040000C6 = 0x400;
    *(u16 *)0x040000D2 = 0x400;
    zero = 0;
    Bios_CpuSet(&zero, work->buffers, 0x05000318);
}
