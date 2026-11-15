#ifndef ALCHEMY_MODULE_FIELD_FIELD_SEQUENCE_H
#define ALCHEMY_MODULE_FIELD_FIELD_SEQUENCE_H

/// @file
/// What the field_sequence object defines for the code outside it.

#include "types.h"
#include "field/encounter_zone.h"
#include "module/field/field.h"
#include "scene_tables.h"

struct Actor;

/// A rule choosing a scene's field music, ended by id -1: a scene or an area
/// (byScene), an entrance (-1 for any), a flag that must be set (-1 for none)
/// and the song.
struct SceneCueRule {
  s16 id;
  s16 entrance : 15;
  u16 byScene : 1;
  s16 flag;
  s16 cue;
};

/// A rule naming the area a scene is in, ended by id -1: a scene or an area
/// (useSceneId), an entrance (-1 for any) and the area name's index.
struct SceneAreaNameRule {
  s16 id;
  s16 entrance : 15;
  u16 useSceneId : 1;
  s32 areaNameIndex;
};

/// A row of the scene's trigger table as the Psynergy trigger search reads
/// it: its kind and flags (-1 ends the table), what it answers, its
/// condition and its value.
struct TriggerEvent {
  s32 control;
  s16 trigger;
  s16 condition;
  u32 value;
};

/// A row of a scene's actor placement table as the field reads it: its
/// sprite, condition, movement type, position, facing, the facing it takes
/// to talk and its flags.
struct ActionDescriptor {
  u16 sprite;
  s16 condition;
  s32 behavior;
  s32 x;
  s32 y;
  s32 z;
  u16 facing;
  u8 talkFacing;
  u8 flags;
};

s32 WorldPicture_Show(s32 mode);
void WorldMap_ZoomOutWhileLHeld(void);
void BattleBackground_SelectForEnvironment(s32 terrain);
s32 GameFlag_IsConditionActive(s32 condition);
void *Scene_FindActorTalkTrigger(s32 id);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Field_UseItem(s32 arg, s32 slot);
void Actor_ReleaseIfPresent(struct Actor *object);
void ItemDisplay_RaiseAboveLeader(struct Actor *actor);
void FieldPsynergy_Lift(void);
s32 Speaker_GetVoice(s32 id);

#endif
