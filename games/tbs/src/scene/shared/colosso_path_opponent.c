/* The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
   the opponent who races Isaac by running a recorded path, and the race gauge
   that shows how near each of them is to the finish. */

/* The halfwords of the opponent's path: x and z for 7,200 ticks, the last pair
   the (0, 0) that ends it, as scColossoUpdatePathOpponent records it. */
#define COLOSSO_PATH_HALFWORDS (7200 * 2)

/* The race's state in HEAP_SLOT_COLOSSO_RACE, allocated whole when the gauge
   starts: the gauge, then the opponent's path. */
typedef struct COLOSSO_RACE_STATE {
	COLOSSO_GAUGE_STATE gauge;
	/* Pixels, as pairs of x and z, one pair a tick, ending with (0, 0):
	   decoded from the map's resource, or recorded (PATH_RECORDER_RECORD). */
	Sint16 path[COLOSSO_PATH_HALFWORDS];
} COLOSSO_RACE_STATE;

STATIC_ASSERT(sizeof(COLOSSO_RACE_STATE) == 29040);

/* The opponent's event, each tick. Playing, it puts him on the next point of
   his path, turning at most FACING_STEP a tick to face the way he runs and
   standing once he has kept still three ticks; at the path's end he walks to
   the first prize, or the second when Isaac is there first, and holds it up.
   Recording, it keeps the recorded actor's place each tick, and once the path
   is full (two minutes) plays it back on the opponent. */
static void scColossoUpdatePathOpponent(void)
{
	COLOSSO_RACE_STATE *state = (COLOSSO_RACE_STATE *)ColossoState;
	PATH_RECORDER	   *recorder = (PATH_RECORDER *)SceneLocalState;
	ACTOR			   *actor;
	Sint32				x, z, turn;
	Uint16			   *itemActorId;

	actor = fdActorGetById(recorder->actorId);
	if(actor == NULL) {
		return;
	}
	if(recorder->mode == PATH_RECORDER_PLAY) {
		x = state->path[(Sint16)recorder->pathIndex++];
		z = state->path[(Sint16)recorder->pathIndex++];
		if(x == 0 && z == 0) {
			FIXED finishZ;
			recorder->mode = PATH_RECORDER_FINISHED;
			fdActorSelectAnimation(actor, ANIM_STAND);
			if(state->gauge.finishX < actor->x.fixed) {
				x = state->gauge.finishX + toFIXED(12);
			} else {
				x = state->gauge.finishX - toFIXED(12);
			}
			if(gmFlagGet(FLAG_ARENA_REACHED)) {
				finishZ = state->gauge.finishZ + toFIXED(16);
				itemActorId = &state->gauge.finishItemActorIdB;
			} else {
				finishZ = state->gauge.finishZ - toFIXED(16);
				itemActorId = &state->gauge.finishItemActorIdA;
			}
			actor->timer = *itemActorId;
			actor->acceleration = toFIXED(1) / 4;
			actor->speedLimit = toFIXED(1);
			syFieldActorSetMoveTargetFX(actor, x, 0, finishZ);
			gmFlagSet(FLAG_ARENA_REACHED);
			fdActorStartScript(actor, ColossoOpponentFinishScript);
			return;
		}
		x = toFIXED(x);
		z = toFIXED(z);
		if(recorder->mirror != 0) {
			x = state->gauge.finishX * 2 - x;
		}
		if(actor->x.fixed != x || actor->z.fixed != z) {
			{
				Uint16 angle = syAtan(z - actor->z.fixed, x - actor->x.fixed);
				turn = (Sint16)(angle - actor->facing);
			}
			if(turn > FACING_STEP) {
				turn = FACING_STEP;
			}
			if(turn < -FACING_STEP) {
				turn = -FACING_STEP;
			}
			actor->facing += turn;
			actor->x.fixed = x;
			actor->z.fixed = z;
			recorder->still = 0;
		} else {
			recorder->still++;
		}
		if(recorder->still > 2) {
			fdActorSelectAnimation(actor, ANIM_STAND);
		} else {
			fdActorSelectAnimation(actor, ANIM_LEADER_RUN);
		}
	} else if(recorder->mode == PATH_RECORDER_RECORD) {
		x = actor->x.part[0].integer;
		z = actor->z.part[0].integer;
		state->path[(Sint16)recorder->pathIndex++] = x;
		state->path[(Sint16)recorder->pathIndex++] = z;
		if((Sint16)recorder->pathIndex == COLOSSO_PATH_HALFWORDS - 2) {
			state->path[(Sint16)recorder->pathIndex++] = 0;
			state->path[(Sint16)recorder->pathIndex] = 0;
			recorder->actorId = state->gauge.opponentActorId;
			recorder->pathIndex = 0;
			recorder->mode = PATH_RECORDER_PLAY;
		}
	}
}

