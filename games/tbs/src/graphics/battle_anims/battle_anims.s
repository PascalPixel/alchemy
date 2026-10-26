	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.global BattleAnim_TileAnimationMask
BattleAnim_TileAnimationMask:
	.incbin "games/tbs/src/graphics/battle_anims/tile_mask_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/tile_mask.parts.lz"
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
	.global BattleAnim_CyanSparkSheet
BattleAnim_CyanSparkSheet:
	.incbin "games/tbs/src/graphics/battle_anims/cyan_spark_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/cyan_spark.parts.lz"
	.global BattleAnim_VioletPaletteA
BattleAnim_VioletPaletteA:
	.incbin "games/tbs/src/graphics/battle_anims/violet_palette_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/violet_palette_a.bitmap.lz"
	.global BattleAnim_RedPaletteA
BattleAnim_RedPaletteA:
	.incbin "games/tbs/src/graphics/battle_anims/red_palette_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/red_palette_a.bitmap.lz"
	.global BattleAnim_YellowPaletteA
BattleAnim_YellowPaletteA:
	.incbin "games/tbs/src/graphics/battle_anims/yellow_palette_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/yellow_palette_a.bitmap.lz"
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
	.global BattleAnim_RevealMaskB
BattleAnim_RevealMaskB:
	.incbin "games/tbs/src/graphics/battle_anims/reveal_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/reveal_b.bitmap.lz"
	.byte 0x00, 0x7b, 0x09	@ PACKER: left in the buffer by BattleAnim_RevealMaskA+0x2e5
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
	.global BattleAnim_LimeColumnSheet
BattleAnim_LimeColumnSheet:
	.incbin "games/tbs/src/graphics/battle_anims/lime_column.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/lime_column.bitmap.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleAnim_RevealMaskF+0x3cf
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
	.global BattleAnim_SwordSheet
BattleAnim_SwordSheet:
	.incbin "games/tbs/src/graphics/battle_anims/sword.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/sword.bitmap.lz"
	.global BattleAnim_IceTileSheet
BattleAnim_IceTileSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ice_tile_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ice_tile.parts.lz"
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
	.global BattleAnim_YellowSparkSheet
BattleAnim_YellowSparkSheet:
	.incbin "games/tbs/src/graphics/battle_anims/yellow_spark_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/yellow_spark.parts.lz"
	.byte 0x17, 0x6e, 0xa0	@ PACKER: left in the buffer by BattleAnim_VioletLightningSheetA+0x2fd
	.global BattleAnim_ProjectileVolleyImage
BattleAnim_ProjectileVolleyImage:
	.incbin "games/tbs/src/graphics/battle_anims/projectile_volley.bitmap.lz"
	.global BattleAnim_ShurikenSheet
BattleAnim_ShurikenSheet:
	.incbin "games/tbs/src/graphics/battle_anims/shuriken.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/shuriken.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleAnim_VioletLightningSheetA+0x395
	.global BattleAnim_FlareSheet
BattleAnim_FlareSheet:
	.incbin "games/tbs/src/graphics/battle_anims/flare_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flare.parts.lz"
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
	.global BattleAnim_FlareImage
BattleAnim_FlareImage:
	.incbin "games/tbs/src/graphics/battle_anims/flare_cels.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleAnim_FlareSheet+0x277
	.global BattleAnim_VioletPaletteB
BattleAnim_VioletPaletteB:
	.incbin "games/tbs/src/graphics/battle_anims/violet_palette_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/violet_palette_b.bitmap.lz"
	.global BattleAnim_FireSwirlSheet
BattleAnim_FireSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fire_swirl_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_swirl.parts.lz"
	.byte 0x12, 0x21	@ PACKER: left in the buffer by BattleAnim_FlareSheet+0x442
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
	.global BattleAnim_BlueCrescentSheet
BattleAnim_BlueCrescentSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_crescent.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_crescent.bitmap.lz"
	.byte 0xe0, 0x02	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x21e
	.global BattleAnim_OverlayImageA
