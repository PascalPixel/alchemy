/* Ropes, tightropes and hanging vines: chains of nodes laid between two points
   or round one, drawn and updated each tick, and the leader crossing them.
   Part of script_commands.c. */

/* The animations of a rope's or tightrope's post (SPRITE_TIGHTROPE_POST). */
typedef enum POST_ANIMATION {
	/* the rope winding round the post, then ANIM_POST_TIED */
	ANIM_POST_TIE = 2,
	ANIM_POST_UNTIE = 3, /* the rope unwinding, then the bare post */
	ANIM_POST_TIED = 4	 /* the rope wound round the post */
} POST_ANIMATION;

/* A sprite's depth slot is its point's y + z in pixels plus a bias: an actor's
   sprite adds 62 and its shadow 60 (fdRenderApplyProjectedPlacement), a rope's
   and a tightrope's sprites this, a hanging vine's links HANGING_VINE_DEPTH. */
#define ROPE_DEPTH 58

/* The ropes Lash ties (HEAP_SLOT_ROPE), as in the Shrine of the Sea God: eight
   slots, each a chain of nodes between two posts, coiled round the foot of the
   first post until tied; tied, the rope runs from post to post and is wound
   round both, and can be crossed hand over hand (LEADER_MOVEMENT_ROPE). Pushing
   at a rope's post for more than ROPE_PUSH_STEPS ticks raises
   FIELD_RAISED_CUE_ROPE_ON, whose scene script puts the leader on the rope; on
   it, it raises FIELD_RAISED_CUE_ROPE_OFF. fdActorCrossRope, which
   ActorRopeScript runs once a tick, adds half a pixel to the rope's swing, up
   to two, on each tick it sends the leader on toward a node, so the swing is
   full within a few ticks of setting off. fdRopeUpdate and fdRopeLayBowed bow
   the middle of the rope that far either way of its line, in a cycle of 16
   ticks, the swing losing a twentieth a tick. */

/* The leader steps onto a rope or tightrope once it has pushed against its post
   (FIELD_STATE.blockedSteps) for more than this many ticks. */
#define ROPE_PUSH_STEPS 12

/* A post whose sceneByte byte holds this keeps in watchedActor a hook giving
   the point Lash throws the rope to (fdRopeLash); no scene marks a post so. */
#define ROPE_POST_HOOKED 99

/* The posts the unused debug hooks (fdRopeDebugSwing, fdTightropeDebugSwing)
   swing rope 0 between: the fifth and sixth actors a scene places. */
#define DEBUG_FIRST_POST  (ACTOR_FIRST_PLACED + 4)
#define DEBUG_SECOND_POST (ACTOR_FIRST_PLACED + 5)

/* Allocates the ropes' state, loads their node picture and starts fdRopeDraw
   and fdRopeUpdate. A scene with ropes to lash calls it first. */
void fdRopeInit(void)
{
	ROPE_WORK *state = syHeapSlotAllocEWRAM(HEAP_SLOT_ROPE, sizeof(ROPE_WORK));
	void	  *buf = syHeapAlloc(256);

	DMA_FILL32(0, state, sizeof(ROPE_WORK));
	state->vramEntry = syVRAMCacheFindFreeEntry();
	syDecompressTaggedLZ(Rope_Tiles, buf);
	syVRAMCacheLoad(state->vramEntry, 256, buf);
	sySetEvent(fdRopeDraw, EVENT_PASS_TICK);
	sySetEvent(fdRopeUpdate, EVENT_PASS_TICK + 15);
	syHeapRelease(buf);
}

/* The ropes' drawing event: a sprite at the midpoint of each pair of
   neighbouring nodes, 4 pixels up, turned along the pair and given its rope's
   priority, for the pairs on screen; a rope's first node keeps the last
   turn. */
void fdRopeDraw(void)
{
	ROPE_WORK *state = ROPE_STATE;
	ROPE_SLOT *slot = state->slots;
	Sint32	   i;
	Sint32	   prevX = 0;
	Sint32	   prevY = 0;
	Uint32	   tile = VramCacheEntries[state->vramEntry].ofs / TILE_SIZE_4BPP;
	Sint32	   prevZ = 0;
	Sint32	   started = 0;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	Sint32	   viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	Sint32	   originY = map->origin[1];
	AFFINE_TRANSFORM transform;
	ROPE_NODE		*node;
	Uint32			 ang;

	for(i = 0; i < ROPE_SLOT_COUNT; i++, slot++) {
		for(node = slot->head; node != NULL; node = node->next) {
			Sint32 pri = slot->pri;
			FIXED  y = node->y;
			Sint32 x = node->x;
			Sint32 z = node->z;

			y -= toFIXED(4);

			if(started) {
				Sint32 screenX;
				Sint32 depth;
				Sint32 screenY;

				ang = ((Uint16)syAtan(prevX - x, (z - prevZ) + (prevY - y)) +
						  DEGtoANG(90)) &
					TRIG_ANGLE_MASK;
				screenX = (prevX + x) / 2 - viewX;
				prevY = (prevY + y) / 2 - originY;
				depth = (prevZ + z) / 2 - viewZ - originY;
				screenY = depth - prevY;
				if(screenX > -toFIXED(32) &&
					screenX < toFIXED(DISPLAY_WIDTH + 32) &&
					screenY > -toFIXED(32) &&
					screenY < toFIXED(DISPLAY_HEIGHT + 64)) {
					Sint32 spriteX = (FIXEDtoINT(screenX) - 4) & OBJ_X_MASK;
					Sint32 spriteY = (FIXEDtoINT(screenY) - 4) & OBJ_Y_MASK;
					ANIMATION_SPR_PART *spr;

					prevY = FIXEDtoINT(prevY + depth) + ROPE_DEPTH;
					spr = &node->spr;

					((Uint32 *)spr)[0] = 0;
					((Uint32 *)&node->spr)[1] =
						spriteY | OBJ_X(spriteX) | OBJ_256_COLORS;
					((Uint32 *)&node->spr)[2] = OBJ_PRIORITY(2) | tile;
					if(ang != 0) {
						transform.ang = ang;
						transform.scaleX = AFFINE_SCALE_ONE;
						transform.scaleY = AFFINE_SCALE_ONE;
						node->spr.affine = 1;
						node->spr.affineIdx =
							syOAMMatrixAllocFromTransform(&transform);
					}
					node->spr.pri = pri;
					node->ang = ang;
					syOAMAddSprAtDepthClamped(spr, prevY);
				}
			}
			prevX = x;
			prevY = y;
			prevZ = z;
			started = 1;
		}
		prevX = 0;
		prevY = 0;
		prevZ = 0;
		started = 0;
		if(slot->head != NULL)
			slot->head->ang = ang;
	}
}

/* The ropes' event: unless the field is handling a request, raises
   FIELD_RAISED_CUE_ROPE_ON when the leader has pushed at one end of a tied
   rope, or FIELD_RAISED_CUE_ROPE_OFF while on one; then swings each swinging
   rope, its swing losing a twentieth a tick until under a tenth of a pixel. */
void fdRopeUpdate(void)
{
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	ROPE_WORK	*state = ROPE_STATE;
	ROPE_SLOT	*slot;
	Sint32		 i = 0;

	if(!syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS) &&
		event->blockedSteps > ROPE_PUSH_STEPS) {
		if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK) {
			Sint32 tgt =
				fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);

			if(tgt != -1) {
				for(i = 0; i < ROPE_SLOT_COUNT; i++)
					if(state->slots[i].first == tgt ||
						state->slots[i].second == tgt)
						break;
				if(i < ROPE_SLOT_COUNT &&
					fdRopeIsLeaderAtEnd(
						state->slots[i].first, state->slots[i].second))
					event->raisedCue = FIELD_RAISED_CUE_ROPE_ON;
			}
		} else if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_ROPE) {
			event->raisedCue = FIELD_RAISED_CUE_ROPE_OFF;
		}
	}
	slot = state->slots;
	for(i = 0; i < ROPE_SLOT_COUNT; i++, slot++) {
		if(slot->swinging != 0) {
			FIXED swing = syMulFX(sySin(slot->phase), slot->amplitude);

			slot->phase += TRIG_TURN / 16;
			slot->amplitude = syMulFX(slot->amplitude, toFIXED(19) / 20);
			if(slot->amplitude < toFIXED(1) / 10) {
				slot->amplitude = 0;
				slot->swinging = 0;
			}
			fdRopeSwingBetween(i, slot->first, slot->second, swing);
		}
	}
}

/* Returns the first unused of the ropes' 128 nodes, or NULL. */
ROPE_NODE *fdRopeAllocNode(void)
{
	ROPE_WORK *state = ROPE_STATE;
	ROPE_NODE *node = state->nodes;
	Sint32	   idx;

	for(idx = 0; idx < ROPE_NODE_COUNT; idx++) {
		if(node->kind == ROPE_NODE_FREE)
			return node;
		node++;
	}
	return NULL;
}

/* Lays rope index tied between posts id and other, and shows both posts with
   the rope wound round them. */
