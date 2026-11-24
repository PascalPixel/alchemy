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
	sprite_source BlueDevilSprites, 526
	sprite_source PinkBruteSprites, 527
	sprite_source ClubTrollSprites, 528
	sprite_source JesterImpSprites, 529
	sprite_source GoldDragonSprites, 530
	sprite_source ApeSprites, 531
	sprite_source SwordImpSprites, 532
	sprite_source RedBeastSprites, 533
	sprite_source RedBeetleSprites, 534
	sprite_source PinkWormSprites, 535
	sprite_source BanditSprites, 536
	sprite_source SeaDragonSprites, 537
	sprite_source SkullMageSprites, 539
	sprite_source ManEaterSprites, 540
	sprite_source BlueCrabSprites, 541
	sprite_source OstrichSprites, 542
	sprite_source BlueWyvernSprites, 543
	sprite_source DarkWizardSprites, 544
	sprite_source WhiteKnightSprites, 545
	sprite_source GoldenBirdSprites, 546
	sprite_source DarkKnightSprites, 547
	sprite_source TreantSprites, 548
	sprite_source GreenOgreSprites, 549
	sprite_source PinkOgreSprites, 550
	sprite_source WerewolfSprites, 551
	sprite_source MummySprites, 552
	sprite_source RaptorSprites, 553
	sprite_source GreenKnightSprites, 554
	sprite_source WyvernSprites, 555
	sprite_source HydraSprites, 556
	sprite_source LarvaSprites, 557
	sprite_source JackalSprites, 558
	sprite_source UrchinSprites, 559
	sprite_source DemonWarriorSprites, 560
	sprite_source DragonLeftSprites, 561
	sprite_source DragonRightSprites, 562
	sprite_source SeaSerpentSprites, 563
	sprite_source SwordsmanSprites, 564
	sprite_source StorkSprites, 565
	sprite_source VultureSprites, 566
	sprite_source ToadSprites, 567
	sprite_source SwordMaidenSprites, 568
	sprite_source BeastLordSprites, 569
	sprite_source BrawlerSprites, 570
	sprite_source MiniKnightSprites, 571
	sprite_source ScorpionSprites, 572
	sprite_source MermanSprites, 573
	sprite_source YoungSwordsmanSprites, 574
	sprite_source GreenMageSprites, 575
	sprite_source BlueOrbSprites, 576
	sprite_source AxeWarriorSprites, 577
	sprite_source SpearWarriorSprites, 578
	sprite_source AxeGoblinSprites, 587
	sprite_source GhoulSprites, 588
	sprite_source HarpySprites, 589
	sprite_source TrollSprites, 590
	sprite_source WitchSprites, 591
	sprite_source GriffinSprites, 592
	sprite_source ShieldKnightSprites, 593
	sprite_source OrcSprites, 594
	sprite_source ChimeraSprites, 595
	sprite_source ManticoreSprites, 596
	sprite_source OrcLordSprites, 597
	sprite_source GargoyleSprites, 598
	sprite_source OgreSprites, 599
	sprite_source TailedImpSprites, 600
	sprite_source WispGhostSprites, 601
	sprite_source SalamanderSprites, 602
	sprite_source AquaBeastSprites, 603
	sprite_source HoundSprites, 604
	sprite_source BearSprites, 605
	sprite_source SirenSprites, 606
	sprite_source BlueWispSprites, 607
	sprite_source MushroomSprites, 608
	sprite_source RedWormSprites, 609
	sprite_source BatSprites, 610
	sprite_source JellySprites, 611
	sprite_source SmallLizardSprites, 612
	sprite_source WalkingPlantSprites, 613
	sprite_source FlySprites, 614
	sprite_source CrabSprites, 615
	sprite_source SmallBirdSprites, 616
	sprite_source MothSprites, 617
	sprite_source FrogSprites, 618
	sprite_source ManedLionSprites, 619
	sprite_source SerpentSprites, 620
	sprite_source WingedDragonSprites, 621
	sprite_source BlueMageSprites, 622
	sprite_source SkeletonSprites, 623
	sprite_source FlameSkullSprites, 624
	sprite_source FireDragonSprites, 625
	sprite_source CrawlerSprites, 626
	sprite_source MimicSprites, 627
	sprite_source SeaBeastSprites, 628
	sprite_source SpiritHorseSprites, 629
	sprite_source LizardManSprites, 630
	sprite_source KrakenSprites, 631
	sprite_source SlimeSprites, 632
	sprite_source SaturosSprites, 633
	sprite_source MenardiSprites, 634
	sprite_source GhostHeadSprites, 636
	sprite_source FusionDragonSprites, 638
	sprite_source PurpleBeastSprites, 639
	sprite_source RedWarriorSprites, 640
	sprite_source SwordGirlSprites, 641
	.2byte 0, 0 @ the end
