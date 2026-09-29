.syntax unified
	.thumb
	.global Resource_FarCall010
Resource_FarCall010:
	.global Func_081c0000
	.thumb_func
Func_081c0000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x1fb5
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0c1d
	.2byte 0x081c
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0cb1
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0f3d
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0f45
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0f59
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0f71
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x081c0f85