void fdRopeTie(Sint32 idx, Sint32 id, Sint32 other)
{
	ROPE_WORK *state = ROPE_STATE;
	Sint32	   pri = 2;
	ACTOR	  *first = fdActorLookup(id);
	ACTOR	  *second = fdActorLookup(other);

	if(first != NULL && second != NULL) {
		ROPE_SLOT *slot;

		if(first->sprStorageKind == ACTOR_SPRITE_ONE)
			pri = ((SPR *)first->spr)->image.pri;
		slot = &state->slots[idx];
		slot->first = id;
		slot->second = other;
		slot->pri = pri;
		fdRopeLayTied(idx, first->x.fixed, first->y.fixed + toFIXED(8),
			first->z.fixed, second->x.fixed, second->y.fixed + toFIXED(8),
			second->z.fixed);
	}
	fdActorSelectAnimationStub(first, ANIM_POST_TIE);
	fdActorSelectAnimationStub(second, ANIM_POST_TIE);
}

/* Builds rope index's nodes on the line between two points, one for each 6
   pixels, waved a pixel either way by one whole sine; marks it tied and
   swinging and sets its flag. */
Sint32 fdRopeLayTied(Sint32 idx, Sint32 x1, Sint32 y1, Sint32 z1, Sint32 x2,
	Sint32 y2, Sint32 z2)
{
	ROPE_WORK  *state = ROPE_STATE;
	Sint32		result = 0;
	Sint32		dx = x2 - x1;
	Sint32		sx = dx >> 8;
	Sint32		dy = y2 - y1;
	Sint32		sy = dy >> 8;
	Sint32		dz = z2 - z1;
	Sint32		sz = dz >> 8;
	Sint32		cnt;
	ROPE_SLOT  *slot;
	ROPE_NODE **link;
	ROPE_NODE  *node;
	Uint16		ang;
	FIXED		sin;
	FIXED		cos;
	Sint32		i;

	cnt =
		sySquartIWRAM(syMulFX(sx, sx) + syMulFX(sy, sy) + syMulFX(sz, sz)) / 6 +
		1;
	slot = &state->slots[idx];
	link = &slot->head;
	ang = syAtan(sx, sy);
	sin = sySin(ang);
	cos = syCos(ang);
	slot->segments = cnt;
	for(i = 0; i <= cnt; i++) {
		FIXED bow;

		node = fdRopeAllocNode();
		if(node == NULL) {
			result = -1;
			break;
		}
		bow = syMulFX(sySin(i * TRIG_TURN / cnt), toFIXED(1));
		SET_POSITION(node, x1 + i * dx / cnt + syMulFX(bow, cos),
			y1 + i * dy / cnt + syMulFX(bow, sin), z1 + i * dz / cnt);
		node->slot = idx + 1;
		if(i == cnt)
			node->kind = ROPE_NODE_UNCROSSABLE;
		else
			node->kind = ROPE_NODE_CROSSABLE;
		*link = node;
		link = &node->next;
	}
	*link = NULL;
	slot->state = ROPE_THROWN;
	slot->swinging = 1;
	slot->amplitude = toFIXED(1);
	slot->phase = DEGtoANG(90);
	if(slot->flg != 0)
		syGameFlagSet(slot->flg);
	return result;
}

/* Lays rope index again between posts from and to, its middle bowed height
   pixels (16.16) aside. */
void fdRopeSwingBetween(Sint32 idx, Sint32 from, Sint32 to, FIXED height)
{
	ACTOR *a = fdActorLookup(from);
	ACTOR *b = fdActorLookup(to);

	if(a != NULL && b != NULL)
		fdRopeLayBowed(idx, a->x.fixed, a->y.fixed + toFIXED(8), a->z.fixed,
			b->x.fixed, b->y.fixed + toFIXED(8), b->z.fixed, height);
}

/* VIEW(ropes): routine syAtan as returning Sint16; system/system.h has it
   return Sint32. */
extern Sint16 syTrigAtanHalfword(Sint32 x, Sint32 y);

/* Lays a rope's nodes along the line between two points again, bowed by half a
   sine height pixels (16.16) high. */
Sint32 fdRopeLayBowed(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ, FIXED height)
{
	ROPE_WORK *state = ROPE_STATE;
	Sint32	   dx = toX - x;
	Sint32	   ax = dx >> 8;
	Sint32	   dy = toY - y;
	ROPE_SLOT *slot = &state->slots[idx];
	ROPE_NODE *node = slot->head;
	Uint16	   ang = syTrigAtanHalfword(ax, dy >> 8);
	FIXED	   sin = sySin(ang);
	FIXED	   cos = syCos(ang);
	Sint32	   segments = slot->segments;
	Sint32	   i;
	Sint32	   kind = ROPE_NODE_CROSSABLE;

	for(i = 0; i <= segments; i++) {
		FIXED bow = syMulFX(sySin(i * (TRIG_TURN / 2) / segments), height);

		SET_POSITION(node, x + i * dx / segments + syMulFX(bow, cos),
			y + i * dy / segments + syMulFX(bow, sin),
			z + i * (toZ - z) / segments);
		node->kind = kind;
		node = node->next;
	}
	return 0;
}

/* Coils rope index at post firstId, its length reaching post secondId: the rope
   as it waits to be lashed. */
void fdRopeCoil(Sint32 idx, Sint32 firstId, Sint32 secondId)
{
	ROPE_WORK *state = ROPE_STATE;
	Sint32	   pri = 2;
	ACTOR	  *first = fdActorLookup(firstId);
	ACTOR	  *second = fdActorLookup(secondId);

	if(first != NULL && second != NULL) {
		ROPE_SLOT *slot;

		if(first->sprStorageKind == ACTOR_SPRITE_ONE)
			pri = ((SPR *)first->spr)->image.pri;
		slot = &state->slots[idx];
		slot->first = firstId;
		slot->second = secondId;
		slot->pri = pri;
		fdRopeLayCoil(idx, first->x.fixed, first->y.fixed + toFIXED(8),
			first->z.fixed, second->x.fixed, second->y.fixed + toFIXED(8),
			second->z.fixed);
	}
}

/* Builds rope index's nodes, one for each 6 pixels between two points, as a
   flat spiral around the first point: the rope coiled at its post. */
Sint32 fdRopeLayCoil(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ)
{
	ROPE_WORK *state = ROPE_STATE;
	Sint32	   result = 0;
	Sint32	   dx = (toX - x) >> 8;
	Sint32	   dy = (toY - y) >> 8;
	Sint32	   dz = (toZ - z) >> 8;
	Sint32	   length =
		sySquartEntry(syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz));
	Sint32		segments = length / 6 + 1;
	ROPE_SLOT  *slot = &state->slots[idx];
	ROPE_NODE **link = &slot->head;
	Sint32		ang = 0;
	FIXED		radius = toFIXED(4);
	Sint32		i;
	Sint32		kind = ROPE_NODE_UNCROSSABLE;

	slot->segments = segments;
	for(i = 0; i <= segments; i++) {
		ROPE_NODE *node = fdRopeAllocNode();
		Uint16	   step;

		if(node == NULL) {
			result = -1;
			break;
		}
		SET_POSITION(node, x + syMulFX(syCos(ang), radius), y,
			z + syMulFX(sySin(ang), radius));
		node->kind = kind;
		*link = node;
		step = syAtan(toFIXED(6), radius);
		ang += step;
		link = &node->next;
		radius += step * 4;
	}
	*link = NULL;
	return result;
}

/* Writes where rope index's last node is, its far end, into position. */
void fdRopeGetEndNodePos(Sint32 idx, Sint32 *pos)
{
	ROPE_WORK *state = ROPE_STATE;
	ROPE_SLOT *slot = &state->slots[idx];
	ROPE_NODE *node = slot->head;
	Sint32	   remaining;

	remaining = slot->segments;
	if(remaining > 0) {
		do {
			remaining--;
			node = node->next;
		} while(remaining != 0);
	}
	SET_VECTOR(pos, node->x, node->y, node->z);
}

extern Sint32 RopeLashPhase;

Sint32 fdRopeThrow(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ, Sint32 wave);
Sint32 fdRopeRecoil(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ, Sint32 wave);

/* Lash: throws rope index from post fromId to post toId and winds it round
   both. A rope coiled too short to reach falls back to its post. */
