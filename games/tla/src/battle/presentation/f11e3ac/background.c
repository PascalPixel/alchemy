/// @file
/// The battle background, its palette and modes, the camera's zoom and the
/// units' highlights, the star field behind a summon and a unit's element
/// effect.
// Part of f11e3ac.c.

#include "../f11e3ac_internal.h"
#include "module/battle/f11e3ac.h"
#include "module/system/system.h"
/* VIEW(f11f66c): routine ColorBuffer_Scale as void ColorBuffer_Scale(u16 *source, u16 *destination, s32 scale, s32 count); f11e3ac has void ColorBuffer_Scale(u16 *source, u32 destination, s32 value, s32 size). */
extern void ColorBuffer_Scale__f11f66c(u16 *source, u16 *destination, s32 scale, s32 count) __asm__("ColorBuffer_Scale");

/// Sets the palette brightness. Level 0 transfers the original colours
/// unchanged.
void BattleBackground_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  u16 *palette = screen->palette;
  u32 ime;
  u32 *slot;

  // The save in a once-only block loads REG_IME's address after the stack
  // frame is made, and a loop on a zero local stores it through slot before
  // IME is cleared; without them, 8 of 54 instructions differ in tla-en.
  do {
    u16 *imeReg;
    s32 once;

    /* FAKEMATCH(temporary): a pointer to ime, which keeps the save in a stack
     * slot stored through r6; as ime directly, 33 of 54 instructions differ
     * in tla-en. */
    slot = &ime;
    imeReg = (u16 *)&REG_IME;
    once = 0;

    do {
      *slot = *imeReg;
      *imeReg = (u32)imeReg;
    } while (once);
  } while (0);

  if (level == 0) {
    Dma_Set(palette, (void *)&BG_PLTT_COLOR(6, 0), DMA_COPY_HALFWORDS(128),
            REG_DMA3);
  } else {
    ColorBuffer_Scale__f11f66c(palette, (u16 *)0x050000c0,
                      screen->brightness = 0x10000 - level * 1092, 128);
  }
  {
    u32 saved = *slot;

    REG_IME = saved;
  }
}

/// The Broken Seal's BattleBackground_SetMode, with this
/// game's heap slots, its callback BattleStaging_UpdateHBlankScroll at 0xcff
/// and its IO write of 0x1f83 to BG1CNT.
void Func_0812628c(s32 mode, s32 value, s32 fade) {
  struct BattleBackgroundView *transition =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;

  if (transition->mode == 0)
    Task_Add(BattleStaging_UpdateHBlankScroll, 0xcff);
  transition->mode = mode;

  if (mode == 1)
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                        BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS |
                            BGCNT_PRIORITY(3),
                        IO_WRITE_16);

  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  *(u16 *)0x050000bc = *(u16 *)0x050001e8;

  if (fade == 0x80) {
    struct BattleWork *work;
    u16 *palette;

    work = Ram_HeapSlots->battleWork;
    palette = work->palette;
    Dma_Set(palette, (void *)&BG_PLTT_COLOR(6, 0), DMA_COPY_HALFWORDS(128),
            REG_DMA3);
  } else if (fade != 0) {
    struct BattleWork *work;
    u16 *source;
    u16 *destination;
    s32 i;

    work = Ram_HeapSlots->battleWork;
    source = work->palette;
    destination = (u16 *)0x050000c0;

    for (i = 0; i != 128; i++) {
      s32 red = source[i] & 31;
      s32 green = (source[i] >> 5) & 31;
      s32 blue = (source[i] >> 10) & 31;

      if (red > fade)
        red -= fade;
      else
        red = 0;
      if (green > fade)
        green -= fade;
      else
        green = 0;
      if (blue > fade)
        blue -= fade;
      else
        blue = 0;
      destination[i] = (blue << 10) | (green << 5) | red;
    }
  }
  Graphics_BuildSequentialTileTable((void *)0x06003800);
  BattleTransition_BuildTilemap((void *)0x0600f800);
}

void Func_081263c4(void) {
  void *block = HeapSlot_AllocEwram(40, 0x2a0);
  struct BattlePresentationTransition *work = Ram_HeapSlots->transitionWork;

  // An empty once-only loop after the work's load keeps the fill's entry
  // ahead of the size's copy; without it, 4 of 18 instructions differ in
  // tla-en.
  do {
  } while (0);
  Iwram_FillWordsEntry(block, 0x2a0, 0);
  work->mode = 0;
}

void Func_081263f0(void) { HeapSlot_Release(40); }

