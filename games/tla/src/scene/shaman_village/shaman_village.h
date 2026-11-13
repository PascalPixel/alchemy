#ifndef TLA_SHAMAN_VILLAGE_H
#define TLA_SHAMAN_VILLAGE_H

#include "types.h"
#include "system.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "frame.h"
#include "scene_tables.h"

struct ShamanVillageObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x18];
  s32 field44;
  s32 field48;
  u8 unk4c[4];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 actionFlags;
  u8 unk5b;
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

struct ShamanVillageEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[0x44];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

void Event_End(void);
void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);
void Import_ActorSetMode(struct ShamanVillageObject *object, s32 mode);
void Actor_Release(struct ShamanVillageObject *object);
void Actor_SetSpriteFlags(struct ShamanVillageObject *object, u32 value);
void Actor_SetBlendMode(struct ShamanVillageObject *object, s32 flags);
void FieldPsynergy_RunHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldCamera_WaitForArrival(void);

void Func_02004a44(void);
void Func_02004a4c(s32 a, s32 b, s32 c, s32 d);
void Event_StartScreenTransition(void);

void Actor_MoveToObject(s32 actor, s32 target);

void BattleEffect_StartArcEffect(void);
void BattleEffect_EndArcEffect(void);
void BattleEffect_WaitForArcGroups(void);
void BattleEffect_SetObjectUpdateMode(struct ShamanVillageObject *object,
                                      s32 value);
void Scene_PlayMusic(void);
void InventorySnapshot_Save(void);

#define SHAMAN_VILLAGE_EMOTE(actor, emote, frames)                             \
  {                                                                            \
    /* FAKEMATCH(pin): the emote pinned to r1, as the native */                \
    /* loads the emote and the frames before the actor at every */             \
    /* emote of this overlay. Measured against it: an inline */                \
    /* call (the actor comes one row early wherever the emote */               \
    /* takes two instructions), an inline with its parameters in */            \
    /* another order or modified, a local (shared between calls), */           \
    /* a once-only block, and barriers that hold r0 (each empty */             \
    /* asm adds four bytes to the pool estimate and moves the */               \
    /* literal pool of a long scene). As an unpinned local, 111 */             \
    /* of 711 instructions differ in tla-en (ShamanVillage_ShowShamansRod). */ \
    register s32 emote_ __asm__("r1") = (emote);                               \
    s32 frames_ = (frames);                                                    \
    Actor_ShowEmoteById((actor), emote_, frames_);                             \
  }

// ---- this overlay

void ShamanVillage_BlowAwaySand(void);
void ShamanVillage_HopFortuneTeller(void);
void ShamanVillage_StartLettering(s32 a, s32 b);
void ShamanVillage_LoadLettering(s32 a);
void ShamanVillage_EnterDoor(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f);
s32 ShamanVillage_IsFacingCounter(void);
void ShamanVillage_ExplainTrialRules(void);
void ShamanVillage_StepBackFromTrial(void);
void ShamanVillage_CheckTrialEntry(void);
void ShamanVillage_CountDownToStart(void);

extern s32 gShamanVillage_EnterDoorScript[];
extern s32 gShamanVillage_HopFortuneTellerScript[];
extern struct SceneEntrance gShamanVillage_Entrances[];
extern struct SceneEntrance gShamanVillage_Room5Entrances[];
extern struct SceneEntrance gShamanVillage_Room6Entrances[];

#endif
