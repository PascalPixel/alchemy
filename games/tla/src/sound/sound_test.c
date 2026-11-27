// The sound test, which nothing in the game calls; the heard-sound flags the link lobby offers its
// tunes by; and the fixed 31536 Hz output rate the game sets at start-up. Linked after
// sySoundPlay's object, with the BIOS sound call between them.

#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "sdk/sound_driver.h"

// Unused: the sound test's own empty function, as games/common/sdk/sound/sound.c keeps
// UnusedDummyFunc; nothing calls it.
void sySoundTestUnusedDummyFunction(void)
{
}

// The sounds the test's three selectors start on: one each for music, jingles and sound effects.
static const SND_TEST_SNDS SoundTestSounds = {
    {0, SOUND_JINGLE_FIRST, SOUND_SE_FIRST}
};

// Unused: the sound test, which never returns. Up and Down choose a selector (music, jingles, sound
// effects), Left and Right step its sound by 1 and L and R by 10, A plays it, B silences the music
// and Start fades it, and Select fades the action sounds and steps the reverb code, from
// SOUND_REVERB_LONG, which this game's sySoundLoadReverbCode ignores.
void sySoundTestRun(void)
{
    SND_TEST_SNDS sounds;
    Sint32        reverbCode;
    Sint32        row;
    Sint32        timer;

    sounds     = SoundTestSounds;
    row        = 0;
    reverbCode = SOUND_REVERB_LONG;
    // Clears the build stamp's first word, as The Broken Seal's test does. Nothing in either game
    // reads the stamp once syCopyBuildStamp has written it at power-on, so nothing on screen
    // changes.
    syIOWrite32(0, &BuildStampWork[0]);
    SoundVBlankStateWork.soundTestActive = 1;
    timer                                = 20;

    for (;;)
    {
        // A countdown, restarted whenever the mixer has clipped a sample since the last tick;
        // nothing reads it.
        if (timer != 0)
            timer--;

        if (SoundMixerClipped != 0)
        {
            SoundMixerClipped = 0;
            timer             = 2;
        }

        if (PerWork.repeat & KEY_SELECT)
        {
            reverbCode = (reverbCode + 1) % 5;
            sySoundLoadReverbCode(reverbCode);
        }
        if (PerWork.repeat & KEY_R)
            sounds.sound[row] += 10;
        if (PerWork.repeat & KEY_L)
            sounds.sound[row] -= 10;
        if (PerWork.repeat & KEY_RIGHT)
            sounds.sound[row] += 1;
        if (PerWork.repeat & KEY_LEFT)
            sounds.sound[row] -= 1;
        if (PerWork.repeat & KEY_UP)
        {
            if (row > 0)
                row--;
        }
        if (PerWork.repeat & KEY_DOWN)
        {
            if (row <= 1)
                row++;
        }
        if (PerWork.repeat & KEY_A)
            sySoundPlay(sounds.sound[row]);
        if (PerWork.repeat & KEY_B)
            sySoundPlay(MUS_SILENCE);
        if (PerWork.repeat & KEY_START)
            sySoundPlay(SOUND_FADE_OUT_BGM);
        if (PerWork.repeat & KEY_SELECT)
            sySoundPlay(SOUND_FADE_OUT_ACTION_SE);

        sySynch(1);
    }
}

// Unused: replaces SoundCommandMask, or flips the bits named with AUDIO_COMMAND_TOGGLE. Nothing
// calls it and nothing reads the mask.
void sySoundUpdateCommandMask(Uint32 command)
{
    Uint32 toggle  = command & AUDIO_COMMAND_TOGGLE;

    command       &= AUDIO_COMMAND_MASK;
    if (toggle != 0)
        SoundCommandMask ^= command;
    else
        SoundCommandMask = command;
}

// Returns the music sySoundPlay last started: MUS_SILENCE before any and after a fade.
Uint16 sySoundGetCurrentBGM(void)
{
    return CurrentBgm;
}

// The sound ids SoundFlags has a bit for.
enum
{
    SOUND_FLAG_COUNT = 1024
};