/* VIEW(f11e3ac): routine Resource_GetTableEntry as u8 *Resource_GetTableEntry(s32 id); resource.h has void *Resource_GetTableEntry(s32 resourceId). */
extern u8 *Resource_GetTableEntry__f11e3ac(s32 id) __asm__("Resource_GetTableEntry");

// Loads a battle backdrop: its picture decoded into BG VRAM, its palette
// (darkened by level steps of 1/60 when level is not negative), the tile
// table and tilemap, and the HBlank scroll the first time.
void Func_081263fc(s32 mode, s32 resource, s32 level) {
  struct BattlePresentationTransition *view = Ram_HeapSlots->transitionWork;
  u8 *data = Resource_GetTableEntry__f11e3ac(resource);
  struct BattleBackdropWork *session =
      (struct BattleBackdropWork *)Ram_HeapSlots->battleWork;
  ModelDecoder decode;
  u16 *palette;

  RamCode_Load(decode, Model_DecodeCode, Model_DecodeCodeSize);
  {
    u32 offset = 0x100;
    void *vram;

    /* FAKEMATCH(dead): two updates that cancel, under a test that emits
     * nothing, end a CSE block so the VRAM address loads before the data's
     * add; without them, 2 of 137 instructions differ in tla-en. */
    if (session) {
      offset++;
      offset--;
    }
    vram = (void *)0x06008000;
    decode(data + offset, vram);
  }
  Heap_Release(decode);
  palette = session->palette;
  Dma_Set(data, palette, 0x84000040, REG_DMA3);
  if (level >= 0)
    ColorBuffer_Scale__f11f66c(palette, (u16 *)0x050000c0,
                      session->brightness = 0x10000 - level * 1092, 128);
  if (session->keepBackdropPalette == 0) {
    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;
  }
  Graphics_BuildSequentialTileTable((void *)0x06003800);
  BattleTransition_BuildTilemap((void *)0x0600f800);
  Iwram_ClearWordsEntry((void *)0x0600ffc0, 64);
  if (view->mode == 0)
    Task_Add(BattleStaging_UpdateHBlankScroll, 0xcff);
  view->mode = mode;
  if (mode == 1)
    REG_BG1CNT = 0x1f83;
}

void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale) {
  struct BattleBackgroundView *view =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  struct BattleAffineHdma *scanlines = Ram_HeapSlots->unk28[0];
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  s32 horizon;
  u16 wrap;
  u16 *line;
  struct BattleAffine *affine;
  s32 ratio;
  s32 step;
  s32 x;
  s32 y;
  u32 scanline;
  u32 groundEndLine;

  horizon = 0x800;
  wrap = 0;
  // ratio is reused for Q16_ONE, the scale test's and the call's; as the
  // constant in both, 2 of 199 instructions differ in tla-en.
  ratio = Q16_ONE;
  if (scale >= ratio) {
    wrap = 0x2000;
    horizon = 0x6800 + -(s16)camera->yaw * 3;
  }
  if (scanlines == NULL)
    return;
  if ((view->mode == 1 || view->secondMode == 1) && view->busy == 0)
    gBgScroll[1].x = horizon >> 8;
  if (view->mode != 2)
    return;
  line = scanlines->lines[scanlines->page ^ 1];
  ratio = Iwram_RatioMulQ14(scale, ratio);
  step = ratio >> 8;
  affine = &scanlines->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulQ16(ratio, Iwram_MulQ16(centerX, scale - 0x10000));
  y = Iwram_MulQ16(ratio, Iwram_MulQ16(centerY, scale - 0x10000));
  affine->x = ((x + 0x7fff) >> 8) + offsetX + horizon;
  affine->y = ((y + 0x7fff) >> 8) + offsetY - 0x1000;
  groundEndLine =
      (Iwram_RatioMulQ14((s16)affine->pd, 0x4000 - affine->y) >> 16) + 1;
  scanline = 0;
  if (!GameFlag_Test(0x16b)) {
    for (; scanline < 16; scanline++)
      *line++ = 0x3f8e;
  }
  if (groundEndLine > 136)
    groundEndLine = 136;
  for (; scanline < groundEndLine; scanline++)
    *line++ = wrap | 0x478a;
  for (; scanline < 136; scanline++)
    *line++ = wrap | 0x478e;
  for (; scanline < 160; scanline++)
    *line++ = 0x3f8e;
  scanlines->page ^= 1;
}

