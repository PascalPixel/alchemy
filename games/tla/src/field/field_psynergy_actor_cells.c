/* @file */
/* Actors standing as blocks in the map: their cells tagged and the tags */
/* cleared, the actor at a position, and effect sprites filled and queued. */
/* Part of field_psynergy.c. */

/* A 16.16 height in the 8-pixel steps a shape entry's height counts. */
#define HEIGHT_STEPS(height) (FIXEDtoINT(height) >> 3)
/* A block's height, 32 pixels, in those steps. */
#define BLOCK_HEIGHT_STEPS 4

/* Makes each listed actor (ids, ended by ACTOR_LIST_END) a block in the map: */
/* its cell's shape is tagged BLOCK_HEIGHT_STEPS above the ground, so the party
 */
/* can stand on it, its cell is marked MAP_CELL_JUMP, a gap to jump onto from */
/* another block (the old mark kept in the actor), and the overhead layer's */
/* cell is a wall. A */
/* leader below the new ground is lifted onto it. Scenes call it as they */
/* start, for the blocks Move pushes. */
void fdActorPlaceBlocks(u16 *actors) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_CELL *blocks = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
	u8 *grid = MapShapeGrid;
	ACTOR *leader;
	s32 height;

	for(; *actors != ACTOR_LIST_END; actors++) {
		ACTOR *actor = fdActorGetById(*actors);
		u32 key = fdMapGridGetTerrainKindStub(actor->layer, actor->x.fixed,
											  actor->z.fixed);
		s32 level = HEIGHT_STEPS(fdMapGridGetTerrainHeightStub(
			actor->layer, actor->x.fixed, actor->z.fixed));
		s32 slot = fdActorGetCellIndex(actor);
		MAP_CELL *cells = map->layers[actor->layer].cells;
		/* The layer's row of the shape grid, as an integer sum with its index
		 */
		/* first. */
		u8 *shapes =
			(u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
		MAP_CELL *cell;
		s32 tag;
		u8 flags;

		actor->motionFlg = 0;
		actor->collisionFlg = 0;
		fdActorSetSprFlagsStub(actor, 0);
		shapes += slot;
		tag =
			fdMapShapeFindOrAdd(actor->layer, key, level + BLOCK_HEIGHT_STEPS);
		cell = &cells[slot];
		flags = cell->attributeB;
		actor->sceneWord = flags;
		cell->attributeB = flags | MAP_CELL_JUMP;
		*shapes = tag;
		cell = &blocks[slot];
		cell->collisionCode = CELL_BEHAVIOR_WALL;
	}
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	height = fdMapGridGetTerrainHeightStub(leader->layer, leader->x.fixed,
										   leader->z.fixed);
	if(leader->y.fixed < height)
		leader->y.fixed = leader->groundHeight = height;
}

/* Takes the block about to move out of the map: the field effect's target */
/* while one runs, else the actor ahead of the leader, if it is one of the */
/* listed actors. Its cell goes back to the ground and its old mark, and the */
/* event state remembers it for fdActorPlaceMovedBlock. */
void fdActorLiftTargetBlock(u16 *actors) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	ACTOR *target;

	if(state != NULL && event->psynergy != 0)
		target = state->tgt;
	else
		target = fdLeaderFindActorAhead();
	event->movedBlock = NULL;

	for(; *actors != ACTOR_LIST_END; actors++) {
		if(target == fdActorGetById(*actors)) {
			MAP_STATE *map = Ram_HeapSlots->mapGrid;
			u8 *grid = MapShapeGrid;
			MAP_CELL *cells = map->layers[target->layer].cells;
			MAP_CELL *blocks = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
			u8 *shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
			u32 key = fdMapGridGetTerrainKindStub(
				target->layer, target->x.fixed, target->z.fixed);
			s32 level = fdMapGridGetTerrainHeightStub(
				target->layer, target->x.fixed, target->z.fixed);
			s32 slot = fdActorGetCellIndex(target);
			s32 tag;

			shapes += slot;
			tag = fdMapShapeFindOrAdd(target->layer, key,
									  HEIGHT_STEPS(level) - BLOCK_HEIGHT_STEPS);
			cells += slot;
			blocks += slot;
			((SHAPE_CELL *)shapes)->shape = tag;
			cells->attributeB = target->sceneWord;
			blocks->collisionCode = 0;
			event->movedBlock = target;
		}
	}
}

