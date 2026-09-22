.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.global Overlay_081c11ec
Overlay_081c11ec:
	push	{r5, lr}
	ldr	r3, [pc, #116]
	lsrs	r0, r0, #16
	ldr	r2, [r3, #0]
	movs	r3, #15
	ands	r0, r3
	movs	r3, #3
	strb	r3, [r2, #11]
	movs	r3, #246
	lsls	r3, r3, #7
	adds	r3, #48
	str	r3, [r2, #20]
	movs	r1, #132
	movs	r3, #133
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	strb	r0, [r2, #8]
	str	r1, [r2, #16]
	str	r3, [r2, #24]
	movs	r2, #0
	ldr	r3, [pc, #80]
	strh	r2, [r3, #0]
	ldr	r0, [pc, #80]
	bl	sub_08002054
	ldr	r3, [pc, #80]
	strh	r0, [r3, #0]
	ldr	r5, [pc, #80]
	movs	r1, #198
	ldr	r3, [pc, #80]
	lsls	r1, r1, #5
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf001
	.2byte 0xfa06
	.2byte 0x2380
	lsls	r3, r3, #19
	adds	r3, #188
	str	r5, [r3, #0]
	movs	r3, #198
	lsls	r3, r3, #4
	adds	r5, r5, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #200
	str	r5, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #6
.L_081c1250:
	ldrb	r3, [r2, #0]
	cmp	r3, #159
	beq.n	.L_081c1250
.L_081c1256:
	ldrb	r3, [r2, #0]
	cmp	r3, #159
	bne.n	.L_081c1256
	ldr	r3, [pc, #8]
	movs	r2, #128
	strh	r2, [r3, #0]
	pop	{r5, pc}
	.4byte 0x03007ff0
	.4byte 0x04000102
	.4byte 0xfffddb60
	.4byte 0x04000100
	.4byte 0x02003a90
	.2byte 0x0258
	.2byte 0x0300
	add	r2, pc, #0
	bx	r2
