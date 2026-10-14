	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.include "games/common/src/graphics/battle_fx/tile_mask.s"
	.include "games/common/src/graphics/battle_fx/cyan_spark.s"
	.include "games/common/src/graphics/battle_fx/violet_palette_a.s"
	.include "games/common/src/graphics/battle_fx/red_palette_a.s"
	.include "games/common/src/graphics/battle_fx/yellow_palette_a.s"
	.global BattleFx_StarBurstSheet
BattleFx_StarBurstSheet:
	.incbin "graphics/battle_fx/star_burst.gbapal"
	.incbin "graphics/battle_fx/star_burst.bitmap.lz"
	.byte 0xa1	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x487
	.global BattleFx_CrescentSheet
BattleFx_CrescentSheet:
	.incbin "graphics/battle_fx/crescent.gbapal"
	.incbin "graphics/battle_fx/crescent.bitmap.lz"
	.byte 0x09, 0x01	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x5ea
	.global BattleFx_RevealMaskA
BattleFx_RevealMaskA:
	.incbin "graphics/battle_fx/reveal_a.gbapal"
	.incbin "graphics/battle_fx/reveal_a.bitmap.lz"
	.byte 0x0a, 0x21	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x6c2
	.include "games/common/src/graphics/battle_fx/reveal_b.s"
	.global BattleFx_RevealMaskC
BattleFx_RevealMaskC:
	.incbin "graphics/battle_fx/reveal_c.gbapal"
	.incbin "graphics/battle_fx/reveal_c.bitmap.lz"
	.global BattleFx_RevealMaskD
BattleFx_RevealMaskD:
	.incbin "graphics/battle_fx/reveal_d.gbapal"
	.incbin "graphics/battle_fx/reveal_d.bitmap.lz"
	.balign 4, 0
	.global BattleFx_RevealMaskE
BattleFx_RevealMaskE:
	.incbin "graphics/battle_fx/reveal_e.gbapal"
	.incbin "graphics/battle_fx/reveal_e.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleFx_RevealMaskF
BattleFx_RevealMaskF:
	.incbin "graphics/battle_fx/reveal_f.gbapal"
	.incbin "graphics/battle_fx/reveal_f.bitmap.lz"
	.byte 0x30, 0xe1, 0x08	@ PACKER: left in the buffer by BattleFx_RevealMaskE+0x3e9
	.include "games/common/src/graphics/battle_fx/lime_column.s"
	.global BattleFx_RainSheet
BattleFx_RainSheet:
	.incbin "graphics/battle_fx/rain.gbapal"
	.incbin "graphics/battle_fx/rain.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x89f
	.global BattleFx_FlameSheetA
BattleFx_FlameSheetA:
	.incbin "graphics/battle_fx/flame_a.gbapal"
	.incbin "graphics/battle_fx/flame_a.bitmap.lz"
	.byte 0x2f, 0x18	@ PACKER: left in the buffer by BattleFx_RainSheet+0x4a2
	.global BattleFx_DemonFaceSheet
BattleFx_DemonFaceSheet:
	.incbin "graphics/battle_fx/demon.gbapal"
	.incbin "graphics/battle_fx/demon.bitmap.lz"
	.global BattleFx_FlameBladeSheet
BattleFx_FlameBladeSheet:
	.incbin "graphics/battle_fx/flame_bld.gbapal"
	.incbin "graphics/battle_fx/flame_bld.bitmap.lz"
	.byte 0xb7	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0xc83
	.include "games/common/src/graphics/battle_fx/sword.s"
	.global BattleFx_IceTileSheet
BattleFx_IceTileSheet:
	.incbin "common/graphics/battle_fx/ice_tile_00.gbapal"
	.incbin "common/graphics/battle_fx/ice_tile.parts.lz"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x106a
	.global BattleFx_FiveModeImage
