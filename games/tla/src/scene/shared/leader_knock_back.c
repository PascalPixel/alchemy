/// @file
/// The leader is thrown back by the flames that struck them: they take the fall
/// damage, blink, and are pushed cell by cell in the flames' direction,
/// sliding over ice, until a wall, another actor or a ledge stops them. Four
/// Mars Lighthouse rooms keep a copy; each includes this file as text after
/// shared/party_take_fall_damage.h (FALL_DAMAGE) and after defining
/// KNOCK_BACK, its name, and the names its copy calls:
/// KNOCK_BACK_TAKE_DAMAGE, KNOCK_BACK_BLINK, the update hook while they are
/// thrown, KNOCK_BACK_SET_ANIMATION, KNOCK_BACK_CELL_ATTRIBUTE and
/// KNOCK_BACK_CHECK_COLLISION.

// Typed as an event that returns a value, though it returns none.
s32 KNOCK_BACK(void) {
  struct MainRoutineState *main = &gMainRoutineState;
  struct FlameHitState *state;
  struct Vector3 position;

  if (main->movementMode == LEADER_MOVEMENT_WALK) {
    struct Actor *actor;

#ifdef UBFIX
    KNOCK_BACK_TAKE_DAMAGE();
#else
    // UB: the fall damage takes no arguments and returns nothing; it is
    // called with the HP change it makes.
    ((s32 (*)())KNOCK_BACK_TAKE_DAMAGE)(-FALL_DAMAGE, 0);
#endif
    actor = Actor_Get(main->leaderActorId);
    state = RIDE_STATE;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    actor->update = KNOCK_BACK_BLINK;
    actor->facing = DEGtoANG(90);
    KNOCK_BACK_SET_ANIMATION(actor, ANIM_LEADER_SIT);
    for (;;) {
      if (KNOCK_BACK_CELL_ATTRIBUTE(actor->layer, actor->x.fixed,
                                    actor->z.fixed) == TERRAIN_ROUNDED_SQUARE) {
        s32 i;

        // Sliding: 2 pixels a frame for up to 10 frames, until the ground
        // 8 pixels ahead stops them.
        actor->velocityX = (s16)state->direction.x * toFIXED(2);
        actor->velocityZ = (s16)state->direction.z * toFIXED(2);
        actor->acceleration = FIXED_ONE / 16;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x.fixed + (s16)state->direction.x * toFIXED(8);
          position.y = actor->y.fixed;
          position.z = actor->z.fixed + (s16)state->direction.z * toFIXED(8);
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
        FIXED x, z;
        s32 off;
#endif

        position.x = actor->x.fixed + (s16)state->direction.x * toFIXED(8);
        position.y = actor->y.fixed;
        position.z = actor->z.fixed + (s16)state->direction.z * toFIXED(8);
        result = KNOCK_BACK_CHECK_COLLISION(actor, &position);
        if (result > 0)
          break;
        if (result < 0) {
          actor->facing = DEGtoANG(90);
          Actor_JumpDownLedge(gMainRoutineState.leaderActorId, 6, -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->actors;
        flags = &other->collisionFlags;
        for (i = 0; i < ACTOR_COUNT;
             i++, flags += sizeof(struct Actor), other++) {
          if (other->script != 0 && (*flags & ACTOR_SOLID) && other != actor &&
              Event_CheckSpheresOverlap(&other->x.fixed, radius,
                                        (s32 *)&position,
                                        other->radius - 2) >= 0) {
            System_Wait(1);
            goto done;
          }
        }
#endif
        // A step of 2 pixels, kept within a pixel of the cell's middle
        // across it.
        x = actor->x.fixed += (s16)state->direction.x * toFIXED(2);
        z = actor->z.fixed + (s16)state->direction.z * toFIXED(2);
        actor->z.fixed = z;
        off = (x & ~FIELD_CELL_MASK) - toFIXED(8);
        if (off > FIXED_ONE)
          off = FIXED_ONE;
        if (off < -FIXED_ONE)
          off = -FIXED_ONE;
        actor->x.fixed = x - off;
        off = (z & ~FIELD_CELL_MASK) - toFIXED(8);
        if (off > FIXED_ONE)
          off = FIXED_ONE;
        if (off < -FIXED_ONE)
          off = -FIXED_ONE;
        actor->z.fixed = z - off;
        state->active = 0;
        System_Wait(1);
      }
    }
#if TLA_PATCH_1
  done:
#endif
    actor->update = 0;
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
  }
  // UB: state is set only when renderMode is 0; otherwise the store goes
  // through whatever its register holds.
#ifdef UBFIX
  if (main->renderMode == 0)
#endif
    state->active = 0;
}
