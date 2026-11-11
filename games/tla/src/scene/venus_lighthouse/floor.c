#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"

struct VenusActor {
  u8 unk00[6];
  u16 facing;
  s16 xLow;
  s16 xPixel;
  s32 y;
  s16 zLow;
  s16 zPixel;
  s32 terrainHeight;
  u8 unk18[0xa];
  u8 terrainId;
  u8 unk23;
  s32 velocityX;
  s32 speed;
  s32 velocityZ;
  s32 moveRateX;
  s32 moveRateZ;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unk44[0x11];
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 selectFlags;
  u8 unk5b[7];
  u8 busy;
  u8 unk63[9];
  void *update;
};

extern const u32 gData_02001d8c[];
extern const u8 gData_02001dd8[];
extern u8 gMapCellBuffer[];

struct VenusActor *Actor_GetById(s32 id);
s32 Party_GetLeaderActor(void);
void Import_EventStart(void);
void Import_EventEnd(void);
void Event_ResolvePendingActions(s32 skip);
void Event_Wait(s32 frames);
void Sound_Play(s32 cue);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                          s32 destY, s32 width, s32 height);
void wait(s32 frames);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Actor_RefreshSelectorById(s32 actor);
void Actor_SetPartAttribute(struct VenusActor *object, s32 value);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Func_02001b2c(struct VenusActor *object, s32 mode);
void Actor_SetMoveTarget(struct VenusActor *object, s32 x, s32 y, s32 z);
void Script_WaitForEventTimeout(struct VenusActor *object);
s32 Func_02001b64(struct VenusActor *object, s32 *position);
void Func_080d47b4(s32 actor, s32 height, s32 frames);
void Func_02001cbc(void);
s32 Func_020005a8(struct VenusActor *object);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline s32 Venus_FlagCheck(s32 flag) { return GameFlag_Test(flag); }

static inline void Venus_CopyCells(s32 sx, s32 sy, s32 dx, s32 dy, s32 w,
                                   s32 h) {
  Map_CopyCellPictureIndicesRect(sx, sy, dx, dy, w, h);
}

void Func_020006ac(void) {
  s32 leader = Party_GetLeaderActor();
  struct VenusActor *actor = Actor_GetById(leader);
  s32 z;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  Actor_SetMovementTypeById(leader, gData_02001dd8);
  Actor_RefreshSelectorById(leader);
  Actor_SetPartAttribute(Actor_GetById(leader), 6);
  {
    /* FAKEMATCH(pin): the leader pinned to r0, so it is copied there before the
     * acceleration is built, as the native does; as
     * Actor_SetSpeedParameters(leader, 0x40000, 0x20000). */
    register s32 who __asm__("r0");

    actor->speed = 0x40000;
    who = leader;
    Actor_SetSpeedParameters(who, 0x40000, 0x20000);
  }
  if (*(s32 *)&actor->zLow >> 20 <= 54) {
    Actor_GetById(leader)->selectFlags &= ~1;
    z = 210;
  } else {
    Actor_GetById(leader)->selectFlags &= ~1;
    z = 238;
  }
  Actor_SetPositionAndReset(leader, actor->xPixel, z * 4);
  Event_Wait(1);
  Actor_GetById(leader)->selectFlags |= 1;
  Event_Wait(20);
  Actor_SetPartAttribute(actor, 0);
  actor->update = Func_020005a8;
  Func_080d47b4(leader, 258, 60);
  Motion_SetModeAndWaitAnimation(leader, 4);
  Actor_SetPartAttribute(Actor_GetById(leader), 0);
  Motion_SetModeAndWaitAnimation(leader, 4);

  /* FAKEMATCH(dead): an empty once-only block, so the null callback is built
   * after the wait; as no block, 46 of 85 instructions differ in tla-en. */
  do {
  } while (0);

  actor->update = 0;
  Import_EventEnd();
}

void Func_0200078c(void) {
  struct VenusActor *leader = Actor_GetById(Party_GetLeaderActor());
  s32 z = leader->zPixel;

  if ((u32)(leader->xPixel - 676) > 7 || z < 788 || z >= 796) {
    Map_CopyCellPictureIndicesRect(53, 50, 42, 49, 1, 1);
    Venus_CopyCells(55, 117, 41, 117, 3, 5);
    GameFlag_ClearBit(0x201);
    leader->motionFlags |= 1;
    leader->terrainHeight = 0;
    leader->y = 0;
  } else if (!Venus_FlagCheck(0x201)) {
    Import_EventStart();
    Event_ResolvePendingActions(0);
    Event_Wait(5);
    Map_CopyCellPictureIndicesRect(52, 50, 42, 49, 1, 1);
    Venus_CopyCells(52, 117, 41, 117, 3, 5);
    GameFlag_SetBit(0x201);
    Sound_Play(161);
    leader->motionFlags &= ~1;
    leader->terrainHeight = -0x20000;
    leader->y = -0x20000;
    Import_EventEnd();
  }
}
