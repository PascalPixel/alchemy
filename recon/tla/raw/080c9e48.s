.syntax unified
	.thumb
	.set sub_0800206c, 0x0800206c
	.set sub_08014878, 0x08014878
	.set sub_08016ce4, 0x08016ce4
	.set sub_080202b0, 0x080202b0
	.set sub_080ad290, 0x080ad290
	.set sub_080ca4a8, 0x080ca4a8
	.set sub_080ca514, 0x080ca514
	.set sub_080ca5d8, 0x080ca5d8
	.global Func_080c9e48
	.thumb_func
Func_080c9e48:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #200]
	movs	r3, #192
	movs	r1, #240
	lsls	r1, r1, #1
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r4, [r3, r1]
	sub	sp, #8
	mov	sl, r4
	movs	r4, #241
	lsls	r4, r4, #1
	adds	r3, r2, r4
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r3, #198
	movs	r4, #0
	lsls	r3, r3, #1
	mov	r8, r1
	adds	r6, r7, r3
	str	r4, [sp, #4]
	str	r4, [sp, #0]
	ldr	r5, [pc, #156]
	cmp	r0, #0
	beq.n	.L_080c9eda
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldr	r1, [pc, #152]
	ldrh	r2, [r5, #0]
	asrs	r0, r1, #16
	cmp	r3, r0
	beq.n	.L_080c9eda
	mov	fp, r1
	mov	r9, r0
.L_080c9e9c:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, sl
	bne.n	.L_080c9ece
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	mov	r4, fp
	asrs	r2, r4, #16
	cmp	r3, r2
	beq.n	.L_080c9eb4
	cmp	r3, r8
	bne.n	.L_080c9ece
.L_080c9eb4:
	movs	r1, #4
	ldrsh	r0, [r5, r1]
	cmp	r0, r2
	beq.n	.L_080c9ec4
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080c9ece
.L_080c9ec4:
	ldrb	r2, [r5, #6]
	str	r2, [sp, #4]
	ldrb	r5, [r5, #7]
	str	r5, [sp, #0]
	b.n	.L_080c9eda
.L_080c9ece:
	adds	r5, #8
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	ldrh	r2, [r5, #0]
	cmp	r3, r9
	bne.n	.L_080c9e9c
.L_080c9eda:
	add	r1, sp, #4
	ldrb	r1, [r1, #0]
	mov	r2, sp
	strb	r1, [r6, #0]
	adds	r6, #1
	ldrb	r2, [r2, #0]
	movs	r3, #0
	strb	r2, [r6, #0]
	adds	r6, #1
	movs	r2, #0
.L_080c9eee:
	adds	r3, #1
	strb	r2, [r6, #0]
	adds	r6, #1
	cmp	r3, #5
	bls.n	.L_080c9eee
	movs	r3, #202
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r4, #204
	movs	r3, #0
	str	r3, [r2, #0]
	lsls	r4, r4, #1
	movs	r3, #128
	adds	r2, r7, r4
	lsls	r3, r3, #13
	str	r3, [r2, #0]
	bl	sub_080ca4a8
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x080ee6d4
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	sub	sp, #4
	ldr	r5, [pc, #136]
	cmp	r3, #8
	bne.n	.L_080c9f4c
	movs	r0, #0
	b.n	.L_080c9fbe
.L_080c9f4c:
	cmp	r3, #7
	bne.n	.L_080c9f52
	ldr	r5, [pc, #124]
.L_080c9f52:
	mov	r1, sp
	bl	sub_080202b0
	movs	r1, #20
	adds	r6, r0, #0
	ldr	r0, [sp, #0]
	bl	sub_0800206c
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldr	r1, [pc, #108]
	movs	r2, #0
	str	r0, [sp, #0]
	asrs	r0, r1, #16
	mov	sl, r2
	ldrh	r2, [r5, #0]
	cmp	r3, r0
	beq.n	.L_080c9fb6
	adds	r7, r1, #0
	mov	r8, r0
.L_080c9f7a:
	lsls	r3, r2, #16
	ldr	r2, [sp, #0]
	asrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_080c9faa
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	asrs	r2, r7, #16
	cmp	r3, r2
	beq.n	.L_080c9f92
	cmp	r3, r6
	bne.n	.L_080c9faa
.L_080c9f92:
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	cmp	r0, r2
	beq.n	.L_080c9fa2
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080c9faa
.L_080c9fa2:
	movs	r2, #6
	ldrsh	r1, [r5, r2]
	mov	sl, r1
	b.n	.L_080c9fb6
.L_080c9faa:
	adds	r5, #8
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, r8
	bne.n	.L_080c9f7a
.L_080c9fb6:
	adds	r0, r6, #0
	bl	sub_080ca514
	mov	r0, sl
.L_080c9fbe:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x080eedbc
	.4byte 0x080eef34
	.2byte 0x0000
	.2byte 0xffff
.L_080c9fd8:
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #96
	adds	r0, #255
	mov	fp, r3
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ca0ea
	movs	r0, #176
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ca072
	movs	r0, #98
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ca072
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L_080ca072
	ldr	r5, [pc, #308]
	movs	r2, #183
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080ca036
	b.n	.L_080ca146
.L_080ca036:
	ldr	r0, [sp, #4]
	ldr	r2, [pc, #284]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r7, [r3, #0]
	mov	r9, r3
	movs	r0, #0
	cmp	r7, #0
	beq.n	.L_080ca146
	bl	sub_080ad290
	mov	r2, r9
	ldrh	r3, [r2, #2]
	subs	r0, r0, r3
	cmp	r0, #0
	bge.n	.L_080ca05c
	movs	r0, #0
.L_080ca05c:
	cmp	r0, #5
	ble.n	.L_080ca062
	movs	r0, #5
.L_080ca062:
	cmp	r0, #0
	ble.n	.L_080ca076
	movs	r2, #153
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080ca076
.L_080ca072:
	movs	r0, #0
	b.n	.L_080ca146
.L_080ca076:
	lsls	r3, r0, #2
	adds	r3, r3, r0
	adds	r7, r7, r3
	movs	r3, #202
	lsls	r3, r3, #1
	add	r3, fp
	ldr	r5, [r3, #0]
	mov	sl, r3
	cmp	r5, #0
	bne.n	.L_080ca0b2
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	mov	r8, r0
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	mov	r2, r8
	subs	r5, r5, r2
	adds	r5, r5, r6
	subs	r5, r5, r0
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	asrs	r5, r5, #1
	mov	r3, sl
	str	r5, [r3, #0]
.L_080ca0b2:
	lsls	r3, r7, #4
	subs	r3, #16
	muls	r3, r5
	lsls	r0, r7, #20
	movs	r1, #128
	adds	r0, r0, r3
	lsls	r1, r1, #13
	ldr	r3, [pc, #152]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b26
	ldr	r1, [sp, #0]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b21
	movs	r2, #150
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	adds	r2, r2, r0
	str	r2, [r3, #0]
	movs	r3, #204
	lsls	r3, r3, #1
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r0, #0
	cmp	r2, r3
	blt.n	.L_080ca146
.L_080ca0ea:
	movs	r2, #202
	lsls	r2, r2, #1
	add	r2, fp
	movs	r3, #0
	str	r3, [r2, #0]
	mov	r2, r9
	movs	r5, #0
	adds	r2, #20
	movs	r1, #7
.L_080ca0fc:
	ldrb	r3, [r2, #0]
	subs	r1, #1
	adds	r2, #1
	adds	r5, r5, r3
	cmp	r1, #0
	bge.n	.L_080ca0fc
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_080ca146
	bl	sub_08014878
	adds	r3, r5, #0
	muls	r3, r0
	mov	r0, r9
	lsrs	r2, r3, #16
	ldrb	r3, [r0, #20]
	movs	r1, #0
	subs	r2, r2, r3
	cmp	r2, #0
	blt.n	.L_080ca136
	adds	r0, #20
.L_080ca126:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_080ca136
	adds	r0, #1
	ldrb	r3, [r0, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_080ca126
.L_080ca136:
	lsls	r3, r1, #1
	adds	r3, #4
	mov	r2, r9
	ldrh	r5, [r2, r3]
	ldr	r0, [sp, #4]
	bl	sub_080ca5d8
	adds	r0, r5, #0
.L_080ca146:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x080edacc
	.4byte 0x03000230
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #198
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrb	r0, [r3, r0]
	bl	.L_080c9fd8
	pop	{pc}
	.2byte 0x0000
