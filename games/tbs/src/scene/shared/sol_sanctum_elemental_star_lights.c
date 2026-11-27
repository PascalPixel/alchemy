// Sol Sanctum's Elemental Star room and its eruption: the ten lights that float in the room, actors
// in a row from the first the map passes, each pulsing and drifting on its own path from the tables
// SolSanctumLight* (sol_sanctum_elemental_star_light_tables.c), by its index.

#include "system.h"

// The most lights the event keeps.
enum
{
	SOL_SANCTUM_LIGHT_MAX = 10
};

// A light moves every this many ticks.
#define SOL_SANCTUM_LIGHT_STEP_TICKS   3
// Shrunk to a tenth, a light is hidden for this many of its steps while it drifts on unseen, then
// shows again where it has drifted to.
#define SOL_SANCTUM_LIGHT_HIDDEN_STEPS 24

// One floating light: its actor, its drift and its pulsing scale, and where it is while hidden.
typedef struct SOL_SANCTUM_LIGHT_ENTRY
{
	ACTOR *actor;
	FIXED  x; // 16.16 fixed-point pixels
	FIXED  y; // 16.16 fixed-point pixels
	FIXED  z; // 16.16 fixed-point pixels
	// Its drift on each axis, which grows at random each step; the light swings by the sine of it.
	Sint32 speedX;
	Sint32 speedY;
	Sint32 speedZ;
	FIXED  scale;     // 16.16 fixed point, where toFIXED(1) is full size
	FIXED  scaleStep; // 16.16 fixed point
	Uint8  timer;     // ticks to its next step
	Uint8  hold;      // steps it has left hidden
} SOL_SANCTUM_LIGHT_ENTRY;

// The floating lights' state in HEAP_SLOT_SOL_SANCTUM_LIGHTS, one entry per light.
typedef struct SOL_SANCTUM_LIGHT_LIST
{
	SOL_SANCTUM_LIGHT_ENTRY entries[SOL_SANCTUM_LIGHT_MAX];
	Uint16                  count;
} SOL_SANCTUM_LIGHT_LIST;

