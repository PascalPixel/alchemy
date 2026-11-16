/// @file
/// The Psynergy gauge over the leader as the ship flies, which the world map
/// and the staff credits' overlays keep a copy of: its tiles redrawn for the
/// party's PP, the task that drains it while A or B is held aloft, and its
/// set-up and end.

/// Redraws the gauge for `progress`.
void Gauge_Draw(s32 progress) {
  s32 level = ((progress * GAUGE_LEVELS + FIXED_ONE / 2) >> 16) + 1;
  s32 kind = GAUGE_EMPTY;
  s32 count, base, i;

  if (progress != 0)
    kind = GAUGE_SHOWN;
  if (level <= 15 && (gFrameCount & 15) <= 1)
    kind = GAUGE_LOW;
  if (level <= 7 && (gFrameCount & 7) <= 1)
    kind = GAUGE_LOW;
  if (kind + level == gGauge_Shown)
    return;
  gGauge_Shown = kind + level;
  Iwram_ClearWords((s32)(gGauge_Tiles), (s32)(GAUGE_SIZE));
  {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    if (kind == GAUGE_EMPTY) {
      copy(gGauge_Tiles, gGauge_SourceTiles[8], 4 * TILE_SIZE_4BPP);
    } else {
      if (kind == GAUGE_SHOWN)
        copy(gGauge_Tiles, gGauge_SourceTiles[4], 4 * TILE_SIZE_4BPP);
      else
        copy(gGauge_Tiles, gGauge_SourceTiles[8], 4 * TILE_SIZE_4BPP);
      base = 0;
      count = level;
      if (count > 7) {
        Iwram_CopyWordsEntry(gGauge_Tiles, gGauge_SourceTiles, TILE_SIZE_4BPP);
        base = 8;
        count -= 8;
      }
      if (level > 15) {
        Iwram_CopyWordsEntry(gGauge_Tiles[1], gGauge_SourceTiles[1],
                             TILE_SIZE_4BPP);
        count = level;
        base = 16;
        count -= 16;
      }
      if (level > 23) {
        Iwram_CopyWordsEntry(gGauge_Tiles[2], gGauge_SourceTiles[2],
                             TILE_SIZE_4BPP);
        count = level;
        base = 24;
        count -= 24;
      }
      if (level > 30) {
        Iwram_CopyWordsEntry(gGauge_Tiles[3], gGauge_SourceTiles[3],
                             TILE_SIZE_4BPP);
        base = 0;
        count = 0;
      }
      if (count != 0) {
        for (i = 0; i < count; i++) {
          u8 *source = &gGauge_SourceTiles[base / 8][4] + i / 2;
          u8 *dest = &gGauge_Tiles[base / 8][4] + i / 2;
          s32 j;

          if ((i & 1) == 0) {
            for (j = 1; j <= 3; j++) {
              *dest = (*dest & 240) | (*source & 15);
              source += 4;
              dest += 4;
            }
          } else {
            for (j = 0; j < 3; j++) {
              *dest = (*dest & 15) | (*source & 240);
              source += 4;
              dest += 4;
            }
          }
        }
      }
    }
  }
  Vram_Load(gGauge_Vram, GAUGE_SIZE, gGauge_Tiles);
}

/// The Psynergy gauge over the leader as the ship flies: while A or B is held
/// aloft it drains a PP every ten frames, then redraws the gauge and queues its
/// sprite.
void Gauge_Update(void) {
  u32 *oam = gGauge_Sprite;
  s32 tile = gVramCacheEntries[gGauge_Vram].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput.held & (KEY_A | KEY_B)) != 0 &&
      Event_GetFlag(FLAG_MENU_OPEN) == 0 && leader->y.fixed > toFIXED(4)) {
    if (--gGauge_Countdown <= 0) {
      Party_DrainPp(1);
      gGauge_Countdown = 10;
    }
  }
  Gauge_Draw(Party_GetPpRatio());
  attributes = OBJ_SIZE_32X16;
  Call2((void (*)())Render_ProjectPoint, (s32)(&leader->x.fixed),
        (s32)(screen));
  {
    s32 x = screen[0] - 16;
    s32 y = screen[1] - 32;

    *oam++ = 0;
    *oam++ = (y | OBJ_X(x)) | attributes;
  }
  *oam = tile | OBJ_PRIORITY(1);
  Oam_AddSpriteAtDepthClamped(gGauge_Sprite, OAM_DEPTH_FRONT);
}

/// Sets up the gauge.
void Gauge_Init(void) {
  Decompress_TaggedLz(gWorldMapPackedTilesC, gGauge_SourceTiles);
  gGauge_Vram = VramCache_FindFreeEntry();
  Vram_Load(gGauge_Vram, GAUGE_SIZE, 0);
  gGauge_Shown = -1;
  gGauge_Countdown = 10;
  Task_AddFrame(Gauge_Update, TASK_PASS_FRAME - 10);
}

/// Takes the gauge down.
void Gauge_End(void) {
  Decompress_TaggedLz(gWorldMapPackedTilesC, gGauge_SourceTiles);
  VramCache_FreeEntry(gGauge_Vram);
  gGauge_Shown = -1;
  FieldTask_Remove(Gauge_Update);
}
