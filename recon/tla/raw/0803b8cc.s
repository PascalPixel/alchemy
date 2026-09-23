.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0803a404, 0x0803a404
	.set sub_0803b094, 0x0803b094
	.set sub_0803b794, 0x0803b794
	.global Func_0803b8cc
	.thumb_func
Func_0803b8cc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r1, #0
	movs	r1, #0
	ldr	r5, [r3, #60]
	adds	r7, r2, #0
	sub	sp, #12
	bl	sub_0803b094
	movs	r2, #244
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0803b8f8
	movs	r0, #0
	b.n	.L_0803b910
.L_0803b8f8:
	ldr	r3, [sp, #32]
	adds	r1, r6, #0
	str	r3, [sp, #0]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #1
	str	r3, [sp, #8]
	adds	r2, r7, #0
	mov	r3, r8
	bl	sub_0803b794
	movs	r0, #1
.L_0803b910:
	add	sp, #12
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
	sub	sp, #36
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	adds	r7, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #60]
	ldr	r3, [pc, #408]
	movs	r2, #15
	mov	ip, r3
	mov	r3, sp
	adds	r3, #20
	mov	r9, r2
	str	r3, [sp, #8]
	movs	r2, #28
	movs	r4, #0
	movs	r1, #0
	add	r2, sp
	mov	fp, r4
	movs	r6, #0
	mov	sl, r2
	mov	r8, r1
.L_0803b952:
	movs	r2, #244
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r5, r3]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r3, #255
	ands	r0, r3
	cmp	r2, #31
	bls.n	.L_0803b996
	cmp	r2, #176
	beq.n	.L_0803b996
	cmp	r2, #32
	bne.n	.L_0803b978
	adds	r6, #5
	adds	r1, #1
	b.n	.L_0803b952
.L_0803b978:
	ldr	r3, [pc, #344]
	subs	r2, #32
	lsls	r2, r2, #5
	ldrh	r2, [r3, r2]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #60
	ldrh	r3, [r3, r5]
	cmp	r3, #1
	beq.n	.L_0803b990
	cmp	r3, #5
	bne.n	.L_0803b992
.L_0803b990:
	adds	r2, #1
.L_0803b992:
	adds	r6, r6, r2
	b.n	.L_0803b952
.L_0803b996:
	cmp	r2, #28
	bhi.n	.L_0803b952
	lsls	r3, r2, #2
	mov	r2, ip
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	hlt	0x0010
	lsrs	r3, r0, #32
	.2byte 0xba48
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xba18
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xba60
	lsrs	r3, r0, #32
	.2byte 0xba6c
	lsrs	r3, r0, #32
	.2byte 0xba60
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xba56
	lsrs	r3, r0, #32
	.2byte 0xba60
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xba60
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xb952
	lsrs	r3, r0, #32
	.2byte 0xba56
	lsrs	r3, r0, #32
	mov	r3, sl
	mov	r2, r8
	adds	r1, #1
	strh	r1, [r3, r2]
	ldr	r3, [sp, #8]
	strh	r6, [r3, r2]
	cmp	r4, r6
	bcs.n	.L_0803ba2a
	adds	r4, r6, #0
.L_0803ba2a:
	mov	r3, fp
	cmp	r3, #2
	bhi.n	.L_0803ba3a
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	lsls	r3, r3, #1
	mov	r8, r3
.L_0803ba3a:
	ldr	r3, [pc, #148]
	movs	r2, #15
	movs	r1, #0
	movs	r6, #0
	add	r9, r2
	mov	ip, r3
	b.n	.L_0803b952
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_0803a404
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	b.n	.L_0803ba90
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r3, #255
	ands	r0, r3
	b.n	.L_0803b952
	movs	r2, #244
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r5, r3]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #60
	adds	r3, r3, r5
	strh	r2, [r3, #0]
	movs	r3, #128
	ldr	r2, [pc, #76]
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r3, #255
	ands	r0, r3
	mov	ip, r2
	b.n	.L_0803b952
.L_0803ba90:
	mov	r3, sl
	mov	r2, r8
	adds	r1, #1
	strh	r1, [r3, r2]
	ldr	r3, [sp, #8]
	strh	r6, [r3, r2]
	cmp	r4, r6
	bcs.n	.L_0803baa2
	adds	r4, r6, #0
.L_0803baa2:
	ldrb	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_0803baaa
	adds	r4, #2
.L_0803baaa:
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #140
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0803badc
	mov	r3, r9
	cmp	r3, #44
	bls.n	.L_0803bad8
	ldr	r3, [pc, #8]
	movs	r2, #32
	strh	r3, [r7, #6]
	mov	r9, r2
	b.n	.L_0803badc
	.4byte 0x00000001
	.4byte 0x0803b9a4
	.2byte 0xa4e0
	.2byte 0x0805
.L_0803bad8:
	ldr	r3, [pc, #44]
	strh	r3, [r7, #6]
.L_0803badc:
	ldr	r3, [sp, #16]
	mov	r2, r9
	str	r4, [r3, #0]
	ldr	r3, [sp, #12]
	str	r2, [r3, #0]
	adds	r3, r4, #0
	adds	r3, #19
	lsrs	r4, r3, #3
	lsls	r3, r4, #3
	adds	r4, r3, #0
	subs	r4, #16
	cmp	r7, #0
	beq.n	.L_0803bb44
	movs	r6, #0
	movs	r5, #0
.L_0803bafa:
	mov	r2, sl
	ldrh	r3, [r5, r2]
	cmp	r3, #1
	bhi.n	.L_0803bb0e
	ldr	r3, [pc, #4]
	strh	r3, [r7, #0]
	b.n	.L_0803bb0c
	.2byte 0x0000
	.2byte 0x0000
.L_0803bb0c:
	b.n	.L_0803bb3a
.L_0803bb0e:
	ldr	r2, [sp, #8]
	ldrh	r3, [r5, r2]
	subs	r0, r4, r3
	subs	r0, #4
	cmp	r0, #0
	bge.n	.L_0803bb1c
	movs	r0, #0
.L_0803bb1c:
	mov	r3, sl
	ldrh	r1, [r5, r3]
	lsls	r0, r0, #8
	subs	r1, #1
	str	r4, [sp, #0]
	bl	sub_08002054
	movs	r2, #192
	lsls	r2, r2, #4
	ldr	r4, [sp, #0]
	cmp	r0, r2
	bls.n	.L_0803bb38
	movs	r0, #128
	lsls	r0, r0, #2
.L_0803bb38:
	strh	r0, [r7, #0]
.L_0803bb3a:
	adds	r7, #2
	adds	r6, #1
	adds	r5, #2
	cmp	r6, fp
	bls.n	.L_0803bafa
.L_0803bb44:
	bl	sub_0803a404
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
