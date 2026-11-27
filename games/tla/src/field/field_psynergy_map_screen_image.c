/* The map screen's image: drawing it, scrolling it with the cursor, its area
   markers, and zooming the view out while L is held. Part of
   field_psynergy.c. */

/* The map screen's image on BG1, in 256 colours. The large image
   (RESOURCE_WORLD_MAP_PICTURE_LARGE) is 64 by 48 tiles, each row of tiles
   packed on its own and decoded into the map-metatile buffer, which holds 24
   rows and wraps. BG1 holds 32 by 24 of its tiles from char block 1 on,
   wrapping the same way, and BG1's map (fdWorldMapLoadGraphics) numbers them
   from the fifth row, where the still image starts. */
#define IMAGE_WIDTH			512
#define IMAGE_HEIGHT		384
#define IMAGE_TILES_ACROSS	(IMAGE_WIDTH / 8)
#define IMAGE_ROW_BYTES		(IMAGE_TILES_ACROSS * TILE_SIZE_8BPP)
/* The image's tiles BG1 holds, where its map's tile numbers wrap. */
#define MAP_SCREEN_TILES	(MAP_SCREEN_TILE_ROWS * BG_SCREEN_WIDTH)
/* The scroll that shows the image's bottom right corner. */
#define IMAGE_SCROLL_MAX_X	toFIXED(IMAGE_WIDTH - DISPLAY_WIDTH)
#define IMAGE_SCROLL_MAX_Y	toFIXED(IMAGE_HEIGHT - DISPLAY_HEIGHT)
/* The tiles drawn for one screen: 30 by 20, and the column and row partly
   scrolled in. */
#define IMAGE_DRAWN_COLUMNS (DISPLAY_WIDTH / 8 + 1)
#define IMAGE_DRAWN_ROWS	(DISPLAY_HEIGHT / 8 + 1)

/* The cursor's limits on the image, in pixels; y goes from 0. */
#define WORLD_MAP_CURSOR_MIN_X		  16
#define WORLD_MAP_CURSOR_MAX_X		  (IMAGE_WIDTH - 1)
#define WORLD_MAP_CURSOR_MAX_Y		  365
#define WORLD_MAP_CURSOR_MIN_SPEED	  toFIXED(1) /* pixels a tick */
#define WORLD_MAP_CURSOR_MAX_SPEED	  toFIXED(6) /* pixels a tick */
/* Added to the cursor's speed each tick the d-pad is held. */
#define WORLD_MAP_CURSOR_ACCELERATION (toFIXED(1) / 8)
/* How near the cursor a marker's name shows, in pixels. */
#define WORLD_MAP_NAME_REACH		  8
#define WORLD_MAP_MARKER_BLEND_STEPS  32 /* WorldMapMarkerBlendCycle's rows */

/* R (step 1) and L (step -1) on the map screen: the cursor jumps to the next or
   the previous marked place round from the one nearest it, and its point on the
   image is written to screenX and screenY. */
void fdMapScreenStepToPlace(Sint32 *screenX, Sint32 *screenY, Sint32 step)
{
	const WORLD_MAP_PLACE *places;
	const WORLD_MAP_PLACE *place;
	const WORLD_MAP_PLACE *next;
	Sint32				   best;
	Sint32				   bestDistance;
	Sint32				   cursorX;
	Sint32				   cursorY;
	Sint32				   cnt;
	Sint32				   wraps;
	Sint32				   flg;
	Uint32				   marker;
	WORLD_MAP_STATE		  *worldMapState = Ram_WorldMapState;

	/* DEAD: the leader looked up and never used; the ROM keeps the call. */
	fdPartyGetLeaderActor();
	places = fdWorldMapPlacesGetTable();
	bestDistance = 8192 * 8192;
	best = -1;
	cursorX = FIXEDtoINT(worldMapState->x);
	cursorY = FIXEDtoINT(worldMapState->z);
	if(worldMapState->searchX != 0) {
		cursorX = FIXEDtoINT(worldMapState->searchX);
		cursorY = FIXEDtoINT(worldMapState->searchZ);
	}
	for(cnt = 0; cnt < MAP_SCREEN_PLACE_COUNT; cnt++) {
		place = &places[cnt];
		if(place->flg == 0)
			break;
		flg = FLAG_PLACE_VISITED(place->flg);
		marker = fdWorldMapPlaceGetMarker(place);

		if(marker != WORLD_MAP_MARKER_NONE && marker != WORLD_MAP_MARKER_SHIP &&
			syGameFlagGet(flg)) {
			Sint32 sx = MAP_SCREEN_X(place->x);
			Sint32 sy = MAP_SCREEN_Y(place->z);
			Sint32 dx = sx - cursorX;
			Sint32 dy = sy - cursorY;
			Sint32 distance = dx * dx + dy * dy;

			if(bestDistance > distance) {
				bestDistance = distance;
				best = cnt;
			}
		}
	}
	if(best == -1) {
		fdMapScreenGetLeaderPoint(screenX, screenY);
		return;
	}
	place = &places[best];
	if(bestDistance <= 4) {
		wraps = 0;
		do {
			best += step;
			if(best < 0) {
				best = cnt - 1;
				wraps++;
			} else {
				next = &places[best];
				if(next->flg == 0) {
					best = 0;
					wraps++;
				} else {
					flg = FLAG_PLACE_VISITED(next->flg);
					marker = fdWorldMapPlaceGetMarker(next);

					if(marker != WORLD_MAP_MARKER_NONE &&
						marker != WORLD_MAP_MARKER_SHIP && syGameFlagGet(flg)) {
						place = next;
						break;
					}
				}
			}
		} while(wraps <= 1);
	}
	*screenX = MAP_SCREEN_X(place->x);
	*screenY = MAP_SCREEN_Y(place->z);
}

