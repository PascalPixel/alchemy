#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_ids.h"

struct ArcEntry {
  u8 unk00[5];
  u8 param;
};

struct ArcGroup {
  u8 unk00[25];
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct ArcEntry *entries[1];
};

struct ArcEffectObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x48];
  struct ArcGroup *records;
  u8 unk54[0x10];
  s16 phase;
  u16 step;
  u8 unk68[4];
  void (*callback)(struct ArcEffectObject *);
};

extern char MsgAbilityWoreOff;
extern char MsgItemWoreOff;

void Func_080e1024(struct ArcEffectObject *caster);
void Func_080cb82c(void);
void Func_080cb8a4(void);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 flags);

/// The caster during Avoid (BattleFx_RunEffect16): sheds a pair of falling
/// arcs every 5 frames (7 in scene Ov64a), and spins in phase 1.
// As ☀️'s function of the same name.
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *caster) {
  s16 phase;
  s32 elapsedFrames;
  struct PartyState *party;

  phase = caster->phase;
  /* FAKEMATCH(one-pass): the step's advance and the party state's address in
   * a one-pass block; as elapsedFrames = (s16)caster->step++ with
   * gPartyState.map, 4 of 43 instructions differ in tla-en. */
  do {
    elapsedFrames = (s16)caster->step++;
    party = &gPartyState;
  } while (0);

  if (party->map == (s32)&SceneId_Ov64a) {
    if (elapsedFrames % 7 == 0)
      Func_080e1024(caster);
  } else if (elapsedFrames % 5 == 0) {
    Func_080e1024(caster);
  }

  if (phase == 1)
    caster->angle += 0xc00;
}

void FieldEvent_ShowStatusMessage(void) {
  gPartyState.avoidDistance = 0;
  Func_080cb82c();
  if (gPartyState.avoidByItem == 0) {
    UiText_DrawQuantity(150, 4);
    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgAbilityWoreOff, 1);
  } else {
    UiText_DrawQuantity(236, 2);
    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgItemWoreOff, 1);
  }
  Func_080cb8a4();
}

struct ArcScene {
  u8 unk00[0x10];
  struct ArcEffectObject *mainObject;
  u8 unk14[0x6fc];
  s16 arcTileSlot;
};

extern const u8 gData_080ed90c[];
extern char MsgMonstersAttackLess;

s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 entry);
void VramCache_Load(s32 slot, s32 size, const u8 *tiles);
s32 GameFlag_Test(s32 flag);
void Animation_ApplyChildValuesFar(void *object, s32 value);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);

/// Psynergy effect 16: the user spins, rains arc particles, then flashes.
void BattleFx_RunEffect16(void) {
  struct ArcScene *scene;
  struct ArcEffectObject *object;
  struct ArcGroup *group;
  struct ArcEntry *entry;
  u32 angle;
  s32 flags;
  s32 slot;
  s32 count;
  s32 phase;
  s32 zero;

  scene = Ram_HeapSlots->effectWork;
  /* FAKEMATCH: forced temporary, as The Broken Seal's twin: the zero the two
   * counters take lives in r6 from here; the one-pass block keeps its set
   * before the object load (one reordered row without it). */
  zero = 0;
  do {
  } while (0);
  object = scene->mainObject;
  group = object->records;
  entry = group->entries[0];
  angle = object->angle;
  flags = group->flags;
  slot = Resource_FindFreeEntry();
  {
    /* FAKEMATCH: forced temporaries and a one-pass block reload the scene
     * before 0x710 and load the tiles between the slot's two shifts; without
     * them, one row each is reordered in tla-en. */
    struct ArcScene *work = scene;
    const u8 *tiles;
    s32 id;

    do {
    } while (0);
    work->arcTileSlot = slot;
    id = slot << 16;
    tiles = gData_080ed90c;
    VramCache_Load(id >> 16, 0x100, tiles);
    gPartyState.avoidDistance = 0x09600000;
    gPartyState.avoidByItem = GameFlag_Test(0x145);
    Animation_ApplyChildValuesFar(object, 0);
    object->callback = BattleFx_UpdatePairedArcSpawner;
    object->phase = zero;
    object->step = zero;
  }
  Audio_PlayCue(140);
  WaitFrames(15);
  object->phase = 1;
  WaitFrames(10);
  for (count = 0; count < 20; count++) {
    entry->param = 7;
    group->dirty = 1;
    WaitFrames(2);
    entry->param = 0;
    group->dirty = 1;
    group->flags = 1;
    WaitFrames(3);
  }
  object->callback = NULL;
  object->angle = angle;

  // For 70 frames, set and clear flags bit 2 on a five-frame cycle.
  for (count = 0; count < 70; count++) {
    phase = count % 5;
    if (phase == 0) {
      group->dirty = 1;
      group->flags = flags | 2;
    } else if (phase == 2) {
      group->dirty = 1;
      group->flags = flags;
      entry->param = 0;
    }
    if (count == 15)
      Audio_PlayCue(174);
    WaitFrames(1);
  }
  {
    /* FAKEMATCH: the flags restored through a pointer and one-pass blocks
     * around the wait keep the native's order of the r11 copy, the param
     * store and the scene reload; without them, one row each is reordered. */
    u8 *restore = &group->flags;

    *restore = flags;
  }
  group->dirty = 1;
  entry->param = 0;
  do {
  } while (0);
  WaitFrames(1);
  do {
  } while (0);
  Resource_ResetEntry(scene->arcTileSlot);
  UiText_ShowPositionedMessageAndWaitFar((s32)&MsgMonstersAttackLess, 1);
}