void fdRopeLash(Sint32 idx, Sint32 fromId, Sint32 toId)
{
	ROPE_WORK *state = ROPE_STATE;
	ACTOR	  *from = fdActorLookup(fromId);
	ACTOR	  *to = fdActorLookup(toId);

	if(from != NULL && to != NULL) {
		FIXED	   toX = to->x.fixed;
		FIXED	   toY = to->y.fixed;
		FIXED	   toZ = to->z.fixed;
		ROPE_SLOT *slot = &state->slots[idx];
		Sint32	   x, y = 0, z; /* Default: y. */
		Sint32	   xx, yy, length, segments, cnt;
		Sint32 (*mul)(Sint32, Sint32);

		if(to->sceneByte == ROPE_POST_HOOKED)
			((TARGET_HOOK)to->watchedActor)(to, &toX, &toZ);
		x = (toX - from->x.fixed) >> 8;
		y = (toY - from->y.fixed) >> 8;
		z = (toZ - from->z.fixed) >> 8;
		mul = syMulFX;
		xx = mul(x, x);
		yy = mul(y, y);
		length = xx + yy + mul(z, z);
		segments = sySquartEntry(length) / 6 + 1;
		cnt = slot->segments;
		if(segments > cnt) {
			cnt++;
			x = from->x.fixed + (toX - from->x.fixed) * cnt / segments;
			y = toY;
			z = from->z.fixed + (toZ - from->z.fixed) * cnt / segments;

			fdRopeThrow(idx, from->x.fixed, from->y.fixed + toFIXED(8),
				from->z.fixed, x, y + toFIXED(8), z, sySin(RopeLashPhase) << 2);
			slot->swinging = 0;
			sySynch(1);
			fdRopeRecoil(idx, from->x.fixed, from->y.fixed + toFIXED(8),
				from->z.fixed, x, y + toFIXED(8), z, sySin(RopeLashPhase) << 2);
		} else {
			fdRopeThrow(idx, from->x.fixed, from->y.fixed + toFIXED(8),
				from->z.fixed, to->x.fixed, to->y.fixed + toFIXED(8),
				to->z.fixed, sySin(RopeLashPhase) << 2);
			RopeLashPhase += TRIG_TURN / 16;
			sySynch(6);
			fdActorSelectAnimationStub(from, ANIM_POST_TIE);
			fdActorSelectAnimationStub(to, ANIM_POST_TIE);
		}
	}
}

/* Throws rope index from one point to another over twice its node count in
   ticks: the end flies on an arc and each node follows the one ahead at 6
   pixels; then the rope swings and its flag is set. Does nothing to a rope
   already thrown. The callers pass the wobble fdTightropeRecoil takes, which a
   rope leaves unused, and read no result: 0, or nothing for a rope already
   thrown. */
Sint32 fdRopeThrow(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ, Sint32 wave)
{
	ROPE_WORK  *state = ROPE_STATE;
	Sint32		dx = toX - x;
	Sint32		ex = dx >> 8;
	Sint32		dy = toY - y;
	Sint32		ey = dy >> 8;
	Sint32		dz = toZ - z;
	Sint32		segments;
	Sint32		frm;
	FIXED		sine;
	FIXED		cosine;
	Sint32		lastX;
	Sint32		lastY;
	Sint32		lastZ;
	ROPE_SLOT  *slot = &state->slots[idx];
	ROPE_NODE  *node = slot->head;
	void	  **cursor;
	Sint32		i;
	Uint16		ang;
	ROPE_NODE **nodes;

	if(slot->state == ROPE_THROWN)
		return;
	slot->state = ROPE_THROWN;
	segments = slot->segments;
	cursor = nodes = syHeapAlloc(512);
	for(i = 0; i <= segments; i++) {
		*cursor++ = node;
		node = node->next;
	}
	ang = syAtan(ex, ey);
	sine = sySin(ang);
	cosine = syCos(ang);
	for(frm = 1; frm <= segments * 2; frm++) {
		for(i = segments; i >= 0; i--) {
			node = nodes[i];
			node->slot = idx + 1;
			if(i == segments) {
				ex = syMulFX(sySin(frm * (TRIG_TURN / 4) / i), toFIXED(8));

				node->x = x + dx * frm / 2 / i + syMulFX(ex, cosine);
				lastX = node->x;
				node->y = y + dy * frm / 2 / i + syMulFX(ex, sine);
				lastY = node->y;
				node->z = z + dz * frm / 2 / i;
				lastZ = node->z;
			} else {
				Sint32 ez;

				ex = (lastX - node->x) >> 8;
				ey = (lastY - node->y) >> 8;
				ez = (lastZ - node->z) >> 8;
				ex = sySquartEntry(
					syMulFX(ex, ex) + syMulFX(ey, ey) + syMulFX(ez, ez));

				if(ex > 6) {
					node->x = lastX + (node->x - lastX) * 6 / ex;
					lastX = node->x;
					node->y = lastY + (node->y - lastY) * 6 / ex;
					lastY = node->y;
					node->z = lastZ + (node->z - lastZ) * 6 / ex;
					node->kind = ROPE_NODE_CROSSABLE;
					lastZ = node->z;
				}
			}
		}
		sySynch(1);
	}
	slot = &state->slots[idx];
	slot->swinging = 1;
	slot->amplitude = toFIXED(4);
	slot->phase = DEGtoANG(90);
	if(slot->flg != 0)
		syGameFlagSet(slot->flg);
	syHeapRelease(nodes);
	return 0;
}

extern Sint32 RopeUntiePhase;

/* Unties rope index between posts fromId and toId (fdRopeRecoil). */
void fdRopeUntie(Sint32 idx, Sint32 fromId, Sint32 toId)
{
	ROPE_WORK *state;
	ACTOR	  *from;
	ACTOR	  *to;

	state = ROPE_STATE;
	from = fdActorLookup(fromId);
	to = fdActorLookup(toId);

	if(from != NULL && to != NULL) {
		state->recoilFrom = from;
		state->recoilTo = to;
		fdRopeRecoil(idx, from->x.fixed, from->y.fixed + toFIXED(8),
			from->z.fixed, to->x.fixed, to->y.fixed + toFIXED(8), to->z.fixed,
			sySin(RopeUntiePhase) * 4);
		RopeUntiePhase += TRIG_TURN / 16;
	}
}

/* Pulls a thrown rope back into its coil at the first point, one node a tick,
   clearing its flag, starting four ticks after both posts start to unwind. The
   far end and the wobble are fdTightropeRecoil's arguments, which a rope leaves
   unused; as with fdRopeThrow, no caller reads the result. */
Sint32 fdRopeRecoil(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ, Sint32 wave)
{
	ROPE_WORK *state = ROPE_STATE;
	/* Default: each count below sets it again. */
	Sint32	   i = 0;
	ROPE_SLOT *slot = &state->slots[idx];
	ROPE_NODE *node = slot->head;
	void	 **cursor;
	Sint32	   ang;
	Sint32	   radius;
	Sint32	   lastZ;
	Sint32	   segments;
	Sint32	   frm;
	Sint32	   lastX;
	Sint32	   lastY;
	void	 **nodes;

	if(slot->state == ROPE_COILED)
		return;
	slot->state = ROPE_COILED;
	segments = slot->segments;
	cursor = nodes = syHeapAlloc(512);
	slot->swinging = 0;
	if(slot->flg != 0)
		syGameFlagClear(slot->flg);
	if(segments >= 0) {
		i = segments + 1;
		do {
			i += -1;
			*cursor++ = node;
			node = node->next;
		} while(i != 0);
	}
	fdActorSelectAnimationStub(state->recoilFrom, ANIM_POST_UNTIE);
	fdActorSelectAnimationStub(state->recoilTo, ANIM_POST_UNTIE);
	sySynch(4);
	for(frm = 0; frm <= segments; frm++) {
		ang = frm * (TRIG_TURN / 16);
		radius = toFIXED(4);
		for(i = 0; i <= segments; i++) {
			node = nodes[i];
			node->slot = 0;
			if(i <= frm) {
				Uint16 step;

				node->x = x + syMulFX(syCos(ang), radius);
				lastX = node->x;
				node->y = y;
				lastY = node->y;
				node->z = z + syMulFX(sySin(ang), radius);
				lastZ = node->z;
				node->kind = ROPE_NODE_UNCROSSABLE;
				step = syAtan(toFIXED(6), radius);
				ang += step;
				radius += step * 4;
			} else {
				Sint32 dx = (lastX - node->x) >> 8;
				Sint32 dy = (lastY - node->y) >> 8;
				Sint32 dz = (lastZ - node->z) >> 8;

				radius = sySquartEntry(
					syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz));

				if(radius <= 6)
					break;
				node->x = lastX + (node->x - lastX) * 6 / radius;
				lastX = node->x;
				node->y = lastY + (node->y - lastY) * 6 / radius;
				lastY = node->y;
				node->z = lastZ + (node->z - lastZ) * 6 / radius;
				node->kind = ROPE_NODE_UNCROSSABLE;
				lastZ = node->z;
			}
		}
		sySynch(1);
	}
	syHeapRelease(nodes);
	return 0;
}

/* Unused: nothing refers to it. Does nothing. */
UNUSED void fdRopeReservedNoOperation(void)
{
}

extern Sint32 RopeDebugSwinging;

/* Unused: nothing refers to it. An actor hook that, once L is pressed, lays
   rope 0 again between DEBUG_FIRST_POST and DEBUG_SECOND_POST every tick. */
UNUSED void fdRopeDebugSwing(ACTOR *actor)
{
	SPR *spr = actor->spr;

	spr->ofsX = -3;
	spr->ofsY = 12;
	if(PerWork.repeat & KEY_L)
		RopeDebugSwinging = 1;
	if(RopeDebugSwinging != 0)
		fdRopeSwingBetween(0, DEBUG_FIRST_POST, DEBUG_SECOND_POST, 0);
}

/* Returns 1 when the leader stands on the metatile of post firstId or secondId,
   or a metatile beside it along an axis, at the same height. */
