@ The Lucky Wheels entry point.
@ C2: aligned 8-byte far-call stubs: export and overlay_veneer emit ldr r4, bx r4 and a target word.
#include "stub.inc"
	.global FarCall_LuckyWheelsTable
FarCall_LuckyWheelsTable:
	export LuckyWheels_StartFar, LuckyWheels_Start
