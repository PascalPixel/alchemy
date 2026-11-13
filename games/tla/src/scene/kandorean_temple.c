/// @file
/// The Kandorean Temple of Master Poi, south-west of Daila, in nine rooms that
/// share this overlay (SceneId_Ov653Room1 to SceneId_Ov653Room9).
///
/// Its gate is shut for the meditation (KandoreanTemple_StartGate): Kraden
/// spots a suspicious wall and the party finds a hidden way in through a cave
/// (KandoreanTemple_FindTheHiddenPassage). Inside, Poi tests three students
/// (KandoreanTemple_TestTheStudents), and the party follows the one who can
/// levitate through the temple's trials: a hot floor, steam that pushes the
/// leader along, two lifts, two air vents whose platforms a pillar holds down
/// (KandoreanTemple_RunLeftVent, KandoreanTemple_RunRightVent), and three
/// Chestbeaters (KandoreanTemple_FightChestbeaters). Poi then gives the party
/// the Lash Pebble (KandoreanTemple_ReceiveLash), and Lash reaches Fog, the
/// Mercury Djinni (KandoreanTemple_FightFog).
///
/// This file is the overlay's own copy of the effect spawner
/// (FieldEffect_Spawn); the rest of the temple is in kandorean_temple/.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_LifeScripts
#include "../field/field_effect/spawn.c"
