@ The title, the introduction and the publisher's and Camelot's logos.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global Stubs_Title
Stubs_Title:
	stub mnTitleShowTitleScreenStub, mnTitleShowTitleScreen
	stub mnTitleAfterIntroNoOperationStub, mnTitleAfterIntroNoOperation
	stub mnTitleBeforeLogosNoOperationStub, mnTitleBeforeLogosNoOperation
	stub mnTitleShowPublisherLogoStub, mnTitleShowPublisherLogo
	stub mnTitleShowCamelotLogoStub, mnTitleShowCamelotLogo
	stub mnTitleIntroRunStub, mnTitleIntroRun