/* Moves every window sprite of a list by dx, dy: the output's position and its
   queued sprite's OAM x and y. */
void fdWindowSprListMove(WINDOW_SPR_LIST *list, Sint32 dx, Sint32 dy)
{
	WINDOW_SPR *output;
	QUEUED_SPR *spr;

	if(list == NULL)
		return;
	output = list->head;
	while(output != NULL) {
		output->x += dx;
		output->y += dy;
		spr = mnWindowSprGetSpr(output);
		output = output->next;
		spr->oam.x += dx;
		spr->oam.y += dy;
	}
}

/* Where the name of the place nearest the cursor is drawn: its top this many
   pixels above the place, and its left edge WORLD_MAP_NAME_LEFT pixels left of
   it (1 when zoomed). A name that would run off the screen's right edge ends
   WORLD_MAP_NAME_RIGHT_MARGIN pixels short of it instead, and is raised
   WORLD_MAP_NAME_RAISE more. */
#define WORLD_MAP_NAME_ABOVE		11
#define WORLD_MAP_NAME_RIGHT_MARGIN 8
#define WORLD_MAP_NAME_RAISE		10
/* The cursor sprite's left edge, this many pixels left of the cursor's
   point. */
#define MAP_SCREEN_CURSOR_LEFT		17

/* The scrolled name's left edge, in pixels left of its place. */
#define WORLD_MAP_NAME_LEFT GERMAN_OR(3, 1)

/* The upper halfword of value, its whole pixels. */
#define WORLD_MAP_HIGH(value)  (((Sint16 *)&(value))[1])
/* A place on the World Map, in pixels, as a place on the image, 300 pixels to
   every 640. */
#define WORLD_MAP_SCALE(value) ((value) * 300 / 640)

/* The names a marker shows besides its place's own. */
#define MARKER_NAME_PARTY -1 /* "Present location" */
#define MARKER_NAME_SHIP  -2 /* "Boat" */

/* Each tick of the map screen: moves the cursor with the d-pad, faster the
   longer it is held, or glides it toward its target; draws a marker at each
   place visited, at the ship once it sails and at the party; with the d-pad
   released draws the cursor onto the nearest marker in reach, and shows that
   marker's name beside it. */