BattleAnim_OverlayImageA:
	.incbin "games/tbs/src/graphics/battle_anims/overlay_a.parts.lz"
	.byte 0x88	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x43b
	.global BattleAnim_VioletMoteSheet
BattleAnim_VioletMoteSheet:
	.incbin "games/tbs/src/graphics/battle_anims/mote.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/mote.bitmap.lz"
	.byte 0x28, 0x41, 0x42	@ PACKER: left in the buffer by BattleAnim_OverlayImageA+0x111
	.global BattleAnim_OverlayImageB
BattleAnim_OverlayImageB:
	.incbin "games/tbs/src/graphics/battle_anims/overlay_b.parts.lz"
	.global BattleAnim_FireballSheet
BattleAnim_FireballSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fireball.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fireball.bitmap.lz"
	.byte 0x32	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x5df
	.global BattleAnim_MemberBurstImage
BattleAnim_MemberBurstImage:
	.incbin "games/tbs/src/graphics/battle_anims/member_burst.bitmap.lz"
	.byte 0x19, 0x14, 0x56	@ PACKER: left in the buffer by BattleAnim_FireballSheet+0x49d
	.global BattleAnim_SlashSheet
BattleAnim_SlashSheet:
	.incbin "games/tbs/src/graphics/battle_anims/slash_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/slash.parts.lz"
	.byte 0xf1, 0x59	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0xd12
	.global BattleAnim_WaveSheet
BattleAnim_WaveSheet:
	.incbin "games/tbs/src/graphics/battle_anims/wave.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/wave.bitmap.lz"
	.global BattleAnim_TargetBurstImage
BattleAnim_TargetBurstImage:
	.incbin "games/tbs/src/graphics/battle_anims/target_burst.bitmap.lz"
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleAnim_SlashSheet+0x83e
	.global BattleAnim_PinkStarSheet
BattleAnim_PinkStarSheet:
	.incbin "games/tbs/src/graphics/battle_anims/pink_star.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/pink_star.bitmap.lz"
	.byte 0x80	@ PACKER: left in the buffer by BattleAnim_TargetBurstImage+0x38f
	.global BattleAnim_WaterSpraySheet
BattleAnim_WaterSpraySheet:
	.incbin "games/tbs/src/graphics/battle_anims/water_spray_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/water_spray.parts.lz"
	.global BattleAnim_BlueFlameSheet
BattleAnim_BlueFlameSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_flame_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_flame.parts.lz"
	.byte 0xb7, 0x7c	@ PACKER: left in the buffer by BattleAnim_WaterSpraySheet+0xaee
	.global BattleAnim_ExplosionSheet
BattleAnim_ExplosionSheet:
	.incbin "games/tbs/src/graphics/battle_anims/explosion.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/explosion.bitmap.lz"
	.byte 0xec, 0x10, 0x3c	@ PACKER: left in the buffer by BattleAnim_WaterSpraySheet+0xdd5
	.global BattleAnim_MagentaSwirlSheet
BattleAnim_MagentaSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_anims/magenta_swirl.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/magenta_swirl.bitmap.lz"
	.byte 0x1c, 0x4c	@ PACKER: left in the buffer by BattleAnim_ExplosionSheet+0x82a
	.global BattleAnim_MagentaTailSheet
BattleAnim_MagentaTailSheet:
	.incbin "games/tbs/src/graphics/battle_anims/magenta_tail.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/magenta_tail.bitmap.lz"
	.global BattleAnim_ParticleSpritesA
BattleAnim_ParticleSpritesA:
	.incbin "games/tbs/src/graphics/battle_anims/dot_cells_a.parts.lz"
	.byte 0x0c, 0x40, 0x08	@ PACKER: left in the buffer by BattleAnim_MagentaTailSheet+0x299
	.global BattleAnim_ParticleSpritesB
