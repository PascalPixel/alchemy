#include "runtime.h"
#include "types.h"
#include "input.h"
#include "edition.h"

void WaitTicks(s32 ticks);

void System_Halt(void) {
  volatile struct InputState *input = &gInput;

  for (;;) {
    input->pressed;
    WaitTicks(1);
  }
}

#if (LANGUAGE != LANGUAGE_JAPANESE) && (LANGUAGE != LANGUAGE_ENGLISH)
/// The last letter of the game code in the cartridge header (rom_head.s): D,
/// F, I or S in the European editions.
#define CARTRIDGE_LANGUAGE_CODE (*(const u8 *)0x080000af)

/// Returns the language of this cartridge from the last letter of the game
/// code in its header, as an enum CartridgeLanguage. Only the German,
/// Spanish, French and Italian editions have it.
s32 System_GetLanguage(void) {
  s32 language;

  switch (CARTRIDGE_LANGUAGE_CODE) {
  case 'D':
    language = CARTRIDGE_LANGUAGE_GERMAN;
    break;
  case 'F':
    language = CARTRIDGE_LANGUAGE_FRENCH;
    break;
  case 'I':
    language = CARTRIDGE_LANGUAGE_ITALIAN;
    break;
  case 'S':
    language = CARTRIDGE_LANGUAGE_SPANISH;
    break;
  default:
    language = CARTRIDGE_LANGUAGE_ENGLISH;
    break;
  }
  return language;
}
#endif

void Func_080132b8(void) {}

void Func_080132bc(void) {}

void Func_080132c0(void) {}

void Func_080132c4(void) {}

void Func_080132c8(void) {}

int Runtime_ConstantZeroResult(void) { return 0; }