void fdMapScreenUpdateMarkers(void)
{
	WORLD_MAP_STATE		  *state;
	WORLD_MAP_MARKER	  *marker;
	Sint32				   tileBase;
	Sint32				   blend;
	Sint32				   nearest;
	Sint32				   nearestDistance;
	Sint32				   nearestX;
	Sint32				   nearestY;
	Sint32				   name;
	Sint32				   nearestName;
	Sint32				   nearestEntrance;
	Sint32				   entrance;
	const WORLD_MAP_PLACE *places;
	Sint32				   scrollX;
	Sint32				   scrollY;
	const WORLD_MAP_PLACE *place;
	Sint32				   width;
	Sint32				   height;
	Sint32				   placeX;
	Sint32				   placeY;
	FIXED				   pos[3];
	Sint32				   x;
	Sint32				   y;
	Sint32				   dx;
	Sint32				   dz;
	FIXED				   distance;
	FIXED				   step;
	Sint32				   cursorX;
	Sint32				   cursorY;
	Uint16				   ang;
	Sint32				   tile;
	Sint32				   always;
	Sint32				   flg;
	Sint32				   i;

	/* DEAD: the leader looked up and never used; the ROM keeps the call. */
	fdPartyGetLeaderActor();
	state = Ram_WorldMapState;
	marker = state->markers;
	tileBase = VramCacheEntries[state->vramBlock].ofs >> 5;
	nearest = -1;
	nearestDistance = WORLD_MAP_NAME_REACH * WORLD_MAP_NAME_REACH;
	nearestX = 0;
	nearestY = 0;
	places = fdWorldMapPlacesGetTable();
	scrollX = state->scrollX & FIELD_PIXEL_MASK;
	scrollY = state->scrollZ & FIELD_PIXEL_MASK;
	blend = WorldMapMarkerBlendCycle[(TickCount >> 1) &
		(WORLD_MAP_MARKER_BLEND_STEPS - 1)];

	if(MapScreenGlideTargetX != 0) {
		x = MapScreenGlideTargetX;
		y = MapScreenGlideTargetY;
		dx = (x - state->x) / toFIXED(1);
		dz = (y - state->z) / toFIXED(1);
		distance = toFIXED(sySquartIWRAM(dx * dx + dz * dz));
		dx = x - state->x;
		dz = y - state->z;
		if(distance < toFIXED(64))
			distance = sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
		step = distance / 4;
		if(step > toFIXED(8))
			step = toFIXED(8);
		if(distance < toFIXED(1) / 2) {
			state->x = x;
			state->z = y;
			MapScreenGlideTargetX = 0;
			MapScreenGlideTargetY = 0;
		} else {
			if(distance > step) {
				dx = syMulFX(syDivFX(distance, dx), step);
				dz = syMulFX(syDivFX(distance, dz), step);
			}
			state->x += dx;
			state->z += dz;
		}
	} else {
		ang = WorldMapCursorDirectionAngles[DPAD_INDEX(PerWork.data)];
		if(ang != WORLD_MAP_KEY_HEADING_NONE) {
			pos[0] = state->x;
			pos[1] = 0;
			pos[2] = state->z;
			syVectorAddPolarOffset(state->speed, ang, pos);
			if(pos[0] < toFIXED(WORLD_MAP_CURSOR_MIN_X))
				pos[0] = toFIXED(WORLD_MAP_CURSOR_MIN_X);
			if(pos[0] > toFIXED(WORLD_MAP_CURSOR_MAX_X))
				pos[0] = toFIXED(WORLD_MAP_CURSOR_MAX_X);
			if(pos[2] < 0)
				pos[2] = 0;
			if(pos[2] > toFIXED(WORLD_MAP_CURSOR_MAX_Y))
				pos[2] = toFIXED(WORLD_MAP_CURSOR_MAX_Y);
			state->x = pos[0];
			state->z = pos[2];
			Ram_WorldMapCursorStart[0] = pos[0];
			Ram_WorldMapCursorStart[1] = pos[2];
			if(state->speed < WORLD_MAP_CURSOR_MAX_SPEED)
				state->speed += WORLD_MAP_CURSOR_ACCELERATION;
		}
	}

	cursorX = WORLD_MAP_HIGH(state->x);
	cursorY = WORLD_MAP_HIGH(state->z);
	place = places;
	i = 0;
	do {
		if(place->flg == 0) {
			if(MapScreenFirstListOnly != 0)
				break;
			place = &places[MAP_SCREEN_PLACE_COUNT];
			i = MAP_SCREEN_PLACE_COUNT;
			name = MARKER_NAME_PARTY;
			tile = WORLD_MARKER_TILE_PARTY;
			always = 0;
			fdMapScreenGetLeaderPoint(&placeX, &placeY);
		} else {
			name = place->visitScene;
			entrance = place->visitEntrance;
			flg = FLAG_PLACE_VISITED(place->flg);
			tile = fdWorldMapPlaceGetMarker(place);
			if(tile == WORLD_MAP_MARKER_NONE)
				continue;
			if(tile == WORLD_MAP_MARKER_SHIP) {
				if(!syGameFlagGet(FLAG_SHIP_CAN_SAIL))
					continue;
				if(MainRoutineStateWork.ownerOverride != 0)
					continue;
				name = MARKER_NAME_SHIP;
				tile = WORLD_MARKER_TILE_SHIP;
				always = 0;
				placeX = MAP_SCREEN_X(FIXEDtoINT(MainRoutineStateWork.shipX));
				placeY = MAP_SCREEN_Y(FIXEDtoINT(MainRoutineStateWork.shipZ));
			} else {
				if(!syGameFlagGet(flg))
					continue;
				always = 1;
				placeX = MAP_SCREEN_X(place->x);
				placeY = MAP_SCREEN_Y(place->z);
			}
		}
		marker->objMode = always;
		marker->tile = tileBase + tile;
		if(MapScreenZoomed == 0) {
			x = placeX - FIXEDtoINT(scrollX) - 1;
			y = placeY - FIXEDtoINT(scrollY) - 1;
			if((Uint32)(x + 4) > 247)
				continue;
			if(y < -4)
				continue;
			if(y > 163)
				continue;
			marker->x = x & OBJ_X_MASK;
			marker->y = y;
		} else {
			marker->x = WORLD_MAP_SCALE(placeX) - 1;
			marker->y = WORLD_MAP_SCALE(placeY) - 1;
		}
		width = placeX - cursorX;
		height = placeY - cursorY;
		if(width * width + height * height < nearestDistance) {
			nearest = i;
			nearestName = name;
			nearestEntrance = entrance;
			nearestDistance = width * width + height * height;
			nearestX = placeX;
			nearestY = placeY;
		}
		if(always != 0 || (TickCount & 15) <= 7)
			syOAMAddSprAtDepthClamped(marker++, MAP_SCREEN_MARKER_DEPTH);
	} while(++i, place++, i < MAP_SCREEN_PLACE_COUNT);

	if(PerWork.data == 0) {
		if(nearest != -1) {
			if(nearestDistance <= 2) {
				state->x = toFIXED(nearestX);
				state->z = toFIXED(nearestY);
				state->speed = WORLD_MAP_CURSOR_MIN_SPEED;
			} else {
				ang = syAtan(nearestY - cursorY, nearestX - cursorX);
				pos[0] = state->x;
				pos[1] = 0;
				pos[2] = state->z;
				syVectorAddPolarOffset(state->speed, (Uint16)ang, pos);
				state->x = pos[0];
				state->z = pos[2];
			}
			cursorX = WORLD_MAP_HIGH(state->x);
			cursorY = WORLD_MAP_HIGH(state->z);
		} else {
			state->speed = WORLD_MAP_CURSOR_MIN_SPEED;
		}
	}

	if(nearest != -1 && (TickCount & 15) <= 7) {
		marker->objMode = 0;
		marker->tile = tileBase + WORLD_MARKER_TILE_NEAREST;
		if(MapScreenZoomed == 0) {
			marker->x = nearestX - FIXEDtoINT(scrollX) - 2;
			marker->y = nearestY - FIXEDtoINT(scrollY) - 2;
		} else {
			marker->x = WORLD_MAP_SCALE(nearestX) - 2;
			marker->y = WORLD_MAP_SCALE(nearestY) - 2;
		}
		syOAMAddSprAtDepthClamped(marker, MAP_SCREEN_MARKER_DEPTH);
	}

	marker = &MapScreenCursorMarker;
	if(MapScreenZoomed == 0) {
		marker->affineIdx = 0;
		marker->x = cursorX - FIXEDtoINT(scrollX) - MAP_SCREEN_CURSOR_LEFT;
		marker->y = cursorY - FIXEDtoINT(scrollY) + 1;
	} else {
		marker->x = WORLD_MAP_SCALE(cursorX) - MAP_SCREEN_CURSOR_LEFT;
		marker->y = WORLD_MAP_SCALE(cursorY) + 1;
	}
	syOAMAddSprAtDepthClamped(marker, MAP_SCREEN_MARKER_DEPTH);

	if(state->shown != nearest) {
		/* The Japanese release prepares the window and draws the new name in
		   the same tick; the later ones draw it 6 ticks after preparing the
		   window. */
#if MAP_SCREEN_NAME_DELAYED_REDRAW
		if(MapScreenNameRedrawPending[0] == 0) {
			mnWindowSprPrepareForRedrawStub(state->win);
			MapScreenNameRedrawPending[0] = 1;
			MapScreenNameRedrawDelay[0] = 6;
		} else if(--MapScreenNameRedrawDelay[0] <= 0) {
			MapScreenNameRedrawPending[0] = 0;
			state->shown = nearest;
#else
		{
			state->shown = nearest;
			mnWindowSprPrepareForRedrawStub(state->win);
#endif
			if(nearest != -1) {
				if(nearestName == MARKER_NAME_PARTY)
					nearestName = (Sint32)MsgPresentLocation;
				else if(nearestName == MARKER_NAME_SHIP)
					nearestName = (Sint32)MsgBoat;
				else
					nearestName =
						fdAreaGetNameIndex(nearestName, nearestEntrance) +
						(Sint32)MsgDebugAreaName;
				mnTextMeasureMessageStub(nearestName, &width, &height);
				if(MapScreenZoomed == 0) {
					cursorX = nearestX - WORLD_MAP_NAME_LEFT;
					cursorY = nearestY - WORLD_MAP_NAME_ABOVE;
					cursorX -= FIXEDtoINT(scrollX);
					cursorY -= FIXEDtoINT(scrollY);
				} else {
					cursorX = WORLD_MAP_SCALE(nearestX) - 1;
					cursorY = WORLD_MAP_SCALE(nearestY) - WORLD_MAP_NAME_ABOVE;
				}
				if(cursorX + width > DISPLAY_WIDTH - 1) {
					cursorX =
						DISPLAY_WIDTH - WORLD_MAP_NAME_RIGHT_MARGIN - width;
					cursorY += -WORLD_MAP_NAME_RAISE;
				}
				if(cursorY < 0)
					cursorY = 0;
				mnTextDrawMessageStub(
					nearestName, state->win, cursorX, cursorY);
				state->nameBox.width = width;
				state->nameBox.height = height;
				state->nameBox.x = cursorX;
				state->nameBox.y = cursorY;
			}
		}
	} else if(nearest != -1) {
		width = state->nameBox.width;
		height = state->nameBox.height;
		if(MapScreenZoomed == 0) {
			cursorX = nearestX - WORLD_MAP_NAME_LEFT;
			cursorY = nearestY - WORLD_MAP_NAME_ABOVE;
			cursorX -= FIXEDtoINT(scrollX);
			cursorY -= FIXEDtoINT(scrollY);
		} else {
			cursorX = WORLD_MAP_SCALE(nearestX) - 1;
			cursorY = WORLD_MAP_SCALE(nearestY) - WORLD_MAP_NAME_ABOVE;
		}
		if(cursorX + width > DISPLAY_WIDTH - 1) {
			cursorX = DISPLAY_WIDTH - WORLD_MAP_NAME_RIGHT_MARGIN - width;
			cursorY += -WORLD_MAP_NAME_RAISE;
		}
		if(cursorY < 0)
			cursorY = 0;
		width = cursorX - state->nameBox.x;
		height = cursorY - state->nameBox.y;
		fdWindowSprListMove(state->win, width, height);
		state->nameBox.x = cursorX;
		state->nameBox.y = cursorY;
	}

	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
		((BLEND_WEIGHT_MAX - blend) << 8) | blend, IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDY, 0, IO_WRITE_HALFWORD);
}

