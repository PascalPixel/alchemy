#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"
#include "types.h"
#include "field_event.h"
#include "battle_action.h"

#define Actor_SetAnimationSpeedFar                                 \
  ((void (*)(s32, s32))Actor_SetAnimationSpeedFar)

s32 *BattleObject_Get(s32);

struct StatusIconRecord {
  u8 unk00[0x14];
  u8 width;
  u8 unk15[4];
  u8 dirty;
};

struct StatusIconEntry {
  u8 unk00[6];
  u8 priority;
};

s32 ResourceMetadata_RegisterFar(struct StatusIconRecord *record,
                                 s32 animation);
void ResourceMetadata_UnregisterFar(struct StatusIconRecord *record, s32 entry);
void Animation_SetWorkEntryFar(struct StatusIconEntry *entry, s32 index);

void *GetMotionRecord(struct MotionObject *object, s32 record_index);

#define ICON_MAX 14

/// Cycles the status icon over the slot's conditions every 80 frames, and
/// switches at once when the shown condition ends.
void BattleStatusIcon_Cycle(struct BattleObj *slot) {
  struct StatusIconEntry *icon;
  struct StatusIconRecord *rec;
  struct MotionObject *owner;
  s32 anim;
  s32 handle;
  s32 prev;
  s32 changed = 0;

  if (slot->cycleTimer >= 0)
    slot->cycleTimer--;

  if (((s16)slot->activeConditions != 0 && slot->animationEntry == 0) ||
      (slot->animationEntry != 0 &&
       (((s16)slot->activeConditions >> slot->selectedCondition) & 1) == 0) ||
      slot->cycleTimer == 0) {
    anim = -1;
    owner = slot->object;
    if ((s16)slot->activeConditions != 0) {
      prev = slot->selectedCondition;
      for (anim = prev + 1;; anim++) {
        if (anim > ICON_MAX - 1)
          anim = 0;
        if ((((s16)slot->activeConditions >> anim) & 1) != 0)
          break;
      }

      if (prev != anim || slot->animationEntry == 0) {
        /* FAKEMATCH: the one-pass block stores the condition before changed is
         * set; as plain statements, 2 instructions differ in tla-en. */
        do {
          slot->selectedCondition = anim;
        } while (0);
        changed = 1;
      }
      slot->cycleTimer = 80;
    } else {
      changed = 1;
    }

    rec = GetMotionRecord(owner, 0);
    if (rec != 0) {
      if (anim >= 0) {
        if (rec->width == 32)
          anim += 702;
        else
          anim += 688;
      }

      if (slot->animationEntry != 0 && changed != 0) {
        ResourceMetadata_UnregisterFar(rec, (s32)slot->animationEntry);
        slot->animationEntry = 0;
      }

      if (anim >= 0 && changed != 0) {
        handle = ResourceMetadata_RegisterFar(rec, anim);
        slot->animationEntry = (struct SpriteEntry *)handle;
        if (handle == -1)
          slot->animationEntry = 0;
        icon = (struct StatusIconEntry *)slot->animationEntry;
        if (icon != 0) {
          icon->priority = 3;
          Animation_SetWorkEntryFar(icon, 0);
        }
      }

      rec->dirty = 1;
      if (anim >= 0)
        slot->animation = anim;
      else
        slot->animation = 0;
    }
  }
}

s32 BattleObject_UpdatePose(s32 id) {
  struct BattleUnit *state;
  s32 value;

  state = Unit_Get(id);
  value = 1;
  if (state->hp != 0) {
    if (state->sleep != 0 || state->stun != 0 || state->immobile != 0)
      value = (state->side != 1) * 4;
  } else {
    s32 changed = state->side ^ value;
    value = (u32)(-changed | changed) >> 31;
    value = 5 - value;
  }

  Actor_SetMode(*BattleObject_Get(id), value);
  Actor_SetAnimationSpeedFar(*BattleObject_Get(id), (id & 3) + 14);
}