BattleAnim_ParticleSpritesB:
	.incbin "games/tbs/src/graphics/battle_anims/dot_cells_b.parts.lz"
	.byte 0x94	@ PACKER: left in the buffer by BattleAnim_ParticleSpritesA+0x1bb
	.global BattleAnim_ParticleSpritesC
BattleAnim_ParticleSpritesC:
	.incbin "games/tbs/src/graphics/battle_anims/dot_cells_c.parts.lz"
	.global BattleAnim_ParticleSpritesD
BattleAnim_ParticleSpritesD:
	.incbin "games/tbs/src/graphics/battle_anims/dot_cells_d.parts.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleAnim_ParticleSpritesC+0x417
	.global BattleAnim_GreenPalette
BattleAnim_GreenPalette:
	.incbin "games/tbs/src/graphics/battle_anims/green_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/green_palette.bitmap.lz"
	.global BattleAnim_SwordSlashSheet
BattleAnim_SwordSlashSheet:
	.incbin "games/tbs/src/graphics/battle_anims/sword_slash_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/sword_slash.parts.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleAnim_ParticleSpritesC+0x4c7
	.global BattleAnim_RuneSheet
BattleAnim_RuneSheet:
	.incbin "games/tbs/src/graphics/battle_anims/rune.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/rune.bitmap.lz"
	.byte 0x43, 0x21, 0xbe	@ PACKER: left in the buffer by BattleAnim_SwordSlashSheet+0x3d9
	.global BattleAnim_FlameColumnSheet
BattleAnim_FlameColumnSheet:
	.incbin "games/tbs/src/graphics/battle_anims/flame_column.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flame_column.bitmap.lz"
	.byte 0x41	@ PACKER: left in the buffer by BattleAnim_RuneSheet+0x3ab
	.global BattleAnim_EarthWallSheet
BattleAnim_EarthWallSheet:
	.incbin "games/tbs/src/graphics/battle_anims/earth_wall_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/earth_wall.parts.lz"
	.global BattleAnim_OrangePaletteA
BattleAnim_OrangePaletteA:
	.incbin "games/tbs/src/graphics/battle_anims/orange_palette_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/orange_palette_a.bitmap.lz"
	.global BattleAnim_FlashBurstSheet
BattleAnim_FlashBurstSheet:
	.incbin "games/tbs/src/graphics/battle_anims/flash_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flash_burst.bitmap.lz"
	.byte 0x60	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x144b
	.global BattleAnim_ThornSheet
BattleAnim_ThornSheet:
	.incbin "games/tbs/src/graphics/battle_anims/thorn_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/thorn.parts.lz"
	.byte 0x78	@ PACKER: left in the buffer by BattleAnim_FlashBurstSheet+0xd9b
	.global BattleAnim_CometSheetA
BattleAnim_CometSheetA:
	.incbin "games/tbs/src/graphics/battle_anims/comet_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/comet_a.bitmap.lz"
	.byte 0x2a	@ PACKER: left in the buffer by BattleAnim_ThornSheet+0x21b
	.global BattleAnim_CometSheetB
BattleAnim_CometSheetB:
	.incbin "games/tbs/src/graphics/battle_anims/comet_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/comet_b.bitmap.lz"
	.byte 0xaf	@ PACKER: left in the buffer by BattleAnim_ThornSheet+0x2fb
	.global BattleAnim_CometSheetC
BattleAnim_CometSheetC:
	.incbin "games/tbs/src/graphics/battle_anims/comet_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/comet_c.bitmap.lz"
	.global BattleAnim_WhirlwindSheet
BattleAnim_WhirlwindSheet:
	.incbin "games/tbs/src/graphics/battle_anims/whirlwind_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/whirlwind.parts.lz"
	.global BattleAnim_GreenPillarSheet
BattleAnim_GreenPillarSheet:
	.incbin "games/tbs/src/graphics/battle_anims/green_pillar_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/green_pillar.parts.lz"
	.byte 0x21, 0xf0, 0x28	@ PACKER: left in the buffer by BattleAnim_WhirlwindSheet+0x75d
	.global BattleAnim_VineSheet