#undef WORLD_MAP_NAME_LEFT
#undef MARKER_NAME_PARTY
#undef MARKER_NAME_SHIP
#undef WORLD_MAP_HIGH
#undef WORLD_MAP_SCALE

/* Draws the map screen on BG1 with the cursor in the middle of the screen (or
   the whole world, RESOURCE_WORLD_MAP_PICTURE, at the origin while
   WorldMapStateWork.centered is set): the palette, then 21 rows of 31 tiles. */
void fdMapScreenDraw(void)
{
	WORLD_MAP_STATE *state = &WorldMapStateWork;
	Uint8			*buf;
	Sint32			 resource = RESOURCE_WORLD_MAP_PICTURE_LARGE;
	Sint32			*cursor = state->cursor;
	Sint32			 x = *cursor++ - toFIXED(DISPLAY_WIDTH / 2);
	Sint32			 z = *cursor - toFIXED(DISPLAY_HEIGHT / 2);
	Uint32			*graphics;
	Uint32			 column;
	Uint32			 firstRow;
	Uint32			 row;
	Sint32			 bufRow;
	Sint32			 unwrapped;
	Uint32			 i;
	FIXED			 right;
	FIXED			 bottom;
	Sint32			 tile;
	Sint32			 metatile;

	if(state->centered != 0) {
		resource = RESOURCE_WORLD_MAP_PICTURE;
		x = 0;
		z = 0;
	}
	graphics = syResourceGetTableEntry(resource);
	syDMAXCopy((Uint8 *)graphics + graphics[0], (void *)BG_PLTT,
		DMA_COPY_WORDS(120), REG_DMA3);
	*(Uint16 *)BG_PLTT = 0;
	buf = Ram_MapMetatileBuffer;
	right = IMAGE_SCROLL_MAX_X;
	bottom = IMAGE_SCROLL_MAX_Y;
	if(x < 0)
		x = 0;
	if(x > right)
		x = right;
	if(z < 0)
		z = 0;
	if(z > bottom)
		z = bottom;
	BgScrollWork[1].x = FIXEDtoINT(x);
	BgScrollWork[1].y = FIXEDtoINT(z);
	x = FIELD_HALF_METATILE((Uint32)x);
	column = x;
	firstRow = FIELD_HALF_METATILE((Uint32)z);
	for(row = 0; row < IMAGE_DRAWN_ROWS; row++) {
		bufRow = firstRow + row;
		unwrapped = bufRow;
		if(bufRow >= MAP_SCREEN_TILE_ROWS)
			bufRow -= MAP_SCREEN_TILE_ROWS;
		metatile = bufRow * IMAGE_TILES_ACROSS + column;
		/* z is reused for the tile's place in BG1's map. */
		z = (column & (BG_SCREEN_WIDTH - 1)) + unwrapped * BG_SCREEN_WIDTH;
		tile = (z + MAP_SCREEN_MAP_START) % MAP_SCREEN_TILES +
			MAP_SCREEN_FIRST_TILE;
		syDecompressTaggedLZ((Uint8 *)graphics + graphics[unwrapped + 1],
			buf + bufRow * IMAGE_ROW_BYTES);
		for(i = 0; i < IMAGE_DRAWN_COLUMNS; i++) {
			syDMAXCopy(buf + metatile * TILE_SIZE_8BPP,
				(void *)(VRAM + tile * TILE_SIZE_8BPP),
				DMA_COPY_WORDS(TILE_SIZE_8BPP / 4), REG_DMA3);
			metatile++;
			tile++;
			/* BG1's map is 32 tiles wide: the next column wraps to its left
			   edge. */
			if(i + column == BG_SCREEN_WIDTH - 1)
				tile -= BG_SCREEN_WIDTH;
		}
	}
}

