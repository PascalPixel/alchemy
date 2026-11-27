// Places a room's vents from its table and the actors their puffs land on, unless the party comes
// back to the room as it left it, and starts the function that times their gusts; their puffs are
// drawn half seen through.
void scVentsStart(Uint16 *points, Uint16 *ids, void (*cycle)(void))
{
	VENT_WORK  *state = VENT_STATE;
	VENT_ENTRY *point = state->entries;

	if (!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
	{
		Sint32 x;
		Sint32 i;

		syWordsClearEntry(state, sizeof(VENT_WORK));
		x = *points++;
		i = 0;
		while (x > 0)
		{
			Sint32 z       = points[0];
			Sint32 speed   = points[1];

			point->active  = 0;
			point->x       = toFIXED(x);
			point->z       = toFIXED(z);
			point->y       = fdMapGetTerrainHeight(0, toFIXED(x), toFIXED(z) + toFIXED(16));
			point->value   = toFIXED(speed);
			point->timer   = i++;
			points        += 2;
			point++;
			if (i > VENT_MAX - 1)
				break;
			x = *points++;
		}
		if (ids != 0)
		{
			x = *ids++;
			i = 0;
			if (x > 0)
			{
				do
				{
					state->ids[i++] = x;
					if (i > VENT_IDS_MAX - 1)
						break;
					x = *ids++;
				} while (x > 0);
			}
		}
	}
	state->hook = (Sint32 (*)(void))cycle;
	if (REG_BLDCNT == 0)
	{
		{
			Sint32 alpha = BLEND_ALPHA(8, 12);
			REG_BLDALPHA = alpha;
		}
		{
			Sint32 count = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
			REG_BLDCNT   = count;
		}
	}
	sySetTickEvent(scVentUpdate, EVENT_PASS_TICK);
}
