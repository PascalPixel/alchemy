/// @file
/// The casting impacts, and the step of the background colours toward a
/// palette.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

// Steps the 64 background colours one shade toward the resource's palette.
void BattleEffect_StepPaletteToResource(s32 resource) {
  u16 dst[64];
  u16 *col = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resource), dst, DMA_COPY_WORDS(0x20),
          REG_DMA3);
  dst[0] = 0;
  for (i = 0; i != 64; i++, col++) {
    s32 r = *col & 0x1f;
    s32 g = (*col >> 5) & 0x1f;
    s32 b = (*col >> 10) & 0x1f;
    s32 dst_r = dst[i] & 0x1f;
    s32 dst_g = (dst[i] >> 5) & 0x1f;
    s32 dst_b = (dst[i] >> 10) & 0x1f;

    if (r < dst_r)
      r++;
    else if (r > dst_r)
      r--;
    if (g < dst_g)
      g++;
    else if (g > dst_g)
      g--;
    if (b < dst_b)
      b++;
    else if (b > dst_b)
      b--;
    dst[i] = (b << 10) | (g << 5) | r;
  }
  Dma_Set(dst, (void *)BG_PLTT, DMA_COPY_HALFWORDS(64), REG_DMA3);
}

/* VIEW(ability_effects): tag struct Particle as struct
 * Particle__ability_effects; field/particle.h has another definition; not
 * tried: struct Particle__ability_effects was not found. */
struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 vz;
  s32 life;
};

struct SummonWork {
  struct Particle particle[64];
  u8 frames[0x5100];
  u8 frames2[0x1880];
  s32 wave[160];
  u8 unk7300[0x480];
  s32 transferMode;
  s32 transferValue;
  u8 unk_7788[0x20];
  s32 shakeFrames;
  u8 unk77ac[8];
  s32 fadeFrames;
  s32 fadeStep;
  u8 unk77bc[0x12c];
  s32 transferPending;
};

typedef s32 (*DrawFunc)(void *, const void *, s32, s32, s32, s32);

struct DrawFuncs {
  DrawFunc draw[2];
};

/* VIEW(ability_effects): tag struct Texture as struct Texture__ability_effects;
 * module/battle_effect/battle_effect.h has another definition; not tried:
 * struct Texture__ability_effects was not found. */
struct Texture {
  u8 width;
  u8 height;
  u16 unk2;
  u8 *pixels;
};

struct Model {
  s32 kind;
  s32 unk4;
  const void *mesh;
  void *matrix;
  struct Texture *texture;
  long fade;
  u8 unk18;
  u8 frame;
};

struct IwramPointers {
  u8 unk00[0x30];
  void *bg;
  u8 unk34[0x28];
  struct SummonWork *summon;
  void *screen;
  void *tiles;
  DrawFunc blitter;
  u8 unk6c[0x50];
  DrawFunc altBlitter;
};

/// The heap-slot table (gHeapSlots), read through the slots this effect uses.
#define IWRAM ((struct IwramPointers *)Ram_HeapSlots)

#define PLTT ((void *)BG_PLTT)
#define BG_VRAM ((void *)BG_CHAR_BLOCK(1))
#define SPARKS sparks
#define RINGS ((struct Particle *)(Ram_MapCellBuffer + 0x6e00))
#define BLITTER (IWRAM->blitter)
#define ALT_BLITTER (IWRAM->altBlitter)
/* FAKEMATCH(dead): the second release in a once-only block. */
#define RELEASE_BLITTERS()                                                     \
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);                                      \
  do {                                                                         \
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);                                      \
  } while (0)

/// The casting impacts, by effect kind less 100: each is named for the first
/// ability whose effect entry selects it as a casting impact, as The Broken
/// Seal's are; those no ability selects are named UNUSED_ and for what they
/// load or share. Kinds up to CASTING_IMPACT_LAST_DJINNI are Djinn;
/// Terra Strike on are the enemies' abilities, in The Broken Seal's order.
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

