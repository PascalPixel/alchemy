/// Drops the leader through a cracked tile: it gives way and the leader spins
/// down into it while the screen fades.
void CrackedTile_Fall(struct CellTarget__s41ac *table,
                      struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = (leader->x >> 20) | (s16)(table->x & 64);
  s32 z = (leader->z >> 20) | (s16)(table->z & 64);

  Leader_CenterInCell();
  Event_PlaySound(SE_SPLASH);
  MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  leader->facing = 0x4000;
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  table->shown = 1;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  x = 0;
  do {
    if (x == 5)
      Event_PlaySound(SE_DUST_PUFF);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    leader->y += -0x9999;
    System_Wait(1);
    x++;
  } while (x <= 39);
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  table->shown = 0;
  gFieldWork->transition = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