BattleAnim_VineSheet:
	.incbin "games/tbs/src/graphics/battle_anims/vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/vine.parts.lz"
	.byte 0xbe	@ PACKER: left in the buffer by BattleAnim_WhirlwindSheet+0xdcb
	.global BattleAnim_FirePillarSheetA
BattleAnim_FirePillarSheetA:
	.incbin "games/tbs/src/graphics/battle_anims/fire_pillar_a_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_pillar_a.parts.lz"
	.global BattleAnim_RedPaletteB
BattleAnim_RedPaletteB:
	.incbin "games/tbs/src/graphics/battle_anims/red_palette_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/red_palette_b.bitmap.lz"
	.global BattleAnim_OrangePaletteB
BattleAnim_OrangePaletteB:
	.incbin "games/tbs/src/graphics/battle_anims/orange_palette_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/orange_palette_b.bitmap.lz"
	.global BattleAnim_VioletCometSheet
BattleAnim_VioletCometSheet:
	.incbin "games/tbs/src/graphics/battle_anims/violet_comet_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/violet_comet.parts.lz"
	.byte 0x5e, 0xda, 0x33	@ PACKER: left in the buffer by BattleAnim_FirePillarSheetA+0x3e5
	.global BattleAnim_FlameballSheet
BattleAnim_FlameballSheet:
	.incbin "games/tbs/src/graphics/battle_anims/flameball.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flameball.bitmap.lz"
	.byte 0x83	@ PACKER: left in the buffer by BattleAnim_FirePillarSheetA+0x557
	.global BattleAnim_BoulderSheet
BattleAnim_BoulderSheet:
	.incbin "games/tbs/src/graphics/battle_anims/boulder_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/boulder.parts.lz"
	.global BattleAnim_RockWallSheet
BattleAnim_RockWallSheet:
	.incbin "games/tbs/src/graphics/battle_anims/rock_wall.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/rock_wall.bitmap.lz"
	.byte 0x07, 0x43, 0x0b	@ PACKER: left in the buffer by BattleAnim_VineSheet+0xcd1
	.global BattleAnim_StarDotSheet
BattleAnim_StarDotSheet:
	.incbin "games/tbs/src/graphics/battle_anims/star_dot_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/star_dot.parts.lz"
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleAnim_RockWallSheet+0x89e
	.global BattleAnim_MarsDjinnSheet
BattleAnim_MarsDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_anims/mars_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/mars_djinn.bitmap.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleAnim_StarDotSheet+0x3db
	.global BattleAnim_MarsDjinnSmallSheet
BattleAnim_MarsDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_anims/mars_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/mars_djinn_small.bitmap.lz"
	.byte 0x8a, 0x14, 0x39	@ PACKER: left in the buffer by BattleAnim_MarsDjinnSheet+0x289
	.global BattleAnim_JupiterDjinnSheet
BattleAnim_JupiterDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_anims/jupiter_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/jupiter_djinn.bitmap.lz"
	.byte 0x13	@ PACKER: left in the buffer by BattleAnim_MarsDjinnSheet+0x3a3
	.global BattleAnim_JupiterDjinnSmallSheet
BattleAnim_JupiterDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_anims/jupiter_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/jupiter_djinn_small.bitmap.lz"
	.byte 0x0b, 0x1f	@ PACKER: left in the buffer by BattleAnim_JupiterDjinnSheet+0x25a
	.balign 4, 0
	.global BattleAnim_MercuryDjinnSheet
BattleAnim_MercuryDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_anims/mercury_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/mercury_djinn.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleAnim_MercuryDjinnSmallSheet
BattleAnim_MercuryDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_anims/mercury_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/mercury_djinn_small.bitmap.lz"
	.global BattleAnim_VenusDjinnSheet
