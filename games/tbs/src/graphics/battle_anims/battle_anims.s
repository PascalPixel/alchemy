	.section .rodata
	.include "games/tbs/src/graphics/battle_anims/tile_mask.s"
	.include "games/tbs/src/graphics/battle_anims/cyan_spark.s"
	.include "games/tbs/src/graphics/battle_anims/violet_palette_a.s"
	.include "games/tbs/src/graphics/battle_anims/red_palette_a.s"
	.include "games/tbs/src/graphics/battle_anims/yellow_palette_a.s"
	.global BattleAnim_StarBurstSheet
BattleAnim_StarBurstSheet:
	.incbin "games/tbs/src/graphics/battle_anims/star_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/star_burst.bitmap.lz"
	.byte 0xa1	@ PACKER: left in the buffer by BattleAnim_CyanSparkSheet+0x487
	.global BattleAnim_CrescentSheet
BattleAnim_CrescentSheet:
	.incbin "games/tbs/src/graphics/battle_anims/crescent.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/crescent.bitmap.lz"
	.byte 0x09, 0x01	@ PACKER: left in the buffer by BattleAnim_CyanSparkSheet+0x5ea
	.global BattleAnim_RevealMaskA
BattleAnim_RevealMaskA:
	.incbin "games/tbs/src/graphics/battle_anims/reveal_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/reveal_a.bitmap.lz"
	.byte 0x0a, 0x21	@ PACKER: left in the buffer by BattleAnim_CyanSparkSheet+0x6c2
	.include "games/tbs/src/graphics/battle_anims/reveal_b.s"
	.global BattleAnim_RevealMaskC
BattleAnim_RevealMaskC:
	.incbin "games/tbs/src/graphics/battle_anims/reveal_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/reveal_c.bitmap.lz"
	.global BattleAnim_RevealMaskD
BattleAnim_RevealMaskD:
	.incbin "games/tbs/src/graphics/battle_anims/reveal_d.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/reveal_d.bitmap.lz"
	.balign 4, 0
	.global BattleAnim_RevealMaskE
BattleAnim_RevealMaskE:
	.incbin "games/tbs/src/graphics/battle_anims/reveal_e.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/reveal_e.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleAnim_RevealMaskF
BattleAnim_RevealMaskF:
	.incbin "games/tbs/src/graphics/battle_anims/reveal_f.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/reveal_f.bitmap.lz"
	.byte 0x30, 0xe1, 0x08	@ PACKER: left in the buffer by BattleAnim_RevealMaskE+0x3e9
	.include "games/tbs/src/graphics/battle_anims/lime_column.s"
	.global BattleAnim_RainSheet
BattleAnim_RainSheet:
	.incbin "games/tbs/src/graphics/battle_anims/rain.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/rain.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleAnim_CyanSparkSheet+0x89f
	.global BattleAnim_FlameSheetA
BattleAnim_FlameSheetA:
	.incbin "games/tbs/src/graphics/battle_anims/flame_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flame_a.bitmap.lz"
	.byte 0x2f, 0x18	@ PACKER: left in the buffer by BattleAnim_RainSheet+0x4a2
	.global BattleAnim_DemonFaceSheet
BattleAnim_DemonFaceSheet:
	.incbin "games/tbs/src/graphics/battle_anims/demon.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/demon.bitmap.lz"
	.global BattleAnim_FlameBladeSheet
BattleAnim_FlameBladeSheet:
	.incbin "games/tbs/src/graphics/battle_anims/flame_bld.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flame_bld.bitmap.lz"
	.byte 0xb7	@ PACKER: left in the buffer by BattleAnim_CyanSparkSheet+0xc83
	.include "games/tbs/src/graphics/battle_anims/sword.s"
	.global BattleAnim_IceTileSheet
BattleAnim_IceTileSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ice_tile_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ice_tile.bitmaps.lz"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleAnim_CyanSparkSheet+0x106a
	.global BattleAnim_FiveModeImage