/* VIEW(f11e3ac): routine Projection_Set as void Projection_Set(s32 span, u32 depth, s32 range); module/system/system.h has void Projection_Set(u32 value0, u32 value1, u32 value2). */
extern void Projection_Set__f11e3ac(s32 span, u32 depth, s32 range) __asm__("Projection_Set");
/* VIEW(f11e3ac): routine SceneTransform_ApplyPosition as void SceneTransform_ApplyPosition(s32 *position); module/system/system.h has void SceneTransform_ApplyPosition(const s32 *pos). */
extern void SceneTransform_ApplyPosition__f11e3ac(s32 *position) __asm__("SceneTransform_ApplyPosition");

#ifdef UBFIX
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom) {
#else
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y) {
#endif
  s32 span = 0x01fe0000;
  // UB: zoom is uninitialized; reads entry r4 directly (lsls r0, r4, #16).
#ifndef UBFIX
  s32 zoom;
#endif
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->pos;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set__f11e3ac(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition__f11e3ac(pos);
  SceneTransform_ApplyYaw((s16)scene->yaw);
  SceneTransform_ApplyPitch((s16)scene->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = scene->distance;
  Iwram_TransformVector(renderBounds, scene->eye);
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set__f11e3ac(width, result, half * 4);
}

// BattleCamera_SetView: The Broken Seal's function of that name
// with the camera from its heap slot. Places the battle camera at a position
// and zoom for a transition: sets the projection, turns the scene to the
// camera's yaw and pitch, projects the origin and gives the camera its
// range from where the origin lands on the screen.

void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->pos;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 span = 0x01fe0000;
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set__f11e3ac(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition__f11e3ac(pos);
  SceneTransform_ApplyYaw((s16)scene->yaw);
  SceneTransform_ApplyPitch((s16)scene->pitch);
  {
    /* FAKEMATCH(pin): the camera pinned to r1, copied before the bound
     * stores; as Iwram_TransformVector(renderBounds, (s32 *)scene), 2 of
     * 111 instructions differ in tla-en. */
    register s32 *camera asm("r1") = (s32 *)scene;

    renderBounds[0] = 0;
    renderBounds[1] = 0;
    renderBounds[2] = span;
    Iwram_TransformVector(renderBounds, camera);
  }
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set__f11e3ac(width, result, half * 4);
}

// Battle presentation: center the camera between two objects.

/// Centres the camera on two objects' midpoint in x and z.
/// a, b: object IDs.
/// zoom: scale in percent.
void BattleCamera_SetViewAtPairMidpoint(s32 a, s32 b, s32 zoom) {
  struct MotionObject *obj_a = BattleObject_Get(a)->object;
  struct MotionObject *obj_b = BattleObject_Get(b)->object;
  s32 ax = obj_a->x;
  s32 bx = obj_b->x;
  s32 az = obj_a->z;
  s32 bz = obj_b->z;
  s32 x = (bx + ax) / 2;
  s32 z = (bz + az) / 2;

  BattleCamera_SetView(x, 0, z, zoom);
}

/// Fades BG2 out over the backdrop in eight frames, its blend weight falling
/// by 2 a frame from 15, while the backdrop's stays 16. Leaves REG_BLDCNT
/// blending BG2 over the backdrop.
void BattleBackground_FadeAlphaOut(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= 16; step += 2) {
    // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
    // code.
    REG_BLDALPHA = BLEND_ALPHA(16, 16) - step;
    wait(1);
  }
}

/// Fades BG2 in over the backdrop in eight frames, its blend weight rising by
/// 2 a frame from 1, while the backdrop's stays 16. Leaves REG_BLDCNT
/// blending BG2 over the backdrop.
void BattleBackground_FadeAlphaIn(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
  // code.
  for (step = 1; step <= 16; step += 2) {
    REG_BLDALPHA = step + BLEND_ALPHA(0, 16);
    wait(1);
  }
}

void Func_081269ac(void) { REG_BLDCNT = 0xbf; }

s32 Func_081269bc(s32 a, s32 b) { return Iwram_MulQ16(a, b); }

struct BattleTransitionEntry {
  u32 value;
  u32 sum;
  u32 field8;
  u32 fieldc;
};

void BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries) {
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(entries);
  entries[0].sum = previous + entries[0].value;
}

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void Func_0814ea58(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Shop_InitializeWork(void);
s32 Inn_RoomPrice(s32 innId);
void Shop_ReleaseWork(void);

s32 Func_081269ec(s32 value) { return value / 16; }

// What Select picks: another value in the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DEBUG_VALUE_SELECT 411
#else
#define DEBUG_VALUE_SELECT 544
#endif

struct DebugBattleWork {
  u8 unk000[0x854];
  s32 debugValue;
};

/// A debug choice of a battle value: with Start held, the d-pad and L/R step
/// the battle work's word at 0x854 (by 1, 10 and 100) until A takes it; Select
/// held picks DEBUG_VALUE_SELECT and R held 399; otherwise value comes back
/// unchanged.
s32 Func_081269f8(s32 value) {
  if (gInput.held & 8) {
    while (1) {
      struct DebugBattleWork *work =
          (struct DebugBattleWork *)Ram_HeapSlots->battleWork;

      if (gInput.repeat & 0x20)
        work->debugValue--;
      if (gInput.repeat & 0x10)
        work->debugValue++;
      if (gInput.repeat & 0x200)
        work->debugValue -= 100;
      if (gInput.repeat & 0x100)
        work->debugValue += 100;
      if (gInput.repeat & 0x40)
        work->debugValue -= 10;
      if (gInput.repeat & 0x80)
        work->debugValue += 10;
      if (gInput.pressed & 1) {
        value = work->debugValue;
        break;
      }
      wait(1);
    }
  }
  if (gInput.held & 4)
    value = DEBUG_VALUE_SELECT;
  // The held keys reread through a volatile pointer, a load apart from the
  // Select test's; as gInput.held, 11 of 114 instructions differ in tla-en.
  if (*(volatile u32 *)&gInput.held & 0x100)
    value = 399;
  return value;
}

// Battle presentation: toggle translucency on unit graphics.

struct ActorSpritePart {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u8 unk06[0x16];
};

struct ActorSpriteRecord {
  struct ActorSpritePart part[2];
};

/// Sets the OBJ mode of both sprite parts of each of the actor's records.
void BattleHighlight_SetSpriteMode(s32 actorId, s32 mode) {
  struct BattleObj *slot = BattleObject_Get(actorId);
  struct MotionObject *object;
  struct ActorSpriteRecord *record;
  struct ActorSpriteRecord **records;
  s32 i;

  if (slot == 0)
    return;
  object = slot->object;
  if (object == 0)
    return;
  switch (object->recordStorageKind & 15) {
  case 1: {
    record = object->records;
    record->part[0].objectMode = mode;
    record->part[1].objectMode = mode;
    break;
  }
  case 2: {
    records = object->records;
    i = 0;
    do {
      record = *records++;
      if (record == 0)
        break;
      record->part[0].objectMode = mode;
      record->part[1].objectMode = mode;
      i++;
    } while (i <= 3);
    break;
  }
  }
}

/// Makes every unit's graphics but except's translucent (OBJ mode 1).
/// except: unit ID to leave unchanged.
void BattleHighlight_SetOtherSpriteModes(s32 except) {
  s16 list[14];
  s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  s32 i;

  for (i = 0; i < cnt; i++) {
    if (list[i] != except)
      BattleHighlight_SetSpriteMode(list[i], 1);
  }
}

/// Restores normal rendering (OBJ mode 0) for all units.
void BattleHighlight_ClearAllSpriteModes(void) {
  s16 list[14];
  s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  s32 i;

  for (i = 0; i < cnt; i++)
    BattleHighlight_SetSpriteMode(list[i], 0);
}

// Steps the battle highlight's blend.

// The battle work block, as this function reads it.
struct HighlightWork {
  u8 unk000[0x676];
  u16 highlightStep;
  u16 highlightMode;
};

extern s8 gBattleEffect_HighlightBlendRamp[];

/// Advances the battle highlight while its mode is set: modes 6 and 4 fade the
/// blend in or out over 32 frames at half steps, mode 2 blends by the
/// sixteen-step ramp, and any other nonzero mode brightens by the ramp.
void Graphics_AdvancePaletteCycle(void) {
  struct HighlightWork *battle =
      (struct HighlightWork *)Ram_HeapSlots->battleWork;

  if (battle != NULL) {
    if (battle->highlightMode == 6) {
      u32 level = battle->highlightStep >> 1;

      if (level > 16)
        level = 16;
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA = ((0x10 - level) << 8) | level;
      battle->highlightStep++;
    } else if (battle->highlightMode == 4) {
      u32 level = battle->highlightStep >> 1;

      if (level > 16)
        level = 16;
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA = (level << 8) | (0x10 - level);
      battle->highlightStep++;
    } else if (battle->highlightMode == 2) {
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA =
          (gBattleEffect_HighlightBlendRamp[battle->highlightStep] << 8) |
          (0x10 - gBattleEffect_HighlightBlendRamp[battle->highlightStep]);
      battle->highlightStep = ((battle->highlightStep + 1) & 0xf) |
                              (battle->highlightStep > 14 ? 0x10 : 0);
    } else if (battle->highlightMode != 0) {
      REG_BLDCNT = 0x3f90;
      REG_BLDALPHA = 0x10;
      REG_BLDY = gBattleEffect_HighlightBlendRamp[battle->highlightStep];
      battle->highlightStep = ((battle->highlightStep + 1) & 0xf) |
                              (battle->highlightStep > 14 ? 0x10 : 0);
    }
  }
}

/// Mode 0 clears every actor's OBJ mode and stops the palette cycle. Any other
/// mode starts the cycle: every actor takes mode & 1, and the actors listed
/// (up to 14, ended by 0xff) take the other.
void BattleHighlight_SetModes(u16 *actors, s32 mode) {
  s16 activeActors[14];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  u32 count;
  u32 i;

  if (mode == 0) {
    Task_Remove(Graphics_AdvancePaletteCycle);
    Io_Write16(mode, &REG_BLDY);
    BattleHighlight_ClearAllSpriteModes();
    wait(1);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  }
  if (battle != NULL && mode != 0) {
    battle->highlightMode = mode;
    battle->highlightStep = 0;
    REG_BLDY = 0;
    Io_Write16(16, &REG_BLDALPHA);

    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)activeActors);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(activeActors[i], mode & 1);

    if (actors != NULL) {
      u32 actor = *actors;

      i = 0;
      actors++;
      if (actor != 0xff) {
        do {
          BattleHighlight_SetSpriteMode(actor, (mode & 1) ^ 1);
          i++;
          if (i > 13)
            break;
          actor = *actors++;
        } while (actor != 0xff);
      }
    }
    wait(1);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
    Task_Add(Graphics_AdvancePaletteCycle, TASK_PASS_FRAME);
  }
}

