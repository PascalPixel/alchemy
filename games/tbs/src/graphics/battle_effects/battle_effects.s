	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.global BattleEffect_TileAnimationMask
BattleEffect_TileAnimationMask:
	.incbin "games/tbs/src/graphics/battle_effects/tile_mask_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/tile_mask.bitmaps.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x32, 0xb9, 0x81	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.ifdef TBS_EDITION_JA
	.byte 0xad, 0x1e, 0xd8	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.ifdef TBS_EDITION_ES
	.byte 0xc6, 0xaf, 0xea	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.ifdef TBS_EDITION_IT
	.byte 0xc7, 0x7f, 0xea	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.ifdef TBS_EDITION_FR
	.byte 0xea, 0x01, 0x59	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.byte 0x1b, 0x90, 0x2f	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.endif
	.endif
	.endif
	.endif
	.endif
	.global BattleEffect_CyanSparkSheet
BattleEffect_CyanSparkSheet:
	.incbin "games/tbs/src/graphics/battle_effects/cyan_spark_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/cyan_spark.bitmaps.lz"
	.global BattleEffect_VioletPaletteA
BattleEffect_VioletPaletteA:
	.incbin "games/tbs/src/graphics/battle_effects/violet_palette_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/violet_palette_a.bitmap.lz"
	.global BattleEffect_RedPaletteA
BattleEffect_RedPaletteA:
	.incbin "games/tbs/src/graphics/battle_effects/red_palette_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/red_palette_a.bitmap.lz"
	.global BattleEffect_YellowPaletteA
BattleEffect_YellowPaletteA:
	.incbin "games/tbs/src/graphics/battle_effects/yellow_palette_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/yellow_palette_a.bitmap.lz"
	.global BattleEffect_StarBurstSheet
BattleEffect_StarBurstSheet:
	.incbin "games/tbs/src/graphics/battle_effects/star_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/star_burst.bitmap.lz"
	.byte 0xa1	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x487
	.global BattleEffect_CrescentSheet
BattleEffect_CrescentSheet:
	.incbin "games/tbs/src/graphics/battle_effects/crescent.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/crescent.bitmap.lz"
	.byte 0x09, 0x01	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x5ea
	.global BattleEffect_RevealMaskA
BattleEffect_RevealMaskA:
	.incbin "games/tbs/src/graphics/battle_effects/reveal_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/reveal_a.bitmap.lz"
	.byte 0x0a, 0x21	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x6c2
	.global BattleEffect_RevealMaskB
BattleEffect_RevealMaskB:
	.incbin "games/tbs/src/graphics/battle_effects/reveal_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/reveal_b.bitmap.lz"
	.byte 0x00, 0x7b, 0x09	@ PACKER: left in the buffer by BattleEffect_RevealMaskA+0x2e5
	.global BattleEffect_RevealMaskC
BattleEffect_RevealMaskC:
	.incbin "games/tbs/src/graphics/battle_effects/reveal_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/reveal_c.bitmap.lz"
	.global BattleEffect_RevealMaskD
BattleEffect_RevealMaskD:
	.incbin "games/tbs/src/graphics/battle_effects/reveal_d.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/reveal_d.bitmap.lz"
	.balign 4, 0
	.global BattleEffect_RevealMaskE
BattleEffect_RevealMaskE:
	.incbin "games/tbs/src/graphics/battle_effects/reveal_e.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/reveal_e.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_RevealMaskF
BattleEffect_RevealMaskF:
	.incbin "games/tbs/src/graphics/battle_effects/reveal_f.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/reveal_f.bitmap.lz"
	.byte 0x30, 0xe1, 0x08	@ PACKER: left in the buffer by BattleEffect_RevealMaskE+0x3e9
	.global BattleEffect_LimeColumnSheet
BattleEffect_LimeColumnSheet:
	.incbin "games/tbs/src/graphics/battle_effects/lime_column.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/lime_column.bitmap.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleEffect_RevealMaskF+0x3cf
	.global BattleEffect_RainSheet
BattleEffect_RainSheet:
	.incbin "games/tbs/src/graphics/battle_effects/rain.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/rain.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x89f
	.global BattleEffect_FlameSheetA
BattleEffect_FlameSheetA:
	.incbin "games/tbs/src/graphics/battle_effects/flame_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/flame_a.bitmap.lz"
	.byte 0x2f, 0x18	@ PACKER: left in the buffer by BattleEffect_RainSheet+0x4a2
	.global BattleEffect_DemonFaceSheet
