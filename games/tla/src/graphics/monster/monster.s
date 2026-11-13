	.section .rodata
	.global Monster_BlueDevilSprites
Monster_BlueDevilSprites:
	.incbin "games/tla/src/graphics/monster/blue_devil.pictures.lz"
	.byte 0x2f	@ PACKER: left in the buffer by Ui_Icons+0x5237
	.global Monster_PinkBruteSprites
Monster_PinkBruteSprites:
	.incbin "games/tla/src/graphics/monster/pink_brute.pictures.lz"
	.byte 0x28, 0x44	@ PACKER: left in the buffer by Monster_BlueDevilSprites+0x2f4a
	.global Monster_ClubTrollSprites
Monster_ClubTrollSprites:
	.incbin "games/tla/src/graphics/monster/club_troll.pictures.lz"
	.byte 0x47, 0x8e, 0x49	@ PACKER: left in the buffer by Monster_BlueDevilSprites+0x3c21
	.global Monster_JesterImpSprites
Monster_JesterImpSprites:
	.incbin "games/tla/src/graphics/monster/jester_imp.pictures.lz"
	.byte 0x00, 0xe4, 0x1a	@ PACKER: left in the buffer by Monster_ClubTrollSprites+0x2249
	.global Monster_GoldDragonSprites
Monster_GoldDragonSprites:
	.incbin "games/tla/src/graphics/monster/gold_dragon.pictures.lz"
	.byte 0x22, 0x40, 0xbf	@ PACKER: left in the buffer by Ui_Icons+0x5599
	.global Monster_ApeSprites
Monster_ApeSprites:
	.incbin "games/tla/src/graphics/monster/ape.pictures.lz"
	.global Monster_SwordImpSprites
Monster_SwordImpSprites:
	.incbin "games/tla/src/graphics/monster/sword_imp.pictures.lz"
	.byte 0xa1	@ PACKER: left in the buffer by Monster_ApeSprites+0xc23
	.global Monster_RedBeastSprites
Monster_RedBeastSprites:
	.incbin "games/tla/src/graphics/monster/red_beast.pictures.lz"
	.byte 0x19, 0xbb, 0x95	@ PACKER: left in the buffer by Ui_Icons+0x5e69
	.global Monster_RedBeetleSprites
Monster_RedBeetleSprites:
	.incbin "games/tla/src/graphics/monster/red_beetle.pictures.lz"
	.byte 0x82, 0x89, 0x74	@ PACKER: left in the buffer by Monster_RedBeastSprites+0x10d1
	.global Monster_PinkWormSprites
Monster_PinkWormSprites:
	.incbin "games/tla/src/graphics/monster/pink_worm.pictures.lz"
	.byte 0x09, 0x25, 0x74	@ PACKER: left in the buffer by Monster_RedBeetleSprites+0x1069
	.global Monster_BanditSprites
Monster_BanditSprites:
	.incbin "games/tla/src/graphics/monster/bandit.pictures.lz"
	.global Monster_SeaDragonSprites
Monster_SeaDragonSprites:
	.incbin "games/tla/src/graphics/monster/sea_dragon.pictures.lz"
	.byte 0xc0	@ PACKER: left in the buffer by Monster_RedBeastSprites+0x2c27
	.global Monster_SkullMageSprites
Monster_SkullMageSprites:
	.incbin "games/tla/src/graphics/monster/skull_mage.pictures.lz"
	.byte 0x05, 0xf5, 0x36	@ PACKER: left in the buffer by Monster_RedBeastSprites+0x3141
	.global Monster_ManEaterSprites
Monster_ManEaterSprites:
	.incbin "games/tla/src/graphics/monster/man_eater.pictures.lz"
	.byte 0x93, 0x72, 0x0a	@ PACKER: left in the buffer by Monster_SkullMageSprites+0x209d
	.global Monster_BlueCrabSprites
Monster_BlueCrabSprites:
	.incbin "games/tla/src/graphics/monster/blue_crab.pictures.lz"
	.byte 0x22, 0x79, 0x3f	@ PACKER: left in the buffer by Monster_SkullMageSprites+0x231d
	.global Monster_OstrichSprites
