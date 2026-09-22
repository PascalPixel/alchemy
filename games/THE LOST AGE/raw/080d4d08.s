.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_08020148, 0x08020148
	.set sub_080201c0, 0x080201c0
	.set sub_080201c8, 0x080201c8
	.set sub_08020210, 0x08020210
	.set sub_08020218, 0x08020218
	.set sub_08020258, 0x08020258
	.set sub_08020348, 0x08020348
	.set sub_080203a0, 0x080203a0
	.set sub_080d2240, 0x080d2240
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2350, 0x080d2350
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d2e40, 0x080d2e40
	.set sub_080d2ea4, 0x080d2ea4
	.set sub_080d30fc, 0x080d30fc
	.set sub_080d3838, 0x080d3838
	.set sub_080d4bc4, 0x080d4bc4
	.set sub_080d4ccc, 0x080d4ccc
	.set sub_081c0010, 0x081c0010
	.global Overlay_080d4d08
Overlay_080d4d08:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #844]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	sub	sp, #24
	bl	sub_080d2d84
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #8]
	adds	r6, r0, #0
	ldrh	r2, [r6, #6]
	movs	r3, #85
	adds	r3, r3, r6
	mov	sl, r2
	ldrb	r2, [r3, #0]
	mov	r9, r3
	str	r2, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #128
	mov	fp, r3
	movs	r3, #1
	str	r3, [sp, #0]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r5, r5, r2
	ldrb	r3, [r5, #0]
	cmp	r3, #2
	bne.n	.L_080d4d58
	b.n	.L_080d50e2
.L_080d4d58:
	cmp	r3, #3
	bne.n	.L_080d4d5e
	b.n	.L_080d50e2
.L_080d4d5e:
	movs	r3, #128
	lsls	r3, r3, #5
	movs	r2, #128
	add	r3, sl
	lsls	r2, r2, #6
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d4d70
	b.n	.L_080d50e2
.L_080d4d70:
	movs	r3, #128
	lsls	r3, r3, #6
	add	sl, r3
	movs	r3, #192
	mov	r5, sl
	lsls	r3, r3, #8
	ands	r5, r3
	mov	sl, r5
	add	r7, sp, #12
.L_080d4d82:
	ldr	r3, [r6, #8]
	ldr	r5, [pc, #736]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r5
	adds	r3, r3, r2
	str	r3, [r7, #0]
	movs	r0, #128
	ldr	r3, [r6, #12]
	lsls	r0, r0, #13
	str	r3, [r7, #4]
	mov	r1, sl
	ldr	r3, [r6, #16]
	ands	r3, r5
	adds	r3, r3, r2
	str	r3, [r7, #8]
	adds	r2, r7, #0
	bl	sub_0801489c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_08020210
	cmp	r0, #1
	bne.n	.L_080d4dba
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080d50e4
.L_080d4dba:
	ldr	r3, [r6, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r5
	adds	r3, r3, r2
	str	r3, [r7, #0]
	movs	r0, #128
	ldr	r3, [r6, #12]
	lsls	r0, r0, #14
	str	r3, [r7, #4]
	mov	r1, sl
	ldr	r3, [r6, #16]
	ands	r3, r5
	adds	r3, r3, r2
	str	r3, [r7, #8]
	adds	r2, r7, #0
	bl	sub_0801489c
	movs	r3, #0
	adds	r0, r6, #0
	adds	r1, r7, #0
	mov	r8, r3
	bl	sub_080d4ccc
	cmp	r0, #0
	bne.n	.L_080d4dfe
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_08020210
	cmp	r0, #0
	bne.n	.L_080d4dfe
	movs	r5, #1
	mov	r8, r5
.L_080d4dfe:
	adds	r3, r6, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d4e82
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #20]
	ldr	r2, [pc, #600]
	adds	r1, r0, #0
	movs	r4, #0
	mov	ip, r2
	adds	r1, #86
.L_080d4e1c:
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_080d4e78
	adds	r3, r0, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d4e78
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_080d4e78
	cmp	r0, r6
	beq.n	.L_080d4e78
	ldr	r3, [r7, #0]
	ldr	r2, [r0, #8]
	movs	r5, #128
	subs	r3, r3, r2
	lsls	r5, r5, #12
	adds	r3, r3, r5
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080d4e78
	ldr	r3, [r7, #8]
	ldr	r2, [r0, #16]
	subs	r3, r3, r2
	adds	r3, r3, r5
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080d4e78
	ldr	r3, [r0, #12]
	ldr	r2, [r7, #4]
	ldr	r5, [pc, #520]
	subs	r2, r2, r3
	adds	r3, r2, r5
	cmp	r3, #0
	bge.n	.L_080d4e6e
	movs	r3, #128
	lsls	r3, r3, #13
	subs	r3, r3, r2
.L_080d4e6e:
	cmp	r3, ip
	bgt.n	.L_080d4e78
	movs	r2, #1
	mov	r8, r2
	b.n	.L_080d4e82
.L_080d4e78:
	adds	r4, #1
	adds	r1, #128
	adds	r0, #128
	cmp	r4, #63
	ble.n	.L_080d4e1c
.L_080d4e82:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #20]
	ldr	r5, [pc, #480]
	movs	r3, #89
	adds	r3, r3, r0
	movs	r4, #0
	mov	ip, r3
.L_080d4e92:
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_080d4ef2
	adds	r3, r0, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d4ef2
	cmp	r0, r6
	beq.n	.L_080d4ef2
	mov	r2, ip
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080d4ef2
	ldr	r2, [r0, #8]
	ldr	r3, [r7, #0]
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080d4ef2
	ldr	r2, [r0, #16]
	ldr	r3, [r7, #8]
	subs	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080d4ef2
	ldr	r1, [r7, #4]
	ldr	r3, [r0, #12]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_080d4ee6
	cmp	r2, r5
	bgt.n	.L_080d4ef2
	b.n	.L_080d4eec
.L_080d4ee6:
	subs	r3, r3, r1
	cmp	r3, r5
	bgt.n	.L_080d4ef2
.L_080d4eec:
	movs	r3, #0
	mov	r8, r3
	b.n	.L_080d4efe
.L_080d4ef2:
	movs	r2, #128
	adds	r4, #1
	add	ip, r2
	adds	r0, #128
	cmp	r4, #63
	ble.n	.L_080d4e92
.L_080d4efe:
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_080d4f06
	b.n	.L_080d50d2
.L_080d4f06:
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d4f16
	ldr	r3, [r6, #80]
	ldrb	r3, [r3, #26]
	str	r3, [sp, #0]
.L_080d4f16:
	bl	sub_080d22a8
	movs	r1, #6
	adds	r0, r6, #0
	bl	sub_08020090
	movs	r0, #6
	bl	sub_08013560
	movs	r0, #152
	bl	sub_081c0010
	adds	r0, r6, #0
	movs	r1, #7
	bl	sub_08020090
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r6, #48]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	mov	r5, r9
	ldrb	r2, [r5, #0]
	movs	r3, #126
	ands	r3, r2
	strb	r3, [r5, #0]
	ldr	r2, [sp, #0]
	movs	r1, #254
	ands	r1, r2
	adds	r0, r6, #0
	bl	sub_08020218
	ldr	r3, [pc, #260]
	movs	r5, #133
	lsls	r5, r5, #2
	adds	r3, r3, r5
	ldr	r0, [r3, #0]
	movs	r2, #2
	ldrsh	r1, [r7, r2]
	movs	r3, #10
	ldrsh	r2, [r7, r3]
	bl	sub_080d2ea4
	adds	r0, r6, #0
	movs	r1, #6
	bl	sub_08020090
	adds	r0, r6, #0
	ldr	r1, [sp, #0]
	bl	sub_08020218
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	movs	r3, #12
	ldrb	r1, [r0, #23]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080d4fe2
	ldr	r3, [r6, #8]
	asrs	r5, r3, #20
	ldr	r3, [r6, #16]
	asrs	r4, r3, #20
	cmp	r0, #0
	bne.n	.L_080d4faa
	movs	r3, #34
	adds	r3, r3, r6
	ldr	r2, [pc, #200]
	mov	r8, r3
	b.n	.L_080d4fc4
.L_080d4faa:
	movs	r2, #34
	adds	r2, r2, r6
	ldrb	r3, [r2, #0]
	mov	r8, r2
	movs	r2, #3
	ands	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [r0, r3]
.L_080d4fc4:
	lsls	r3, r4, #7
	adds	r3, r5, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r2, [r2, #3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d4fe8
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	adds	r0, r6, #0
	bl	sub_080203a0
	b.n	.L_080d4fe8
.L_080d4fe2:
	movs	r3, #34
	adds	r3, r3, r6
	mov	r8, r3
.L_080d4fe8:
	movs	r1, #129
	lsls	r1, r1, #1
	adds	r0, r6, #0
	bl	sub_080d4bc4
	cmp	r0, #0
	bne.n	.L_080d5004
	movs	r1, #4
	adds	r1, #255
	adds	r0, r6, #0
	bl	sub_080d4bc4
	cmp	r0, #0
	beq.n	.L_080d5078
.L_080d5004:
	adds	r3, r0, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d5078
	movs	r1, #7
	bl	sub_08020090
	ldr	r5, [pc, #88]
	ldr	r3, [r6, #12]
	movs	r0, #2
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r3, r3, r5
	str	r3, [r6, #20]
	bl	sub_08013560
	ldr	r3, [r6, #12]
	movs	r0, #10
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r3, r3, r5
	str	r3, [r6, #20]
	bl	sub_08013560
	ldr	r3, [r6, #12]
	movs	r5, #128
	lsls	r5, r5, #9
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	movs	r0, #4
	adds	r3, r3, r5
	str	r3, [r6, #20]
	bl	sub_08013560
	ldr	r3, [r6, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r3, r3, r5
	str	r3, [r6, #20]
	b.n	.L_080d507e
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x0007ffff
	.4byte 0x02010000
	.2byte 0x0000
	.2byte 0xffff
.L_080d5078:
	.2byte 0x2006
	bl	sub_08013560
.L_080d507e:
	add	r5, sp, #4
	ldrb	r5, [r5, #0]
	mov	r2, r9
	strb	r5, [r2, #0]
	bl	sub_080d2350
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L_080d50ae
	movs	r3, #206
	lsls	r3, r3, #1
	add	r3, fp
	movs	r1, #128
	ldr	r0, [r3, #0]
	lsls	r1, r1, #14
	ldr	r3, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22d0
	lsls	r2, r2, #1
	add	r2, fp
	ldr	r3, [r2, #0]
	adds	r3, r3, r0
	str	r3, [r2, #0]
.L_080d50ae:
	mov	r3, r8
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl	sub_080201c8
	cmp	r0, #254
	bne.n	.L_080d50ce
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08020090
	movs	r0, #6
	bl	sub_08013560
	b.n	.L_080d4d82
.L_080d50ce:
	movs	r5, #0
	str	r5, [sp, #8]
.L_080d50d2:
	adds	r2, r6, #0
	movs	r3, #8
	adds	r2, #100
	strh	r3, [r2, #0]
	adds	r3, r6, #0
	movs	r1, #0
	adds	r3, #102
	strh	r1, [r3, #0]
.L_080d50e2:
	ldr	r0, [sp, #8]
.L_080d50e4:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #560]
	movs	r3, #133
	mov	fp, r0
	lsls	r3, r3, #2
	add	r3, fp
	ldr	r0, [r3, #0]
	sub	sp, #32
	bl	sub_080d2d84
	movs	r1, #192
	lsls	r1, r1, #18
	ldr	r1, [r1, #32]
	movs	r2, #1
	str	r2, [sp, #8]
	str	r1, [sp, #16]
	adds	r7, r0, #0
	movs	r3, #10
	ldrsh	r5, [r7, r3]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #240
	ands	r5, r3
	adds	r0, r5, #0
	adds	r0, #8
	str	r0, [sp, #12]
	movs	r2, #8
	movs	r1, #14
	ldrsh	r6, [r7, r1]
	movs	r1, #18
	ldrsh	r0, [r7, r1]
	ands	r6, r3
	ands	r0, r3
	adds	r2, r2, r6
	mov	r8, r0
	mov	sl, r2
	movs	r2, #8
	add	r2, r8
	mov	r9, r2
	bl	sub_080d22a8
	adds	r3, r7, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d5166
	ldr	r3, [r7, #80]
	ldrb	r3, [r3, #26]
	str	r3, [sp, #8]
.L_080d5166:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	add	fp, r3
	mov	r0, fp
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_080d5178
	b.n	.L_080d5344
.L_080d5178:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bge.n	.L_080d5182
	adds	r2, r5, #0
	adds	r2, #23
.L_080d5182:
	asrs	r2, r2, #4
	mov	r3, sl
	mov	ip, r2
	cmp	r3, #0
	bge.n	.L_080d5190
	adds	r3, r6, #0
	adds	r3, #23
.L_080d5190:
	asrs	r3, r3, #4
	mov	r2, r9
	mov	lr, r3
	cmp	r2, #0
	bge.n	.L_080d519e
	mov	r2, r8
	adds	r2, #23
.L_080d519e:
	adds	r6, r7, #0
	adds	r6, #34
	ldrb	r0, [r6, #0]
	movs	r1, #156
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r1, [sp, #16]
	asrs	r5, r2, #4
	ldr	r3, [r1, r3]
	lsls	r2, r5, #7
	add	r2, ip
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r1, r3, r2
	ldrb	r2, [r3, #3]
	movs	r4, #64
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d51d2
	b.n	.L_080d53a0
.L_080d51d2:
	ldrb	r2, [r1, #3]
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d51de
	b.n	.L_080d53a0
.L_080d51de:
	ldr	r3, [sp, #16]
	movs	r1, #212
	lsls	r1, r1, #1
	adds	r2, r3, r1
	mov	r1, lr
	subs	r3, r5, r1
	lsls	r3, r3, #7
	ldr	r2, [r2, #0]
	add	r3, ip
	lsls	r3, r3, #2
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r1, r2, r3
	ldrb	r3, [r1, #2]
	cmp	r3, #255
	bne.n	.L_080d5202
	b.n	.L_080d53a0
.L_080d5202:
	ldr	r1, [sp, #12]
	mov	r2, r9
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	mov	fp, r1
	str	r2, [sp, #4]
	bl	sub_080201c0
	mov	r2, r8
	adds	r2, #24
	adds	r5, r0, #0
	lsls	r2, r2, #16
	ldrb	r0, [r6, #0]
	mov	r1, fp
	bl	sub_080201c0
	cmp	r5, r0
	bgt.n	.L_080d5228
	b.n	.L_080d53a0
.L_080d5228:
	ldr	r3, [r7, #8]
	add	r6, sp, #20
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	ldr	r0, [pc, #264]
	adds	r3, r3, r0
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	sub_08020258
	cmp	r0, #0
	beq.n	.L_080d5246
	b.n	.L_080d53a0
.L_080d5246:
	ldrh	r3, [r7, #32]
	ldr	r1, [pc, #244]
	subs	r3, #2
	mov	sl, r3
	ldr	r3, [r7, #8]
	movs	r2, #128
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	movs	r0, #89
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r4, #0
	adds	r0, r0, r5
	mov	r8, r0
.L_080d5270:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080d52a0
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d52a0
	cmp	r5, r7
	beq.n	.L_080d52a0
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	mov	r1, sl
	adds	r2, r6, #0
	str	r4, [sp, #0]
	bl	sub_08020348
	ldr	r4, [sp, #0]
	cmp	r0, #0
	blt.n	.L_080d52a0
	b.n	.L_080d53a0
.L_080d52a0:
	movs	r2, #128
	adds	r4, #1
	add	r8, r2
	adds	r5, #128
	cmp	r4, #63
	ble.n	.L_080d5270
	ldr	r6, [pc, #136]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r6, r3
	ldr	r1, [sp, #12]
	ldr	r0, [r5, #0]
	mov	r2, r9
	bl	sub_080d2ea4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl	sub_080d3838
	ldr	r0, [r5, #0]
	bl	sub_080d2e40
	adds	r1, r7, #0
	adds	r1, #90
	movs	r3, #1
	strb	r3, [r1, #0]
	adds	r3, r7, #0
	movs	r2, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	ldr	r0, [sp, #8]
	movs	r3, #254
	ands	r0, r3
	str	r0, [sp, #8]
	ldr	r1, [sp, #8]
	adds	r0, r7, #0
	bl	sub_08020218
	adds	r0, r7, #0
	movs	r1, #13
	bl	sub_08020090
	ldr	r1, [pc, #60]
	ldr	r0, [sp, #4]
	ldr	r2, [r7, #12]
	mov	r8, r1
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r0, r1
	add	r2, r8
	mov	r1, fp
	adds	r0, r7, #0
	bl	sub_08020148
	ldr	r0, [r5, #0]
	bl	sub_080d30fc
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	movs	r3, #2
	adds	r6, r6, r2
	strb	r3, [r6, #0]
	ldr	r3, [r7, #16]
	mov	r0, r8
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r7, #16]
	b.n	.L_080d5398
	.4byte 0x02000240
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0xffe0
.L_080d5344:
	.2byte 0x1c38
	movs	r1, #10
	bl	sub_08020090
	adds	r2, r7, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	ldr	r3, [r7, #12]
	ldr	r6, [pc, #52]
	str	r3, [r7, #20]
	ldr	r2, [sp, #8]
	adds	r0, r7, #0
	orrs	r2, r6
	adds	r1, r2, #0
	str	r2, [sp, #8]
	bl	sub_08020218
	movs	r0, #6
	bl	sub_080d2240
	movs	r5, #0
	mov	r3, fp
	strb	r5, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #90
	strb	r6, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r7, #6]
	adds	r3, r7, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	adds	r3, #2
	strh	r5, [r3, #0]
	b.n	.L_080d5398
	movs	r0, r0
	.2byte 0x0001
	.2byte 0x0000
.L_080d5398:
	bl	sub_080d2350
	movs	r0, #0
	b.n	.L_080d53a8
.L_080d53a0:
	bl	sub_080d2350
	movs	r0, #1
	negs	r0, r0
.L_080d53a8:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
