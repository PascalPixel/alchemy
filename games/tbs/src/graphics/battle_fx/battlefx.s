	.section .rodata
	.include "games/tbs/src/graphics/battle_fx/tile_mask.s"
	.include "games/tbs/src/graphics/battle_fx/cyan_spark.s"
	.include "games/tbs/src/graphics/battle_fx/violet_palette_a.s"
	.include "games/tbs/src/graphics/battle_fx/red_palette_a.s"
	.include "games/tbs/src/graphics/battle_fx/yellow_palette_a.s"
	.global BattleEffect_StarBurstSheet
BattleEffect_StarBurstSheet:
	.incbin "games/tbs/src/graphics/battle_fx/star_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/star_burst.bitmap.lz"
	.byte 0xa1	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x487
	.global BattleEffect_CrescentSheet
BattleEffect_CrescentSheet:
	.incbin "games/tbs/src/graphics/battle_fx/crescent.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/crescent.bitmap.lz"
	.byte 0x09, 0x01	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x5ea
	.global BattleEffect_RevealMaskA
BattleEffect_RevealMaskA:
	.incbin "games/tbs/src/graphics/battle_fx/reveal_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/reveal_a.bitmap.lz"
	.byte 0x0a, 0x21	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x6c2
	.include "games/tbs/src/graphics/battle_fx/reveal_b.s"
	.global BattleEffect_RevealMaskC
BattleEffect_RevealMaskC:
	.incbin "games/tbs/src/graphics/battle_fx/reveal_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/reveal_c.bitmap.lz"
	.global BattleEffect_RevealMaskD
BattleEffect_RevealMaskD:
	.incbin "games/tbs/src/graphics/battle_fx/reveal_d.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/reveal_d.bitmap.lz"
	.balign 4, 0
	.global BattleEffect_RevealMaskE
BattleEffect_RevealMaskE:
	.incbin "games/tbs/src/graphics/battle_fx/reveal_e.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/reveal_e.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_RevealMaskF
BattleEffect_RevealMaskF:
	.incbin "games/tbs/src/graphics/battle_fx/reveal_f.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/reveal_f.bitmap.lz"
	.byte 0x30, 0xe1, 0x08	@ PACKER: left in the buffer by BattleEffect_RevealMaskE+0x3e9
	.include "games/tbs/src/graphics/battle_fx/lime_column.s"
	.global BattleEffect_RainSheet
BattleEffect_RainSheet:
	.incbin "games/tbs/src/graphics/battle_fx/rain.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/rain.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x89f
	.global BattleEffect_FlameSheetA
BattleEffect_FlameSheetA:
	.incbin "games/tbs/src/graphics/battle_fx/flame_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/flame_a.bitmap.lz"
	.byte 0x2f, 0x18	@ PACKER: left in the buffer by BattleEffect_RainSheet+0x4a2
	.global BattleEffect_DemonFaceSheet
BattleEffect_DemonFaceSheet:
	.incbin "games/tbs/src/graphics/battle_fx/demon.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/demon.bitmap.lz"
	.global BattleEffect_FlameBladeSheet
BattleEffect_FlameBladeSheet:
	.incbin "games/tbs/src/graphics/battle_fx/flame_bld.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/flame_bld.bitmap.lz"
	.byte 0xb7	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0xc83
	.include "games/tbs/src/graphics/battle_fx/sword.s"
	.global BattleEffect_IceTileSheet
BattleEffect_IceTileSheet:
	.incbin "games/tbs/src/graphics/battle_fx/ice_tile_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/ice_tile.parts.lz"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x106a
	.global BattleEffect_FiveModeImage
BattleEffect_FiveModeImage:
	.incbin "games/tbs/src/graphics/battle_fx/five_mode.bitmap.lz"
	.byte 0x96	@ PACKER: left in the buffer by BattleEffect_IceTileSheet+0xfc7
	.global BattleEffect_SpiderWebSheet
