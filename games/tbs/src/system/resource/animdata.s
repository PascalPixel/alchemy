	.section .rodata
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "system/resource/shadow.8bpp"

@ 敵スプライトの資源番号と敵番号
	.global ResourceSlot_NumberTable
ResourceSlot_NumberTable:
	.2byte ResourceId_AxeGoblinSprites, 400
	.2byte ResourceId_GhoulSprites, 401
	.2byte ResourceId_HarpySprites, 402
	.2byte ResourceId_TrollSprites, 403
	.2byte ResourceId_WitchSprites, 404
	.2byte ResourceId_GriffinSprites, 408
	.2byte ResourceId_ShieldKnightSprites, 409
	.2byte ResourceId_OrcSprites, 410
	.2byte ResourceId_ChimeraSprites, 411
	.2byte ResourceId_LivingArmorSprites, 415
	.2byte ResourceId_ManticoreSprites, 417
	.2byte ResourceId_OrcLordSprites, 421
	.2byte ResourceId_GargoyleSprites, 424
	.2byte ResourceId_LizardManSprites, 429
	.2byte ResourceId_OgreSprites, 430
	.2byte ResourceId_TailedImpSprites, 433
	.2byte ResourceId_WispGhostSprites, 434
	.2byte ResourceId_SalamanderSprites, 435
	.2byte ResourceId_AquaBeastSprites, 436
	.2byte ResourceId_HoundSprites, 448
	.2byte ResourceId_BearSprites, 450
	.2byte ResourceId_SirenSprites, 451
	.2byte ResourceId_BlueWispSprites, 453
	.2byte ResourceId_MushroomSprites, 454
	.2byte ResourceId_RedWormSprites, 455
	.2byte ResourceId_FairySprites, 456
	.2byte ResourceId_BatSprites, 457
	.2byte ResourceId_SmallLizardSprites, 458
	.2byte ResourceId_WalkingPlantSprites, 459
	.2byte ResourceId_FlySprites, 460
	.2byte ResourceId_CrabSprites, 461
	.2byte ResourceId_SmallBirdSprites, 462
	.2byte ResourceId_MothSprites, 463
	.2byte ResourceId_FrogSprites, 464
	.2byte ResourceId_ManedLionSprites, 465
	.2byte ResourceId_SerpentSprites, 466
	.2byte ResourceId_WingedDragonSprites, 467
	.2byte ResourceId_BlueMageSprites, 468
	.2byte ResourceId_SkeletonSprites, 469
	.2byte ResourceId_FlameSkullSprites, 470
	.2byte ResourceId_FireDragonSprites, 471
	.2byte ResourceId_CrawlerSprites, 472
	.2byte ResourceId_MimicSprites, 473
	.2byte ResourceId_SeaBeastSprites, 474
	.2byte ResourceId_SpiritHorseSprites, 475
	.2byte ResourceId_KrakenSprites, 476
	.2byte ResourceId_SlimeSprites, 477
	.2byte ResourceId_SaturosSprites, 478
	.2byte ResourceId_MenardiSprites, 479
	.2byte ResourceId_GhostHeadSprites, 481
	.2byte ResourceId_FusionDragonSprites, 483
	.2byte ResourceId_PurpleBeastSprites, 484
	.2byte ResourceId_RedWarriorSprites, 485
	.2byte ResourceId_SwordGirlSprites, 486
	.2byte 0, 0
