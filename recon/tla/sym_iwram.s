@ IWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_iwram,"aw",%nobits
	.space 0x000000d4
	.global gDecodeFillByte
gDecodeFillByte:
	.space 0x0000001c
	.global gOamBucketMasks
gOamBucketMasks:
	.space 0x00000050
	.global gCameraSceneParameters
gCameraSceneParameters:
	.space 0x0000001c
	.global gObjAffineCount
gObjAffineCount:
	.space 0x0000004c
	.global gOamUsage
gOamUsage:
	.space 0x00000004
