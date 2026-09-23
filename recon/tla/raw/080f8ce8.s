.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08016cfc, 0x08016cfc
	.set sub_08038078, 0x08038078
	.set sub_08038080, 0x08038080
	.set sub_08038108, 0x08038108
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_080ad010, 0x080ad010
	.set sub_080f811c, 0x080f811c
	.set sub_080f815c, 0x080f815c
	.set sub_080f8f40, 0x080f8f40
	.set sub_080f93a4, 0x080f93a4
	.global Func_080f8ce8
	.thumb_func
Func_080f8ce8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	r9, r2
	ldr	r2, [r3, #20]
	mov	r8, r3
	movs	r3, #13
	strb	r3, [r2, #5]
	movs	r2, #1
	negs	r2, r2
	sub	sp, #24
	adds	r7, r0, #0
	mov	sl, r1
	cmp	r9, r2
	beq.n	.L_080f8d54
	add	r0, sp, #8
	add	r1, sp, #20
	add	r2, sp, #16
	add	r3, sp, #12
	str	r0, [sp, #0]
	adds	r0, r7, #0
	bl	sub_08038108
	ldr	r2, [sp, #8]
	mov	r5, r8
	str	r2, [sp, #0]
	movs	r2, #129
	lsls	r2, r2, #1
	adds	r5, #64
	str	r2, [sp, #4]
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	mov	r1, sl
	mov	r2, r9
	bl	sub_080f811c
	cmp	r0, #0
	bne.n	.L_080f8d50
	ldr	r2, [sp, #8]
	ldr	r0, [r5, #0]
	ldr	r3, [sp, #12]
	str	r2, [sp, #0]
	mov	r1, sl
	mov	r2, r9
	bl	sub_080f93a4
.L_080f8d50:
	ldr	r6, [r5, #0]
	b.n	.L_080f8d58
.L_080f8d54:
	mov	r3, r8
	ldr	r6, [r3, #48]
.L_080f8d58:
	adds	r0, r6, #0
	bl	sub_08038260
	adds	r0, r6, #0
	bl	sub_08038268
	movs	r2, #1
	negs	r2, r2
	cmp	r9, r2
	bne.n	.L_080f8d7a
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	b.n	.L_080f8d86
.L_080f8d7a:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038078
.L_080f8d86:
	movs	r3, #1
	negs	r3, r3
	cmp	sl, r3
	beq.n	.L_080f8dc8
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [pc, #44]
	movs	r7, #1
.L_080f8d98:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r5, #4]
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_080f8dba
	ldr	r3, [r5, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080f8dba
	ldr	r3, [r5, #4]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f8d98
.L_080f8dba:
	adds	r0, r6, #0
	bl	sub_08038268
	b.n	.L_080f8dd0
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
.L_080f8dc8:
	movs	r0, #82
	adds	r0, #255
	bl	sub_08016cfc
.L_080f8dd0:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #30
	add	r2, r8
	movs	r3, #1
	strh	r3, [r2, #0]
	mov	r2, r8
	ldr	r3, [r2, #20]
	ldr	r1, [pc, #20]
	strb	r1, [r3, #5]
	movs	r3, #1
	negs	r3, r3
	cmp	r9, r3
	beq.n	.L_080f8dfc
	mov	r0, r8
	adds	r0, #64
	movs	r1, #1
	bl	sub_080f815c
	b.n	.L_080f8dfc
	.2byte 0x0001
	.2byte 0x0000
.L_080f8dfc:
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #112
	str	r0, [sp, #12]
	add	r5, sp, #16
	movs	r0, #0
	str	r0, [sp, #8]
	mov	fp, r0
	adds	r0, r1, #0
	adds	r1, r5, #0
	bl	sub_080f8f40
	add	r1, sp, #48
	mov	r9, r1
	movs	r2, #0
	movs	r6, #14
.L_080f8e32:
	ldr	r0, [sp, #12]
	subs	r6, #1
	ldrh	r3, [r2, r0]
	strh	r3, [r2, r1]
	adds	r2, #2
	cmp	r6, #0
	bge.n	.L_080f8e32
	movs	r1, #0
	mov	r8, r1
	mov	r2, r9
	movs	r6, #14
.L_080f8e48:
	ldrh	r3, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	beq.n	.L_080f8e54
	movs	r3, #1
	add	r8, r3
.L_080f8e54:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_080f8e48
	mov	r0, r8
	cmp	r0, #14
	bgt.n	.L_080f8e7c
	add	r3, sp, #80
	lsls	r2, r0, #1
	ldr	r1, [pc, #16]
	adds	r2, r2, r3
	movs	r3, #15
	subs	r6, r3, r0
.L_080f8e6c:
	subs	r6, #1
	strh	r1, [r2, #0]
	adds	r2, #2
	cmp	r6, #0
	bne.n	.L_080f8e6c
	b.n	.L_080f8e7c
	.2byte 0x0000
	.2byte 0x0000
.L_080f8e7c:
	ldrb	r3, [r5, #0]
	cmp	r3, #255
	beq.n	.L_080f8f14
	mov	r1, sp
	adds	r1, #80
	str	r1, [sp, #4]
	mov	sl, r9
	adds	r7, r5, #0
.L_080f8e8c:
	movs	r6, #0
	movs	r4, #0
	cmp	r6, r8
	bge.n	.L_080f8ee2
	mov	r5, r9
.L_080f8e96:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080f8eda
	adds	r0, r3, #0
	str	r4, [sp, #0]
	bl	sub_080ad010
	ldrb	r1, [r7, #0]
	ldrb	r3, [r0, #2]
	movs	r2, #127
	ands	r2, r1
	ldr	r4, [sp, #0]
	cmp	r2, r3
	bne.n	.L_080f8eda
	movs	r3, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080f8ecc
	ldrh	r2, [r5, #0]
	ldr	r3, [pc, #8]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f8eda
	b.n	.L_080f8ece
	movs	r0, r0
	.2byte 0x0200
	.2byte 0x0000
.L_080f8ecc:
	ldrh	r2, [r5, #0]
.L_080f8ece:
	ldr	r3, [pc, #44]
	ands	r3, r2
	cmp	r4, r3
	bge.n	.L_080f8eda
	str	r6, [sp, #8]
	adds	r4, r3, #0
.L_080f8eda:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r8
	blt.n	.L_080f8e96
.L_080f8ee2:
	cmp	r4, #0
	beq.n	.L_080f8f0c
	ldr	r0, [sp, #8]
	mov	r3, fp
	lsls	r2, r0, #1
	mov	r0, sl
	lsls	r1, r3, #1
	ldrh	r3, [r0, r2]
	ldr	r0, [sp, #4]
	strh	r3, [r0, r1]
	ldr	r3, [pc, #8]
	mov	r1, sl
	b.n	.L_080f8f04
	.4byte 0x000001ff
	.2byte 0x0000
	.2byte 0x0000
.L_080f8f04:
	strh	r3, [r1, r2]
	movs	r3, #1
	add	fp, r3
	b.n	.L_080f8e8c
.L_080f8f0c:
	adds	r7, #1
	ldrb	r3, [r7, #0]
	cmp	r3, #255
	bne.n	.L_080f8e8c
.L_080f8f14:
	mov	r0, r8
	cmp	r0, #0
	ble.n	.L_080f8f2e
	add	r1, sp, #80
	movs	r2, #0
	mov	r6, r8
.L_080f8f20:
	ldrh	r3, [r2, r1]
	ldr	r0, [sp, #12]
	subs	r6, #1
	strh	r3, [r2, r0]
	adds	r2, #2
	cmp	r6, #0
	bne.n	.L_080f8f20
.L_080f8f2e:
	movs	r0, #1
	add	sp, #112
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
