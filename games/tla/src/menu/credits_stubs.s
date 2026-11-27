@ The credits' entry point.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global StubsCredits
StubsCredits:
	stub mnCreditsRunStub, mnCreditsRun
	stub mnEndingImageStartStub, mnEndingImageStart
	stub mnEndingImageShowStub, mnEndingImageShow
	stub mnEndingImageSetTargetRowStub, mnEndingImageSetTargetRow
	stub mnEndingImageWaitForScrollStub, mnEndingImageWaitForScroll
	stub mnEndingImageLowerTopStripStub, mnEndingImageLowerTopStrip