BattleEffect_DemonFaceSheet:
	.incbin "games/tbs/src/graphics/battle_effects/demon.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/demon.bitmap.lz"
	.global BattleEffect_FlameBladeSheet
BattleEffect_FlameBladeSheet:
	.incbin "games/tbs/src/graphics/battle_effects/flame_bld.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/flame_bld.bitmap.lz"
	.byte 0xb7	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0xc83
	.global BattleEffect_SwordSheet
BattleEffect_SwordSheet:
	.incbin "games/tbs/src/graphics/battle_effects/sword.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/sword.bitmap.lz"
	.global BattleEffect_IceTileSheet
BattleEffect_IceTileSheet:
	.incbin "games/tbs/src/graphics/battle_effects/ice_tile_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/ice_tile.bitmaps.lz"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0x106a
	.global BattleEffect_FiveModeImage
BattleEffect_FiveModeImage:
	.incbin "games/tbs/src/graphics/battle_effects/five_mode.bitmap.lz"
	.byte 0x96	@ PACKER: left in the buffer by BattleEffect_IceTileSheet+0xfc7
	.global BattleEffect_SpiderWebSheet
BattleEffect_SpiderWebSheet:
	.incbin "games/tbs/src/graphics/battle_effects/web.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/web.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleEffect_FiveModeImage+0x25a
	.global BattleEffect_VioletLightningSheetA
BattleEffect_VioletLightningSheetA:
	.incbin "games/tbs/src/graphics/battle_effects/bolt_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/bolt_a.bitmap.lz"
	.global BattleEffect_YellowSparkSheet
BattleEffect_YellowSparkSheet:
	.incbin "games/tbs/src/graphics/battle_effects/yellow_spark_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/yellow_spark.bitmaps.lz"
	.byte 0x17, 0x6e, 0xa0	@ PACKER: left in the buffer by BattleEffect_VioletLightningSheetA+0x2fd
	.global BattleEffect_ProjectileVolleyImage
BattleEffect_ProjectileVolleyImage:
	.incbin "games/tbs/src/graphics/battle_effects/projectile_volley.bitmap.lz"
	.global BattleEffect_ShurikenSheet
BattleEffect_ShurikenSheet:
	.incbin "games/tbs/src/graphics/battle_effects/shuriken.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/shuriken.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleEffect_VioletLightningSheetA+0x395
	.global BattleEffect_FlareSheet
BattleEffect_FlareSheet:
	.incbin "games/tbs/src/graphics/battle_effects/flare_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/flare.bitmaps.lz"
	.ifdef TBS_EDITION_ES
	.byte 0x10, 0xb6	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.ifdef TBS_EDITION_JA
	.byte 0x80, 0x47	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.ifdef TBS_EDITION_EN
	.byte 0x90, 0x38	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xc5, 0x01	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.ifdef TBS_EDITION_IT
	.byte 0xd1, 0x0a	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.byte 0x0a, 0x16	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.endif
	.endif
	.endif
	.endif
	.endif
	.global BattleEffect_FlareImage
BattleEffect_FlareImage:
	.incbin "games/tbs/src/graphics/battle_effects/flare_cels.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x277
	.global BattleEffect_VioletPaletteB
BattleEffect_VioletPaletteB:
	.incbin "games/tbs/src/graphics/battle_effects/violet_palette_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/violet_palette_b.bitmap.lz"
	.global BattleEffect_FireSwirlSheet
BattleEffect_FireSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_effects/fire_swirl_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/fire_swirl.bitmaps.lz"
	.byte 0x12, 0x21	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x442
	.global BattleEffect_VioletLightningSheetB
BattleEffect_VioletLightningSheetB:
	.incbin "games/tbs/src/graphics/battle_effects/bolt_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/bolt_b.bitmap.lz"
	.byte 0xe5	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x1bd3
	.balign 4, 0
	.global BattleEffect_FireBurstSheet
BattleEffect_FireBurstSheet:
	.incbin "games/tbs/src/graphics/battle_effects/fire_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/fire_burst.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_BlueCrescentSheet
BattleEffect_BlueCrescentSheet:
	.incbin "games/tbs/src/graphics/battle_effects/blue_crescent.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_crescent.bitmap.lz"
	.byte 0xe0, 0x02	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x21e
	.global BattleEffect_OverlayImageA
BattleEffect_OverlayImageA:
	.incbin "games/tbs/src/graphics/battle_effects/overlay_a.bitmaps.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x43b
	.global BattleEffect_VioletMoteSheet
