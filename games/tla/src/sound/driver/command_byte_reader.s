.syntax unified
	.thumb
	.global ld_r3_tp_adr_i_unchecked
	.thumb_func
ld_r3_tp_adr_i_unchecked:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
	.2byte	0x0000
