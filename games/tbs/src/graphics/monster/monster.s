	.include "games/tbs/include/ed_asm.h"
	.section .rodata
	.global Monster_AxeGoblinSprites
Monster_AxeGoblinSprites:
	.incbin "games/tbs/src/graphics/monster/axe_goblin.pictures.lz"
	.byte 0xda, 0x01	@ PACKER: left in the buffer by Ui_CommandIcons+0x21b2
	.global Monster_GhoulSprites
Monster_GhoulSprites:
	.incbin "games/tbs/src/graphics/monster/ghoul.pictures.lz"
	.global Monster_HarpySprites
Monster_HarpySprites:
	.incbin "games/tbs/src/graphics/monster/harpy.pictures.lz"
	.byte 0xbb, 0x01, 0x75	@ PACKER: left in the buffer by Ui_CommandIcons+0x2871
	.global Monster_TrollSprites
Monster_TrollSprites:
	.incbin "games/tbs/src/graphics/monster/troll.pictures.lz"
	.byte 0x2b, 0x00	@ PACKER: left in the buffer by Ui_CommandIcons+0x3222
	.global Monster_WitchSprites
Monster_WitchSprites:
	.incbin "games/tbs/src/graphics/monster/witch.pictures.lz"
	.byte 0xd8, 0x61	@ PACKER: left in the buffer by Monster_TrollSprites+0x1702
	.global Monster_GriffinSprites
Monster_GriffinSprites:
	.incbin "games/tbs/src/graphics/monster/griffin.pictures.lz"
	.global Monster_ShieldKnightSprites
Monster_ShieldKnightSprites:
	.incbin "games/tbs/src/graphics/monster/shield_knight.pictures.lz"
	.global Monster_OrcSprites
Monster_OrcSprites:
	.incbin "games/tbs/src/graphics/monster/orc.pictures.lz"
	.byte 0xfe, 0x7b	@ PACKER: left in the buffer by Monster_GriffinSprites+0x260e
	.global Monster_ChimeraSprites
Monster_ChimeraSprites:
	.incbin "games/tbs/src/graphics/monster/chimera.pictures.lz"
	.global Monster_LivingArmorSprites
Monster_LivingArmorSprites:
	.incbin "games/tbs/src/graphics/monster/living_armor.pictures.lz"
	.byte 0x0e, 0x40	@ PACKER: left in the buffer by Monster_ChimeraSprites+0x3e7e
	.global Monster_ManticoreSprites
Monster_ManticoreSprites:
	.incbin "games/tbs/src/graphics/monster/manticore.pictures.lz"
	.global Monster_OrcLordSprites
Monster_OrcLordSprites:
	.incbin "games/tbs/src/graphics/monster/orc_lord.pictures.lz"
	.byte 0x0b, 0x00	@ PACKER: left in the buffer by Monster_ManticoreSprites+0x364a
	.global Monster_GargoyleSprites
Monster_GargoyleSprites:
	.incbin "games/tbs/src/graphics/monster/gargoyle.pictures.lz"
	.byte 0xb1, 0x5b	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x3092
	.global Monster_LizardManSprites
Monster_LizardManSprites:
	.incbin "games/tbs/src/graphics/monster/lizard_man.pictures.lz"
	.byte 0x47, 0x0b, 0x00	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x340d
	.global Monster_OgreSprites
Monster_OgreSprites:
	.incbin "games/tbs/src/graphics/monster/ogre.pictures.lz"
	.byte 0x4f, 0x40, 0x6e	@ PACKER: left in the buffer by Monster_LizardManSprites+0x2869
	.global Monster_TailedImpSprites
Monster_TailedImpSprites:
	.incbin "games/tbs/src/graphics/monster/tailed_imp.pictures.lz"
	.global Monster_WispGhostSprites
Monster_WispGhostSprites:
	.incbin "games/tbs/src/graphics/monster/wisp_ghost.pictures.lz"
	.byte 0x3e, 0xca	@ PACKER: left in the buffer by Monster_OgreSprites+0xe46
	.global Monster_SalamanderSprites
