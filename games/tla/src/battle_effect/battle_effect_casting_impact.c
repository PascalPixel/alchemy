/// @file
/// The casting impacts, and the step of the background colours toward a
/// palette.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Steps background colours 0 to 63 one level towards a palette resource.
void BattleEffect_StepPaletteToResource(s32 resourceId) {
  u16 target[SHEET_PALETTE_COLORS];
  u16 *color = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resourceId), target,
          DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
  target[0] = 0;
  for (i = 0; i != SHEET_PALETTE_COLORS; i++, color++) {
    s32 red = RGB_RED(*color);
    s32 green = RGB_GREEN(*color);
    s32 blue = RGB_BLUE(*color);
    s32 targetRed = RGB_RED(target[i]);
    s32 targetGreen = RGB_GREEN(target[i]);
    s32 targetBlue = RGB_BLUE(target[i]);

    if (red < targetRed)
      red++;
    else if (red > targetRed)
      red--;
    if (green < targetGreen)
      green++;
    else if (green > targetGreen)
      green--;
    if (blue < targetBlue)
      blue++;
    else if (blue > targetBlue)
      blue--;
    target[i] = RGB(red, green, blue);
  }
  Dma_Set(target, (void *)BG_PLTT, DMA_COPY_HALFWORDS(SHEET_PALETTE_COLORS),
          REG_DMA3);
}

typedef s32 (*DrawFunc)(void *, const void *, s32, s32, s32, s32);

/// The two blitters an impact draws with.
struct DrawFuncs {
  DrawFunc draw[2];
};

#define BG_VRAM ((void *)BG_CHAR_BLOCK(1))
#define SPARKS sparks
#define RINGS ((struct Particle *)(Ram_MapCellBuffer + 0x6e00))
#define BLITTER ((DrawFunc)Ram_HeapSlots->ramCode)
#define ALT_BLITTER ((DrawFunc)Ram_HeapSlots->ramCodePaired)
/// The casting impacts, by effect kind less 100: each is named for the first
/// ability whose effect entry selects it as a casting impact; those no ability
/// selects are named UNUSED_ and for what they load or share. Kinds up to
/// CASTING_IMPACT_LAST_DJINNI are Djinn; Terra Strike on are the enemies'
/// abilities.
enum CastingImpact {
  CASTING_IMPACT_IMPACT_BANE = 0,
  CASTING_IMPACT_IMPACT_SMOG,
  CASTING_IMPACT_IMPACT_MIST,
  CASTING_IMPACT_IMPACT_FEVER,
  CASTING_IMPACT_IMPACT_FLINT,
  CASTING_IMPACT_IMPACT_SCORCH,
  CASTING_IMPACT_IMPACT_SQUALL,
  CASTING_IMPACT_IMPACT_SAP,
  CASTING_IMPACT_IMPACT_ECHO,
  CASTING_IMPACT_IMPACT_SOUR,
  CASTING_IMPACT_IMPACT_CHILL,
  CASTING_IMPACT_IMPACT_GEL,
  CASTING_IMPACT_IMPACT_STEEL,
  CASTING_IMPACT_IMPACT_FOG,
  CASTING_IMPACT_IMPACT_UNUSED_BOLTS,
  CASTING_IMPACT_IMPACT_UNUSED_SKULLS,
  CASTING_IMPACT_IMPACT_WHEEZE,
  CASTING_IMPACT_IMPACT_BLITZ,
  CASTING_IMPACT_IMPACT_TERRA_STRIKE = 38,
  CASTING_IMPACT_IMPACT_POISON_CLOUD,
  CASTING_IMPACT_IMPACT_POISON_DEATH,
  CASTING_IMPACT_IMPACT_MORTAL_DANGER, ///< also Death Scythe
  CASTING_IMPACT_IMPACT_BAD_OMEN,
  CASTING_IMPACT_IMPACT_LIFE_NOURISH,
  CASTING_IMPACT_IMPACT_AQUA_SOCK,
  CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS,
  CASTING_IMPACT_IMPACT_FROST_BITE,
  CASTING_IMPACT_IMPACT_UNUSED_GATHERING,
  CASTING_IMPACT_IMPACT_LIFE_LEECH,
  CASTING_IMPACT_IMPACT_PSY_LEECH,
  CASTING_IMPACT_IMPACT_BROIL,
  CASTING_IMPACT_IMPACT_MELTDOWN,
  CASTING_IMPACT_IMPACT_HEAT_MIRAGE, ///< also Heat Flash
  CASTING_IMPACT_IMPACT_BARRAGE,
  CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME,
  CASTING_IMPACT_IMPACT_ACID_BATH,
  CASTING_IMPACT_IMPACT_UNUSED_COMETS,
  CASTING_IMPACT_IMPACT_STUN_VOLTAGE,
  CASTING_IMPACT_IMPACT_BLINDING_SMOG,
  CASTING_IMPACT_IMPACT_MURK,
  CASTING_IMPACT_IMPACT_CYCLONE_SLASH,
  CASTING_IMPACT_IMPACT_PSYPHON_SEAL,
  CASTING_IMPACT_IMPACT_ASURA,
  CASTING_IMPACT_IMPACT_FORCIBLE_ARM, ///< also Grand Golem
  CASTING_IMPACT_IMPACT_TRUNCHEON_FIST,
  CASTING_IMPACT_IMPACT_SEVERE_BLOW,
  CASTING_IMPACT_IMPACT_POWER_DRIVE,
  CASTING_IMPACT_IMPACT_HEAT_SMASH,
  CASTING_IMPACT_IMPACT_STUN_CLOUD,
  CASTING_IMPACT_IMPACT_SEARING_FOG,
  CASTING_IMPACT_IMPACT_ICE_CRUSH,
  CASTING_IMPACT_IMPACT_SARGASSO,
  CASTING_IMPACT_IMPACT_EVIL_EYE,
  CASTING_IMPACT_IMPACT_VEIN_TAP,
  CASTING_IMPACT_IMPACT_SCORPIONFISH,
  CASTING_IMPACT_IMPACT_MOON_AIR,
  CASTING_IMPACT_IMPACT_FLASH_EDGE,
  CASTING_IMPACT_IMPACT_HIGH_VITALS,
  CASTING_IMPACT_IMPACT_FLARE_BURST,
  CASTING_IMPACT_IMPACT_HAMMERSPHERE,
  CASTING_IMPACT_IMPACT_FLASH_FORCE,
  CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE,
  CASTING_IMPACT_IMPACT_REVERSE_STAR,
  CASTING_IMPACT_IMPACT_SUPERNOVA,
  CASTING_IMPACT_IMPACT_STUN_BOLT,
  CASTING_IMPACT_IMPACT_SARCOPHAGUS,
  CASTING_IMPACT_IMPACT_RAIDENS_WRATH,
  CASTING_IMPACT_IMPACT_UNUSED_TILED_CANVAS,
  CASTING_IMPACT_IMPACT_APOCALYPSE,
  CASTING_IMPACT_IMPACT_FLASH_PUNCH,
  CASTING_IMPACT_IMPACT_ARMOR_CRUSH,
  CASTING_IMPACT_IMPACT_STUN_JIP,
  CASTING_IMPACT_IMPACT_QUICK_SLASH,
  CASTING_IMPACT_IMPACT_BOSCA_HIT,
  CASTING_IMPACT_IMPACT_MOTHER_EARTH,
  CASTING_IMPACT_IMPACT_ROC,       ///< also Orc
  CASTING_IMPACT_IMPACT_WILD_WOLF, ///< also Fatal Fang
  CASTING_IMPACT_IMPACT_DINOX,     ///< also Power Bite
  CASTING_IMPACT_IMPACT_TERRIBLE_BITE,
  CASTING_IMPACT_IMPACT_CALL_ZOMBIE,
  CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI,
  /// The last of the kinds a Djinni selects.
  CASTING_IMPACT_LAST_DJINNI = 37
};

/// The impact orb's pictures, packed at half width, after the streak's.
#define CASTING_IMPACT_ORB_PICTURES (6 * PICTURE_BYTES(FIRE_STREAK))
/// The Djinni's picture, in Ram_MapGridCellBuffer after the impact's own.
#define CASTING_IMPACT_DJINNI_WIDTH 20
#define CASTING_IMPACT_DJINNI_HEIGHT 40
#define CASTING_IMPACT_DJINNI_PICTURE 15446
#define CASTING_IMPACT_GATHER_FRAMES 32 ///< frames the sparks gather
#define CASTING_IMPACT_SPARKS 64        ///< sparks in work->particles
/// The impact pictures' particles, SPARKS, in the map cell buffer after the
/// Djinni.
#define CASTING_IMPACT_IMPACT_PARTICLE_COUNT 64

/// Asura's sheet: the masked swordsman with his sword low, then swung up, then
/// the red crescent his stroke leaves.
#define ASURA_SHEET_LOW_WIDTH 80
#define ASURA_SHEET_LOW_HEIGHT 96
#define ASURA_SHEET_SWUNG_WIDTH 80
#define ASURA_SHEET_SWUNG_HEIGHT 104
#define ASURA_SHEET_CRESCENT_WIDTH 128
#define ASURA_SHEET_CRESCENT_HEIGHT 91
#define ASURA_SHEET_SWUNG PICTURE_BYTES(ASURA_SHEET_LOW)
#define ASURA_SHEET_CRESCENT                                                   \
  (ASURA_SHEET_SWUNG + PICTURE_BYTES(ASURA_SHEET_SWUNG))

/// RedCrescentSheetB, Asura's crescent fading in four pictures the width of
/// the canvas.
#define ASURA_CRESCENT_FADE_A_HEIGHT 91
#define ASURA_CRESCENT_FADE_B_HEIGHT 59
#define ASURA_CRESCENT_FADE_C_HEIGHT 29
#define ASURA_CRESCENT_FADE_D_HEIGHT 26
#define ASURA_CRESCENT_FADE_B (CANVAS_SIZE * ASURA_CRESCENT_FADE_A_HEIGHT)
#define ASURA_CRESCENT_FADE_C                                                  \
  (ASURA_CRESCENT_FADE_B + CANVAS_SIZE * ASURA_CRESCENT_FADE_B_HEIGHT)
#define ASURA_CRESCENT_FADE_D                                                  \
  (ASURA_CRESCENT_FADE_C + CANVAS_SIZE * ASURA_CRESCENT_FADE_C_HEIGHT)

/// Sarcophagus's sheet: the coffin, then seven pictures of the ghost rising
/// out of it, one after another.
#define SARCOPHAGUS_COFFIN_WIDTH 69
#define SARCOPHAGUS_COFFIN_HEIGHT 96
#define SARCOPHAGUS_GHOST_A PICTURE_BYTES(SARCOPHAGUS_COFFIN)
#define SARCOPHAGUS_GHOST_A_WIDTH 59
#define SARCOPHAGUS_GHOST_A_HEIGHT 24
#define SARCOPHAGUS_GHOST_B                                                    \
  (SARCOPHAGUS_GHOST_A + PICTURE_BYTES(SARCOPHAGUS_GHOST_A))
#define SARCOPHAGUS_GHOST_B_WIDTH 70
#define SARCOPHAGUS_GHOST_B_HEIGHT 27
#define SARCOPHAGUS_GHOST_C                                                    \
  (SARCOPHAGUS_GHOST_B + PICTURE_BYTES(SARCOPHAGUS_GHOST_B))
#define SARCOPHAGUS_GHOST_C_WIDTH 84
#define SARCOPHAGUS_GHOST_C_HEIGHT 55
#define SARCOPHAGUS_GHOST_D                                                    \
  (SARCOPHAGUS_GHOST_C + PICTURE_BYTES(SARCOPHAGUS_GHOST_C))
#define SARCOPHAGUS_GHOST_D_WIDTH 75
#define SARCOPHAGUS_GHOST_D_HEIGHT 57
#define SARCOPHAGUS_GHOST_E                                                    \
  (SARCOPHAGUS_GHOST_D + PICTURE_BYTES(SARCOPHAGUS_GHOST_D))
#define SARCOPHAGUS_GHOST_E_WIDTH 67
#define SARCOPHAGUS_GHOST_E_HEIGHT 86
#define SARCOPHAGUS_GHOST_F                                                    \
  (SARCOPHAGUS_GHOST_E + PICTURE_BYTES(SARCOPHAGUS_GHOST_E))
#define SARCOPHAGUS_GHOST_F_WIDTH 50
#define SARCOPHAGUS_GHOST_F_HEIGHT 65
#define SARCOPHAGUS_GHOST_G                                                    \
  (SARCOPHAGUS_GHOST_F + PICTURE_BYTES(SARCOPHAGUS_GHOST_F))
#define SARCOPHAGUS_GHOST_G_WIDTH 48
#define SARCOPHAGUS_GHOST_G_HEIGHT 29

/// Terra Strike's rock spire at its tallest.
#define ROCK_SPIRE_WIDTH 32
#define ROCK_SPIRE_HEIGHT 96
/// The flame Forcible Arm and Truncheon Fist drop.
#define FALLING_FLAME_WIDTH 32

/// ImpFaceSheet's 32x32 face, and the seven dimmer copies Wheeze makes after
/// it.
#define IMP_FACE_WIDTH 32
#define IMP_FACE_HEIGHT 32
#define IMP_FACE_FADES 8
/// The Stun Bolt's four bolts, LightningBoltSheet's pictures centred in 32x128
/// textures after the tornado's glints.
#define STUN_BOLT_TEXTURE_WIDTH 32
#define STUN_BOLT_TEXTURE_HEIGHT 128
#define STUN_BOLT_TEXTURES GLINT_SET_BYTES
#define STUN_BOLT_PICTURES 4
/// Hammersphere's ball: a 48x48 shaded circle mirrored into a 48x96 picture.
#define HAMMERSPHERE_BALL 0x800
#define HAMMERSPHERE_BALL_WIDTH 48
#define HAMMERSPHERE_BALL_HEIGHT 96
/// TwelveModeImage's four 18x48 pictures, each half of the bite: drawn once,
/// then mirrored beside it.
#define BITE_HALF_WIDTH 18
#define BITE_HALF_HEIGHT 48

/// Where SmokeSheet is decoded in the effect's pictures.
#define CASTING_IMPACT_SMOKE_PICTURES 0x1809
/// Where FlashBurstSheet is decoded in the effect's pictures.
#define CASTING_IMPACT_FLASH_BURST_PICTURES 0xd80
/// Where RuneSheet is decoded in the effect's pictures.
#define CASTING_IMPACT_RUNE_PICTURES 0x1000
/// Where BoulderPicture is decoded in the effect's pictures.
#define CASTING_IMPACT_BOULDER_PICTURES 0x2000

/// The impact particles that carry the rising pictures: flash bursts, skulls
/// or puffs of smoke.
#define CASTING_IMPACT_RISING_PICTURES 16