BattleFx_FiveModeImage:
	.incbin "common/graphics/battle_fx/five_mode.bitmap.lz"
	.byte 0x96	@ PACKER: left in the buffer by BattleFx_IceTileSheet+0xfc7
	.global BattleFx_SpiderWebSheet
BattleFx_SpiderWebSheet:
	.incbin "graphics/battle_fx/web.gbapal"
	.incbin "graphics/battle_fx/web.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleFx_FiveModeImage+0x25a
	.global BattleFx_VioletLightningSheetA
BattleFx_VioletLightningSheetA:
	.incbin "graphics/battle_fx/bolt_a.gbapal"
	.incbin "graphics/battle_fx/bolt_a.bitmap.lz"
	.include "games/common/src/graphics/battle_fx/yellow_spark.s"
	.include "games/common/src/graphics/battle_fx/projectile_volley.s"
	.global BattleFx_ShurikenSheet
BattleFx_ShurikenSheet:
	.incbin "graphics/battle_fx/shuriken.gbapal"
	.incbin "graphics/battle_fx/shuriken.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleFx_VioletLightningSheetA+0x395
	.include "games/common/src/graphics/battle_fx/flare.s"
	.global BattleFx_FlareImage
BattleFx_FlareImage:
	.incbin "common/graphics/battle_fx/flare_cels.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x277
	.include "games/common/src/graphics/battle_fx/violet_palette_b.s"
	.include "games/common/src/graphics/battle_fx/fire_swirl.s"
	.global BattleFx_VioletLightningSheetB
BattleFx_VioletLightningSheetB:
	.incbin "graphics/battle_fx/bolt_b.gbapal"
	.incbin "graphics/battle_fx/bolt_b.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x1bd3
	.balign 4, 0
	.global BattleFx_FireBurstSheet
BattleFx_FireBurstSheet:
	.incbin "graphics/battle_fx/fire_burst.gbapal"
	.incbin "graphics/battle_fx/fire_burst.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.include "games/common/src/graphics/battle_fx/blue_crescent.s"
	.global BattleFx_OverlayImageA
BattleFx_OverlayImageA:
	.incbin "common/graphics/battle_fx/overlay_a.parts.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x43b
	.global BattleFx_VioletMoteSheet
BattleFx_VioletMoteSheet:
	.incbin "graphics/battle_fx/mote.gbapal"
	.incbin "graphics/battle_fx/mote.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleFx_OverlayImageA+0x111
	.global BattleFx_OverlayImageB
BattleFx_OverlayImageB:
	.incbin "graphics/battle_fx/overlay_b.parts.lz"
	.global BattleFx_FireballSheet
BattleFx_FireballSheet:
	.incbin "graphics/battle_fx/fireball.gbapal"
	.incbin "graphics/battle_fx/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x5df
	.global BattleFx_MemberBurstImage
BattleFx_MemberBurstImage:
	.incbin "common/graphics/battle_fx/member_burst.bitmap.lz"
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleFx_FireballSheet+0x49d
	.include "games/common/src/graphics/battle_fx/slash.s"
	.include "games/common/src/graphics/battle_fx/wave.s"
	.global BattleFx_TargetBurstImage
BattleFx_TargetBurstImage:
	.incbin "common/graphics/battle_fx/target_burst.bitmap.lz"
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleFx_SlashSheet+0x83e
	.include "games/common/src/graphics/battle_fx/pink_star.s"
	.include "games/common/src/graphics/battle_fx/water_spray.s"
	.include "games/common/src/graphics/battle_fx/blue_flame.s"
	.include "games/common/src/graphics/battle_fx/explosion.s"
	.include "games/common/src/graphics/battle_fx/magenta_swirl.s"
	.include "games/common/src/graphics/battle_fx/magenta_tail.s"
	.global BattleFx_ParticleSpritesA
BattleFx_ParticleSpritesA:
	.incbin "graphics/battle_fx/dot_cells_a.parts.lz"
	.byte 0x0c, 0x40, 0x08	@ PACKER: left in the buffer by BattleFx_MagentaTailSheet+0x299
	.global BattleFx_ParticleSpritesB