/* The gauge's packed tiles, decoded as it first shows. */
extern Uint8 ColossoGaugeGraphics[];

/* The gauge's sprite palette. */
#define GAUGE_PALETTE 14

/* Pixels of course a pixel of the gauge stands for. */
#define GAUGE_COURSE_PER_PIXEL 14

/* The gauge's tiles (gauge.png): the bar's end, a 16-pixel piece of bar, half
   of the finish at its centre, and the marks of the player (1P) and the
   opponent (2P). */
typedef enum COLOSSO_GAUGE_TILE {
	GAUGE_TILE_END = 0,
	GAUGE_TILE_BAR = 2,
	GAUGE_TILE_FINISH = 6,
	GAUGE_TILE_PLAYER_MARK = 8,
	GAUGE_TILE_OPPONENT_MARK = 12,
	GAUGE_TILE_COUNT = 16
} COLOSSO_GAUGE_TILE;

/* The race gauge's event: a bar across the top of the screen, with the finish
   at its centre and blinking marks for the opponent and Isaac at their
   distances from it. It slides down when the race starts and back up while
   FLAG_MENU_OPEN is set; hold keeps it down. */
static void scColossoDrawGauge(void)
{
	COLOSSO_GAUGE_STATE *gauge = (COLOSSO_GAUGE_STATE *)ColossoState;
	Sint32				*dest = (Sint32 *)gauge;
	COLOSSO_GAUGE_SPR	*entry = gauge->sprites;
	Sint32				 tile;
	Uint32				 segments;
	Uint32				 i;
	Sint32				 y;
	Sint32				 x;
	Sint32				 width;
	Uint8				*buf;
	ACTOR				*actor;
	Uint32				 shape;

	tile = VramCacheEntries[gauge->vramBlock].offset >> 5;
	segments = gauge->segments;
	if(gauge->hold != 0) {
		gauge->level = 2;
	} else if(gmFlagGet(FLAG_MENU_OPEN)) {
		if(gauge->level > 0) {
			gauge->level--;
		}
	} else if(gauge->level <= 1) {
		if(++gauge->level == 1) {
			syDMAXCopy(ColossoGaugePalette,
				(void *)&OBJ_PLTT_COLOR(GAUGE_PALETTE, 0),
				DMA_COPY_HALFWORDS(16), REG_DMA3);
			buf = syHeapCallAlternatePool(
				syHeapAllocateEWRAM, GAUGE_TILE_COUNT * TILE_SIZE_4BPP);
			syFieldDecompressTaggedLZ(ColossoGaugeGraphics, buf);
			syVRAMCacheLoad(
				gauge->vramBlock, GAUGE_TILE_COUNT * TILE_SIZE_4BPP, buf);
			syHeapRelease(buf);
		}
	}
	if(gauge->level == 0) {
		syVRAMCacheReleaseVRAM(gauge->vramBlock);
		return;
	}

	y = (gauge->level * 6 - 8) & 255;
	width = segments * 16;
	x = 104 - width;
	/* shape holds each sprite's size in turn. */
	shape = OBJ_SIZE_8X16;
	*dest++ = 0;
	*dest++ = OBJ_X(x) | y | shape;
	*dest++ =
		(tile + GAUGE_TILE_END) | OBJ_PRIORITY(1) | OBJ_PALETTE(GAUGE_PALETTE);
	syOAMAddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
	for(i = 0; i < segments; i++) {
		x = 96 - i * 16;
		shape = OBJ_SIZE_16X16;
		*dest++ = 0;
		*dest++ = OBJ_X(x) | y | shape;
		*dest++ = (tile + GAUGE_TILE_BAR) | OBJ_PRIORITY(1) |
			OBJ_PALETTE(GAUGE_PALETTE);
		syOAMAddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
	}
	x = 112;
	shape = OBJ_SIZE_8X16;
	*dest++ = 0;
	*dest++ = OBJ_X(x) | y | shape;
	*dest++ = (tile + GAUGE_TILE_FINISH) | OBJ_PRIORITY(1) |
		OBJ_PALETTE(GAUGE_PALETTE);
	syOAMAddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
	x = 120;
	*dest++ = 0;
	*dest++ = OBJ_X(x) | y | shape | OBJ_HFLIP;
	*dest++ = (tile + GAUGE_TILE_FINISH) | OBJ_PRIORITY(1) |
		OBJ_PALETTE(GAUGE_PALETTE);
	syOAMAddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
	for(i = 0; i < segments; i++) {
		x = 128 + i * 16;
		shape = OBJ_SIZE_16X16;
		*dest++ = 0;
		*dest++ = y | OBJ_X(x) | shape | OBJ_HFLIP;
		*dest++ = (tile + GAUGE_TILE_BAR) | OBJ_PRIORITY(1) |
			OBJ_PALETTE(GAUGE_PALETTE);
		syOAMAddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
	}
	x = width + 128;
	shape = OBJ_SIZE_8X16;
	*dest++ = 0;
	y |= OBJ_X(x);
	y |= shape;
	y |= OBJ_HFLIP;
	*dest++ = y;
	*dest++ =
		(tile + GAUGE_TILE_END) | OBJ_PRIORITY(1) | OBJ_PALETTE(GAUGE_PALETTE);
	syOAMAddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);

	if((TickCount & 15) > 4) {
		shape = OBJ_SIZE_16X16;
		actor = fdActorLookup(gauge->opponentActorId);
		if(actor != NULL) {
			x = ((actor->x.fixed - gauge->finishX) /
					toFIXED(GAUGE_COURSE_PER_PIXEL)) +
				112;
			y = ((actor->z.fixed - gauge->finishZ) /
					toFIXED(GAUGE_COURSE_PER_PIXEL)) +
				gauge->level * 6 - 4;
			y &= 255;
			*dest++ = 0;
			y |= OBJ_X(x);
			y |= shape;
			*dest++ = y;
			*dest++ = (tile + GAUGE_TILE_OPPONENT_MARK) | OBJ_PRIORITY(1) |
				OBJ_PALETTE(GAUGE_PALETTE);
			syOAMAddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
		}
		actor = fdActorLookup(gauge->playerActorId);
		if(actor != NULL) {
			x = ((actor->x.fixed - gauge->finishX) /
					toFIXED(GAUGE_COURSE_PER_PIXEL)) +
				112;
			y = ((actor->z.fixed - gauge->finishZ) /
					toFIXED(GAUGE_COURSE_PER_PIXEL)) +
				gauge->level * 6 - 4;
			y &= 255;
			*dest++ = 0;
			y |= OBJ_X(x);
			y |= shape;
			*dest++ = y;
			*dest++ = (tile + GAUGE_TILE_PLAYER_MARK) | OBJ_PRIORITY(1) |
				OBJ_PALETTE(GAUGE_PALETTE);
			syOAMAddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
		}
	}
}