BattleAnim_VenusDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_anims/venus_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/venus_djinn.bitmap.lz"
	.global BattleAnim_VenusDjinnSmallSheet
BattleAnim_VenusDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_anims/venus_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/venus_djinn_small.bitmap.lz"
	.global BattleAnim_FireBlobSheet
BattleAnim_FireBlobSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fire_blob.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_blob.bitmap.lz"
	.byte 0x1a	@ PACKER: left in the buffer by BattleAnim_RockWallSheet+0xb87
	.global BattleAnim_FireStreakSheet
BattleAnim_FireStreakSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fire_streak.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_streak.bitmap.lz"
	.byte 0x00, 0x48	@ PACKER: left in the buffer by BattleAnim_WhirlwindSheet+0x15c2
	.global BattleAnim_LavaOrbSheet
BattleAnim_LavaOrbSheet:
	.incbin "games/tbs/src/graphics/battle_anims/lava_orb_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/lava_orb.parts.lz"
	.global BattleAnim_YellowRingSheet
BattleAnim_YellowRingSheet:
	.incbin "games/tbs/src/graphics/battle_anims/yellow_ring.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/yellow_ring.bitmap.lz"
	.global BattleAnim_YellowOrbSheet
BattleAnim_YellowOrbSheet:
	.incbin "games/tbs/src/graphics/battle_anims/yellow_orb.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/yellow_orb.bitmap.lz"
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by BattleAnim_LavaOrbSheet+0xa12
	.global BattleAnim_BubbleSheet
BattleAnim_BubbleSheet:
	.incbin "games/tbs/src/graphics/battle_anims/bubble.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/bubble.bitmap.lz"
	.byte 0x14, 0xb1, 0x50	@ PACKER: left in the buffer by BattleAnim_YellowOrbSheet+0x381
	.global BattleAnim_ShieldSheet
BattleAnim_ShieldSheet:
	.incbin "games/tbs/src/graphics/battle_anims/shield.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/shield.bitmap.lz"
	.byte 0x45, 0xb1, 0x12	@ PACKER: left in the buffer by BattleAnim_FireStreakSheet+0xcad
	.global BattleAnim_DaggerSheet
BattleAnim_DaggerSheet:
	.incbin "games/tbs/src/graphics/battle_anims/dagger.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/dagger.bitmap.lz"
	.byte 0x0a, 0x77, 0x19	@ PACKER: left in the buffer by BattleAnim_ShieldSheet+0xb45
	.global BattleAnim_PaletteRampImage
BattleAnim_PaletteRampImage:
	.incbin "games/tbs/src/graphics/battle_anims/spark_cells.parts.lz"
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleAnim_DaggerSheet+0x196
	.global BattleAnim_SmokeSheet
BattleAnim_SmokeSheet:
	.incbin "games/tbs/src/graphics/battle_anims/smoke.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/smoke.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleAnim_DaggerSheet+0x88b
	.global BattleAnim_BluePalette
BattleAnim_BluePalette:
	.incbin "games/tbs/src/graphics/battle_anims/blue_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_palette.bitmap.lz"
	.global BattleAnim_LimePalette
BattleAnim_LimePalette:
	.incbin "games/tbs/src/graphics/battle_anims/lime_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/lime_palette.bitmap.lz"
	.global BattleAnim_PinkPalette
BattleAnim_PinkPalette:
	.incbin "games/tbs/src/graphics/battle_anims/pink_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/pink_palette.bitmap.lz"
	.global BattleAnim_VioletPaletteC
BattleAnim_VioletPaletteC:
	.incbin "games/tbs/src/graphics/battle_anims/violet_palette_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/violet_palette_c.bitmap.lz"
	.global BattleAnim_CyanPalette
BattleAnim_CyanPalette:
	.incbin "games/tbs/src/graphics/battle_anims/cyan_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/cyan_palette.bitmap.lz"
	.global BattleAnim_TanPalette
