@ The display-blanked callback runner.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_BlankRunTable
FarCall_BlankRunTable:
	export LuckyDice_StartFar, LuckyDice_Start