struct Spark {
  s32 pos[6]; // x, y, z and their velocities, 16.16
  s32 life;
};

struct Ring {
  s32 x;
  s32 y;
  s32 velX;
  s32 velY;
  s32 age;
};

struct SparkWork {
  u8 sheet[0x11c0];
  struct Spark sparks[16];
  struct Ring rings[3];
  s32 frames;
  s32 ready;
  s32 originX;
  s32 originY;
  s32 originPending;
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width,
                         s32 height);

// The Broken Seal's star tables: each frame's size and sheet offset, and
// the ring's four frames.
extern const u8 Data_08129828[];
extern const s32 Data_0812980c[];
extern const s32 Data_08129830[];

// One frame of the star field: sixteen sparks fall in toward the centre
// along a damped spiral and shrink as they near it; while the field is
// young (24 frames) a spark that dies is born again on a random ring.
// Then three rings drift outward through their four frames. The canvas
// is marked ready for the transfer callback at the end.
void ElementGlow_UpdateStarField(void) {
  s32 j;
  s32 frame;
  u32 size;
  s32 ringX;
  s32 ringY;
  s32 phase;
  void *dest;
  s32 i;
  struct SparkWork *work;
  DrawFunc draw;
  struct Spark *spark;
  struct Ring *ring;
  s32 *pos;
  s32 k;
  s32 dist;
  s32 scale;
  s32 step;
  u32 radius;
  s32 angle;
  u32 half;
  s32 x;
  s32 y;
  s32 life;
  s32 (*mul)(s32 a, s32 b);

  dest = Ram_HeapSlots->battleFxWork[1];
  work = (struct SparkWork *)Ram_HeapSlots->battleFxWork[0];
  work->ready = 0;
  draw = (DrawFunc)Ram_HeapSlots->unk64[1];
  for (i = 0; i < 16; i++) {
    spark = &work->sparks[i];
    life = spark->life;
    if (life != 0) {
      dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8) +
                       (spark->pos[1] >> 8) * (spark->pos[1] >> 8) +
                       (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
      if (dist <= 0xfff) {
        spark->life = 0;
      } else {
        scale = Iwram_RatioMulQ14(dist, 0x10000);
        spark->life--;
        // The multiply's entry held in a local keeps its address ahead of the
        // cursor and the axis count; called directly, 2 of 267 instructions
        // differ in tla-en.
        mul = Iwram_MulQ16;
        pos = spark->pos;
        for (k = 2; k >= 0; k--) {
          step = mul(mul(-*pos >> 8, scale), 0x13000);
          pos[3] = pos[3] - (pos[3] >> 7) + step;
          *pos += pos[3];
          pos++;
        }
      }
      life = spark->life;
    }
    if (life == 0 && work->frames <= 24) {
      angle = Random16();
      radius = Random16() + 0x10000;
      half = radius >> 1;
      spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
      spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
      if (spark->pos[0] & 1)
        spark->pos[0] = -spark->pos[0];
      if (spark->pos[1] & 1)
        spark->pos[1] = -spark->pos[1];
      spark->pos[2] = (u32)(Random16() + 0x8000) >> 2;
      spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
      spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
      spark->pos[5] = 0;
      life = spark->life = (radius >> 13) + 1;
    }
    if (life != 0) {
      x = (spark->pos[0] >> 10) + 64;
      y = (spark->pos[1] >> 10) + 64;
      frame = life;
      if (frame < 0)
        frame = 0;
      else if (frame > 6)
        frame = 6;
      size = Data_08129828[frame];
      draw(dest, (u8 *)work + Data_0812980c[frame], x - (size >> 1),
           y - (size >> 1), size, size);
    }
  }
  draw = (DrawFunc)Ram_HeapSlots->unkb8[1];
  for (ring = work->rings, j = 2; j >= 0; j--, ring++) {
    ring->x += ring->velX;
    ring->y += ring->velY;
    ringX = ring->x >> 10;
    ringY = ring->y >> 10;
    phase = 3 - ring->age / 8;
    if (phase >= 0) {
      ring->age++;
      /* FAKEMATCH(dead): an empty once-only block after the age's store, so
       * Data_08129830's address loads after it; without it, 2 of 267
       * instructions differ in tla-en. */
      do {
      } while (0);
      draw(dest, (u8 *)work + Data_08129830[phase], ringX + 48, ringY + 48, 32,
           32);
    }
  }
  work->frames++;
  work->ready = 1;
}

