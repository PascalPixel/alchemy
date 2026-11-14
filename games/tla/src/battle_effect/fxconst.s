	.section .rodata
@ The constants the effect code copies into its locals, placed after its code
@ as The Broken Seal's effect/effect.c places them: {x, y} scales and
@ speeds in 16.16 fixed point, points and offsets.
@ read by BattleEffect_RunCruelRuin
	.global gCruelRuin_UnitScale
gCruelRuin_UnitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunEclipse
	.global gEclipse_Scales
gEclipse_Scales:
	.4byte 0x10000, 0x10000, 0x55555556, 0x55555556
	.4byte 0x55555556, 0x55555556
@ read by BattleEffect_RunProcne
	.global gProcne_UnitScale
gProcne_UnitScale:
	.4byte 0x10000, 0x10000, 0x71c71c8, 0x2d82d83
	.4byte 0x2d82d83
@ read by BattleEffect_RunZagan
	.global gZagan_UnitScale
gZagan_UnitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunBoreas
	.global gBoreas_UnitScale
gBoreas_UnitScale:
	.4byte 0x10000, 0x10000
@ The Broken Seal's wallHeights in BattleEffect_RunQuake; read by BattleEffect_RunQuake
	.global gRockWall_Heights
gRockWall_Heights:
	.4byte -32, -32, -32, -32
@ The Broken Seal's points in BattleEffect_RunThor; read by BattleEffect_RunThor
	.global gThor_Points
gThor_Points:
	.2byte 8, 135, 184, 135, 128, 135, 96, 32
	.2byte 96, 32, 64, 135
@ read by BattleEffect_RunGaia
	.global gGaia_UnitScale
gGaia_UnitScale:
	.4byte 0x10000, 0x10000
@ read by Ulysses_MoveLastParticle
	.global gUlysses_ParticleScale
gUlysses_ParticleScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunUlysses
	.global gUlysses_ShipScale
gUlysses_ShipScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_PlaceFormationObjects
	.global gFormationObjects_Scale
gFormationObjects_Scale:
	.4byte 0x10000, 0x10000
@ The Broken Seal's smallScale; read by BattleEffect_PlaceFormationObjects
	.global gFormationObjects_LargeScale
gFormationObjects_LargeScale:
	.4byte 0x14000, 0x14000
@ read by BattleEffect_RunRamses
	.global gRamses_UnitScale
gRamses_UnitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunJudgment
	.global gJudgment_UnitScale
gJudgment_UnitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunKirin
	.global gKirin_RowScale
gKirin_RowScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunKirin
	.global gKirin_ChargeScale
gKirin_ChargeScale:
	.4byte 0x10000, 0x10000
@ The Broken Seal's unitScale in BattleEffect_RunNeptune; read by BattleEffect_RunNeptune
	.global gNeptune_UnitScale
gNeptune_UnitScale:
	.4byte 0x18000, 0x18000
@ The Broken Seal's unitScale in BattleEffect_RunNeptune; read by BattleEffect_RunMegaera
	.global gMegaera_UnitScale
gMegaera_UnitScale:
	.4byte 0x18000, 0x18000
@ read by BattleEffect_RunVoidBeam
	.global gVoidBeam_Corners
gVoidBeam_Corners:
	.byte 40, -40, 0, 40, -40, -40, 40, 40, 0, -40, -40, 40
@ read by BattleEffect_RunCharon
	.global gCharon_SwarmScale
gCharon_SwarmScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunCharon
	.global gCharon_PulseScale
gCharon_PulseScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunCharon
	.global gCharon_RevealScale
gCharon_RevealScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunCharon
	.global gCharon_FinaleScale
gCharon_FinaleScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunFlora
	.global gFlora_UnitScale
gFlora_UnitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunDaedalus
	.global gDaedalus_WaveScale
gDaedalus_WaveScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunDaedalus
	.global gDaedalus_ShipScale
gDaedalus_ShipScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunDaedalus
	.global gDaedalus_ParticleScale
gDaedalus_ParticleScale:
	.4byte 0x11999, 0x11999
@ read by BattleEffect_RunDaedalusAlt
	.global gDaedalusAlt_SummonScale
gDaedalusAlt_SummonScale:
	.4byte 0x20000, 0x20000
@ read by BattleEffect_RunCoatlicue
	.global gCoatlicue_UnitScale
gCoatlicue_UnitScale:
	.4byte 0x10000, 0x10000
@ read by Func_081848ec
	.global gCatastrophe_UnitScale
gCatastrophe_UnitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunHaures
	.global gHaures_ChargeScale
gHaures_ChargeScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunHaures
	.global gHaures_OrbitScale
gHaures_OrbitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunHaures
	.global gHaures_OpeningScale
gHaures_OpeningScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunTrident
	.global gTrident_UnitScale
gTrident_UnitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunMeteorBlow
	.global gMeteorBlow_DoubleScale
gMeteorBlow_DoubleScale:
	.4byte 0x20000, 0x20000
@ read by BattleEffect_RunMoloch
	.global gMoloch_MoteSizes
gMoloch_MoteSizes:
	.byte 2, 3, 5, 8, 7, 6, 4, 0
@ read by Iris_DrawPillar
	.global gIris_PillarVertices
gIris_PillarVertices:
	.4byte 0xf0f0, 0xf010, 3312, 3088
	.4byte 240, 16
@ read by BattleEffect_RunIris
	.global gIris_UnitScale
gIris_UnitScale:
	.4byte 0x10000, 0x10000
@ read by BattleEffect_RunIris
	.global gIris_RisingPillarWidths
gIris_RisingPillarWidths:
	.4byte 2, 3, 4, 5
	.4byte 6
@ read by BattleEffect_RunIris
	.global gIris_WideningPillarWidths
gIris_WideningPillarWidths:
	.4byte 2, 3, 4, 5
	.4byte 6, 7, 8
@ read by BattleEffect_RunIris
	.global gIris_NarrowingPillarWidths
gIris_NarrowingPillarWidths:
	.4byte 8, 7, 6, 5
	.4byte 4, 3, 2
@ read by BattleEffect_RunIris
	.global gIris_LeftStripSpans
gIris_LeftStripSpans:
	.4byte 112, 27, 104, 28
	.4byte 92, 27, 74, 27
@ read by BattleEffect_RunIris
	.global gIris_RightStripSpans
gIris_RightStripSpans:
	.4byte 196, 44, 176, 48
	.4byte 152, 45, 116, 45