BattleEffect_VioletMoteSheet:
	.incbin "games/tbs/src/graphics/battle_effects/mote.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/mote.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleEffect_OverlayImageA+0x111
	.global BattleEffect_OverlayImageB
BattleEffect_OverlayImageB:
	.incbin "games/tbs/src/graphics/battle_effects/overlay_b.bitmaps.lz"
	.global BattleEffect_FireballSheet
BattleEffect_FireballSheet:
	.incbin "games/tbs/src/graphics/battle_effects/fireball.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x5df
	.global BattleEffect_MemberBurstImage
BattleEffect_MemberBurstImage:
	.incbin "games/tbs/src/graphics/battle_effects/member_burst.bitmap.lz"
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleEffect_FireballSheet+0x49d
	.global BattleEffect_SlashSheet
BattleEffect_SlashSheet:
	.incbin "games/tbs/src/graphics/battle_effects/slash_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/slash.bitmaps.lz"
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0xd12
	.global BattleEffect_WaveSheet
BattleEffect_WaveSheet:
	.incbin "games/tbs/src/graphics/battle_effects/wave.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/wave.bitmap.lz"
	.global BattleEffect_TargetBurstImage
BattleEffect_TargetBurstImage:
	.incbin "games/tbs/src/graphics/battle_effects/target_burst.bitmap.lz"
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleEffect_SlashSheet+0x83e
	.global BattleEffect_PinkStarSheet
BattleEffect_PinkStarSheet:
	.incbin "games/tbs/src/graphics/battle_effects/pink_star.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/pink_star.bitmap.lz"
	.byte 0x80	@ PACKER: left in the buffer by BattleEffect_TargetBurstImage+0x38f
	.global BattleEffect_WaterSpraySheet
BattleEffect_WaterSpraySheet:
	.incbin "games/tbs/src/graphics/battle_effects/water_spray_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/water_spray.bitmaps.lz"
	.global BattleEffect_BlueFlameSheet
BattleEffect_BlueFlameSheet:
	.incbin "games/tbs/src/graphics/battle_effects/blue_flame_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_flame.bitmaps.lz"
	.byte 0xb7, 0x7c	@ PACKER: left in the buffer by BattleEffect_WaterSpraySheet+0xaee
	.global BattleEffect_ExplosionSheet
BattleEffect_ExplosionSheet:
	.incbin "games/tbs/src/graphics/battle_effects/explosion.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/explosion.bitmap.lz"
	.byte 0xec, 0x10, 0x3c	@ PACKER: left in the buffer by BattleEffect_WaterSpraySheet+0xdd5
	.global BattleEffect_MagentaSwirlSheet
BattleEffect_MagentaSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_effects/magenta_swirl.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/magenta_swirl.bitmap.lz"
	.byte 0x1c, 0x4c	@ PACKER: left in the buffer by BattleEffect_ExplosionSheet+0x82a
	.global BattleEffect_MagentaTailSheet
BattleEffect_MagentaTailSheet:
	.incbin "games/tbs/src/graphics/battle_effects/magenta_tail.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/magenta_tail.bitmap.lz"
	.global BattleEffect_ParticleSpritesA
BattleEffect_ParticleSpritesA:
	.incbin "games/tbs/src/graphics/battle_effects/dot_cells_a.bitmaps.lz"
	.byte 0x0c, 0x40, 0x08	@ PACKER: left in the buffer by BattleEffect_MagentaTailSheet+0x299
	.global BattleEffect_ParticleSpritesB
BattleEffect_ParticleSpritesB:
	.incbin "games/tbs/src/graphics/battle_effects/dot_cells_b.bitmaps.lz"
	.byte 0x94	@ PACKER: left in the buffer by BattleEffect_ParticleSpritesA+0x1bb
	.global BattleEffect_ParticleSpritesC
BattleEffect_ParticleSpritesC:
	.incbin "games/tbs/src/graphics/battle_effects/dot_cells_c.bitmaps.lz"
	.global BattleEffect_ParticleSpritesD
BattleEffect_ParticleSpritesD:
	.incbin "games/tbs/src/graphics/battle_effects/dot_cells_d.bitmaps.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleEffect_ParticleSpritesC+0x417
	.global BattleEffect_GreenPalette
BattleEffect_GreenPalette:
	.incbin "games/tbs/src/graphics/battle_effects/green_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/green_palette.bitmap.lz"
	.global BattleEffect_SwordSlashSheet
