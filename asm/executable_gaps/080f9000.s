.syntax unified
.text
	.thumb
	.global Fragment_080f9000
	.thumb_func
Fragment_080f9000:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080fa319
