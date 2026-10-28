#include "types.h"
#include "adventure.h"
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

struct VenusActor *Import_ActorGet(s32 id);
s32 Import_PartyGetLeaderActor(void);
void Import_EventStart(void);
void Import_EventEnd(void);
void Import_EventResolvePendingActions(s32 skip);
void Import_EventWaitFrames(s32 frames);
void Import_SoundPlay(s32 cue);
void Import_GameFlagSet(s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Import_MapCopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                           s32 height);
void WaitFrames(s32 frames);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_RefreshSelectorById(s32 actor);
void Object_SetPartAttribute(struct VenusActor *object, s32 value);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Func_02001b2c(struct VenusActor *object, s32 mode);
void Func_02001b44(struct VenusActor *object, s32 x, s32 y, s32 z);
void Func_02001b4c(struct VenusActor *object);
s32 Func_02001b64(struct VenusActor *object, s32 *position);
void Func_02001c34(s32 actor, s32 height, s32 frames);
void Func_02001cbc(void);
s32 Func_020005a8(struct VenusActor *object);

static inline s32 Venus_FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void Venus_FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Import_GameFlagSet(flag);
}

static inline void Venus_CopyCells(s32 sx, s32 sy, s32 dx, s32 dy, s32 w,
                                   s32 h) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Import_MapCopyCellPictureIndicesRect(sx, sy, dx, dy, w, h);
}

void Func_020006ac(void) {
  s32 leader = Import_PartyGetLeaderActor();
  struct VenusActor *actor = Import_ActorGet(leader);
  s32 z;

  Import_EventStart();
  Import_EventResolvePendingActions(0);
  ObjectMotion_EnableActionAndSetCallback(leader, gData_02001dd8);
  Object_RefreshSelectorById(leader);
  Object_SetPartAttribute(Import_ActorGet(leader), 6);
  {
    /* FAKEMATCH: the native copies the leader into r0 before it builds the
     * acceleration. */
    register s32 who __asm__("r0");

    actor->speed = 0x40000;
    who = leader;
    ObjectMotion_SetSpeedParameters(who, 0x40000, 0x20000);
  }
  if (*(s32 *)&actor->zLow >> 20 <= 54) {
    Import_ActorGet(leader)->selectFlags &= ~1;
    z = 210;
  } else {
    Import_ActorGet(leader)->selectFlags &= ~1;
    z = 238;
  }
  ObjectMotion_SetPositionAndReset(leader, actor->xPixel, z * 4);
  Import_EventWaitFrames(1);
  Import_ActorGet(leader)->selectFlags |= 1;
  Import_EventWaitFrames(20);
  Object_SetPartAttribute(actor, 0);
  actor->update = Func_020005a8;
  Func_02001c34(leader, 258, 60);
  Motion_SetModeAndWaitAnimation(leader, 4);
  Object_SetPartAttribute(Import_ActorGet(leader), 0);
  Motion_SetModeAndWaitAnimation(leader, 4);
  {
    void *none;

    /* FAKEMATCH: the one-pass block builds the null callback after the wait;
     * as a plain initializer, 46 of 85 instructions differ in tla-en. */
    do {
      none = 0;
    } while (0);

    actor->update = none;
  }
  Import_EventEnd();
}

void Func_0200078c(void) {
  struct VenusActor *leader = Import_ActorGet(Import_PartyGetLeaderActor());
  s32 x = leader->xPixel;
  s32 z = leader->zPixel;

  if ((u32)(x - 676) > 7 || z < 788 || z >= 796) {
    Venus_CopyCells(53, 50, 42, 49, 1, 1);
    Venus_CopyCells(55, 117, 41, 117, 3, 5);
    GameFlag_ClearBit(0x201);
    leader->motionFlags |= 1;
    leader->terrainHeight = 0;
    leader->y = 0;
  } else if (!Venus_FlagCheck(0x201)) {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventWaitFrames(5);
    Venus_CopyCells(52, 50, 42, 49, 1, 1);
    Venus_CopyCells(52, 117, 41, 117, 3, 5);
    Venus_FlagSet(0x201);
    Import_SoundPlay(161);
    leader->motionFlags &= ~1;
    leader->terrainHeight = -0x20000;
    leader->y = -0x20000;
    Import_EventEnd();
  }
}
