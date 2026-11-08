// Differ 48 of 28 (tla-en, 2026-11-08, -mtune=gba; unchanged, blocked as below): the game route, which cannot
// match; through the sound route all 28 of the listing's instructions come out.
// Blocked, 5 November 2026: the work copy of the sound initialization, as ☀️'s
// m4aSoundInit (games/tbs/src/sound/audio.c). The listing starts at the copy's
// second call: its head, from 081c04cc, is recon/tla/raw/081c04cc.s. Through the
// sound route (old_agbcc -O2 -mthumb-interwork, SOUND_C) this C emits the head's
// instructions and all of the listing's (rechecked 6 November 2026). The game
// route differs by 48 and has to: the listing keeps r4 live across a call, which
// -fcall-used-r4 forbids, and returns through bx r0, which needs interworking.
// What stops it is not the code: the first call is a bl to itself, the others reach
// addresses of an older layout, and the pool words but the block copy's
// control word are 0. C reaches those only by names at those addresses, which
// O2 forbids.
// ☀️'s route for code that runs from a RAM copy does not reach it (checked 6
// November 2026): ☀️ links such code into an output section run in IWRAM and
// stored in ROM (`> IWRAM AT>ROM` in games/tbs/main.ld), and the linker resolves
// every relocation. This copy is never moved or run: it is an older sound
// library kept in ROM, as player_work_copy.c and the leftover driver objects
// beside it (games/tla/src/sound), and its bl to itself (f7ff fffe) and zero
// pool words are an object's unapplied relocations. No load or run address from
// the object list's layout produces them; names at 0 or at those older targets
// would be equates O2 forbids. It stays a draft.
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