Monster_OstrichSprites:
	.incbin "games/tla/src/graphics/monster/ostrich.pictures.lz"
	.byte 0x1e, 0x02	@ PACKER: left in the buffer by Monster_SkullMageSprites+0x2c76
	.global Monster_BlueWyvernSprites
Monster_BlueWyvernSprites:
	.incbin "games/tla/src/graphics/monster/blue_wyvern.pictures.lz"
	.byte 0x12, 0x2a, 0x02	@ PACKER: left in the buffer by Monster_OstrichSprites+0x1ef9
	.global Monster_DarkWizardSprites
Monster_DarkWizardSprites:
	.incbin "games/tla/src/graphics/monster/dark_wizard.pictures.lz"
	.global Monster_WhiteKnightSprites
Monster_WhiteKnightSprites:
	.incbin "games/tla/src/graphics/monster/white_knight.pictures.lz"
	.byte 0x8d, 0x1e, 0x36	@ PACKER: left in the buffer by Monster_RedBeastSprites+0x4b49
	.global Monster_GoldenBirdSprites
Monster_GoldenBirdSprites:
	.incbin "games/tla/src/graphics/monster/golden_bird.pictures.lz"
	.global Monster_DarkKnightSprites
Monster_DarkKnightSprites:
	.incbin "games/tla/src/graphics/monster/dark_knight.pictures.lz"
	.global Monster_TreantSprites
Monster_TreantSprites:
	.incbin "games/tla/src/graphics/monster/treant.pictures.lz"
	.byte 0xe0	@ PACKER: left in the buffer by Monster_DarkKnightSprites+0x300f
	.global Monster_GreenOgreSprites
Monster_GreenOgreSprites:
	.incbin "games/tla/src/graphics/monster/green_ogre.pictures.lz"
	.byte 0x90, 0x49, 0x50	@ PACKER: left in the buffer by Monster_TreantSprites+0x2b79
	.global Monster_PinkOgreSprites
Monster_PinkOgreSprites:
	.incbin "games/tla/src/graphics/monster/pink_ogre.pictures.lz"
	.global Monster_WerewolfSprites
Monster_WerewolfSprites:
	.incbin "games/tla/src/graphics/monster/werewolf.pictures.lz"
	.byte 0xb4, 0x85	@ PACKER: left in the buffer by Monster_WhiteKnightSprites+0x3906
	.global Monster_MummySprites
Monster_MummySprites:
	.incbin "games/tla/src/graphics/monster/mummy.pictures.lz"
	.byte 0x72, 0x78	@ PACKER: left in the buffer by Monster_WerewolfSprites+0x2af2
	.global Monster_RaptorSprites
Monster_RaptorSprites:
	.incbin "games/tla/src/graphics/monster/raptor.pictures.lz"
	.byte 0xaa, 0x28	@ PACKER: left in the buffer by Monster_MummySprites+0x1aee
	.global Monster_GreenKnightSprites
Monster_GreenKnightSprites:
	.incbin "games/tla/src/graphics/monster/green_knight.pictures.lz"
@ Twelve drawn pictures in a sixteen-picture table; four empty pictures use index 255.
	.global Resource_Data21F
Resource_Data21F:
	.incbin "games/tla/src/graphics/monster/r21f.pictures.lz"
	.byte 0x34	@ PACKER: left in the buffer by Monster_WerewolfSprites+0x343f
	.global Monster_HydraSprites
Monster_HydraSprites:
	.incbin "games/tla/src/graphics/monster/hydra.pictures.lz"
	.byte 0x38, 0x41	@ PACKER: left in the buffer by Monster_WhiteKnightSprites+0x49a6
	.global Monster_LarvaSprites
Monster_LarvaSprites:
	.incbin "games/tla/src/graphics/monster/larva.pictures.lz"
	.byte 0x15, 0x6f	@ PACKER: left in the buffer by Monster_HydraSprites+0xf9a
	.global Monster_JackalSprites
Monster_JackalSprites:
	.incbin "games/tla/src/graphics/monster/jackal.pictures.lz"
	.byte 0xe1, 0xc9, 0x58	@ PACKER: left in the buffer by Monster_HydraSprites+0x2729
	.global Monster_UrchinSprites
