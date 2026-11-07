// Not measurable by diff (2026-11-06): the listing, recon/tla/raw/081c04cc.s,
// is five .4byte words, so `alchemy-devtools diff` finds no instructions in
// it ("the selected reference function contains no instructions"). Decoded by
// hand they are nine instructions: the entry of the work copy of the sound
// initialization (☀️'s m4aSoundInit, games/tbs/src/sound/audio.c), up to the
// load of the second call's argument. The rest of the function is the listing
// at 081c04e0, drafted as recon/tla/en/main/081c04e0.c; this is the same C
// from the true entry. By hand: through the sound route (old_agbcc -O2
// -mthumb-interwork, SOUND_C) this C emits the five words exactly, its bl to
// Bios_CpuSet included as an unapplied f7ff fffe; through the game route 8 of
// the 9 differ (push {r5, r6, r7, lr} for {r4, r5, r6, lr}, r3 for r1 in the
// mask, and the pool offsets of a shorter function), and the whole function
// from here is the 9 head instructions plus 081c04e0's differ 48 of 28.
// Blocked as 081c04e0.c says: this copy is an older sound library kept in ROM,
// never moved or run; its calls to themselves and zero pool words are an
// object's unapplied relocations, which C reaches only through names at 0 or
// at older addresses, equates O2 forbids. It stays a draft.
#include "types.h"
#include "sound_driver.h"
#include "ram_buffer.h"

extern char Sound_PlayerCount[];
#define SOUND_PLAYER_COUNT ((u16)(u32)Sound_PlayerCount)

void Sound_Mixer(void);
void SoundInit(struct SoundWork *work);
void MPlayExtender(struct CgbNote *notes);
void m4aSoundMode(u32 mode);
void MPlayOpen(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count);
extern struct SoundWork gSound_Work;
extern struct CgbNote gSound_CgbNotes[4];
extern u8 gSound_WorkBytes[];
extern const struct PlayerSlot gSound_PlayerSlots[];

void Func_081c04cc(void) {
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
