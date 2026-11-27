@ The title, the introduction and the publisher's and Camelot's logos.
	.include "games/tbs/src/system/stub_table.inc"
	.global Stubs_Title
Stubs_Title:
	stub mnTitleShowTitleScrStub, mnTitleShowTitleScr
	unnamed_stub mnTitleAfterIntroNoOp
	unnamed_stub mnTitleBeforeLogosNoOp
	stub mnTitleShowPublisherLogoStub, mnTitleShowPublisherLogo
	stub mnTitleShowCamelotLogoStub, mnTitleShowCamelotLogo
