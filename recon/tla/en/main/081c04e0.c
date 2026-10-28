// Draft, differ 48 of 28: the work copy of the sound initialization, as ☀️'s
// m4aSoundInit (games/tbs/src/sound/audio.c). The listing starts at
// the copy's second call: its first ten instructions (from 081c04cc) sit in
// the pool words of 081c04ac's listing, so the draft compiles the whole
// function and the comparison covers only the listing's part. A
// sound-library function: the draft compiles with the game profile, where the
// build routes the library through old_agbcc. Other: register choice
// (r5-r7 and r3 against the listing's r4-r6 and r0), the count's
// narrowing and the game compiler's rotated loop.
#include "types.h"
#include "sound_driver.h"
#include "ram_buffer.h"

extern char Sound_PlayerCount[];
#define SOUND_PLAYER_COUNT ((u16)(u32)Sound_PlayerCount)

void Sound_Mixer(void);
void SoundInit(struct SoundWork *work);
void MPlayExtender(struct CgbNote *notes);
void m4aSoundMode(u32 mode);
void MPlayOpen(struct SoundPlayer *player,
                            struct SoundTrack *tracks, u8 count);
extern struct SoundWork gSound_Work;
extern struct CgbNote gSound_CgbNotes[4];
extern u8 gSound_WorkBytes[];
extern const struct PlayerSlot gSound_PlayerSlots[];

void Func_081c04e0(void) {
  s32 playerIndex;

  Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), Ram_SoundMixer, 0x04000100);
  SoundInit(&gSound_Work);
  MPlayExtender(gSound_CgbNotes);
  m4aSoundMode(AUDIO_INITIAL_MODE);
  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++) {
    struct SoundPlayer *player = gSound_PlayerSlots[playerIndex].player;

    MPlayOpen(player, gSound_PlayerSlots[playerIndex].tracks,
                           gSound_PlayerSlots[playerIndex].trackCount);
    player->checkPriority = gSound_PlayerSlots[playerIndex].checkPriority;
    player->workBytes = gSound_WorkBytes;
  }
}
