.syntax unified
.text
	.thumb
	.global Fragment_0801e738
	.thumb_func
Fragment_0801e738:
	ldr	r3, [pc, #8]
	ldr	r2, [pc, #12]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	strh	r0, [r3, #0]
	bx	lr
	.4byte 0x03001e8c
	.4byte 0x00000eac
