@ The packed resource that holds each animation number's pictures
@ (struct SpritePictureCacheSource), which the sprite picture cache decodes.
@ Each row: the resource and the animation number; number 0 ends the table.
	.macro sprite_source resource:req, number:req
	.2byte ResourceId_\resource, \number
	.endm

	.section .rodata
	.balign 4
	.global gSpritePictureCache_Sources
gSpritePictureCache_Sources:
	sprite_source AxeGoblinSprites, 400
	sprite_source GhoulSprites, 401
	sprite_source HarpySprites, 402
	sprite_source TrollSprites, 403
	sprite_source WitchSprites, 404
	sprite_source GriffinSprites, 408
	sprite_source ShieldKnightSprites, 409
	sprite_source OrcSprites, 410
	sprite_source ChimeraSprites, 411
	sprite_source LivingArmorSprites, 415
	sprite_source ManticoreSprites, 417
	sprite_source OrcLordSprites, 421
	sprite_source GargoyleSprites, 424
	sprite_source LizardManSprites, 429
	sprite_source OgreSprites, 430
	sprite_source TailedImpSprites, 433
	sprite_source WispGhostSprites, 434
	sprite_source SalamanderSprites, 435
	sprite_source AquaBeastSprites, 436
	sprite_source HoundSprites, 448
	sprite_source BearSprites, 450
	sprite_source SirenSprites, 451
	sprite_source BlueWispSprites, 453
	sprite_source MushroomSprites, 454
	sprite_source RedWormSprites, 455
	sprite_source FairySprites, 456
	sprite_source BatSprites, 457
	sprite_source SmallLizardSprites, 458
	sprite_source WalkingPlantSprites, 459
	sprite_source FlySprites, 460
	sprite_source CrabSprites, 461
	sprite_source SmallBirdSprites, 462
	sprite_source MothSprites, 463
	sprite_source FrogSprites, 464
	sprite_source ManedLionSprites, 465
	sprite_source SerpentSprites, 466
	sprite_source WingedDragonSprites, 467
	sprite_source BlueMageSprites, 468
	sprite_source SkeletonSprites, 469
	sprite_source FlameSkullSprites, 470
	sprite_source FireDragonSprites, 471
	sprite_source CrawlerSprites, 472
	sprite_source MimicSprites, 473
	sprite_source SeaBeastSprites, 474
	sprite_source SpiritHorseSprites, 475
	sprite_source KrakenSprites, 476
	sprite_source SlimeSprites, 477
	sprite_source SaturosSprites, 478
	sprite_source MenardiSprites, 479
	sprite_source GhostHeadSprites, 481
	sprite_source FusionDragonSprites, 483
	sprite_source PurpleBeastSprites, 484
	sprite_source RedWarriorSprites, 485
	sprite_source SwordGirlSprites, 486
	.2byte 0, 0 @ the end
