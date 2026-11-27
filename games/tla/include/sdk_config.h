#ifndef ALCHEMY_SDK_CONFIG_H
#define ALCHEMY_SDK_CONFIG_H

/* What The Lost Age sets for the development kit's code in games/common/sdk:
   where the sound driver's jump table and mixer sit in RAM, and the sound mode
   the driver starts in. The kit's C takes each as a number: written as a linked
   name, old_agbcc builds other code. */

#include "types.h"

/* The sound driver's jump table, MPlayJumpTable, at its fixed address: the
   driver's core (driver.c) reaches it by this number, MPlayExtender by its
   name. */
#define Ram_MPlayJumpTable \
	FIXED_ADDRESS((MPlayFunc *), 0x02006800, MPlayJumpTable)

/* The sound driver's mixer, SoundMainRAM, 932 bytes, where the linker script
   places .iwram_mixer. m4aSoundInit copies SOUND_MIXER_CODE_SIZE (1 KB) there
   at once, so the 92 bytes after the mixer in the ROM land just after it. */
#define Ram_SoundMixer FIXED_ADDRESS((Uint8 *), 0x03006000, SoundMainRAM)

/* The sound mode m4aSoundInit sets for the whole game: 10 PCM notes (bits
   11..8), volume 15 (15..12), rate 9 (19..16), 528 samples a VBlank or 31536 a
   second, and sound bias resolution 1 (from bits 23..20). */
#define SOUND_GAME_MODE 0x0099FA00

#endif
