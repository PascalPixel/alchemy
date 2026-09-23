.syntax unified
	.thumb
	.global Func_0814355c
	.thumb_func
Func_0814355c:
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #0
	adds	r3, #40
	str	r1, [r3, #0]
	ldr	r3, [pc, #44]
	subs	r2, #38
	str	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #12
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #34
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	ldr	r3, [pc, #16]
	adds	r2, #6
	b.n	.L_081435a8
	.4byte 0x00003f44
	.4byte 0x0000100e
	.4byte 0x00000080
	.4byte 0x00000100
	.2byte 0xf000
	.2byte 0xffff
.L_081435a8:
	strh	r3, [r2, #0]
	ldr	r1, [pc, #36]
	movs	r3, #128
	ldr	r2, [pc, #36]
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #74
	strh	r3, [r2, #0]
	b.n	.L_081435dc
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x00001088
	.2byte 0x3f21
	.2byte 0x0000
.L_081435dc:
	bx	lr
	.align 2, 0
