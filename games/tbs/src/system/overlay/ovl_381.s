	.section .bss,"aw",%nobits
	.space 8
	.global gEmberState
gEmberState:
	.space 64
	.global gEmberTimer
gEmberTimer:
	.space 16
	.global gArcEffects
gArcEffects:
	.space 48
	.global gArcEffectTimers
gArcEffectTimers:
	.space 40
	.global gEmberMask
gEmberMask:
	.space 4
	.global gEmberLevel
gEmberLevel:
	.space 4
	.global gEmberLevelTimer
gEmberLevelTimer:
	.space 4