// An event: every SOL_SANCTUM_LIGHT_STEP_TICKS each light pulses between a tenth of its size and
// its largest scale and drifts, swinging about on a sine of its drift speed on each axis it drifts
// on.
static void scSolSanctumUpdateFloatingLights(void)
{
	SOL_SANCTUM_LIGHT_LIST *list =
		syHeapSlotAllocateEWRAM(HEAP_SLOT_SOL_SANCTUM_LIGHTS, sizeof(SOL_SANCTUM_LIGHT_LIST));
	SOL_SANCTUM_LIGHT_ENTRY *entry = list->entries;
	Uint32                   i;

	i = 0;
	while (i != list->count)
	{
		ACTOR *actor;
		Sint32 speedX;
		Sint32 speedY;
		Sint32 speedZ;
		FIXED  scale;
		Sint32 step;
		Uint8  timer;
		FIXED  x;
		FIXED  y;
		FIXED  z;
		Uint8  hold;

		actor  = entry->actor;
		speedX = entry->speedX;
		speedY = entry->speedY;
		speedZ = entry->speedZ;
		scale  = entry->scale;
		step   = entry->scaleStep;
		hold   = entry->hold;
		x      = entry->x;
		y      = entry->y;
		z      = entry->z;
		timer  = entry->timer;
		if (--timer == 0)
		{
			Uint32 rx;
			Uint32 ry;
			Uint32 rz;
			FIXED  dx;
			FIXED  dy;

			timer = SOL_SANCTUM_LIGHT_STEP_TICKS;
			if (hold == 0)
			{
				scale += step;
				if (scale >= SolSanctumLightMaxScales[i])
				{
					step = -SolSanctumLightScaleSteps[i];
				}
				else if (scale <= toFIXED(1) / 10)
				{
					step           = SolSanctumLightScaleSteps[i];
					scale          = toFIXED(1) / 10;
					hold           = SOL_SANCTUM_LIGHT_HIDDEN_STEPS;
					x              = actor->x.fixed;
					y              = actor->y.fixed;
					z              = actor->z.fixed;
					actor->x.fixed = 0;
					actor->y.fixed = 0;
					actor->z.fixed = 0;
				}
				actor->scaleX = scale;
				actor->scaleY = scale;
			}
			rx = syRandomBelow(SolSanctumLightDrift[i][0]);
			ry = syRandomBelow(SolSanctumLightDrift[i][1]);
			rz = syRandomBelow(SolSanctumLightDrift[i][2]);
			rx = rx != 0 ? INTtoFIXED(rx) / 1000 : 0;
			ry = ry != 0 ? INTtoFIXED(ry) / 1000 : 0;
			rz = rz != 0 ? INTtoFIXED(rz) / 1000 : 0;
			if (SolSanctumLightDirection[i][0] == 1)
				speedX += rx;
			else if (SolSanctumLightDirection[i][0] == -1)
				speedX -= rx;
			else
				speedX = 0;
			if (SolSanctumLightDirection[i][1] == 1)
				speedY += ry;
			else if (SolSanctumLightDirection[i][1] == -1)
				speedY -= ry;
			else
				speedY = 0;
			if (SolSanctumLightDirection[i][2] == 1)
				speedZ += rz;
			else if (SolSanctumLightDirection[i][2] == -1)
				speedZ -= rz;
			else
				speedZ = 0;
			dx = sySin(speedX * SolSanctumLightSwing[i][0]) * 2;
			dy = sySin(speedY * SolSanctumLightSwing[i][1]) * 2;
			{
				FIXED dz = syCos(speedZ * SolSanctumLightSwing[i][2]) * 2;

				if (hold != 0)
				{
					x += dx;
					y += dy;
					z += dz;
					if (--hold == 0)
					{
						actor->x.fixed = x;
						actor->targetX = x;
						if (ry != 0)
						{
							actor->y.fixed = y;
							actor->targetY = y;
						}
						actor->z.fixed = z;
						actor->targetZ = z;
					}
				}
				else
				{
					actor->x.fixed += dx;
					actor->targetX  = actor->x.fixed;
					if (ry != 0)
					{
						actor->y.fixed += dy;
						actor->targetY  = actor->y.fixed;
					}
					actor->z.fixed += dz;
					actor->targetZ  = actor->z.fixed;
				}
			}
		}
		entry->speedX    = speedX;
		entry->speedY    = speedY;
		entry->speedZ    = speedZ;
		entry->scale     = scale;
		entry->scaleStep = step;
		entry->hold      = hold;
		entry->x         = x;
		entry->y         = y;
		entry->z         = z;
		entry->timer     = timer;
		i++;
		entry++;
	}
}

// Starts the floating lights: count actors (at most SOL_SANCTUM_LIGHT_MAX) from first, each drawn
// translucent and starting at its largest scale.
void scSolSanctumStartFloatingLights(Sint32 first, Uint32 count)
{
	// Each light is set up through its effect view (ACTOR_OR_EFFECT); the event reads it as an
	// actor.
	SOL_SANCTUM_LIGHT_LIST  *list;
	SOL_SANCTUM_LIGHT_ENTRY *entry;
	FIELD_EFFECT            *light;
	Uint32                   i;
	Sint32                   cleared;

	list  = syHeapSlotAllocateEWRAM(HEAP_SLOT_SOL_SANCTUM_LIGHTS, sizeof(SOL_SANCTUM_LIGHT_LIST));
	entry = list->entries;
	DMA_FILL32(0, list, sizeof(SOL_SANCTUM_LIGHT_LIST));
	if (count > SOL_SANCTUM_LIGHT_MAX)
		count = SOL_SANCTUM_LIGHT_MAX;
	i = 0;
	if (count != 0)
	{
		cleared = 0;
		do
		{
			light = (FIELD_EFFECT *)fdActorGetById(first);
			{
				Uint8 *flags = &light->sprite->flags;

				entry->actor = (ACTOR *)light;
				*flags       = cleared;
			}
			light->motionFlags = cleared;
			fdActorSetBlendMode(fdActorGetById(first), ACTOR_BLEND_TRANSLUCENT);
			entry->scale     = SolSanctumLightMaxScales[i];
			entry->scaleStep = -SolSanctumLightScaleSteps[i];
			entry->timer     = SOL_SANCTUM_LIGHT_STEP_TICKS;
			i++;
			entry++;
			first++;
		} while (i != count);
	}
	list->count = count;
	sySetTickEvent(scSolSanctumUpdateFloatingLights, EVENT_PASS_TICK);
}
