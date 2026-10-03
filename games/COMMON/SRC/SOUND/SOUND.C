#include "AUDIO_ENGINE.H"
#include "RAM_BUFFER.H"

/* Shared driver settings, as MusicPlayer2000's C configuration supplies them. */
#define SOUND_PLAYER_COUNT 8
#define SOUND_MAX_LINES 0
#define SOUND_CLEAR_CONTROL 0x05000040
#define SOUND_STRINGIFY_IMPL(value) #value
#define SOUND_STRINGIFY(value) SOUND_STRINGIFY_IMPL(value)

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
    u16 count;
    /* FAKEMATCH: an unknown word followed by the u16 copy retains the extension. */
    u32 total;
    s32 i;
    /* FAKEMATCH: keep the slot cursor in r5 across the library calls. */
    register const struct PlayerSlot *slots asm("r5");
    /* FAKEMATCH: the work-area literal loads straight into the store's r0. */
    register u8 *bytes asm("r0");

    Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), Ram_SoundMixer, 0x04000100);
    AudioEngine_Initialize(&Sound_Work);
    CgbAudio_Initialize(Sound_CgbNotes);
    AudioEngine_SetMode(AUDIO_INITIAL_MODE);
    /* FAKEMATCH: retain the count's literal load, extension and zero guard. */
    asm volatile("ldr %0, .LSoundInitCount" : "=r"(total));
    count = total;
    if (count != 0) {
        /* FAKEMATCH: this literal follows the count in the same local pool. */
        asm volatile("ldr %0, .LSoundInitSlots" : "=r"(slots));
        i = count;
        do {
            struct SoundPlayer *player = slots->player;

            MusicPlayer_Initialize(player, slots->tracks, slots->track_count);
            player->check_priority = slots->check_priority;
            /* FAKEMATCH: the loop reloads the work-area literal for each player. */
            asm volatile("ldr %0, .LSoundInitBytes" : "=r"(bytes));
            player->work_bytes = bytes;
            slots++;
            i--;
        } while (i != 0);
    }
}

/* FAKEMATCH: retain the three local pool words and include them in the function extent. */
asm(".align 2, 0\n"
    ".LSoundInitCount:\n.word " SOUND_STRINGIFY(SOUND_PLAYER_COUNT) "\n"
    ".LSoundInitSlots:\n.word Sound_PlayerSlots\n"
    ".LSoundInitBytes:\n.word Sound_WorkBytes\n"
    ".size Audio_Initialize, .-Audio_Initialize");

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
    u16 count;
    /* FAKEMATCH: an unknown word followed by the u16 copy retains the extension. */
    u32 total;
    s32 i;
    /* FAKEMATCH: keep the slot cursor in r5 and the countdown in r4. */
    register const struct PlayerSlot *slots asm("r5");

    /* FAKEMATCH: retain the count's literal load, extension and zero guard. */
    asm volatile("ldr %0, .LSoundStopCount" : "=r"(total));
    count = total;
    if (count != 0) {
        /* FAKEMATCH: retain the slot cursor's literal after the guard. */
        asm volatile("ldr %0, .LSoundStopSlots" : "=r"(slots));
        i = count;
        do {
            MusicPlayer_Stop(slots->player);
            slots++;
            i--;
        } while (i != 0);
    }
}

/* FAKEMATCH: source-owned settings and table name, both in the complete function extent. */
asm(".align 2, 0\n"
    ".LSoundStopCount:\n.word " SOUND_STRINGIFY(SOUND_PLAYER_COUNT) "\n"
    ".LSoundStopSlots:\n.word Sound_PlayerSlots\n"
    ".size Audio_StopAllPlayers, .-Audio_StopAllPlayers");

void MusicPlayer_Resume(struct SoundPlayer *player)
{
    Audio_ResumePlayer(player);
}

void Audio_ResumeAllPlayers(void)
{
    u16 count;
    /* FAKEMATCH: an unknown word followed by the u16 copy retains the extension. */
    u32 total;
    s32 i;
    /* FAKEMATCH: keep the slot cursor in r5 and the countdown in r4. */
    register const struct PlayerSlot *slots asm("r5");

    /* FAKEMATCH: retain the count's literal load, extension and zero guard. */
    asm volatile("ldr %0, .LSoundResumeCount" : "=r"(total));
    count = total;
    if (count != 0) {
        /* FAKEMATCH: retain the slot cursor's literal after the guard. */
        asm volatile("ldr %0, .LSoundResumeSlots" : "=r"(slots));
        i = count;
        do {
            Audio_ResumePlayer(slots->player);
            slots++;
            i--;
        } while (i != 0);
    }
}

/* FAKEMATCH: source-owned settings and table name, both in the complete function extent. */
asm(".align 2, 0\n"
    ".LSoundResumeCount:\n.word " SOUND_STRINGIFY(SOUND_PLAYER_COUNT) "\n"
    ".LSoundResumeSlots:\n.word Sound_PlayerSlots\n"
    ".size Audio_ResumeAllPlayers, .-Audio_ResumeAllPlayers");

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
    u32 zero;
    /* FAKEMATCH: keep the configured limit and fill control as opaque literal outputs. */
    u32 limit;
    u32 control;

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
    /* FAKEMATCH: plain zero removes this limit literal and changes register allocation. */
    asm volatile("ldr %0, .LSoundMaxLines" : "=r"(limit));
    work->max_lines = limit;

    zero = 0;
    /* FAKEMATCH: keep the following control word beside the limit; materialize zero first. */
    asm volatile("ldr %0, .LSoundClearControl" : "=r"(control) : "m"(zero));
    Bios_CpuSet(&zero, notes, control);
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

/* FAKEMATCH: the two configured literals complete the initializer's existing pool. */
asm(".align 2, 0\n"
    ".LSoundMaxLines:\n.word " SOUND_STRINGIFY(SOUND_MAX_LINES) "\n"
    ".LSoundClearControl:\n.word " SOUND_STRINGIFY(SOUND_CLEAR_CONTROL) "\n"
    ".size CgbAudio_Initialize, .-CgbAudio_Initialize");
