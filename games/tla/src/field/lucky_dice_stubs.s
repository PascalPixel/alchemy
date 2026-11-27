@ Lucky Dice's entry point.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global StubsLuckyDice
StubsLuckyDice:
	stub fdLuckyDiceStartStub, fdLuckyDiceStart
