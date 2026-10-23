@ The reel-game entry point.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_BlankLoadTable
FarCall_BlankLoadTable:
	export LuckyWheels_StartFar, LuckyWheels_Start
