@ IWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
@ The heap's slot words open IWRAM, below the resident modules.
	.section .sym_iwram_slots,"aw",%nobits
	.global gHeapSlots
gHeapSlots:
	.space 0x00000070
	.section .sym_iwram,"aw",%nobits
	.space 0x00000060
	.global gFrameTick
gFrameTick:
	.space 0x00000008
	.global Data_03001108
Data_03001108:
	.space 0x00000004
	.global Data_0300110c
Data_0300110c:
	.space 0x00000004
	.global Data_03001110
Data_03001110:
	.space 0x00000010
	.global Data_03001120
Data_03001120:
	.space 0x00000018
	.global Data_03001138
Data_03001138:
	.space 0x00000004
	.global Data_0300113c
Data_0300113c:
	.space 0x00000004
	.global Data_03001140
Data_03001140:
	.space 0x00000004
	.global Data_03001144
Data_03001144:
	.space 0x0000000c
	.global gInput
gInput:
	.space 0x00000024
	.global gDecodeFillByte
gDecodeFillByte:
	.space 0x00000004
	.global Data_03001178
Data_03001178:
	.space 0x00000004
	.global Data_0300117c
Data_0300117c:
	.space 0x00000004
	.global Data_03001180
Data_03001180:
	.space 0x00000004
	.global Data_03001184
Data_03001184:
	.space 0x0000000c
	.global gOamBucketMasks
gOamBucketMasks:
	.space 0x00000020
	.global Data_030011b0
Data_030011b0:
	.space 0x00000008
	.global Data_030011b8
Data_030011b8:
	.space 0x00000004
	.global Data_030011bc
Data_030011bc:
	.space 0x00000004
	.global Data_030011c0
Data_030011c0:
	.space 0x00000004
	.global Data_030011c4
Data_030011c4:
	.space 0x00000008
	.global Data_030011cc
Data_030011cc:
	.space 0x00000004
	.global Data_030011d0
Data_030011d0:
	.space 0x00000004
	.global Data_030011d4
Data_030011d4:
	.space 0x00000004
	.global Data_030011d8
Data_030011d8:
	.space 0x00000004
	.global Data_030011dc
Data_030011dc:
	.space 0x00000004
	.global gCameraSceneParameters
gCameraSceneParameters:
	.space 0x0000000c
	.global Data_030011ec
Data_030011ec:
	.space 0x00000004
	.global Data_030011f0
Data_030011f0:
	.space 0x00000004
	.global Data_030011f4
Data_030011f4:
	.space 0x00000004
	.global Data_030011f8
Data_030011f8:
	.space 0x00000004
	.global gObjAffineCount
gObjAffineCount:
	.space 0x00000004
	.global Data_03001200
Data_03001200:
	.space 0x0000000c
	.global Data_0300120c
Data_0300120c:
	.space 0x00000008
	.global Data_03001214
Data_03001214:
	.space 0x00000004
	.global Data_03001218
Data_03001218:
	.space 0x00000004
	.global Data_0300121c
Data_0300121c:
	.space 0x00000004
	.global Data_03001220
Data_03001220:
	.space 0x00000008
	.global Data_03001228
Data_03001228:
	.space 0x00000004
	.global Data_0300122c
Data_0300122c:
	.space 0x00000004
	.global Data_03001230
Data_03001230:
	.space 0x00000008
	.global Data_03001238
Data_03001238:
	.space 0x00000004
	.global Data_0300123c
Data_0300123c:
	.space 0x00000008
	.global Data_03001244
Data_03001244:
	.space 0x00000004
	.global gOamUsage
gOamUsage:
	.space 0x00000004
	.global Data_0300124c
Data_0300124c:
	.space 0x00000004
	.global Data_03001250
Data_03001250:
	.space 0x00000008
	.global Data_03001258
Data_03001258:
	.space 0x00000002
	.global Data_0300125a
Data_0300125a:
	.space 0x000000a6
	.global Data_03001300
Data_03001300:
