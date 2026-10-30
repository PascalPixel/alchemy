#ifndef ALCHEMY_CHARACTER_IDS_H
#define ALCHEMY_CHARACTER_IDS_H

/// @file
/// Character numbers, with each character's Japanese name.

/// The characters, one for each row of gCharacter_DefinitionTable
/// (character_table.c) in order; unit ids below ENEMY_ID_FIRST are these.
enum CharacterId {
  ISAAC, ///< Robin
  GARET, ///< Jerald
  IVAN,  ///< Ivan
  MIA,   ///< Mary
  FELIX, ///< Garcia
  JENNA, ///< Jasmine
  SHEBA, ///< Sheba
  PIERS  ///< Picard
};

#define BATTLE_CHARACTER(no) (300 + (no)) ///< the character's battle sprite

/// The childhood sprites, shown while FLAG_CHILDHOOD is set; the party
/// party members' other sprites mostly share their character numbers.
#define ISAAC_CHILD 18
#define GARET_CHILD 19
#define FELIX_CHILD 36
#define JENNA_CHILD 48
/// Isaac's grown sprite while FLAG_ISAAC_WEARS_TUNIC is set.
#define ISAAC_TUNIC 17

/// The sprite of each element's Djinni (gDjinn_ElementSprites).
#define VENUS_DJINNI 90
#define MERCURY_DJINNI 91
#define MARS_DJINNI 92
#define JUPITER_DJINNI 93

/// The childhood portraits' numbers.
#define FACE_ISAAC_CHILD 56
#define FACE_GARET_CHILD 57

#endif
