#include "audio_engine.h"
#include "system.h"
#include "types.h"

void Sound_WaitForBgmRestore(void) {
  s32 wait_count = 0;

  do {
    if (gMusicRestoreDelay == 0)
      break;
    WaitFrames(1);
    wait_count++;
  } while (wait_count <= 299);
}

void m4aSoundVSyncOff(void);

void Sound_SuspendDirectSound(void) {
  m4aSoundVSyncOff();
}
