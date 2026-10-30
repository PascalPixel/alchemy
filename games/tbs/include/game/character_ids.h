#ifndef CHARACTER_IDS_H
#define CHARACTER_IDS_H

/// @file
/// Character numbers, with each character's Japanese name.

#define ISAAC 0 ///< Robin
#define GARET 1 ///< Jerald
#define IVAN 2  ///< Ivan
#define MIA 3   ///< Mary
#define FELIX 4 ///< Garcia
#define JENNA 5 ///< Jasmine
#define SHEBA 6 ///< Sheba
#define PIERS 7 ///< Picard

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