/* Draws one row of the map screen on BG1 as it scrolls in, from a column to the
   image's right edge, at most 31 tiles (the Japanese release always draws 32):
   decodes the row into the map-metatile buffer and copies its tiles to
   BG1's. */
void fdMapScreenDrawRow(Uint32 column, Sint32 row)
{
	Sint32	resource = RESOURCE_WORLD_MAP_PICTURE_LARGE;
	Uint32 *graphics;
	Sint32	unwrapped;
	Sint32	metatile;
	Sint32	tile;
	Uint32	i;

	if(MapScreenZoomed != 0)
		resource = RESOURCE_WORLD_MAP_PICTURE;
	graphics = syResourceGetTableEntry(resource);
	unwrapped = row;
	if(row >= MAP_SCREEN_TILE_ROWS)
		row -= MAP_SCREEN_TILE_ROWS;
	metatile = row * IMAGE_TILES_ACROSS + column;
	tile = (column & (BG_SCREEN_WIDTH - 1)) + row * BG_SCREEN_WIDTH;
	tile = (tile + MAP_SCREEN_MAP_START) % MAP_SCREEN_TILES +
		MAP_SCREEN_FIRST_TILE;
	syDecompressTaggedLZ((Uint8 *)graphics + graphics[unwrapped + 1],
		Ram_MapMetatileBuffer + row * IMAGE_ROW_BYTES);
	/* In every edition but the Japanese the copy stops at the image's last
	   column instead of running past it. */
#if MAP_SCREEN_ROW_COPIES_31_TILES_WITHIN_IMAGE
	for(i = 0; i < IMAGE_DRAWN_COLUMNS && column + i < IMAGE_TILES_ACROSS;
		i++) {
#else
	for(i = 0; i < BG_SCREEN_WIDTH; i++) {
#endif
		syDMAXCopy(Ram_MapMetatileBuffer + metatile * TILE_SIZE_8BPP,
			(void *)(VRAM + tile * TILE_SIZE_8BPP),
			DMA_COPY_WORDS(TILE_SIZE_8BPP / 4), REG_DMA3);
		metatile++;
		tile++;
		if(i + column == BG_SCREEN_WIDTH - 1)
			tile -= BG_SCREEN_WIDTH;
	}
}

/* Draws one column of the map screen on BG1 as it scrolls in, 21 tiles down
   from a row, from the rows already decoded in the map-metatile buffer. */
void fdMapScreenDrawColumn(Sint32 column, Sint32 row)
{
	Uint32 i;

	for(i = 0; i < IMAGE_DRAWN_ROWS; i++) {
		Sint32 line = row + i;
		Sint32 bufRow = line;
		Sint32 metatile;
		Sint32 tile;

		if(line >= MAP_SCREEN_TILE_ROWS)
			bufRow = line - MAP_SCREEN_TILE_ROWS;
		metatile = column + bufRow * IMAGE_TILES_ACROSS;
		tile = ((column & (BG_SCREEN_WIDTH - 1)) + line * BG_SCREEN_WIDTH +
				   MAP_SCREEN_MAP_START) %
			MAP_SCREEN_TILES;
		metatile *= TILE_SIZE_8BPP;
		tile *= TILE_SIZE_8BPP;
		syDMAXCopy(Ram_MapMetatileBuffer + metatile,
			(Uint8 *)(VRAM + MAP_SCREEN_FIRST_TILE * TILE_SIZE_8BPP) + tile,
			DMA_COPY_WORDS(TILE_SIZE_8BPP / 4), REG_DMA3);
		/* FAKEMATCH(dead): line set to 0 at the step's end, where nothing reads
		 * it; as no store. */
		line = 0;
	}
}

/* The map screen's scroll, x then y. */
#define SCROLL_X Ram_WorldMapScroll[0]
#define SCROLL_Y Ram_WorldMapScroll[1]

/* The map screen's scroll follows the cursor. The target puts the cursor in the
   middle of the screen (clamped to the image); unless the whole world shows,
   the scroll moves a quarter of the way there each tick, at most 8 pixels, and
   stops when nearer than a quarter pixel. Each time the scroll crosses into a
   new column or row of tiles, that column (fdMapScreenDrawColumn) or row
   (fdMapScreenDrawRow) is drawn; BG1 scrolls with it. */
void fdMapScreenUpdateScroll(void)
{
	Sint32 *cursor = MapScreenCursor;
	Sint32	x = *cursor++ - toFIXED(DISPLAY_WIDTH / 2);
	Sint32	y = *cursor - toFIXED(DISPLAY_HEIGHT / 2);
	Sint32	dx;
	Sint32	dy;
	FIXED	distance;
	FIXED	step;
	Uint32	column;
	Uint32	row;
	Sint32	limit;
	Sint32	bottom = IMAGE_SCROLL_MAX_Y;

	if(x < 0)
		x = 0;
	limit = IMAGE_SCROLL_MAX_X;
	if(x > limit)
		x = limit;
	if(y < 0)
		y = 0;
	if(y > bottom)
		y = bottom;
	if(MapScreenZoomed == 0) {
		dx = (x - SCROLL_X) / toFIXED(1);
		dy = (y - SCROLL_Y) / toFIXED(1);
		distance = toFIXED(sySquartIWRAM(dx * dx + dy * dy));
		dx = x - SCROLL_X;
		dy = y - SCROLL_Y;
		/* Near the target the whole-pixel distance is too coarse. */
		if(distance < toFIXED(64))
			distance = sySquartFX(syMulFX(dx, dx) + syMulFX(dy, dy));
		step = distance / 4;
		if(step > toFIXED(8))
			step = toFIXED(8);
		if(distance >= toFIXED(1) / 4) {
			if(distance > step) {
				dx = syMulFX(syDivFX(distance, dx), step);
				dy = syMulFX(syDivFX(distance, dy), step);
			}
			x = SCROLL_X + dx;
			y = SCROLL_Y + dy;
			if(x < 0)
				x = 0;
			if(y < 0)
				y = 0;
			if(x > limit)
				x = limit;
			/* DEAD: y held to x's limit, which a scroll toward a target within
			   the image never passes; the ROM keeps the compare and the
			   store. */
			if(y > limit)
				y = limit;
		}
		column = FIELD_HALF_METATILE((Uint32)x);
		row = FIELD_HALF_METATILE((Uint32)y);
		if((SCROLL_X ^ x) & toFIXED(8)) {
			if(SCROLL_X < x)
				fdMapScreenDrawColumn(column + DISPLAY_WIDTH / 8, row);
			else
				fdMapScreenDrawColumn(column, row);
		}
		if((SCROLL_Y ^ y) & toFIXED(8)) {
			if(SCROLL_Y < y)
				fdMapScreenDrawRow(column, row + DISPLAY_HEIGHT / 8);
			else
				fdMapScreenDrawRow(column, row);
		}
		BgScrollWork[1].x = FIXEDtoINT(x);
		BgScrollWork[1].y = FIXEDtoINT(y);
	}
	SCROLL_X = x;
	SCROLL_Y = y;
}

/* The map screen, over the map-metatile buffer, until A or B closes it, to look
   or to choose where to Teleport (MAP_SCREEN_MODE). While Start is held the
   cursor glides to the party, and back when it is let go; R and L step it
   between the places, and Select shows the whole world or the scrolling
   image. */
Sint32 fdMapScreenShow(Sint32 mode)
{
	Sint32				   savedDelay;
	Uint8				   savedFlg[ARRAY_COUNT(((MAP_STATE *)NULL)->anim)];
	Sint32				   screenX;
	Sint32				   screenY;
	MAP_STATE			  *map = Ram_HeapSlots->mapGrid;
	WORLD_MAP_VIEW		  *view = Ram_HeapSlots->mapGrid;
	FIELD_STATE			  *field = Ram_HeapSlots->fieldState;
	ACTOR_SYSTEM_STATE	  *actorSystem = Ram_HeapSlots->actorSystem;
	Sint16				   blend;
	Bool				   msgShown = FALSE;
	Sint32				   zoomed = 0;
	Sint32				   gliding = 0;
	WORLD_MAP_STATE		  *state = &WorldMapStateWork;
	MAP_ANIMATION		  *layer;
	const WORLD_MAP_PLACE *place;
	Uint32				  *tiles;
	Uint32				   tile;
	Sint32				   row;
	Sint32				   column;
	Sint32				   i;
	INPUT_STATE			  *input;
	INPUT_STATE			  *keys;
	Sint32				  *wayX;
	Sint32				  *wayZ;
	Sint32				  *cursorX;
	Sint32				  *cursorZ;
	Sint32				  *scrollX;
	Sint32				  *scrollZ;
	Sint32				   value;
	WORLD_MAP_CHOICE	  *choice = &MapScreenChoice;
	MAIN_ROUTINE_STATE	  *routine = &MainRoutineStateWork;

	state->mode = mode;
	if(mode != MAP_SCREEN_LOOK || field->mode != ACTOR_SYSTEM_WORLD_MAP)
		fdActorTableSnapshot();
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
		savedDelay = field->transitionFrm;
		field->transitionFrm = 6;
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		layer = map->anim;
		for(i = 0; i < (Sint32)ARRAY_COUNT(savedFlg); i++) {
			savedFlg[i] = layer->paused;
			layer->paused = 1;
			layer++;
		}
		actorSystem->suspended = 1;
		sySynch(1);
		view->viewHeld = 1;
		do {
			/* FAKEMATCH(dead): the read in a once-only block; as the plain
			 * statement. */
			blend = REG_BLDALPHA;
		} while(0);
	} else {
		syIOWrite8(1, &field->camera->paused);
		savedDelay = field->transitionFrm;
		field->transitionFrm = 6;
		field->transition = 0;
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		syInitEvent();
		mnWindowSystemInitFieldStub();
		actorSystem->suspended = 1;
		sySynch(1);
		blend = REG_BLDALPHA;
		/* BG1's map numbers the image's tiles as fdWorldMapLoadGraphics does,
		   two a word: from the fifth row, then on round. */
		tiles = (Uint32 *)BG_SCREEN_BLOCK(6);
		tile = TILE_PAIR(MAP_SCREEN_FIRST_TILE + MAP_SCREEN_MAP_START);
		for(row = 0; row < MAP_SCREEN_TILE_ROWS - 4; row++)
			for(column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
				*tiles++ = tile;
				tile += TILE_PAIR_STEP;
			}
		tile = TILE_PAIR(MAP_SCREEN_FIRST_TILE);
		for(row = 0; row < MAP_SCREEN_TILE_ROWS; row++)
			for(column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
				*tiles++ = tile;
				tile += TILE_PAIR_STEP;
			}
		tile = TILE_PAIR(MAP_SCREEN_FIRST_TILE);
		for(row = 0; row < 4; row++)
			for(column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
				*tiles++ = tile;
				tile += TILE_PAIR_STEP;
			}
		map->origin = 0;
		BgScrollWork[1].x = 0;
		BgScrollWork[1].y = 0;
		syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
			&REG_BG1CNT);
		syIOWrite16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
				DISPCNT_OBJ_ON,
			&REG_DISPCNT);
	}
	wayX = NULL;
	cursorX = NULL;
	cursorZ = NULL;
	scrollX = NULL;
	scrollZ = NULL;
	fdMapScreenInit();
	for(;;) {
		MapScreenZoomed = zoomed;
		fdMapScreenDraw();
		sySynch(1);
		syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS |
				BGCNT_SCREEN_BASE(6) | BGCNT_SIZE(2),
			&REG_BG1CNT);
		syIOWrite16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
				DISPCNT_OBJ_ON,
			&REG_DISPCNT);
		if((syGameFlagGet(FLAG_WORLD_MAP_LEFT) ||
			   syGameFlagGet(FLAG_LEADER_OFF_MAP_SCREEN)) &&
			!msgShown) {
			mnFieldMessageShowStub((Sint32)MsgNotOnMap, FIELD_MESSAGE_ZOOM);
			msgShown = TRUE;
		}
		keys = &PerWork;
		wayX = &MapScreenCursorStartX;
		cursorX = &MapScreenGlideTargetX;
		cursorZ = &MapScreenGlideTargetY;
		for(;;) {
			fdMapScreenUpdateScroll();
			fdMapScreenUpdateMarkers();
			input = keys;
			sySynch(1);
			if((input->repeat & KEY_START) && gliding == 0) {
				fdMapScreenGetLeaderPoint(&screenX, &screenY);
				*cursorX = toFIXED(screenX);
				/* DEAD: input set again to the &PerWork it already holds; the
				   ROM keeps the load. */
				input = &PerWork;
				*cursorZ = toFIXED(screenY);
				gliding = 1;
			}
			if(!(keys->data & KEY_START) && gliding == 1) {
				*cursorX = *wayX;
				/* DEAD: input set again to the &PerWork it already holds; the
				   ROM keeps the reload. */
				input = &PerWork;
				*cursorZ = MapScreenCursorStartY;
				gliding = 0;
			}
			if(keys->repeat & KEY_R) {
				fdMapScreenStepToPlace(&screenX, &screenY, 1);
				*cursorX = toFIXED(screenX);
				/* wayZ only ever holds &MapScreenCursorStartY, set where it is
				   used. */
				wayZ = &MapScreenCursorStartY;
				*cursorZ = toFIXED(screenY);
				*wayX = *cursorX;
				*wayZ = *cursorZ;
			}
			if(input->repeat & KEY_L) {
				fdMapScreenStepToPlace(&screenX, &screenY, -1);
				*cursorX = toFIXED(screenX);
				wayZ = &MapScreenCursorStartY;
				*cursorZ = toFIXED(screenY);
				*wayX = *cursorX;
				*wayZ = *cursorZ;
			}
			if(input->repeat & KEY_SELECT) {
				state->shown = -1;
				mnWindowSprPrepareForRedrawStub(state->win);
				syIOWrite16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
				zoomed ^= 1;
				if(*cursorX != 0) {
					Sint32 minimum = 0;
					Sint32 limitX = toFIXED(272);
					Sint32 limitZ = toFIXED(224);

					state->x = *cursorX;
					state->z = *cursorZ;
					scrollX = &MapScreenScrollX;
					*scrollX = state->x - toFIXED(120);
					scrollZ = &MapScreenScrollY;
					*scrollZ = state->z - toFIXED(80);
					value = *scrollX;
					if(value < minimum) {
						*scrollX = minimum;
						value = *scrollX;
					}
					if(value > limitX)
						*scrollX = limitX;
					value = *scrollZ;
					if(value < minimum) {
						*scrollZ = minimum;
						value = *scrollZ;
					}
					if(value > limitZ)
						*scrollZ = limitZ;
				}
				break;
			}
			if(input->repeat & KEY_A) {
				if(mode == MAP_SCREEN_LOOK)
					goto done;
				if(state->shown == -1)
					continue;
				place = fdWorldMapPlacesGetTable();
				place += state->shown;
				routine->teleportScene = SCENE_WORLD_MAP;
				choice->flg = place->flg;
				fdWorldMapSetReturnForScene(place->scene, place->entrance);
				goto done;
			}
			if(input->repeat & KEY_B) {
				if(mode != MAP_SCREEN_LOOK) {
					routine->teleportScene = -1;
					{
						/* FAKEMATCH(temporary): none, -1 held in a local; as -1
						 * at its uses. */
						Sint32 none = -1;

						choice->flg = none;
					}
				}
				goto done;
			}
		}
	}
