/// @file
/// The leader is thrown back by the flames that struck them: they take the fall
/// damage, blink, and are pushed cell by cell in the flames' direction,
/// sliding over ice, until a wall, another actor or a ledge stops them. Three
/// Mars Lighthouse rooms keep a copy; each includes this file as text after
/// defining KNOCK_BACK, its name, and the names its copy calls:
/// KNOCK_BACK_TAKE_DAMAGE, KNOCK_BACK_BLINK, the update hook while they are
/// thrown, KNOCK_BACK_SET_ANIMATION, KNOCK_BACK_CELL_ATTRIBUTE,
/// KNOCK_BACK_CHECK_COLLISION and KNOCK_BACK_SPHERES_OVERLAP.

void KNOCK_BACK(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  struct Vector3 position;

  if (main->renderMode == 0) {
    struct Actor *actor;

#ifdef UBFIX
    KNOCK_BACK_TAKE_DAMAGE();
#else
    // UB: the fall damage takes no arguments and returns nothing.
    ((s32 (*)())KNOCK_BACK_TAKE_DAMAGE)(-10, 0);
#endif
    actor = Actor_Get(main->currentOwner);
    work = (struct Work020023c4 *)(gInventorySnapshot + 0x188);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    actor->update = KNOCK_BACK_BLINK;
    actor->facing = TRIG_TURN / 4;
    KNOCK_BACK_SET_ANIMATION(actor, 49);
    for (;;) {
      if (KNOCK_BACK_CELL_ATTRIBUTE(actor->terrainId, actor->x, actor->z) ==
          7) {
        s32 i;

        /* FAKEMATCH(temporary): velocityX stored through a union. */
        ((union {
           s32 value;
           u16 half;
         } *)&actor->velocityX)
            ->value = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = Q16_ONE / 16;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (work->unk68 << 19);
          position.y = actor->y;
#if TLA_PATCH_1
          position.z = actor->z + (work->unk6a << 19);
#else
          // A once-only block: its start holds position's first two stores
          // ahead of the work read.
          do {
            position.z = actor->z + (work->unk6a << 19);
          } while (0);
#endif
          if (KNOCK_BACK_CHECK_COLLISION(actor, &position) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          System_Wait(1);
        }
        break;
      } else {
        s32 result;
#if TLA_PATCH_1
        s32 radius;
        struct Actor *other;
        u8 *flags;
        s32 x, z, off;
        s32 i;
#else
        s32 x, z, off, oldZ;
#endif

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
#if TLA_PATCH_1
        position.z = actor->z + (work->unk6a << 19);
        result = KNOCK_BACK_CHECK_COLLISION(actor, &position);
#else
        // A once-only block, as in the loop above, with the call inside so
        // the call reuses position's address.
        do {
          position.z = actor->z + (work->unk6a << 19);
          result = KNOCK_BACK_CHECK_COLLISION(actor, &position);
        } while (0);
#endif
        if (result > 0)
          break;
        if (result < 0) {
          actor->facing = TRIG_TURN / 4;
          Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        /* FAKEMATCH(wrapper): the overlap test forwarded through Value4. */
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Value4((s32 (*)())KNOCK_BACK_SPHERES_OVERLAP, (s32)&other->x,
                     radius, (s32)&position, other->radius - 2) >= 0) {
            System_Wait(1);
            goto done;
          }
        }
        do {
          x = actor->x += work->unk68 << 17;
        } while (0);
        z = actor->z + (work->unk6a << 17);
#else
        x = actor->x += work->unk68 << 17;
        // The old z read in a once-only block, whose end keeps the read
        // between x's store and the work read.
        /* FAKEMATCH(temporary): oldZ carries the z read out of the block. */
        do {
          oldZ = actor->z;
        } while (0);
        z = oldZ + (work->unk6a << 17);
#endif
        actor->z = z;
        off = (x & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->x = x - off;
        off = (z & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->z = z - off;
        work->unk04 = 0;
        System_Wait(1);
      }
    }
#if TLA_PATCH_1
  done:
#endif
    actor->update = 0;
    Actor_SetSpriteColorMode(actor, 0);
  }
#if !TLA_PATCH_1
  /* FAKEMATCH(barrier): an empty asm reading work before the last store. */
  asm("" : : "r"(work));
#endif
  // UB: work is set only when renderMode is 0; otherwise the store goes
  // through whatever its register holds.
#ifdef UBFIX
  if (main->renderMode == 0)
#endif
    work->unk04 = 0;
}
