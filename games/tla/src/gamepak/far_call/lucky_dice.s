@ Lucky Dice's entry point.
	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global FarCall_LuckyDiceTable
FarCall_LuckyDiceTable:
	export LuckyDice_StartFar, LuckyDice_Start
