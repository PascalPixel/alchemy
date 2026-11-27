@ The title, the introduction and the publisher's and Camelot's logos.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global Stubs_Title
Stubs_Title:
	stub mnTitleShowTitleScrStub, mnTitleShowTitleScr
	stub mnTitleAfterIntroNoOpStub, mnTitleAfterIntroNoOp
	stub mnTitleBeforeLogosNoOpStub, mnTitleBeforeLogosNoOp
	stub mnTitleShowPublisherLogoStub, mnTitleShowPublisherLogo
	stub mnTitleShowCamelotLogoStub, mnTitleShowCamelotLogo
	stub mnTitleIntroRunStub, mnTitleIntroRun