BattleEffect_SwordSlashSheet:
	.incbin "games/tbs/src/graphics/battle_effects/sword_slash_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/sword_slash.bitmaps.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleEffect_ParticleSpritesC+0x4c7
	.global BattleEffect_RuneSheet
BattleEffect_RuneSheet:
	.incbin "games/tbs/src/graphics/battle_effects/rune.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/rune.bitmap.lz"
	.byte 0x43, 0x21, 0xbe	@ PACKER: left in the buffer by BattleEffect_SwordSlashSheet+0x3d9
	.global BattleEffect_FlameColumnSheet
BattleEffect_FlameColumnSheet:
	.incbin "games/tbs/src/graphics/battle_effects/flame_column.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/flame_column.bitmap.lz"
	.byte 0x41	@ PACKER: left in the buffer by BattleEffect_RuneSheet+0x3ab
	.global BattleEffect_EarthWallSheet
BattleEffect_EarthWallSheet:
	.incbin "games/tbs/src/graphics/battle_effects/earth_wall_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/earth_wall.bitmaps.lz"
	.global BattleEffect_OrangePaletteA
BattleEffect_OrangePaletteA:
	.incbin "games/tbs/src/graphics/battle_effects/orange_palette_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/orange_palette_a.bitmap.lz"
	.global BattleEffect_FlashBurstSheet
BattleEffect_FlashBurstSheet:
	.incbin "games/tbs/src/graphics/battle_effects/flash_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/flash_burst.bitmap.lz"
	.byte 0x60	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x144b
	.global BattleEffect_ThornSheet
BattleEffect_ThornSheet:
	.incbin "games/tbs/src/graphics/battle_effects/thorn_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/thorn.bitmaps.lz"
	.byte 0x78	@ PACKER: left in the buffer by BattleEffect_FlashBurstSheet+0xd9b
	.global BattleEffect_CometSheetA
BattleEffect_CometSheetA:
	.incbin "games/tbs/src/graphics/battle_effects/comet_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/comet_a.bitmap.lz"
	.byte 0x2a	@ PACKER: left in the buffer by BattleEffect_ThornSheet+0x21b
	.global BattleEffect_CometSheetB
BattleEffect_CometSheetB:
	.incbin "games/tbs/src/graphics/battle_effects/comet_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/comet_b.bitmap.lz"
	.byte 0xaf	@ PACKER: left in the buffer by BattleEffect_ThornSheet+0x2fb
	.global BattleEffect_CometSheetC
BattleEffect_CometSheetC:
	.incbin "games/tbs/src/graphics/battle_effects/comet_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/comet_c.bitmap.lz"
	.global BattleEffect_WhirlwindSheet
BattleEffect_WhirlwindSheet:
	.incbin "games/tbs/src/graphics/battle_effects/whirlwind_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/whirlwind.bitmaps.lz"
	.global BattleEffect_GreenPillarSheet
BattleEffect_GreenPillarSheet:
	.incbin "games/tbs/src/graphics/battle_effects/green_pillar_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/green_pillar.bitmaps.lz"
	.byte 0x21, 0xf0, 0x28	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0x75d
	.global BattleEffect_VineSheet
BattleEffect_VineSheet:
	.incbin "games/tbs/src/graphics/battle_effects/vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/vine.bitmaps.lz"
	.byte 0xbe	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0xdcb
	.global BattleEffect_FirePillarSheetA
BattleEffect_FirePillarSheetA:
	.incbin "games/tbs/src/graphics/battle_effects/fire_pillar_a_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/fire_pillar_a.bitmaps.lz"
	.global BattleEffect_RedPaletteB
BattleEffect_RedPaletteB:
	.incbin "games/tbs/src/graphics/battle_effects/red_palette_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/red_palette_b.bitmap.lz"
	.global BattleEffect_OrangePaletteB
BattleEffect_OrangePaletteB:
	.incbin "games/tbs/src/graphics/battle_effects/orange_palette_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/orange_palette_b.bitmap.lz"
	.global BattleEffect_VioletCometSheet
BattleEffect_VioletCometSheet:
	.incbin "games/tbs/src/graphics/battle_effects/violet_comet_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/violet_comet.bitmaps.lz"
	.byte 0x5e, 0xda, 0x33	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetA+0x3e5
	.global BattleEffect_FlameballSheet
