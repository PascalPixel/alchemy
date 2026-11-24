	.include "enums/sprite_ids.inc"

@ The packed resource that holds each battle sprite's pictures
@ (struct SpritePictureCacheSource), which the sprite picture cache decodes.
@ Each row: the resource and the sprite (enum SpriteId, sprite_ids.h); sprite 0
@ ends the table.
	.macro sprite_source resource:req, sprite:req
	.2byte ResourceId_\resource, \sprite
	.endm

	.section .rodata
	.balign 4
	.global gSpritePictureCache_Sources
gSpritePictureCache_Sources:
	sprite_source BlueDevilSprites, SPRITE_BATTLE_BLUE_DRAGON
	sprite_source PinkBruteSprites, SPRITE_BATTLE_RED_DEMON
	sprite_source ClubTrollSprites, SPRITE_BATTLE_AKA_MANAH
	sprite_source JesterImpSprites, SPRITE_BATTLE_KOBOLD
	sprite_source GoldDragonSprites, SPRITE_BATTLE_FLAME_DRAGON
	sprite_source ApeSprites, SPRITE_BATTLE_CHESTBEATER
	sprite_source SwordImpSprites, SPRITE_BATTLE_PIXIE
	sprite_source RedBeastSprites, SPRITE_BATTLE_RED_BEAST
	sprite_source RedBeetleSprites, SPRITE_BATTLE_PUNCH_ANT
	sprite_source PinkWormSprites, SPRITE_BATTLE_PINK_WORM
	sprite_source BanditSprites, SPRITE_BATTLE_ASSASSIN
	sprite_source SeaDragonSprites, SPRITE_BATTLE_WYVERN_CHICK
	sprite_source SkullMageSprites, SPRITE_BATTLE_DOOMSAYER
	sprite_source ManEaterSprites, SPRITE_BATTLE_MAD_PLANT
	sprite_source BlueCrabSprites, SPRITE_BATTLE_BLUE_CRAB
	sprite_source OstrichSprites, SPRITE_BATTLE_EMU
	sprite_source BlueWyvernSprites, SPRITE_BATTLE_GRESSIL
	sprite_source DarkWizardSprites, SPRITE_BATTLE_STAR_MAGICIAN
	sprite_source WhiteKnightSprites, SPRITE_BATTLE_GHOST_ARMY
	sprite_source GoldenBirdSprites, SPRITE_BATTLE_PHOENIX
	sprite_source DarkKnightSprites, SPRITE_BATTLE_LIVING_ARMOR
	sprite_source TreantSprites, SPRITE_BATTLE_WOOD_WALKER
	sprite_source GreenOgreSprites, SPRITE_BATTLE_MINI_GOBLIN
	sprite_source PinkOgreSprites, SPRITE_BATTLE_MINOTAURUS
	sprite_source WerewolfSprites, SPRITE_BATTLE_WOLFKIN_CUB
	sprite_source MummySprites, SPRITE_BATTLE_MUMMY
	sprite_source RaptorSprites, SPRITE_BATTLE_DINO
	sprite_source GreenKnightSprites, SPRITE_BATTLE_SENTINEL
	sprite_source WyvernSprites, SPRITE_BATTLE_WYVERN
	sprite_source HydraSprites, SPRITE_BATTLE_AQUA_HYDRA
	sprite_source LarvaSprites, SPRITE_BATTLE_CONCH_SHELL
	sprite_source JackalSprites, SPRITE_BATTLE_WILD_WOLF
	sprite_source UrchinSprites, SPRITE_BATTLE_URCHIN_BEAST
	sprite_source DemonWarriorSprites, SPRITE_BATTLE_VALUKAR
	sprite_source DragonLeftSprites, SPRITE_BATTLE_SERPENT
	sprite_source DragonRightSprites, SPRITE_BATTLE_SERPENT_RIGHT
	sprite_source SeaSerpentSprites, SPRITE_BATTLE_SEA_DRAGON
	sprite_source SwordsmanSprites, SPRITE_BATTLE_BRIGGS
	sprite_source StorkSprites, SPRITE_BATTLE_SEABIRD
	sprite_source VultureSprites, SPRITE_BATTLE_ROC
	sprite_source ToadSprites, SPRITE_BATTLE_MOMONGA
	sprite_source SwordMaidenSprites, SPRITE_BATTLE_KARST
	sprite_source BeastLordSprites, SPRITE_BATTLE_AVIMANDER
	sprite_source BrawlerSprites, SPRITE_BATTLE_RUFFIAN
	sprite_source MiniKnightSprites, SPRITE_BATTLE_ANGLE_WORM
	sprite_source ScorpionSprites, SPRITE_BATTLE_KING_SCORPION
	sprite_source MermanSprites, SPRITE_BATTLE_MERMAN
	sprite_source YoungSwordsmanSprites, SPRITE_BATTLE_SEA_FIGHTER
	sprite_source GreenMageSprites, SPRITE_BATTLE_AGATIO
	sprite_source BlueOrbSprites, SPRITE_BATTLE_REFRESH_BALL
	sprite_source AxeWarriorSprites, SPRITE_BATTLE_KNIGHT
	sprite_source SpearWarriorSprites, SPRITE_BATTLE_MOAPA
	sprite_source AxeGoblinSprites, SPRITE_BATTLE_TROLL
	sprite_source GhoulSprites, SPRITE_BATTLE_ZOMBIE
	sprite_source HarpySprites, SPRITE_BATTLE_HARPY
	sprite_source TrollSprites, SPRITE_BATTLE_GOLEM
	sprite_source WitchSprites, SPRITE_BATTLE_GHOST
	sprite_source GriffinSprites, SPRITE_BATTLE_GRYPHON
	sprite_source ShieldKnightSprites, SPRITE_BATTLE_RAT_SOLDIER
	sprite_source OrcSprites, SPRITE_BATTLE_GHOUL
	sprite_source ChimeraSprites, SPRITE_BATTLE_CHIMERA
	sprite_source ManticoreSprites, SPRITE_BATTLE_MANTICORE
	sprite_source OrcLordSprites, SPRITE_BATTLE_GOBLIN
	sprite_source GargoyleSprites, SPRITE_BATTLE_GARGOYLE
	sprite_source OgreSprites, SPRITE_BATTLE_ORC
	sprite_source TailedImpSprites, SPRITE_BATTLE_VENUS_DJINNI
	sprite_source WispGhostSprites, SPRITE_BATTLE_JUPITER_DJINNI
	sprite_source SalamanderSprites, SPRITE_BATTLE_MARS_DJINNI
	sprite_source AquaBeastSprites, SPRITE_BATTLE_MERCURY_DJINNI
	sprite_source HoundSprites, SPRITE_BATTLE_DREAD_HOUND
	sprite_source BearSprites, SPRITE_BATTLE_MAULER
	sprite_source SirenSprites, SPRITE_BATTLE_SIREN
	sprite_source BlueWispSprites, SPRITE_BATTLE_VERMIN
	sprite_source MushroomSprites, SPRITE_BATTLE_WILD_MUSHROOM
	sprite_source RedWormSprites, SPRITE_BATTLE_SLIME
	sprite_source BatSprites, SPRITE_BATTLE_BAT
	sprite_source JellySprites, SPRITE_BATTLE_CUTTLE
	sprite_source SmallLizardSprites, SPRITE_BATTLE_RAT
	sprite_source WalkingPlantSprites, SPRITE_BATTLE_STONE_SOLDIER
	sprite_source FlySprites, SPRITE_BATTLE_DRONE_BEE
	sprite_source CrabSprites, SPRITE_BATTLE_SPIDER
	sprite_source SmallBirdSprites, SPRITE_BATTLE_MOLE
	sprite_source MothSprites, SPRITE_BATTLE_DIRGE
	sprite_source FrogSprites, SPRITE_BATTLE_GRUB
	sprite_source ManedLionSprites, SPRITE_BATTLE_APE
	sprite_source SerpentSprites, SPRITE_BATTLE_SALAMANDER
	sprite_source WingedDragonSprites, SPRITE_BATTLE_LIVING_STATUE
	sprite_source BlueMageSprites, SPRITE_BATTLE_GNOME
	sprite_source SkeletonSprites, SPRITE_BATTLE_SKELETON
	sprite_source FlameSkullSprites, SPRITE_BATTLE_WILL_HEAD
	sprite_source FireDragonSprites, SPRITE_BATTLE_TORNADO_LIZARD
	sprite_source CrawlerSprites, SPRITE_BATTLE_ANT_LION
	sprite_source MimicSprites, SPRITE_BATTLE_MIMIC
	sprite_source SeaBeastSprites, SPRITE_BATTLE_TOADONPA
	sprite_source SpiritHorseSprites, SPRITE_BATTLE_AMAZE
	sprite_source LizardManSprites, SPRITE_BATTLE_LIZARD_MAN
	sprite_source KrakenSprites, SPRITE_BATTLE_KRAKEN
	sprite_source SlimeSprites, SPRITE_BATTLE_KRAKEN_RIGHT
	sprite_source SaturosSprites, SPRITE_BATTLE_SATUROS
	sprite_source MenardiSprites, SPRITE_BATTLE_MENARDI
	sprite_source GhostHeadSprites, SPRITE_BATTLE_TRET
	sprite_source FusionDragonSprites, SPRITE_BATTLE_FUSION_DRAGON
	sprite_source PurpleBeastSprites, SPRITE_BATTLE_FUSION_DRAGON_RIGHT
	sprite_source RedWarriorSprites, SPRITE_BATTLE_WARRIOR_IN_GOLDEN_HELMET
	sprite_source SwordGirlSprites, SPRITE_BATTLE_BANDIT
	.2byte 0, 0 @ the end
