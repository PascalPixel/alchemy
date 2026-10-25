#include "types.h"
#include "ram_buffer.h"

struct FxGroupEntry {
  u8 unk00[5];
  u8 param;
  u8 unk06[16];
  u8 frame;
};

struct FxGroup {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct FxGroupEntry *entries[4];
};

struct FxSlot {
  void *object;
  u8 unk04[28];
  struct FxGroupEntry *animationEntry;
  struct FxGroupEntry *effectEntry;
  u8 unk28[2];
  u8 fading;
};

struct FxTimerWork {
  u8 unk0000[0x78dc];
  u8 actorTimers[8];
};

struct FxSlot *GetBattleObjectSlotFar(s32 id);
struct FxGroup *GetMotionRecordFar(void *object, s32 index);
s32 BattleMotion_GetSlotPaletteFar(s32 id);
void Animation_ApplyChildArgumentFar(void *group, s32 value);

static __inline__ u8 GroupCount(struct FxGroup *group) { return group->count; }

/// The Broken Seal's ObjectGroup_UpdateMembers as The Lost Age has it: the
/// member count is a byte at 0x1b, the timers sit at 0x78dc of the effect
/// work, and a member's frame is reset only when its parameter changes.
void Func_0814cd48(s32 setId, s32 objectValue, s32 groupValue, s32 stateSlot,
                   s32 stateValue) {
  struct FxSlot *set;
  struct FxGroup *group;
  struct FxTimerWork *state;
  s32 groupIndex;

  set = GetBattleObjectSlotFar(setId);
  state = (struct FxTimerWork *)Ram_HeapSlots->battleFxWork[0];
  groupIndex = 0;

  while ((group = GetMotionRecordFar(set->object, groupIndex)) != NULL) {
    if (stateSlot != -1)
      state->actorTimers[stateSlot] = ((u8 *)&stateValue)[0];
    if (set->fading == 0) {
      if (objectValue != -1) {
        s32 objectIndex;

        objectIndex = 0;
        if (GroupCount(group) != 0) {
          struct FxGroupEntry **objects;

          objects = group->entries;
          do {
            struct FxGroupEntry *object;

            object = *objects++;
            if (object != NULL && object != set->effectEntry &&
                object != set->animationEntry) {
              u8 old = object->param;

              if (objectValue == 0)
                object->param = BattleMotion_GetSlotPaletteFar(setId);
              else
                object->param = objectValue;
              if (object->param != old)
                object->frame = 0xff;
            }
            objectIndex++;
          } while (objectIndex != GroupCount(group));
        }
      }
      if (groupValue != -1)
        Animation_ApplyChildArgumentFar(group, groupValue);
    }
    groupIndex++;
  }
}