BattleEffect_SpiderWebSheet:
	.incbin "games/tbs/src/graphics/battle_fx/web.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/web.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleEffect_FiveModeImage+0x25a
	.global BattleEffect_VioletLightningSheetA
BattleEffect_VioletLightningSheetA:
	.incbin "games/tbs/src/graphics/battle_fx/bolt_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/bolt_a.bitmap.lz"
	.include "games/tbs/src/graphics/battle_fx/yellow_spark.s"
	.include "games/tbs/src/graphics/battle_fx/projectile_volley.s"
	.global BattleEffect_ShurikenSheet
BattleEffect_ShurikenSheet:
	.incbin "games/tbs/src/graphics/battle_fx/shuriken.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/shuriken.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleEffect_VioletLightningSheetA+0x395
	.include "games/tbs/src/graphics/battle_fx/flare.s"
	.global BattleEffect_FlareImage
BattleEffect_FlareImage:
	.incbin "games/tbs/src/graphics/battle_fx/flare_cels.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x277
	.include "games/tbs/src/graphics/battle_fx/violet_palette_b.s"
	.include "games/tbs/src/graphics/battle_fx/fire_swirl.s"
	.global BattleEffect_VioletLightningSheetB
BattleEffect_VioletLightningSheetB:
	.incbin "games/tbs/src/graphics/battle_fx/bolt_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/bolt_b.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x1bd3
	.balign 4, 0
	.global BattleEffect_FireBurstSheet
BattleEffect_FireBurstSheet:
	.incbin "games/tbs/src/graphics/battle_fx/fire_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/fire_burst.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.include "games/tbs/src/graphics/battle_fx/blue_crescent.s"
	.global BattleEffect_OverlayImageA
BattleEffect_OverlayImageA:
	.incbin "games/tbs/src/graphics/battle_fx/overlay_a.parts.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x43b
	.global BattleEffect_VioletMoteSheet
BattleEffect_VioletMoteSheet:
	.incbin "games/tbs/src/graphics/battle_fx/mote.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/mote.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleEffect_OverlayImageA+0x111
	.global BattleEffect_OverlayImageB
BattleEffect_OverlayImageB:
	.incbin "games/tbs/src/graphics/battle_fx/overlay_b.parts.lz"
	.global BattleEffect_FireballSheet
BattleEffect_FireballSheet:
	.incbin "games/tbs/src/graphics/battle_fx/fireball.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x5df
	.global BattleEffect_MemberBurstImage
BattleEffect_MemberBurstImage:
	.incbin "games/tbs/src/graphics/battle_fx/member_burst.bitmap.lz"
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleEffect_FireballSheet+0x49d
	.include "games/tbs/src/graphics/battle_fx/slash.s"
	.include "games/tbs/src/graphics/battle_fx/wave.s"
	.global BattleEffect_TargetBurstImage
BattleEffect_TargetBurstImage:
	.incbin "games/tbs/src/graphics/battle_fx/target_burst.bitmap.lz"
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleEffect_SlashSheet+0x83e
	.include "games/tbs/src/graphics/battle_fx/pink_star.s"
	.include "games/tbs/src/graphics/battle_fx/water_spray.s"
	.include "games/tbs/src/graphics/battle_fx/blue_flame.s"
	.include "games/tbs/src/graphics/battle_fx/explosion.s"
	.include "games/tbs/src/graphics/battle_fx/magenta_swirl.s"
	.include "games/tbs/src/graphics/battle_fx/magenta_tail.s"
	.global BattleEffect_ParticleSpritesA
BattleEffect_ParticleSpritesA:
	.incbin "games/tbs/src/graphics/battle_fx/dot_cells_a.parts.lz"
	.byte 0x0c, 0x40, 0x08	@ PACKER: left in the buffer by BattleEffect_MagentaTailSheet+0x299
	.global BattleEffect_ParticleSpritesB