BattleFx_ParticleSpritesB:
	.incbin "graphics/battle_fx/dot_cells_b.parts.lz"
	.byte 0x94	@ PACKER: left in the buffer by BattleFx_ParticleSpritesA+0x1bb
	.include "games/common/src/graphics/battle_fx/dot_cells_c.s"
	.global BattleFx_ParticleSpritesD
BattleFx_ParticleSpritesD:
	.incbin "common/graphics/battle_fx/dot_cells_d.parts.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleFx_ParticleSpritesC+0x417
	.include "games/common/src/graphics/battle_fx/green_palette.s"
	.global BattleFx_SwordSlashSheet
BattleFx_SwordSlashSheet:
	.incbin "graphics/battle_fx/sword_slash_00.gbapal"
	.incbin "graphics/battle_fx/sword_slash.parts.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleFx_ParticleSpritesC+0x4c7
	.include "games/common/src/graphics/battle_fx/rune.s"
	.include "games/common/src/graphics/battle_fx/flame_column.s"
	.include "games/common/src/graphics/battle_fx/earth_wall.s"
	.include "games/common/src/graphics/battle_fx/orange_palette_a.s"
	.include "games/common/src/graphics/battle_fx/flash_burst.s"
	.include "games/common/src/graphics/battle_fx/thorn.s"
	.include "games/common/src/graphics/battle_fx/comet_a.s"
	.include "games/common/src/graphics/battle_fx/comet_b.s"
	.include "games/common/src/graphics/battle_fx/comet_c.s"
	.include "games/common/src/graphics/battle_fx/whirlwind.s"
	.include "games/common/src/graphics/battle_fx/green_pillar.s"
	.include "games/common/src/graphics/battle_fx/vine.s"
	.include "games/common/src/graphics/battle_fx/fire_pillar_a.s"
	.include "games/common/src/graphics/battle_fx/red_palette_b.s"
	.include "games/common/src/graphics/battle_fx/orange_palette_b.s"
	.include "games/common/src/graphics/battle_fx/violet_comet.s"
	.include "games/common/src/graphics/battle_fx/flameball.s"
	.include "games/common/src/graphics/battle_fx/boulder.s"
	.include "games/common/src/graphics/battle_fx/rock_wall.s"
	.global BattleFx_StarDotSheet
BattleFx_StarDotSheet:
	.incbin "common/graphics/battle_fx/star_dot_00.gbapal"
	.incbin "common/graphics/battle_fx/star_dot.parts.lz"
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleFx_RockWallSheet+0x89e
	.include "games/common/src/graphics/battle_fx/mars_djinn.s"
	.include "games/common/src/graphics/battle_fx/mars_djinn_small.s"
	.include "games/common/src/graphics/battle_fx/jupiter_djinn.s"
	.include "games/common/src/graphics/battle_fx/jupiter_djinn_small.s"
	.balign 4, 0
	.include "games/common/src/graphics/battle_fx/mercury_djinn.s"
	.include "games/common/src/graphics/battle_fx/mercury_djinn_small.s"
	.include "games/common/src/graphics/battle_fx/venus_djinn.s"
	.include "games/common/src/graphics/battle_fx/venus_djinn_small.s"
	.include "games/common/src/graphics/battle_fx/fire_blob.s"
	.include "games/common/src/graphics/battle_fx/fire_streak.s"
	.include "games/common/src/graphics/battle_fx/lava_orb.s"
	.include "games/common/src/graphics/battle_fx/yellow_ring.s"
	.include "games/common/src/graphics/battle_fx/yellow_orb.s"
	.include "games/common/src/graphics/battle_fx/bubble.s"
	.include "games/common/src/graphics/battle_fx/shield.s"
	.include "games/common/src/graphics/battle_fx/dagger.s"
	.global BattleFx_PaletteRampImage
