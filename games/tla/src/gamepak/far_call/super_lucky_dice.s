@ Super Lucky Dice's entry point, a bank The Broken Seal does not have.
	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global FarCall_SuperLuckyDiceTable
FarCall_SuperLuckyDiceTable:
	export SuperLuckyDice_StartFar, SuperLuckyDice_Start