BattleEffect_ParticleSpritesB:
	.incbin "games/tbs/src/graphics/battle_fx/dot_cells_b.parts.lz"
	.byte 0x94	@ PACKER: left in the buffer by BattleEffect_ParticleSpritesA+0x1bb
	.include "games/tbs/src/graphics/battle_fx/dot_cells_c.s"
	.global BattleEffect_ParticleSpritesD
BattleEffect_ParticleSpritesD:
	.incbin "games/tbs/src/graphics/battle_fx/dot_cells_d.parts.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleEffect_ParticleSpritesC+0x417
	.include "games/tbs/src/graphics/battle_fx/green_palette.s"
	.global BattleEffect_SwordSlashSheet
BattleEffect_SwordSlashSheet:
	.incbin "games/tbs/src/graphics/battle_fx/sword_slash_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/sword_slash.parts.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleEffect_ParticleSpritesC+0x4c7
	.include "games/tbs/src/graphics/battle_fx/rune.s"
	.include "games/tbs/src/graphics/battle_fx/flame_column.s"
	.include "games/tbs/src/graphics/battle_fx/earth_wall.s"
	.include "games/tbs/src/graphics/battle_fx/orange_palette_a.s"
	.include "games/tbs/src/graphics/battle_fx/flash_burst.s"
	.include "games/tbs/src/graphics/battle_fx/thorn.s"
	.include "games/tbs/src/graphics/battle_fx/comet_a.s"
	.include "games/tbs/src/graphics/battle_fx/comet_b.s"
	.include "games/tbs/src/graphics/battle_fx/comet_c.s"
	.include "games/tbs/src/graphics/battle_fx/whirlwind.s"
	.include "games/tbs/src/graphics/battle_fx/green_pillar.s"
	.include "games/tbs/src/graphics/battle_fx/vine.s"
	.include "games/tbs/src/graphics/battle_fx/fire_pillar_a.s"
	.include "games/tbs/src/graphics/battle_fx/red_palette_b.s"
	.include "games/tbs/src/graphics/battle_fx/orange_palette_b.s"
	.include "games/tbs/src/graphics/battle_fx/violet_comet.s"
	.include "games/tbs/src/graphics/battle_fx/flameball.s"
	.include "games/tbs/src/graphics/battle_fx/boulder.s"
	.include "games/tbs/src/graphics/battle_fx/rock_wall.s"
	.global BattleEffect_StarDotSheet
BattleEffect_StarDotSheet:
	.incbin "games/tbs/src/graphics/battle_fx/star_dot_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/star_dot.parts.lz"
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleEffect_RockWallSheet+0x89e
	.include "games/tbs/src/graphics/battle_fx/mars_djinn.s"
	.include "games/tbs/src/graphics/battle_fx/mars_djinn_small.s"
	.include "games/tbs/src/graphics/battle_fx/jupiter_djinn.s"
	.include "games/tbs/src/graphics/battle_fx/jupiter_djinn_small.s"
	.balign 4, 0
	.include "games/tbs/src/graphics/battle_fx/mercury_djinn.s"
	.include "games/tbs/src/graphics/battle_fx/mercury_djinn_small.s"
	.include "games/tbs/src/graphics/battle_fx/venus_djinn.s"
	.include "games/tbs/src/graphics/battle_fx/venus_djinn_small.s"
	.include "games/tbs/src/graphics/battle_fx/fire_blob.s"
	.include "games/tbs/src/graphics/battle_fx/fire_streak.s"
	.include "games/tbs/src/graphics/battle_fx/lava_orb.s"
	.include "games/tbs/src/graphics/battle_fx/yellow_ring.s"
	.include "games/tbs/src/graphics/battle_fx/yellow_orb.s"
	.include "games/tbs/src/graphics/battle_fx/bubble.s"
	.include "games/tbs/src/graphics/battle_fx/shield.s"
	.include "games/tbs/src/graphics/battle_fx/dagger.s"
	.global BattleEffect_PaletteRampImage