BattleEffect_FlameballSheet:
	.incbin "games/tbs/src/graphics/battle_effects/flameball.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/flameball.bitmap.lz"
	.byte 0x83	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetA+0x557
	.global BattleEffect_BoulderSheet
BattleEffect_BoulderSheet:
	.incbin "games/tbs/src/graphics/battle_effects/boulder_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/boulder.bitmaps.lz"
	.global BattleEffect_RockWallSheet
BattleEffect_RockWallSheet:
	.incbin "games/tbs/src/graphics/battle_effects/rock_wall.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/rock_wall.bitmap.lz"
	.byte 0x07, 0x43, 0x0b	@ PACKER: left in the buffer by BattleEffect_VineSheet+0xcd1
	.global BattleEffect_StarDotSheet
BattleEffect_StarDotSheet:
	.incbin "games/tbs/src/graphics/battle_effects/star_dot_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/star_dot.bitmaps.lz"
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleEffect_RockWallSheet+0x89e
	.global BattleEffect_MarsDjinnSheet
BattleEffect_MarsDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_effects/mars_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/mars_djinn.bitmap.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleEffect_StarDotSheet+0x3db
	.global BattleEffect_MarsDjinnSmallSheet
BattleEffect_MarsDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_effects/mars_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/mars_djinn_small.bitmap.lz"
	.byte 0x8a, 0x14, 0x39	@ PACKER: left in the buffer by BattleEffect_MarsDjinnSheet+0x289
	.global BattleEffect_JupiterDjinnSheet
BattleEffect_JupiterDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_effects/jupiter_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/jupiter_djinn.bitmap.lz"
	.byte 0x13	@ PACKER: left in the buffer by BattleEffect_MarsDjinnSheet+0x3a3
	.global BattleEffect_JupiterDjinnSmallSheet
BattleEffect_JupiterDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_effects/jupiter_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/jupiter_djinn_small.bitmap.lz"
	.byte 0x0b, 0x1f	@ PACKER: left in the buffer by BattleEffect_JupiterDjinnSheet+0x25a
	.balign 4, 0
	.global BattleEffect_MercuryDjinnSheet
BattleEffect_MercuryDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_effects/mercury_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/mercury_djinn.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_MercuryDjinnSmallSheet
BattleEffect_MercuryDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_effects/mercury_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/mercury_djinn_small.bitmap.lz"
	.global BattleEffect_VenusDjinnSheet
BattleEffect_VenusDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_effects/venus_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/venus_djinn.bitmap.lz"
	.global BattleEffect_VenusDjinnSmallSheet
BattleEffect_VenusDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_effects/venus_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/venus_djinn_small.bitmap.lz"
	.global BattleEffect_FireBlobSheet
BattleEffect_FireBlobSheet:
	.incbin "games/tbs/src/graphics/battle_effects/fire_blob.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/fire_blob.bitmap.lz"
	.byte 0x1a	@ PACKER: left in the buffer by BattleEffect_RockWallSheet+0xb87
	.global BattleEffect_FireStreakSheet
BattleEffect_FireStreakSheet:
	.incbin "games/tbs/src/graphics/battle_effects/fire_streak.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/fire_streak.bitmap.lz"
	.byte 0x00, 0x48	@ PACKER: left in the buffer by BattleEffect_WhirlwindSheet+0x15c2
	.global BattleEffect_LavaOrbSheet
BattleEffect_LavaOrbSheet:
	.incbin "games/tbs/src/graphics/battle_effects/lava_orb_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/lava_orb.bitmaps.lz"
	.global BattleEffect_YellowRingSheet
BattleEffect_YellowRingSheet:
	.incbin "games/tbs/src/graphics/battle_effects/yellow_ring.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/yellow_ring.bitmap.lz"
	.global BattleEffect_YellowOrbSheet
BattleEffect_YellowOrbSheet:
	.incbin "games/tbs/src/graphics/battle_effects/yellow_orb.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/yellow_orb.bitmap.lz"
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by BattleEffect_LavaOrbSheet+0xa12
	.global BattleEffect_BubbleSheet
BattleEffect_BubbleSheet:
	.incbin "games/tbs/src/graphics/battle_effects/bubble.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/bubble.bitmap.lz"
	.byte 0x14, 0xb1, 0x50	@ PACKER: left in the buffer by BattleEffect_YellowOrbSheet+0x381
	.global BattleEffect_ShieldSheet
