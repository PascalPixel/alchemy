@ Lucky Dice's entry point.
@ C2: aligned 8-byte far-call stubs: export and overlay_veneer emit ldr r4, bx r4 and a target word.
#include "stub.inc"
	.global FarCall_LuckyDiceTable
FarCall_LuckyDiceTable:
	export LuckyDice_StartFar, LuckyDice_Start
