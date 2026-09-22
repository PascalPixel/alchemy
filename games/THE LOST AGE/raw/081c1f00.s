.syntax unified
	.thumb
	.global Overlay_081c1f00
Overlay_081c1f00:
	adds	r2, r0, #0
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #12]
	cmp	r3, r0
	bne.n	.L_081c1f12
	ldr	r0, [r2, #4]
	ldr	r1, [pc, #8]
	ands	r0, r1
	str	r0, [r2, #4]
.L_081c1f12:
	bx	lr
	.4byte 0x68736d53
	.2byte 0xffff
	.2byte 0x7fff
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #16]
	cmp	r3, r0
	bne.n	.L_081c1f34
	strh	r1, [r2, #38]
	strh	r1, [r2, #36]
	movs	r0, #128
	lsls	r0, r0, #1
	strh	r0, [r2, #40]
.L_081c1f34:
	bx	lr
	movs	r0, r0
	.4byte 0x68736d53