Bool fdRopeIsLeaderAtEnd(Sint32 firstId, Sint32 secondId)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *first = fdActorGetById(firstId);
	ACTOR *second = fdActorGetById(secondId);
	Sint32 x = FIELD_METATILE(leader->x.fixed);
	Sint32 y = FIELD_METATILE(leader->y.fixed);
	Sint32 z = FIELD_METATILE(leader->z.fixed);
	Sint32 firstX = FIELD_METATILE(first->x.fixed);
	Sint32 firstY = FIELD_METATILE(first->y.fixed);
	Sint32 firstZ = FIELD_METATILE(first->z.fixed);
	Sint32 secondX = FIELD_METATILE(second->x.fixed);
	Sint32 secondY = FIELD_METATILE(second->y.fixed);
	Sint32 secondZ = FIELD_METATILE(second->z.fixed);

	if(y == firstY) {
		if(x == firstX) {
			if(z == firstZ - 1 || z == firstZ + 1 || z == firstZ)
				return TRUE;
		} else if(z == firstZ) {
			if(x == firstX - 1 || x == firstX + 1 || x == firstX)
				return TRUE;
		}
	}
	if(y == secondY) {
		if(x == secondX) {
			if(z == secondZ - 1 || z == secondZ + 1 || z == secondZ)
				return TRUE;
		} else if(z == secondZ) {
			if(x == secondX - 1 || x == secondX + 1 || x == secondX)
				return TRUE;
		}
	}
	return FALSE;
}

/* Finds the rope whose first post is id: writes its other post and its index
   and returns 0, or returns -1. */
Sint32 fdRopeFindById(Sint32 id, Sint32 *value, Sint32 *idx)
{
	ROPE_WORK *state = ROPE_STATE;
	ROPE_SLOT *slot = state->slots;
	Sint32	   i;

	for(i = 0; i < ROPE_SLOT_COUNT; i++) {
		if(slot->head != NULL && slot->first == id) {
			*value = slot->second;
			*idx = i;
			return 0;
		}
		slot++;
	}
	return -1;
}

/* Returns rope index's ROPE_SLOT_STATE, or -1 while the ropes have no state. */
Sint32 fdRopeGetState(Sint32 idx)
{
	ROPE_WORK *state = ROPE_STATE;
	ROPE_SLOT *slot;

	if(state == NULL)
		return -1;
	slot = &state->slots[idx];
	return slot->state;
}

/* Gives rope index its flag and places it between posts firstId and secondId:
   tied when the flag is set, else coiled at the first. A scene places each of
   its ropes with it. */
void fdRopePlace(Sint32 idx, Sint32 flg, Sint32 firstId, Sint32 secondId)
{
	ROPE_WORK *state = ROPE_STATE;
	ROPE_SLOT *slot;

	slot = &state->slots[idx];
	slot->flg = flg;
	if(!syGameFlagGet(flg))
		fdRopeCoil(idx, firstId, secondId);
	else
		fdRopeTie(idx, firstId, secondId);
}

/* The tightropes (HEAP_SLOT_TIGHTROPE), as in Champa: ropes stretched straight
   from post to post across a drop, that the party walks
   (LEADER_MOVEMENT_TIGHTROPE) and that wobble underfoot. The group copies the
   ropes' functions. */

/* Allocates the tightropes' state, loads their node picture and starts
   fdTightropeDraw and fdTightropeUpdate. */
void fdTightropeInit(void)
{
	TIGHTROPE_WORK *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_TIGHTROPE, sizeof(TIGHTROPE_WORK));
	void *buf = syHeapAlloc(2304);

	DMA_FILL32(0, state, sizeof(TIGHTROPE_WORK));
	state->vramEntry = syVRAMCacheFindFreeEntry();
	syDecompressTaggedLZ(Rope_Tiles, buf);
	syVRAMCacheLoad(state->vramEntry, 64, buf);
	sySetEvent(fdTightropeDraw, EVENT_PASS_TICK);
	sySetEvent(fdTightropeUpdate, EVENT_PASS_TICK + 15);
	syHeapRelease(buf);
}

/* The tightropes' drawing event: as fdRopeDraw, a sprite at the midpoint of
   each pair of neighbouring nodes, 4 pixels up, for the pairs on screen, all of
   a tightrope turned along it. */
void fdTightropeDraw(void)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	Sint32			i;
	Sint32			prevX = 0;
	FIXED			prevY = 0;
	Sint32			prevZ = 0;
	Sint32			started = 0;
	Uint32	   tile = VramCacheEntries[state->vramEntry].ofs / TILE_SIZE_4BPP;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	Sint32	   viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	Sint32	   originY = map->origin[1];
	AFFINE_TRANSFORM transform;

	for(i = 0; i < ROPE_SLOT_COUNT; i++) {
		Uint32			ang = state->slots[i].dir;
		TIGHTROPE_NODE *node = state->slots[i].head;
		Sint32			affine;
		Sint32			matrix;

		if(ang != 0) {
			transform.ang = ang;
			transform.scaleX = AFFINE_SCALE_ONE;
			transform.scaleY = AFFINE_SCALE_ONE;
			affine = 1;
			matrix = syOAMMatrixAllocFromTransform(&transform);
		} else {
			affine = 0;
			matrix = 0;
		}
		for(; node != NULL; node = node->next) {
			FIXED  y = node->y;
			Sint32 x = node->x;
			Sint32 z = node->z;

			y -= toFIXED(4);

			if(started) {
				Sint32 screenX = (prevX + x) / 2 - viewX;
				FIXED  screenY;

				prevY = (prevY + y) / 2 - originY;
				prevZ = (prevZ + z) / 2 - viewZ - originY;
				screenY = prevZ - prevY;

				if(screenX > -toFIXED(32) &&
					screenX < toFIXED(DISPLAY_WIDTH + 32) &&
					screenY > -toFIXED(32) &&
					screenY < toFIXED(DISPLAY_HEIGHT + 64)) {
					Sint32 left = (FIXEDtoINT(screenX) - 4) & OBJ_X_MASK;
					Sint32 top = (FIXEDtoINT(screenY) - 4) & OBJ_Y_MASK;
					Sint32 key = FIXEDtoINT(prevY + prevZ) + ROPE_DEPTH;

					((Uint32 *)&node->spr)[0] = 0;
					((Uint32 *)&node->spr)[1] =
						top | OBJ_X(left) | OBJ_256_COLORS;
					((Uint32 *)&node->spr)[2] = tile | OBJ_PRIORITY(2);
					node->spr.affine = affine;
					node->spr.affineIdx = matrix;
					syOAMAddSprAtDepthClamped(&node->spr, key);
				}
			}
			prevX = x;
			prevY = y;
			prevZ = z;
			started = 1;
		}
		prevX = 0;
		prevY = 0;
		prevZ = 0;
		started = 0;
	}
}

/* The tightropes' event: fdRopeUpdate for the tightropes, raising
   FIELD_RAISED_CUE_TIGHTROPE_ON and FIELD_RAISED_CUE_TIGHTROPE_OFF; the middle
   of a wobbling tightrope bobs up and down. */
void fdTightropeUpdate(void)
{
	FIELD_STATE	   *event = Ram_HeapSlots->fieldState;
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	TIGHTROPE_SLOT *slot;
	Sint32			i = 0;

	if(!syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS) &&
		event->blockedSteps > ROPE_PUSH_STEPS) {
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		if(party->movementMode == LEADER_MOVEMENT_WALK) {
			Sint32 tgt = fdFindNearbyTalkTarget(party->leaderActorId);

			if(tgt != -1) {
				for(i = 0; i < ROPE_SLOT_COUNT; i++) {
					if(state->slots[i].first == tgt ||
						state->slots[i].second == tgt)
						break;
				}
				if(i < ROPE_SLOT_COUNT &&
					fdRopeIsLeaderAtEnd(
						state->slots[i].first, state->slots[i].second))
					event->raisedCue = FIELD_RAISED_CUE_TIGHTROPE_ON;
			}
		} else if(party->movementMode == LEADER_MOVEMENT_TIGHTROPE) {
			event->raisedCue = FIELD_RAISED_CUE_TIGHTROPE_OFF;
		}
	}

	slot = state->slots;
	for(i = 0; i < ROPE_SLOT_COUNT; i++, slot++) {
		if(slot->wobbling != 0) {
			FIXED wobble = syMulFX(sySin(slot->phase), slot->amplitude);

			slot->phase += TRIG_TURN / 16;
			slot->amplitude = syMulFX(slot->amplitude, toFIXED(19) / 20);
			if(slot->amplitude < toFIXED(1) / 10) {
				slot->amplitude = 0;
				slot->wobbling = 0;
			}
			fdTightropeWobbleBetween(i, slot->first, slot->second, wobble);
		}
	}
}

/* Returns the first unused of the tightropes' 128 nodes, or NULL. */
TIGHTROPE_NODE *fdTightropeAllocNode(void)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	TIGHTROPE_NODE *node = state->nodes;
	Sint32			idx;

	for(idx = 0; idx < ROPE_NODE_COUNT; idx++) {
		if(node->kind == ROPE_NODE_FREE)
			return node;
		node++;
	}
	return NULL;
}

/* Lays tightrope slotNo straight between posts firstId and secondId, and shows
   the rope wound round both. Declared Sint32, as the scenes call it, though it
   returns no value; no caller reads one. */
