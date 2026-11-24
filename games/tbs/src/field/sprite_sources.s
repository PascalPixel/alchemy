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
	sprite_source AxeGoblinSprites, SPRITE_BATTLE_TROLL
	sprite_source GhoulSprites, SPRITE_BATTLE_ZOMBIE
	sprite_source HarpySprites, SPRITE_BATTLE_HARPY
	sprite_source TrollSprites, SPRITE_BATTLE_GOLEM
	sprite_source WitchSprites, SPRITE_BATTLE_GHOST
	sprite_source GriffinSprites, SPRITE_BATTLE_GRYPHON
	sprite_source ShieldKnightSprites, SPRITE_BATTLE_RAT_SOLDIER
	sprite_source OrcSprites, SPRITE_BATTLE_GHOUL
	sprite_source ChimeraSprites, SPRITE_BATTLE_CHIMERA
	sprite_source LivingArmorSprites, SPRITE_BATTLE_DEADBEARD
	sprite_source ManticoreSprites, SPRITE_BATTLE_MANTICORE
	sprite_source OrcLordSprites, SPRITE_BATTLE_KOBOLD
	sprite_source GargoyleSprites, SPRITE_BATTLE_GARGOYLE
	sprite_source LizardManSprites, SPRITE_BATTLE_LIZARD_MAN
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
	sprite_source FairySprites, SPRITE_BATTLE_CUTTLE
	sprite_source BatSprites, SPRITE_BATTLE_BAT
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