BattleFx_PaletteRampImage:
	.incbin "common/graphics/battle_fx/spark_cells.parts.lz"
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleFx_DaggerSheet+0x196
	.global BattleFx_SmokeSheet
BattleFx_SmokeSheet:
	.incbin "common/graphics/battle_fx/smoke.gbapal"
	.incbin "common/graphics/battle_fx/smoke.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleFx_DaggerSheet+0x88b
	.include "games/common/src/graphics/battle_fx/blue_palette.s"
	.include "games/common/src/graphics/battle_fx/lime_palette.s"
	.include "games/common/src/graphics/battle_fx/pink_palette.s"
	.include "games/common/src/graphics/battle_fx/violet_palette_c.s"
	.include "games/common/src/graphics/battle_fx/cyan_palette.s"
	.include "games/common/src/graphics/battle_fx/tan_palette.s"
	.include "games/common/src/graphics/battle_fx/wisp.s"
	.include "games/common/src/graphics/battle_fx/green_vine.s"
	.global BattleFx_PortalSheet
BattleFx_PortalSheet:
	.incbin "common/graphics/battle_fx/portal_00.gbapal"
	.incbin "common/graphics/battle_fx/portal.parts.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleFx_WispSheet+0x622
	.global BattleFx_SheepSheet
BattleFx_SheepSheet:
	.incbin "graphics/battle_fx/sheep.gbapal"
	.incbin "graphics/battle_fx/sheep.bitmap.lz"
	.byte 0x13, 0x4e, 0x03	@ PACKER: left in the buffer by BattleFx_SmokeSheet+0x865
	.include "games/common/src/graphics/battle_fx/skull.s"
	.include "games/common/src/graphics/battle_fx/heart.s"
	.include "games/common/src/graphics/battle_fx/counter_reveal.s"
	.global BattleFx_RedCrescentSheetA
BattleFx_RedCrescentSheetA:
	.incbin "common/graphics/battle_fx/red_crescent_a_00.gbapal"
	.incbin "common/graphics/battle_fx/red_crescent_a.parts.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleFx_CounterRevealSheet+0x49b
	.include "games/common/src/graphics/battle_fx/blue_beast.s"
	.include "games/common/src/graphics/battle_fx/red_crescent_b.s"
	.include "games/common/src/graphics/battle_fx/spiral.s"
	.include "games/common/src/graphics/battle_fx/violet_streak.s"
	.include "games/common/src/graphics/battle_fx/crescent_moon.s"
	.global BattleFx_ThornVineSheet
BattleFx_ThornVineSheet:
	.incbin "common/graphics/battle_fx/thorn_vine_00.gbapal"
	.incbin "common/graphics/battle_fx/thorn_vine.parts.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0x691
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "common/graphics/battle_fx/ice_shard_00.gbapal"
	.incbin "common/graphics/battle_fx/ice_shard.parts.lz"
	.ifeq GAME_TLA
	.byte 0x3e	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0xa33
	.endif
	.include "games/common/src/graphics/battle_fx/ember_streak.s"
	.include "games/common/src/graphics/battle_fx/blue_arc_a.s"
	.include "games/common/src/graphics/battle_fx/blue_arc_b.s"
	.include "games/common/src/graphics/battle_fx/glow_orb.s"
	.global BattleFx_IceChipSheet
BattleFx_IceChipSheet:
	.incbin "common/graphics/battle_fx/ice_chip_00.gbapal"
	.incbin "common/graphics/battle_fx/ice_chip.parts.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleFx_GlowOrbSheet+0x63f
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "common/graphics/battle_fx/ice_block_00.gbapal"
	.incbin "common/graphics/battle_fx/ice_block.parts.lz"
	.ifeq GAME_TLA
	.byte 0x30, 0xae	@ PACKER: left in the buffer by BattleFx_BlueBeastSheet+0x1586
	.endif
	.global BattleFx_SparkleDots
