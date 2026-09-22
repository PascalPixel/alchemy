.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.global Overlay_080c9000
Overlay_080c9000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xd409
	.2byte 0x080e
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x3a3d
	.2byte 0x080e
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x6579
	.2byte 0x080d
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x6661
	.2byte 0x080d
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xb7f9
	.2byte 0x080c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xbc0d
	.2byte 0x080c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc5d9
	.2byte 0x080c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xcaed
	.2byte 0x080c
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xcbdd
	.2byte 0x080c
	push	{lr}
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #48]
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #48]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #32]
	ldr	r1, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [pc, #44]
	ldrh	r3, [r0, #0]
	b.n	.L_080c90ac
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0000100e
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.4byte 0x04000050
	.4byte 0x04000040
	.4byte 0x04000048
	.4byte 0x02002090
	.2byte 0x0208
	.2byte 0x0400
.L_080c90ac:
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_080c90d4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #28]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080c90d4:
	strh	r4, [r0, #0]
	movs	r0, #1
	bl	sub_080030f8
	pop	{r0}
	bx	r0
	.4byte 0x00007741