BattleAnim_TanPalette:
	.incbin "games/tbs/src/graphics/battle_anims/tan_palette.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/tan_palette.bitmap.lz"
	.global BattleAnim_WispSheet
BattleAnim_WispSheet:
	.incbin "games/tbs/src/graphics/battle_anims/wisp.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/wisp.bitmap.lz"
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleAnim_SmokeSheet+0x7ae
	.global BattleAnim_GreenVineSheet
BattleAnim_GreenVineSheet:
	.incbin "games/tbs/src/graphics/battle_anims/green_vine.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/green_vine.bitmap.lz"
	.byte 0x1c, 0xaf	@ PACKER: left in the buffer by BattleAnim_WispSheet+0x516
	.global BattleAnim_PortalSheet
BattleAnim_PortalSheet:
	.incbin "games/tbs/src/graphics/battle_anims/portal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/portal.parts.lz"
	.byte 0x3f, 0x10	@ PACKER: left in the buffer by BattleAnim_WispSheet+0x622
	.global BattleAnim_SheepSheet
BattleAnim_SheepSheet:
	.incbin "games/tbs/src/graphics/battle_anims/sheep.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/sheep.bitmap.lz"
	.byte 0x13, 0x4e, 0x03	@ PACKER: left in the buffer by BattleAnim_SmokeSheet+0x865
	.global BattleAnim_SkullSheet
BattleAnim_SkullSheet:
	.incbin "games/tbs/src/graphics/battle_anims/skull.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/skull.bitmap.lz"
	.byte 0x98, 0x43, 0x4a	@ PACKER: left in the buffer by BattleAnim_SheepSheet+0x421
	.global BattleAnim_HeartSheet
BattleAnim_HeartSheet:
	.incbin "games/tbs/src/graphics/battle_anims/heart.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/heart.bitmap.lz"
	.byte 0x09	@ PACKER: left in the buffer by BattleAnim_SkullSheet+0x297
	.global BattleAnim_CounterRevealSheet
BattleAnim_CounterRevealSheet:
	.incbin "games/tbs/src/graphics/battle_anims/counter_reveal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/counter_reveal.parts.lz"
	.global BattleAnim_RedCrescentSheetA
BattleAnim_RedCrescentSheetA:
	.incbin "games/tbs/src/graphics/battle_anims/red_crescent_a_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/red_crescent_a.parts.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleAnim_CounterRevealSheet+0x49b
	.global BattleAnim_BlueBeastSheet
BattleAnim_BlueBeastSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_beast_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_beast.parts.lz"
	.global BattleAnim_RedCrescentSheetB
BattleAnim_RedCrescentSheetB:
	.incbin "games/tbs/src/graphics/battle_anims/red_crescent_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/red_crescent_b.bitmap.lz"
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleAnim_BlueBeastSheet+0x3ea
	.global BattleAnim_SpiralSheet
BattleAnim_SpiralSheet:
	.incbin "games/tbs/src/graphics/battle_anims/spiral.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/spiral.bitmap.lz"
	.global BattleAnim_VioletStreakSheet
BattleAnim_VioletStreakSheet:
	.incbin "games/tbs/src/graphics/battle_anims/violet_streak.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/violet_streak.bitmap.lz"
	.byte 0x5b	@ PACKER: left in the buffer by BattleAnim_SpiralSheet+0xc8b
	.global BattleAnim_CrescentMoonSheet
BattleAnim_CrescentMoonSheet:
	.incbin "games/tbs/src/graphics/battle_anims/crescent_moon.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/crescent_moon.bitmap.lz"
	.byte 0x2b	@ PACKER: left in the buffer by BattleAnim_VioletStreakSheet+0x4cf
	.global BattleAnim_ThornVineSheet
BattleAnim_ThornVineSheet:
	.incbin "games/tbs/src/graphics/battle_anims/thorn_vine_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/thorn_vine.parts.lz"
	.byte 0x1e, 0x99, 0x03	@ PACKER: left in the buffer by BattleAnim_VioletStreakSheet+0x691
	.global BattleAnim_IceShardSheet
