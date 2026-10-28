#include "audio_engine.h"

void Sound_UpdateCommandMask(u32 command) {
  u32 toggle = command & AUDIO_COMMAND_TOGGLE;

  command &= AUDIO_COMMAND_MASK;
  if (toggle != 0)
    gAudio_CommandMask ^= command;
  else
    gAudio_CommandMask = command;
}
