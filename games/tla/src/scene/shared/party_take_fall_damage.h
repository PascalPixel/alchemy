/// The party takes 10 damage from a fall; who goes down is noted.
void Party_TakeFallDamage(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 leader = FieldParty_GetLeaderActor();
  s32 count;
  s32 i;
  s16 hp[8];

  (void *)Actor_Get(leader);
  count = Party_CountMembers();
  for (i = 0; i < count; i++)
    hp[i] = Unit_Get(gMainRoutineWork.activeOwners[i])->hp;
  Party_AddHp(-10, 0);
  work->fallenCount = 0;
  work->survivorCount = 0;
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  for (i = 0; i < count; i++) {
    struct MainRoutineWork *main = &gMainRoutineWork;
    struct BattleUnit *unit = Unit_Get(main->activeOwners[i]);

    if (unit->hp > 0) {
      work->survivorCount++;
    } else if (hp[i] != 0) {
      work->fallenOwners[(s16)work->fallenCount++] = main->activeOwners[i];
      work->raisedTrigger = 0xffff;
      ((u8 *)unit)[0x131] = 0;
    }
  }
}
