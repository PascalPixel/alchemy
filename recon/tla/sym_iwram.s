@ IWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
@ The heap's slot words open IWRAM, below the resident modules.
	.section .sym_iwram_slots,"aw",%nobits
	.global gHeapSlots
gHeapSlots:
	.space 0x00000070
	.section .sym_iwram,"aw",%nobits
	.space 0x000000b0
	.global gInput
gInput:
	.space 0x00000024
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