BattleAnim_FiveModeImage:
	.incbin "games/tbs/src/graphics/battle_anims/five_mode.bitmap.lz"
	.byte 0x96	@ PACKER: left in the buffer by BattleAnim_IceTileSheet+0xfc7
	.global BattleAnim_SpiderWebSheet
BattleAnim_SpiderWebSheet:
	.incbin "games/tbs/src/graphics/battle_anims/web.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/web.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleAnim_FiveModeImage+0x25a
	.global BattleAnim_VioletLightningSheetA
BattleAnim_VioletLightningSheetA:
	.incbin "games/tbs/src/graphics/battle_anims/bolt_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/bolt_a.bitmap.lz"
	.include "games/tbs/src/graphics/battle_anims/yellow_spark.s"
	.include "games/tbs/src/graphics/battle_anims/projectile_volley.s"
	.global BattleAnim_ShurikenSheet
BattleAnim_ShurikenSheet:
	.incbin "games/tbs/src/graphics/battle_anims/shuriken.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/shuriken.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleAnim_VioletLightningSheetA+0x395
	.include "games/tbs/src/graphics/battle_anims/flare.s"
	.global BattleAnim_FlareImage
BattleAnim_FlareImage:
	.incbin "games/tbs/src/graphics/battle_anims/flare_cels.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleAnim_FlareSheet+0x277
	.include "games/tbs/src/graphics/battle_anims/violet_palette_b.s"
	.include "games/tbs/src/graphics/battle_anims/fire_swirl.s"
	.global BattleAnim_VioletLightningSheetB
BattleAnim_VioletLightningSheetB:
	.incbin "games/tbs/src/graphics/battle_anims/bolt_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/bolt_b.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleAnim_FlareSheet+0x1bd3
	.balign 4, 0
	.global BattleAnim_FireBurstSheet
BattleAnim_FireBurstSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fire_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_burst.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.include "games/tbs/src/graphics/battle_anims/blue_crescent.s"
	.global BattleAnim_OverlayImageA
BattleAnim_OverlayImageA:
	.incbin "games/tbs/src/graphics/battle_anims/overlay_a.bitmaps.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x43b
	.global BattleAnim_VioletMoteSheet
BattleAnim_VioletMoteSheet:
	.incbin "games/tbs/src/graphics/battle_anims/mote.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/mote.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleAnim_OverlayImageA+0x111
	.global BattleAnim_OverlayImageB
BattleAnim_OverlayImageB:
	.incbin "games/tbs/src/graphics/battle_anims/overlay_b.bitmaps.lz"
	.global BattleAnim_FireballSheet
BattleAnim_FireballSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fireball.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x5df
	.global BattleAnim_MemberBurstImage
BattleAnim_MemberBurstImage:
	.incbin "games/tbs/src/graphics/battle_anims/member_burst.bitmap.lz"
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleAnim_FireballSheet+0x49d
	.include "games/tbs/src/graphics/battle_anims/slash.s"
	.include "games/tbs/src/graphics/battle_anims/wave.s"
	.global BattleAnim_TargetBurstImage
BattleAnim_TargetBurstImage:
	.incbin "games/tbs/src/graphics/battle_anims/target_burst.bitmap.lz"
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleAnim_SlashSheet+0x83e
	.include "games/tbs/src/graphics/battle_anims/pink_star.s"
	.include "games/tbs/src/graphics/battle_anims/water_spray.s"
	.include "games/tbs/src/graphics/battle_anims/blue_flame.s"
	.include "games/tbs/src/graphics/battle_anims/explosion.s"
	.include "games/tbs/src/graphics/battle_anims/magenta_swirl.s"
	.include "games/tbs/src/graphics/battle_anims/magenta_tail.s"
	.global BattleAnim_ParticleSpritesA
