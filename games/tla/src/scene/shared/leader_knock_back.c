/* The leader is thrown back by the flames that struck them: they take the fall
   damage, blink, and are pushed metatile by metatile in the flames' direction,
   sliding over ice, until a wall, another actor or a ledge stops them. Four
   Mars Lighthouse rooms keep a copy; each includes this file as text after
   shared/party_take_fall_damage.h (FALL_DAMAGE) and after defining KNOCK_BACK,
   its name, and the names its copy calls: KNOCK_BACK_TAKE_DAMAGE,
   KNOCK_BACK_BLINK, the update hook while they are thrown,
   KNOCK_BACK_SET_ANIMATION, KNOCK_BACK_METATILE_ATTRIBUTE and
   KNOCK_BACK_CHECK_COLLISION. */

/* Typed as a script that returns a value, though it returns none. */
Sint32 KNOCK_BACK(void)
{
	MAIN_ROUTINE_STATE *main = &MainRoutineStateWork;
	FLAME_HIT_STATE	   *state;
	VECTOR3				pos;

	if(main->movementMode == LEADER_MOVEMENT_WALK) {
		ACTOR *actor;

#if FIX_FUNCTION_SIGNATURES
		KNOCK_BACK_TAKE_DAMAGE();
#else
		/* UB: the fall damage takes no arguments and returns nothing; it is
		   called with the HP change it makes. */
		((Sint32 (*)())KNOCK_BACK_TAKE_DAMAGE)(-FALL_DAMAGE, 0);
#endif
		actor = fdActorGet(main->leaderActorId);
		state = FLAME_THROWERS_STATE;
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		actor->update = KNOCK_BACK_BLINK;
		actor->facing = DEGtoANG(90);
		KNOCK_BACK_SET_ANIMATION(actor, ANIM_LEADER_SIT);
		for(;;) {
			if(KNOCK_BACK_METATILE_ATTRIBUTE(actor->layer, actor->x.fixed,
				   actor->z.fixed) == TERRAIN_ROUNDED_SQUARE) {
				Sint32 i;

				/* Sliding: 2 pixels a tick for up to 10 ticks, until the
				   ground 8 pixels ahead stops them. */
				actor->velX = (Sint16)state->dir.x * toFIXED(2);
				actor->velZ = (Sint16)state->dir.z * toFIXED(2);
				actor->acceleration = toFIXED(1) / 16;
				for(i = 0; i <= 9; i++) {
					pos.x = actor->x.fixed + (Sint16)state->dir.x * toFIXED(8);
					pos.y = actor->y.fixed;
					pos.z = actor->z.fixed + (Sint16)state->dir.z * toFIXED(8);
					if(KNOCK_BACK_CHECK_COLLISION(actor, &pos) != 0) {
						actor->velX = 0;
						actor->velZ = 0;
						break;
					}
					syWait(1);
				}
				break;
			} else {
				Sint32 result;
#if KNOCK_BACK_STOPS_AT_ACTORS
				Sint32 radius;
				ACTOR *other;
				Uint8 *flg;
				Sint32 x, z, off;
				Sint32 i;
#else
				FIXED  x, z;
				Sint32 off;
#endif

				pos.x = actor->x.fixed + (Sint16)state->dir.x * toFIXED(8);
				pos.y = actor->y.fixed;
				pos.z = actor->z.fixed + (Sint16)state->dir.z * toFIXED(8);
				result = KNOCK_BACK_CHECK_COLLISION(actor, &pos);
				if(result > 0)
					break;
				if(result < 0) {
					actor->facing = DEGtoANG(90);
					fdActorJumpDownLedge(
						MainRoutineStateWork.leaderActorId, 6, -1);
					break;
				}
#if KNOCK_BACK_STOPS_AT_ACTORS
				radius = actor->radius - 2;
				other = RamHeapSlots->actors;
				flg = &other->collisionFlg;
				for(i = 0; i < ACTOR_COUNT;
					i++, flg += sizeof(ACTOR), other++) {
					if(other->script != 0 && (*flg & ACTOR_SOLID) &&
						other != actor &&
						scScriptCheckSpheresOverlap(&other->x.fixed, radius,
							(Sint32 *)&pos, other->radius - 2) >= 0) {
						syWait(1);
						goto done;
					}
				}
#endif
				/* A step of 2 pixels, kept within a pixel of the metatile's
				   middle across it. */
				x = actor->x.fixed += (Sint16)state->dir.x * toFIXED(2);
				z = actor->z.fixed + (Sint16)state->dir.z * toFIXED(2);
				actor->z.fixed = z;
				off = (x & ~FIELD_METATILE_MASK) - toFIXED(8);
				if(off > toFIXED(1))
					off = toFIXED(1);
				if(off < -toFIXED(1))
					off = -toFIXED(1);
				actor->x.fixed = x - off;
				off = (z & ~FIELD_METATILE_MASK) - toFIXED(8);
				if(off > toFIXED(1))
					off = toFIXED(1);
				if(off < -toFIXED(1))
					off = -toFIXED(1);
				actor->z.fixed = z - off;
				state->active = 0;
				syWait(1);
			}
		}
#if KNOCK_BACK_STOPS_AT_ACTORS
	done:
#endif
		actor->update = 0;
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
	}
	/* UB: state is set only when renderMode is 0; otherwise the store goes
	   through whatever its register holds. */
#if FIX_UNINITIALIZED_READ
	if(main->renderMode == 0)
#endif
		state->active = 0;
}