done:
	fdMapScreenRelease();
	syIOWrite16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	if(mode != MAP_SCREEN_LOOK || field->mode != ACTOR_SYSTEM_WORLD_MAP) {
		field->exit = EXIT_KEEP_DESTINATION;
		syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
	} else {
		fdWorldMapLoadAreaGraphicsStub();
		IO_WRITE_QUEUE_PUSH(
			(Uint32)&REG_BLDALPHA, (Uint16)blend, IO_WRITE_HALFWORD);
		layer = map->anim;
		for(i = 0; i < (Sint32)ARRAY_COUNT(savedFlg); i++) {
			layer->paused = savedFlg[i];
			layer++;
		}
		actorSystem->suspended = 0;
		BgScrollWork[1].x = 0;
		BgScrollWork[1].y = 0;
		view->viewHeld = 0;
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		field->transitionFrm = savedDelay;
		field->camera->paused = 0;
	}
	return 0;
}

/* The World Map's places, ended by a zero flag. */
extern const WORLD_MAP_PLACE WorldMapPlaces[];

/* On the world map, zooms the view out to eight thirteenths while L is held,
   then back, six ticks each way (FIELD_RAISED_CUE_WORLD_MAP_ZOOM). */
void fdWorldMapZoomOutWhileLHeld(void)
{
	MAIN_ROUTINE_STATE *party;

	if(FieldStateWork->mode == ACTOR_SYSTEM_WORLD_MAP) {
		fdLeaderStopMoving();
		fdWorldMapStartZoom(toFIXED(8) / 13, 6);
		while(PerWork.data & KEY_L)
			sySynch(1);
		party = &MainRoutineStateWork;

		/* DEAD: both arms alike; the ROM keeps the test and both calls. */
		if(party->ownerOverride != 0)
			fdWorldMapStartZoom(toFIXED(1), 6);
		else
			fdWorldMapStartZoom(toFIXED(1), 6);
	}
}

/* Returns the flag entry table, 20-byte entries ended by a zero flag. */
const WORLD_MAP_PLACE *fdWorldMapPlacesGetTable(void)
{
	return WorldMapPlaces;
}

#undef IMAGE_WIDTH
#undef IMAGE_HEIGHT
#undef IMAGE_TILES_ACROSS
#undef IMAGE_ROW_BYTES
#undef MAP_SCREEN_TILES
#undef IMAGE_SCROLL_MAX_X
#undef IMAGE_SCROLL_MAX_Y
#undef IMAGE_DRAWN_COLUMNS
#undef IMAGE_DRAWN_ROWS
#undef WORLD_MAP_CURSOR_MIN_X
#undef WORLD_MAP_CURSOR_MAX_X
#undef WORLD_MAP_CURSOR_MAX_Y
#undef WORLD_MAP_CURSOR_MIN_SPEED
#undef WORLD_MAP_CURSOR_MAX_SPEED
#undef WORLD_MAP_CURSOR_ACCELERATION
#undef WORLD_MAP_NAME_REACH
#undef WORLD_MAP_MARKER_BLEND_STEPS
#undef SCROLL_X
#undef SCROLL_Y