Monster_SalamanderSprites:
	.incbin "games/tbs/src/graphics/monster/salamander.pictures.lz"
	.byte 0x62, 0x4f	@ PACKER: left in the buffer by Monster_WispGhostSprites+0xe16
	.global Monster_AquaBeastSprites
Monster_AquaBeastSprites:
	.incbin "games/tbs/src/graphics/monster/aqua_beast.pictures.lz"
	.byte 0xba, 0x77, 0xc9	@ PACKER: left in the buffer by Monster_SalamanderSprites+0xce9
	.global Monster_HoundSprites
Monster_HoundSprites:
	.incbin "games/tbs/src/graphics/monster/hound.pictures.lz"
	.global Monster_BearSprites
Monster_BearSprites:
	.incbin "games/tbs/src/graphics/monster/bear.pictures.lz"
	.global Monster_SirenSprites
Monster_SirenSprites:
	.incbin "games/tbs/src/graphics/monster/siren.pictures.lz"
	.global Monster_BlueWispSprites
Monster_BlueWispSprites:
	.incbin "games/tbs/src/graphics/monster/blue_wisp.pictures.lz"
	.byte 0xe9, 0x10	@ PACKER: left in the buffer by Monster_SirenSprites+0x105a
	.global Monster_MushroomSprites
Monster_MushroomSprites:
	.incbin "games/tbs/src/graphics/monster/mushroom.pictures.lz"
	.byte 0x47, 0x7d, 0x5a	@ PACKER: left in the buffer by Monster_BlueWispSprites+0xd09
	.global Monster_RedWormSprites
Monster_RedWormSprites:
	.incbin "games/tbs/src/graphics/monster/red_worm.pictures.lz"
	.global Monster_FairySprites
Monster_FairySprites:
	.incbin "games/tbs/src/graphics/monster/fairy.pictures.lz"
	.global Monster_BatSprites
Monster_BatSprites:
	.incbin "games/tbs/src/graphics/monster/bat.pictures.lz"
	.byte 0xf2, 0x2e, 0xa3	@ PACKER: left in the buffer by Monster_FairySprites+0x7dd
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "games/tbs/src/graphics/monster/small_lizard.pictures.lz"
	.byte 0x70	@ PACKER: left in the buffer by Monster_FairySprites+0xc6f
	.global Monster_WalkingPlantSprites
Monster_WalkingPlantSprites:
	.incbin "games/tbs/src/graphics/monster/walking_plant.pictures.lz"
	.byte 0xd5	@ PACKER: left in the buffer by Monster_SmallLizardSprites+0xc17
	.global Monster_FlySprites
Monster_FlySprites:
	.incbin "games/tbs/src/graphics/monster/fly.pictures.lz"
	.byte 0x13	@ PACKER: left in the buffer by Monster_WalkingPlantSprites+0x773
	.global Monster_CrabSprites
Monster_CrabSprites:
	.incbin "games/tbs/src/graphics/monster/crab.pictures.lz"
	.balign 4, 0
	.global Monster_SmallBirdSprites
Monster_SmallBirdSprites:
	.incbin "games/tbs/src/graphics/monster/small_bird.pictures.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Monster_MothSprites
Monster_MothSprites:
	.incbin "games/tbs/src/graphics/monster/moth.pictures.lz"
	.byte 0x92, 0x11, 0x5e	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0x9cd
	.global Monster_FrogSprites
Monster_FrogSprites:
	.incbin "games/tbs/src/graphics/monster/frog.pictures.lz"
	.byte 0xb3	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0xc1b
	.global Monster_ManedLionSprites
Monster_ManedLionSprites:
	.incbin "games/tbs/src/graphics/monster/maned_lion.pictures.lz"
	.byte 0x35	@ PACKER: left in the buffer by Monster_SirenSprites+0x26b3
	.global Monster_SerpentSprites
Monster_SerpentSprites:
	.incbin "games/tbs/src/graphics/monster/serpent.pictures.lz"
	.byte 0x0c, 0xb5	@ PACKER: left in the buffer by Monster_SirenSprites+0x2712
	.global Monster_WingedDragonSprites
