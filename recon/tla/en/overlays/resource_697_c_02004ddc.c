// Draft, differ 476 of 442: the Trial Road rival's per-frame update, by the scene
// state's mode: 4 follows the leader's position mirrored across the course;
// 3 places the rival and its partner from the linked player's record
// (gLinkPeerSignatures by flag 760's byte, when gLinkStatus has that player),
// walking it to the link point when flag 547 says so; 1 replays the recorded
// path (from the resource the state names), turning toward each point at most
// 1/16 turn a frame, until the path's end (mode 9); 2 records the leader's path
// two words at a time and, at its end (point 7198), closes it and returns to
// mode 1. The rival walks (mode 5) while it moves, stands (mode 1) once it has
// been still for 3 frames.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

struct TrialRoadReplay {
  s16 mode;
  s16 mirrored;
  s16 id;
  s16 point;
  s16 still;
  s16 resource;
};

struct LinkSignature {
  s16 x;
  s16 z;
  s16 angle;
  s16 state;
  s16 partner;
  s16 partnerX;
  s16 partnerZ;
  s16 partnerAngle;
  s16 partnerState;
  u8 unk12[6];
};

struct TrialRoadLinkWork {
  u8 unk00[0xe4];
  u16 rival;
  u16 unke6;
  u16 linkAngle;
  u16 resource;
  s32 linkX;
  s32 linkZ;
  s16 path[0x1c20];
};

extern struct TrialRoadReplay gSceneState;
extern struct LinkSignature gLinkPeerSignatures[];
extern u16 gLinkStatus;

s32 ArcTan2(s32 dz, s32 dx);
s32 GameFlag_GetByte(s32 flag);
void *Resource_GetTableEntry(s32 id);
void Func_02005770(const void *source, void *destination);
void Func_020057f8(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_020057f0(struct ObjectRuntime *object);

static __inline__ void TurnToward(struct ObjectRuntime *rival, s32 x, s32 z) {
  s32 turn;
  u16 angle;

  turn = (s16)((u16)ArcTan2(z - rival->z, x - rival->x) - rival->angle);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  angle = rival->angle;
  rival->angle = angle + turn;
  rival->x = x;
  rival->z = z;
}

void Func_02004ddc(void) {
  struct TrialRoadReplay *state = &gSceneState;
  struct TrialRoadLinkWork *work =
      *(struct TrialRoadLinkWork **)((u8 *)Ram_HeapSlots + 236);
  struct ObjectRuntime *rival = Object_GetById(state->id);
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct LinkSignature *signature;
  s32 x;
  s32 z;
  s32 angle;
  s32 mode;
  s32 player;
  s32 ok;

  if (rival == 0)
    return;
  if (state->mode == 4) {
    x = leader->x;
    z = leader->z;
    if ((u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - (x & 0xffff0000);
    if (gAdventure.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x == x && rival->z == z)
      goto walk;
    TurnToward(rival, x, z);
    goto walk;
  }
  if (state->mode == 3) {
    player = GameFlag_GetByte(760) ^ 1;
    signature = &gLinkPeerSignatures[player];
    if (((gLinkStatus >> player) & 1) == 0)
      return;
    x = signature->x;
    z = signature->z;
    angle = signature->angle;
    mode = (s16)signature->state >> 8;
    ok = signature->state & 0xff;
    if (GameFlag_Test(529) && mode != 0) {
      state->mode = 9;
      Func_020057c8(rival, 1);
      x = work->linkX + 0x60000;
      if (work->linkX < rival->x)
        x = work->linkX - 0xc0000;
      if (!GameFlag_Test(530)) {
        z = work->linkZ + 0x100000;
        rival->action = work->linkAngle;
      } else {
        z = work->linkZ - 0x100000;
        rival->action = work->rival;
      }
      rival->acceleration = 0x4000;
      rival->speedLimit = 0x10000;
      Func_020057f8(rival, x, 0, z);
      Func_020057f0(rival);
    } else {
      rival->angle = angle;
      rival->x = x << 16;
      rival->z = z << 16;
      Func_020057c8(rival, ok);
    }
    if (signature->partner == 0)
      return;
    rival = Object_GetById(signature->partner);
    if (rival == 0)
      return;
    rival->x = signature->partnerX << 16;
    rival->angle = signature->partnerAngle;
    rival->z = signature->partnerZ << 16;
    Func_020057c8(rival, signature->partnerState);
    return;
  }
  if (state->mode == 1) {
    if (state->resource != (s16)work->resource) {
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
    if (gAdventure.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x == x && rival->z == z) {
      state->still++;
    } else {
      TurnToward(rival, x, z);
      state->still = 0;
    }
    goto walk;
  }
  if (state->mode == 2) {
    work->path[state->point] = ((s16 *)&rival->x)[1];
    work->path[state->point + 1] = ((s16 *)&rival->z)[1];
    if (!GameFlag_Test(260))
      state->point += 2;
    if (state->point == 7198) {
      work->path[state->point] = 0;
      work->path[state->point + 1] = 0;
      state->id = work->rival;
      state->point = 0;
      state->mode = 1;
    }
  }
  return;
walk:
  if (state->still > 2)
    Func_020057c8(rival, 1);
  else
    Func_020057c8(rival, 5);
}
