// Measured under -Os, 5 November 2026: differ 58 (tla-en); notes below may predate it.
// Near miss, differ 173 of 442 (tla-en, 2026-10-31): control flow, calls and
// data accesses are the native's; 66 of the differing lines are pool and branch
// offsets, shifted because the draft is 3 instructions longer (445).
// The Trial Road rival's per-frame update, by the scene state's mode: 4 follows
// the leader's position mirrored across the course; 3 places the rival and its
// partner from the linked player's record (gLinkPeerSignatures by flag 760's
// byte, when gLinkStatus has that player), walking it to the link point when
// flag 529 says so; 1 replays the recorded path (from the resource the state
// names), turning toward each point at most 1/16 turn a frame, until the path's
// end (mode 9); 2 records the actor's whole-pixel position two halfwords a
// frame and, at its end (point 7198), closes it with 0, 0, hands it to the
// actor at work+228 and returns to mode 1. The rival walks (mode 5) while it
// moves, stands (mode 1) once it has been still for 3 frames.
// What reached the native's allocation: x and z are one pair of variables for
// every mode, mode 2 included (their extra references put x above the rival and
// make z's caller-save profitable, so x is r6, the rival r7 and z r4 saved
// around calls); the walk test is written in both modes and cross-jumped; the
// mode 3 status is an s16 split into link and ok (link lands on the stack, the
// scene state in r11); the else branch shifts x and z in place; the heading is
// a u16 temporary; the terminator's second index is an s16 local.
// Remaining: GCSE PRE inserts the turn's rival->z load on the edge from the x
// test and the z test's branch jumps past it (native reloads it, in modes 4
// and 1); the scene mode stays in r4 where the native has r2; in mode 3 the
// native computes work+240 and rival+100 before the flag 530 branch (no form
// tried moves them), and reload picks other scratch registers (st r2 for r3,
// the state copy r1 for r0, the record offsets).
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

void Func_02004ddc(void) {
  struct TrialRoadLinkWork *work =
      *(struct TrialRoadLinkWork **)((u8 *)Ram_HeapSlots + 236);
  struct TrialRoadReplay *state = &gSceneState;
  struct Actor *rival = Actor_GetById(state->id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x, z, turn;
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
      heading = ArcTan2(z - rival->z, x - rival->x);
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
    if (GameFlag_Test(529) && link != 0) {
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
      heading = ArcTan2(z - rival->z, x - rival->x);
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
      s16 next = state->point + 1;

      work->path[state->point] = 0;
      work->path[next] = 0;
      state->id = work->rival;
      state->point = 0;
      state->mode = 1;
    }
  }
}