s32 ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);

s32 ElementGlow_UploadStarField(void) {
  void *source = Ram_HeapSlots->battleFxWork[1];
  u8 *context = Ram_HeapSlots->battleFxWork[0];

  if (source != 0) {
    u32 *active = (u32 *)(context + 0x13c0);

    if (*active != 0) {
      *active = 0;
      return ColorBuffer_ScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
    }
  }
  return (s32)source;
}

extern char ResourceId_YellowPaletteB;
extern char ResourceId_BlueRingSheet;
extern char ResourceId_RedPaletteC;
extern char ResourceId_VioletPaletteD;

s32 Resource_DecodeType01(const void *source, void *destination);
s32 Func_08138048(s32 slot, s32 modeAndFlags);
// The Broken Seal's five-argument blitter loader, named but never called.
s32 BattleEffect_BuildBlitterFar(s32 slot, s32 source, s32 destination,
                                 s32 flags, s32 mode);

// The rings' vectors go through these two polar helpers, as the native
// rebuilds 0x200 at each call (movs, lsls) and keeps only the radius and the
// multiply's entry in registers; written out, loop.c hoists 0x200 too (81 of
// 255 differ, 2026-11-08).
static __inline__ s32 PolarX(s32 angle, s32 radius) {
  return Iwram_MulQ16(Trig_Cos(angle), radius);
}

