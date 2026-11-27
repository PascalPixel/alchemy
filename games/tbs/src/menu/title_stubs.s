@ The title, the introduction and the publisher's and Camelot's logos.
	.include "games/tbs/src/system/stub_table.inc"
	.global StubsTitle
StubsTitle:
	stub mnTitleShowTitleScreenStub, mnTitleShowTitleScreen
	unnamed_stub mnTitleAfterIntroNoOperation
	unnamed_stub mnTitleBeforeLogosNoOperation
	stub mnTitleShowPublisherLogoStub, mnTitleShowPublisherLogo
	stub mnTitleShowCamelotLogoStub, mnTitleShowCamelotLogo