Monster_WingedDragonSprites:
	.incbin "games/tbs/src/graphics/monster/winged_dragon.pictures.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xee	@ PACKER: left in the buffer by Ui_Icons+0x4f53
	.else
	.byte 0x8c	@ PACKER: left in the buffer by Ui_Icons+0x4f53
	.endif
	.global Monster_BlueMageSprites
Monster_BlueMageSprites:
	.incbin "games/tbs/src/graphics/monster/blue_mage.pictures.lz"
	.global Monster_SkeletonSprites
Monster_SkeletonSprites:
	.incbin "games/tbs/src/graphics/monster/skeleton.pictures.lz"
	.byte 0xe6, 0x60, 0x43	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x2a39
	.global Monster_FlameSkullSprites
Monster_FlameSkullSprites:
	.incbin "games/tbs/src/graphics/monster/flame_skull.pictures.lz"
	.byte 0xc1, 0xb3	@ PACKER: left in the buffer by Monster_SkeletonSprites+0xbea
	.global Monster_FireDragonSprites
Monster_FireDragonSprites:
	.incbin "games/tbs/src/graphics/monster/fire_dragon.pictures.lz"
	.byte 0x0f	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x4bb3
	.global Monster_CrawlerSprites
Monster_CrawlerSprites:
	.incbin "games/tbs/src/graphics/monster/crawler.pictures.lz"
	.byte 0xf0, 0x33, 0x17	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x2369
	.global Monster_MimicSprites
Monster_MimicSprites:
	.incbin "games/tbs/src/graphics/monster/mimic.pictures.lz"
	.byte 0xb9	@ PACKER: left in the buffer by Monster_CrawlerSprites+0x199f
	.global Monster_SeaBeastSprites
Monster_SeaBeastSprites:
	.incbin "games/tbs/src/graphics/monster/sea_beast.pictures.lz"
	.global Monster_SpiritHorseSprites
Monster_SpiritHorseSprites:
	.incbin "games/tbs/src/graphics/monster/spirit_horse.pictures.lz"
	.byte 0x31, 0x20, 0x83	@ PACKER: left in the buffer by Monster_SeaBeastSprites+0xcc5
	.global Monster_KrakenSprites
Monster_KrakenSprites:
	.incbin "games/tbs/src/graphics/monster/kraken.pictures.lz"
	.byte 0xbd, 0xca, 0x95	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x40c5
	.global Monster_SlimeSprites
Monster_SlimeSprites:
	.incbin "games/tbs/src/graphics/monster/slime.pictures.lz"
	.byte 0x40, 0x2c	@ PACKER: left in the buffer by Monster_KrakenSprites+0x36c6
	.global Monster_SaturosSprites
Monster_SaturosSprites:
	.incbin "games/tbs/src/graphics/monster/saturos.pictures.lz"
	.global Monster_MenardiSprites
Monster_MenardiSprites:
	.incbin "games/tbs/src/graphics/monster/menardi.pictures.lz"
	.byte 0x52	@ PACKER: left in the buffer by Monster_SaturosSprites+0x3da7
	.global Monster_GhostHeadSprites
Monster_GhostHeadSprites:
	.incbin "games/tbs/src/graphics/monster/ghost_head.pictures.lz"
	.global Monster_FusionDragonSprites
Monster_FusionDragonSprites:
	.incbin "games/tbs/src/graphics/monster/fusion_dragon.pictures.lz"
	.byte 0x70	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x47f3
	.balign 4, 0
	.global Monster_PurpleBeastSprites
Monster_PurpleBeastSprites:
	.incbin "games/tbs/src/graphics/monster/purple_beast.pictures.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Monster_RedWarriorSprites
Monster_RedWarriorSprites:
	.incbin "games/tbs/src/graphics/monster/red_warrior.pictures.lz"
	.global Monster_SwordGirlSprites
Monster_SwordGirlSprites:
	.incbin "games/tbs/src/graphics/monster/sword_girl.pictures.lz"