BattleAnim_ParticleSpritesA:
	.incbin "games/tbs/src/graphics/battle_anims/dot_cells_a.bitmaps.lz"
	.byte 0x0c, 0x40, 0x08	@ PACKER: left in the buffer by BattleAnim_MagentaTailSheet+0x299
	.global BattleAnim_ParticleSpritesB
BattleAnim_ParticleSpritesB:
	.incbin "games/tbs/src/graphics/battle_anims/dot_cells_b.bitmaps.lz"
	.byte 0x94	@ PACKER: left in the buffer by BattleAnim_ParticleSpritesA+0x1bb
	.include "games/tbs/src/graphics/battle_anims/dot_cells_c.s"
	.global BattleAnim_ParticleSpritesD
BattleAnim_ParticleSpritesD:
	.incbin "games/tbs/src/graphics/battle_anims/dot_cells_d.bitmaps.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleAnim_ParticleSpritesC+0x417
	.include "games/tbs/src/graphics/battle_anims/green_palette.s"
	.global BattleAnim_SwordSlashSheet
BattleAnim_SwordSlashSheet:
	.incbin "games/tbs/src/graphics/battle_anims/sword_slash_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/sword_slash.bitmaps.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleAnim_ParticleSpritesC+0x4c7
	.include "games/tbs/src/graphics/battle_anims/rune.s"
	.include "games/tbs/src/graphics/battle_anims/flame_column.s"
	.include "games/tbs/src/graphics/battle_anims/earth_wall.s"
	.include "games/tbs/src/graphics/battle_anims/orange_palette_a.s"
	.include "games/tbs/src/graphics/battle_anims/flash_burst.s"
	.include "games/tbs/src/graphics/battle_anims/thorn.s"
	.include "games/tbs/src/graphics/battle_anims/comet_a.s"
	.include "games/tbs/src/graphics/battle_anims/comet_b.s"
	.include "games/tbs/src/graphics/battle_anims/comet_c.s"
	.include "games/tbs/src/graphics/battle_anims/whirlwind.s"
	.include "games/tbs/src/graphics/battle_anims/green_pillar.s"
	.include "games/tbs/src/graphics/battle_anims/vine.s"
	.include "games/tbs/src/graphics/battle_anims/fire_pillar_a.s"
	.include "games/tbs/src/graphics/battle_anims/red_palette_b.s"
	.include "games/tbs/src/graphics/battle_anims/orange_palette_b.s"
	.include "games/tbs/src/graphics/battle_anims/violet_comet.s"
	.include "games/tbs/src/graphics/battle_anims/flameball.s"
	.include "games/tbs/src/graphics/battle_anims/boulder.s"
	.include "games/tbs/src/graphics/battle_anims/rock_wall.s"
	.global BattleAnim_StarDotSheet
BattleAnim_StarDotSheet:
	.incbin "games/tbs/src/graphics/battle_anims/star_dot_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/star_dot.bitmaps.lz"
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleAnim_RockWallSheet+0x89e
	.include "games/tbs/src/graphics/battle_anims/mars_djinn.s"
	.include "games/tbs/src/graphics/battle_anims/mars_djinn_small.s"
	.include "games/tbs/src/graphics/battle_anims/jupiter_djinn.s"
	.include "games/tbs/src/graphics/battle_anims/jupiter_djinn_small.s"
	.balign 4, 0
	.include "games/tbs/src/graphics/battle_anims/mercury_djinn.s"
	.include "games/tbs/src/graphics/battle_anims/mercury_djinn_small.s"
	.include "games/tbs/src/graphics/battle_anims/venus_djinn.s"
	.include "games/tbs/src/graphics/battle_anims/venus_djinn_small.s"
	.include "games/tbs/src/graphics/battle_anims/fire_blob.s"
	.include "games/tbs/src/graphics/battle_anims/fire_streak.s"
	.include "games/tbs/src/graphics/battle_anims/lava_orb.s"
	.include "games/tbs/src/graphics/battle_anims/yellow_ring.s"
	.include "games/tbs/src/graphics/battle_anims/yellow_orb.s"
	.include "games/tbs/src/graphics/battle_anims/bubble.s"
	.include "games/tbs/src/graphics/battle_anims/shield.s"
	.include "games/tbs/src/graphics/battle_anims/dagger.s"
	.global BattleAnim_PaletteRampImage