Sint32 fdTightropePlace(Sint32 slotNo, Sint32 firstId, Sint32 secondId)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	ACTOR		   *first = fdActorLookup(firstId);
	ACTOR		   *second = fdActorLookup(secondId);

	if(first != NULL && second != NULL) {
		TIGHTROPE_SLOT *slot = &state->slots[slotNo];

		slot->first = firstId;
		slot->second = secondId;
		fdTightropeLay(slotNo, first->x.fixed, first->y.fixed + toFIXED(8),
			first->z.fixed, second->x.fixed, second->y.fixed + toFIXED(8),
			second->z.fixed);
		fdActorSelectAnimationStub(first, ANIM_POST_TIED);
		fdActorSelectAnimationStub(second, ANIM_POST_TIED);
	}
}

/* Builds tightrope index's nodes on the straight line between two points, one
   for each 6 pixels. */
Sint32 fdTightropeLay(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	Sint32			result = 0;
	Sint32			dx = toX - x;
	Sint32			sx = dx >> 8;
	Sint32			dy = toY - y;
	Sint32			sy = dy >> 8;
	Sint32			dz = toZ - z;
	Sint32			sz = dz >> 8;
	Sint32			segments =
		sySquartEntry(syMulFX(sx, sx) + syMulFX(sy, sy) + syMulFX(sz, sz)) / 6 +
		1;
	TIGHTROPE_NODE **link = &state->slots[idx].head;
	Sint32			 i;

	state->slots[idx].dir = (Uint16)syAtan(sz, sx);
	for(i = 0; i <= segments; i++) {
		TIGHTROPE_NODE *node = fdTightropeAllocNode();

		if(node == NULL) {
			result = -1;
			break;
		}
		node->slot = idx + 1;
		SET_POSITION(node, x + i * dx / segments, y + i * dy / segments,
			z + i * dz / segments);
		if(i == segments)
			node->kind = ROPE_NODE_UNCROSSABLE;
		else
			node->kind = ROPE_NODE_CROSSABLE;
		*link = node;
		link = &node->next;
	}
	*link = NULL;
	state->slots[idx].wobbling = 1;
	return result;
}

/* Lays tightrope index again between posts from and to, its middle raised
   height pixels (16.16). */
void fdTightropeWobbleBetween(Sint32 idx, Sint32 from, Sint32 to, FIXED height)
{
	ACTOR *a = fdActorLookup(from);
	ACTOR *b = fdActorLookup(to);

	if(a != NULL && b != NULL)
		fdTightropeLayBowed(idx, a->x.fixed, a->y.fixed + toFIXED(8),
			a->z.fixed, b->x.fixed, b->y.fixed + toFIXED(8), b->z.fixed,
			height);
}

/* fdRopeLayBowed for a tightrope: lays its nodes along the line between two
   points again, raised by half a sine height pixels (16.16) high. */
Sint32 fdTightropeLayBowed(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ, FIXED height)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	Sint32			dx = toX - x;
	Sint32			sx = dx >> 8;
	Sint32			dy = toY - y;
	Sint32			sy = dy >> 8;
	Sint32			dz = toZ - z;
	Sint32			sz = dz >> 8;
	Sint32			segments = sySquartEntry(syIWRAMMulFX(sx, sx) +
								   syIWRAMMulFX(sy, sy) + syIWRAMMulFX(sz, sz)) /
			6 +
		1;
	TIGHTROPE_NODE *node = state->slots[idx].head;
	Sint32			i;
	Sint32			kind = ROPE_NODE_CROSSABLE;

	for(i = 0; i <= segments; i++) {
		FIXED bow = syIWRAMMulFX(sySin(i * (TRIG_TURN / 2) / segments), height);

		SET_POSITION(node, x + i * dx / segments, y + i * dy / segments + bow,
			z + i * dz / segments);
		node->kind = kind;
		node = node->next;
	}
	return 0;
}

/* Unused: nothing refers to it. fdRopeCoil for a tightrope. */
UNUSED void fdTightropeCoil(Sint32 idx, Sint32 from, Sint32 to)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	ACTOR		   *a = fdActorLookup(from);
	ACTOR		   *b = fdActorLookup(to);

	if(a != NULL && b != NULL) {
		TIGHTROPE_SLOT *slot = &state->slots[idx];

		slot->first = from;
		slot->second = to;
		fdTightropeLayCoil(idx, a->x.fixed, a->y.fixed + toFIXED(8), a->z.fixed,
			b->x.fixed, b->y.fixed + toFIXED(8), b->z.fixed);
	}
}

/* fdRopeLayCoil for a tightrope. Only the unused fdTightropeCoil calls it, and
   it takes its nodes from the ropes' pool. */
Sint32 fdTightropeLayCoil(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	Sint32			result = 0;
	Sint32			dx = (toX - x) >> 8;
	Sint32			dy = (toY - y) >> 8;
	Sint32			dz = (toZ - z) >> 8;
	Sint32			length =
		sySquartEntry(syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz));
	Sint32			 segments = length / 6 + 1;
	TIGHTROPE_NODE **link = &state->slots[idx].head;
	Sint32			 ang = 0;
	FIXED			 radius = toFIXED(4);
	Sint32			 i;
	Sint32			 kind = ROPE_NODE_UNCROSSABLE;

	for(i = 0; i <= segments; i++) {
		TIGHTROPE_NODE *node = (TIGHTROPE_NODE *)fdRopeAllocNode();
		Uint16			step;

		if(node == NULL) {
			result = -1;
			break;
		}
		SET_POSITION(node, x + syMulFX(syCos(ang), radius), y,
			z + syMulFX(sySin(ang), radius));
		node->kind = kind;
		*link = node;
		step = syAtan(toFIXED(6), radius);
		ang += step;
		link = &node->next;
		radius += step * 4;
	}
	*link = NULL;
	return result;
}

extern Sint32 TightropeLashPhase;

/* Unused: nothing refers to it. fdRopeLash for a tightrope, without the short
   rope's fall. */
UNUSED void fdTightropeLash(Sint32 idx, Sint32 firstId, Sint32 secondId)
{
	ACTOR *first = fdActorLookup(firstId);
	ACTOR *second = fdActorLookup(secondId);

	if(first != NULL && second != NULL) {
		fdTightropeThrow(idx, first->x.fixed, first->y.fixed + toFIXED(8),
			first->z.fixed, second->x.fixed, second->y.fixed + toFIXED(8),
			second->z.fixed, sySin(TightropeLashPhase) << 2);
		TightropeLashPhase += TRIG_TURN / 16;
		sySynch(6);
		fdActorSelectAnimationStub(first, ANIM_POST_TIE);
		fdActorSelectAnimationStub(second, ANIM_POST_TIE);
	}
}

/* fdRopeThrow for a tightrope. */
Sint32 fdTightropeThrow(Sint32 idx, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	Sint32			dx = toX - x;
	Sint32			ex = dx >> 8;
	Sint32			dy = toY - y;
	Sint32			ey = dy >> 8;
	Sint32			dz = toZ - z;
	Sint32			ez = dz >> 8;
	Sint32			segments =
		sySquartEntry(syMulFX(ex, ex) + syMulFX(ey, ey) + syMulFX(ez, ez)) / 6 +
		1;
	Sint32			frm;
	TIGHTROPE_NODE *node = state->slots[idx].head;
	FIXED			sine;
	Sint32			height;
	FIXED			cosine;
	Sint32			lastX, lastY, lastZ;
	void		  **nodes;
	void		  **cursor;
	TIGHTROPE_SLOT *slot;
	Sint32			i;
	Uint16			ang;

	cursor = nodes = syHeapAlloc(512);
	if(segments >= 0) {
		i = segments + 1;
		do {
			i += -1;
			*cursor++ = node;
			node = node->next;
		} while(i != 0);
	}
	ang = syAtan(ex, ey);
	sine = sySin(ang);
	cosine = syCos(ang);
	for(frm = 1; frm <= segments * 2; frm++) {
		for(i = segments; i >= 0; i--) {
			node = nodes[i];
			node->slot = idx + 1;
			if(i == segments) {
				FIXED ofsX;
				FIXED ofsY;

				height = syMulFX(sySin(frm * (TRIG_TURN / 4) / i), toFIXED(8));
				ofsX = syMulFX(height, cosine);
				lastX = node->x = x + dx * frm / 2 / i + ofsX;
				ofsY = syMulFX(height, sine);
				lastY = node->y = y + dy * frm / 2 / i + ofsY;
				lastZ = node->z = z + dz * frm / 2 / i;
			} else {
				ex = (lastX - node->x) >> 8;
				ey = (lastY - node->y) >> 8;
				ez = (lastZ - node->z) >> 8;
				height = sySquartEntry(
					syMulFX(ex, ex) + syMulFX(ey, ey) + syMulFX(ez, ez));

				if(height <= 6)
					break;
				lastX = node->x = lastX + (node->x - lastX) * 6 / height;
				lastY = node->y = lastY + (node->y - lastY) * 6 / height;
				lastZ = node->z = lastZ + (node->z - lastZ) * 6 / height;
				node->kind = ROPE_NODE_CROSSABLE;
			}
		}
		sySynch(1);
	}
	slot = &state->slots[idx];
	slot->wobbling = 1;
	slot->amplitude = toFIXED(4);
	slot->phase = DEGtoANG(90);
	syHeapRelease(nodes);
	return 0;
}

