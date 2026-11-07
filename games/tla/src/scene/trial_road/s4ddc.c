/// @file
/// The Trial Road rival's per-frame update.

#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

struct TrialRoadReplay {
  s16 mode;
  s16 mirrored;
  s16 id;
  s16 point;
  s16 still;
  s16 resource;
};

struct TrialRoadLinkWork {
  u8 unk00[0xe4];
  u16 rival;
  u16 unke6;
  u16 linkAngle;
  s16 resource;
  s32 linkX;
  s32 linkZ;
  s16 path[0x1c20];
};

extern struct TrialRoadReplay gSceneState;
extern u8 gLinkPeerSignatures[][24];
extern volatile u16 gLinkStatus;

s32 ArcTan2(s32 dz, s32 dx);
s32 GameFlag_GetByte(s32 flag);
void *Resource_GetTableEntry(s32 id);
void Func_02005770(const void *source, void *destination);
void Func_020057f8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020057f0(struct Actor *object);

/// Updates the Trial Road rival by the scene state's mode: 4 follows the
/// leader's position mirrored across the course; 3 places the rival and its
/// partner from the linked player's record; 1 replays the recorded path; 2
/// records the actor's position.
void Func_02004ddc(void) {
  struct TrialRoadLinkWork *work =
      *(struct TrialRoadLinkWork **)((u8 *)Ram_HeapSlots + 236);
  struct TrialRoadReplay *state = &gSceneState;
  struct Actor *rival = Actor_GetById(state->id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x, z, turn, dx, dz;
  u16 heading;

  if (rival == 0)
    return;
  if (state->mode == 4) {
    x = leader->x;
    z = leader->z;
    if ((u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - (x & 0xffff0000);
    if (gMainRoutineWork.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x != x || rival->z != z) {
      dx = x - rival->x;
      dz = z - rival->z;
      heading = ArcTan2(dz, dx);
      turn = (s16)(heading - rival->angle);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      rival->angle += turn;
      rival->x = x;
      rival->z = z;
    }
    if (state->still > 2)
      Func_020057c8(rival, 1);
    else
      Func_020057c8(rival, 5);
  } else if (state->mode == 3) {
    s32 player;
    s16 *signature;
    s32 angle, link, ok, partner;
    s16 st;

    player = GameFlag_GetByte(760) ^ 1;
    signature = (s16 *)gLinkPeerSignatures[player];
    if (((gLinkStatus >> player) & 1) == 0)
      return;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    st = *signature++;
    link = st >> 8;
    ok = st & 0xff;
    if (GameFlag_Test(dz = 529) && link != 0) {
      state->mode = 9;
      Func_020057c8(rival, 1);
      if (work->linkX < rival->x)
        x = work->linkX + 0xc0000;
      else
        x = work->linkX - 0xc0000;
      if (!GameFlag_Test(530)) {
        z = work->linkZ + 0x100000;
        rival->action = work->linkAngle;
      } else {
        z = work->linkZ - 0x100000;
        rival->action = work->unke6;
      }
      rival->acceleration = 0x4000;
      rival->speedLimit = 0x10000;
      Func_020057f8(rival, x, 0, z);
      Func_020057f0(rival);
    } else {
      x <<= 16;
      z <<= 16;
      rival->angle = angle;
      rival->x = x;
      rival->z = z;
      Func_020057c8(rival, ok);
    }
    partner = *signature++;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    ok = *signature++;
    if (partner == 0)
      return;
    rival = Actor_GetById(partner);
    if (rival == 0)
      return;
    rival->x = x << 16;
    rival->angle = angle;
    rival->z = z << 16;
    Func_020057c8(rival, ok);
  } else if (state->mode == 1) {
    if (state->resource != work->resource) {
      Func_02005770(Resource_GetTableEntry(state->resource), work->path);
      work->resource = state->resource;
    }
    x = work->path[state->point];
    z = work->path[state->point + 1];
    if (!GameFlag_Test(260))
      state->point += 2;
    if (x == 0 && z == 0) {
      state->mode = 9;
      return;
    }
    x <<= 16;
    z <<= 16;
    if (state->mirrored != 0 && (u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - x;
    if (gMainRoutineWork.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x != x || rival->z != z) {
      dx = x - rival->x;
      dz = z - rival->z;
      heading = ArcTan2(dz, dx);
      turn = (s16)(heading - rival->angle);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      rival->angle += turn;
      rival->x = x;
      rival->z = z;
      state->still = 0;
    } else {
      state->still++;
    }
    if (state->still > 2)
      Func_020057c8(rival, 1);
    else
      Func_020057c8(rival, 5);
  } else if (state->mode == 2) {
    x = rival->x >> 16;
    z = rival->z >> 16;
    work->path[state->point] = x;
    work->path[state->point + 1] = z;
    if (!GameFlag_Test(260))
      state->point += 2;
    if (state->point == 7198) {
      // A u16 read back as s16 at its use: the same value, the spelling that
      // matches.
      u16 next = state->point + 1;

      work->path[state->point] = 0;
      work->path[(s16)next] = 0;
      state->id = work->rival;
      state->point = 0;
      state->mode = 1;
    }
  }
}