Monster_UrchinSprites:
	.incbin "games/tla/src/graphics/monster/urchin.pictures.lz"
	.byte 0x32, 0x91	@ PACKER: left in the buffer by Monster_JackalSprites+0x1712
	.global Monster_DemonWarriorSprites
Monster_DemonWarriorSprites:
	.incbin "games/tla/src/graphics/monster/demon_warrior.pictures.lz"
	.global Monster_DragonLeftSprites
Monster_DragonLeftSprites:
	.incbin "games/tla/src/graphics/monster/dragon_left.pictures.lz"
	.byte 0xc0	@ PACKER: left in the buffer by Monster_HydraSprites+0x4623
	.global Monster_DragonRightSprites
Monster_DragonRightSprites:
	.incbin "games/tla/src/graphics/monster/dragon_right.pictures.lz"
	.byte 0x4c	@ PACKER: left in the buffer by Monster_DragonLeftSprites+0x42c7
	.global Monster_SeaSerpentSprites
Monster_SeaSerpentSprites:
	.incbin "games/tla/src/graphics/monster/sea_serpent.pictures.lz"
	.global Monster_SwordsmanSprites
Monster_SwordsmanSprites:
	.incbin "games/tla/src/graphics/monster/swordsman.pictures.lz"
	.byte 0x40, 0x96, 0x00	@ PACKER: left in the buffer by Monster_SeaSerpentSprites+0x220d
	.global Monster_StorkSprites
Monster_StorkSprites:
	.incbin "games/tla/src/graphics/monster/stork.pictures.lz"
	.byte 0xd6, 0x91	@ PACKER: left in the buffer by Monster_SwordsmanSprites+0x17e2
	.global Monster_VultureSprites
Monster_VultureSprites:
	.incbin "games/tla/src/graphics/monster/vulture.pictures.lz"
	.byte 0x21, 0xe4, 0x01	@ PACKER: left in the buffer by Monster_SeaSerpentSprites+0x2d95
	.global Monster_ToadSprites
Monster_ToadSprites:
	.incbin "games/tla/src/graphics/monster/toad.pictures.lz"
	.byte 0x06	@ PACKER: left in the buffer by Monster_VultureSprites+0x1047
	.global Monster_SwordMaidenSprites
Monster_SwordMaidenSprites:
	.incbin "games/tla/src/graphics/monster/sword_maiden.pictures.lz"
	.global Monster_BeastLordSprites
Monster_BeastLordSprites:
	.incbin "games/tla/src/graphics/monster/beast_lord.pictures.lz"
	.byte 0xef	@ PACKER: left in the buffer by Monster_SeaSerpentSprites+0x49ab
	.global Monster_BrawlerSprites
Monster_BrawlerSprites:
	.incbin "games/tla/src/graphics/monster/brawler.pictures.lz"
	.byte 0x5a, 0x29, 0x0b	@ PACKER: left in the buffer by Monster_BeastLordSprites+0x1df9
	.global Monster_MiniKnightSprites
Monster_MiniKnightSprites:
	.incbin "games/tla/src/graphics/monster/mini_knight.pictures.lz"
	.byte 0x05	@ PACKER: left in the buffer by Monster_BrawlerSprites+0x13c3
	.global Monster_ScorpionSprites
Monster_ScorpionSprites:
	.incbin "games/tla/src/graphics/monster/scorpion.pictures.lz"
	.global Monster_MermanSprites
Monster_MermanSprites:
	.incbin "games/tla/src/graphics/monster/merman.pictures.lz"
	.byte 0x1e, 0x04, 0x98	@ PACKER: left in the buffer by Monster_ScorpionSprites+0x2d61
	.global Monster_YoungSwordsmanSprites
Monster_YoungSwordsmanSprites:
	.incbin "games/tla/src/graphics/monster/young_swordsman.pictures.lz"
	.byte 0xbc, 0x12	@ PACKER: left in the buffer by Monster_MermanSprites+0x22fe
	.global Monster_GreenMageSprites