static __inline__ s32 PolarY(s32 angle, s32 radius) {
  return Iwram_MulQ16(Trig_Sin(angle), radius);
}

/// Sets up the star field behind a summon, the twin of The Broken Seal's
/// ElementGlow_InitializeStarField: sixteen sparks on random rings around the
/// centre, three rings leaving it at thirds of a turn, the blue ring sheet and
/// its palette with the mode's colour (0 yellow, 1 blue, 2 red, else violet)
/// over the first 32 colours, BG2 unscaled at the origin, the additive and max
/// blitters (Func_08138048, the veneer of BattleEffect_LoadBlitter), and the
/// two frame callbacks.
s32 ElementGlow_InitializeStarField(s32 mode) {
  struct SparkWork *work;
  struct Spark *spark;
  struct Ring *ring;
  s32 i;
  u8 *data;
  s32 zero;

  ((s32 *)Ram_HeapSlots->transitionWork)[2] = 1;
  work = HeapSlot_AllocEwram(92, sizeof *work);
  HeapSlot_Alloc(96, 0x4000);
  for (i = 0; i < 16; i++) {
    s32 angle = Random16();
    u32 radius = Random16() + 0x10000;
    u32 half = radius >> 1;

    spark = &work->sparks[i];
    spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
    spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
    if (spark->pos[0] & 1)
      spark->pos[0] = -spark->pos[0];
    if (spark->pos[1] & 1)
      spark->pos[1] = -spark->pos[1];
    spark->pos[2] = (u32)(Random16() + 0x8000) >> 2;
    spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
    spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
    spark->pos[5] = 0;
    spark->life = (radius >> 13) + 1;
  }
  for (i = 0; i < 3; i++) {
    s32 radius = 0x1000;
    s32 angle;

    ring = &work->rings[i];
    angle = i * 0x5555;
    ring->x = PolarX(angle, radius);
    ring->y = PolarY(angle, radius);
    ring->velX = PolarX(angle, 0x200);
    ring->velY = PolarY(angle, 0x200);
    ring->age = 0;
  }
  work->frames = 0;
  work->ready = 0;
  work->originPending = 0;
  Iwram_ClearWordsEntry(Ram_HeapSlots->battleFxWork[1], 0x4000);
  data = Resource_GetTableEntry__f11e3ac((s32)&ResourceId_BlueRingSheet);
  Iwram_CopyWordsEntry((void *)BG_PLTT, data, 128);
  data += 128;
  Resource_DecodeType01(data, work);
  switch (mode) {
  case 0:
    data = Resource_GetTableEntry__f11e3ac((s32)&ResourceId_YellowPaletteB);
    break;
  case 1:
    data = Resource_GetTableEntry__f11e3ac((s32)&ResourceId_BlueRingSheet);
    break;
  case 2:
    data = Resource_GetTableEntry__f11e3ac((s32)&ResourceId_RedPaletteC);
    break;
  default:
    data = Resource_GetTableEntry__f11e3ac((s32)&ResourceId_VioletPaletteD);
    break;
  }
  Dma_Set(data, (void *)BG_PLTT, 0x84000000 | 32, REG_DMA3);
  // A once-only block, as in The Broken Seal: it keeps the zero ahead of the
  // BG2X address, which reload derives from the DMA register's (8 without it).
  do {
    zero = 0;
    REG_BG2X = zero;
    REG_BG2Y = zero;
  } while (0);
  Io_Write16(0x100, &REG_BG2PA);
  Io_Write16(0, &REG_BG2PB);
  Io_Write16(0, &REG_BG2PC);
  Io_Write16(0x100, &REG_BG2PD);
  /* FAKEMATCH(dead): a never-taken five-argument call, which reserves the
   * frame's outgoing argument word below mode; as no call, 10 of 255
   * instructions differ in tla-en (6 with an unused u8[4] in its place). */
  if (0)
    BattleEffect_BuildBlitterFar(104, 7, 7, 3, 3);
  Func_08138048(104, 0x03);
  Func_08138048(188, 0x13);
  Task_Add(ElementGlow_UpdateStarField, 0x480);
  return Task_Add(ElementGlow_UploadStarField, 0x480);
}

