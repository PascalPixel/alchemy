@ The Lucky Wheels entry point.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_LuckyWheelsTable
FarCall_LuckyWheelsTable:
	export LuckyWheels_StartFar, LuckyWheels_Start
