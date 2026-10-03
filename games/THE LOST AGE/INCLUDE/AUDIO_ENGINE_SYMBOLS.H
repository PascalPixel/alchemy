#ifndef TLA_AUDIO_ENGINE_SYMBOLS_H
#define TLA_AUDIO_ENGINE_SYMBOLS_H

#include "TYPES.H"

struct SoundPlayer;
struct AudioTrackSlotWork;
extern struct AudioTrackSlotWork *Flash_Handler3;
extern struct SoundPlayer gMusicPlayerBgm;

enum AudioCommandMask {
    AUDIO_COMMAND_TOGGLE = 0x80,
    AUDIO_COMMAND_MASK = 0x7f
};

#define AUDIO_INITIAL_MODE 0x0099FA00

extern u16 gMusicVolume;
extern u16 gMusicVolumeTarget;
extern u8 gMusicRestoreDelay;
extern u8 Audio_CommandMask;

#endif