extern Sint32 TightropeUntiePhase;

/* Unused: nothing refers to it. fdRopeUntie for a tightrope. */
UNUSED void fdTightropeUntie(Sint32 idx, Sint32 firstId, Sint32 secondId)
{
	TIGHTROPE_WORK *state;
	ACTOR		   *first;
	ACTOR		   *second;

	state = TIGHTROPE_STATE;
	first = fdActorLookup(firstId);
	second = fdActorLookup(secondId);

	if(first != NULL && second != NULL) {
		state->recoilFrom = first;
		state->recoilTo = second;
		fdTightropeRecoil(idx, first->x.fixed, first->y.fixed + toFIXED(8),
			first->z.fixed, second->x.fixed, second->y.fixed + toFIXED(8),
			second->z.fixed, sySin(TightropeUntiePhase) << 2);
		TightropeUntiePhase += TRIG_TURN / 16;
	}
}

/* fdRopeRecoil for a tightrope. */
Sint32 fdTightropeRecoil(Sint32 slot, Sint32 x1, Sint32 y1, Sint32 z1,
	Sint32 x2, Sint32 y2, Sint32 z2, Sint32 wave)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	Sint32			wobbling = 0;
	TIGHTROPE_NODE *node;
	Sint32			dx, dy, dz;
	Sint32			dist;
	Sint32			cnt;
	Sint32			frm;
	Sint32			i;
	Sint32			ang;
	Sint32			x, y, z;
	void		  **nodes;
	void		  **entry;

	dx = (x2 - x1) >> 8;
	dy = (y2 - y1) >> 8;
	dz = (z2 - z1) >> 8;
	dist = syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz);
	cnt = sySquartIWRAM(dist) / 6 + 1;
	node = state->slots[slot].head;
	entry = nodes = syHeapAlloc(512);
	state->slots[slot].wobbling = wobbling;
	if(cnt >= 0) {
		i = cnt + 1;
		do {
			i += -1;
			*entry++ = node;
			node = node->next;
		} while(i != 0);
	}
	fdActorSelectAnimationStub(state->recoilFrom, ANIM_POST_UNTIE);
	fdActorSelectAnimationStub(state->recoilTo, ANIM_POST_UNTIE);
	sySynch(4);
	for(frm = 0; frm <= cnt; frm++) {
		ang = frm * (TRIG_TURN / 16);
		dist = toFIXED(4);
		for(i = 0; i <= cnt; i++) {
			node = nodes[i];
			node->slot = 0;
			if(i <= frm) {
				x = node->x = x1 + syMulFX(syCos(ang), dist);
				y = node->y = y1;
				z = node->z = z1 + syMulFX(sySin(ang), dist);
				node->kind = ROPE_NODE_UNCROSSABLE;
				{
					Uint16 step = syAtan(toFIXED(6), dist);

					ang += step;
					dist += step * 4;
				}
			} else {
				dx = (x - node->x) >> 8;
				dy = (y - node->y) >> 8;
				dz = (z - node->z) >> 8;
				dist = syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz);
				dist = sySquartIWRAM(dist);
				if(dist <= 6)
					break;
				x = node->x = x + (node->x - x) * 6 / dist;
				y = node->y = y + (node->y - y) * 6 / dist;
				node->z = z + (node->z - z) * 6 / dist;
				node->kind = ROPE_NODE_UNCROSSABLE;
				z = node->z;
			}
		}
		sySynch(1);
	}
	syHeapRelease(nodes);
	return 0;
}

extern Sint32 TightropeDebugSwinging;

/* Frees tightrope index's nodes; it is no longer drawn. */
void fdTightropeRelease(Sint32 idx)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	TIGHTROPE_NODE *node;

	for(node = state->slots[idx].head; node != NULL; node = node->next)
		node->kind = ROPE_NODE_FREE;
	state->slots[idx].head = NULL;
}

/* Stops tightrope index's wobble and marks its nodes not to be walked. */
void fdTightropeSettle(Sint32 idx)
{
	TIGHTROPE_WORK *state = TIGHTROPE_STATE;
	TIGHTROPE_NODE *node;

	for(node = state->slots[idx].head; node != NULL; node = node->next)
		node->kind = ROPE_NODE_UNCROSSABLE;
	state->slots[idx].wobbling = 0;
}

/* Unused: nothing refers to it. fdRopeDebugSwing for a tightrope. */
UNUSED void fdTightropeDebugSwing(ACTOR *actor)
{
	SPR *spr = actor->spr;

	spr->ofsX = -3;
	spr->ofsY = 12;
	if(PerWork.repeat & KEY_L)
		TightropeDebugSwinging = 1;
	if(TightropeDebugSwinging != 0)
		fdTightropeWobbleBetween(0, DEBUG_FIRST_POST, DEBUG_SECOND_POST, 0);
}

/* The four pictures of each kind of hanging link, a vine's and a chain's: top,
   upper, lower and bottom. */
extern const Uint8 HangingVine_VineTopPicture[], HangingVine_VineUpperPicture[],
	HangingVine_VineLowerPicture[];
extern const Uint8 HangingVine_VineBottomPicture[],
	HangingVine_ChainTopPicture[], HangingVine_ChainMiddlePicture[];

/* The bytes of a link's picture, 16 by 16 pixels of 256 colours. */
#define VINE_LINK_PICTURE_SIZE (16 * 16)

/* HANGING_VINE_LINK.kind: a free link, the anchor (not drawn), then a link
   showing each of the four pictures, in their order in VRAM. */
typedef enum HANGING_VINE_LINK_KIND {
	VINE_LINK_FREE,
	VINE_LINK_ANCHOR,
	VINE_LINK_TOP,
	VINE_LINK_UPPER,
	VINE_LINK_LOWER,
	VINE_LINK_BOTTOM
} HANGING_VINE_LINK_KIND;

/* A vine's swing: its top link stays put, the first link below it sways one and
   a half times the swing either way, and each link below that half a swing more
   (fdHangingVineUpdate). A vine left alone eases to the first, and while the
   leader stands beside it to the second. */
#define VINE_SWING_ALONE  (TRIG_TURN / 256)
#define VINE_SWING_BESIDE (TRIG_TURN / 2048)
/* Each link sways this far behind the one above it, a turn being TRIG_TURN. */
#define VINE_LINK_LAG	  800

/* A vine link's depth slot bias, as ROPE_DEPTH a rope's. */
#define HANGING_VINE_DEPTH 34

/* The hanging vines (HEAP_SLOT_HANGING_VINE): eight slots, each seven links
   hung below an anchor actor, as the leafy vine that hangs down a wall in Gaia
   Rock's altar room, hanging down the wall to just over the leader's head. A
   vine sways, swings less while the leader stands beside it
   (fdHangingVineUpdate), and once Whirlwind blows on it, swings the leader
   across (fdHangingVineSwing, which fdPsynergyWhirlwind calls on its
   target). */

/* Starts the hanging vines: takes and clears their state, loads the vine's
   pictures for an anchor of SPRITE_SPROUTING_LEAVES, else the chain's, and adds
   the events fdHangingVineDraw and fdHangingVineUpdate. */
HANGING_VINE_WORK *fdHangingVineInit(Sint32 sprId)
{
	HANGING_VINE_WORK *state;
	Uint8			  *pictures;
	Sint32			   size;

	state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_HANGING_VINE, sizeof(HANGING_VINE_WORK));
	size = 4 * VINE_LINK_PICTURE_SIZE;
	pictures = syHeapAlloc(size);
	DMA_FILL32(0, state, sizeof(HANGING_VINE_WORK));
	state->vramEntry = syVRAMCacheFindFreeEntry();
	if(sprId == SPRITE_SPROUTING_LEAVES) {
		syDecompressTaggedLZ(HangingVine_VineTopPicture, pictures);
		syDecompressTaggedLZ(
			HangingVine_VineUpperPicture, pictures + VINE_LINK_PICTURE_SIZE);
		syDecompressTaggedLZ(HangingVine_VineLowerPicture,
			pictures + 2 * VINE_LINK_PICTURE_SIZE);
		syDecompressTaggedLZ(HangingVine_VineBottomPicture,
			pictures + 3 * VINE_LINK_PICTURE_SIZE);
		syVRAMCacheLoad(state->vramEntry, size, pictures);
	} else {
		syDecompressTaggedLZ(HangingVine_ChainTopPicture, pictures);
		syDecompressTaggedLZ(
			HangingVine_ChainMiddlePicture, pictures + VINE_LINK_PICTURE_SIZE);
		syDecompressTaggedLZ(HangingVine_ChainMiddlePicture,
			pictures + 2 * VINE_LINK_PICTURE_SIZE);
		syDecompressTaggedLZ(HangingVine_ChainBottomPicture,
			pictures + 3 * VINE_LINK_PICTURE_SIZE);
		syVRAMCacheLoad(state->vramEntry, size, pictures);
	}
	sySetEvent(fdHangingVineDraw, EVENT_PASS_TICK);
	sySetEvent(fdHangingVineUpdate, EVENT_PASS_TICK + 15);
	syHeapRelease(pictures);
	return state;
}

/* The vines' drawing event: each link's picture at the midpoint between it and
   the link above, turned along the vine, for the links on screen. */
