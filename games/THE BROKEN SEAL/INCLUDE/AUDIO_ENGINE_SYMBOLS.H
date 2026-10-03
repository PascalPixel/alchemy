#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

#include "TYPES.H"

#define AUDIO_INITIAL_MODE 0x0097F800

void Bios_CpuSet(const void *source, void *destination, u32 control);

struct SoundPlayer;
struct AudioTrackSlotWork;
extern struct AudioTrackSlotWork *Data_02004c00;
extern struct SoundPlayer gMusicPlayerBgm;
extern struct SoundPlayer gMusicPlayerFanfare;
extern struct SoundPlayer Data_02004360;
extern u16 gMusicPlayerVolumes[8];
extern u8 gMusicRestoreDelay;

#endif
