.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08020018, 0x08020018
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08020048, 0x08020048
	.set sub_080c82b8, 0x080c82b8
	.set sub_081c0010, 0x081c0010
	.global Overlay_08104c00
Overlay_08104c00:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #184]
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [r5, #12]
	ldr	r6, [r0, #0]
	adds	r7, r2, #0
	movs	r2, #32
	ands	r3, r2
	mov	fp, r0
	mov	r8, r1
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_08104c32
	subs	r6, #1
	adds	r0, r6, r1
	bl	sub_08002064
	adds	r6, r0, #0
.L_08104c32:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08104c4a
	adds	r6, #1
	mov	r3, r8
	adds	r0, r6, r3
	mov	r1, r8
	bl	sub_08002064
	adds	r6, r0, #0
.L_08104c4a:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08104c5c
	subs	r3, r6, r7
	cmp	r3, #0
	blt.n	.L_08104c5c
	adds	r6, r3, #0
.L_08104c5c:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08104c88
	mov	r3, r8
	adds	r0, r7, r3
	subs	r0, #1
	adds	r1, r7, #0
	bl	sub_08002054
	adds	r3, r7, #0
	muls	r3, r0
	adds	r5, r6, r7
	cmp	r5, r3
	bge.n	.L_08104c7e
	adds	r6, r5, #0
.L_08104c7e:
	mov	r1, r8
	subs	r1, #1
	cmp	r6, r1
	ble.n	.L_08104c88
	adds	r6, r1, #0
.L_08104c88:
	cmp	r9, r6
	beq.n	.L_08104c96
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	mov	sl, r3
.L_08104c96:
	adds	r1, r7, #0
	mov	r0, r9
	bl	sub_08002054
	adds	r1, r7, #0
	adds	r5, r7, #0
	muls	r5, r0
	adds	r0, r6, #0
	bl	sub_08002054
	adds	r3, r7, #0
	muls	r3, r0
	cmp	r5, r3
	beq.n	.L_08104cb6
	movs	r3, #2
	mov	sl, r3
.L_08104cb6:
	mov	r3, fp
	str	r6, [r3, #0]
	mov	r0, sl
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
.L_08104ccc:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r5, r1, #0
	mov	ip, r3
	cmp	r0, #0
	bge.n	.L_08104ce0
	adds	r0, #3
.L_08104ce0:
	asrs	r3, r0, #2
	lsls	r0, r3, #2
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r2, [r3, #0]
	movs	r4, #13
	movs	r1, #0
	strb	r4, [r2, #5]
	cmp	r0, #0
	beq.n	.L_08104cfc
	movs	r3, #17
	strb	r3, [r2, #5]
	strh	r1, [r2, #12]
.L_08104cfc:
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r2, [r3, #0]
	adds	r3, r0, #4
	strb	r4, [r2, #5]
	cmp	r3, r5
	bge.n	.L_08104d12
	movs	r3, #15
	strb	r3, [r2, #5]
	strh	r1, [r2, #12]
.L_08104d12:
	pop	{r5, pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r5, r1, #0
	mov	ip, r3
	cmp	r0, #0
	bge.n	.L_08104d28
	adds	r0, #3
.L_08104d28:
	asrs	r3, r0, #2
	lsls	r0, r3, #2
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r2, [r3, #0]
	movs	r4, #13
	movs	r1, #0
	strb	r4, [r2, #5]
	cmp	r0, #0
	beq.n	.L_08104d44
	movs	r3, #16
	strb	r3, [r2, #5]
	strh	r1, [r2, #12]
.L_08104d44:
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r2, [r3, #0]
	adds	r3, r0, #4
	strb	r4, [r2, #5]
	cmp	r3, r5
	bge.n	.L_08104d5a
	movs	r3, #14
	strb	r3, [r2, #5]
	strh	r1, [r2, #12]
.L_08104d5a:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r1, #0
	adds	r3, #220
	ldr	r7, [r3, #0]
	adds	r1, r2, #0
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08104d72
	adds	r3, r5, #3
.L_08104d72:
	asrs	r3, r3, #2
	adds	r0, r5, #0
	lsls	r6, r3, #2
	bl	.L_08104ccc
	movs	r3, #148
	ldr	r4, [pc, #20]
	ldr	r0, [pc, #20]
	lsls	r3, r3, #1
	movs	r1, #0
	adds	r2, r7, r3
.L_08104d88:
	adds	r3, r6, r1
	cmp	r3, r5
	bne.n	.L_08104d9c
	strh	r4, [r2, #0]
	b.n	.L_08104d9e
	movs	r0, r0
	.4byte 0x0000001e
	.2byte 0x001a
	.2byte 0x0000
.L_08104d9c:
	strh	r0, [r2, #0]
.L_08104d9e:
	adds	r1, #1
	adds	r2, #2
	cmp	r1, #3
	ble.n	.L_08104d88
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r2, [r3, #0]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #9
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #32
	cmp	r3, #0
	beq.n	.L_08104e30
	movs	r1, #140
	movs	r3, #0
	lsls	r1, r1, #1
	adds	r5, r2, #0
	mov	r8, r3
	add	r4, sp, #8
	add	r6, sp, #16
	adds	r7, r2, r1
	adds	r5, #248
.L_08104ddc:
	movs	r2, #16
	ldrsh	r3, [r7, r2]
	ldr	r0, [r5, #0]
	movs	r2, #229
	lsls	r3, r3, #15
	lsls	r2, r2, #15
	subs	r2, r2, r3
	cmp	r0, #0
	beq.n	.L_08104e22
	ldr	r3, [r5, #64]
	str	r4, [sp, #4]
	str	r3, [sp, #8]
	ldr	r3, [r5, #64]
	str	r3, [r4, #4]
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	str	r2, [r6, #4]
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r1, #16
	ldrsh	r3, [r7, r1]
	adds	r1, r6, #0
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r3, #0
	str	r3, [r6, #12]
	movs	r3, #250
	str	r3, [sp, #0]
	movs	r3, #128
	adds	r2, r4, #0
	lsls	r3, r3, #7
	bl	sub_08020018
	ldr	r4, [sp, #4]
.L_08104e22:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #2
	adds	r5, #4
	cmp	r3, #7
	ble.n	.L_08104ddc
.L_08104e30:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r0, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	movs	r2, #0
	ldr	r6, [r3, #0]
	mov	r9, r2
	movs	r2, #172
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #1
	negs	r2, r2
	mov	sl, r2
	movs	r2, #7
	mov	fp, r1
	movs	r7, #248
	mov	r8, r2
.L_08104e6a:
	ldr	r5, [r7, r6]
	cmp	r5, #0
	beq.n	.L_08104e82
	adds	r0, r5, #0
	str	r3, [sp, #0]
	bl	sub_08020048
	ldr	r3, [sp, #0]
	mov	r2, r9
	str	r2, [r7, r6]
	mov	r2, sl
	strh	r2, [r3, #0]
.L_08104e82:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r2, r8
	adds	r3, #2
	adds	r7, #4
	cmp	r2, #0
	bge.n	.L_08104e6a
	movs	r3, #0
	movs	r2, #172
	adds	r7, r6, #0
	mov	r8, r3
	lsls	r2, r2, #1
	subs	r3, #13
	adds	r7, #248
	adds	r6, r6, r2
	mov	sl, r3
.L_08104ea4:
	mov	r2, r8
	ldr	r0, [sp, #4]
	cmp	r2, #0
	beq.n	.L_08104eae
	mov	r0, fp
.L_08104eae:
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_08104eda
	strh	r0, [r6, #0]
	bl	sub_080c82b8
	bl	sub_08020040
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08104ed8
	movs	r1, #1
	bl	sub_08020030
	ldrb	r3, [r5, #9]
	mov	r2, sl
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #0
	strb	r3, [r5, #26]
.L_08104ed8:
	str	r5, [r7, #0]
.L_08104eda:
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	adds	r7, #4
	adds	r6, #2
	cmp	r2, #1
	ble.n	.L_08104ea4
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