Monster_GreenMageSprites:
	.incbin "games/tla/src/graphics/monster/green_mage.pictures.lz"
	.global Monster_BlueOrbSprites
Monster_BlueOrbSprites:
	.incbin "games/tla/src/graphics/monster/blue_orb.pictures.lz"
	.byte 0x0a	@ PACKER: left in the buffer by Monster_GreenMageSprites+0x108b
	.global Monster_AxeWarriorSprites
Monster_AxeWarriorSprites:
	.incbin "games/tla/src/graphics/monster/axe_warrior.pictures.lz"
	.global Monster_SpearWarriorSprites
Monster_SpearWarriorSprites:
	.incbin "games/tla/src/graphics/monster/spear_warrior.pictures.lz"
	.byte 0x20, 0x2a	@ PACKER: left in the buffer by Monster_GreenMageSprites+0x30f6
	.global Monster_AxeGoblinSprites
Monster_AxeGoblinSprites:
	.incbin "games/tla/src/graphics/monster/axe_goblin.pictures.lz"
	.byte 0x83, 0x12	@ PACKER: left in the buffer by Monster_SpearWarriorSprites+0x21b2
	.global Monster_GhoulSprites
Monster_GhoulSprites:
	.incbin "games/tla/src/graphics/monster/ghoul.pictures.lz"
	.global Monster_HarpySprites
Monster_HarpySprites:
	.incbin "games/tla/src/graphics/monster/harpy.pictures.lz"
	.byte 0x0b, 0x01, 0x00	@ PACKER: left in the buffer by Monster_SpearWarriorSprites+0x2871
	.global Monster_TrollSprites
Monster_TrollSprites:
	.incbin "games/tla/src/graphics/monster/troll.pictures.lz"
	.byte 0x3c, 0x86	@ PACKER: left in the buffer by Monster_GreenMageSprites+0x3222
	.global Monster_WitchSprites
Monster_WitchSprites:
	.incbin "games/tla/src/graphics/monster/witch.pictures.lz"
	.byte 0xd8, 0x61	@ PACKER: left in the buffer by Monster_TrollSprites+0x1702
	.global Monster_GriffinSprites
Monster_GriffinSprites:
	.incbin "games/tla/src/graphics/monster/griffin.pictures.lz"
	.global Monster_ShieldKnightSprites
Monster_ShieldKnightSprites:
	.incbin "games/tla/src/graphics/monster/shield_knight.pictures.lz"
	.global Monster_OrcSprites
Monster_OrcSprites:
	.incbin "games/tla/src/graphics/monster/orc.pictures.lz"
	.byte 0xfe, 0x7b	@ PACKER: left in the buffer by Monster_GriffinSprites+0x260e
	.global Monster_ChimeraSprites
Monster_ChimeraSprites:
	.incbin "games/tla/src/graphics/monster/chimera.pictures.lz"
	.global Monster_ManticoreSprites
Monster_ManticoreSprites:
	.incbin "games/tla/src/graphics/monster/manticore.pictures.lz"
	.global Monster_OrcLordSprites
Monster_OrcLordSprites:
	.incbin "games/tla/src/graphics/monster/orc_lord.pictures.lz"
	.byte 0x0b, 0x00	@ PACKER: left in the buffer by Monster_ManticoreSprites+0x364a
	.global Monster_GargoyleSprites
Monster_GargoyleSprites:
	.incbin "games/tla/src/graphics/monster/gargoyle.pictures.lz"
	.byte 0xb1, 0x5b	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x3092
	.global Monster_OgreSprites
Monster_OgreSprites:
	.incbin "games/tla/src/graphics/monster/ogre.pictures.lz"
	.byte 0x76, 0xbf, 0x83	@ PACKER: left in the buffer by Monster_GargoyleSprites+0x2869
	.global Monster_TailedImpSprites
Monster_TailedImpSprites:
	.incbin "games/tla/src/graphics/monster/tailed_imp.pictures.lz"
	.global Monster_WispGhostSprites
