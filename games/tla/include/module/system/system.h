#ifndef ALCHEMY_MODULE_SYSTEM_SYSTEM_H
#define ALCHEMY_MODULE_SYSTEM_SYSTEM_H

#include "types.h"

/// The cartridge's language, from its header (System_GetLanguage).
enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH = 1,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_ITALIAN,
  CARTRIDGE_LANGUAGE_SPANISH
};

s32 System_ReturnZero(s32 unused0, const u8 *unused1);

#endif
