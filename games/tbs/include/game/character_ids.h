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

/// The childhood portraits' numbers.
#define FACE_ISAAC_CHILD 56
#define FACE_GARET_CHILD 57

#endif