Monster_WispGhostSprites:
	.incbin "games/tla/src/graphics/monster/wisp_ghost.pictures.lz"
	.byte 0x3e, 0xca	@ PACKER: left in the buffer by Monster_OgreSprites+0xe46
	.global Monster_SalamanderSprites
Monster_SalamanderSprites:
	.incbin "games/tla/src/graphics/monster/salamander.pictures.lz"
	.byte 0x62, 0x4f	@ PACKER: left in the buffer by Monster_WispGhostSprites+0xe16
	.global Monster_AquaBeastSprites
Monster_AquaBeastSprites:
	.incbin "games/tla/src/graphics/monster/aqua_beast.pictures.lz"
	.byte 0xba, 0x77, 0xc9	@ PACKER: left in the buffer by Monster_SalamanderSprites+0xce9
	.global Monster_HoundSprites
Monster_HoundSprites:
	.incbin "games/tla/src/graphics/monster/hound.pictures.lz"
	.global Monster_BearSprites
Monster_BearSprites:
	.incbin "games/tla/src/graphics/monster/bear.pictures.lz"
	.global Monster_SirenSprites
Monster_SirenSprites:
	.incbin "games/tla/src/graphics/monster/siren.pictures.lz"
	.global Monster_BlueWispSprites
Monster_BlueWispSprites:
	.incbin "games/tla/src/graphics/monster/blue_wisp.pictures.lz"
	.byte 0xe9, 0x10	@ PACKER: left in the buffer by Monster_SirenSprites+0x105a
	.global Monster_MushroomSprites
Monster_MushroomSprites:
	.incbin "games/tla/src/graphics/monster/mushroom.pictures.lz"
	.byte 0x47, 0x7d, 0x5a	@ PACKER: left in the buffer by Monster_BlueWispSprites+0xd09
	.global Monster_RedWormSprites
Monster_RedWormSprites:
	.incbin "games/tla/src/graphics/monster/red_worm.pictures.lz"
	.global Monster_BatSprites
Monster_BatSprites:
	.incbin "games/tla/src/graphics/monster/bat.pictures.lz"
	.byte 0x3c, 0xcc, 0xb9	@ PACKER: left in the buffer by Monster_RedWormSprites+0x7dd
	.global Monster_JellySprites
Monster_JellySprites:
	.incbin "games/tla/src/graphics/monster/jelly.pictures.lz"
	.byte 0x39	@ PACKER: left in the buffer by Monster_BlueWispSprites+0xf2f
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "games/tla/src/graphics/monster/small_lizard.pictures.lz"
	.byte 0x1d	@ PACKER: left in the buffer by Monster_JellySprites+0xc6f
	.global Monster_WalkingPlantSprites
Monster_WalkingPlantSprites:
	.incbin "games/tla/src/graphics/monster/walking_plant.pictures.lz"
	.byte 0xd5	@ PACKER: left in the buffer by Monster_SmallLizardSprites+0xc17
	.global Monster_FlySprites
Monster_FlySprites:
	.incbin "games/tla/src/graphics/monster/fly.pictures.lz"
	.byte 0x13	@ PACKER: left in the buffer by Monster_WalkingPlantSprites+0x773
	.global Monster_CrabSprites
Monster_CrabSprites:
	.incbin "games/tla/src/graphics/monster/crab.pictures.lz"
	.global Monster_SmallBirdSprites
Monster_SmallBirdSprites:
	.incbin "games/tla/src/graphics/monster/small_bird.pictures.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Monster_MothSprites
Monster_MothSprites:
	.incbin "games/tla/src/graphics/monster/moth.pictures.lz"
	.byte 0x92, 0x11, 0x5e	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0x9cd
	.global Monster_FrogSprites
Monster_FrogSprites:
	.incbin "games/tla/src/graphics/monster/frog.pictures.lz"
	.byte 0xb3	@ PACKER: left in the buffer by Monster_SmallBirdSprites+0xc1b
	.global Monster_ManedLionSprites
Monster_ManedLionSprites:
	.incbin "games/tla/src/graphics/monster/maned_lion.pictures.lz"
	.byte 0x35	@ PACKER: left in the buffer by Monster_SirenSprites+0x26b3
	.section .rodata.a
	.global Monster_WingedDragonSprites