BattleEffect_PaletteRampImage:
	.incbin "games/tbs/src/graphics/battle_fx/spark_cells.parts.lz"
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleEffect_DaggerSheet+0x196
	.global BattleEffect_SmokeSheet
BattleEffect_SmokeSheet:
	.incbin "games/tbs/src/graphics/battle_fx/smoke.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/smoke.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleEffect_DaggerSheet+0x88b
	.include "games/tbs/src/graphics/battle_fx/blue_palette.s"
	.include "games/tbs/src/graphics/battle_fx/lime_palette.s"
	.include "games/tbs/src/graphics/battle_fx/pink_palette.s"
	.include "games/tbs/src/graphics/battle_fx/violet_palette_c.s"
	.include "games/tbs/src/graphics/battle_fx/cyan_palette.s"
	.include "games/tbs/src/graphics/battle_fx/tan_palette.s"
	.include "games/tbs/src/graphics/battle_fx/wisp.s"
	.include "games/tbs/src/graphics/battle_fx/green_vine.s"
	.global BattleEffect_PortalSheet
BattleEffect_PortalSheet:
	.incbin "games/tbs/src/graphics/battle_fx/portal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/portal.parts.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleEffect_WispSheet+0x622
	.global BattleEffect_SheepSheet
BattleEffect_SheepSheet:
	.incbin "games/tbs/src/graphics/battle_fx/sheep.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/sheep.bitmap.lz"
	.byte 0x13, 0x4e, 0x03	@ PACKER: left in the buffer by BattleEffect_SmokeSheet+0x865
	.include "games/tbs/src/graphics/battle_fx/skull.s"
	.include "games/tbs/src/graphics/battle_fx/heart.s"
	.include "games/tbs/src/graphics/battle_fx/counter_reveal.s"
	.global BattleEffect_RedCrescentSheetA
BattleEffect_RedCrescentSheetA:
	.incbin "games/tbs/src/graphics/battle_fx/red_crescent_a_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/red_crescent_a.parts.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleEffect_CounterRevealSheet+0x49b
	.include "games/tbs/src/graphics/battle_fx/blue_beast.s"
	.include "games/tbs/src/graphics/battle_fx/red_crescent_b.s"
	.include "games/tbs/src/graphics/battle_fx/spiral.s"
	.include "games/tbs/src/graphics/battle_fx/violet_streak.s"
	.include "games/tbs/src/graphics/battle_fx/crescent_moon.s"
	.global BattleEffect_ThornVineSheet
BattleEffect_ThornVineSheet:
	.incbin "games/tbs/src/graphics/battle_fx/thorn_vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/thorn_vine.parts.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0x691
	.global BattleEffect_IceShardSheet
BattleEffect_IceShardSheet:
	.incbin "games/tbs/src/graphics/battle_fx/ice_shard_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/ice_shard.parts.lz"
	.byte 0x3e	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0xa33
	.include "games/tbs/src/graphics/battle_fx/ember_streak.s"
	.include "games/tbs/src/graphics/battle_fx/blue_arc_a.s"
	.include "games/tbs/src/graphics/battle_fx/blue_arc_b.s"
	.include "games/tbs/src/graphics/battle_fx/glow_orb.s"
	.global BattleEffect_IceChipSheet
BattleEffect_IceChipSheet:
	.incbin "games/tbs/src/graphics/battle_fx/ice_chip_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/ice_chip.parts.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleEffect_GlowOrbSheet+0x63f
	.global BattleEffect_IceBlockSheet
BattleEffect_IceBlockSheet:
	.incbin "games/tbs/src/graphics/battle_fx/ice_block_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/ice_block.parts.lz"
	.byte 0x30, 0xae	@ PACKER: left in the buffer by BattleEffect_BlueBeastSheet+0x1586
	.global BattleEffect_SparkleDots