void fdHangingVineDraw(void)
{
	HANGING_VINE_WORK *state = HANGING_VINE_STATE;
	Sint32			   i;
	Uint32			   ang;
	Sint32			   prevX = 0;
	Sint32			   prevY = 0;
	Uint32	   tiles = VramCacheEntries[state->vramEntry].ofs / TILE_SIZE_4BPP;
	Sint32	   prevZ = 0;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	Sint32	   viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	Sint32	   height = map->origin[1];
	AFFINE_TRANSFORM   transform;
	HANGING_VINE_LINK *link;

	for(i = 0; i < ROPE_SLOT_COUNT; i++) {
		for(link = state->slots[i].root; link != NULL; link = link->next) {
			Sint32 x = link->x;
			Sint32 y = link->y;
			Sint32 z = link->z;

			if(link->kind > VINE_LINK_ANCHOR) {
				Sint32 dz = prevZ - z;
				Sint32 absZ = dz >= 0 ? dz : z - prevZ;
				Sint32 dy = prevY - y;
				Sint32 screenX, screenY;
				Uint32 tile;

				if(dy >= 0 ? absZ < dy : absZ < y - prevY)
					ang = (Uint16)syAtan(prevX - x, dy);
				else
					ang = (Uint16)syAtan(prevX - x, dz);
				tile = tiles +
					(link->kind - VINE_LINK_TOP) *
						(VINE_LINK_PICTURE_SIZE / TILE_SIZE_4BPP);
				screenX = (prevX + x) / 2 - viewX;
				prevY = (prevY + y) / 2 - height;
				prevZ = (prevZ + z) / 2 - viewZ - height;
				screenY = prevZ - prevY;
				if(screenX > -toFIXED(32) &&
					screenX < toFIXED(DISPLAY_WIDTH + 32) &&
					screenY > -toFIXED(32) &&
					screenY < toFIXED(DISPLAY_HEIGHT + 64)) {
					Sint32 sx = (FIXEDtoINT(screenX) - 8) & OBJ_X_MASK;
					Sint32 sy = (FIXEDtoINT(screenY) - 8) & OBJ_Y_MASK;
					Sint32 depth =
						FIXEDtoINT(prevY + prevZ) + HANGING_VINE_DEPTH;
					ANIMATION_SPR_PART *spr = &link->spr;

					((Uint32 *)spr)[0] = 0;
					((Uint32 *)&link->spr)[1] =
						sy | OBJ_X(sx) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
					((Uint32 *)&link->spr)[2] = tile | OBJ_PRIORITY(2);
					if(ang != 0) {
						transform.ang = ang;
						transform.scaleX = AFFINE_SCALE_ONE;
						transform.scaleY = AFFINE_SCALE_ONE;
						link->spr.affine = 1;
						link->spr.affineIdx =
							syOAMMatrixAllocFromTransform(&transform);
					}
					link->ang = ang;
					syOAMAddSprAtDepthClamped(spr, depth);
				}
			}
			prevX = x;
			prevY = y;
			prevZ = z;
		}
		prevX = 0;
		prevY = 0;
		link = state->slots[i].root;
		prevZ = 0;
		if(link != NULL)
			link->ang = ang;
	}
}

#undef VINE_LINK_PICTURE_SIZE

/* The vines' event: sways each active vine's links below its anchor, the swing
   easing towards VINE_SWING_BESIDE while the leader stands beside the vine,
   else towards VINE_SWING_ALONE. */
void fdHangingVineUpdate(void)
{
	HANGING_VINE_WORK *state = HANGING_VINE_STATE;
	HANGING_VINE_SLOT *slot;
	ACTOR			  *leader;
	Sint32			   x, y, z;
	HANGING_VINE_LINK *link;
	Sint32			   i;
	Sint32			   tgt;
	Sint32			   swing;
	Sint32			   sway;
	Sint32			   radius;
	Sint32			   ang;
#if FIX_VINE_SLOT_COUNTER
	Sint32 slotIndex;
#endif

	i = 0;
	leader = fdActorGetById(fdPartyGetLeaderActor());
	tgt = VINE_SWING_ALONE;
	for(slot = state->slots; i < ROPE_SLOT_COUNT; i++, slot++) {
		if(slot->root == 0 || slot->active == 0)
			continue;

		slot->phase += TRIG_TURN / 128;
		link = ((HANGING_VINE_LINK *)slot->root)->next;
		x = link->x;
		y = link->y;
		z = link->z;
		swing = slot->swing;

		/* The leader stands beside the vine: within 8 pixels across and 16 deep
		   of its top link, and between 8 above and 80 below it. */
		if((x - leader->x.fixed >= 0 ? x - leader->x.fixed < toFIXED(8)
									 : leader->x.fixed - x < toFIXED(8)) &&
			(Uint32)(z - leader->z.fixed + (toFIXED(16) - 1)) <=
				toFIXED(16) - 2 &&
			y + toFIXED(8) > leader->y.fixed &&
			y - toFIXED(80) < leader->y.fixed)
			tgt = VINE_SWING_BESIDE;

		/* Hang the five links below the top one. */
		/* BUG: the link loop reuses the slot counter and leaves it at 6, so
		   past the first active vine the loop goes on only while the next slot
		   holds an active vine too. */
		link = link->next;
#if FIX_VINE_SLOT_COUNTER
		slotIndex = i;
#endif
		for(i = 1, sway = swing; i <= 5; i++) {
			radius = i * slot->spacing;
			ang = syMulFX(
				sySin(slot->phase - i * VINE_LINK_LAG), swing + sway / 2);
			link->x = x - syMulFX(sySin(ang), radius);
			sway += swing;
			link->y = y - syMulFX(syCos(ang), radius);
			link->z = z;
			link = link->next;
		}
#if FIX_VINE_SLOT_COUNTER
		i = slotIndex;
#endif

		/* Ease the swing towards the target. */
		if(slot->swing > tgt)
			slot->swing = syMulFX(slot->swing, toFIXED(99) / 100);
		else
			slot->swing = syMulFX(slot->swing, toFIXED(111) / 100);
	}
}

/* Returns the first of HANGING_VINE_STATE's eight slots with no chain, or
   NULL. */
HANGING_VINE_SLOT *fdHangingVineFindFreeSlot(void)
{
	HANGING_VINE_WORK *state = HANGING_VINE_STATE;
	HANGING_VINE_SLOT *found = NULL;
	Sint32			   idx = 0;

	for(idx = 0; idx < ROPE_SLOT_COUNT; idx++) {
		if(state->slots[idx].root == NULL) {
			found = &state->slots[idx];
			break;
		}
	}
	return found;
}

/* Returns the vine slot among HANGING_VINE_STATE's eight that holds actor id,
   or NULL. */
HANGING_VINE_SLOT *fdHangingVineFindSlot(Sint32 id)
{
	HANGING_VINE_WORK *state = HANGING_VINE_STATE;
	HANGING_VINE_SLOT *found = NULL;
	Sint32			   idx = 0;

	for(idx = 0; idx < ROPE_SLOT_COUNT; idx++) {
		if(state->slots[idx].id == id) {
			found = &state->slots[idx];
			break;
		}
	}
	return found;
}

/* Returns the first unused of the vines' 128 links, or NULL. */
HANGING_VINE_LINK *fdHangingVineAllocLink(void)
{
	HANGING_VINE_WORK *state = HANGING_VINE_STATE;
	HANGING_VINE_LINK *link = state->links;
	Sint32			   idx;

	for(idx = 0; idx < ROPE_NODE_COUNT; idx++) {
		if(link->kind == VINE_LINK_FREE)
			return link;
		link++;
	}
	return NULL;
}

/* Hangs a vine of seven links from the anchor actor objectId, clearing the five
   metatiles it hangs over, starting the vines first if they are not running;
   with grow set, the vine drops into place over 28 ticks. */
