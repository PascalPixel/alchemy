#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "dma.h"
#include "io_reg.h"
#include "adventure.h"

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void Func_0814ea58(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
void wait(s32 frames);
s32 Func_08020310(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Func_08108148(void);
s32 Inn_RoomPrice(s32 innId);
void Func_0810824c(void);
void *Func_080ccd78(s32 group, s32 id);

s32 Func_080cce84(void) { return ((s32 (*)(void))gOverlayArea[1])(); }

/// Finds the talk event of object id; when the party has stopped that object,
/// an event of kind 7 comes first.
void *Func_080cce94(s32 id) {
  /* FAKEMATCH: the search's result waits in r0 and is copied after the
     stopped object's load; held in a plain local it is copied right after
     the call (differ 2) */
  register void *found asm("r0") = Func_080ccd78(0, id);
  struct Adventure *party = &gAdventure;
  void *event;
  s32 stopped;

  /* FAKEMATCH: the barrier loads the adventure's address before the
     field offset is built (differ 2 without it) */
  asm volatile("");
  stopped = party->pausedObject;
  event = found;
  if (stopped == id) {
    void *alt = Func_080ccd78(7, stopped);

    if (alt != NULL)
      return alt;
  }
  return event;
}
