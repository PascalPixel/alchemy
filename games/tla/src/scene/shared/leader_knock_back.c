/// @file
/// The leader is thrown back by the flames that struck them: they take the fall
/// damage, blink, and are pushed cell by cell in the flames' direction,
/// sliding over ice, until a wall, another actor or a ledge stops them. Three
/// Mars Lighthouse rooms keep a copy; each includes this file as text after
/// defining KNOCK_BACK, its name, and the names its copy calls:
/// KNOCK_BACK_TAKE_DAMAGE, KNOCK_BACK_BLINK, the update hook while they are
/// thrown, KNOCK_BACK_SET_ANIMATION, KNOCK_BACK_CELL_ATTRIBUTE,
/// KNOCK_BACK_CHECK_COLLISION and KNOCK_BACK_SPHERES_OVERLAP.

#if TLA_PATCH_1
/// Whether two spheres overlap (KNOCK_BACK_SPHERES_OVERLAP), from an event.
static inline s32 Event_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b,
                                            s32 radiusB) {
  return KNOCK_BACK_SPHERES_OVERLAP(a, radiusA, b, radiusB);
}
#endif

void KNOCK_BACK(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct FlameHitWork *work;
  struct Vector3 position;

  if (main->movementMode == 0) {
    struct Actor *actor;

#ifdef UBFIX
    KNOCK_BACK_TAKE_DAMAGE();
#else
    // UB: the fall damage takes no arguments and returns nothing.
    ((s32 (*)())KNOCK_BACK_TAKE_DAMAGE)(-10, 0);
#endif
    actor = Actor_Get(main->leaderActorId);
    work = RIDE_WORK;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    actor->update = KNOCK_BACK_BLINK;
    actor->facing = DEGtoANG(90);
    KNOCK_BACK_SET_ANIMATION(actor, 49);
    for (;;) {
      if (KNOCK_BACK_CELL_ATTRIBUTE(actor->layer, actor->x.fixed,
                                    actor->z.fixed) == 7) {
        s32 i;

        actor->velocityX = (s16)work->direction.x << 17;
        actor->velocityZ = (s16)work->direction.z << 17;
        actor->acceleration = FIXED_ONE / 16;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x.fixed + ((s16)work->direction.x << 19);
          position.y = actor->y.fixed;
          position.z = actor->z.fixed + ((s16)work->direction.z << 19);
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
        s32 x, z, off;
#endif

        position.x = actor->x.fixed + ((s16)work->direction.x << 19);
        position.y = actor->y.fixed;
        position.z = actor->z.fixed + ((s16)work->direction.z << 19);
        result = KNOCK_BACK_CHECK_COLLISION(actor, &position);
        if (result > 0)
          break;
        if (result < 0) {
          actor->facing = DEGtoANG(90);
          Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->actors;
        flags = &other->collisionFlags;
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Event_CheckSpheresOverlap(&other->x.fixed, radius,
                                        (s32 *)&position,
                                        other->radius - 2) >= 0) {
            System_Wait(1);
            goto done;
          }
        }
#endif
        x = actor->x.fixed += (s16)work->direction.x << 17;
        z = actor->z.fixed + ((s16)work->direction.z << 17);
        actor->z.fixed = z;
        off = (x & 0xfffff) + 0xfff80000;
        if (off > FIXED_ONE)
          off = FIXED_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->x.fixed = x - off;
        off = (z & 0xfffff) + 0xfff80000;
        if (off > FIXED_ONE)
          off = FIXED_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->z.fixed = z - off;
        work->active = 0;
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
    work->active = 0;
}