BattleEffect_ShieldSheet:
	.incbin "games/tbs/src/graphics/battle_effects/shield.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/shield.bitmap.lz"
	.byte 0x45, 0xb1, 0x12	@ PACKER: left in the buffer by BattleEffect_FireStreakSheet+0xcad
	.global BattleEffect_DaggerSheet
BattleEffect_DaggerSheet:
	.incbin "games/tbs/src/graphics/battle_effects/dagger.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/dagger.bitmap.lz"
	.byte 0x0a, 0x77, 0x19	@ PACKER: left in the buffer by BattleEffect_ShieldSheet+0xb45
	.global BattleEffect_PaletteRampImage
BattleEffect_PaletteRampImage:
	.incbin "games/tbs/src/graphics/battle_effects/spark_cells.bitmaps.lz"
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleEffect_DaggerSheet+0x196
	.global BattleEffect_SmokeSheet
BattleEffect_SmokeSheet:
	.incbin "games/tbs/src/graphics/battle_effects/smoke.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/smoke.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleEffect_DaggerSheet+0x88b
	.global BattleEffect_BluePalette
BattleEffect_BluePalette:
	.incbin "games/tbs/src/graphics/battle_effects/blue_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_palette.bitmap.lz"
	.global BattleEffect_LimePalette
BattleEffect_LimePalette:
	.incbin "games/tbs/src/graphics/battle_effects/lime_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/lime_palette.bitmap.lz"
	.global BattleEffect_PinkPalette
BattleEffect_PinkPalette:
	.incbin "games/tbs/src/graphics/battle_effects/pink_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/pink_palette.bitmap.lz"
	.global BattleEffect_VioletPaletteC
BattleEffect_VioletPaletteC:
	.incbin "games/tbs/src/graphics/battle_effects/violet_palette_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/violet_palette_c.bitmap.lz"
	.global BattleEffect_CyanPalette
BattleEffect_CyanPalette:
	.incbin "games/tbs/src/graphics/battle_effects/cyan_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/cyan_palette.bitmap.lz"
	.global BattleEffect_TanPalette
BattleEffect_TanPalette:
	.incbin "games/tbs/src/graphics/battle_effects/tan_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/tan_palette.bitmap.lz"
	.global BattleEffect_WispSheet
BattleEffect_WispSheet:
	.incbin "games/tbs/src/graphics/battle_effects/wisp.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/wisp.bitmap.lz"
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleEffect_SmokeSheet+0x7ae
	.global BattleEffect_GreenVineSheet
BattleEffect_GreenVineSheet:
	.incbin "games/tbs/src/graphics/battle_effects/green_vine.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/green_vine.bitmap.lz"
	.byte 0x1c, 0xaf	@ PACKER: left in the buffer by BattleEffect_WispSheet+0x516
	.global BattleEffect_PortalSheet
BattleEffect_PortalSheet:
	.incbin "games/tbs/src/graphics/battle_effects/portal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/portal.bitmaps.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleEffect_WispSheet+0x622
	.global BattleEffect_SheepSheet
BattleEffect_SheepSheet:
	.incbin "games/tbs/src/graphics/battle_effects/sheep.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/sheep.bitmap.lz"
	.byte 0x13, 0x4e, 0x03	@ PACKER: left in the buffer by BattleEffect_SmokeSheet+0x865
	.global BattleEffect_SkullSheet
BattleEffect_SkullSheet:
	.incbin "games/tbs/src/graphics/battle_effects/skull.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/skull.bitmap.lz"
	.byte 0x98, 0x43, 0x4a	@ PACKER: left in the buffer by BattleEffect_SheepSheet+0x421
	.global BattleEffect_HeartSheet
BattleEffect_HeartSheet:
	.incbin "games/tbs/src/graphics/battle_effects/heart.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/heart.bitmap.lz"
	.byte 0x09	@ PACKER: left in the buffer by BattleEffect_SkullSheet+0x297
	.global BattleEffect_CounterRevealSheet
BattleEffect_CounterRevealSheet:
	.incbin "games/tbs/src/graphics/battle_effects/counter_reveal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/counter_reveal.bitmaps.lz"
	.global BattleEffect_RedCrescentSheetA
BattleEffect_RedCrescentSheetA:
	.incbin "games/tbs/src/graphics/battle_effects/red_crescent_a_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/red_crescent_a.bitmaps.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleEffect_CounterRevealSheet+0x49b
	.global BattleEffect_BlueBeastSheet
BattleEffect_BlueBeastSheet:
	.incbin "games/tbs/src/graphics/battle_effects/blue_beast_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_beast.bitmaps.lz"
	.global BattleEffect_RedCrescentSheetB
