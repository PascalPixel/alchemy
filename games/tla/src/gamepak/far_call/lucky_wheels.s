@ The Lucky Wheels entry point.
	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global FarCall_LuckyWheelsTable
FarCall_LuckyWheelsTable:
	export LuckyWheels_StartFar, LuckyWheels_Start
