/// Places a room's vents from its table and the actors their puffs land on,
/// unless the party comes back to the room as it left it, and starts the
/// routine that times their gusts; their puffs are drawn half seen through.
void Vents_Start(u16 *points, u16 *ids, void (*cycle)(void)) {
  struct VentWork *work = VENT_WORK;
  struct VentEntry *point = work->entries;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 x;
    s32 i;

    Iwram_ClearWordsEntry(work, sizeof(struct VentWork));
    x = *points++;
    i = 0;
    while (x > 0) {
      s32 z = points[0];
      s32 speed = points[1];

      point->active = 0;
      point->x = toFIXED(x);
      point->z = toFIXED(z);
      point->y = Map_GetTerrainHeight(0, toFIXED(x), toFIXED(z) + toFIXED(16));
      point->value = toFIXED(speed);
      point->timer = i++;
      points += 2;
      point++;
      if (i > VENTS_MAX - 1)
        break;
      x = *points++;
    }
    if (ids != 0) {
      x = *ids++;
      i = 0;
      if (x > 0) {
        do {
          work->ids[i++] = x;
          if (i > VENT_IDS_MAX - 1)
            break;
          x = *ids++;
        } while (x > 0);
      }
    }
  }
  work->hook = (s32 (*)(void))cycle;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = BLEND_ALPHA(8, 12);
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      REG_BLDCNT = cnt;
    }
  }
  Task_AddFrame(Vent_Update, TASK_PASS_FRAME);
}
