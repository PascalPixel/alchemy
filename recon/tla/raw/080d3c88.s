.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08015778, 0x08015778
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_08038050, 0x08038050
	.set sub_080380f8, 0x080380f8
	.set sub_08038100, 0x08038100
	.set sub_08038108, 0x08038108
	.set sub_08038110, 0x08038110
	.set sub_08038140, 0x08038140
	.set sub_080381e0, 0x080381e0
	.set sub_080cad84, 0x080cad84
	.set sub_080cccb8, 0x080cccb8
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d1eac, 0x080d1eac
	.set sub_080d2240, 0x080d2240
	.set sub_080d2840, 0x080d2840
	.set sub_080d3bf8, 0x080d3bf8
	.set sub_080d4330, 0x080d4330
	.set sub_080d5bec, 0x080d5bec
	.global Func_080d3c88
	.thumb_func
Func_080d3c88:
.L_080d3c88:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #60]
	sub	sp, #64
	str	r1, [sp, #32]
	movs	r2, #0
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	str	r3, [sp, #28]
	mov	sl, r2
	mov	r9, r2
	bl	sub_080d3bf8
	movs	r2, #240
	movs	r1, #4
	lsls	r2, r2, #8
	movs	r3, #0
	str	r1, [sp, #12]
	mov	fp, r2
	ldr	r1, [sp, #28]
	str	r0, [sp, #24]
	str	r3, [sp, #20]
	str	r3, [sp, #16]
	movs	r2, #226
	mov	r3, fp
	lsls	r2, r2, #1
	ands	r3, r6
	mov	fp, r3
	adds	r3, r1, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r6, r3
	movs	r4, #0
	adds	r0, r6, #0
	mov	r8, r1
	str	r4, [sp, #4]
	bl	sub_080cad84
	ldr	r1, [sp, #28]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	str	r6, [r3, #0]
	subs	r2, #40
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r5, #0
	movs	r7, #0
	ldr	r4, [sp, #4]
	cmp	r3, #0
	beq.n	.L_080d3d04
	b.n	.L_080d3f7a
.L_080d3d04:
	cmp	r0, #0
	beq.n	.L_080d3d36
	subs	r2, #46
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080d3d2e
	add	r5, sp, #52
	adds	r1, r5, #0
	adds	r0, #8
	bl	sub_08015778
	ldr	r3, [r5, #0]
	movs	r7, #1
	asrs	r4, r3, #3
	ldr	r3, [r5, #4]
	asrs	r5, r3, #3
	subs	r5, #2
	b.n	.L_080d3d82
.L_080d3d2e:
	add	r5, sp, #52
	adds	r1, r5, #0
	adds	r0, r6, #0
	b.n	.L_080d3d6e
.L_080d3d36:
	cmp	r6, #7
	bgt.n	.L_080d3d82
	str	r6, [sp, #24]
	bl	sub_080cdf5c
	bl	sub_080cad84
	ldr	r1, [sp, #28]
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080d3d66
	add	r5, sp, #52
	adds	r1, r5, #0
	adds	r0, #8
	bl	sub_08015778
	ldr	r3, [r5, #0]
	movs	r7, #1
	b.n	.L_080d3d7c
.L_080d3d66:
	bl	sub_080cdf5c
	add	r5, sp, #52
	adds	r1, r5, #0
.L_080d3d6e:
	bl	sub_080d5bec
	mvns	r0, r0
	negs	r3, r0
	orrs	r3, r0
	lsrs	r7, r3, #31
	ldr	r3, [r5, #0]
.L_080d3d7c:
	asrs	r4, r3, #3
	ldr	r3, [r5, #4]
	asrs	r5, r3, #3
.L_080d3d82:
	cmp	r7, #0
	bne.n	.L_080d3d8e
	movs	r3, #15
	str	r3, [sp, #48]
	movs	r3, #10
	b.n	.L_080d3dde
.L_080d3d8e:
	movs	r3, #0
	add	r0, sp, #36
	str	r3, [sp, #48]
	str	r3, [sp, #44]
	add	r1, sp, #48
	add	r2, sp, #44
	add	r3, sp, #40
	str	r0, [sp, #0]
	mov	r0, r8
	str	r4, [sp, #4]
	bl	sub_08038110
	ldr	r3, [sp, #40]
	ldr	r4, [sp, #4]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r3, r4, r3
	str	r3, [sp, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080d3dc8
	ldr	r3, [sp, #36]
	subs	r3, r5, r3
	subs	r3, #1
	b.n	.L_080d3dde
.L_080d3dc8:
	mov	r2, fp
	lsrs	r3, r2, #15
	cmp	r3, #0
	bne.n	.L_080d3ddc
	cmp	r5, #8
	bgt.n	.L_080d3ddc
	ldr	r3, [sp, #36]
	subs	r3, r5, r3
	subs	r3, #1
	b.n	.L_080d3dde
.L_080d3ddc:
	adds	r3, r5, #4
.L_080d3dde:
	str	r3, [sp, #44]
	ldr	r1, [sp, #32]
	ldrb	r3, [r1, #4]
	cmp	r3, #0
	beq.n	.L_080d3dec
	movs	r2, #5
	str	r2, [sp, #12]
.L_080d3dec:
	movs	r3, #128
	lsls	r3, r3, #5
	mov	r1, fp
	ands	r3, r1
	adds	r6, r4, #0
	cmp	r3, #0
	beq.n	.L_080d3e08
	ldr	r2, [sp, #12]
	subs	r3, r6, r2
	subs	r6, r3, #2
	cmp	r6, #0
	bge.n	.L_080d3e44
	movs	r6, #0
	b.n	.L_080d3e44
.L_080d3e08:
	movs	r3, #128
	lsls	r3, r3, #6
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080d3e24
	ldr	r2, [sp, #12]
	adds	r6, #2
	adds	r3, r6, r2
	cmp	r3, #29
	ble.n	.L_080d3e44
	movs	r3, #29
	subs	r6, r3, r2
	b.n	.L_080d3e44
.L_080d3e24:
	cmp	r6, #15
	bgt.n	.L_080d3e36
	ldr	r1, [sp, #12]
	subs	r3, r6, r1
	subs	r6, r3, #2
	cmp	r6, #0
	bge.n	.L_080d3e44
	adds	r6, r4, #2
	b.n	.L_080d3e44
.L_080d3e36:
	ldr	r2, [sp, #12]
	adds	r6, #2
	adds	r3, r6, r2
	cmp	r3, #29
	ble.n	.L_080d3e44
	subs	r3, r4, r2
	subs	r6, r3, #2
.L_080d3e44:
	ldr	r0, [sp, #24]
	bl	sub_080381e0
	movs	r3, #1
	negs	r3, r3
	adds	r7, r0, #0
	mov	sl, r3
	cmp	r7, sl
	beq.n	.L_080d3ec4
	mov	r3, sp
	movs	r1, #48
	movs	r2, #44
	add	r1, sp
	add	r2, sp
	adds	r3, #40
	add	r7, sp, #36
	mov	r0, r8
	mov	r9, r1
	mov	fp, r2
	str	r3, [sp, #8]
	str	r7, [sp, #0]
	bl	sub_08038110
	ldr	r2, [sp, #44]
	mov	r8, sl
	subs	r1, r2, #5
	str	r1, [sp, #16]
	cmp	r2, r5
	bgt.n	.L_080d3e84
	ldr	r3, [sp, #36]
	adds	r3, r2, r3
	str	r3, [sp, #16]
.L_080d3e84:
	ldr	r3, [sp, #16]
	cmp	r3, #0
	bge.n	.L_080d3e92
	ldr	r3, [sp, #36]
	adds	r3, r2, r3
	str	r3, [sp, #16]
	b.n	.L_080d3e9e
.L_080d3e92:
	ldr	r3, [sp, #16]
	adds	r3, #5
	cmp	r3, #19
	ble.n	.L_080d3e9e
	subs	r1, r2, #5
	str	r1, [sp, #16]
.L_080d3e9e:
	ldr	r3, [sp, #16]
	cmp	r2, r3
	bge.n	.L_080d3ee6
	movs	r0, #1
	mov	r1, r9
	ldr	r3, [sp, #8]
	negs	r0, r0
	mov	r2, fp
	ldr	r5, [sp, #36]
	str	r7, [sp, #0]
	bl	sub_08038108
	ldr	r3, [sp, #36]
	movs	r1, #1
	subs	r5, r5, r3
	negs	r1, r1
	adds	r5, #1
	mov	r8, r1
	b.n	.L_080d3ee4
.L_080d3ec4:
	ldr	r3, [sp, #44]
	cmp	r3, r5
	bge.n	.L_080d3ee6
	add	r0, sp, #36
	add	r3, sp, #40
	str	r0, [sp, #0]
	add	r1, sp, #48
	mov	r0, r8
	add	r2, sp, #44
	ldr	r5, [sp, #36]
	bl	sub_08038108
	ldr	r3, [sp, #36]
	mov	r8, r7
	subs	r5, r5, r3
	adds	r5, #1
.L_080d3ee4:
	str	r5, [sp, #20]
.L_080d3ee6:
	cmp	r6, #0
	bge.n	.L_080d3eee
	movs	r6, #0
	b.n	.L_080d3efa
.L_080d3eee:
	ldr	r2, [sp, #12]
	adds	r3, r6, r2
	cmp	r3, #29
	ble.n	.L_080d3efa
	movs	r3, #29
	subs	r6, r3, r2
.L_080d3efa:
	ldr	r1, [sp, #32]
	ldrb	r3, [r1, #4]
	cmp	r3, #0
	beq.n	.L_080d3f28
	movs	r0, #8
	bl	sub_08013560
	ldr	r2, [sp, #20]
	cmp	r2, #0
	beq.n	.L_080d3f1e
	ldr	r3, [sp, #20]
	ldr	r2, [sp, #44]
	ldr	r1, [sp, #48]
	adds	r2, r2, r3
	subs	r2, #1
	mov	r0, r8
	movs	r3, #18
	b.n	.L_080d3f52
.L_080d3f1e:
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #44]
	mov	r0, r8
	movs	r3, #2
	b.n	.L_080d3f52
.L_080d3f28:
	ldr	r0, [sp, #24]
	bl	sub_080d1eac
	ldr	r1, [sp, #20]
	cmp	r1, #0
	beq.n	.L_080d3f46
	ldr	r3, [sp, #20]
	ldr	r2, [sp, #44]
	ldr	r1, [sp, #48]
	adds	r2, r2, r3
	lsls	r3, r0, #16
	movs	r0, #17
	orrs	r3, r0
	subs	r2, #1
	b.n	.L_080d3f50
.L_080d3f46:
	lsls	r3, r0, #16
	movs	r0, #1
	orrs	r3, r0
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #44]
.L_080d3f50:
	mov	r0, r8
.L_080d3f52:
	bl	sub_08038038
	mov	sl, r0
	ldr	r1, [sp, #32]
	ldrb	r3, [r1, #4]
	ldr	r0, [sp, #24]
	movs	r1, #0
	adds	r2, r6, #0
	ldr	r3, [sp, #16]
	bl	sub_080380f8
	mov	r9, r0
	b.n	.L_080d3f72
.L_080d3f6c:
	movs	r0, #1
	bl	sub_08013560
.L_080d3f72:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_080d3f6c
.L_080d3f7a:
	ldr	r2, [sp, #28]
	movs	r1, #242
	lsls	r1, r1, #1
	adds	r3, r2, r1
	mov	r2, sl
	str	r2, [r3, #0]
	ldr	r1, [sp, #28]
	movs	r2, #244
	lsls	r2, r2, #1
	adds	r3, r1, r2
	mov	r1, r9
	str	r1, [r3, #0]
	ldr	r3, [sp, #28]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	mov	r0, sl
	adds	r3, #1
	strh	r3, [r2, #0]
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_080d3fb0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r7, r0, #0
	mov	r9, r3
	bl	.L_080d3c88
	mov	sl, r0
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r7, #0
	bl	sub_080d3bf8
	movs	r5, #0
	mov	r8, r0
	cmp	r7, #7
	bgt.n	.L_080d3ff2
	movs	r6, #240
	lsls	r6, r6, #4
	adds	r6, #255
	ands	r6, r7
	adds	r0, r6, #0
	bl	sub_080cccb8
	cmp	r0, #0
	bne.n	.L_080d3ff2
	mov	r8, r6
.L_080d3ff2:
	mov	r0, r8
	bl	sub_08038100
	movs	r3, #220
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080d4050
	b.n	.L_080d4046
.L_080d4006:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #150
	adds	r5, #1
	lsls	r3, r3, #2
	cmp	r5, r3
	bhi.n	.L_080d4042
	ldr	r1, [pc, #80]
	movs	r3, #4
	ldr	r2, [r1, #0]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080d4046
	ldr	r2, [r1, #0]
	adds	r3, #252
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080d4046
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080d4046
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d4046
.L_080d4042:
	bl	sub_08038140
.L_080d4046:
	mov	r0, sl
	bl	sub_08038050
	cmp	r0, #0
	beq.n	.L_080d4006
.L_080d4050:
	movs	r0, #128
	lsls	r0, r0, #24
	bl	sub_080d4330
	movs	r0, #1
	bl	sub_08013560
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r2, #0
	bl	.L_080d3fb0
	adds	r0, r5, #0
	bl	sub_080d2240
	pop	{r5, pc}
	push	{lr}
	bl	.L_080d3fb0
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	.L_080d3c88
	bl	sub_080cdf5c
	movs	r1, #0
	bl	sub_080d2840
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_080d40ba
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	.L_080d3fb0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_080d40d4
.L_080d40ba:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r3, #1
	strh	r3, [r2, #0]
	adds	r1, r6, #0
	bl	.L_080d3fb0
.L_080d40d4:
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	bx	lr
	.2byte 0x0000