BattleEffect_SparkleDots:
	.incbin "games/tbs/src/graphics/battle_fx/sparkle_dots.parts.lz"
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x61
	.include "games/tbs/src/graphics/battle_fx/pink_burst.s"
	.include "games/tbs/src/graphics/battle_fx/light_fan.s"
	.global BattleEffect_BlueBurstSheet
BattleEffect_BlueBurstSheet:
	.incbin "games/tbs/src/graphics/battle_fx/blue_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/blue_burst.bitmap.lz"
	.byte 0xc4, 0x99, 0xd3	@ PACKER: left in the buffer by BattleEffect_LightFanSheet+0x331
	.balign 4, 0
	.include "games/tbs/src/graphics/battle_fx/dust_puff.s"
	.global BattleEffect_GoldShellSheet
BattleEffect_GoldShellSheet:
	.incbin "games/tbs/src/graphics/battle_fx/gold_shell_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/gold_shell.parts.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x1016
	.include "games/tbs/src/graphics/battle_fx/blast.s"
	.include "games/tbs/src/graphics/battle_fx/flame_b.s"
	.global BattleEffect_FirePillarSheetB
BattleEffect_FirePillarSheetB:
	.incbin "games/tbs/src/graphics/battle_fx/fire_pillar_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/fire_pillar_b.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleEffect_BlastSheet+0x11fe
	.include "games/tbs/src/graphics/battle_fx/rock_spire.s"
	.include "games/tbs/src/graphics/battle_fx/lightning_bolt.s"
	.global BattleEffect_BeamSequenceImage
BattleEffect_BeamSequenceImage:
	.incbin "games/tbs/src/graphics/battle_fx/beam_sequence.parts.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x37b
	.global BattleEffect_TwelveModeImage
BattleEffect_TwelveModeImage:
	.incbin "games/tbs/src/graphics/battle_fx/twelve_mode.bitmap.lz"
	.byte 0xa5	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x42f
	.include "games/tbs/src/graphics/battle_fx/wind_streak.s"
	.include "games/tbs/src/graphics/battle_fx/yellow_palette_b.s"
	.include "games/tbs/src/graphics/battle_fx/blue_ring.s"
	.include "games/tbs/src/graphics/battle_fx/red_palette_c.s"
	.include "games/tbs/src/graphics/battle_fx/violet_palette_d.s"
	.global BattleEffect_BlueBeamSheet
BattleEffect_BlueBeamSheet:
	.incbin "games/tbs/src/graphics/battle_fx/blue_beam_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/blue_beam.parts.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleEffect_WindStreakSheet+0x6b5
	.global BattleEffect_VortexSheet
BattleEffect_VortexSheet:
	.incbin "games/tbs/src/graphics/battle_fx/vortex_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/vortex.parts.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleEffect_WindStreakSheet+0x912
	.global BattleEffect_TornadoSheet
BattleEffect_TornadoSheet:
	.incbin "games/tbs/src/graphics/battle_fx/tornado_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/tornado.parts.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x1b32
	.global BattleEffect_CrystalSheet
BattleEffect_CrystalSheet:
	.incbin "games/tbs/src/graphics/battle_fx/crystal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/crystal.parts.lz"
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleEffect_TornadoSheet+0x104d
	.global BattleEffect_WaterSparkleSheet
BattleEffect_WaterSparkleSheet:
	.incbin "games/tbs/src/graphics/battle_fx/water_sparkle.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/water_sparkle.bitmap.lz"
	.byte 0x11	@ PACKER: left in the buffer by BattleEffect_CrystalSheet+0x6b3
	.balign 4, 0
	.include "games/tbs/src/graphics/battle_fx/lightning_pillar.s"
	.include "games/tbs/src/graphics/battle_fx/staged_particle_path.s"
	.include "games/tbs/src/graphics/battle_fx/dual_stream_path.s"
