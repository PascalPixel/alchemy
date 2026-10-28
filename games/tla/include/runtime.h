#ifndef TLA_RUNTIME_H
#define TLA_RUNTIME_H

/// The cartridge's language, from its header (System_GetLanguage).
enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH = 1,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_ITALIAN,
  CARTRIDGE_LANGUAGE_SPANISH
};

int System_ReturnZero(void);

#endif