Monster_WingedDragonSprites:
	.incbin "games/tla/src/graphics/monster/winged_dragon.pictures.lz"
	.byte 0xc6	@ PACKER: left in the buffer by Monster_SeaSerpentSprites+0x4f53
	.global Monster_BlueMageSprites
Monster_BlueMageSprites:
	.incbin "games/tla/src/graphics/monster/blue_mage.pictures.lz"
	.global Monster_SkeletonSprites
Monster_SkeletonSprites:
	.incbin "games/tla/src/graphics/monster/skeleton.pictures.lz"
	.byte 0xe6, 0x60, 0x43	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x2a39
	.global Monster_FlameSkullSprites
Monster_FlameSkullSprites:
	.incbin "games/tla/src/graphics/monster/flame_skull.pictures.lz"
	.byte 0xc1, 0xb3	@ PACKER: left in the buffer by Monster_SkeletonSprites+0xbea
	.global Monster_FireDragonSprites
Monster_FireDragonSprites:
	.incbin "games/tla/src/graphics/monster/fire_dragon.pictures.lz"
	.byte 0x0f	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x4bb3
	.global Monster_CrawlerSprites
Monster_CrawlerSprites:
	.incbin "games/tla/src/graphics/monster/crawler.pictures.lz"
	.byte 0xf0, 0x33, 0x17	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x2369
	.global Monster_MimicSprites
Monster_MimicSprites:
	.incbin "games/tla/src/graphics/monster/mimic.pictures.lz"
	.byte 0xb9	@ PACKER: left in the buffer by Monster_CrawlerSprites+0x199f
	.global Monster_SeaBeastSprites
Monster_SeaBeastSprites:
	.incbin "games/tla/src/graphics/monster/sea_beast.pictures.lz"
	.global Monster_SpiritHorseSprites
Monster_SpiritHorseSprites:
	.incbin "games/tla/src/graphics/monster/spirit_horse.pictures.lz"
	.byte 0x31, 0x20, 0x83	@ PACKER: left in the buffer by Monster_SeaBeastSprites+0xcc5
	.global Monster_LizardManSprites
Monster_LizardManSprites:
	.incbin "games/tla/src/graphics/monster/lizard_man.pictures.lz"
	.byte 0xda, 0xf1, 0x0d	@ PACKER: left in the buffer by Monster_SeaBeastSprites+0x340d
	.global Monster_KrakenSprites
Monster_KrakenSprites:
	.incbin "games/tla/src/graphics/monster/kraken.pictures.lz"
	.byte 0xbd, 0xca, 0x95	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x40c5
	.global Monster_SlimeSprites
Monster_SlimeSprites:
	.incbin "games/tla/src/graphics/monster/slime.pictures.lz"
	.byte 0x40, 0x2c	@ PACKER: left in the buffer by Monster_KrakenSprites+0x36c6
	.global Monster_SaturosSprites
Monster_SaturosSprites:
	.incbin "games/tla/src/graphics/monster/saturos.pictures.lz"
	.global Monster_MenardiSprites
Monster_MenardiSprites:
	.incbin "games/tla/src/graphics/monster/menardi.pictures.lz"
	.byte 0x52	@ PACKER: left in the buffer by Monster_SaturosSprites+0x3da7
	.global Monster_GhostHeadSprites
Monster_GhostHeadSprites:
	.incbin "games/tla/src/graphics/monster/ghost_head.pictures.lz"
	.global Monster_FusionDragonSprites
Monster_FusionDragonSprites:
	.incbin "games/tla/src/graphics/monster/fusion_dragon.pictures.lz"
	.byte 0x70	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x47f3
	.global Monster_PurpleBeastSprites
Monster_PurpleBeastSprites:
	.incbin "games/tla/src/graphics/monster/purple_beast.pictures.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Monster_RedWarriorSprites
Monster_RedWarriorSprites:
	.incbin "games/tla/src/graphics/monster/red_warrior.pictures.lz"
	.global Monster_SwordGirlSprites
Monster_SwordGirlSprites:
	.incbin "games/tla/src/graphics/monster/sword_girl.pictures.lz"
