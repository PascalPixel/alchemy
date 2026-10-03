#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

#include "TYPES.H"

enum AudioCommandMask {
    AUDIO_COMMAND_TOGGLE = 0x80,
    AUDIO_COMMAND_MASK = 0x7f
};

#define AUDIO_INITIAL_MODE 0x0097F800

enum AudioCueControl {
    AUDIO_CUE_ID_MASK = 0xfff,
    AUDIO_CUE_FLAGS_MASK = 0xf000,
    AUDIO_CUE_START_SILENT = 0x1000,
    AUDIO_CUE_FADE_BGM = 17,
    AUDIO_CUE_KEEP_BGM = 18,
    AUDIO_CUE_FADE_SLOT3 = 0x121,
    AUDIO_JINGLE_FIRST = 80,
    AUDIO_SONG_FIRST = 100
};

s32 Sound_LoadPresetParameters(s32 preset);

void Bios_CpuSet(const void *source, void *destination, u32 control);

struct SoundPlayer;
struct AudioTrackSlotWork;
extern struct AudioTrackSlotWork *Data_02004c00;
extern struct SoundPlayer gMusicPlayerBgm;
extern struct SoundPlayer gMusicPlayerFanfare;
extern struct SoundPlayer Data_02004360;
extern u16 gMusicPlayerVolumes[8];
extern u8 gMusicRestoreDelay;

extern u16 gMusicVolume;
extern u16 gMusicVolumeTarget;
extern u16 gMusicVolumeStep;
extern u16 gMusicPitch;
extern u16 gMusicPitchTarget;
extern u16 gMusicPitchStep;
extern u8 gAudioSecondaryState;
extern u8 Audio_CommandMask;

#endif