BattleAnim_PaletteRampImage:
	.incbin "games/tbs/src/graphics/battle_anims/spark_cells.bitmaps.lz"
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleAnim_DaggerSheet+0x196
	.global BattleAnim_SmokeSheet
BattleAnim_SmokeSheet:
	.incbin "games/tbs/src/graphics/battle_anims/smoke.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/smoke.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleAnim_DaggerSheet+0x88b
	.include "games/tbs/src/graphics/battle_anims/blue_palette.s"
	.include "games/tbs/src/graphics/battle_anims/lime_palette.s"
	.include "games/tbs/src/graphics/battle_anims/pink_palette.s"
	.include "games/tbs/src/graphics/battle_anims/violet_palette_c.s"
	.include "games/tbs/src/graphics/battle_anims/cyan_palette.s"
	.include "games/tbs/src/graphics/battle_anims/tan_palette.s"
	.include "games/tbs/src/graphics/battle_anims/wisp.s"
	.include "games/tbs/src/graphics/battle_anims/green_vine.s"
	.global BattleAnim_PortalSheet
BattleAnim_PortalSheet:
	.incbin "games/tbs/src/graphics/battle_anims/portal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/portal.bitmaps.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleAnim_WispSheet+0x622
	.global BattleAnim_SheepSheet
BattleAnim_SheepSheet:
	.incbin "games/tbs/src/graphics/battle_anims/sheep.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/sheep.bitmap.lz"
	.byte 0x13, 0x4e, 0x03	@ PACKER: left in the buffer by BattleAnim_SmokeSheet+0x865
	.include "games/tbs/src/graphics/battle_anims/skull.s"
	.include "games/tbs/src/graphics/battle_anims/heart.s"
	.include "games/tbs/src/graphics/battle_anims/counter_reveal.s"
	.global BattleAnim_RedCrescentSheetA
BattleAnim_RedCrescentSheetA:
	.incbin "games/tbs/src/graphics/battle_anims/red_crescent_a_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/red_crescent_a.bitmaps.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleAnim_CounterRevealSheet+0x49b
	.include "games/tbs/src/graphics/battle_anims/blue_beast.s"
	.include "games/tbs/src/graphics/battle_anims/red_crescent_b.s"
	.include "games/tbs/src/graphics/battle_anims/spiral.s"
	.include "games/tbs/src/graphics/battle_anims/violet_streak.s"
	.include "games/tbs/src/graphics/battle_anims/crescent_moon.s"
	.global BattleAnim_ThornVineSheet
BattleAnim_ThornVineSheet:
	.incbin "games/tbs/src/graphics/battle_anims/thorn_vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/thorn_vine.bitmaps.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleAnim_VioletStreakSheet+0x691
	.global BattleAnim_IceShardSheet
BattleAnim_IceShardSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ice_shard_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ice_shard.bitmaps.lz"
	.byte 0x3e	@ PACKER: left in the buffer by BattleAnim_VioletStreakSheet+0xa33
	.include "games/tbs/src/graphics/battle_anims/ember_streak.s"
	.include "games/tbs/src/graphics/battle_anims/blue_arc_a.s"
	.include "games/tbs/src/graphics/battle_anims/blue_arc_b.s"
	.include "games/tbs/src/graphics/battle_anims/glow_orb.s"
	.global BattleAnim_IceChipSheet
BattleAnim_IceChipSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ice_chip_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ice_chip.bitmaps.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleAnim_GlowOrbSheet+0x63f
	.global BattleAnim_IceBlockSheet