/// Ends a star field: clears the tiles in character block 1, releases its
/// four heap slots, turns BG2 off and removes both its tasks. Called by
/// Func_08127308 once mode 0's field has played. Returns Task_Remove's
/// result, which the caller does not read.
s32 ElementGlow_End(s32 unused) {
  Iwram_ClearWordsEntry((void *)0x06004000, 0x4000);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  Task_Remove(ElementGlow_UploadStarField);
  return Task_Remove(ElementGlow_UpdateStarField);
}

void Func_08127304(void) {}

// The parts of the battle work this effect uses.
struct ElementSession {
  u8 unk000[0x56c];
  u16 palette[128];
  s32 brightness;
  u16 background;
};

s32 BattleMotion_ProjectScaledPosition(s32 slot, s32 *screen);
void Func_08138020(struct BattlePresentationWork *launch);
void Func_08138030(struct BattlePresentationWork *launch);
void Func_08138038(s32 mode);
void Func_08138040(void);

// Plays a unit's element effect in one of four ways: 0, the star field
// around the unit while the backdrop darkens over 25 frames (45 frames);
// 1, the prepared canvas scene of the kind, centred on the unit (40
// frames); 2, the ice fleck burst; 3, the gathering burst.
void Func_08127308(s32 unit, s32 kind, s32 mode, s32 variant) {
  struct EffectPosition position;
  struct EffectPosition position2;
  struct BattlePresentationWork launch;
  struct BattlePresentationWork launch2;
  s32 origin[2];
  struct ElementSession *session =
      (struct ElementSession *)Ram_HeapSlots->battleWork;
  volatile u16 *ime;
  struct SparkWork *work;
  s32 frame;
  s32 *brightness;
  struct EffectPosition *pos;

  wait(1);
  Func_0812628c(1, session->background, 0);
  Iwram_ClearWordsEntry((void *)0x06004000, 0x4000);
  QueueIoWriteDelay2(0x04000000, 0x3741);
  QueueIoWriteDelay2(0x0400000c, 0x784);
  QueueIoWriteDelay2(0x04000050, 0x3f44);
  wait(1);
  Io_Write16(240, &REG_WIN0H);
  Io_Write16(0x1088, &REG_WIN0V);
  Io_Write16(63, &REG_WININ);
  Io_Write16(17, &REG_WINOUT);
  if (mode == 0) {
    QueueIoWriteDelay2(0x04000052, 0x100e);
    ElementGlow_InitializeStarField(kind);
    for (frame = 0, pos = &position; frame <= 44; frame++) {
      brightness = &session->brightness;
      ime = &REG_IME;
      work = (struct SparkWork *)Ram_HeapSlots->battleFxWork[0];
      if (frame <= 24)
        ColorBuffer_Scale__f11f66c(session->palette, (u16 *)0x050000c0,
                          *brightness = 0x10000 - frame * 1092, 128);
      BattleMotion_ProjectScaledPosition(unit, (s32 *)pos);
      work->originX = (64 - pos->x) << 8;
      work->originY = (64 - pos->y) << 8;
      IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000028, (u32)&work->originX, 0x84000002);
      work->originPending = 1;
      wait(1);
    }
    ElementGlow_End(kind);
  } else if (mode == 1) {
    Func_08138038(kind);
    for (frame = 0; frame < 40; frame++) {
      BattleMotion_ProjectScaledPosition(unit, (s32 *)&position2);
      origin[0] = (64 - position2.x) << 8;
      origin[1] = (64 - position2.y) << 8;
      IO_WRITE_QUEUE_PUSH(0x04000028, (u32)origin, 0x84000002);
      wait(1);
    }
    Func_08138040();
  } else if (mode == 2) {
    launch.flags = 0;
    launch.kind = kind;
    launch.variant = variant;
    launch.actor = unit;
    launch.actors[0] = unit;
    launch.firstTarget = unit;
    launch.count = 1;
    launch.initialValue = 1;
    Func_08138020(&launch);
  } else {
    launch2.flags = 0;
    launch2.kind = kind;
    launch2.variant = 0;
    launch2.actor = unit;
    launch2.firstTarget = unit;
    launch2.actors[0] = unit;
    launch2.count = 1;
    launch2.initialValue = 1;
    Func_08138030(&launch2);
  }
}