void fdHangingVineHang(Sint32 objectId, Sint32 grow)
{
	ACTOR			   *actor;
	MAP_STATE		   *map;
	HANGING_VINE_SLOT  *slot;
	HANGING_VINE_LINK  *link;
	HANGING_VINE_LINK **tail;
	MAP_METATILE	   *metatiles;
	MAP_METATILE	   *metatile;
	Sint32				i;
	Sint32				x;
	FIXED				top;
	Sint32				z;
	Sint32				y;
	Sint32				frm;
	Sint32				last;
	Sint32				sprId;
	Sint32				running;

	actor = fdActorGetById(objectId);
	running = Ram_HeapSlots->hangingVine;
	sprId = SPRITE_SPROUTING_LEAVES;
	map = Ram_HeapSlots->mapGrid;
	if(running == 0) {
		if(actor->spr != 0 && ((SPR *)actor->spr)->entries[0] != 0)
			sprId = ((SPR *)actor->spr)->entries[0]->anmId;
		fdHangingVineInit(sprId);
	}
	if(fdHangingVineFindSlot(objectId) != 0)
		return;
	slot = fdHangingVineFindFreeSlot();
	if(slot == 0)
		return;

	/* Clear the collision codes of the five metatiles the vine hangs over. */
	x = FIELD_METATILE(actor->x.fixed);
	z = FIELD_METATILE(actor->z.fixed - actor->y.fixed);
	metatiles = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
	for(i = 0; i < 5; i++) {
		z++;
		metatile = metatiles;
		metatile += x + z * MAPGRID_SIZE;
		metatile->collisionCode = 0;
	}

	x = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	y = actor->y.fixed & FIELD_METATILE_MASK;
	top = y + toFIXED(8);
	z = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(16);
	fdActorSelectAnimationStub((ACTOR *)actor, 0);
	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	actor->z.fixed += toFIXED(16);
	actor->y.fixed -= toFIXED(64);
	fdActorSetSprFlagsStub(actor, 0);

	/* Hang the links 14 pixels apart, the upper and lower pictures taking turns
	   between the top and the bottom. */
	last = 6;
	tail = (HANGING_VINE_LINK **)&slot->root;
	for(i = 0; i <= last && (link = fdHangingVineAllocLink()) != 0; i++) {
		SET_POSITION(link, x, y + toFIXED(15) - i * toFIXED(14), z);
		if(i == 0)
			link->kind = VINE_LINK_ANCHOR;
		else if(i == 1)
			link->kind = VINE_LINK_TOP;
		else if(i == last)
			link->kind = VINE_LINK_BOTTOM;
		else
			link->kind = i % 2 + VINE_LINK_UPPER;
		*tail = link;
		tail = &link->next;
	}

	if(grow == 0) {
		for(i = 0, link = slot->root; i <= last && link != 0; i++)
			link = link->next;
	} else {
		for(frm = 1; frm <= 28; frm++) {
			i = 0;
			link = slot->root;
			if(i <= last && link != 0) {
				Sint32 drop = -(frm * toFIXED(0.5));

				do {
					SET_POSITION(link, x, top - drop / 2, z);
					drop += toFIXED(frm);
					i++;
					link = link->next;
				} while(i <= last && link != 0);
			}
			sySynch(1);
		}
	}
	slot->active = 1;
	slot->phase = 0;
	slot->swing = VINE_SWING_ALONE;
	slot->id = objectId;
	slot->spacing = toFIXED(14);
}

/* The half turn on the leader's side of the anchor: 0 on its right, 1 on its
   left. */
#define SIDE_TURN(side) ((side) * (DEGtoANG(180)))

/* A vine's phase within one turn, its low byte dropped: a phase grows by
   TRIG_TURN / 128 a tick and never wraps. */
#define VINE_PHASE_MASK (TRIG_ANGLE_MASK & ~0xff)

/* A vine's swing, as VINE_SWING_ALONE: wound up a step a tick as Whirlwind
   blows on it, while the leader hangs on, and while it carries the leader
   across. */
#define VINE_SWING_STEP		(TRIG_TURN / 1024)
#define VINE_SWING_HELD		(TRIG_TURN * 3 / 128)
#define VINE_SWING_CARRYING (TRIG_TURN / 32)

/* Whirlwind on the vine hanging from actor id: the vine swings out to the
   leader's side and the leader takes hold. While the leader hangs on, B, Down
   or the direction away from the vine lets go; any other button leaps, and the
   vine carries the leader across to the far side. */
void fdHangingVineSwing(Sint32 id)
{
	Sint32			   rootX, rootY;
	Sint32			   motionFlg;
	SPR				  *heldSpr;
	Sint32			   side = 0;
	ACTOR			  *leader = fdActorGetById(fdPartyGetLeaderActor());
	ACTOR			  *anchor = fdActorGetById(id);
	HANGING_VINE_SLOT *slot;
	SPR				  *spr;
	Sint32			   swing;
	Sint32			   start;
	Sint32			   dir;
	Uint16			   letGo;
	FIXED			   radius;
	Sint32			   steps;
	Sint32			   ang;
	FIXED			   sine;
	Sint32			   i;

	if(leader == NULL || anchor == NULL)
		return;
	motionFlg = leader->motionFlg;
	heldSpr = leader->spr;
	if(leader->x.fixed < anchor->x.fixed)
		side = 1;
	slot = fdHangingVineFindSlot(id);
	if(slot == NULL)
		return;

	start = slot->swing;
	if((slot->phase ^ SIDE_TURN(side)) & (DEGtoANG(180))) {
		for(swing = start; swing >= 0; swing -= VINE_SWING_STEP) {
			slot->swing = swing;
			sySynch(1);
		}
		start = 0;
	}
	for(swing = start; swing <= VINE_SWING_HELD; swing += VINE_SWING_STEP) {
		slot->swing = swing;
		slot->phase = SIDE_TURN(side) + DEGtoANG(90);
		sySynch(1);
	}
	while(
		(slot->phase & VINE_PHASE_MASK) != SIDE_TURN(side ^ 1) + DEGtoANG(90)) {
		slot->swing = VINE_SWING_HELD;
		sySynch(1);
	}

	spr = leader->spr;
	dir = 1;
	if(side) {
		letGo = KEY_DOWN | KEY_LEFT | KEY_B;
		dir -= 2;
	} else
		letGo = KEY_DOWN | KEY_RIGHT | KEY_B;
	slot->active = 1;
	fdActorSelectAnimationStub(leader, ANIM_LEADER_HANG);
	while(1) {
		/* The vine bounces under the leader twice in 64 ticks. */
		slot->phase = SIDE_TURN(side ^ 1) + DEGtoANG(90);
		switch(TickCount & 63) {
		case 0:
		case 16:
			slot->swing = VINE_SWING_HELD + TRIG_TURN / 2048;
			slot->spacing = toFIXED(29) / 2;
			spr->rotation = dir * (TRIG_TURN / 64);
			break;
		case 1:
		case 2:
		case 17:
		case 18:
			slot->swing += TRIG_TURN / 4096;
			slot->spacing += toFIXED(1) / 4;
			spr->rotation += dir * (TRIG_TURN / 128);
			break;
		default:
			if(slot->swing > VINE_SWING_HELD)
				slot->swing -= TRIG_TURN / 2048;
			else
				slot->swing = VINE_SWING_HELD;
			if(slot->spacing > toFIXED(14))
				slot->spacing -= toFIXED(1) / 32;
			if(spr->rotation != 0)
				spr->rotation -= dir * (TRIG_TURN / 512);
			break;
		}
		if(PerWork.push & letGo) {
			spr->rotation = 0;
			slot->active = 1;
			return;
		}
		if(PerWork.push != 0)
			break;
		sySynch(1);
	}

	spr->rotation = 0;
	slot->active = 0;
	fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
	sySynch(6);
	sySndPlayStub(SE_JUMP);
	fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAP);
	leader->speedLimit = toFIXED(3);
	leader->acceleration = toFIXED(2);
	leader->velY = toFIXED(4);
	leader->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
	fdActorSetSprFlagsStub(leader, 0);
	sySynch(5);
	fdActorSelectAnimationStub(leader, ANIM_LEADER_SWING);
	leader->motionFlg = 0;
	rootX = ((HANGING_VINE_LINK *)slot->root)->next->x;
	rootY = ((HANGING_VINE_LINK *)slot->root)->next->y;
	radius = toFIXED(65);
	steps = 8;
	while((slot->phase & VINE_PHASE_MASK) !=
		SIDE_TURN(side) + TRIG_TURN * 7 / 32) {
		if(steps != 0) {
			radius += toFIXED(5) / 16;
			slot->spacing += toFIXED(1) / 16;
			steps--;
		}
		slot->swing = VINE_SWING_CARRYING;
		slot->active = 1;
		/* The leader hangs where the bottom link sways. */
		sine = sySin(slot->phase - 5 * VINE_LINK_LAG);
		ang = syIWRAMMulFX(sine, slot->swing + slot->swing * 5 / 2);
		leader->x.fixed = rootX - syIWRAMMulFX(sySin(ang), radius);
		leader->y.fixed = rootY - syIWRAMMulFX(syCos(ang), radius);
		leader->tgtX = leader->x.fixed;
		leader->tgtY = leader->y.fixed;
		heldSpr->rotation = ang;
		sySynch(1);
	}
	slot->active = 1;
	fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAP);
	leader->motionFlg = motionFlg & ~ACTOR_FOLLOWS_TERRAIN;
	leader->velY = toFIXED(4);
	for(i = 0; i < 8; i++) {
		slot->spacing -= toFIXED(1) / 16;
		sySynch(1);
	}
	slot->spacing = toFIXED(14);
	heldSpr->rotation = 0;
	fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
	fdActorSetSprFlagsStub(leader, SPRITE_FLAG_SHADOW);
	leader->motionFlg = motionFlg;
	sySynch(6);
	slot->active = 1;
}

/* Hangs a vine at once from high above actor id down to its feet
   (fdHangingVineHang): an actor that is not moving is lifted and moved 16
   pixels north first, which fdHangingVineHang undoes; a moving one has its vine
   only when force is set. */
void fdHangingVinePlace(Sint32 id, Sint32 force)
{
	ACTOR *actor = fdActorGetById(id);

	if(actor != NULL) {
		if(actor->motionFlg == 0) {
			actor->z.fixed -= toFIXED(16);
			actor->y.fixed += toFIXED(64);
			fdHangingVineHang(id, 0);
		} else if(force) {
			fdHangingVineHang(id, 0);
		}
	}
}

#undef SIDE_TURN
