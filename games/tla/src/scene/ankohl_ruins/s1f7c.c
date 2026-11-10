#include "ankohl_ruins.h"


/// Each frame, moves the raft (object 9) down by 0xcccc and carries the leader
/// and the event work's unk1cc object with it while the leader stands on it;
/// copies the cells it passes in rows 31 to 37 (flag 0x146), and past row 41
/// removes itself, stops the shaking, copies the landing cells and sets flag
/// 0x201.
void Func_02001f7c(void) {
  struct Actor *rider =
      ((struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork)->unk1cc;
  struct Actor *raft = Actor_GetById(9);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;
  s32 column;
  s32 row;

  if (GameFlag_Test(0x104) && work->unk154 != 40)
    return;
  raft->z += 0xcccc;
  column = leader->x >> 20;
  if (gMainRoutineWork.renderMode != 5 && column > 12 && column <= 15) {
    /* FAKEMATCH(temporary): a second pointer to the leader loads the raft's
     * z before the leader's in the second test; as leader, 16 of 167
     * instructions differ in tla-en. */
    struct Actor *player = leader;

    if (raft->z + 0x180000 >= player->z && player->z >= raft->z) {
      player->z = raft->z + 0x180000;
      rider->z = raft->z + 0x180000;
    }
    if (raft->z >= player->z && player->z >= raft->z - 0x100000) {
      player->z = raft->z - 0x100000;
      rider->z = raft->z - 0x100000;
    }
  }
  row = raft->z >> 20;
  if ((u16)(row - 31) <= 4 && !AnkohlRuins_FlagCheck(0x146)) {
    AnkohlRuins_Call2dfc(35, 83, 5, 4, 12, 96);
    Event_SetFlag(0x146);
  }
  if (row == 37 && AnkohlRuins_FlagCheck(0x146)) {
    AnkohlRuins_Call2dfc(40, 83, 5, 4, 12, 96);
    Event_ClearFlag(0x146);
  }
  if (row > 41) {
    FieldTask_Remove(Func_02001f7c);
    AnkohlRuins_Call2e24(-1, -1, 0xe666);
    AnkohlRuins_Call2dfc(41, 48, 3, 2, 13, 41);
    Event_SetFlag(0x201);
    Event_ClearFlag(0x17d);
    Event_ClearFlag(0x161);
    Event_PlaySound(208);
  }
}