BattleEffect_RedCrescentSheetB:
	.incbin "games/tbs/src/graphics/battle_effects/red_crescent_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/red_crescent_b.bitmap.lz"
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleEffect_BlueBeastSheet+0x3ea
	.global BattleEffect_SpiralSheet
BattleEffect_SpiralSheet:
	.incbin "games/tbs/src/graphics/battle_effects/spiral.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/spiral.bitmap.lz"
	.global BattleEffect_VioletStreakSheet
BattleEffect_VioletStreakSheet:
	.incbin "games/tbs/src/graphics/battle_effects/violet_streak.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/violet_streak.bitmap.lz"
	.byte 0x5b	@ PACKER: left in the buffer by BattleEffect_SpiralSheet+0xc8b
	.global BattleEffect_CrescentMoonSheet
BattleEffect_CrescentMoonSheet:
	.incbin "games/tbs/src/graphics/battle_effects/crescent_moon.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/crescent_moon.bitmap.lz"
	.byte 0x2b	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0x4cf
	.global BattleEffect_ThornVineSheet
BattleEffect_ThornVineSheet:
	.incbin "games/tbs/src/graphics/battle_effects/thorn_vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/thorn_vine.bitmaps.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0x691
	.global BattleEffect_IceShardSheet
BattleEffect_IceShardSheet:
	.incbin "games/tbs/src/graphics/battle_effects/ice_shard_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/ice_shard.bitmaps.lz"
	.byte 0x3e	@ PACKER: left in the buffer by BattleEffect_VioletStreakSheet+0xa33
	.global BattleEffect_EmberStreakSheet
BattleEffect_EmberStreakSheet:
	.incbin "games/tbs/src/graphics/battle_effects/ember_streak_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/ember_streak.bitmaps.lz"
	.global BattleEffect_BlueArcSheetA
BattleEffect_BlueArcSheetA:
	.incbin "games/tbs/src/graphics/battle_effects/blue_arc_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_arc_a.bitmap.lz"
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0x7d5
	.global BattleEffect_BlueArcSheetB
BattleEffect_BlueArcSheetB:
	.incbin "games/tbs/src/graphics/battle_effects/blue_arc_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_arc_b.bitmap.lz"
	.byte 0xaa, 0x30	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0xace
	.global BattleEffect_GlowOrbSheet
BattleEffect_GlowOrbSheet:
	.incbin "games/tbs/src/graphics/battle_effects/glow_orb.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/glow_orb.bitmap.lz"
	.byte 0xe0	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0xb3b
	.global BattleEffect_IceChipSheet
BattleEffect_IceChipSheet:
	.incbin "games/tbs/src/graphics/battle_effects/ice_chip_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/ice_chip.bitmaps.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleEffect_GlowOrbSheet+0x63f
	.global BattleEffect_IceBlockSheet
BattleEffect_IceBlockSheet:
	.incbin "games/tbs/src/graphics/battle_effects/ice_block_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/ice_block.bitmaps.lz"
	.byte 0x30, 0xae	@ PACKER: left in the buffer by BattleEffect_BlueBeastSheet+0x1586
	.global BattleEffect_SparkleDots
BattleEffect_SparkleDots:
	.incbin "games/tbs/src/graphics/battle_effects/sparkle_dots.bitmaps.lz"
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x61
	.global BattleEffect_PinkBurstSheet
BattleEffect_PinkBurstSheet:
	.incbin "games/tbs/src/graphics/battle_effects/pink_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/pink_burst.bitmap.lz"
	.byte 0x57	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x36b
	.global BattleEffect_LightFanSheet
BattleEffect_LightFanSheet:
	.incbin "games/tbs/src/graphics/battle_effects/light_fan.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/light_fan.bitmap.lz"
	.byte 0x09, 0x82, 0x76	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x59d
	.global BattleEffect_BlueBurstSheet
BattleEffect_BlueBurstSheet:
	.incbin "games/tbs/src/graphics/battle_effects/blue_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_burst.bitmap.lz"
	.byte 0xc4, 0x99, 0xd3	@ PACKER: left in the buffer by BattleEffect_LightFanSheet+0x331
	.balign 4, 0
	.global BattleEffect_DustPuffSheet
BattleEffect_DustPuffSheet:
	.incbin "games/tbs/src/graphics/battle_effects/dust_puff.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/dust_puff.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleEffect_GoldShellSheet