/// Gathers sparks around the caster, launches the Djinni and draws the
/// impact on the first target.
void BattleEffect_RunCastingImpact(struct BattleEffectParams *action,
                                   s32 kind) {
  u8 *buffer;
  struct Particle *sparks;
  struct BattleEffectWork *work;
  void *screen;
  s32 frame;
  struct BattleCamera *bg;
  void *tiles;
  s32 scrollA;
  s32 scrollB;
  s32 duration;
  s32 base[3];
  s32 target[3];
  s32 origin[3];
  s32 center[3];
  s32 pos[3];
  s32 point[3];
  s32 targetPoint[3];
  struct DrawFuncs drawFuncs;
  struct ModelTexture stunBoltTexture;
  struct ModelTexture reverseStarTexture;
  struct ModelTexture boscaHitTexture;
  struct ModelTexture stunJipTexture;
  struct ModelTexture armorCrushTexture;
  s32 step;
  s32 i;
  u8 *src;
  u8 *dst;
  s32 v;
  s32 *vec;

  buffer = Ram_MapCellBuffer;
  sparks = (struct Particle *)(Ram_MapCellBuffer + 0x6000);
  work = Ram_HeapSlots->battleEffect;
  screen = Ram_HeapSlots->battleCanvas;
  bg = Ram_HeapSlots->cameraWork;
  tiles = Ram_HeapSlots->battleSheet;
  scrollA = 0;
  scrollB = 0;

  if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
      kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
      kind == CASTING_IMPACT_IMPACT_ASURA ||
      kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS ||
      kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
      kind == CASTING_IMPACT_IMPACT_UNUSED_TILED_CANVAS ||
      kind == CASTING_IMPACT_IMPACT_APOCALYPSE)
    BattleEffect_StartTiledCanvas(BGCNT_WRAP);
  else
    BattleEffect_StartCanvasLayer(0);

  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, tiles, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, buffer, 1, 0);
  BattleEffect_PackTileRows(buffer, work->sheet + CASTING_IMPACT_ORB_PICTURES,
                            YELLOW_ORB_WIDTH,
                            YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT);

  if (kind == CASTING_IMPACT_IMPACT_SCORCH ||
      kind == CASTING_IMPACT_IMPACT_BARRAGE ||
      kind == CASTING_IMPACT_IMPACT_FLARE_BURST) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
             kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS) {
    Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_WHEEZE) {

    s32 j;
    s32 v;
    u8 *src;

    Resource_LoadAndDecompress((s32)&ResourceId_ImpFaceSheet, buffer, 1, 0);
    for (i = 1; i != IMP_FACE_FADES; i++) {
      s32 fade;
      j = 0;
      fade = i * 8;
      src = buffer;
      for (; j != PICTURE_BYTES(IMP_FACE); j++) {
        v = *src++;
        if (v == 0) {
          buffer[j + i * PICTURE_BYTES(IMP_FACE)] = 0;
          continue;
        }
        v -= fade;
        if (v <= 0)
          v = 1;
        buffer[j + i * PICTURE_BYTES(IMP_FACE)] = v;
      }
    }
  } else if (kind == CASTING_IMPACT_IMPACT_CHILL) {
    Resource_LoadAndDecompress((s32)&ResourceId_CloudPuffSheet, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_SQUALL ||
             kind == CASTING_IMPACT_IMPACT_STUN_VOLTAGE ||
             kind == CASTING_IMPACT_IMPACT_UNUSED_BOLTS ||
             kind == CASTING_IMPACT_IMPACT_BLITZ) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                               buffer + GLINT_SET_BYTES, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_STUN_CLOUD) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               buffer + CASTING_IMPACT_SMOKE_PICTURES, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
    Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, buffer, 1, 1);
  } else if (kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
    Resource_LoadAndDecompress((s32)&ResourceId_RockSpireSheet, buffer, 1, 1);
  } else if (kind == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
    Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_FROST_BITE) {
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_BROIL) {
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH) {
    Resource_LoadAndDecompress((s32)&ResourceId_BillowTexture, buffer, 0, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_STUN_JIP) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlameBandTexture,
                               buffer + 0x4000, 0, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_STUN_BOLT) {
    s32 j;
    s32 k;

    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, work->sheet,
                               1, 1);
    Iwram_ClearWords(buffer + STUN_BOLT_TEXTURES,
                     STUN_BOLT_PICTURES * PICTURE_BYTES(STUN_BOLT_TEXTURE));
    for (i = 0; i != STUN_BOLT_PICTURES; i++) {
      for (j = 0; j != LIGHTNING_BOLT_HEIGHT; j++) {
        for (k = 0; k != LIGHTNING_BOLT_WIDTH; k++)
          buffer[STUN_BOLT_TEXTURES +
                 (STUN_BOLT_TEXTURE_WIDTH - LIGHTNING_BOLT_WIDTH) / 2 +
                 i * PICTURE_BYTES(STUN_BOLT_TEXTURE) +
                 j * STUN_BOLT_TEXTURE_WIDTH + k] =
              work->sheet[i * LIGHTNING_BOLT_PICTURE_BYTES +
                          j * LIGHTNING_BOLT_WIDTH + k];
      }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1,
                               0);
  } else if (kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
             kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetA, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_SARGASSO ||
             kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
    Resource_LoadAndDecompress((s32)&ResourceId_SwirlTexture, buffer, 0, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
             kind == CASTING_IMPACT_IMPACT_DINOX ||
             kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage, buffer, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               buffer + CASTING_IMPACT_FLASH_BURST_PICTURES, 1,
                               0);
  } else if (kind == CASTING_IMPACT_IMPACT_FLASH_FORCE) {
    s32 k;
    s32 i;

    Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet,
                               buffer + CASTING_IMPACT_RUNE_PICTURES, 1, 0);
    Iwram_ClearWords(buffer, RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE);
    for (i = 0; i != RUNE_HALF_HEIGHT; i++) {
      for (k = 0; k != RUNE_HALF_WIDTH; k++) {
        buffer[RUNE_TEXTURE_RUNE + i * RUNE_TEXTURE_SIZE + k] =
            buffer[CASTING_IMPACT_RUNE_PICTURES + i * RUNE_HALF_WIDTH + k];
        buffer[RUNE_TEXTURE_RUNE + 2 * RUNE_HALF_WIDTH - 1 +
               i * RUNE_TEXTURE_SIZE - k] =
            buffer[CASTING_IMPACT_RUNE_PICTURES + i * RUNE_HALF_WIDTH + k];
      }
    }
  } else if (kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
    s32 i;
    s32 k;

    BattleEffect_DrawShadedCircles(buffer, HAMMERSPHERE_BALL_WIDTH, FIXED_ONE);
    for (i = 0; i != HAMMERSPHERE_BALL_HEIGHT / 2; i++) {
      for (k = 0; k != HAMMERSPHERE_BALL_WIDTH / 2; k++) {
        buffer[HAMMERSPHERE_BALL + i * HAMMERSPHERE_BALL_WIDTH + k] =
            buffer[i * (HAMMERSPHERE_BALL_WIDTH / 2) + k];
        buffer[HAMMERSPHERE_BALL + i * HAMMERSPHERE_BALL_WIDTH +
               HAMMERSPHERE_BALL_WIDTH - 1 - k] =
            buffer[i * (HAMMERSPHERE_BALL_WIDTH / 2) + k];
        buffer[HAMMERSPHERE_BALL +
               (HAMMERSPHERE_BALL_HEIGHT - 1 - i) * HAMMERSPHERE_BALL_WIDTH +
               k] = buffer[i * (HAMMERSPHERE_BALL_WIDTH / 2) + k];
        buffer[HAMMERSPHERE_BALL +
               (HAMMERSPHERE_BALL_HEIGHT - 1 - i) * HAMMERSPHERE_BALL_WIDTH +
               HAMMERSPHERE_BALL_WIDTH - 1 - k] =
            buffer[i * (HAMMERSPHERE_BALL_WIDTH / 2) + k];
      }
    }
  } else if (kind == CASTING_IMPACT_IMPACT_MOTHER_EARTH) {
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, buffer, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderPicture,
                               buffer + CASTING_IMPACT_BOULDER_PICTURES, 1, 1);
  } else if (kind == CASTING_IMPACT_IMPACT_VEIN_TAP) {
    REG_BLDALPHA = BLEND_ALPHA(16, 12);
  } else if (kind == CASTING_IMPACT_IMPACT_ICE_CRUSH) {
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
    Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, buffer, 1, 1);
  } else if (kind != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
             kind != CASTING_IMPACT_IMPACT_ASURA) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, buffer, 1, 0);
  }

  switch (kind) {
  case CASTING_IMPACT_IMPACT_BANE:
  case CASTING_IMPACT_IMPACT_FLINT:
  case CASTING_IMPACT_IMPACT_SAP:
  case CASTING_IMPACT_IMPACT_ECHO:
  case CASTING_IMPACT_IMPACT_STEEL:
  case CASTING_IMPACT_IMPACT_TERRA_STRIKE:
  case CASTING_IMPACT_IMPACT_POISON_CLOUD:
  case CASTING_IMPACT_IMPACT_POISON_DEATH:
  case CASTING_IMPACT_IMPACT_MORTAL_DANGER:
  case CASTING_IMPACT_IMPACT_BAD_OMEN:
  case CASTING_IMPACT_IMPACT_LIFE_NOURISH:
  case CASTING_IMPACT_IMPACT_FORCIBLE_ARM:
  case CASTING_IMPACT_IMPACT_EVIL_EYE:
  case CASTING_IMPACT_IMPACT_VEIN_TAP:
  case CASTING_IMPACT_IMPACT_HAMMERSPHERE:
  case CASTING_IMPACT_IMPACT_SARCOPHAGUS:
  case CASTING_IMPACT_IMPACT_MOTHER_EARTH:
  case CASTING_IMPACT_IMPACT_DINOX:
  case CASTING_IMPACT_IMPACT_TERRIBLE_BITE:
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_BlastSheet),
            (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                                 buffer + CASTING_IMPACT_DJINNI_PICTURE, 1, 0);
    break;
  case CASTING_IMPACT_IMPACT_MIST:
  case CASTING_IMPACT_IMPACT_SOUR:
  case CASTING_IMPACT_IMPACT_CHILL:
  case CASTING_IMPACT_IMPACT_GEL:
  case CASTING_IMPACT_IMPACT_FOG:
  case CASTING_IMPACT_IMPACT_AQUA_SOCK:
  case CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS:
  case CASTING_IMPACT_IMPACT_FROST_BITE:
  case CASTING_IMPACT_IMPACT_UNUSED_GATHERING:
  case CASTING_IMPACT_IMPACT_LIFE_LEECH:
  case CASTING_IMPACT_IMPACT_PSY_LEECH:
  case CASTING_IMPACT_IMPACT_STUN_CLOUD:
  case CASTING_IMPACT_IMPACT_SEARING_FOG:
  case CASTING_IMPACT_IMPACT_ICE_CRUSH:
  case CASTING_IMPACT_IMPACT_SARGASSO:
  case CASTING_IMPACT_IMPACT_FLASH_FORCE:
  case CASTING_IMPACT_IMPACT_REVERSE_STAR:
  case CASTING_IMPACT_IMPACT_CALL_ZOMBIE:
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                                 buffer + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet),
              (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
              REG_DMA3);
    break;
  case CASTING_IMPACT_IMPACT_FEVER:
  case CASTING_IMPACT_IMPACT_SCORCH:
  case CASTING_IMPACT_IMPACT_UNUSED_BOLTS:
  case CASTING_IMPACT_IMPACT_UNUSED_SKULLS:
  case CASTING_IMPACT_IMPACT_BROIL:
  case CASTING_IMPACT_IMPACT_MELTDOWN:
  case CASTING_IMPACT_IMPACT_HEAT_MIRAGE:
  case CASTING_IMPACT_IMPACT_BARRAGE:
  case CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME:
  case CASTING_IMPACT_IMPACT_ACID_BATH:
  case CASTING_IMPACT_IMPACT_TRUNCHEON_FIST:
  case CASTING_IMPACT_IMPACT_SEVERE_BLOW:
  case CASTING_IMPACT_IMPACT_POWER_DRIVE:
  case CASTING_IMPACT_IMPACT_HEAT_SMASH:
  case CASTING_IMPACT_IMPACT_SCORPIONFISH:
  case CASTING_IMPACT_IMPACT_FLARE_BURST:
  case CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE:
  case CASTING_IMPACT_IMPACT_ARMOR_CRUSH:
  case CASTING_IMPACT_IMPACT_STUN_JIP:
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                                 buffer + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
              (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
              REG_DMA3);
    break;
  case CASTING_IMPACT_IMPACT_SMOG:
  case CASTING_IMPACT_IMPACT_SQUALL:
  case CASTING_IMPACT_IMPACT_WHEEZE:
  case CASTING_IMPACT_IMPACT_BLITZ:
  case CASTING_IMPACT_IMPACT_UNUSED_COMETS:
  case CASTING_IMPACT_IMPACT_STUN_VOLTAGE:
  case CASTING_IMPACT_IMPACT_BLINDING_SMOG:
  case CASTING_IMPACT_IMPACT_MURK:
  case CASTING_IMPACT_IMPACT_CYCLONE_SLASH:
  case CASTING_IMPACT_IMPACT_PSYPHON_SEAL:
  case CASTING_IMPACT_IMPACT_ASURA:
  case CASTING_IMPACT_IMPACT_MOON_AIR:
  case CASTING_IMPACT_IMPACT_FLASH_EDGE:
  case CASTING_IMPACT_IMPACT_HIGH_VITALS:
  case CASTING_IMPACT_IMPACT_SUPERNOVA:
  case CASTING_IMPACT_IMPACT_STUN_BOLT:
  case CASTING_IMPACT_IMPACT_RAIDENS_WRATH:
  case CASTING_IMPACT_IMPACT_APOCALYPSE:
  case CASTING_IMPACT_IMPACT_FLASH_PUNCH:
  case CASTING_IMPACT_IMPACT_QUICK_SLASH:
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                                 buffer + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSmallSheet),
              (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
              REG_DMA3);
    break;
  case CASTING_IMPACT_IMPACT_BOSCA_HIT:
  case CASTING_IMPACT_IMPACT_ROC:
  case CASTING_IMPACT_IMPACT_WILD_WOLF:
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_DustPuffSheet),
            (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
    break;
  case CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI:
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               buffer + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
    break;
  }

  if (kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
      kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
      kind == CASTING_IMPACT_IMPACT_STUN_JIP ||
      kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
      kind == CASTING_IMPACT_IMPACT_BOSCA_HIT)
    Resource_LoadAndDecompress((s32)&ResourceId_FireRingTexture, buffer, 0, 0);
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (kind == CASTING_IMPACT_IMPACT_BAD_OMEN)
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  else
    work->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EffectPosition_ProjectScaledBattler(action->targets[0], target);
  EffectPosition_ProjectScaledBattler(action->user, base);
  BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
  {
    struct MotionObject *object;
    s32 savedVelocityX;
    s32 savedVelocityY;
    s32 save2c;
    s32 savedAcceleration;
    s32 savedVerticalStrength;
    struct MotionObject *targetObject;
    s32 n;
    s32 m;
    s32 t;
    s32 r;
    s32 x;
    s32 y;
    s32 h;
    s32 count;
    s32 start;
    s32 half;
    s32 height;
    s32 angle;
    s32 top;
    s32 scroll;
    s32 life;
    s32 row;
    s32 limit;
    s32 size;
    s32 fade;
    s32 scale;
    void *matrix;
    struct RenderModel *model;
    const u8 *table;
    s32 k;
    struct Particle *p;

    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
    object = BattleObject_GetFar(action->user)->object;
    for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
      work->particles[i].x = (Random16() & 63) + 32;
      work->particles[i].y = 0;
      work->particles[i].z = 0;
      work->particles[i].velocityX = Random16() & TRIG_ANGLE_MASK;
      work->particles[i].velocityY = Random16() & TRIG_ANGLE_MASK;
      work->particles[i].velocityZ = Random16() & TRIG_ANGLE_MASK;
    }
    Actor_SetAnimationSpeedFar(object, 0);
    /* FAKEMATCH(temporary): vec holds origin and then base while each is
     * filled. */
    vec = origin;
    vec[0] = object->x;
    vec[1] = object->y + toFIXED(80);
    vec[2] = object->z;
    savedVelocityX = object->velocityX;
    savedVelocityY = object->velocityY;
    save2c = object->velocityZ;
    savedAcceleration = object->acceleration;
    savedVerticalStrength = object->verticalMotionStrength;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->acceleration = 0;
    object->verticalMotionStrength = 0;
    vec = base;
    EffectPosition_ProjectScaledBattler(action->user, vec);
    vec[0] = vec[0] / 2;
    Sound_PlayFar(SE_POWER_UP);

    for (frame = 0; frame != CASTING_IMPACT_GATHER_FRAMES; frame++) {
      struct Particle *p;
      s32 n;
      count = 0;
      p = work->particles;
      for (i = 0; i != CASTING_IMPACT_SPARKS; i++, p++) {
        if (p->x >= 0) {
          if (frame >= i / 4) {
            n = 5;
            Matrix_Initialize();
            Matrix_RotateZ(p->velocityZ);
            Matrix_RotateX(p->velocityX);
            Matrix_RotateY(p->velocityY);
            EffectPosition_ProjectPoint(p, pos);
            pos[0] = pos[0] / 2 + base[0];
            if (kind <= CASTING_IMPACT_LAST_DJINNI)
              pos[1] = pos[1] + base[1] - 8;
            else if (kind == CASTING_IMPACT_IMPACT_SEVERE_BLOW)
              pos[1] = pos[1] + base[1] + 44;
            else
              pos[1] = pos[1] + base[1] + 12;
            if (pos[2] < -60)
              pos[2] = -60;
            if (pos[2] > 60)
              pos[2] = 60;
            pos[2] += 60;
            drawFuncs.draw[1](screen,
                              (u8 *)tiles + gBattleEffect_MotePictures[n - 1],
                              pos[0] - n / 2, pos[1] - n, n, n * 2);
            p->x -= 4;
          }
          count++;
        }
      }
      if (kind <= CASTING_IMPACT_LAST_DJINNI && count < CASTING_IMPACT_SPARKS) {
        Matrix_Initialize();
        Matrix_SetLookAt(bg->eye, bg->focus);
        EffectPosition_ProjectPoint(origin, pos);
        pos[0] = pos[0] / 2;
        drawFuncs.draw[0](screen, buffer + CASTING_IMPACT_DJINNI_PICTURE,
                          pos[0] - CASTING_IMPACT_DJINNI_WIDTH / 2, pos[1] - 4,
                          CASTING_IMPACT_DJINNI_WIDTH,
                          CASTING_IMPACT_DJINNI_HEIGHT);
        /* FAKEMATCH(barrier): an empty asm reading origin. */
        __asm__("" : : "X"(origin));
      }
      EFFECT_PRESENT_CANVAS(work);
    }

    if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == EFFECT_SIDE_ENEMIES)
        REG_BG2X = (frame - target[0]) << 8;
      else
        REG_BG2X = (96 - target[0]) << 8;
    } else if (kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == EFFECT_SIDE_ENEMIES)
        REG_BG2X = (64 - target[0]) << 8;
      else
        REG_BG2X = (64 - target[0]) << 8;
    } else if (kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == EFFECT_SIDE_ENEMIES) {
        scrollA = 0xffc00000;
        scrollB = 0x20000;
      } else {
        scrollA = 0xffc80000;
        scrollB = 0xfffe0000;
      }
      REG_BG2X = (FIXEDtoINT(scrollA)) << 8;
    } else if (kind == CASTING_IMPACT_IMPACT_ASURA ||
               kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == EFFECT_SIDE_ENEMIES) {
        scrollA = 0xff800000;
        scrollB = 0xc0000;
      } else {
        scrollA = 0;
        scrollB = 0xfff40000;
      }
      REG_BG2X = (FIXEDtoINT(scrollA)) << 8;
    }
    if (kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
      REG_BG2PA = Q_8_8(1);
      REG_BG2X = (64 - target[0]) << 8;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
      Iwram_ClearWords(BG_VRAM, CANVAS_VIEW_BYTES);
      Iwram_ClearWords(screen, CANVAS_VIEW_BYTES);
      REG_BLDCNT = 0;
    }
    if (kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == EFFECT_SIDE_ENEMIES)
        REG_BG2X = (32 - target[0]) << 8;
      else
        REG_BG2X = (96 - target[0]) << 8;
    }
    if (kind == CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_GATHERING ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_COMETS) {
      Iwram_ClearWords(BG_VRAM, CANVAS_VIEW_BYTES);
      Iwram_ClearWords(screen, CANVAS_VIEW_BYTES);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      EFFECT_RELEASE_BLITTERS();
      Actor_SelectAnimationFar(object, 3);
      if (kind == CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS)
        BattleEffect_RunProjectileVolley(action, PROJECTILE_VOLLEY_BLIZZARD);
      if (kind == CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME)
        BattleEffect_RunMysticFlame(action);
      if (kind == CASTING_IMPACT_IMPACT_UNUSED_COMETS)
        BattleEffect_RunProjectileVolley(action,
                                         PROJECTILE_VOLLEY_IMPACT_COMETS);
      return;
    }
    Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
    object->velocityX = savedVelocityX;
    object->velocityY = savedVelocityY;
    object->velocityZ = save2c;
    object->acceleration = savedAcceleration;
    object->verticalMotionStrength = savedVerticalStrength;
    if (kind == CASTING_IMPACT_IMPACT_SEVERE_BLOW) {
      Iwram_ClearWords(BG_VRAM, CANVAS_VIEW_BYTES);
      Iwram_ClearWords(screen, CANVAS_VIEW_BYTES);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      EFFECT_RELEASE_BLITTERS();
      action->variant = 3;
      BattleEffect_RunSparkGroups(action, SPARK_GROUPS_RISING);
      return;
    }
    Iwram_ClearWords(screen, CANVAS_VIEW_BYTES);
    Iwram_ClearWords(BG_VRAM, CANVAS_VIEW_BYTES);
    if (kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
      for (i = 1; i != 64; i++) {
        v = 31 - (FIXEDtoINT(Trig_Cos(i << 8) << 5));
        BG_PLTT[i] = RGB(v, v, v);
      }
    }
    targetObject = BattleObject_GetFar(action->targets[0])->object;
    {
      center[0] = (targetObject->x - origin[0]) / 6;
      center[1] = (targetObject->y - origin[1] + toFIXED(30)) / 6;
      center[2] = (targetObject->z - origin[2]) / 6;
    }
    for (i = 0; i != CASTING_IMPACT_SPARKS; i++)
      work->particles[i].variant = 0;
    if (kind != CASTING_IMPACT_IMPACT_AQUA_SOCK) {
      half = BattleUnit_GetEffectHeightFar(action->targets[0]) / 2;
      i = 0;
      p = work->particles;
      for (; i != 32; i++, p++) {
        SET_POSITION(p, targetObject->x, half, targetObject->z);
        if (kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
          p->velocityX = ((Random16() & 255) - 127) << 12;
          p->velocityY = ((Random16() & 255) - 64) << 10;
          p->velocityZ = ((Random16() & 255) - 127) << 12;
        } else {
          p->velocityX = ((Random16() & 255) - 127) << 12;
          p->velocityY = ((Random16() & 255) - 64) << 12;
          p->velocityZ = ((Random16() & 255) - 127) << 12;
        }
        p->velocityX = p->velocityX / 2;
        p->velocityY = p->velocityY / 2;
        p->velocityZ = p->velocityZ / 2;
        p->variant = i / 2 + 32;
      }
    }
    if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
      Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet,
                                 work->sheet, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA, buffer, 1,
                                 0);
      REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 14);
    }
    if (kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
      Resource_LoadAndDecompress((s32)&ResourceId_RaidensWrathSheet, buffer, 1,
                                 1);
      REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 8);
    }
    if (kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
      Resource_LoadAndDecompress((s32)&ResourceId_ApocalypseSheet, buffer, 1,
                                 1);
      REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 8);
    }
    if (kind == CASTING_IMPACT_IMPACT_ASURA) {
      Resource_LoadAndDecompress((s32)&ResourceId_BlueBeastSheet, work->sheet,
                                 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetB, buffer, 1,
                                 0);
      REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 14);
    }
    if (kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
      Resource_LoadAndDecompress((s32)&ResourceId_SarcophagusSheet, buffer, 1,
                                 1);
      REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 14);
    }

    if (kind == CASTING_IMPACT_IMPACT_SAP ||
        kind == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
        kind == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
        kind == CASTING_IMPACT_IMPACT_VEIN_TAP ||
        kind == CASTING_IMPACT_IMPACT_HIGH_VITALS ||
        kind == CASTING_IMPACT_IMPACT_FLARE_BURST ||
        kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
        kind == CASTING_IMPACT_IMPACT_ASURA ||
        kind == CASTING_IMPACT_IMPACT_PSY_LEECH ||
        kind == CASTING_IMPACT_IMPACT_STEEL ||
        kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
        kind == CASTING_IMPACT_IMPACT_STUN_BOLT ||
        kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS ||
        kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
        kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
    } else if (kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
      for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++) {
        struct Particle *q;
        q = &SPARKS[i];
        SET_POSITION(q, targetObject->x, toFIXED(20), targetObject->z);
        if (targetObject->x < 0)
          q->velocityX = -((Random16() & 255) + 128) << 11;
        else
          q->velocityX = ((Random16() & 255) + 128) << 11;
        q->velocityY = ((Random16() & 255) - 127) << 10;
        q->velocityZ = 0;
        q->variant = 0;
      }
    } else {
      struct Particle *p;
      if (kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
          kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS)
        height = 0;
      else
        height = toFIXED(20);
      p = SPARKS;
      for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++, p++) {
        SET_POSITION(p, targetObject->x, height, targetObject->z);
        if (kind == CASTING_IMPACT_IMPACT_SCORCH ||
            kind == CASTING_IMPACT_IMPACT_BARRAGE) {
          p->velocityX = ((Random16() & 255) - 127) << 11;
          p->velocityY = (Random16() & 255) << 11;
          p->velocityZ = ((Random16() & 255) - 127) << 11;
        } else if (kind == CASTING_IMPACT_IMPACT_STUN_CLOUD ||
                   kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
                   kind == CASTING_IMPACT_IMPACT_SCORPIONFISH ||
                   kind == CASTING_IMPACT_IMPACT_MOON_AIR ||
                   kind == CASTING_IMPACT_IMPACT_WHEEZE ||
                   kind == CASTING_IMPACT_IMPACT_DINOX ||
                   kind == CASTING_IMPACT_IMPACT_CHILL) {
          p->velocityX = ((Random16() & 255) - 127) << 13;
          p->velocityY = ((Random16() & 255) - 127) << 12;
          p->velocityZ = ((Random16() & 255) - 127) << 13;
        } else if (kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
          p->velocityX = ((Random16() & 255) - 127) << 14;
          p->velocityY = ((Random16() & 255) - 127) << 13;
          p->velocityZ = ((Random16() & 255) - 127) << 14;
        } else if (kind == CASTING_IMPACT_IMPACT_ACID_BATH) {
          p->velocityX = ((Random16() & 255) - 127) << 11;
          p->velocityY = (Random16() & 127) << 10;
          p->velocityZ = ((Random16() & 255) - 127) << 11;
        } else {
          p->velocityX = ((Random16() & 255) - 127) << 10;
          p->velocityY = (Random16() & 127) << 10;
          p->velocityZ = ((Random16() & 255) - 127) << 10;
        }
        p->velocityX = p->velocityX / 4;
        p->velocityY = p->velocityY / 4;
        p->velocityZ = p->velocityZ / 4;
        p->variant = 0;
      }
    }

    if (kind == CASTING_IMPACT_IMPACT_MIST ||
        kind == CASTING_IMPACT_IMPACT_FEVER ||
        kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS ||
        kind == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
        kind == CASTING_IMPACT_IMPACT_EVIL_EYE ||
        kind == CASTING_IMPACT_IMPACT_MURK ||
        kind == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
        kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
        kind == CASTING_IMPACT_IMPACT_FLASH_PUNCH)
      Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);

    if (kind == CASTING_IMPACT_IMPACT_FLINT ||
        kind == CASTING_IMPACT_IMPACT_SCORCH ||
        kind == CASTING_IMPACT_IMPACT_SQUALL ||
        kind == CASTING_IMPACT_IMPACT_BLITZ ||
        kind == CASTING_IMPACT_IMPACT_BARRAGE ||
        kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
        kind == CASTING_IMPACT_IMPACT_STUN_VOLTAGE ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_BOLTS ||
        kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
        kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI ||
        kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
        kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH ||
        kind == CASTING_IMPACT_IMPACT_STUN_JIP ||
        kind == CASTING_IMPACT_IMPACT_FLASH_EDGE ||
        kind == CASTING_IMPACT_IMPACT_POWER_DRIVE ||
        kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
        kind == CASTING_IMPACT_IMPACT_BOSCA_HIT ||
        kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
        kind == CASTING_IMPACT_IMPACT_DINOX)
      duration = 32;
    else if (kind == CASTING_IMPACT_IMPACT_BANE ||
             kind == CASTING_IMPACT_IMPACT_SMOG ||
             kind == CASTING_IMPACT_IMPACT_MIST ||
             kind == CASTING_IMPACT_IMPACT_FEVER ||
             kind == CASTING_IMPACT_IMPACT_FOG ||
             kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
             kind == CASTING_IMPACT_IMPACT_POISON_CLOUD ||
             kind == CASTING_IMPACT_IMPACT_SCORPIONFISH ||
             kind == CASTING_IMPACT_IMPACT_MOON_AIR ||
             kind == CASTING_IMPACT_IMPACT_POISON_DEATH ||
             kind == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
             kind == CASTING_IMPACT_IMPACT_EVIL_EYE ||
             kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
             kind == CASTING_IMPACT_IMPACT_ACID_BATH ||
             kind == CASTING_IMPACT_IMPACT_MURK ||
             kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL ||
             kind == CASTING_IMPACT_IMPACT_AQUA_SOCK ||
             kind == CASTING_IMPACT_IMPACT_ICE_CRUSH ||
             kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
             kind == CASTING_IMPACT_IMPACT_APOCALYPSE ||
             kind == CASTING_IMPACT_IMPACT_FLASH_PUNCH ||
             kind == CASTING_IMPACT_IMPACT_WHEEZE ||
             kind == CASTING_IMPACT_IMPACT_CHILL)
      duration = RUNE_HALF_HEIGHT;
    else if (kind == CASTING_IMPACT_IMPACT_MELTDOWN ||
             kind == CASTING_IMPACT_IMPACT_HEAT_SMASH ||
             kind == CASTING_IMPACT_IMPACT_SOUR ||
             kind == CASTING_IMPACT_IMPACT_GEL ||
             kind == CASTING_IMPACT_IMPACT_ROC ||
             kind == CASTING_IMPACT_IMPACT_CALL_ZOMBIE)
      duration = 20;
    else if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
             kind == CASTING_IMPACT_IMPACT_ASURA ||
             kind == CASTING_IMPACT_IMPACT_BROIL ||
             kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
             kind == CASTING_IMPACT_IMPACT_STUN_BOLT ||
             kind == CASTING_IMPACT_IMPACT_STUN_JIP ||
             kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
             kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE)
      duration = 40;
    else if (kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS)
      duration = 72;
    else if (kind == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
             kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
             kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS ||
             kind == CASTING_IMPACT_IMPACT_STUN_CLOUD)
      duration = 64;
    else if (kind == CASTING_IMPACT_IMPACT_ECHO ||
             kind == CASTING_IMPACT_IMPACT_SARGASSO ||
             kind == CASTING_IMPACT_IMPACT_FLASH_FORCE ||
             kind == CASTING_IMPACT_IMPACT_MOTHER_EARTH)
      duration = 54;
    else
      duration = 80;

    for (frame = 0; frame != duration; frame++) {
      struct Particle *p;
      struct Particle *particle;
      if (kind != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
          kind != CASTING_IMPACT_IMPACT_ASURA &&
          kind != CASTING_IMPACT_IMPACT_SUPERNOVA &&
          kind != CASTING_IMPACT_IMPACT_SARCOPHAGUS &&
          kind != CASTING_IMPACT_IMPACT_RAIDENS_WRATH &&
          kind != CASTING_IMPACT_IMPACT_APOCALYPSE) {
        s32 *w;
        w = work->bg2xLines;
        for (i = 0; i != DISPLAY_HEIGHT; i++)
          *w++ = (0x40000 - (Trig_Sin((frame << 12) + (i << 11)) << 2)) >> 10;
      }
      switch (kind) {
      case CASTING_IMPACT_IMPACT_CHILL:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CloudPuffSheet);
        break;
      case CASTING_IMPACT_IMPACT_POWER_DRIVE:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_FireballSheet);
        break;
      case CASTING_IMPACT_IMPACT_SEARING_FOG:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_GreyPaletteA);
        break;
      case CASTING_IMPACT_IMPACT_FORCIBLE_ARM:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_FlameSheetA);
        break;
      case CASTING_IMPACT_IMPACT_AQUA_SOCK:
      case CASTING_IMPACT_IMPACT_STUN_CLOUD:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlueFlameSheet);
        break;
      case CASTING_IMPACT_IMPACT_PSYPHON_SEAL:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_RuneSheet);
        break;
      case CASTING_IMPACT_IMPACT_TERRA_STRIKE:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_RockSpireSheet);
        break;
      case CASTING_IMPACT_IMPACT_BANE:
      case CASTING_IMPACT_IMPACT_POISON_DEATH:
      case CASTING_IMPACT_IMPACT_SCORPIONFISH:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
        break;
      case CASTING_IMPACT_IMPACT_BAD_OMEN:
      case CASTING_IMPACT_IMPACT_LIFE_NOURISH:
      case CASTING_IMPACT_IMPACT_ACID_BATH:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_PinkBurstSheet);
        break;
      case CASTING_IMPACT_IMPACT_LIFE_LEECH:
      case CASTING_IMPACT_IMPACT_HIGH_VITALS:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_IceBlockSheet);
        break;
      case CASTING_IMPACT_IMPACT_PSY_LEECH:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
        break;
      case CASTING_IMPACT_IMPACT_MIST:
      case CASTING_IMPACT_IMPACT_MURK:
      case CASTING_IMPACT_IMPACT_MOON_AIR:
      case CASTING_IMPACT_IMPACT_REVERSE_STAR:
      case CASTING_IMPACT_IMPACT_STUN_JIP:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_TanPalette);
        break;
      case CASTING_IMPACT_IMPACT_SMOG:
      case CASTING_IMPACT_IMPACT_FOG:
      case CASTING_IMPACT_IMPACT_BLINDING_SMOG:
      case CASTING_IMPACT_IMPACT_FLASH_PUNCH:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);
        break;
      case CASTING_IMPACT_IMPACT_FEVER:
      case CASTING_IMPACT_IMPACT_BROIL:
      case CASTING_IMPACT_IMPACT_HEAT_MIRAGE:
      case CASTING_IMPACT_IMPACT_FLARE_BURST:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_EmberStreakSheet);
        break;
      case CASTING_IMPACT_IMPACT_POISON_CLOUD:
      case CASTING_IMPACT_IMPACT_SUPERNOVA:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_LimePalette);
        break;
      case CASTING_IMPACT_IMPACT_SCORCH:
      case CASTING_IMPACT_IMPACT_BARRAGE:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_FlashBurstSheet);
        break;
      case CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE:
      case CASTING_IMPACT_IMPACT_ARMOR_CRUSH:
      case CASTING_IMPACT_IMPACT_TERRIBLE_BITE:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_OrangePaletteB);
        break;
      case CASTING_IMPACT_IMPACT_VEIN_TAP:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_RedPaletteB);
        break;
      case CASTING_IMPACT_IMPACT_STUN_BOLT:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_VioletPaletteG);
        break;
      }
      if (frame < 6) {
        EffectPosition_ProjectScaledBattler(action->user, base);
        base[0] = base[0] / 2;
        base[1] += 8;
      }
      if (kind != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
          kind != CASTING_IMPACT_IMPACT_TERRA_STRIKE &&
          kind != CASTING_IMPACT_IMPACT_ASURA &&
          kind != CASTING_IMPACT_IMPACT_FORCIBLE_ARM &&
          kind != CASTING_IMPACT_IMPACT_TRUNCHEON_FIST &&
          kind != CASTING_IMPACT_IMPACT_HAMMERSPHERE &&
          kind != CASTING_IMPACT_IMPACT_ICE_CRUSH &&
          kind != CASTING_IMPACT_IMPACT_REVERSE_STAR &&
          kind != CASTING_IMPACT_IMPACT_SARCOPHAGUS &&
          kind != CASTING_IMPACT_IMPACT_RAIDENS_WRATH &&
          kind != CASTING_IMPACT_IMPACT_APOCALYPSE &&
          kind != CASTING_IMPACT_IMPACT_FLASH_PUNCH &&
          kind != CASTING_IMPACT_IMPACT_MOTHER_EARTH &&
          kind != CASTING_IMPACT_IMPACT_WILD_WOLF && frame < 12) {
        if (action->side == EFFECT_SIDE_ENEMIES)
          drawFuncs.draw[0](
              screen, work->sheet + frame / 2 * PICTURE_BYTES(FIRE_STREAK),
              base[0] - 28, base[1] - 40, FIRE_STREAK_WIDTH,
              FIRE_STREAK_HEIGHT);
        else
          drawFuncs.draw[0](
              screen, work->sheet + frame / 2 * PICTURE_BYTES(FIRE_STREAK),
              base[0] - 20, base[1] - 40, FIRE_STREAK_WIDTH,
              FIRE_STREAK_HEIGHT);
      }
      if (kind != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
          kind != CASTING_IMPACT_IMPACT_TERRA_STRIKE &&
          kind != CASTING_IMPACT_IMPACT_ASURA &&
          kind != CASTING_IMPACT_IMPACT_SUPERNOVA &&
          kind != CASTING_IMPACT_IMPACT_SARCOPHAGUS &&
          kind != CASTING_IMPACT_IMPACT_RAIDENS_WRATH &&
          kind != CASTING_IMPACT_IMPACT_APOCALYPSE &&
          kind != CASTING_IMPACT_IMPACT_MOTHER_EARTH) {
        if (frame >= 4 && frame < 16)
          drawFuncs.draw[1](
              screen,
              work->sheet + CASTING_IMPACT_ORB_PICTURES +
                  ((frame - 4) / 2) * PICTURE_BYTES(YELLOW_ORB_PACKED),
              target[0] / 2 - 8, base[1] - 24, 20, YELLOW_ORB_PACKED_HEIGHT);
        Matrix_Initialize();
        Matrix_SetLookAt(bg->eye, bg->focus);
        if (frame > 3) {
          if (kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
            struct Particle *r;
            for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
              r = &work->particles[i];
              if (r->variant > 0) {
                s32 n;
                n = (r->variant >> 2) + 1;
                if (n > 10)
                  n = 10;
                EffectPosition_ProjectPoint(r, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](
                    screen, (u8 *)tiles + gBattleEffect_MotePictures[n - 1],
                    point[0] - n / 2, point[1] - n, n, n * 2);
                // DEAD: this arm runs only for kind 79, so the kind 89 test
                // never holds; the ROM keeps the compare and both arms.
                {
                  s32 gravity;
                  if (kind == CASTING_IMPACT_IMPACT_FLASH_PUNCH)
                    gravity = 0;
                  else
                    gravity = -toFIXED(1) / 32;
                  BattleEffectKernels_IntegrateVector3(r, 62, gravity);
                }
                r->variant -= 2;
              }
            }
          } else {
            for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
              p = &work->particles[i / 2];
              if (p->variant > 0) {
                s32 n;
                n = p->variant / 20 + 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[(i / 2) & 1](
                    screen, (u8 *)tiles + gBattleEffect_MotePictures[n - 1],
                    point[0] - n / 2, point[1] - n, n, n * 2);
                BattleEffectKernels_IntegrateVector3(p, 62, -toFIXED(1) / 32);
                p->variant--;
              }
            }
          }
        }
      }

      if (kind == CASTING_IMPACT_IMPACT_SAP ||
          kind == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
          kind == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
          kind == CASTING_IMPACT_IMPACT_VEIN_TAP ||
          kind == CASTING_IMPACT_IMPACT_HIGH_VITALS ||
          kind == CASTING_IMPACT_IMPACT_FLARE_BURST ||
          kind == CASTING_IMPACT_IMPACT_PSY_LEECH ||
          kind == CASTING_IMPACT_IMPACT_STEEL ||
          kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
        s32 start;
        struct MotionObject *casterObject;
        s32 half;
        s32 dx;
        s32 dy;
        s32 dz;
        s32 slot;

        start = 12;
        if (frame == 12)
          Sound_PlayFar(SE_SWIRL);
        if (kind == CASTING_IMPACT_IMPACT_FLARE_BURST)
          start = 0;
        if (frame == 50)
          BattleEffect_TintBattler(action->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                                   TINT_KEEP, 0);
        if (frame == 79)
          BattleEffect_TintBattler(action->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                                   TINT_KEEP, 0);
        if (frame == start) {
          struct Particle *p;
          p = SPARKS;
          for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++, p++) {
            SET_POSITION(p, targetObject->x, toFIXED(20), targetObject->z);
            p->velocityX = ((Random16() & 255) - 128) << 10;
            p->velocityY = ((Random16() & 255) - 128) << 10;
            p->velocityZ = ((Random16() & 255) - 128) << 10;
            if (kind == CASTING_IMPACT_IMPACT_FLARE_BURST) {
              p->velocityX <<= 1;
              p->velocityY <<= 1;
              p->velocityZ <<= 1;
            }
            p->variant = 0;
          }
        }
        if (frame >= start) {
          p = SPARKS;
          casterObject = BattleObject_GetFar(action->user)->object;
          half = BattleUnit_GetEffectHeightFar(action->user) / 2;
          slot = kind == CASTING_IMPACT_IMPACT_FLARE_BURST;
          for (i = 0; i != 32; i++, p++) {
            if (p->variant >= 0) {
              n = (i & 1) + 6;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] >> 1;
              drawFuncs.draw[slot](
                  screen, (u8 *)tiles + gBattleEffect_MotePictures[n - 1],
                  point[0] - n / 2, point[1] - n, n, n * 2);
              BattleEffectKernels_IntegrateVector3(p, 62, 0);
              if (frame > i + 22) {
                dx = (casterObject->x - p->x) >> 8;
                dy = (casterObject->y + half - p->y) >> 8;
                dz = (casterObject->z - p->z) >> 8;
                p->velocityX += dx;
                p->velocityY += dy;
                p->velocityZ += dz;
                if ((dx > -0x1000 && dx < 0x1000) &&
                    (dz > -0x1000 && dz < 0x1000))
                  p->variant = -1;
              }
            }
          }
        }
      }

      if (kind == CASTING_IMPACT_IMPACT_SAP ||
          kind == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
          kind == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
          kind == CASTING_IMPACT_IMPACT_VEIN_TAP ||
          kind == CASTING_IMPACT_IMPACT_HIGH_VITALS ||
          kind == CASTING_IMPACT_IMPACT_PSY_LEECH ||
          kind == CASTING_IMPACT_IMPACT_STEEL) {
      } else if (kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
        struct Particle *p;
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i + 4 && p->variant <= 23) {
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[0](screen, buffer, point[0] - 20, point[1] - 32,
                              FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
            BattleEffectKernels_IntegrateVector3(p, PARTICLE_DAMPING_NONE, 0);
            p->variant++;
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_MELTDOWN ||
                 kind == CASTING_IMPACT_IMPACT_HEAT_SMASH ||
                 kind == CASTING_IMPACT_IMPACT_SOUR ||
                 kind == CASTING_IMPACT_IMPACT_ROC ||
                 kind == CASTING_IMPACT_IMPACT_CALL_ZOMBIE) {
      } else if (kind == CASTING_IMPACT_IMPACT_SQUALL ||
                 kind == CASTING_IMPACT_IMPACT_STUN_VOLTAGE ||
                 kind == CASTING_IMPACT_IMPACT_UNUSED_BOLTS ||
                 kind == CASTING_IMPACT_IMPACT_BLITZ) {
        s32 y;
        if (kind == CASTING_IMPACT_IMPACT_BLITZ) {
          if (frame == 4)
            work->shakeFrames = 12;
          if (frame == 8)
            Sound_PlayFar(SE_RISE_WITH_SPARKS);
        }
        if (kind == CASTING_IMPACT_IMPACT_SQUALL ||
            kind == CASTING_IMPACT_IMPACT_STUN_VOLTAGE) {
          if (frame >= 6 && frame < 20) {
            for (i = 0; i != 2; i++)
              drawFuncs.draw[0](screen,
                                buffer + GLINT_SET_BYTES +
                                    (((frame + i * 3) / 2) & 3) *
                                        LIGHTNING_BOLT_PICTURE_BYTES,
                                target[0] / 2 - 8, 0, LIGHTNING_BOLT_WIDTH,
                                104);
          }
        } else if (kind == CASTING_IMPACT_IMPACT_BLITZ &&
                   (frame >= 6 && frame < 20)) {
          s32 x;
          for (i = 0; i != 4; i++) {
            s32 m;
            angle = frame * 1848 + i * (DEGtoANG(90));
            m = ((frame + i * 3) / 2) & 3;
            x = target[0] / 2 + (FIXEDtoINT(Trig_Sin(angle) << 5));
            // DEAD: the cosine is computed and dropped; the ROM keeps the call.
            Trig_Cos(angle);
            drawFuncs.draw[0](screen,
                              buffer + GLINT_SET_BYTES +
                                  m * LIGHTNING_BOLT_PICTURE_BYTES,
                              x - 8, 0, LIGHTNING_BOLT_WIDTH, 104);
          }
        }
        if (frame >= 8 && frame < 24) {
          s32 angle;
          s32 off;
          s32 amp;

          if (kind == CASTING_IMPACT_IMPACT_UNUSED_BOLTS) {
            amp = 12;
            off = 24;
          } else {
            off = 0;
            amp = 32;
          }
          for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
            u32 flip;
            m = i & 3;
            angle = Random16() & TRIG_ANGLE_MASK;
            x = (FIXEDtoINT(Trig_Sin(angle) << 3)) + target[0] / 2 -
                gBattleEffect_GlintPictureWidths[m] / 2;
            y = (FIXEDtoINT(Trig_Cos(angle) * amp)) + off -
                gBattleEffect_GlintPictureHeights[m] / 2;
            EFFECT_RELEASE_BLITTERS();
            flip = Random16();
            EFFECT_LOAD_BLITTER(
                188, gCastingImpact_SquallGlintFlips[flip & 3] | 3, 0);
            ALT_BLITTER(screen, buffer + gBattleEffect_GlintPictures[m], x,
                        y + 56, gBattleEffect_GlintPictureWidths[m],
                        gBattleEffect_GlintPictureHeights[m]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_STUN_CLOUD) {
        s32 angle;
        if (frame >= 8 && frame < 24) {
          s32 y;
          for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
            u32 flip;
            m = i & 3;
            angle = Random16() & TRIG_ANGLE_MASK;
            x = (FIXEDtoINT(Trig_Sin(angle) << 4)) + target[0] / 2 -
                gBattleEffect_GlintPictureWidths[m] / 2;
            y = (FIXEDtoINT(Trig_Cos(angle) << 4)) -
                gBattleEffect_GlintPictureHeights[m] / 2;
            EFFECT_RELEASE_BLITTERS();
            flip = Random16();
            EFFECT_LOAD_BLITTER(188, gCastingImpact_SapGlintFlips[flip & 3] | 3,
                                1);
            ALT_BLITTER(screen, buffer + gBattleEffect_GlintPictures[m], x,
                        y + 80, gBattleEffect_GlintPictureWidths[m],
                        gBattleEffect_GlintPictureHeights[m]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
          }
        }
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i + 4) {
            s32 life;
            life = p->variant;
            if (life <= 23) {
              s32 m;
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[1](screen,
                                buffer + CASTING_IMPACT_SMOKE_PICTURES +
                                    m * PICTURE_BYTES(SMOKE_PUFF),
                                point[0] - SMOKE_PUFF_WIDTH / 2,
                                point[1] - SMOKE_PUFF_HEIGHT / 2,
                                SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
              BattleEffectKernels_IntegrateVector3(p, 60, toFIXED(1) / 16);
              p->variant++;
            }
          }
        }
        if (frame == 6)
          Sound_PlayFar(SE_RISE_WITH_SPARKS);
      } else if (kind == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
        s32 count;
        EFFECT_RELEASE_BLITTERS();
        if (frame >= 0 && frame < 24) {
          s32 x;
          x = target[0] / 2;
          top = frame * 32 - 232;
          scroll = frame * 16 - 48;
          if (top > 0)
            top = 0;
          while (scroll > BLUE_FLAME_COLUMN_HEIGHT)
            scroll -= BLUE_FLAME_COLUMN_HEIGHT;
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                              BLITTER_MAX);
          ALT_BLITTER(screen, buffer, x - 8,
                      top + scroll - BLUE_FLAME_COLUMN_HEIGHT,
                      BLUE_FLAME_COLUMN_WIDTH, BLUE_FLAME_COLUMN_HEIGHT);
          ALT_BLITTER(screen, buffer, x - 8, top + scroll,
                      BLUE_FLAME_COLUMN_WIDTH,
                      BLUE_FLAME_COLUMN_HEIGHT - scroll);
          ALT_BLITTER(screen, buffer + PICTURE_BYTES(BLUE_FLAME_COLUMN), x - 17,
                      top + 47, BLUE_FLAME_SPLASH_WIDTH,
                      BLUE_FLAME_SPLASH_HEIGHT);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
          if (frame == 8)
            work->shakeFrames = 8;
          if (frame > 1) {
            struct Particle *p;
            i = 0;
            p = work->particles;
            count = 0;
            for (; i != CASTING_IMPACT_SPARKS; i++, p++) {
              if (p->variant == 0) {
                count++;
                SET_POSITION(p, targetObject->x, toFIXED(20), targetObject->z);
                p->velocityX = ((Random16() & 255) - 127) << 12;
                p->velocityY = ((Random16() & 255) - 64) << 10;
                p->velocityZ = ((Random16() & 255) - 127) << 12;
                p->variant = i / 2 + 32;
                if (count == 4)
                  break;
              }
            }
          }
        }
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
        s32 x;
        EFFECT_RELEASE_BLITTERS();
        if (frame >= 4 && frame < 24) {
          s32 h;
          x = target[0] / 2;
          h = RUNE_HALF_HEIGHT;
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                              BLITTER_MAX);
          ALT_BLITTER(screen, buffer, x - RUNE_HALF_WIDTH, h, RUNE_HALF_WIDTH,
                      h);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              BLITTER_MAX);
          ALT_BLITTER(screen, buffer, x, h, RUNE_HALF_WIDTH, h);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
        // A tornado stands over the target.
        s32 m;
        if (frame > 15)
          REG_BLDALPHA = BLEND_ALPHA(32 - frame, BLEND_WEIGHT_MAX);
        if (frame > 5) {
          s32 x;
          s32 o1;
          x = target[0] / 2;
          m = frame / 2 % 3;
          x -= TORNADO_STRIP_WIDTH / 2;
          o1 = m * (2 * PICTURE_BYTES(TORNADO_STRIP));
          drawFuncs.draw[0](screen, buffer + GLINT_SET_BYTES + o1, x, 16,
                            TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
          drawFuncs.draw[0](screen,
                            buffer + GLINT_SET_BYTES +
                                6 * PICTURE_BYTES(TORNADO_STRIP) +
                                m * PICTURE_BYTES(TORNADO_STRIP),
                            x, 48, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
          {
            u8 *o0;
            o0 = buffer + GLINT_SET_BYTES + PICTURE_BYTES(TORNADO_STRIP) + o1;
            drawFuncs.draw[0](screen, o0, x, 80, TORNADO_STRIP_WIDTH,
                              TORNADO_STRIP_HEIGHT);
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_SCORCH ||
                 kind == CASTING_IMPACT_IMPACT_BARRAGE ||
                 kind == CASTING_IMPACT_IMPACT_FLARE_BURST) {
        if (frame < 24) {
          p = SPARKS;
          for (i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++, p++) {
            if (frame >= i / 2 + 4) {
              s32 life;
              life = p->variant;
              if (life <= 11) {
                s32 half;
                half = life / 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](screen,
                                  buffer + half * PICTURE_BYTES(FLASH_BURST),
                                  point[0] - FLASH_BURST_WIDTH / 2,
                                  point[1] - FLASH_BURST_HEIGHT / 2,
                                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
                BattleEffectKernels_IntegrateVector3(p, 60, toFIXED(1) / 16);
                p->variant++;
              }
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_FLINT) {
      } else if (kind == CASTING_IMPACT_IMPACT_ECHO) {
      } else if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
        y = (FIXEDtoINT(target[1])) + (FIXEDtoINT(Trig_Cos(frame << 9) << 2)) +
            16;
        if (frame < 4) {
          drawFuncs.draw[0](
              screen, work->sheet, gCastingImpact_PictureX[action->side * 7],
              y + gCastingImpact_PictureY[0], REAPER_SHEET_RAISED_WIDTH,
              REAPER_SHEET_RAISED_HEIGHT);
        } else {
          if (frame < 8)
            drawFuncs.draw[0](
                screen, work->sheet, gCastingImpact_PictureX[action->side * 7],
                y + gCastingImpact_PictureY[0], REAPER_SHEET_RAISED_WIDTH,
                REAPER_SHEET_RAISED_HEIGHT);
          EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, work->sheet,
                              REAPER_SHEET_SWUNG,
                              gCastingImpact_PictureX[action->side * 7 + 1],
                              y + gCastingImpact_PictureY[1]);
          if (frame >= 4 && frame < 6)
            Iwram_FillWords(screen, CANVAS_BYTES,
                            CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          if (frame >= 6 && frame < 8)
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, work->sheet,
                                REAPER_SHEET_CRESCENT,
                                gCastingImpact_PictureX[action->side * 7 + 2],
                                y + gCastingImpact_PictureY[2]);
          if (frame >= 8 && frame < 10)
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                RED_CRESCENT_FADE_A,
                                gCastingImpact_PictureX[action->side * 7 + 3],
                                y + gCastingImpact_PictureY[3]);
          if (frame >= 10 && frame < 12)
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                RED_CRESCENT_FADE_B,
                                gCastingImpact_PictureX[action->side * 7 + 4],
                                y + gCastingImpact_PictureY[4]);
          if (frame >= 12 && frame < 14)
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                RED_CRESCENT_FADE_C,
                                gCastingImpact_PictureX[action->side * 7 + 5],
                                y + gCastingImpact_PictureY[5]);
          if (frame >= 14 && frame < 16)
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                RED_CRESCENT_FADE_D,
                                gCastingImpact_PictureX[action->side * 7 + 6],
                                y + gCastingImpact_PictureY[6]);
        }
      } else if (kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
        s32 h;
        s32 y;
        REG_BLDCNT = 0;
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        EFFECT_RELEASE_BLITTERS();
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                            BLITTER_TRANSPARENT);
        drawFuncs.draw[0] = BLITTER;
        if (frame == 0) {
          for (i = 0; i != 16 * SARCOPHAGUS_COFFIN_WIDTH; i++)
            work->sheet[i] = Random16() & 31;
        }
        y = frame * 28 - 96;
        if (y > 16)
          y = 16;
        {
          /* FAKEMATCH(pin): hold, an input of the empty asm below. */
          register s32 hold __asm__("r2");
          h = SARCOPHAGUS_COFFIN_HEIGHT;
          /* FAKEMATCH(barrier): an empty asm through h, with hold as an input.
           */
          __asm__("" : "=r"(h) : "0"(h), "r"(hold));
        }
        if (frame > 7) {
          s32 t;
          i = 0;
          t = 0;
          row = 112 - frame * 2;
          for (; i != 34; i++, t++, row++) {
            if (row >= 0 && row < 96) {
              s32 k;
              s32 threshold;
              for (k = 0, threshold = t; k != SARCOPHAGUS_COFFIN_WIDTH; k++) {
                /* FAKEMATCH(barrier): an empty asm through threshold. */
                __asm__("" : "=r"(threshold) : "0"(threshold));
                if (work->sheet[(row & 15) * SARCOPHAGUS_COFFIN_WIDTH + k] <
                    threshold)
                  buffer[row * SARCOPHAGUS_COFFIN_WIDTH + k] = 0;
              }
            }
          }
        }
        drawFuncs.draw[0](screen, buffer, 29, y, SARCOPHAGUS_COFFIN_WIDTH, h);
        if (frame > 5) {
          switch ((frame - 6) / 4) {
          case 0:
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                SARCOPHAGUS_GHOST_A, 32, 92);
            break;
          case 1:
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                SARCOPHAGUS_GHOST_B, 30, 87);
            break;
          case 2:
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                SARCOPHAGUS_GHOST_C, 23, 57);
            break;
          case 3:
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                SARCOPHAGUS_GHOST_D, 27, 55);
            break;
          case 4:
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                SARCOPHAGUS_GHOST_E, 28, 19);
            break;
          case 5:
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                SARCOPHAGUS_GHOST_F, 33, 22);
            break;
          case 6:
            EFFECT_DRAW_PICTURE(drawFuncs.draw[0], screen, buffer,
                                SARCOPHAGUS_GHOST_G, 41, 21);
            break;
          }
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
                 kind == CASTING_IMPACT_IMPACT_DINOX ||
                 kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
        if (frame < 20) {
          s32 m;
          if (frame < 8)
            m = gCastingImpact_BitePictures[frame / 2];
          else
            m = 3;
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
          /* FAKEMATCH(dead): a once-only block. */
          do {
            /* FAKEMATCH(pin): a 56 pinned to r3 for the first draw. */
            register s32 pinned __asm__("r3");
            HeapSlot_Release(HEAP_SLOT_RAM_CODE);
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                                BLITTER_ADDITIVE);
            BLITTER(screen, buffer + m * PICTURE_BYTES(BITE_HALF),
                    target[0] / 2 - BITE_HALF_WIDTH, pinned = 56,
                    BITE_HALF_WIDTH, BITE_HALF_HEIGHT);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE);
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                BLITTER_ADDITIVE);
          } while (0);
          BLITTER(screen, buffer + m * PICTURE_BYTES(BITE_HALF), target[0] / 2,
                  56, BITE_HALF_WIDTH, BITE_HALF_HEIGHT);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
          BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
        }
        if ((kind == CASTING_IMPACT_IMPACT_DINOX ||
             kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) &&
            frame < 24) {
          if (kind == CASTING_IMPACT_IMPACT_DINOX)
            limit = 16;
          else
            limit = 24;
          for (i = 0; i != limit; i++) {
            struct Particle *p;
            p = &SPARKS[i];
            if (frame >= i / 2 + 4) {
              s32 life;
              life = p->variant;
              if (life <= 9) {
                s32 half;
                half = life / 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](screen,
                                  buffer + CASTING_IMPACT_FLASH_BURST_PICTURES +
                                      half * PICTURE_BYTES(FLASH_BURST),
                                  point[0] - FLASH_BURST_WIDTH / 2,
                                  point[1] - FLASH_BURST_HEIGHT / 2,
                                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
                BattleEffectKernels_IntegrateVector3(p, PARTICLE_DAMPING_NONE,
                                                     toFIXED(1) / 16);
                p->variant++;
              }
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
        s32 m;
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        EFFECT_RELEASE_BLITTERS();
        if (action->side == EFFECT_SIDE_ENEMIES)
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                              BLITTER_TRANSPARENT);
        else
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              BLITTER_TRANSPARENT);
        drawFuncs.draw[0] = BLITTER;
        scrollA += scrollB;
        if (frame > 6)
          scrollB = scrollB * 48 / DAMPING_SCALE;
        REG_BG2X = (FIXEDtoINT(scrollA)) << 8;
        t = frame - 28;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = ((t / 2 + 8) << 8) | (BLEND_WEIGHT_MAX - t);
        if (frame >= 4 && frame < 8)
          Iwram_FillWords(screen, CANVAS_BYTES, CANVAS_FILL_WORD(1));
        if (frame > 3) {
          if (action->side == EFFECT_SIDE_ENEMIES)
            drawFuncs.draw[0](screen, buffer, 32, 21, 96, 88);
          else
            drawFuncs.draw[0](screen, buffer, 0, 21, 96, 88);
        }
        m = frame / 2;
        if (m < 10) {
          if (action->side == EFFECT_SIDE_ENEMIES)
            drawFuncs.draw[0](screen,
                              buffer + gCastingImpact_RaidensWrathPictures[m],
                              gCastingImpact_RaidensWrathX[m],
                              gCastingImpact_RaidensWrathY[m],
                              gCastingImpact_RaidensWrathWidths[m],
                              gCastingImpact_RaidensWrathHeights[m]);
          else
            drawFuncs.draw[0](screen,
                              buffer + gCastingImpact_RaidensWrathPictures[m],
                              128 - gCastingImpact_RaidensWrathX[m] -
                                  gCastingImpact_RaidensWrathWidths[m],
                              gCastingImpact_RaidensWrathY[m],
                              gCastingImpact_RaidensWrathWidths[m],
                              gCastingImpact_RaidensWrathHeights[m]);
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
        s32 m;
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        if (frame == 7) {
          work->shakeFrames = 16;
          Sound_PlayFar(SE_HEAVY_IMPACT);
        }
        if (frame == 11)
          Sound_PlayFar(SE_HEAVY_IMPACT);
        if (frame == 15)
          Sound_PlayFar(SE_HEAVY_IMPACT);
        EFFECT_RELEASE_BLITTERS();
        if (action->side == EFFECT_SIDE_ENEMIES)
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                              BLITTER_TRANSPARENT);
        else
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              BLITTER_TRANSPARENT);
        drawFuncs.draw[0] = BLITTER;
        scrollA += scrollB;
        if (frame == 5)
          scrollA += 0x200000;
        if (frame > 6)
          scrollB = scrollB * 48 / DAMPING_SCALE;
        REG_BG2X = (FIXEDtoINT(scrollA)) << 8;
        t = frame - 29;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = ((t / 2 + 8) << 8) | (BLEND_WEIGHT_MAX - t);
        if (frame > 4)
          drawFuncs.draw[0](screen, buffer, 35, 17, 60, 95);
        if (frame == 5)
          Iwram_FillWords(screen, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        if (frame >= 0 && (m = frame / 3) < 7) {
          if (action->side == EFFECT_SIDE_ENEMIES)
            drawFuncs.draw[0](screen,
                              buffer + gCastingImpact_ApocalypsePictures[m],
                              gCastingImpact_ApocalypseX[m],
                              gCastingImpact_ApocalypseY[m] - 16,
                              gCastingImpact_ApocalypseWidths[m],
                              gCastingImpact_ApocalypseHeights[m]);
          else
            drawFuncs.draw[0](screen,
                              buffer + gCastingImpact_ApocalypsePictures[m],
                              128 - gCastingImpact_ApocalypseX[m] -
                                  gCastingImpact_ApocalypseWidths[m],
                              gCastingImpact_ApocalypseY[m] - 16,
                              gCastingImpact_ApocalypseWidths[m],
                              gCastingImpact_ApocalypseHeights[m]);
        }
        for (i = 0; i != 3; i++) {
          s32 m;
          t = frame - 8 - i * 2;
          if (t >= 0 && t <= 8) {
            m = t / 3;
            if (action->side == EFFECT_SIDE_ENEMIES)
              drawFuncs.draw[0](
                  screen, buffer + gCastingImpact_ApocalypseBurstPictures[m],
                  gCastingImpact_ApocalypseBurstPoints[i][0] -
                      gCastingImpact_ApocalypseBurstWidths[m] / 2,
                  gCastingImpact_ApocalypseBurstPoints[i][1] -
                      gCastingImpact_ApocalypseBurstHeights[m] / 2 - 16,
                  gCastingImpact_ApocalypseBurstWidths[m],
                  gCastingImpact_ApocalypseBurstHeights[m]);
            else
              drawFuncs.draw[0](
                  screen, buffer + gCastingImpact_ApocalypseBurstPictures[m],
                  128 - gCastingImpact_ApocalypseBurstPoints[i][0] -
                      gCastingImpact_ApocalypseBurstWidths[m] / 2,
                  gCastingImpact_ApocalypseBurstPoints[i][1] -
                      gCastingImpact_ApocalypseBurstHeights[m] / 2 - 16,
                  gCastingImpact_ApocalypseBurstWidths[m],
                  gCastingImpact_ApocalypseBurstHeights[m]);
          }
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == CASTING_IMPACT_IMPACT_ASURA) {
        s32 t;
        scrollA += scrollB;
        if (frame > 6)
          scrollB = scrollB * 48 / DAMPING_SCALE;
        REG_BG2X = (FIXEDtoINT(scrollA)) << 8;
        if (frame == 0)
          REG_BLDALPHA = BLEND_ALPHA(16, 8);
        t = frame - 24;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = BLEND_ALPHA(16 - t, BLEND_WEIGHT_MAX);
        if (frame >= 4 && frame < 6)
          Iwram_FillWords(screen, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        if (frame < 4) {
          if (action->side == EFFECT_SIDE_PARTY)
            drawFuncs.draw[0](screen, work->sheet, 0, 24, ASURA_SHEET_LOW_WIDTH,
                              104);
          else
            drawFuncs.draw[0](screen, work->sheet, 48, 24,
                              ASURA_SHEET_LOW_WIDTH, 104);
        } else {
          if (frame < 8) {
            if (action->side == EFFECT_SIDE_PARTY)
              drawFuncs.draw[0](screen, work->sheet, 0, 24,
                                ASURA_SHEET_LOW_WIDTH, 104);
            else
              drawFuncs.draw[0](screen, work->sheet, 48, 24,
                                ASURA_SHEET_LOW_WIDTH, 104);
          }
          if (action->side == EFFECT_SIDE_PARTY)
            drawFuncs.draw[0](screen, work->sheet + ASURA_SHEET_SWUNG, 16, 16,
                              ASURA_SHEET_SWUNG_WIDTH,
                              ASURA_SHEET_SWUNG_HEIGHT);
          else
            drawFuncs.draw[0](screen, work->sheet + ASURA_SHEET_SWUNG, 32, 16,
                              ASURA_SHEET_SWUNG_WIDTH,
                              ASURA_SHEET_SWUNG_HEIGHT);
          if (frame >= 6 && frame < 8)
            drawFuncs.draw[0](screen, work->sheet + ASURA_SHEET_CRESCENT, 0, 16,
                              ASURA_SHEET_CRESCENT_WIDTH,
                              ASURA_SHEET_CRESCENT_HEIGHT);
          if (frame >= 8 && frame < 10)
            drawFuncs.draw[0](screen, buffer, 0, 16, CANVAS_SIZE,
                              ASURA_CRESCENT_FADE_A_HEIGHT);
          if (frame >= 10 && frame < 12)
            drawFuncs.draw[0](screen, buffer + ASURA_CRESCENT_FADE_B, 0, 16,
                              CANVAS_SIZE, ASURA_CRESCENT_FADE_B_HEIGHT);
          if (frame >= 12 && frame < 14)
            drawFuncs.draw[0](screen, buffer + ASURA_CRESCENT_FADE_C, 0, 16,
                              CANVAS_SIZE, ASURA_CRESCENT_FADE_C_HEIGHT);
          if (frame >= 14 && frame < 16)
            drawFuncs.draw[0](screen, buffer + ASURA_CRESCENT_FADE_D, 0, 16,
                              CANVAS_SIZE, ASURA_CRESCENT_FADE_D_HEIGHT);
        }
      } else if (kind == CASTING_IMPACT_IMPACT_BROIL) {
        s32 m;
        for (i = 0; i != 12; i++) {
          if (frame >= i + 6 && frame < i + 18) {
            s32 x;
            m = (frame - i - 6) / 2;
            x = target[0] / 2 - gBattleEffect_PuffArcPictureWidths[m] / 2;
            if (i & 1)
              x += (i + 1) / 2 * 3;
            else
              x -= (i + 1) / 2 * 3;
            {
              s32 which;
              which = 1;
              if (i != 0)
                which = ((i - 1) & 3) > 1;
              drawFuncs.draw[which](
                  screen, buffer + gBattleEffect_PuffArcPictures[m], x,
                  gBattleEffect_PuffArcPictureBiasY[m] + 48,
                  gBattleEffect_PuffArcPictureWidths[m],
                  gBattleEffect_PuffArcPictureHeights[m]);
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_FROST_BITE) {
        if (frame == 0) {
          struct Particle *p;
          p = SPARKS;
          for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++, p++) {
            SET_POSITION(p, (Random16() & 127) + 32, 0, 0);
            p->velocityX = Random16() & TRIG_ANGLE_MASK;
            p->velocityY = Random16() & TRIG_ANGLE_MASK;
            p->velocityZ = Random16() & TRIG_ANGLE_MASK;
          }
          SPARKS[63].y = 159;
        }
        p = SPARKS;
        for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++, p++) {
          if (p->x >= 0 && frame >= i / 2) {
            s32 m;
            m = i & 3;
            Matrix_Initialize();
            Matrix_RotateX(p->velocityX);
            Matrix_RotateY(p->velocityY);
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2 + target[0] / 2;
            point[1] = point[1] + target[1] + 32;
            drawFuncs.draw[1](screen,
                              buffer + gCastingImpact_IceChipPictures[m],
                              point[0] - 4, point[1] - 4, 8, 8);
            p->x -= 6;
            if (p->x < 0 && ((i & 7) == 0 || i == 63)) {
              Sound_PlayFar(SE_IMPACT);
              BattleEffect_TintBattler(action->targets[0], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, 0, 4);
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
        if (frame >= 5 && frame < 50) {
          if (frame > 25)
            h = 196 - frame * 4;
          else
            h = frame * 16 - 64;
          if (h > ROCK_SPIRE_HEIGHT)
            h = ROCK_SPIRE_HEIGHT;
          drawFuncs.draw[0](screen, buffer, 48, 104 - h, ROCK_SPIRE_WIDTH, h);
        }
      } else if (kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
        struct Particle *p;
        p = SPARKS;
        if (frame == 0) {
          p->x = INTtoFIXED(target[0] / 2);
          p->y = INTtoFIXED(target[1] + 48);
          if (action->side == EFFECT_SIDE_ENEMIES)
            p->velocityX = -toFIXED(3);
          else
            p->velocityX = toFIXED(3);
          p->velocityY = toFIXED(8);
          p->x += -p->velocityX * 12;
          p->y += -p->velocityY * 12;
        }
        if (frame <= 150) {
          REG_BLDCNT = 0;
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        } else {
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
          REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        }
        if (frame < 32) {
          drawFuncs.draw[1](screen, buffer + HAMMERSPHERE_BALL,
                            FIXEDtoINT(p->x) - HAMMERSPHERE_BALL_WIDTH / 2,
                            FIXEDtoINT(p->y) - HAMMERSPHERE_BALL_HEIGHT,
                            HAMMERSPHERE_BALL_WIDTH, HAMMERSPHERE_BALL_HEIGHT);
          if (frame < 12)
            BattleEffectKernels_IntegrateVector2(SPARKS, PARTICLE_DAMPING_NONE,
                                                 0);
          else if (frame == 12)
            p->velocityY = -p->velocityY / 3;
          else if (frame < 13 || frame > 18)
            BattleEffectKernels_IntegrateVector2(SPARKS, PARTICLE_DAMPING_NONE,
                                                 toFIXED(1) / 4);
        }
        if (frame == 11) {
          Sound_PlayFar(SE_CLATTER);
          work->shakeFrames = 16;
        }
      } else if (kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
                 kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
        s32 x;
        s32 y;
        if (frame < 6) {
          if (action->side == EFFECT_SIDE_ENEMIES) {
            x = target[0] / 2 + (6 - frame) * 3 * 2;
            y = target[1] - (6 - frame) * 3 * 4 + 24;
          } else {
            x = target[0] / 2 - (6 - frame) * 3 * 2;
            y = target[1] - (6 - frame) * 3 * 4 + 24;
          }
          drawFuncs.draw[1](screen, buffer, x - 16, y - 32, 32, 64);
        }
      } else if (kind == CASTING_IMPACT_IMPACT_ICE_CRUSH) {
        s32 x;
        s32 y;
        if (frame > 31)
          REG_BLDALPHA = BLEND_ALPHA(48 - frame, BLEND_WEIGHT_MAX);
        if (frame < 14) {
          struct Particle *p;
          if (action->side == EFFECT_SIDE_ENEMIES) {
            x = target[0] / 2 + (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          } else {
            x = target[0] / 2 - (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          }
          drawFuncs.draw[1](screen, buffer, x - 16, y - 32, 32, 64);
          if (frame == 13) {
            Sound_PlayFar(SE_HEAVY_IMPACT);
            work->shakeFrames = 8;
            BattleEffect_StartActorMotion(action->targets[0], 1, toFIXED(2),
                                          toFIXED(8), toFIXED(3), 150);
            p = SPARKS;
            table = gPrism_ShardStarts;
            for (i = 0; i != 16; i++, p++) {
              p->x = INTtoFIXED(*table++ + x - 40);
              p->y = INTtoFIXED(*table++);
              p->velocityX = ((Random16() & 127) - 64) << 11;
              p->velocityY = -(Random16() & 127) << 11;
              p->z = 32;
              p->variant = 0;
            }
          }
        } else {
          for (i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++) {
            s32 m;
            particle = &SPARKS[i];
            {
              s32 a;
              a = i % 5 * 3;
              m = a + particle->variant / 96 % 3;
            }
            drawFuncs.draw[i > 2 ? 0 : 1](
                screen, buffer + 0x800 + gPrism_ShardPictures[m],
                (FIXEDtoINT(particle->x)) - gPrism_ShardWidths[m] / 2,
                (FIXEDtoINT(particle->y)) - gPrism_ShardHeights[m] / 2,
                gPrism_ShardWidths[m], gPrism_ShardHeights[m]);
            BattleEffectKernels_IntegrateVector2(
                particle, PARTICLE_DAMPING_NONE, toFIXED(1) / 8);
            particle->variant += particle->z;
            if (particle->z > 1 && (frame & 1))
              particle->z--;
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_MOTHER_EARTH) {
        s32 x;
        s32 y;
        if (frame == 0) {
          struct Particle *p;
          Resource_LoadAndDecompress((s32)&ResourceId_BoulderPicture,
                                     buffer + CASTING_IMPACT_BOULDER_PICTURES,
                                     1, 1);
          REG_BLDALPHA = BLEND_ALPHA(16, 4);
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
          REG_BG2PA = Q_8_8(1);
          p = work->particles;
          for (i = 0; i != 32; i++, p++) {
            SET_POSITION(p, INTtoFIXED((Random16() & 31) + 48),
                         INTtoFIXED((Random16() & 31) + 64), Random16());
            p->velocityX = ((Random16() & 63) - 32) << 12;
            p->velocityY = -((Random16() & 127) + 16) << 12;
            p->velocityZ = (Random16() & 15) + 16;
            p->variant = 0;
          }
        }
        if (frame == 14) {
          BattleEffect_StartActorMotion(action->targets[0], 1, toFIXED(2),
                                        toFIXED(8), toFIXED(3), 150);
          Sound_PlayFar(SE_HEAVY_IMPACT);
          work->shakeFrames = 8;
        }
        i = 0;
        particle = work->particles;
        if (frame < 14) {
          if (action->side == EFFECT_SIDE_ENEMIES) {
            x = target[0] + (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          } else {
            x = target[0] - (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          }
          drawFuncs.draw[0](screen, buffer + CASTING_IMPACT_BOULDER_PICTURES,
                            32, y - 32, 64, 64);
          REG_BG2X = (64 - x) << 8;
        } else {
          for (; i != 32; i++, particle++) {
            s32 m;
            s32 w;
            s32 h;
            s32 left;
            s32 top;
            m = i % 5 * 3;
            m += particle->z / 96 % 3;
            w = gSpire_ShardWidths[m];
            left = (FIXEDtoINT(particle->x)) - w / 2;
            top = FIXEDtoINT(particle->y);
            h = gSpire_ShardHeights[m];
            top -= h / 2;
            drawFuncs.draw[0](screen, buffer + 0x83c + gSpire_ShardPictures[m],
                              left, top, w, h);
            BattleEffectKernels_IntegrateVector2(particle, 63, toFIXED(1) / 2);
            particle->z += particle->velocityZ;
            if ((frame & 1) && particle->velocityZ > 0)
              particle->velocityZ--;
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_STUN_BOLT) {
        s32 fade;
        s32 angle;
        if (frame == 0) {
          struct Particle *p;
          p = RINGS;
          for (i = 0; i != 32; i++, p++) {
            p->z = Random16();
            p->velocityZ = (Random16() & 255) + 128;
            if (Random16() & 1)
              p->velocityZ = -p->velocityZ;
            p->variant = Random16() & 3;
          }
        }
        if (frame == 4)
          work->shakeFrames = 8;
        if (frame == 6)
          Sound_PlayFar(SE_RISE_WITH_SPARKS);
        if (frame == 24)
          Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
        if (frame >= 8 && frame < 24) {
          s32 y;
          for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
            u32 flip;
            m = i & 3;
            angle = Random16() & TRIG_ANGLE_MASK;
            x = (FIXEDtoINT(Trig_Sin(angle) << 4)) + target[0] / 2 -
                gBattleEffect_GlintPictureWidths[m] / 2;
            y = (FIXEDtoINT(Trig_Cos(angle) << 4)) -
                gBattleEffect_GlintPictureHeights[m] / 2;
            EFFECT_RELEASE_BLITTERS();
            flip = Random16();
            EFFECT_LOAD_BLITTER(
                188, gCastingImpact_StunBoltGlintFlips[flip & 3] | 3, 1);
            ALT_BLITTER(screen, buffer + gBattleEffect_GlintPictures[m], x,
                        y + 80, gBattleEffect_GlintPictureWidths[m],
                        gBattleEffect_GlintPictureHeights[m]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
          }
        }
        if (frame > 3) {
          struct Particle *p;
          struct RenderModel *model;
          matrix = Heap_AllocEwram(32);
          model = Render_AllocateModelList(1);
          stunBoltTexture.logWidth = 5;
          stunBoltTexture.logHeight = 7;
          model->kind = MODEL_SHADED_MAX;
          model->texture = &stunBoltTexture;
          model->mesh = gBattleEffect_ColumnMesh;
          model->vertices = matrix;
          for (i = 0; i != 20; i++) {
            struct Particle *p;
            p = &RINGS[i];
            if (frame >= i && frame < i + 8) {
              fade = 0;
              if (frame == i)
                fade = -16;
              else if (frame >= i + 4)
                fade = -((frame - i - 4) * 16);
              model->shade = fade;
              {
                s32 life = p->variant;
                stunBoltTexture.pixels =
                    buffer + STUN_BOLT_TEXTURES +
                    life * PICTURE_BYTES(STUN_BOLT_TEXTURE);
                p->variant = (life + 1) & (STUN_BOLT_PICTURES - 1);
              }
              Matrix_Initialize();
              Matrix_Translate(INTtoFIXED(target[0] / 2 - 64),
                               INTtoFIXED(target[1] - 64), 0);
              Matrix_RotateZ(p->z);
              p->z += p->velocityZ;
              Matrix_Scale(FIXED_ONE * 4, FIXED_ONE, FIXED_ONE);
              Render_TransformVertices(gBattleEffect_TaperedBeamCorners, matrix,
                                       4);
              Render_DrawModelList(model);
            }
          }
          Heap_Release(model);
          Heap_Release(matrix);
        }
      } else if (kind == CASTING_IMPACT_IMPACT_SARGASSO ||
                 kind == CASTING_IMPACT_IMPACT_FLASH_FORCE ||
                 kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
        void *matrix;
        struct RenderModel *model;
        s32 fade;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        fade = 0;
        if (frame > 39)
          fade = (40 - frame) * 4;
        else
          work->shakeFrames = 1;
        reverseStarTexture.logWidth = 6;
        reverseStarTexture.logHeight = 6;
        reverseStarTexture.pixels = buffer;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &reverseStarTexture;
        model->mesh = gBattleEffect_SquareMesh;
        model->vertices = matrix;
        model->shade = fade;
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(target[0] / 2 - 64),
                         INTtoFIXED(target[1] - 64), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        if (frame == 8)
          Sound_PlayFar(SE_LAUNCH);
        if (fade >= -63) {
          fade = frame << 12;
          if (kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
            Matrix_RotateX(-TRIG_TURN / 32);
            Matrix_RotateY(-frame << 11);
            Matrix_RotateX(-frame << 4);
          } else {
            Matrix_RotateX(-TRIG_TURN / 16);
            Matrix_RotateY(frame << 11);
          }
          Matrix_ScaleUniform(fade);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, matrix,
                                   4);
          Render_DrawModelList(model);
          if (kind != CASTING_IMPACT_IMPACT_REVERSE_STAR) {
            Matrix_RotateY(-frame << 12);
            Matrix_ScaleUniform(FIXED_ONE / 2);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, matrix,
                                     4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
        if (kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
          s32 m;
          s32 two;
          m = frame / 4;
          two = m * 2;
          fade = 0x10000;
          if (frame > 39)
            fade = 0x38000 - (frame << 12);
          if (two > 1 && fade > 0) {
            BattleEffect_DrawShadedCircles(work->sheet, m * 4, fade);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            Canvas_BlitAtFoot(work->sheet, target[0] / 2, target[1], m * 4);
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                BLITTER_ADDITIVE);
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
                 kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
                 kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
                 kind == CASTING_IMPACT_IMPACT_BOSCA_HIT) {
        void *matrix;
        s32 t;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        boscaHitTexture.logWidth = 7;
        boscaHitTexture.logHeight = 7;
        boscaHitTexture.pixels = buffer;
        model->texture = &boscaHitTexture;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = matrix;
        if (kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
            kind == CASTING_IMPACT_IMPACT_BOSCA_HIT) {
          s32 fade;
          for (i = 0; i != 3; i++) {
            t = frame - i * 2;
            if (t >= 0) {
              angle = t * 3 << 12;
              fade = 0;
              if (t > 7)
                fade = (8 - t) * 8;
              model->shade = fade;
              if (fade > -64) {
                Matrix_Initialize();
                Matrix_Translate(INTtoFIXED(target[0] / 2 - 64),
                                 INTtoFIXED(target[1] - 64), 0);
                Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
                Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
                Matrix_RotateX(-TRIG_TURN / 16);
                Matrix_ScaleUniform(angle >> 1);
                Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                         matrix, 4);
                Render_DrawModelList(model);
              }
            }
          }
        } else if (kind == CASTING_IMPACT_IMPACT_SUPERNOVA) {
          s32 fade;
          s32 angle;
          t = frame - 4;
          if (t >= 0) {
            angle = t << 13;
            fade = 0;
            if (t > 7)
              fade = (8 - t) * 8;
            model->shade = fade;
            if (fade >= -63) {
              Matrix_Initialize();
              Matrix_Translate(INTtoFIXED(target[0] / 2 - 64),
                               INTtoFIXED(target[1] - 64), 0);
              Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
              Matrix_RotateX(DEGtoANG(90));
              Matrix_ScaleUniform(angle);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                       matrix, 4);
              Render_DrawModelList(model);
            }
          }
        } else {
          for (i = 0; i != 3; i++) {
            t = frame - i * 4;
            if (t >= 0) {
              s32 fade;
              fade = 0;
              if (t > 7)
                fade = (8 - t) * 8;
              model->shade = fade;
              if (fade > -64) {
                Matrix_Initialize();
                Matrix_Translate(INTtoFIXED(target[0] / 2 - 64),
                                 INTtoFIXED(target[1] - 64), 0);
                Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
                Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
                Matrix_RotateX(-TRIG_TURN / 16);
                Matrix_ScaleUniform(t * 3 << 12);
                Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                         matrix, 4);
                Render_DrawModelList(model);
              }
            }
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
        if (kind != CASTING_IMPACT_IMPACT_QUICK_SLASH &&
            kind != CASTING_IMPACT_IMPACT_BOSCA_HIT) {
          s32 x;
          if (frame == 4) {
            struct Particle *p;
            for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++) {
              struct Particle *p;
              p = &SPARKS[i];
              p->x = Random16();
              p->y = 0;
              p->velocityY = ((Random16() & 3) + 3) << 17;
            }
          }
          if (frame > 4) {
            for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++) {
              struct Particle *p;
              s32 k;
              p = &SPARKS[i];
              for (k = 0; k != 12; k++) {
                r = (FIXEDtoINT(p->y)) - k * 2;
                if (r >= 0) {
                  s32 n;
                  x = target[0] / 2 + (FIXEDtoINT(Trig_Sin(p->x) * r));
                  y = target[1] + (FIXEDtoINT(Trig_Cos(p->x) * r));
                  n = 2 - k / 8;
                  drawFuncs.draw[0](
                      screen, (u8 *)tiles + gBattleEffect_MotePictures[n - 1],
                      x - n / 2, y - n, n, n * 2);
                }
              }
              p->y += p->velocityY;
              p->velocityY = p->velocityY * 62 / DAMPING_SCALE;
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_STUN_JIP) {
        void *matrix;
        s32 t;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        stunJipTexture.logWidth = 7;
        stunJipTexture.logHeight = 7;
        stunJipTexture.pixels = buffer;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &stunJipTexture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = matrix;
        t = frame - 4;
        if (t >= 0) {
          s32 fade;
          angle = t << 14;
          fade = 0;
          if (t > 7)
            fade = (8 - t) * 8;
          model->shade = fade;
          if (fade >= -63) {
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(target[0] / 2 - 64), toFIXED(40), 0);
            Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
            Matrix_RotateX(1000);
            Matrix_ScaleUniform(angle);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, matrix,
                                     4);
            Render_DrawModelList(model);
          }
        }
        stunJipTexture.logWidth = 7;
        stunJipTexture.logHeight = 4;
        stunJipTexture.pixels = buffer + 0x4000;
        model->mesh = gCastingImpact_BandMesh;
        for (i = 0; i != 2; i++) {
          s32 start;
          start = gCastingImpact_StunJipBandStarts[i] + 4;
          if (frame > start) {
            fade = (start - frame) * 8;
            if (fade > -16)
              fade = -16;
            if (fade > -64) {
              scale =
                  (frame - start) * gCastingImpact_StunJipBandGrowth[i] * 1500 +
                  0x4180;
              model->shade = fade;
              Matrix_Initialize();
              Matrix_Translate(INTtoFIXED(target[0] / 2 - 60),
                               INTtoFIXED(gCastingImpact_StunJipBandHeights[i]),
                               0);
              Matrix_Scale(scale * 2, scale * 4, scale / 4);
              Matrix_RotateX(1000);
              if (i & 1)
                Matrix_RotateY((i * 2 + frame) << 12);
              else
                Matrix_RotateY((i * 2 - frame) << 12);
              Render_TransformVertices(gBattleEffect_OctagonBandCorners, matrix,
                                       16);
              Render_DrawModelList(model);
            }
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
        if (frame == 4)
          work->shakeFrames = 8;
      } else if (kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH) {
        struct ModelTexture *tex;
        s32 scale;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        /* FAKEMATCH(temporary): tex holds &armorCrushTexture. */
        tex = &armorCrushTexture;
        tex->logWidth = 6;
        tex->logHeight = 6;
        model->texture = tex;
        model->vertices = matrix;
        tex->pixels = buffer;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->mesh = gBattleEffect_RingMesh;
        for (i = 0; i != 4; i++) {
          start = gCastingImpact_ArmorCrushWaveStarts[i] + 4;
          model->scrollV = (frame * 8 + i * 32) & 127;
          if (frame == start + 4)
            Sound_PlayFar(SE_LAUNCH);
          if (frame > start) {
            fade = (start - frame) * 8 + 56;
            if (fade > -16)
              fade = -16;
            if (fade > -64) {
              scale = (frame - start) * gCastingImpact_ArmorCrushWaveGrowth[i] *
                          2000 +
                      FIXED_ONE / 4;
              model->shade = fade;
              Matrix_Initialize();
              Matrix_Translate(INTtoFIXED(target[0] / 2 - 60), 0x280000, 0);
              Matrix_Scale(scale,
                           gCastingImpact_ArmorCrushWaveStretch[i] * scale * 2,
                           scale / 4);
              Matrix_RotateX(1000);
              if (i & 1)
                Matrix_RotateY((i * 8 + frame) << 10);
              else
                Matrix_RotateY((i * 8 - frame) << 10);
              Render_TransformVertices(gBattleEffect_RingCorners, matrix, 32);
              Render_DrawModelList(model);
            }
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
        if (frame == 4)
          work->shakeFrames = 20;
      } else if (kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS) {
        if (frame == 0) {
          struct Particle *p;
          p = SPARKS;
          for (i = 0; i != 16; i++, p++) {
            p->variant = -i * 2;
            p->x = Random16();
            p->velocityX = (Random16() & 511) + 512;
            p->z = i * 2 + 80;
          }
        }
        for (i = 0; i != 0x400; i++) {
          if (buffer[0x400 + i] >> 1 == frame / 2)
            buffer[0x800 + i] = buffer[i];
        }
        EffectPosition_ProjectScaledBattler(action->targets[0], targetPoint);
        targetPoint[0] = targetPoint[0] / 2;
        for (i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++) {
          struct Particle *p;
          p = &SPARKS[i];
          if (p->variant >= 0 && p->z > 0) {
            s32 m;
            s32 x;
            s32 y;
            m = i % 3;
            x = targetPoint[0] + (FIXEDtoINT(Trig_Sin(p->x) * p->z));
            y = targetPoint[1] + (FIXEDtoINT(Trig_Cos(p->x) * p->z));
            p->z -= 4;
            p->x += p->velocityX;
            // Unused: this arm runs only for kind 15, so the kind 16 draw never
            // runs; the source keeps it.
            if (kind == CASTING_IMPACT_IMPACT_WHEEZE)
              drawFuncs.draw[0](screen, buffer, x - 16, y - 16,
                                FALLING_FLAME_WIDTH, 32);
            else
              drawFuncs.draw[0](screen, buffer + m * PICTURE_BYTES(SKULL),
                                x - SKULL_WIDTH / 2, y - SKULL_HEIGHT / 2,
                                SKULL_WIDTH, SKULL_HEIGHT);
          }
          p->variant++;
        }
      } else if (kind == CASTING_IMPACT_IMPACT_WHEEZE) {
        p = SPARKS;
        for (i = 0; i != 32; i++, p++) {
          if (frame >= i / 2 + 4 && p->variant <= 23) {
            s32 m;
            m = 0;
            if (p->variant > 11)
              m = (p->variant - 12) / 2;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[1](screen, buffer + m * 0x400, point[0] - 16,
                              point[1] - 16, FALLING_FLAME_WIDTH, 32);
            BattleEffectKernels_IntegrateVector3(p, 62, toFIXED(1) / 32);
            BattleEffectKernels_IntegrateVector3(p, 62, toFIXED(1) / 32);
            p->variant++;
          }
        }
        if (frame == 4)
          work->shakeFrames = 12;
        if (frame == 8)
          Sound_PlayFar(SE_CURSE);
      } else if (kind == CASTING_IMPACT_IMPACT_CHILL) {
        if (frame == 0) {
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
          REG_BLDALPHA = BLEND_ALPHA(16, 6);
        }
        p = SPARKS;
        for (i = 0; i != 24; i++, p++) {
          if (frame >= i / 2 + 4 && p->variant <= 23) {
            s32 m;
            m = 0;
            if (p->variant > 11)
              m = (p->variant - 12) / 3;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[0](screen, buffer + m * PICTURE_BYTES(FLASH_BURST),
                              point[0] - FLASH_BURST_WIDTH / 2,
                              point[1] - FLASH_BURST_HEIGHT / 2,
                              FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
            BattleEffectKernels_IntegrateVector3(p, 58, toFIXED(1) / 16);
            p->variant++;
          }
        }
        if (frame == 4)
          work->shakeFrames = 12;
        if (frame == 8)
          Sound_PlayFar(SE_CURSE);
      } else if (kind == CASTING_IMPACT_IMPACT_BAD_OMEN) {
        s32 m;
        if (frame > 47)
          REG_BLDALPHA = BLEND_ALPHA(64 - frame, BLEND_WEIGHT_MAX);
        p = SPARKS;
        for (i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++, p++) {
          m = i % 3;
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[i & 1](screen, buffer + m * PICTURE_BYTES(SKULL),
                                point[0] - SKULL_WIDTH / 2,
                                point[1] - SKULL_HEIGHT / 2, SKULL_WIDTH,
                                SKULL_HEIGHT);
          BattleEffectKernels_IntegrateVector3(p, 60, 1 << ((i & 3) + 11));
          p->variant++;
        }
      } else if (kind == CASTING_IMPACT_IMPACT_FLASH_EDGE) {
        if (frame == 18)
          Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
        for (i = 0; i != 3; i++) {
          if (frame == i * 3 + 3) {
            Sound_PlayFar(SE_POWER_UP);
            Iwram_FillWords(screen, CANVAS_BYTES, CANVAS_FILL_WORD(47));
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_SCORPIONFISH) {
        if (frame == 8)
          Sound_PlayFar(SE_CURSE);
        p = SPARKS;
        for (i = 0; i != 32; i++, p++) {
          if (frame >= i / 2 + 4) {
            s32 life;
            life = p->variant;
            if (life <= 23) {
              s32 m;
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[1](screen, buffer + m * PICTURE_BYTES(SMOKE_PUFF),
                                point[0] - SMOKE_PUFF_WIDTH / 2,
                                point[1] - SMOKE_PUFF_HEIGHT / 2,
                                SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
              BattleEffectKernels_IntegrateVector3(p, 62, toFIXED(1) / 32);
              BattleEffectKernels_IntegrateVector3(p, 62, toFIXED(1) / 32);
              p->variant++;
            }
          }
        }
      } else if (kind != CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI) {
        s32 m;
        p = SPARKS;
        for (i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++, p++) {
          if (frame >= i + 4) {
            s32 life;
            life = p->variant;
            if (life <= 23) {
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              // DEAD: kind 74 has its own arm above, so only kind 69 reaches
              // this test; the ROM keeps both compares.
              if (kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
                  kind == CASTING_IMPACT_IMPACT_SCORPIONFISH)
                drawFuncs.draw[1](screen,
                                  buffer + m * PICTURE_BYTES(SMOKE_PUFF),
                                  point[0] - SMOKE_PUFF_WIDTH / 2,
                                  point[1] - SMOKE_PUFF_HEIGHT / 2,
                                  SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
              else
                drawFuncs.draw[i & 1](screen,
                                      buffer + m * PICTURE_BYTES(SMOKE_PUFF),
                                      point[0] - SMOKE_PUFF_WIDTH / 2,
                                      point[1] - SMOKE_PUFF_HEIGHT / 2,
                                      SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
              if (kind == CASTING_IMPACT_IMPACT_ACID_BATH)
                BattleEffectKernels_IntegrateVector3(p, 60, toFIXED(1) / 64);
              else
                BattleEffectKernels_IntegrateVector3(p, 60, toFIXED(1) / 16);
              p->variant++;
            }
          }
        }
      }

      if (kind == CASTING_IMPACT_IMPACT_ECHO && frame < 44) {
        s32 half;
        EffectPosition_ProjectPoint(origin, point);
        point[0] = point[0] / 2;
        drawFuncs.draw[1](screen, buffer + CASTING_IMPACT_DJINNI_PICTURE,
                          point[0] - CASTING_IMPACT_DJINNI_WIDTH / 2,
                          point[1] - 4, CASTING_IMPACT_DJINNI_WIDTH,
                          CASTING_IMPACT_DJINNI_HEIGHT);
        if (frame == 23)
          (BattleObject_GetFar(action->user)->object)->velocityY = toFIXED(3);
        if (frame > 19)
          origin[1] += 0x40000;
        if (frame == 23) {
          struct Particle *p;
          half =
              BattleUnit_GetEffectHeightFar(action->targets[0]) / 2 + 0x1e0000;
          p = work->particles;
          for (i = 0; i != CASTING_IMPACT_SPARKS; i++, p++) {
            SET_POSITION(p, targetObject->x, half, targetObject->z);
            p->velocityX = ((Random16() & 255) - 127) << 11;
            p->velocityY = ((Random16() & 255) - 64) << 11;
            p->velocityZ = ((Random16() & 255) - 127) << 11;
            p->variant = i / 2 + 32;
          }
        }
        if (frame == 24) {
          BattleEffect_StartActorMotion(action->targets[0], 1, 0, 0, toFIXED(9),
                                        100);
          BattleEffect_TintBattler(action->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 4);
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          work->shakeFrames = 8;
        }
        if (frame >= 24 && frame < 36)
          drawFuncs.draw[1](screen,
                            work->sheet + CASTING_IMPACT_ORB_PICTURES +
                                ((frame - 24) / 2) *
                                    PICTURE_BYTES(YELLOW_ORB_PACKED),
                            target[0] / 2 - 8, 28, 20, 48);
      }
      if (kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
        for (i = 0; i != 4; i++) {
          if (frame == i * 4 + 8) {
            BattleEffect_TintBattler(action->targets[0], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, 0, 2);
            work->shakeFrames = 2;
            Sound_PlayFar(SE_IMPACT);
          }
        }
      }
      if (kind <= CASTING_IMPACT_LAST_DJINNI && frame <= 5) {
        EffectPosition_ProjectPoint(origin, point);
        point[0] = point[0] / 2;
        drawFuncs.draw[1](screen, buffer + CASTING_IMPACT_DJINNI_PICTURE,
                          point[0] - CASTING_IMPACT_DJINNI_WIDTH / 2,
                          point[1] - 4, CASTING_IMPACT_DJINNI_WIDTH,
                          CASTING_IMPACT_DJINNI_HEIGHT);
        origin[0] += center[0];
        origin[1] += center[1];
        origin[2] += center[2];
      }
      if (kind != CASTING_IMPACT_IMPACT_ECHO && frame == 3)
        EFFECT_SHOW_DAMAGE(-1);
      if (frame == 4) {
        if (kind == CASTING_IMPACT_IMPACT_STUN_JIP)
          Sound_PlayFar(SE_CRASH);
        else if (kind == CASTING_IMPACT_IMPACT_FLARE_BURST ||
                 kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
                 kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE)
          Sound_PlayFar(SE_CLATTER);
        else
          Sound_PlayFar(SE_HEAVY_IMPACT);
      }
      if (frame == 6) {
        if (kind == CASTING_IMPACT_IMPACT_FLINT ||
            kind == CASTING_IMPACT_IMPACT_SCORCH ||
            kind == CASTING_IMPACT_IMPACT_SAP ||
            kind == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
            kind == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
            kind == CASTING_IMPACT_IMPACT_VEIN_TAP ||
            kind == CASTING_IMPACT_IMPACT_HIGH_VITALS ||
            kind == CASTING_IMPACT_IMPACT_FLARE_BURST ||
            kind == CASTING_IMPACT_IMPACT_PSY_LEECH ||
            kind == CASTING_IMPACT_IMPACT_STEEL ||
            kind == CASTING_IMPACT_IMPACT_BARRAGE ||
            kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST ||
            kind == CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI ||
            kind == CASTING_IMPACT_IMPACT_POWER_DRIVE ||
            kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
            kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
            kind == CASTING_IMPACT_IMPACT_FLASH_EDGE ||
            kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
          BattleMotion_ApplyKnockbackFar(action->targets[0], KNOCKBACK_HURL);
          work->shakeFrames = 12;
        } else if (kind == CASTING_IMPACT_IMPACT_ECHO ||
                   kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
                   kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
          work->shakeFrames = 8;
        } else if (kind == CASTING_IMPACT_IMPACT_BROIL ||
                   kind == CASTING_IMPACT_IMPACT_AQUA_SOCK ||
                   kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM) {
          BattleMotion_ApplyKnockbackFar(action->targets[0], KNOCKBACK_HOP);
          work->shakeFrames = 2;
        } else if (kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
                   kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
          BattleMotion_ApplyKnockbackFar(action->targets[0], KNOCKBACK_LAUNCH);
          work->shakeFrames = 16;
        } else if (kind == CASTING_IMPACT_IMPACT_ROC ||
                   kind == CASTING_IMPACT_IMPACT_CALL_ZOMBIE ||
                   kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
                   kind == CASTING_IMPACT_IMPACT_DINOX ||
                   kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
                   kind == CASTING_IMPACT_IMPACT_BOSCA_HIT ||
                   kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
                   kind == CASTING_IMPACT_IMPACT_SCORPIONFISH) {
          BattleMotion_ApplyKnockbackFar(action->targets[0], KNOCKBACK_NUDGE);
          work->shakeFrames = 12;
        }
      }
      if (frame == 6)
        BattleEffect_TintBattler(action->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      if (frame == 14)
        BattleEffect_TintBattler(action->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }

    if (kind == CASTING_IMPACT_IMPACT_MELTDOWN ||
        kind == CASTING_IMPACT_IMPACT_HEAT_SMASH ||
        kind == CASTING_IMPACT_IMPACT_SOUR ||
        kind == CASTING_IMPACT_IMPACT_CHILL ||
        kind == CASTING_IMPACT_IMPACT_GEL ||
        kind == CASTING_IMPACT_IMPACT_ICE_CRUSH ||
        kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
        kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH) {
      Iwram_ClearWords(BG_VRAM, CANVAS_VIEW_BYTES);
      Iwram_ClearWords(screen, CANVAS_VIEW_BYTES);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      EFFECT_RELEASE_BLITTERS();
      if (kind == CASTING_IMPACT_IMPACT_MELTDOWN ||
          kind == CASTING_IMPACT_IMPACT_CHILL ||
          kind == CASTING_IMPACT_IMPACT_ICE_CRUSH ||
          kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
          kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH)
        BattleEffect_RunImpair(action);
      else if (kind == CASTING_IMPACT_IMPACT_SOUR ||
               kind == CASTING_IMPACT_IMPACT_HEAT_SMASH)
        BattleEffect_RunWeaken(action);
      else if (kind == CASTING_IMPACT_IMPACT_GEL)
        BattleEffect_RunDull(action);
      return;
    }
    if (kind == CASTING_IMPACT_IMPACT_MIST ||
        kind == CASTING_IMPACT_IMPACT_FEVER ||
        kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS ||
        kind == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
        kind == CASTING_IMPACT_IMPACT_EVIL_EYE ||
        kind == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
        kind == CASTING_IMPACT_IMPACT_MURK ||
        kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
        kind == CASTING_IMPACT_IMPACT_FLASH_PUNCH)
      Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
    EFFECT_FINISH_CANVAS();
  }
}
