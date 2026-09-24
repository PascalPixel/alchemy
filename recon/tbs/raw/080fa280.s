.syntax unified
	.thumb
	.global Func_080fa280
	.thumb_func
Func_080fa280:
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #16]
	cmp	r3, r0
	bne.n	.L_080fa298
	strh	r1, [r2, #38]
	strh	r1, [r2, #36]
	movs	r0, #128
	lsls	r0, r0, #1
	strh	r0, [r2, #40]
.L_080fa298:
	bx	lr
	movs	r0, r0
	.4byte 0x68736d53