BattleEffect_GoldShellSheet:
	.incbin "games/tbs/src/graphics/battle_effects/gold_shell_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/gold_shell.bitmaps.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x1016
	.global BattleEffect_BlastSheet
BattleEffect_BlastSheet:
	.incbin "games/tbs/src/graphics/battle_effects/blast_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blast.bitmaps.lz"
	.global BattleEffect_FlameSheetB
BattleEffect_FlameSheetB:
	.incbin "games/tbs/src/graphics/battle_effects/flame_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/flame_b.bitmap.lz"
	.global BattleEffect_FirePillarSheetB
BattleEffect_FirePillarSheetB:
	.incbin "games/tbs/src/graphics/battle_effects/fire_pillar_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/fire_pillar_b.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleEffect_BlastSheet+0x11fe
	.global BattleEffect_RockSpireSheet
BattleEffect_RockSpireSheet:
	.incbin "games/tbs/src/graphics/battle_effects/rock_spire.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/rock_spire.bitmap.lz"
	.byte 0xa0, 0x20, 0x72	@ PACKER: left in the buffer by BattleEffect_FirePillarSheetB+0x7a1
	.global BattleEffect_LightningBoltSheet
BattleEffect_LightningBoltSheet:
	.incbin "games/tbs/src/graphics/battle_effects/lightning_bolt.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/lightning_bolt.bitmap.lz"
	.global BattleEffect_BeamImage
BattleEffect_BeamImage:
	.incbin "games/tbs/src/graphics/battle_effects/beam_image.bitmaps.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x37b
	.global BattleEffect_TwelveModeImage
BattleEffect_TwelveModeImage:
	.incbin "games/tbs/src/graphics/battle_effects/twelve_mode.bitmap.lz"
	.byte 0xa5	@ PACKER: left in the buffer by BattleEffect_LightningBoltSheet+0x42f
	.global BattleEffect_WindStreakSheet
BattleEffect_WindStreakSheet:
	.incbin "games/tbs/src/graphics/battle_effects/wind_streak_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/wind_streak.bitmaps.lz"
	.global BattleEffect_YellowPaletteB
BattleEffect_YellowPaletteB:
	.incbin "games/tbs/src/graphics/battle_effects/yellow_palette_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/yellow_palette_b.bitmap.lz"
	.global BattleEffect_BlueRingSheet
BattleEffect_BlueRingSheet:
	.incbin "games/tbs/src/graphics/battle_effects/blue_ring_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_ring.bitmaps.lz"
	.global BattleEffect_RedPaletteC
BattleEffect_RedPaletteC:
	.incbin "games/tbs/src/graphics/battle_effects/red_palette_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/red_palette_c.bitmap.lz"
	.global BattleEffect_VioletPaletteD
BattleEffect_VioletPaletteD:
	.incbin "games/tbs/src/graphics/battle_effects/violet_palette_d.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/violet_palette_d.bitmap.lz"
	.global BattleEffect_BlueBeamSheet
BattleEffect_BlueBeamSheet:
	.incbin "games/tbs/src/graphics/battle_effects/blue_beam_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/blue_beam.bitmaps.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleEffect_WindStreakSheet+0x6b5
	.global BattleEffect_VortexSheet
BattleEffect_VortexSheet:
	.incbin "games/tbs/src/graphics/battle_effects/vortex_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/vortex.bitmaps.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleEffect_WindStreakSheet+0x912
	.global BattleEffect_TornadoSheet
BattleEffect_TornadoSheet:
	.incbin "games/tbs/src/graphics/battle_effects/tornado_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/tornado.bitmaps.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x1b32
	.global BattleEffect_CrystalSheet
BattleEffect_CrystalSheet:
	.incbin "games/tbs/src/graphics/battle_effects/crystal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/crystal.bitmaps.lz"
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleEffect_TornadoSheet+0x104d
	.global BattleEffect_WaterSparkleSheet
BattleEffect_WaterSparkleSheet:
	.incbin "games/tbs/src/graphics/battle_effects/water_sparkle.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/water_sparkle.bitmap.lz"
	.byte 0x11	@ PACKER: left in the buffer by BattleEffect_CrystalSheet+0x6b3
	.balign 4, 0
	.global BattleEffect_LightningPillarSheet
BattleEffect_LightningPillarSheet:
	.incbin "games/tbs/src/graphics/battle_effects/lightning_pillar.gbapal"
	.incbin "games/tbs/src/graphics/battle_effects/lightning_pillar.bitmap.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