BattleAnim_IceShardSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ice_shard_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ice_shard.parts.lz"
	.byte 0x3e	@ PACKER: left in the buffer by BattleAnim_VioletStreakSheet+0xa33
	.global BattleAnim_EmberStreakSheet
BattleAnim_EmberStreakSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ember_streak_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ember_streak.parts.lz"
	.global BattleAnim_BlueArcSheetA
BattleAnim_BlueArcSheetA:
	.incbin "games/tbs/src/graphics/battle_anims/blue_arc_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_arc_a.bitmap.lz"
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleAnim_EmberStreakSheet+0x7d5
	.global BattleAnim_BlueArcSheetB
BattleAnim_BlueArcSheetB:
	.incbin "games/tbs/src/graphics/battle_anims/blue_arc_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_arc_b.bitmap.lz"
	.byte 0xaa, 0x30	@ PACKER: left in the buffer by BattleAnim_EmberStreakSheet+0xace
	.global BattleAnim_GlowOrbSheet
BattleAnim_GlowOrbSheet:
	.incbin "games/tbs/src/graphics/battle_anims/glow_orb.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/glow_orb.bitmap.lz"
	.byte 0xe0	@ PACKER: left in the buffer by BattleAnim_EmberStreakSheet+0xb3b
	.global BattleAnim_IceChipSheet
BattleAnim_IceChipSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ice_chip_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ice_chip.parts.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleAnim_GlowOrbSheet+0x63f
	.global BattleAnim_IceBlockSheet
BattleAnim_IceBlockSheet:
	.incbin "games/tbs/src/graphics/battle_anims/ice_block_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/ice_block.parts.lz"
	.byte 0x30, 0xae	@ PACKER: left in the buffer by BattleAnim_BlueBeastSheet+0x1586
	.global BattleAnim_SparkleDots
BattleAnim_SparkleDots:
	.incbin "games/tbs/src/graphics/battle_anims/sparkle_dots.parts.lz"
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleAnim_IceBlockSheet+0x61
	.global BattleAnim_PinkBurstSheet
BattleAnim_PinkBurstSheet:
	.incbin "games/tbs/src/graphics/battle_anims/pink_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/pink_burst.bitmap.lz"
	.byte 0x57	@ PACKER: left in the buffer by BattleAnim_IceBlockSheet+0x36b
	.global BattleAnim_LightFanSheet
BattleAnim_LightFanSheet:
	.incbin "games/tbs/src/graphics/battle_anims/light_fan.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/light_fan.bitmap.lz"
	.byte 0x09, 0x82, 0x76	@ PACKER: left in the buffer by BattleAnim_IceBlockSheet+0x59d
	.global BattleAnim_BlueBurstSheet
BattleAnim_BlueBurstSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_burst.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_burst.bitmap.lz"
	.byte 0xc4, 0x99, 0xd3	@ PACKER: left in the buffer by BattleAnim_LightFanSheet+0x331
	.balign 4, 0
	.global BattleAnim_DustPuffSheet
BattleAnim_DustPuffSheet:
	.incbin "games/tbs/src/graphics/battle_anims/dust_puff.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/dust_puff.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global BattleAnim_GoldShellSheet
BattleAnim_GoldShellSheet:
	.incbin "games/tbs/src/graphics/battle_anims/gold_shell_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/gold_shell.parts.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleAnim_IceBlockSheet+0x1016
	.global BattleAnim_BlastSheet
BattleAnim_BlastSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blast_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blast.parts.lz"
	.global BattleAnim_FlameSheetB
BattleAnim_FlameSheetB:
	.incbin "games/tbs/src/graphics/battle_anims/flame_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/flame_b.bitmap.lz"
	.global BattleAnim_FirePillarSheetB