void BattleEffect_RunCastingImpact(struct BattleEffectArgument *action,
                                   s32 kind) {
  u8 *buffer;
  struct Particle *sparks;
  struct SummonWork *work;
  void *screen;
  s32 frame;
  void *bg;
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
  struct Texture stunBoltTexture;
  struct Texture reverseStarTexture;
  struct Texture boscaHitTexture;
  struct Texture stunJipTexture;
  struct Texture armorCrushTexture;
  s32 step;
  s32 i;
  u8 *src;
  u8 *dst;
  s32 v;
  s32 *vec;

  buffer = Ram_MapCellBuffer;
  sparks = (struct Particle *)(Ram_MapCellBuffer + 0x6000);
  work = IWRAM->summon;
  screen = IWRAM->screen;
  bg = IWRAM->bg;
  tiles = IWRAM->tiles;
  scrollA = 0;
  scrollB = 0;

  if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
      kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
      kind == CASTING_IMPACT_IMPACT_ASURA ||
      kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS ||
      kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
      kind == CASTING_IMPACT_IMPACT_UNUSED_TILED_CANVAS ||
      kind == CASTING_IMPACT_IMPACT_APOCALYPSE)
    BattleEffect_StartTiledCanvas(0x2000);
  else
    BattleEffect_StartCanvasLayer(0);

  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, tiles, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->frames, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, buffer, 1, 0);
  BattleEffect_PackTileRows(buffer, work->frames + 0x5100, 40, 0x120);

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
    for (i = 1; i != 8; i++) {
      s32 fade;
      j = 0;
      fade = i * 8;
      src = buffer;
      for (; j != 0x400; j++) {
        v = *src++;
        if (v == 0) {
          buffer[j + i * 0x400] = 0;
          continue;
        }
        v -= fade;
        if (v <= 0)
          v = 1;
        buffer[j + i * 0x400] = v;
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
                               buffer + 0xc56, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_STUN_CLOUD) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, buffer + 0x1809, 1,
                               0);
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
    Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, buffer, 0, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_STUN_JIP) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, buffer + 0x4000, 0, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_STUN_BOLT) {
    s32 j;
    s32 k;

    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                               work->frames, 1, 1);
    Iwram_ClearWords(buffer + 0xc56, 0x4000);
    for (i = 0; i != 4; i++) {
      for (j = 0; j != 120; j++) {
        for (k = 0; k != 24; k++)
          buffer[0xc5a + i * 0x1000 + j * 32 + k] =
              work->frames[i * 2880 + j * 24 + k];
      }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->frames,
                               1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
             kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetA, buffer, 1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_SARGASSO ||
             kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, buffer, 0, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
             kind == CASTING_IMPACT_IMPACT_DINOX ||
             kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage, buffer, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, buffer + 0xd80,
                               1, 0);
  } else if (kind == CASTING_IMPACT_IMPACT_FLASH_FORCE) {
    s32 k;
    s32 i;

    Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, buffer + 0x1000, 1,
                               0);
    Iwram_ClearWords(buffer, 0x1000);
    for (i = 0; i != 48; i++) {
      for (k = 0; k != 24; k++) {
        buffer[0x208 + i * 64 + k] = buffer[0x1000 + i * 24 + k];
        buffer[0x237 + i * 64 - k] = buffer[0x1000 + i * 24 + k];
      }
    }
  } else if (kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
    s32 i;
    s32 k;

    BattleEffect_DrawShadedCircles(buffer, 48, 0x10000);
    for (i = 0; i != 48; i++) {
      for (k = 0; k != 24; k++) {
        buffer[0x800 + i * 48 + k] = buffer[i * 24 + k];
        buffer[0x800 + i * 48 + 47 - k] = buffer[i * 24 + k];
        buffer[0x800 + (95 - i) * 48 + k] = buffer[i * 24 + k];
        buffer[0x800 + (95 - i) * 48 + 47 - k] = buffer[i * 24 + k];
      }
    }
  } else if (kind == CASTING_IMPACT_IMPACT_MOTHER_EARTH) {
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, buffer, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderPicture, buffer + 0x2000,
                               1, 1);
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
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_BlastSheet), PLTT,
            0x84000020, REG_DMA3);
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                                 buffer + 0x3c56, 1, 0);
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
                                 buffer + 0x3c56, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet),
              PLTT, 0x84000020, REG_DMA3);
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
                                 buffer + 0x3c56, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
              PLTT, 0x84000020, REG_DMA3);
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
                                 buffer + 0x3c56, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSmallSheet),
              PLTT, 0x84000020, REG_DMA3);
    break;
  case CASTING_IMPACT_IMPACT_BOSCA_HIT:
  case CASTING_IMPACT_IMPACT_ROC:
  case CASTING_IMPACT_IMPACT_WILD_WOLF:
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_DustPuffSheet), PLTT,
            0x84000020, REG_DMA3);
    break;
  case CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI:
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               buffer + 0x3c56, 1, 1);
    break;
  }

  if (kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
      kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
      kind == CASTING_IMPACT_IMPACT_STUN_JIP ||
      kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
      kind == CASTING_IMPACT_IMPACT_BOSCA_HIT)
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, buffer, 0, 0);
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (kind == CASTING_IMPACT_IMPACT_BAD_OMEN)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EffectPosition_ProjectScaledBattler(action->actors[0], target);
  EffectPosition_ProjectScaledBattler(action->actor, base);
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
    struct Model *model;
    const u8 *table;
    s32 k;
    struct Particle *p;

    work->fadeFrames = 24;
    work->fadeStep = 0;
    object = BattleObject_GetFar(action->actor)->object;
    for (i = 0; i != 64; i++) {
      work->particle[i].x = (Random16() & 63) + 32;
      work->particle[i].y = 0;
      work->particle[i].z = 0;
      work->particle[i].vx = Random16() & 0xffff;
      work->particle[i].vy = Random16() & 0xffff;
      work->particle[i].vz = Random16() & 0xffff;
    }
    Actor_SetAnimationSpeedFar(object, 0);
    /* FAKEMATCH(temporary): vec holds origin and then base while each is
     * filled. */
    vec = origin;
    vec[0] = object->x;
    vec[1] = object->y + 0x500000;
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
    EffectPosition_ProjectScaledBattler(action->actor, vec);
    vec[0] = vec[0] / 2;
    Sound_PlayFar(SE_POWER_UP);

    for (frame = 0; frame != 32; frame++) {
      struct Particle *p;
      s32 n;
      count = 0;
      p = work->particle;
      for (i = 0; i != 64; i++, p++) {
        if (p->x >= 0) {
          if (frame >= i / 4) {
            n = 5;
            Transform_Reset();
            Transform_ApplyRoll(p->vz);
            Transform_ApplyPitch(p->vx);
            Transform_ApplyYaw(p->vy);
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
      if (kind <= CASTING_IMPACT_LAST_DJINNI && count < 64) {
        Transform_Reset();
        Transform_SetLookAt(bg, (u8 *)bg + 12);
        EffectPosition_ProjectPoint(origin, pos);
        pos[0] = pos[0] / 2;
        drawFuncs.draw[0](screen, buffer + 0x3c56, pos[0] - 10, pos[1] - 4, 20,
                          40);
        /* FAKEMATCH(barrier): an empty asm reading origin. */
        __asm__("" : : "X"(origin));
      }
      EFFECT_PRESENT_CANVAS(work);
    }

    if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == 0)
        REG_BG2X = (frame - target[0]) << 8;
      else
        REG_BG2X = (96 - target[0]) << 8;
    } else if (kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == 0)
        REG_BG2X = (64 - target[0]) << 8;
      else
        REG_BG2X = (64 - target[0]) << 8;
    } else if (kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == 0) {
        scrollA = 0xffc00000;
        scrollB = 0x20000;
      } else {
        scrollA = 0xffc80000;
        scrollB = 0xfffe0000;
      }
      REG_BG2X = (scrollA >> 16) << 8;
    } else if (kind == CASTING_IMPACT_IMPACT_ASURA ||
               kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == 0) {
        scrollA = 0xff800000;
        scrollB = 0xc0000;
      } else {
        scrollA = 0;
        scrollB = 0xfff40000;
      }
      REG_BG2X = (scrollA >> 16) << 8;
    }
    if (kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
      REG_BG2PA = Q_8_8(1);
      REG_BG2X = (64 - target[0]) << 8;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
      Iwram_ClearWords(BG_VRAM, 0x3c00);
      Iwram_ClearWords(screen, 0x3c00);
      REG_BLDCNT = 0;
    }
    if (kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
      REG_BG2PA = Q_8_8(1);
      if (action->side == 0)
        REG_BG2X = (32 - target[0]) << 8;
      else
        REG_BG2X = (96 - target[0]) << 8;
    }
    if (kind == CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_GATHERING ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME ||
        kind == CASTING_IMPACT_IMPACT_UNUSED_COMETS) {
      Iwram_ClearWords(BG_VRAM, 0x3c00);
      Iwram_ClearWords(screen, 0x3c00);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      Actor_SelectAnimationFar(object, 3);
      if (kind == CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS)
        BattleEffect_RunProjectileVolley(action, 9);
      if (kind == CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME)
        BattleEffect_RunMysticFlame(action);
      if (kind == CASTING_IMPACT_IMPACT_UNUSED_COMETS)
        BattleEffect_RunProjectileVolley(action, 8);
      return;
    }
    Actor_SetAnimationSpeedFar(object, 16);
    object->velocityX = savedVelocityX;
    object->velocityY = savedVelocityY;
    object->velocityZ = save2c;
    object->acceleration = savedAcceleration;
    object->verticalMotionStrength = savedVerticalStrength;
    if (kind == CASTING_IMPACT_IMPACT_SEVERE_BLOW) {
      Iwram_ClearWords(BG_VRAM, 0x3c00);
      Iwram_ClearWords(screen, 0x3c00);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      action->variant = 3;
      BattleEffect_RunSparkGroups(action, 2);
      return;
    }
    Iwram_ClearWords(screen, 0x3c00);
    Iwram_ClearWords(BG_VRAM, 0x3c00);
    if (kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
      for (i = 1; i != 64; i++) {
        v = 31 - ((Trig_Cos(i << 8) << 5) >> 16);
        BG_PLTT[i] = (v << 10) | (v << 5) | v;
      }
    }
    targetObject = BattleObject_GetFar(action->actors[0])->object;
    {
      center[0] = (targetObject->x - origin[0]) / 6;
      center[1] = (targetObject->y - origin[1] + 0x1e0000) / 6;
      center[2] = (targetObject->z - origin[2]) / 6;
    }
    for (i = 0; i != 64; i++)
      work->particle[i].life = 0;
    if (kind != CASTING_IMPACT_IMPACT_AQUA_SOCK) {
      half = BattleUnit_GetEffectHeightFar(action->actors[0]) / 2;
      i = 0;
      p = work->particle;
      for (; i != 32; i++, p++) {
        SET_POSITION(p, targetObject->x, half, targetObject->z);
        if (kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
          p->vx = ((Random16() & 255) - 127) << 12;
          p->vy = ((Random16() & 255) - 64) << 10;
          p->vz = ((Random16() & 255) - 127) << 12;
        } else {
          p->vx = ((Random16() & 255) - 127) << 12;
          p->vy = ((Random16() & 255) - 64) << 12;
          p->vz = ((Random16() & 255) - 127) << 12;
        }
        p->vx = p->vx / 2;
        p->vy = p->vy / 2;
        p->vz = p->vz / 2;
        p->life = i / 2 + 32;
      }
    }
    if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
      Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet,
                                 work->frames, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA, buffer, 1,
                                 0);
      REG_BLDALPHA = BLEND_ALPHA(16, 14);
    }
    if (kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0F1, buffer, 1, 1);
      REG_BLDALPHA = BLEND_ALPHA(16, 8);
    }
    if (kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0DD, buffer, 1, 1);
      REG_BLDALPHA = BLEND_ALPHA(16, 8);
    }
    if (kind == CASTING_IMPACT_IMPACT_ASURA) {
      Resource_LoadAndDecompress((s32)&ResourceId_BlueBeastSheet, work->frames,
                                 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetB, buffer, 1,
                                 0);
      REG_BLDALPHA = BLEND_ALPHA(16, 14);
    }
    if (kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0F0, buffer, 1, 1);
      REG_BLDALPHA = BLEND_ALPHA(16, 14);
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
      for (i = 0; i != 64; i++) {
        struct Particle *q;
        q = &SPARKS[i];
        SET_POSITION(q, targetObject->x, px(20), targetObject->z);
        if (targetObject->x < 0)
          q->vx = -((Random16() & 255) + 128) << 11;
        else
          q->vx = ((Random16() & 255) + 128) << 11;
        q->vy = ((Random16() & 255) - 127) << 10;
        q->vz = 0;
        q->life = 0;
      }
    } else {
      struct Particle *p;
      if (kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
          kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS)
        height = 0;
      else
        height = 0x140000;
      p = SPARKS;
      for (i = 0; i != 64; i++, p++) {
        SET_POSITION(p, targetObject->x, height, targetObject->z);
        if (kind == CASTING_IMPACT_IMPACT_SCORCH ||
            kind == CASTING_IMPACT_IMPACT_BARRAGE) {
          p->vx = ((Random16() & 255) - 127) << 11;
          p->vy = (Random16() & 255) << 11;
          p->vz = ((Random16() & 255) - 127) << 11;
        } else if (kind == CASTING_IMPACT_IMPACT_STUN_CLOUD ||
                   kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
                   kind == CASTING_IMPACT_IMPACT_SCORPIONFISH ||
                   kind == CASTING_IMPACT_IMPACT_MOON_AIR ||
                   kind == CASTING_IMPACT_IMPACT_WHEEZE ||
                   kind == CASTING_IMPACT_IMPACT_DINOX ||
                   kind == CASTING_IMPACT_IMPACT_CHILL) {
          p->vx = ((Random16() & 255) - 127) << 13;
          p->vy = ((Random16() & 255) - 127) << 12;
          p->vz = ((Random16() & 255) - 127) << 13;
        } else if (kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
          p->vx = ((Random16() & 255) - 127) << 14;
          p->vy = ((Random16() & 255) - 127) << 13;
          p->vz = ((Random16() & 255) - 127) << 14;
        } else if (kind == CASTING_IMPACT_IMPACT_ACID_BATH) {
          p->vx = ((Random16() & 255) - 127) << 11;
          p->vy = (Random16() & 127) << 10;
          p->vz = ((Random16() & 255) - 127) << 11;
        } else {
          p->vx = ((Random16() & 255) - 127) << 10;
          p->vy = (Random16() & 127) << 10;
          p->vz = ((Random16() & 255) - 127) << 10;
        }
        p->vx = p->vx / 4;
        p->vy = p->vy / 4;
        p->vz = p->vz / 4;
        p->life = 0;
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
      duration = 48;
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
        w = work->wave;
        for (i = 0; i != 160; i++)
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
        BattleEffect_StepPaletteToResource((s32)&ResourceId_EffectPalette01);
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
        BattleEffect_StepPaletteToResource((s32)&ResourceId_EffectPalette05);
        break;
      }
      if (frame < 6) {
        EffectPosition_ProjectScaledBattler(action->actor, base);
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
        if (action->side == 0)
          drawFuncs.draw[0](screen, work->frames + (frame / 2) * 0xd80,
                            base[0] - 28, base[1] - 40, 48, 72);
        else
          drawFuncs.draw[0](screen, work->frames + (frame / 2) * 0xd80,
                            base[0] - 20, base[1] - 40, 48, 72);
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
          drawFuncs.draw[1](screen,
                            work->frames + 0x5100 + ((frame - 4) / 2) * 0x3c0,
                            target[0] / 2 - 8, base[1] - 24, 20, 48);
        Transform_Reset();
        Transform_SetLookAt(bg, (u8 *)bg + 12);
        if (frame > 3) {
          if (kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
            struct Particle *r;
            for (i = 0; i != 64; i++) {
              r = &work->particle[i];
              if (r->life > 0) {
                s32 n;
                n = (r->life >> 2) + 1;
                if (n > 10)
                  n = 10;
                EffectPosition_ProjectPoint(r, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](
                    screen, (u8 *)tiles + gBattleEffect_MotePictures[n - 1],
                    point[0] - n / 2, point[1] - n, n, n * 2);
                // DEAD: this arm runs only for kind 79, so the kind 89 test
                // never holds; the source keeps it.
                {
                  s32 gravity;
                  if (kind == CASTING_IMPACT_IMPACT_FLASH_PUNCH)
                    gravity = 0;
                  else
                    gravity = -0x800;
                  BattleEffectKernels_IntegrateVector3(r, 62, gravity);
                }
                r->life -= 2;
              }
            }
          } else {
            for (i = 0; i != 64; i++) {
              p = &work->particle[i / 2];
              if (p->life > 0) {
                s32 n;
                n = p->life / 20 + 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[(i / 2) & 1](
                    screen, (u8 *)tiles + gBattleEffect_MotePictures[n - 1],
                    point[0] - n / 2, point[1] - n, n, n * 2);
                BattleEffectKernels_IntegrateVector3(p, 62, -0x800);
                p->life--;
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
          BattleEffect_TintBattler(action->actor, 7, -1, -1, 0);
        if (frame == 79)
          BattleEffect_TintBattler(action->actor, 0, -1, -1, 0);
        if (frame == start) {
          struct Particle *p;
          p = SPARKS;
          for (i = 0; i != 64; i++, p++) {
            SET_POSITION(p, targetObject->x, px(20), targetObject->z);
            p->vx = ((Random16() & 255) - 128) << 10;
            p->vy = ((Random16() & 255) - 128) << 10;
            p->vz = ((Random16() & 255) - 128) << 10;
            if (kind == CASTING_IMPACT_IMPACT_FLARE_BURST) {
              p->vx <<= 1;
              p->vy <<= 1;
              p->vz <<= 1;
            }
            p->life = 0;
          }
        }
        if (frame >= start) {
          p = SPARKS;
          casterObject = BattleObject_GetFar(action->actor)->object;
          half = BattleUnit_GetEffectHeightFar(action->actor) / 2;
          slot = kind == CASTING_IMPACT_IMPACT_FLARE_BURST;
          for (i = 0; i != 32; i++, p++) {
            if (p->life >= 0) {
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
                p->vx += dx;
                p->vy += dy;
                p->vz += dz;
                if ((dx > -0x1000 && dx < 0x1000) &&
                    (dz > -0x1000 && dz < 0x1000))
                  p->life = -1;
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
          if (frame >= i + 4 && p->life <= 23) {
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[0](screen, buffer, point[0] - 20, point[1] - 32, 40,
                              64);
            BattleEffectKernels_IntegrateVector3(p, 64, 0);
            p->life++;
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
              drawFuncs.draw[0](
                  screen, buffer + 0xc56 + (((frame + i * 3) / 2) & 3) * 2880,
                  target[0] / 2 - 8, 0, 24, 104);
          }
        } else if (kind == CASTING_IMPACT_IMPACT_BLITZ &&
                   (frame >= 6 && frame < 20)) {
          s32 x;
          for (i = 0; i != 4; i++) {
            s32 m;
            angle = frame * 1848 + i * (TRIG_TURN / 4);
            m = ((frame + i * 3) / 2) & 3;
            x = target[0] / 2 + ((Trig_Sin(angle) << 5) >> 16);
            // DEAD: the cosine is computed and dropped; the source keeps the
            // call.
            Trig_Cos(angle);
            drawFuncs.draw[0](screen, buffer + 0xc56 + m * 2880, x - 8, 0, 24,
                              104);
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
            m = i & 3;
            angle = Random16() & 0xffff;
            x = ((Trig_Sin(angle) << 3) >> 16) + target[0] / 2 -
                gBattleEffect_GlintPictureWidths[m] / 2;
            y = ((Trig_Cos(angle) * amp) >> 16) + off -
                gBattleEffect_GlintPictureHeights[m] / 2;
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE);
            /* FAKEMATCH(dead): a once-only block. */
            do {
              BattleEffect_BuildBlitter(
                  188, 7, 7,
                  gCastingImpact_SquallGlintFlips[Random16() & 3] | 3, 0);
            } while (0);
            ALT_BLITTER(screen, buffer + gBattleEffect_GlintPictures[m], x,
                        y + 56, gBattleEffect_GlintPictureWidths[m],
                        gBattleEffect_GlintPictureHeights[m]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
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
            m = i & 3;
            angle = Random16() & 0xffff;
            x = ((Trig_Sin(angle) << 4) >> 16) + target[0] / 2 -
                gBattleEffect_GlintPictureWidths[m] / 2;
            y = ((Trig_Cos(angle) << 4) >> 16) -
                gBattleEffect_GlintPictureHeights[m] / 2;
            RELEASE_BLITTERS();
            BattleEffect_BuildBlitter(
                188, 7, 7, gCastingImpact_SapGlintFlips[Random16() & 3] | 3, 1);
            ALT_BLITTER(screen, buffer + gBattleEffect_GlintPictures[m], x,
                        y + 80, gBattleEffect_GlintPictureWidths[m],
                        gBattleEffect_GlintPictureHeights[m]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
            BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
          }
        }
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i + 4) {
            s32 life;
            life = p->life;
            if (life <= 23) {
              s32 m;
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[1](screen, buffer + 0x1809 + m * 0x480,
                                point[0] - 12, point[1] - 24, 24, 48);
              BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
              p->life++;
            }
          }
        }
        if (frame == 6)
          Sound_PlayFar(SE_RISE_WITH_SPARKS);
      } else if (kind == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
        s32 count;
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        if (frame >= 0 && frame < 24) {
          s32 x;
          x = target[0] / 2;
          top = frame * 32 - 232;
          scroll = frame * 16 - 48;
          if (top > 0)
            top = 0;
          while (scroll > 104)
            scroll -= 104;
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                              BLITTER_MAX);
          ALT_BLITTER(screen, buffer, x - 8, top + scroll - 104, 17, 104);
          ALT_BLITTER(screen, buffer, x - 8, top + scroll, 17, 104 - scroll);
          ALT_BLITTER(screen, buffer + 0x6e8, x - 17, top + 47, 34, 65);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
          if (frame == 8)
            work->shakeFrames = 8;
          if (frame > 1) {
            struct Particle *p;
            i = 0;
            p = work->particle;
            count = 0;
            for (; i != 64; i++, p++) {
              if (p->life == 0) {
                count++;
                SET_POSITION(p, targetObject->x, px(20), targetObject->z);
                p->vx = ((Random16() & 255) - 127) << 12;
                p->vy = ((Random16() & 255) - 64) << 10;
                p->vz = ((Random16() & 255) - 127) << 12;
                p->life = i / 2 + 32;
                if (count == 4)
                  break;
              }
            }
          }
        }
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
        s32 x;
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        if (frame >= 4 && frame < 24) {
          s32 h;
          x = target[0] / 2;
          h = 48;
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                              BLITTER_MAX);
          ALT_BLITTER(screen, buffer, x - 24, h, 24, h);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              BLITTER_MAX);
          ALT_BLITTER(screen, buffer, x, h, 24, h);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        }
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
        s32 m;
        if (frame > 15)
          REG_BLDALPHA = (32 - frame) | 0x1000;
        if (frame > 5) {
          s32 x;
          s32 o1;
          x = target[0] / 2;
          m = frame / 2 % 3;
          x -= 20;
          o1 = m * 0xa00;
          drawFuncs.draw[0](screen, buffer + 0xc56 + o1, x, 16, 40, 32);
          drawFuncs.draw[0](screen, buffer + 0x2a56 + m * 0x500, x, 48, 40, 32);
          {
            u8 *o0;
            o0 = buffer + 0x1156 + o1;
            drawFuncs.draw[0](screen, o0, x, 80, 40, 32);
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_SCORCH ||
                 kind == CASTING_IMPACT_IMPACT_BARRAGE ||
                 kind == CASTING_IMPACT_IMPACT_FLARE_BURST) {
        if (frame < 24) {
          p = SPARKS;
          for (i = 0; i != 16; i++, p++) {
            if (frame >= i / 2 + 4) {
              s32 life;
              life = p->life;
              if (life <= 11) {
                s32 half;
                half = life / 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](screen, buffer + half * 0x800, point[0] - 16,
                                  point[1] - 32, 32, 64);
                BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
                p->life++;
              }
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_FLINT) {
      } else if (kind == CASTING_IMPACT_IMPACT_ECHO) {
      } else if (kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
        y = (target[1] >> 16) + ((Trig_Cos(frame << 9) << 2) >> 16) + 16;
        if (frame < 4) {
          drawFuncs.draw[0](screen, work->frames,
                            gCastingImpact_PictureX[action->side * 7],
                            y + gCastingImpact_PictureY[0], 57, 98);
        } else {
          if (frame < 8)
            drawFuncs.draw[0](screen, work->frames,
                              gCastingImpact_PictureX[action->side * 7],
                              y + gCastingImpact_PictureY[0], 57, 98);
          drawFuncs.draw[0](screen, work->frames + 0x15d2,
                            gCastingImpact_PictureX[action->side * 7 + 1],
                            y + gCastingImpact_PictureY[1], 99, 69);
          if (frame >= 4 && frame < 6)
            Iwram_FillWords(screen, 0x4000, 0x3f3f3f3f);
          if (frame >= 6 && frame < 8)
            drawFuncs.draw[0](screen, work->frames + 0x3081,
                              gCastingImpact_PictureX[action->side * 7 + 2],
                              y + gCastingImpact_PictureY[2], 128, 91);
          if (frame >= 8 && frame < 10)
            drawFuncs.draw[0](screen, buffer,
                              gCastingImpact_PictureX[action->side * 7 + 3],
                              y + gCastingImpact_PictureY[3], 128, 91);
          if (frame >= 10 && frame < 12)
            drawFuncs.draw[0](screen, buffer + 0x2d80,
                              gCastingImpact_PictureX[action->side * 7 + 4],
                              y + gCastingImpact_PictureY[4], 128, 59);
          if (frame >= 12 && frame < 14)
            drawFuncs.draw[0](screen, buffer + 0x4b00,
                              gCastingImpact_PictureX[action->side * 7 + 5],
                              y + gCastingImpact_PictureY[5], 122, 29);
          if (frame >= 14 && frame < 16)
            drawFuncs.draw[0](screen, buffer + 0x58d2,
                              gCastingImpact_PictureX[action->side * 7 + 6],
                              y + gCastingImpact_PictureY[6], 76, 25);
        }
      } else if (kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
        s32 h;
        s32 y;
        REG_BLDCNT = 0;
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                            BLITTER_TRANSPARENT);
        drawFuncs.draw[0] = BLITTER;
        if (frame == 0) {
          for (i = 0; i != 1104; i++)
            work->frames[i] = Random16() & 31;
        }
        y = frame * 28 - 96;
        if (y > 16)
          y = 16;
        {
          /* FAKEMATCH(pin): hold, an input of the empty asm below. */
          register s32 hold __asm__("r2");
          h = 96;
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
              for (k = 0, threshold = t; k != 69; k++) {
                /* FAKEMATCH(barrier): an empty asm through threshold. */
                __asm__("" : "=r"(threshold) : "0"(threshold));
                if (work->frames[(row & 15) * 69 + k] < threshold)
                  buffer[row * 69 + k] = 0;
              }
            }
          }
        }
        drawFuncs.draw[0](screen, buffer, 29, y, 69, h);
        if (frame > 5) {
          switch ((frame - 6) / 4) {
          case 0:
            drawFuncs.draw[0](screen, buffer + 0x19e0, 32, 92, 59, 24);
            break;
          case 1:
            drawFuncs.draw[0](screen, buffer + 0x1f68, 30, 87, 70, 27);
            break;
          case 2:
            drawFuncs.draw[0](screen, buffer + 0x26ca, 23, 57, 84, 55);
            break;
          case 3:
            drawFuncs.draw[0](screen, buffer + 0x38d6, 27, 55, 75, 57);
            break;
          case 4:
            drawFuncs.draw[0](screen, buffer + 0x4989, 28, 19, 67, 86);
            break;
          case 5:
            drawFuncs.draw[0](screen, buffer + 0x600b, 33, 22, 50, 65);
            break;
          case 6:
            drawFuncs.draw[0](screen, buffer + 0x6cbd, 41, 21, 48, 29);
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
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
          /* FAKEMATCH(dead): a once-only block. */
          do {
            /* FAKEMATCH(pin): a 56 pinned to r3 for the first draw. */
            register s32 pinned __asm__("r3");
            HeapSlot_Release(HEAP_SLOT_RAM_CODE);
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                                BLITTER_ADDITIVE);
            BLITTER(screen, buffer + m * 0x360, target[0] / 2 - 18, pinned = 56,
                    18, 48);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE);
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                BLITTER_ADDITIVE);
          } while (0);
          BLITTER(screen, buffer + m * 0x360, target[0] / 2, 56, 18, 48);
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
              life = p->life;
              if (life <= 9) {
                s32 half;
                half = life / 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](screen, buffer + 0xd80 + half * 0x800,
                                  point[0] - 16, point[1] - 32, 32, 64);
                BattleEffectKernels_IntegrateVector3(p, 64, 0x1000);
                p->life++;
              }
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
        s32 m;
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        if (action->side == 0)
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                              BLITTER_TRANSPARENT);
        else
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              BLITTER_TRANSPARENT);
        drawFuncs.draw[0] = BLITTER;
        scrollA += scrollB;
        if (frame > 6)
          scrollB = scrollB * 48 / 64;
        REG_BG2X = (scrollA >> 16) << 8;
        t = frame - 28;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = ((t / 2 + 8) << 8) | (16 - t);
        if (frame >= 4 && frame < 8)
          Iwram_FillWords(screen, 0x4000, 0x01010101);
        if (frame > 3) {
          if (action->side == 0)
            drawFuncs.draw[0](screen, buffer, 32, 21, 96, 88);
          else
            drawFuncs.draw[0](screen, buffer, 0, 21, 96, 88);
        }
        m = frame / 2;
        if (m < 10) {
          if (action->side == 0)
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
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        if (action->side == 0)
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
          scrollB = scrollB * 48 / 64;
        REG_BG2X = (scrollA >> 16) << 8;
        t = frame - 29;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = ((t / 2 + 8) << 8) | (16 - t);
        if (frame > 4)
          drawFuncs.draw[0](screen, buffer, 35, 17, 60, 95);
        if (frame == 5)
          Iwram_FillWords(screen, 0x4000, 0x3f3f3f3f);
        if (frame >= 0 && (m = frame / 3) < 7) {
          if (action->side == 0)
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
            if (action->side == 0)
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
          scrollB = scrollB * 48 / 64;
        REG_BG2X = (scrollA >> 16) << 8;
        if (frame == 0)
          REG_BLDALPHA = BLEND_ALPHA(16, 8);
        t = frame - 24;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = (16 - t) | 0x1000;
        if (frame >= 4 && frame < 6)
          Iwram_FillWords(screen, 0x4000, 0x3f3f3f3f);
        if (frame < 4) {
          if (action->side == 1)
            drawFuncs.draw[0](screen, work->frames, 0, 24, 80, 104);
          else
            drawFuncs.draw[0](screen, work->frames, 48, 24, 80, 104);
        } else {
          if (frame < 8) {
            if (action->side == 1)
              drawFuncs.draw[0](screen, work->frames, 0, 24, 80, 104);
            else
              drawFuncs.draw[0](screen, work->frames, 48, 24, 80, 104);
          }
          if (action->side == 1)
            drawFuncs.draw[0](screen, work->frames + 0x1e00, 16, 16, 80, 104);
          else
            drawFuncs.draw[0](screen, work->frames + 0x1e00, 32, 16, 80, 104);
          if (frame >= 6 && frame < 8)
            drawFuncs.draw[0](screen, work->frames + 0x3e80, 0, 16, 128, 91);
          if (frame >= 8 && frame < 10)
            drawFuncs.draw[0](screen, buffer, 0, 16, 128, 91);
          if (frame >= 10 && frame < 12)
            drawFuncs.draw[0](screen, buffer + 0x2d80, 0, 16, 128, 59);
          if (frame >= 12 && frame < 14)
            drawFuncs.draw[0](screen, buffer + 0x4b00, 0, 16, 128, 29);
          if (frame >= 14 && frame < 16)
            drawFuncs.draw[0](screen, buffer + 0x5980, 0, 16, 128, 26);
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
          for (i = 0; i != 64; i++, p++) {
            SET_POSITION(p, (Random16() & 127) + 32, 0, 0);
            p->vx = Random16() & 0xffff;
            p->vy = Random16() & 0xffff;
            p->vz = Random16() & 0xffff;
          }
          SPARKS[63].y = 159;
        }
        p = SPARKS;
        for (i = 0; i != 64; i++, p++) {
          if (p->x >= 0 && frame >= i / 2) {
            s32 m;
            m = i & 3;
            Transform_Reset();
            Transform_ApplyPitch(p->vx);
            Transform_ApplyYaw(p->vy);
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2 + target[0] / 2;
            point[1] = point[1] + target[1] + 32;
            drawFuncs.draw[1](screen,
                              buffer + gCastingImpact_IceChipPictures[m],
                              point[0] - 4, point[1] - 4, 8, 8);
            p->x -= 6;
            if (p->x < 0 && ((i & 7) == 0 || i == 63)) {
              Sound_PlayFar(SE_IMPACT);
              BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 4);
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
        if (frame >= 5 && frame < 50) {
          if (frame > 25)
            h = 196 - frame * 4;
          else
            h = frame * 16 - 64;
          if (h > 96)
            h = 96;
          drawFuncs.draw[0](screen, buffer, 48, 104 - h, 32, h);
        }
      } else if (kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
        struct Particle *p;
        p = SPARKS;
        if (frame == 0) {
          p->x = (target[0] / 2) << 16;
          p->y = (target[1] + 48) << 16;
          if (action->side == 0)
            p->vx = -0x30000;
          else
            p->vx = 0x30000;
          p->vy = 0x80000;
          p->x += -p->vx * 12;
          p->y += -p->vy * 12;
        }
        if (frame <= 150) {
          REG_BLDCNT = 0;
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        } else {
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
          REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        }
        if (frame < 32) {
          drawFuncs.draw[1](screen, buffer + 0x800, (p->x >> 16) - 24,
                            (p->y >> 16) - 96, 48, 96);
          if (frame < 12)
            BattleEffectKernels_IntegrateVector2(SPARKS, 64, 0);
          else if (frame == 12)
            p->vy = -p->vy / 3;
          else if (frame < 13 || frame > 18)
            BattleEffectKernels_IntegrateVector2(SPARKS, 64, 0x4000);
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
          if (action->side == 0) {
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
          REG_BLDALPHA = (48 - frame) | 0x1000;
        if (frame < 14) {
          struct Particle *p;
          if (action->side == 0) {
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
            BattleEffect_StartActorMotion(action->actors[0], 1, 0x20000,
                                          0x80000, 0x30000, 150);
            p = SPARKS;
            table = gPrism_ShardStarts;
            for (i = 0; i != 16; i++, p++) {
              p->x = (*table++ + x - 40) << 16;
              p->y = *table++ << 16;
              p->vx = ((Random16() & 127) - 64) << 11;
              p->vy = -(Random16() & 127) << 11;
              p->z = 32;
              p->life = 0;
            }
          }
        } else {
          for (i = 0; i != 16; i++) {
            s32 m;
            particle = &SPARKS[i];
            {
              s32 a;
              a = i % 5 * 3;
              m = a + particle->life / 96 % 3;
            }
            drawFuncs.draw[i > 2 ? 0 : 1](
                screen, buffer + 0x800 + gPrism_ShardPictures[m],
                (particle->x >> 16) - gPrism_ShardWidths[m] / 2,
                (particle->y >> 16) - gPrism_ShardHeights[m] / 2,
                gPrism_ShardWidths[m], gPrism_ShardHeights[m]);
            BattleEffectKernels_IntegrateVector2(particle, 64, 0x2000);
            particle->life += particle->z;
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
                                     buffer + 0x2000, 1, 1);
          REG_BLDALPHA = BLEND_ALPHA(16, 4);
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
          REG_BG2PA = Q_8_8(1);
          p = work->particle;
          for (i = 0; i != 32; i++, p++) {
            SET_POSITION(p, ((Random16() & 31) + 48) << 16,
                         ((Random16() & 31) + 64) << 16, Random16());
            p->vx = ((Random16() & 63) - 32) << 12;
            p->vy = -((Random16() & 127) + 16) << 12;
            p->vz = (Random16() & 15) + 16;
            p->life = 0;
          }
        }
        if (frame == 14) {
          BattleEffect_StartActorMotion(action->actors[0], 1, 0x20000, 0x80000,
                                        0x30000, 150);
          Sound_PlayFar(SE_HEAVY_IMPACT);
          work->shakeFrames = 8;
        }
        i = 0;
        particle = work->particle;
        if (frame < 14) {
          if (action->side == 0) {
            x = target[0] + (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          } else {
            x = target[0] - (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          }
          drawFuncs.draw[0](screen, buffer + 0x2000, 32, y - 32, 64, 64);
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
            left = (particle->x >> 16) - w / 2;
            top = particle->y >> 16;
            h = gSpire_ShardHeights[m];
            top -= h / 2;
            drawFuncs.draw[0](screen, buffer + 0x83c + gSpire_ShardPictures[m],
                              left, top, w, h);
            BattleEffectKernels_IntegrateVector2(particle, 63, 0x8000);
            particle->z += particle->vz;
            if ((frame & 1) && particle->vz > 0)
              particle->vz--;
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
            p->vz = (Random16() & 255) + 128;
            if (Random16() & 1)
              p->vz = -p->vz;
            p->life = Random16() & 3;
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
            m = i & 3;
            angle = Random16() & 0xffff;
            x = ((Trig_Sin(angle) << 4) >> 16) + target[0] / 2 -
                gBattleEffect_GlintPictureWidths[m] / 2;
            y = ((Trig_Cos(angle) << 4) >> 16) -
                gBattleEffect_GlintPictureHeights[m] / 2;
            RELEASE_BLITTERS();
            BattleEffect_BuildBlitter(
                188, 7, 7,
                gCastingImpact_StunBoltGlintFlips[Random16() & 3] | 3, 1);
            ALT_BLITTER(screen, buffer + gBattleEffect_GlintPictures[m], x,
                        y + 80, gBattleEffect_GlintPictureWidths[m],
                        gBattleEffect_GlintPictureHeights[m]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
            BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
          }
        }
        if (frame > 3) {
          struct Particle *p;
          struct Model *model;
          matrix = Heap_AllocEwram(32);
          model = Render_AllocateModelList(1);
          stunBoltTexture.width = 5;
          stunBoltTexture.height = 7;
          model->kind = 6;
          model->texture = &stunBoltTexture;
          model->mesh = gBattleEffect_ColumnMesh;
          model->matrix = matrix;
          for (i = 0; i != 20; i++) {
            struct Particle *p;
            p = &RINGS[i];
            if (frame >= i && frame < i + 8) {
              fade = 0;
              if (frame == i)
                fade = -16;
              else if (frame >= i + 4)
                fade = -((frame - i - 4) * 16);
              model->fade = fade;
              {
                s32 life = p->life;
                stunBoltTexture.pixels = buffer + 0xc56 + (life << 12);
                p->life = (life + 1) & 3;
              }
              Transform_Reset();
              SceneTransform_ApplyPosition3((target[0] / 2 - 64) << 16,
                                            (target[1] - 64) << 16, 0);
              Transform_ApplyRoll(p->z);
              p->z += p->vz;
              SceneTransform_ApplyScale3(Q16_ONE * 4, Q16_ONE, Q16_ONE);
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
        struct Model *model;
        s32 fade;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        fade = 0;
        if (frame > 39)
          fade = (40 - frame) * 4;
        else
          work->shakeFrames = 1;
        reverseStarTexture.width = 6;
        reverseStarTexture.height = 6;
        reverseStarTexture.pixels = buffer;
        model->kind = 7;
        model->texture = &reverseStarTexture;
        model->mesh = gBattleEffect_SquareMesh;
        model->matrix = matrix;
        model->fade = fade;
        Transform_Reset();
        SceneTransform_ApplyPosition3((target[0] / 2 - 64) << 16,
                                      (target[1] - 64) << 16, 0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        if (frame == 8)
          Sound_PlayFar(SE_104);
        if (fade >= -63) {
          fade = frame << 12;
          if (kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
            Transform_ApplyPitch(-TRIG_TURN / 32);
            Transform_ApplyYaw(-frame << 11);
            Transform_ApplyPitch(-frame << 4);
          } else {
            Transform_ApplyPitch(-TRIG_TURN / 16);
            Transform_ApplyYaw(frame << 11);
          }
          SceneTransform_ApplyUniformScale(fade);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, matrix,
                                   4);
          Render_DrawModelList(model);
          if (kind != CASTING_IMPACT_IMPACT_REVERSE_STAR) {
            Transform_ApplyYaw(-frame << 12);
            SceneTransform_ApplyUniformScale(Q16_ONE / 2);
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
            BattleEffect_DrawShadedCircles(work->frames, m * 4, fade);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
            Canvas_BlitAtFoot(work->frames, target[0] / 2, target[1], m * 4);
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
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
        boscaHitTexture.width = 7;
        boscaHitTexture.height = 7;
        boscaHitTexture.pixels = buffer;
        model->texture = &boscaHitTexture;
        model->kind = 7;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->matrix = matrix;
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
              model->fade = fade;
              if (fade > -64) {
                Transform_Reset();
                SceneTransform_ApplyPosition3((target[0] / 2 - 64) << 16,
                                              (target[1] - 64) << 16, 0);
                SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
                Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
                Transform_ApplyPitch(-TRIG_TURN / 16);
                SceneTransform_ApplyUniformScale(angle >> 1);
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
            model->fade = fade;
            if (fade >= -63) {
              Transform_Reset();
              SceneTransform_ApplyPosition3((target[0] / 2 - 64) << 16,
                                            (target[1] - 64) << 16, 0);
              SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
              Transform_ApplyPitch(TRIG_TURN / 4);
              SceneTransform_ApplyUniformScale(angle);
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
              model->fade = fade;
              if (fade > -64) {
                Transform_Reset();
                SceneTransform_ApplyPosition3((target[0] / 2 - 64) << 16,
                                              (target[1] - 64) << 16, 0);
                SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
                Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
                Transform_ApplyPitch(-TRIG_TURN / 16);
                SceneTransform_ApplyUniformScale(t * 3 << 12);
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
            for (i = 0; i != 64; i++) {
              struct Particle *p;
              p = &SPARKS[i];
              p->x = Random16();
              p->y = 0;
              p->vy = ((Random16() & 3) + 3) << 17;
            }
          }
          if (frame > 4) {
            for (i = 0; i != 64; i++) {
              struct Particle *p;
              s32 k;
              p = &SPARKS[i];
              for (k = 0; k != 12; k++) {
                r = (p->y >> 16) - k * 2;
                if (r >= 0) {
                  s32 n;
                  x = target[0] / 2 + ((Trig_Sin(p->x) * r) >> 16);
                  y = target[1] + ((Trig_Cos(p->x) * r) >> 16);
                  n = 2 - k / 8;
                  drawFuncs.draw[0](
                      screen, (u8 *)tiles + gBattleEffect_MotePictures[n - 1],
                      x - n / 2, y - n, n, n * 2);
                }
              }
              p->y += p->vy;
              p->vy = p->vy * 62 / 64;
            }
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_STUN_JIP) {
        void *matrix;
        s32 t;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        stunJipTexture.width = 7;
        stunJipTexture.height = 7;
        stunJipTexture.pixels = buffer;
        model->kind = 7;
        model->texture = &stunJipTexture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->matrix = matrix;
        t = frame - 4;
        if (t >= 0) {
          s32 fade;
          angle = t << 14;
          fade = 0;
          if (t > 7)
            fade = (8 - t) * 8;
          model->fade = fade;
          if (fade >= -63) {
            Transform_Reset();
            SceneTransform_ApplyPosition3((target[0] / 2 - 64) << 16, 0x280000,
                                          0);
            SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
            Transform_ApplyPitch(1000);
            SceneTransform_ApplyUniformScale(angle);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, matrix,
                                     4);
            Render_DrawModelList(model);
          }
        }
        stunJipTexture.width = 7;
        stunJipTexture.height = 4;
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
              model->fade = fade;
              Transform_Reset();
              SceneTransform_ApplyPosition3(
                  (target[0] / 2 - 60) << 16,
                  gCastingImpact_StunJipBandHeights[i] << 16, 0);
              SceneTransform_ApplyScale3(scale * 2, scale * 4, scale / 4);
              Transform_ApplyPitch(1000);
              if (i & 1)
                Transform_ApplyYaw((i * 2 + frame) << 12);
              else
                Transform_ApplyYaw((i * 2 - frame) << 12);
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
        struct Texture *tex;
        s32 scale;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        /* FAKEMATCH(temporary): tex holds &armorCrushTexture. */
        tex = &armorCrushTexture;
        tex->width = 6;
        tex->height = 6;
        model->texture = tex;
        model->matrix = matrix;
        tex->pixels = buffer;
        model->kind = 7;
        model->mesh = gBattleEffect_RingMesh;
        for (i = 0; i != 4; i++) {
          start = gCastingImpact_ArmorCrushWaveStarts[i] + 4;
          model->frame = (frame * 8 + i * 32) & 127;
          if (frame == start + 4)
            Sound_PlayFar(SE_104);
          if (frame > start) {
            fade = (start - frame) * 8 + 56;
            if (fade > -16)
              fade = -16;
            if (fade > -64) {
              scale = (frame - start) * gCastingImpact_ArmorCrushWaveGrowth[i] *
                          2000 +
                      Q16_ONE / 4;
              model->fade = fade;
              Transform_Reset();
              SceneTransform_ApplyPosition3((target[0] / 2 - 60) << 16,
                                            0x280000, 0);
              SceneTransform_ApplyScale3(
                  scale, gCastingImpact_ArmorCrushWaveStretch[i] * scale * 2,
                  scale / 4);
              Transform_ApplyPitch(1000);
              if (i & 1)
                Transform_ApplyYaw((i * 8 + frame) << 10);
              else
                Transform_ApplyYaw((i * 8 - frame) << 10);
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
            p->life = -i * 2;
            p->x = Random16();
            p->vx = (Random16() & 511) + 512;
            p->z = i * 2 + 80;
          }
        }
        for (i = 0; i != 0x400; i++) {
          if (buffer[0x400 + i] >> 1 == frame / 2)
            buffer[0x800 + i] = buffer[i];
        }
        EffectPosition_ProjectScaledBattler(action->actors[0], targetPoint);
        targetPoint[0] = targetPoint[0] / 2;
        for (i = 0; i != 16; i++) {
          struct Particle *p;
          p = &SPARKS[i];
          if (p->life >= 0 && p->z > 0) {
            s32 m;
            s32 x;
            s32 y;
            m = i % 3;
            x = targetPoint[0] + ((Trig_Sin(p->x) * p->z) >> 16);
            y = targetPoint[1] + ((Trig_Cos(p->x) * p->z) >> 16);
            p->z -= 4;
            p->x += p->vx;
            // Unused: this arm runs only for kind 15, so the kind 16 draw never
            // runs; the source keeps it.
            if (kind == CASTING_IMPACT_IMPACT_WHEEZE)
              drawFuncs.draw[0](screen, buffer, x - 16, y - 16, 32, 32);
            else
              drawFuncs.draw[0](screen, buffer + m * 0x240, x - 12, y - 12, 24,
                                24);
          }
          p->life++;
        }
      } else if (kind == CASTING_IMPACT_IMPACT_WHEEZE) {
        p = SPARKS;
        for (i = 0; i != 32; i++, p++) {
          if (frame >= i / 2 + 4 && p->life <= 23) {
            s32 m;
            m = 0;
            if (p->life > 11)
              m = (p->life - 12) / 2;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[1](screen, buffer + m * 0x400, point[0] - 16,
                              point[1] - 16, 32, 32);
            BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
            BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
            p->life++;
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
          if (frame >= i / 2 + 4 && p->life <= 23) {
            s32 m;
            m = 0;
            if (p->life > 11)
              m = (p->life - 12) / 3;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[0](screen, buffer + m * 0x800, point[0] - 16,
                              point[1] - 32, 32, 64);
            BattleEffectKernels_IntegrateVector3(p, 58, 0x1000);
            p->life++;
          }
        }
        if (frame == 4)
          work->shakeFrames = 12;
        if (frame == 8)
          Sound_PlayFar(SE_CURSE);
      } else if (kind == CASTING_IMPACT_IMPACT_BAD_OMEN) {
        s32 m;
        if (frame > 47)
          REG_BLDALPHA = (64 - frame) | 0x1000;
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          m = i % 3;
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[i & 1](screen, buffer + m * 0x240, point[0] - 12,
                                point[1] - 12, 24, 24);
          BattleEffectKernels_IntegrateVector3(p, 60, 1 << ((i & 3) + 11));
          p->life++;
        }
      } else if (kind == CASTING_IMPACT_IMPACT_FLASH_EDGE) {
        if (frame == 18)
          Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
        for (i = 0; i != 3; i++) {
          if (frame == i * 3 + 3) {
            Sound_PlayFar(SE_POWER_UP);
            Iwram_FillWords(screen, 0x4000, 0x2f2f2f2f);
          }
        }
      } else if (kind == CASTING_IMPACT_IMPACT_SCORPIONFISH) {
        if (frame == 8)
          Sound_PlayFar(SE_CURSE);
        p = SPARKS;
        for (i = 0; i != 32; i++, p++) {
          if (frame >= i / 2 + 4) {
            s32 life;
            life = p->life;
            if (life <= 23) {
              s32 m;
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[1](screen, buffer + m * 0x480, point[0] - 12,
                                point[1] - 24, 24, 48);
              BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
              BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
              p->life++;
            }
          }
        }
      } else if (kind != CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI) {
        s32 m;
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i + 4) {
            s32 life;
            life = p->life;
            if (life <= 23) {
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              // DEAD: kind 74 has its own arm above, so only kind 69 reaches
              // this test.
              if (kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
                  kind == CASTING_IMPACT_IMPACT_SCORPIONFISH)
                drawFuncs.draw[1](screen, buffer + m * 0x480, point[0] - 12,
                                  point[1] - 24, 24, 48);
              else
                drawFuncs.draw[i & 1](screen, buffer + m * 0x480, point[0] - 12,
                                      point[1] - 24, 24, 48);
              if (kind == CASTING_IMPACT_IMPACT_ACID_BATH)
                BattleEffectKernels_IntegrateVector3(p, 60, 0x400);
              else
                BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
              p->life++;
            }
          }
        }
      }

      if (kind == CASTING_IMPACT_IMPACT_ECHO && frame < 44) {
        s32 half;
        EffectPosition_ProjectPoint(origin, point);
        point[0] = point[0] / 2;
        drawFuncs.draw[1](screen, buffer + 0x3c56, point[0] - 10, point[1] - 4,
                          20, 40);
        if (frame == 23)
          (BattleObject_GetFar(action->actor)->object)->velocityY = px(3);
        if (frame > 19)
          origin[1] += 0x40000;
        if (frame == 23) {
          struct Particle *p;
          half =
              BattleUnit_GetEffectHeightFar(action->actors[0]) / 2 + 0x1e0000;
          p = work->particle;
          for (i = 0; i != 64; i++, p++) {
            SET_POSITION(p, targetObject->x, half, targetObject->z);
            p->vx = ((Random16() & 255) - 127) << 11;
            p->vy = ((Random16() & 255) - 64) << 11;
            p->vz = ((Random16() & 255) - 127) << 11;
            p->life = i / 2 + 32;
          }
        }
        if (frame == 24) {
          BattleEffect_StartActorMotion(action->actors[0], 1, 0, 0, 0x90000,
                                        100);
          BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 4);
          BattlePlayback_StartFar(134);
          work->shakeFrames = 8;
        }
        if (frame >= 24 && frame < 36)
          drawFuncs.draw[1](screen,
                            work->frames + 0x5100 + ((frame - 24) / 2) * 0x3c0,
                            target[0] / 2 - 8, 28, 20, 48);
      }
      if (kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
        for (i = 0; i != 4; i++) {
          if (frame == i * 4 + 8) {
            BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 2);
            work->shakeFrames = 2;
            Sound_PlayFar(SE_IMPACT);
          }
        }
      }
      if (kind <= CASTING_IMPACT_LAST_DJINNI && frame <= 5) {
        EffectPosition_ProjectPoint(origin, point);
        point[0] = point[0] / 2;
        drawFuncs.draw[1](screen, buffer + 0x3c56, point[0] - 10, point[1] - 4,
                          20, 40);
        origin[0] += center[0];
        origin[1] += center[1];
        origin[2] += center[2];
      }
      if (kind != CASTING_IMPACT_IMPACT_ECHO && frame == 3)
        BattlePlayback_StartFar(-1);
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
          BattleMotion_ApplyKnockbackFar(action->actors[0], 4);
          work->shakeFrames = 12;
        } else if (kind == CASTING_IMPACT_IMPACT_ECHO ||
                   kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
                   kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
          work->shakeFrames = 8;
        } else if (kind == CASTING_IMPACT_IMPACT_BROIL ||
                   kind == CASTING_IMPACT_IMPACT_AQUA_SOCK ||
                   kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM) {
          BattleMotion_ApplyKnockbackFar(action->actors[0], 1);
          work->shakeFrames = 2;
        } else if (kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
                   kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
          BattleMotion_ApplyKnockbackFar(action->actors[0], 3);
          work->shakeFrames = 16;
        } else if (kind == CASTING_IMPACT_IMPACT_ROC ||
                   kind == CASTING_IMPACT_IMPACT_CALL_ZOMBIE ||
                   kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
                   kind == CASTING_IMPACT_IMPACT_DINOX ||
                   kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
                   kind == CASTING_IMPACT_IMPACT_BOSCA_HIT ||
                   kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
                   kind == CASTING_IMPACT_IMPACT_SCORPIONFISH) {
          BattleMotion_ApplyKnockbackFar(action->actors[0], 0);
          work->shakeFrames = 12;
        }
      }
      if (frame == 6)
        BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 4);
      if (frame == 14)
        BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 4);
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
      Iwram_ClearWords(BG_VRAM, 0x3c00);
      Iwram_ClearWords(screen, 0x3c00);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
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
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    BattleEffect_EndCanvasLayer();
  }
}