BattleAnim_IceBlockSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ice_block_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ice_block.bitmaps.lz"
	.byte 0x30, 0xae	@ PACKER: left in the buffer by BattleAnim_BlueBeastSheet+0x1586
	.global BattleAnim_SparkleDots
BattleAnim_SparkleDots:
	.incbin "games/tbs/src/graphics/battle_anims/sparkle_dots.bitmaps.lz"
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleAnim_IceBlockSheet+0x61
	.include "games/tbs/src/graphics/battle_anims/pink_burst.s"
	.include "games/tbs/src/graphics/battle_anims/light_fan.s"
	.global BattleAnim_BlueBurstSheet
BattleAnim_BlueBurstSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_burst.bitmap.lz"
	.byte 0xc4, 0x99, 0xd3	@ PACKER: left in the buffer by BattleAnim_LightFanSheet+0x331
	.balign 4, 0
	.include "games/tbs/src/graphics/battle_anims/dust_puff.s"
	.global BattleAnim_GoldShellSheet
BattleAnim_GoldShellSheet:
	.incbin "games/tbs/src/graphics/battle_anims/gold_shell_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/gold_shell.bitmaps.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleAnim_IceBlockSheet+0x1016
	.include "games/tbs/src/graphics/battle_anims/blast.s"
	.include "games/tbs/src/graphics/battle_anims/flame_b.s"
	.global BattleAnim_FirePillarSheetB
BattleAnim_FirePillarSheetB:
	.incbin "games/tbs/src/graphics/battle_anims/fire_pillar_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_pillar_b.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleAnim_BlastSheet+0x11fe
	.include "games/tbs/src/graphics/battle_anims/rock_spire.s"
	.include "games/tbs/src/graphics/battle_anims/lightning_bolt.s"
	.global BattleAnim_BeamSequenceImage
BattleAnim_BeamSequenceImage:
	.incbin "games/tbs/src/graphics/battle_anims/beam_sequence.bitmaps.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleAnim_LightningBoltSheet+0x37b
	.global BattleAnim_TwelveModeImage
BattleAnim_TwelveModeImage:
	.incbin "games/tbs/src/graphics/battle_anims/twelve_mode.bitmap.lz"
	.byte 0xa5	@ PACKER: left in the buffer by BattleAnim_LightningBoltSheet+0x42f
	.include "games/tbs/src/graphics/battle_anims/wind_streak.s"
	.include "games/tbs/src/graphics/battle_anims/yellow_palette_b.s"
	.include "games/tbs/src/graphics/battle_anims/blue_ring.s"
	.include "games/tbs/src/graphics/battle_anims/red_palette_c.s"
	.include "games/tbs/src/graphics/battle_anims/violet_palette_d.s"
	.global BattleAnim_BlueBeamSheet
BattleAnim_BlueBeamSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_beam_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_beam.bitmaps.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleAnim_WindStreakSheet+0x6b5
	.global BattleAnim_VortexSheet
BattleAnim_VortexSheet:
	.incbin "games/tbs/src/graphics/battle_anims/vortex_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/vortex.bitmaps.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleAnim_WindStreakSheet+0x912
	.global BattleAnim_TornadoSheet
BattleAnim_TornadoSheet:
	.incbin "games/tbs/src/graphics/battle_anims/tornado_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/tornado.bitmaps.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x1b32
	.global BattleAnim_CrystalSheet
BattleAnim_CrystalSheet:
	.incbin "games/tbs/src/graphics/battle_anims/crystal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/crystal.bitmaps.lz"
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleAnim_TornadoSheet+0x104d
	.global BattleAnim_WaterSparkleSheet
BattleAnim_WaterSparkleSheet:
	.incbin "games/tbs/src/graphics/battle_anims/water_sparkle.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/water_sparkle.bitmap.lz"
	.byte 0x11	@ PACKER: left in the buffer by BattleAnim_CrystalSheet+0x6b3
	.balign 4, 0
	.include "games/tbs/src/graphics/battle_anims/lightning_pillar.s"
