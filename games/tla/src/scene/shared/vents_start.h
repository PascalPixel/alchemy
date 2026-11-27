/* Places a room's vents from its table and the actors their puffs land on, */
/* unless the party comes back to the room as it left it, and starts the */
/* function that times their gusts; their puffs are drawn half seen through. */
void syVentsStart(u16 *points, u16 *ids, void (*cycle)(void)) {
	VENT_WORK *state = VENT_STATE;
	VENT_ENTRY *point = state->entries;

	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		s32 x;
		s32 i;

		syWordsClearEntry(state, sizeof(VENT_WORK));
		x = *points++;
		i = 0;
		while(x > 0) {
			s32 z = points[0];
			s32 speed = points[1];

			point->active = 0;
			point->x = toFIXED(x);
			point->z = toFIXED(z);
			point->y =
				fdMapGetTerrainHeight(0, toFIXED(x), toFIXED(z) + toFIXED(16));
			point->value = toFIXED(speed);
			point->tmr = i++;
			points += 2;
			point++;
			if(i > VENT_MAX - 1)
				break;
			x = *points++;
		}
		if(ids != 0) {
			x = *ids++;
			i = 0;
			if(x > 0) {
				do {
					state->ids[i++] = x;
					if(i > VENT_IDS_MAX - 1)
						break;
					x = *ids++;
				} while(x > 0);
			}
		}
	}
	state->hook = (s32 (*)(void))cycle;
	if(REG_BLDCNT == 0) {
		{
			s32 alpha = BLEND_ALPHA(8, 12);
			REG_BLDALPHA = alpha;
		}
		{
			s32 cnt = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
			REG_BLDCNT = cnt;
		}
	}
	syTaskAddFrame(scVentUpdate, TASK_PASS_FRAME);
}