BattleAnim_FirePillarSheetB:
	.incbin "games/tbs/src/graphics/battle_anims/fire_pillar_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_pillar_b.bitmap.lz"
	.byte 0x0d, 0x03	@ PACKER: left in the buffer by BattleAnim_BlastSheet+0x11fe
	.global BattleAnim_RockSpireSheet
BattleAnim_RockSpireSheet:
	.incbin "games/tbs/src/graphics/battle_anims/rock_spire.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/rock_spire.bitmap.lz"
	.byte 0xa0, 0x20, 0x72	@ PACKER: left in the buffer by BattleAnim_FirePillarSheetB+0x7a1
	.global BattleAnim_LightningBoltSheet
BattleAnim_LightningBoltSheet:
	.incbin "games/tbs/src/graphics/battle_anims/lightning_bolt.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/lightning_bolt.bitmap.lz"
	.global BattleAnim_BeamSequenceImage
BattleAnim_BeamSequenceImage:
	.incbin "games/tbs/src/graphics/battle_anims/beam_sequence.parts.lz"
	.byte 0x5f	@ PACKER: left in the buffer by BattleAnim_LightningBoltSheet+0x37b
	.global BattleAnim_TwelveModeImage
BattleAnim_TwelveModeImage:
	.incbin "games/tbs/src/graphics/battle_anims/twelve_mode.bitmap.lz"
	.byte 0xa5	@ PACKER: left in the buffer by BattleAnim_LightningBoltSheet+0x42f
	.global BattleAnim_WindStreakSheet
BattleAnim_WindStreakSheet:
	.incbin "games/tbs/src/graphics/battle_anims/wind_streak_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/wind_streak.parts.lz"
	.global BattleAnim_YellowPaletteB
BattleAnim_YellowPaletteB:
	.incbin "games/tbs/src/graphics/battle_anims/yellow_palette_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/yellow_palette_b.bitmap.lz"
	.global BattleAnim_BlueRingSheet
BattleAnim_BlueRingSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_ring_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_ring.parts.lz"
	.global BattleAnim_RedPaletteC
BattleAnim_RedPaletteC:
	.incbin "games/tbs/src/graphics/battle_anims/red_palette_c.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/red_palette_c.bitmap.lz"
	.global BattleAnim_VioletPaletteD
BattleAnim_VioletPaletteD:
	.incbin "games/tbs/src/graphics/battle_anims/violet_palette_d.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/violet_palette_d.bitmap.lz"
	.global BattleAnim_BlueBeamSheet
BattleAnim_BlueBeamSheet:
	.incbin "games/tbs/src/graphics/battle_anims/blue_beam_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/blue_beam.parts.lz"
	.byte 0x01, 0x53, 0x33	@ PACKER: left in the buffer by BattleAnim_WindStreakSheet+0x6b5
	.global BattleAnim_VortexSheet
BattleAnim_VortexSheet:
	.incbin "games/tbs/src/graphics/battle_anims/vortex_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/vortex.parts.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleAnim_WindStreakSheet+0x912
	.global BattleAnim_TornadoSheet
BattleAnim_TornadoSheet:
	.incbin "games/tbs/src/graphics/battle_anims/tornado_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/tornado.parts.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleAnim_FireBurstSheet+0x1b32
	.global BattleAnim_CrystalSheet
BattleAnim_CrystalSheet:
	.incbin "games/tbs/src/graphics/battle_anims/crystal_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/crystal.parts.lz"
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleAnim_TornadoSheet+0x104d
	.global BattleAnim_WaterSparkleSheet
BattleAnim_WaterSparkleSheet:
	.incbin "games/tbs/src/graphics/battle_anims/water_sparkle.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/water_sparkle.bitmap.lz"
	.byte 0x11	@ PACKER: left in the buffer by BattleAnim_CrystalSheet+0x6b3
	.balign 4, 0
	.global BattleAnim_LightningPillarSheet
BattleAnim_LightningPillarSheet:
	.incbin "games/tbs/src/graphics/battle_anims/lightning_pillar.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/lightning_pillar.bitmap.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