BattleFx_SparkleDots:
	.incbin "common/graphics/battle_fx/sparkle_dots.parts.lz"
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x61
	.include "games/common/src/graphics/battle_fx/pink_burst.s"
	.include "games/common/src/graphics/battle_fx/light_fan.s"
	.global BattleFx_BlueBurstSheet
BattleFx_BlueBurstSheet:
	.incbin "graphics/battle_fx/blue_burst.gbapal"
	.incbin "graphics/battle_fx/blue_burst.bitmap.lz"
	.byte 0xc4, 0x99, 0xd3	@ PACKER: left in the buffer by BattleFx_LightFanSheet+0x331
	.balign 4, 0
	.include "games/common/src/graphics/battle_fx/dust_puff.s"
	.global BattleFx_GoldShellSheet
BattleFx_GoldShellSheet:
	.incbin "common/graphics/battle_fx/gold_shell_00.gbapal"
	.incbin "common/graphics/battle_fx/gold_shell.parts.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x1016
	.include "games/common/src/graphics/battle_fx/blast.s"
	.include "games/common/src/graphics/battle_fx/flame_b.s"
	.global BattleFx_FirePillarSheetB
BattleFx_FirePillarSheetB:
	.incbin "graphics/battle_fx/fire_pillar_b.gbapal"
	.incbin "graphics/battle_fx/fire_pillar_b.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleFx_BlastSheet+0x11fe
	.include "games/common/src/graphics/battle_fx/rock_spire.s"
	.include "games/common/src/graphics/battle_fx/lightning_bolt.s"
	.global BattleFx_BeamSequenceImage
BattleFx_BeamSequenceImage:
	.incbin "common/graphics/battle_fx/beam_sequence.parts.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleFx_LightningBoltSheet+0x37b
	.global BattleFx_TwelveModeImage
BattleFx_TwelveModeImage:
	.incbin "common/graphics/battle_fx/twelve_mode.bitmap.lz"
	.byte 0xa5	@ PACKER: left in the buffer by BattleFx_LightningBoltSheet+0x42f
	.include "games/common/src/graphics/battle_fx/wind_streak.s"
	.include "games/common/src/graphics/battle_fx/yellow_palette_b.s"
	.include "games/common/src/graphics/battle_fx/blue_ring.s"
	.include "games/common/src/graphics/battle_fx/red_palette_c.s"
	.include "games/common/src/graphics/battle_fx/violet_palette_d.s"
	.global BattleFx_BlueBeamSheet
BattleFx_BlueBeamSheet:
	.incbin "common/graphics/battle_fx/blue_beam_00.gbapal"
	.incbin "common/graphics/battle_fx/blue_beam.parts.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleFx_WindStreakSheet+0x6b5
	.global BattleFx_VortexSheet
BattleFx_VortexSheet:
	.incbin "graphics/battle_fx/vortex_00.gbapal"
	.incbin "graphics/battle_fx/vortex.parts.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleFx_WindStreakSheet+0x912
	.global BattleFx_TornadoSheet
BattleFx_TornadoSheet:
	.incbin "common/graphics/battle_fx/tornado_00.gbapal"
	.incbin "common/graphics/battle_fx/tornado.parts.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x1b32
	.global BattleFx_CrystalSheet
BattleFx_CrystalSheet:
	.incbin "common/graphics/battle_fx/crystal_00.gbapal"
	.incbin "common/graphics/battle_fx/crystal.parts.lz"
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleFx_TornadoSheet+0x104d
	.global BattleFx_WaterSparkleSheet
BattleFx_WaterSparkleSheet:
	.incbin "graphics/battle_fx/water_sparkle.gbapal"
	.incbin "graphics/battle_fx/water_sparkle.bitmap.lz"
	.byte 0x11	@ PACKER: left in the buffer by BattleFx_CrystalSheet+0x6b3
	.balign 4, 0
	.include "games/common/src/graphics/battle_fx/lightning_pillar.s"
	.include "games/common/src/graphics/battle_fx/staged_particle_path.s"
	.include "games/common/src/graphics/battle_fx/dual_stream_path.s"