/* Puts the block fdActorLiftTargetBlock remembered back into the map at the */
/* cell it has moved to, as fdActorPlaceBlocks places each. */
void fdActorPlaceMovedBlock(void) {
	ACTOR *target = FieldStateWork->movedBlock;

	if(target != NULL) {
		MAP_STATE *map = Ram_HeapSlots->mapGrid;
		u8 *grid = Ram_MapShapeGrid;
		MAP_CELL *cells = map->layers[target->layer].cells;
		MAP_CELL *blocks = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
		u8 *shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
		u32 key = fdMapGridGetTerrainKindStub(target->layer, target->x.fixed,
											  target->z.fixed);
		s32 level = fdMapGridGetTerrainHeightStub(
			target->layer, target->x.fixed, target->z.fixed);
		s32 slot = fdActorGetCellIndex(target);
		s32 tag;

		shapes += slot;
		tag = fdMapShapeFindOrAdd(target->layer, key,
								  HEIGHT_STEPS(level) + BLOCK_HEIGHT_STEPS);
		((SHAPE_CELL *)shapes)->shape = tag;
		cells += slot;
		target->sceneWord = cells->attributeB;
		cells->attributeB |= MAP_CELL_JUMP;
		blocks += slot;
		blocks->collisionCode = CELL_BEHAVIOR_WALL;
	}
}

/* Returns the actor one cell, or failing that two cells, ahead of the leader */
/* at the leader's height, or NULL. */
ACTOR *fdLeaderFindActorAhead(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIXED position[3];
	/* The leader's facing rounded to the nearest quarter turn, in sixteenths.
	 */
	s32 angle = (leader->facing / FACING_STEP + 2) & 0xc;
	ACTOR *found;

	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle * FACING_STEP, position);
	found = fdActorFindAtCell(position, 1);
	if(found != NULL)
		return found;
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	syVectorAddPolarOffset(toFIXED(32), angle * FACING_STEP, position);
	found = fdActorFindAtCell(position, 1);
	if(found != NULL)
		return found;
	return NULL;
}

/* Returns the first of the placed actors (not the treasure spots) on */
/* position's cell, at its height too when checkY is set, or NULL. */
ACTOR *fdActorFindAtCell(s32 *position, s32 checkY) {
	u32 i;

	for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
		FIELD_STATE *event = Ram_HeapSlots->fieldState;
		ACTOR *actor = event->actors[i];

		if(checkY) {
			if(FIELD_CELL(position[0]) == FIELD_CELL(actor->x.fixed) &&
			   position[1] / FIXED_ONE == actor->y.fixed / FIXED_ONE &&
			   FIELD_CELL(position[2]) == FIELD_CELL(actor->z.fixed))
				return actor;
		}
		else {
			if(FIELD_CELL(position[0]) == FIELD_CELL(actor->x.fixed) &&
			   FIELD_CELL(position[2]) == FIELD_CELL(actor->z.fixed))
				return actor;
		}
	}
	return NULL;
}

/* The width and height of each OAM shape and size, in pixels, by the shape */
/* times 4 plus the size. */
extern const u8 Oam_ShapeSizes[][2];
/* Where an attr01 word holds a sprite's size (attribute 1's top two bits) and
 */
/* shape (attribute 0's). */
#define OBJ_SIZE_SHIFT	30
#define OBJ_SHAPE_SHIFT 14

/* Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix, */
/* 256 colours), the tiles, unscaled and unrotated at depth 255, and the */
/* picture's size from its shape, pivoted at (pivotX, pivotY). */
void fdEffectSprInit(EFFECT_SPR *sprite, s32 pivotX, s32 pivotY, u32 attributes,
					 s32 tile) {
	u32 *words;
	s32 shape;

	if(sprite == NULL)
		return;
	syWordsFillEntry(sprite, sizeof(*sprite), 0);
	sprite->scaleX = FIXED_ONE;
	sprite->scaleY = FIXED_ONE;
	sprite->depth = OAM_DEPTH_FRONT;
	sprite->pivotX = pivotX;
	sprite->pivotY = pivotY;
	shape = ((attributes >> OBJ_SIZE_SHIFT) |
			 (attributes >> (OBJ_SHAPE_SHIFT - 2))) &
			0xf;
	sprite->width = Oam_ShapeSizes[shape][0];
	sprite->height = Oam_ShapeSizes[shape][1];
	words = (u32 *)&sprite->oam;
	*words++ = attributes;
	*words = 0;
	/* Spelt through the OAM's address, as the two words above are written. */
	(&sprite->oam)->fullCol = 1;
	sprite->oam.idx = tile;
}