// Returns sound `sound`'s bit in SoundFlags, one bit for each of the SOUND_FLAG_COUNT sound ids.
Bool sySoundFlagTest(Sint32 sound)
{
    Sint32 mask;
    Uint8 *bytes;

    mask  = 1 << (7 & sound);
    sound = (sound & (SOUND_FLAG_COUNT - 1)) >> 3;
    bytes = SoundFlags;
    if (bytes[sound] & mask)
        return TRUE;
    return FALSE;
}

// Sets sound `sound`'s bit in SoundFlags.
void sySoundFlagSet(Sint32 sound)
{
    Sint32 mask;
    Uint8 *bytes;

    mask         = 1 << (7 & sound);
    bytes        = SoundFlags;
    sound        = (sound & (SOUND_FLAG_COUNT - 1)) >> 3;
    bytes[sound] = (Uint8)(bytes[sound] | mask);
}

// Clears sound `sound`'s bit in SoundFlags.
void sySoundFlagClear(Sint32 sound)
{
    Sint32 mask;
    Uint8 *bytes;

    mask         = 1 << (7 & sound);
    bytes        = SoundFlags;
    sound        = (sound & (SOUND_FLAG_COUNT - 1)) >> 3;
    bytes[sound] = (Uint8)(bytes[sound] & ~mask);
}

// The fixed output rate sySoundInit sets for the whole game.
enum
{
    // samples a second: SOUND_MODE_FREQ_GAME's rate
    SOUND_FIXED_RATE               = 31536,
    // SOUND_FIXED_RATE over the 59.7275 frames a second, rounded.
    SOUND_FIXED_SAMPLES_PER_VBLANK = 528
};

// Sets direct sound to the fixed SOUND_FIXED_RATE with SOUND_FIXED_SAMPLES_PER_VBLANK samples a
// frame, keeping mode's rate nibble, clears the two PCM buffers and points DMA 1 and 2 at them,
// then restarts timer 0 as the screen's last line begins.
void sySoundSetFixedSampleFrequency(Uint32 mode)
{
    struct SoundInfo *work = SOUND_INFO_PTR;
    Uint32            samples;
    Uint8            *buf;
    Uint32            control;
    volatile Uint8   *vcount;

    mode = (mode >> SOUND_MODE_FREQ_SHIFT) & (SOUND_MODE_FREQ >> SOUND_MODE_FREQ_SHIFT);
    work->transferPeriod      = SOUND_PCM_BUFFER_SIZE / SOUND_FIXED_SAMPLES_PER_VBLANK;
    work->outputRate          = SOUND_FIXED_RATE;
    work->rate                = mode;
    samples                   = SOUND_FIXED_SAMPLES_PER_VBLANK;
    work->pcmSamplesPerVBlank = samples;
    // Half the rate's reciprocal in 24-bit fixed point, as SampleFreqSet's.
    work->rateStep            = SOUND_RATE_STEP_ONE / 2 / SOUND_FIXED_RATE;
    control                   = 0;
    syIOWrite16(control, &REG_TM0CNT_H);
    // Timer 0 overflows twice a sample: half a frame's cycles over its samples.
    syIOWrite16(__divsi3(-(SOUND_CYCLES_PER_VBLANK / 2), samples), &REG_TM0CNT_L);
    buf = MixerPcmBuffer;
    // Each channel's buffer here is twice the driver's own.
    syWordsClear(buf, 4 * SOUND_PCM_BUFFER_SIZE);
    m4aSoundVSyncOn();
    REG_DMA1SAD  = (Uint32)buf;
    buf         += 2 * SOUND_PCM_BUFFER_SIZE;
    syIOWrite32((Uint32)buf, &REG_DMA2SAD);
    vcount = (volatile Uint8 *)&REG_VCOUNT;
    while (*vcount == DISPLAY_HEIGHT - 1)
        ;
    while (*vcount != DISPLAY_HEIGHT - 1)
        ;
    control      = TIMER_ENABLE;
    REG_TM0CNT_H = control;
}
