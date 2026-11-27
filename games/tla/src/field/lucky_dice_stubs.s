@ Lucky Dice's entry point.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global Stubs_LuckyDice
Stubs_LuckyDice:
	stub fdLuckyDiceStartStub, fdLuckyDiceStart