/* Draws a field effect's sprite. An unscaled, unrotated sprite needs no */
/* affine matrix; any other takes one, and one scaled past 1 is drawn in a */
/* double-size box. The picture's pivot is turned and scaled with it, so the */
/* pivot stays on the sprite's position. A sprite wholly off screen is not */
/* queued; the rest join the OAM bucket of their depth. */
void fdEffectSprDraw(EFFECT_SPR *sprite) {
	s32 affineMode;
	s32 matrix;
	FIXED x = sprite->x;
	FIXED y = sprite->y;
	s32 scaleX = sprite->scaleX;
	s32 scaleY = sprite->scaleY;
	s32 width = sprite->width;
	s32 height = sprite->height;
	s32 left;
	s32 top;
	s32 screenX;
	s32 screenY;
	s32 magnitude;
	AFFINE_TRANSFORM transform;

	if(sprite->depth == EFFECT_SPRITE_HIDDEN || scaleX == 0 || scaleY == 0)
		return;

	/* The affine matrix, unless the sprite is drawn as it is. */
	if(scaleX == FIXED_ONE && scaleY == scaleX && sprite->ang == 0) {
		matrix = 0;
		affineMode = OAM_AFFINE_OFF;
	}
	else {
		transform.ang = sprite->ang;
		transform.scaleX = scaleX >> 8;
		transform.scaleY = scaleY >> 8;
		matrix = syOamMatrixAllocFromTransform(&transform);
		affineMode = OAM_AFFINE_ON;
	}

	/* Grown past its own size, it needs the double-size box. */
	magnitude = scaleX;
	if(magnitude < 0)
		magnitude = -magnitude;
	if(magnitude > FIXED_ONE ||
	   (magnitude = scaleY < 0 ? -scaleY : scaleY, magnitude > FIXED_ONE)) {
		affineMode = OAM_AFFINE_DOUBLE_SIZE;
		width *= 2;
		height *= 2;
	}

	/* Where the pivot sits in the drawn box, from the box's top left. */
	if(sprite->ang == 0) {
		if(scaleX == FIXED_ONE && scaleY == scaleX) {
			left = sprite->pivotX - (sprite->width >> 1) + width / 2;
			top = sprite->pivotY - (sprite->height >> 1) + height / 2;
		}
		else {
			left = FIXEDtoINT(scaleX * (sprite->pivotX - (sprite->width >> 1)) +
							  (FIXED_ONE - 1)) +
				   width / 2;
			top = FIXEDtoINT(scaleY * (sprite->pivotY - (sprite->height >> 1)) +
							 (FIXED_ONE - 1)) +
				  height / 2;
		}
	}
	else {
		FIXED sin = sySin(sprite->ang);
		FIXED cos = syCos(sprite->ang);
		s32 halfWidth = sprite->width * scaleX / 2;
		s32 halfHeight = sprite->height * scaleY / 2;
		FIXED cornerX = syMulFX(cos, halfWidth) - syMulFX(sin, halfWidth);
		FIXED cornerY = syMulFX(cos, halfHeight) + syMulFX(sin, halfHeight);
		s32 pivotX = scaleX * sprite->pivotX;
		s32 pivotY = sprite->pivotY * scaleY;
		s32 turnedX = syMulFX(cos, pivotX) - syMulFX(sin, pivotY);
		s32 turnedY = syMulFX(cos, pivotY) + syMulFX(sin, pivotX);

		left = width / 2 + FIXEDtoINT(turnedX - cornerX);
		top = height / 2 + FIXEDtoINT(turnedY - cornerY);
	}

	/* The box on screen; skip it when none of it shows. */
	screenX = FIXEDtoINT(x) - left;
	screenY = FIXEDtoINT(y) - top;
	if(screenX + width < 0 || screenX >= DISPLAY_WIDTH ||
	   screenY + height <= 0 || screenY >= DISPLAY_HEIGHT)
		return;

	sprite->oam.x = screenX;
	sprite->oam.y = screenY;
	sprite->oam.affineMode = affineMode;
	sprite->oam.matrix = matrix;
	syOamAddSprAtDepthClamped(sprite, sprite->depth);
}

/* Places sprite at the screen position of world position and draws it. */
void fdEffectSprDrawAtWorldPos(EFFECT_SPR *sprite, s32 *position) {
	s32 screen[3];

	if(sprite != 0) {

		screen[0] = position[0];
		screen[1] = position[1];
		screen[2] = position[2];
		fdCameraWorldToScr(screen);
		sprite->x = screen[0];
		sprite->y = screen[2];
		fdEffectSprDraw(sprite);
	}
}

#undef HEIGHT_STEPS
#undef BLOCK_HEIGHT_STEPS
#undef OBJ_SIZE_SHIFT
#undef OBJ_SHAPE_SHIFT
