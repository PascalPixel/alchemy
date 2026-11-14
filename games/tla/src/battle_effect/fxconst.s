	.section .rodata
@ The constants the effects copy into their locals, placed after the effect
@ code: {x, y} scales in 16.16 fixed point, points and offsets.
	.global gCruelRuin_UnitScale
gCruelRuin_UnitScale:
	.4byte 0x10000, 0x10000
	.global gEclipse_Scales
gEclipse_Scales:
	.4byte 0x10000, 0x10000, 0x55555556, 0x55555556
	.4byte 0x55555556, 0x55555556
	.global gProcne_UnitScale
gProcne_UnitScale:
	.4byte 0x10000, 0x10000, 0x71c71c8, 0x2d82d83
	.4byte 0x2d82d83
	.global gZagan_UnitScale
gZagan_UnitScale:
	.4byte 0x10000, 0x10000
	.global gBoreas_UnitScale
gBoreas_UnitScale:
	.4byte 0x10000, 0x10000
	.global gRockWall_Heights
gRockWall_Heights:
	.4byte -32, -32, -32, -32
	.global gThor_Points
gThor_Points:
	.2byte 8, 135, 184, 135, 128, 135, 96, 32
	.2byte 96, 32, 64, 135
	.global gGaia_UnitScale
gGaia_UnitScale:
	.4byte 0x10000, 0x10000
	.global gUlysses_ParticleScale
gUlysses_ParticleScale:
	.4byte 0x10000, 0x10000
	.global gUlysses_ShipScale
gUlysses_ShipScale:
	.4byte 0x10000, 0x10000
	.global gFormationObjects_Scale
gFormationObjects_Scale:
	.4byte 0x10000, 0x10000
	.global gFormationObjects_LargeScale
gFormationObjects_LargeScale:
	.4byte 0x14000, 0x14000
	.global gRamses_UnitScale
gRamses_UnitScale:
	.4byte 0x10000, 0x10000
	.global gJudgment_UnitScale
gJudgment_UnitScale:
	.4byte 0x10000, 0x10000
	.global gKirin_RowScale
gKirin_RowScale:
	.4byte 0x10000, 0x10000
	.global gKirin_ChargeScale
gKirin_ChargeScale:
	.4byte 0x10000, 0x10000
	.global gNeptune_UnitScale
gNeptune_UnitScale:
	.4byte 0x18000, 0x18000
	.global gMegaera_UnitScale
gMegaera_UnitScale:
	.4byte 0x18000, 0x18000
	.global gVoidBeam_Corners
gVoidBeam_Corners:
	.byte 40, -40, 0, 40, -40, -40, 40, 40, 0, -40, -40, 40
	.global gCharon_SwarmScale
gCharon_SwarmScale:
	.4byte 0x10000, 0x10000
	.global gCharon_PulseScale
gCharon_PulseScale:
	.4byte 0x10000, 0x10000
	.global gCharon_RevealScale
gCharon_RevealScale:
	.4byte 0x10000, 0x10000
	.global gCharon_FinaleScale
gCharon_FinaleScale:
	.4byte 0x10000, 0x10000
	.global gFlora_UnitScale
gFlora_UnitScale:
	.4byte 0x10000, 0x10000
	.global gDaedalus_WaveScale
gDaedalus_WaveScale:
	.4byte 0x10000, 0x10000
	.global gDaedalus_ShipScale
gDaedalus_ShipScale:
	.4byte 0x10000, 0x10000
	.global gDaedalus_ParticleScale
gDaedalus_ParticleScale:
	.4byte 0x11999, 0x11999
	.global gDaedalusAlt_SummonScale
gDaedalusAlt_SummonScale:
	.4byte 0x20000, 0x20000
	.global gCoatlicue_UnitScale
gCoatlicue_UnitScale:
	.4byte 0x10000, 0x10000
	.global gCatastrophe_UnitScale
gCatastrophe_UnitScale:
	.4byte 0x10000, 0x10000
	.global gHaures_ChargeScale
gHaures_ChargeScale:
	.4byte 0x10000, 0x10000
	.global gHaures_OrbitScale
gHaures_OrbitScale:
	.4byte 0x10000, 0x10000
	.global gHaures_OpeningScale
gHaures_OpeningScale:
	.4byte 0x10000, 0x10000
	.global gTrident_UnitScale
gTrident_UnitScale:
	.4byte 0x10000, 0x10000
	.global gMeteorBlow_DoubleScale
gMeteorBlow_DoubleScale:
	.4byte 0x20000, 0x20000
	.global gMoloch_MoteSizes
gMoloch_MoteSizes:
	.byte 2, 3, 5, 8, 7, 6, 4, 0
	.global gIris_PillarVertices
gIris_PillarVertices:
	.4byte 0xf0f0, 0xf010, 3312, 3088
	.4byte 240, 16
	.global gIris_UnitScale
gIris_UnitScale:
	.4byte 0x10000, 0x10000
	.global gIris_RisingPillarWidths
gIris_RisingPillarWidths:
	.4byte 2, 3, 4, 5
	.4byte 6
	.global gIris_WideningPillarWidths
gIris_WideningPillarWidths:
	.4byte 2, 3, 4, 5
	.4byte 6, 7, 8
	.global gIris_NarrowingPillarWidths
gIris_NarrowingPillarWidths:
	.4byte 8, 7, 6, 5
	.4byte 4, 3, 2
	.global gIris_LeftStripSpans
gIris_LeftStripSpans:
	.4byte 112, 27, 104, 28
	.4byte 92, 27, 74, 27
	.global gIris_RightStripSpans
gIris_RightStripSpans:
	.4byte 196, 44, 176, 48
	.4byte 152, 45, 116, 45
