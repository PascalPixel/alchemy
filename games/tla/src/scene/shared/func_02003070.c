/// @file
/// A routine 4 scenes' overlays each keep a copy of
/// (mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s3d80,
/// mars_lighthouse_star_room/s2148, prox), included as text where each copy
/// sits after the scene defines SHARED_F02003070_NAME, SHARED_F02003070_ID2,
/// SHARED_F02003070_ID3, SHARED_F02003070_ID4, SHARED_F02003070_ID5,
/// SHARED_F02003070_ID6, SHARED_F02003070_ID7, SHARED_F02003070_ID8.

void SHARED_F02003070_NAME(void) {
  struct Actor *object;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *q;
  s32 *p;

  /* FAKEMATCH(volatile): gData_020068b0 read as a volatile s16, reread after
   * each test and for lsls #16; as a plain u16, 54 of 347 instructions differ
   * in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  SHARED_F02003070_ID2 = 0;
retry:
  SHARED_F02003070_ID3();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = MapGrid_GetCellBehaviorAt(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = SHARED_F02003070_ID4[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
#if !TLA_PATCH_1
      return;
#else
      goto end;
#endif
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  FieldVector_AddPolarOffset(0x100000, angle, position);
  cell = MapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->timer = 0;
  Actor_SetMoveTarget(object, x, object->y, z);
  SHARED_F02003070_ID5(object, 2);
  Actor_SetAnimationSpeed(object, 48);
#if !TLA_PATCH_1
  FieldActor_WaitForMove(object);
#else
  for (i = 0; i <= 179 && SHARED_F02003070_ID6(object) == 0; i++) {
    if (SHARED_F02003070_ID2 != 0)
      goto end;
    System_Wait(1);
  }
#endif
  object->update = (void (*)(void))SHARED_F02003070_ID7;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (SHARED_F02003070_ID2 != 0)
#if !TLA_PATCH_1
    goto blocked;
#else
    goto end;
#endif
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct
     * call, 2 of 347 instructions differ in tla-en. */
    if (Value4((s32 (*)())SHARED_F02003070_ID8, (s32)&entry->x, radius,
               (s32)position, entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  i = 0;
  goto test;
next:
  System_Wait(1);
  i++;
test:
  if (i <= 179 && SHARED_F02003070_ID6(object) == 0 &&
      SHARED_F02003070_ID2 == 0)
    goto next;
  if (SHARED_F02003070_ID2 != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 94 of 347 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  FieldVector_AddPolarOffset(0x100000, angle, position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  FieldActor_WaitForMove(object);
  System_Wait(2);
  goto retry;
changed:
  System_Wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  SHARED_F02003070_ID5(object, 1);
}