/// Puts the background's palette back undarkened, at level 0.
/// Called by Func_0811df70 and BattleStaging_RunPsynergyCast.
void BattleBackground_RestorePalette(void) {
  BattleBackground_SetPaletteLevel(0, 0);
}

/// Removes the VCount interrupt's handler; nothing calls it.
void Irq_RemoveVCountHandler(void) { Irq_SetHandler(IRQ_VCOUNT, 0, 0); }

/// The Broken Seal's BattleFormation_AverageLevel without its level check:
/// the average level of a formation's enemy kinds, counting only kinds the
/// player has met (flag 0x608 + id, or all with flag 0x174); -1 for an empty
/// formation, -2 when one present kind is unmet, -3 when none counts.
s32 Func_08127588(s32 recordId) {
  struct BattleFormationRecord *record;
  const struct EnemyDefinition *enemy;
  u32 memberIndex;
  s32 enemyId;
  s32 levelTotal;
  s32 enemyCount;
  // Unused: gives the stack frame its 28 bytes.
  UNUSED s32 levels[7];

  enemyCount = 0;
  levelTotal = 0;
  record = &gBattleFormation_Records[recordId];
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->minimumCounts[memberIndex] != 0)
      break;
  }
  if (memberIndex == 5)
    return -1;
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->maximumCounts[memberIndex] == 0)
      continue;
    enemyId = record->memberIds[memberIndex];
    enemy = Enemy_GetDefinitionFar(enemyId + 8);
    if (enemy == 0)
      continue;
    if (!GameFlag_Test(0x174) && !GameFlag_Test(enemyId + 0x608))
      return -2;
    levelTotal += enemy->level;
    enemyCount++;
  }
  if (enemyCount == 0)
    return -3;
  return levelTotal / enemyCount;
}
