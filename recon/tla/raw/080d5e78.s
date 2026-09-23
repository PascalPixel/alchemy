.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080140d8, 0x080140d8
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014d00, 0x08014d00
	.set sub_0801591c, 0x0801591c
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016d18, 0x08016d18
	.set sub_08020058, 0x08020058
	.set sub_08020070, 0x08020070
	.set sub_08020090, 0x08020090
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_080201c0, 0x080201c0
	.set sub_080cad84, 0x080cad84
	.set sub_080d170c, 0x080d170c
	.set sub_080d2240, 0x080d2240
	.set sub_080d2c64, 0x080d2c64
	.set sub_080d4384, 0x080d4384
	.set sub_080d440c, 0x080d440c
	.set sub_080dc390, 0x080dc390
	.set sub_081c0010, 0x081c0010
	.global Func_080d5e78
	.thumb_func
Func_080d5e78:
.L_080d5e78:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #136]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #12
	mov	r8, r2
	adds	r1, #255
	mov	r0, r8
	bl	sub_08020058
	movs	r3, #0
	mov	sl, r3
	mov	r1, sl
	mov	r2, r8
	strb	r1, [r2, #26]
	movs	r3, #15
	ldr	r2, [pc, #96]
	strb	r3, [r0, #5]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r3, r2
	mov	r9, r1
	add	r3, r9
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	adds	r1, r6, #0
	ands	r3, r2
	str	r3, [r5, #16]
	adds	r0, r5, #0
	bl	sub_08020090
	movs	r0, #30
	bl	sub_08013560
	movs	r6, #1
	mov	r2, r8
	strb	r6, [r2, #27]
	ldr	r0, [r2, #44]
	bl	sub_08020070
	mov	r1, r8
	strb	r6, [r1, #26]
	mov	r3, sl
	str	r3, [r1, #44]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	str	r3, [r5, #48]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	add	r3, r9
	bl	sub_08020148
	adds	r0, r5, #0
	bl	sub_08020150
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb560
	movs	r5, #144
	lsls	r5, r5, #1
	adds	r0, r5, #0
	movs	r6, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d5f3a
	movs	r0, #24
	bl	.L_080d5e78
	adds	r0, r5, #0
	bl	sub_08016d18
	movs	r6, #1
	b.n	.L_080d5fcc
.L_080d5f3a:
	movs	r5, #34
	adds	r5, #255
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d5f58
	movs	r0, #23
	bl	.L_080d5e78
	adds	r0, r5, #0
	bl	sub_08016d18
	movs	r6, #2
	b.n	.L_080d5fcc
.L_080d5f58:
	movs	r5, #145
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d5fcc
	adds	r0, r5, #0
	bl	sub_08016d18
	ldr	r3, [pc, #96]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r6, [r3, #0]
	adds	r0, r6, #0
	bl	sub_080cad84
	adds	r5, r0, #0
	ldr	r3, [r5, #12]
	movs	r2, #160
	lsls	r2, r2, #16
	adds	r3, r3, r2
	movs	r2, #1
	negs	r2, r2
	str	r3, [r5, #12]
	adds	r0, r2, #0
	adds	r1, r2, #0
	movs	r3, #0
	bl	sub_080d440c
	b.n	.L_080d5f9e
.L_080d5f98:
	movs	r0, #1
	bl	sub_08013560
.L_080d5f9e:
	ldr	r2, [r5, #40]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	ldr	r2, [r5, #20]
	cmp	r3, r2
	bgt.n	.L_080d5f98
	movs	r0, #159
	bl	sub_081c0010
	ldr	r3, [r5, #20]
	movs	r1, #22
	str	r3, [r5, #12]
	adds	r0, r5, #0
	bl	sub_08020090
	movs	r0, #15
	bl	sub_080d2240
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080d4384
	movs	r6, #3
.L_080d5fcc:
	adds	r0, r6, #0
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	movs	r1, #134
	lsls	r1, r1, #1
	ldr	r0, [r0, #80]
	bl	sub_08020058
	movs	r3, #15
	strb	r3, [r0, #5]
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r3, r0, #0
	adds	r3, #136
	ldr	r1, [r3, #0]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r3, r1, r2
	ldrb	r2, [r3, #0]
	movs	r5, #128
	lsls	r3, r2, #4
	subs	r3, r3, r2
	ldr	r2, [r0, #108]
	lsls	r3, r3, #7
	lsls	r5, r5, #19
	adds	r4, r1, r3
	adds	r5, #20
	cmp	r2, #0
	beq.n	.L_080d6040
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080d6076
	movs	r1, #178
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_080d6076
.L_080d6040:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	adds	r1, r5, #0
	ldrh	r2, [r3, #10]
	ldmia	r4!, {r2}
	str	r2, [r5, #0]
	ldmia	r4!, {r2}
	str	r2, [r5, #0]
	ldmia	r4!, {r2}
	str	r2, [r5, #0]
	adds	r0, r4, #0
	ldr	r2, [pc, #4]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080d6076:
	pop	{r5, pc}
	.2byte 0x0003
	.2byte 0xa660
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #136
	ldr	r4, [r3, #0]
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #1
	adds	r3, r4, r0
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #860]
	sub	sp, #36
	movs	r5, #4
	ldrsh	r1, [r3, r5]
	str	r1, [sp, #32]
	movs	r1, #6
	ldrsh	r6, [r3, r1]
	str	r6, [sp, #28]
	movs	r6, #8
	ldrsh	r5, [r3, r6]
	mov	fp, r5
	movs	r5, #10
	ldrsh	r1, [r3, r5]
	str	r1, [sp, #24]
	movs	r6, #14
	ldrsh	r5, [r3, r6]
	movs	r6, #12
	ldrsh	r1, [r3, r6]
	str	r5, [sp, #20]
	ldr	r3, [r2, #108]
	cmp	r3, #0
	beq.n	.L_080d60de
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080d60de
	b.n	.L_080d63ec
.L_080d60de:
	cmp	r0, #0
	beq.n	.L_080d60e4
	b.n	.L_080d626c
.L_080d60e4:
	movs	r5, #240
	lsls	r5, r5, #4
	adds	r3, r4, r5
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r6, #241
	lsls	r3, r3, #7
	lsls	r6, r6, #4
	adds	r5, r4, r3
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #2
	mov	r8, r3
	adds	r3, r4, r0
	ldrh	r2, [r3, #0]
	ldr	r3, [sp, #28]
	subs	r6, #8
	lsls	r3, r3, #16
	str	r3, [sp, #16]
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	mov	r0, r8
	adds	r7, r3, #0
	muls	r7, r2
	cmp	r0, #0
	bne.n	.L_080d6140
	movs	r6, #0
	adds	r3, r5, #0
.L_080d612a:
	add	r2, sp, #32
	ldrh	r2, [r2, #0]
	mov	r5, fp
	adds	r6, #1
	strh	r2, [r3, #0]
	strh	r5, [r3, #4]
	strh	r1, [r3, #8]
	adds	r3, #12
	cmp	r6, #160
	bne.n	.L_080d612a
	b.n	.L_080d61a2
.L_080d6140:
	movs	r6, #240
	lsls	r6, r6, #4
	adds	r6, #24
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #32]
	mov	r2, fp
	lsls	r2, r2, #16
	mov	r9, r3
	str	r2, [sp, #8]
	lsls	r3, r0, #16
	lsls	r1, r1, #16
	lsrs	r3, r3, #16
	movs	r6, #0
	mov	sl, r1
	mov	fp, r3
.L_080d6160:
	movs	r2, #255
	ldr	r1, [pc, #668]
	asrs	r3, r7, #16
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r0, [r1, r3]
	str	r4, [sp, #0]
	mov	r1, r9
	ldr	r3, [pc, #656]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c00
	cmp	r0, #0
	bge.n	.L_080d617e
	adds	r0, #255
.L_080d617e:
	lsls	r2, r0, #8
	lsrs	r2, r2, #16
	mov	r0, fp
	adds	r3, r0, r2
	strh	r3, [r5, #0]
	ldr	r1, [sp, #8]
	mov	r0, sl
	lsrs	r3, r1, #16
	adds	r3, r3, r2
	strh	r3, [r5, #4]
	lsrs	r3, r0, #16
	adds	r3, r3, r2
	adds	r6, #1
	strh	r3, [r5, #8]
	add	r7, r8
	adds	r5, #12
	cmp	r6, #160
	bne.n	.L_080d6160
.L_080d61a2:
	movs	r1, #240
	lsls	r1, r1, #4
	adds	r3, r4, r1
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r2, #240
	lsls	r3, r3, #7
	lsls	r2, r2, #4
	adds	r3, r4, r3
	adds	r2, #20
	adds	r5, r3, #2
	adds	r3, r4, r2
	ldr	r3, [r3, #0]
	movs	r6, #240
	lsls	r6, r6, #4
	adds	r6, #2
	mov	r8, r3
	ldr	r0, [sp, #16]
	adds	r3, r4, r6
	ldrh	r2, [r3, #0]
	adds	r1, #12
	lsrs	r3, r0, #16
	adds	r2, r2, r3
	adds	r3, r4, r1
	ldr	r3, [r3, #0]
	adds	r7, r3, #0
	muls	r7, r2
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080d6204
	movs	r6, #0
	adds	r3, r5, #0
.L_080d61e8:
	add	r5, sp, #28
	add	r0, sp, #24
	add	r1, sp, #20
	ldrh	r5, [r5, #0]
	ldrh	r0, [r0, #0]
	ldrh	r1, [r1, #0]
	adds	r6, #1
	strh	r5, [r3, #0]
	strh	r0, [r3, #4]
	strh	r1, [r3, #8]
	adds	r3, #12
	cmp	r6, #160
	bne.n	.L_080d61e8
	b.n	.L_080d63d0
.L_080d6204:
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #28
	adds	r3, r4, r2
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #20]
	mov	r9, r3
	ldr	r3, [sp, #24]
	ldr	r1, [sp, #16]
	lsls	r3, r3, #16
	str	r3, [sp, #4]
	lsls	r0, r0, #16
	lsrs	r1, r1, #16
	movs	r6, #0
	mov	sl, r0
	mov	fp, r1
.L_080d6224:
	movs	r2, #255
	asrs	r3, r7, #16
	ands	r3, r2
	ldr	r2, [pc, #468]
	lsls	r3, r3, #1
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #0]
	mov	r1, r9
	ldr	r2, [pc, #460]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x9c00
	cmp	r0, #0
	bge.n	.L_080d6242
	adds	r0, #255
.L_080d6242:
	lsls	r2, r0, #8
	lsrs	r2, r2, #16
	mov	r0, fp
	adds	r3, r0, r2
	strh	r3, [r5, #0]
	ldr	r1, [sp, #4]
	adds	r5, #4
	lsrs	r3, r1, #16
	adds	r3, r3, r2
	mov	r0, sl
	strh	r3, [r5, #0]
	lsrs	r3, r0, #16
	adds	r5, #4
	adds	r3, r3, r2
	adds	r6, #1
	strh	r3, [r5, #0]
	add	r7, r8
	adds	r5, #4
	cmp	r6, #160
	bne.n	.L_080d6224
	b.n	.L_080d63d0
.L_080d626c:
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r6, #241
	lsls	r3, r3, #7
	lsls	r6, r6, #4
	adds	r5, r4, r3
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #2
	mov	r8, r3
	adds	r3, r4, r0
	ldrh	r2, [r3, #0]
	ldr	r3, [sp, #28]
	subs	r6, #8
	lsls	r3, r3, #16
	str	r3, [sp, #12]
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	mov	r0, r8
	adds	r7, r3, #0
	muls	r7, r2
	cmp	r0, #0
	bne.n	.L_080d62c8
	movs	r6, #0
	adds	r3, r5, #0
.L_080d62b2:
	add	r2, sp, #32
	ldrh	r2, [r2, #0]
	mov	r5, fp
	adds	r6, #1
	strh	r2, [r3, #0]
	strh	r5, [r3, #4]
	strh	r1, [r3, #8]
	adds	r3, #12
	cmp	r6, #160
	bne.n	.L_080d62b2
	b.n	.L_080d631a
.L_080d62c8:
	movs	r6, #240
	lsls	r6, r6, #4
	adds	r6, #24
	adds	r3, r4, r6
	lsls	r1, r1, #16
	mov	sl, r1
	ldr	r3, [r3, #0]
	mov	r0, sl
	lsrs	r0, r0, #16
	mov	r9, r3
	movs	r6, #0
	mov	sl, r0
.L_080d62e0:
	movs	r2, #255
	ldr	r1, [pc, #284]
	asrs	r3, r7, #16
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r0, [r1, r3]
	str	r4, [sp, #0]
	mov	r1, r9
	ldr	r3, [pc, #272]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c00
	cmp	r0, #0
	bge.n	.L_080d62fe
	adds	r0, #255
.L_080d62fe:
	lsls	r3, r0, #8
	add	r0, sp, #32
	ldrh	r0, [r0, #0]
	lsrs	r3, r3, #16
	mov	r1, fp
	add	r3, sl
	adds	r6, #1
	strh	r0, [r5, #0]
	strh	r1, [r5, #4]
	strh	r3, [r5, #8]
	add	r7, r8
	adds	r5, #12
	cmp	r6, #160
	bne.n	.L_080d62e0
.L_080d631a:
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r6, #240
	lsls	r3, r3, #7
	lsls	r6, r6, #4
	adds	r3, r4, r3
	adds	r6, #20
	adds	r5, r3, #2
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #2
	mov	r8, r3
	ldr	r1, [sp, #12]
	adds	r3, r4, r0
	ldrh	r2, [r3, #0]
	subs	r6, #8
	lsrs	r3, r1, #16
	adds	r2, r2, r3
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	mov	r0, r8
	adds	r7, r3, #0
	muls	r7, r2
	cmp	r0, #0
	bne.n	.L_080d637c
	movs	r6, #0
	adds	r3, r5, #0
.L_080d6360:
	add	r1, sp, #28
	add	r2, sp, #24
	add	r5, sp, #20
	ldrh	r1, [r1, #0]
	ldrh	r2, [r2, #0]
	ldrh	r5, [r5, #0]
	adds	r6, #1
	strh	r1, [r3, #0]
	strh	r2, [r3, #4]
	strh	r5, [r3, #8]
	adds	r3, #12
	cmp	r6, #160
	bne.n	.L_080d6360
	b.n	.L_080d63d0
.L_080d637c:
	movs	r6, #240
	lsls	r6, r6, #4
	adds	r6, #28
	ldr	r0, [sp, #20]
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #120]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	r9, r3
	movs	r6, #0
	mov	fp, r1
	mov	sl, r0
.L_080d6396:
	movs	r2, #255
	asrs	r3, r7, #16
	ands	r3, r2
	ldr	r2, [pc, #96]
	lsls	r3, r3, #1
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #0]
	mov	r1, r9
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9c00
	cmp	r0, #0
	bge.n	.L_080d63b2
	adds	r0, #255
.L_080d63b2:
	lsls	r3, r0, #8
	add	r2, sp, #28
	add	r0, sp, #24
	ldrh	r2, [r2, #0]
	ldrh	r0, [r0, #0]
	lsrs	r3, r3, #16
	add	r3, sl
	adds	r6, #1
	strh	r2, [r5, #0]
	strh	r0, [r5, #4]
	strh	r3, [r5, #8]
	add	r7, r8
	adds	r5, #12
	cmp	r6, #160
	bne.n	.L_080d6396
.L_080d63d0:
	movs	r1, #240
	lsls	r1, r1, #4
	adds	r1, #2
	adds	r2, r4, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r1, r4, r2
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
.L_080d63ec:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001120
	.4byte 0x080f08b0
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	movs	r1, #242
	adds	r5, r0, #0
	lsls	r1, r1, #4
	movs	r0, #136
	sub	sp, #4
	mov	r8, r2
	adds	r7, r3, #0
	bl	sub_08014d00
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r4, #0
	ldr	r2, [pc, #120]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	adds	r1, r3, #0
	lsls	r2, r2, #24
.L_080d643e:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d643e
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #1
	adds	r3, r4, r2
	adds	r2, #7
	strb	r5, [r3, #0]
	adds	r3, r4, r2
	str	r6, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #12
	adds	r2, r4, r3
	ldr	r3, [sp, #24]
	movs	r1, #144
	str	r3, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #24
	adds	r3, r4, r2
	str	r7, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #28
	adds	r2, r4, r3
	ldr	r3, [sp, #32]
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	movs	r2, #241
	lsls	r2, r2, #4
	adds	r3, r4, r2
	mov	r2, r8
	str	r2, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #20
	adds	r2, r4, r3
	ldr	r3, [sp, #28]
	ldr	r0, [pc, #28]
	str	r3, [r2, #0]
	bl	sub_080145a8
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x850003c8
	.4byte 0x080d607d
	.2byte 0x5ff9
	.2byte 0x080d
	push	{lr}
	ldr	r0, [pc, #44]
	bl	sub_08014644
	ldr	r0, [pc, #40]
	bl	sub_08014644
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #176
	ldrh	r1, [r2, #10]
	movs	r3, #197
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	movs	r3, #254
	ldrh	r1, [r2, #10]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	pop	{pc}
	.4byte 0x080d5ff9
	.2byte 0x607d
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [r3, #116]
	mov	sl, r1
	mov	r8, r3
	mov	r3, sl
	adds	r3, #228
	ldr	r2, [r3, #0]
	sub	sp, #12
	str	r2, [sp, #8]
	mov	r7, r8
	ldr	r3, [r3, #4]
	movs	r4, #63
	str	r3, [sp, #4]
	movs	r3, #0
	adds	r7, #8
	mov	r9, r3
	mov	fp, r4
.L_080d6524:
	ldrh	r3, [r7, #28]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	adds	r2, r1, #0
	ands	r2, r3
	strh	r3, [r7, #28]
	cmp	r2, r1
	bne.n	.L_080d653a
	b.n	.L_080d6654
.L_080d653a:
	movs	r0, #179
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d654c
	ldrh	r3, [r7, #28]
	adds	r3, #1
	strh	r3, [r7, #28]
.L_080d654c:
	ldrh	r2, [r7, #28]
	ldr	r5, [sp, #8]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #160]
	lsls	r3, r3, #1
	adds	r0, r3, r2
	ldr	r3, [r7, #12]
	subs	r2, r3, r5
	cmp	r2, #0
	bge.n	.L_080d656a
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r2, r2, r1
.L_080d656a:
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	asrs	r2, r2, #16
	adds	r1, r2, r3
	ldr	r3, [r7, #20]
	ldr	r2, [r7, #16]
	ldr	r5, [sp, #4]
	subs	r3, r3, r2
	subs	r3, r3, r5
	adds	r0, #2
	cmp	r3, #0
	bge.n	.L_080d658a
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_080d658a:
	movs	r4, #0
	ldrsh	r2, [r0, r4]
	asrs	r3, r3, #16
	adds	r4, r3, r2
	adds	r3, r1, #0
	adds	r3, #16
	adds	r0, #2
	cmp	r3, #255
	bhi.n	.L_080d661a
	movs	r5, #32
	negs	r5, r5
	cmp	r4, r5
	blt.n	.L_080d661a
	cmp	r4, #159
	bgt.n	.L_080d661a
	ldrb	r2, [r7, #9]
	adds	r5, #19
	adds	r3, r5, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldr	r3, [pc, #48]
	strb	r2, [r7, #9]
	ands	r1, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r7, #6]
	strb	r4, [r7, #4]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #6]
	mov	r2, r8
	ldrh	r3, [r0, #0]
	ldr	r1, [r2, #4]
	ldr	r2, [pc, #32]
	adds	r1, r1, r3
	ldr	r3, [pc, #32]
	adds	r0, #2
	ands	r1, r3
	ldrh	r3, [r7, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	ldrb	r1, [r7, #5]
	ldrb	r2, [r0, #0]
	mov	r3, fp
	b.n	.L_080d65fc
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.2byte 0x0ab0
	.2byte 0x080f
.L_080d65fc:
	lsls	r2, r2, #6
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #5]
	ldrb	r1, [r7, #7]
	ldrb	r2, [r0, #2]
	mov	r3, fp
	ands	r3, r1
	lsls	r2, r2, #6
	orrs	r3, r2
	strb	r3, [r7, #7]
	adds	r0, r7, #0
	movs	r1, #240
	bl	sub_080140d8
.L_080d661a:
	ldrh	r3, [r7, #28]
	cmp	r3, #0
	bne.n	.L_080d6654
	mov	r3, sl
	ldr	r6, [r3, #0]
	bl	sub_08014878
	ldr	r3, [r6, #0]
	ldr	r5, [pc, #68]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	adds	r1, r3, r5
	str	r1, [sp, #0]
	bl	sub_08014878
	ldr	r3, [r6, #8]
	lsls	r0, r0, #8
	ldr	r1, [sp, #0]
	adds	r3, r3, r0
	adds	r4, r3, r5
	str	r1, [r7, #12]
	str	r4, [r7, #20]
	movs	r0, #0
	adds	r2, r4, #0
	bl	sub_080201c0
	movs	r3, #16
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
.L_080d6654:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	adds	r7, #32
	cmp	r5, #63
	bhi.n	.L_080d6662
	b.n	.L_080d6524
.L_080d6662:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xff80
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	ldr	r3, [r3, #116]
	mov	sl, r0
	mov	r8, r3
	mov	r3, sl
	adds	r3, #228
	ldr	r1, [r3, #0]
	sub	sp, #4
	str	r1, [sp, #0]
	mov	r6, r8
	ldr	r3, [r3, #4]
	movs	r2, #0
	adds	r6, #8
	mov	fp, r3
	mov	r9, r2
.L_080d66a4:
	ldrh	r3, [r6, #28]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	adds	r2, r1, #0
	ands	r2, r3
	strh	r3, [r6, #28]
	cmp	r2, r1
	bne.n	.L_080d66ba
	b.n	.L_080d67c4
.L_080d66ba:
	movs	r0, #179
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d66cc
	ldrh	r3, [r6, #28]
	adds	r3, #1
	strh	r3, [r6, #28]
.L_080d66cc:
	ldrh	r3, [r6, #28]
	ldr	r2, [pc, #164]
	lsls	r3, r3, #2
	adds	r1, r3, r2
	ldr	r0, [sp, #0]
	ldr	r3, [r6, #12]
	subs	r2, r3, r0
	cmp	r2, #0
	bge.n	.L_080d66e6
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_080d66e6:
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	asrs	r2, r2, #16
	adds	r7, r2, r3
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #20]
	adds	r1, #2
	subs	r3, r3, r2
	mov	r2, fp
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_080d6706
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r3, r0
.L_080d6706:
	movs	r0, #0
	ldrsh	r2, [r1, r0]
	asrs	r3, r3, #16
	adds	r4, r3, r2
	adds	r3, r7, #0
	adds	r3, #16
	cmp	r3, #255
	bhi.n	.L_080d678a
	movs	r1, #32
	negs	r1, r1
	cmp	r4, r1
	blt.n	.L_080d678a
	cmp	r4, #159
	bgt.n	.L_080d678a
	ldrb	r2, [r6, #9]
	movs	r0, #13
	negs	r0, r0
	adds	r3, r0, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldr	r3, [pc, #48]
	strb	r2, [r6, #9]
	ands	r7, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r6, #6]
	strb	r4, [r6, #4]
	ands	r3, r2
	orrs	r3, r7
	mov	r2, r8
	strh	r3, [r6, #6]
	ldr	r1, [r2, #4]
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #36]
	ands	r1, r3
	ldrh	r3, [r6, #8]
	adds	r0, r6, #0
	ands	r3, r2
	ldrb	r2, [r6, #5]
	orrs	r3, r1
	movs	r1, #63
	strh	r3, [r6, #8]
	adds	r3, r1, #0
	ands	r3, r2
	movs	r2, #64
	orrs	r3, r2
	b.n	.L_080d6778
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x0b50
	.2byte 0x080f
.L_080d6778:
	strb	r3, [r6, #5]
	ldrb	r3, [r6, #7]
	ands	r1, r3
	movs	r3, #128
	orrs	r1, r3
	strb	r1, [r6, #7]
	movs	r1, #240
	bl	sub_080140d8
.L_080d678a:
	ldrh	r3, [r6, #28]
	cmp	r3, #0
	bne.n	.L_080d67c4
	mov	r3, sl
	ldr	r5, [r3, #0]
	bl	sub_08014878
	ldr	r3, [r5, #0]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	ldr	r0, [pc, #64]
	adds	r7, r3, r0
	bl	sub_08014878
	ldr	r3, [r5, #8]
	ldr	r1, [pc, #56]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	adds	r4, r3, r1
	str	r7, [r6, #12]
	str	r4, [r6, #20]
	movs	r0, #0
	adds	r1, r7, #0
	adds	r2, r4, #0
	bl	sub_080201c0
	movs	r3, #12
	str	r0, [r6, #16]
	strh	r3, [r6, #28]
.L_080d67c4:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r6, #32
	cmp	r3, #63
	bhi.n	.L_080d67d2
	b.n	.L_080d66a4
.L_080d67d2:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffc00000
	.2byte 0x0000
	.2byte 0xff80
	.2byte 0xb5e0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #120]
	movs	r1, #252
	lsls	r1, r1, #5
	adds	r3, #128
	adds	r5, r6, r1
	ldr	r7, [r3, #0]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bge.n	.L_080d6804
	b.n	.L_080d6902
.L_080d6804:
	movs	r0, #179
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d6814
	movs	r3, #128
	strh	r3, [r5, #0]
.L_080d6814:
	ldrh	r3, [r5, #0]
	subs	r2, r3, #1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	strh	r2, [r5, #0]
	cmp	r3, #11
	bhi.n	.L_080d6902
	ldr	r2, [pc, #224]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080d685c
	.4byte 0x080d68de
	.4byte 0x080d6902
	.4byte 0x080d6902
	.4byte 0x080d6902
	.4byte 0x080d68b2
	.4byte 0x080d68de
	.4byte 0x080d6902
	.4byte 0x080d6902
	.4byte 0x080d6902
	.4byte 0x080d68b2
	.2byte 0x68de
	.2byte 0x080d
	movs	r1, #248
	lsls	r1, r1, #5
	adds	r1, #130
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080d68b2
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r3, r5, #1
	adds	r3, r3, r5
	movs	r2, #100
	lsls	r3, r3, #3
	muls	r2, r0
	adds	r3, r3, r5
	lsls	r3, r3, #4
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	movs	r1, #252
	subs	r3, r3, r2
	lsls	r1, r1, #5
	adds	r2, r6, r1
	adds	r3, #150
	strh	r3, [r2, #0]
	movs	r2, #248
	lsls	r2, r2, #5
	adds	r2, #132
	adds	r3, r6, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080d68ac
	movs	r0, #172
	bl	sub_081c0010
	b.n	.L_080d68b2
.L_080d68ac:
	movs	r0, #171
	bl	sub_081c0010
.L_080d68b2:
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080d170c
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r0, r6, r2
	movs	r3, #128
	movs	r2, #196
	lsls	r2, r2, #5
	lsls	r3, r3, #19
	adds	r1, r7, r2
	adds	r3, #212
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #168
	lsls	r3, r3, #6
	adds	r3, #1
	adds	r2, r7, r3
	movs	r3, #12
	b.n	.L_080d68f4
	movs	r3, #168
	lsls	r3, r3, #4
	adds	r0, r6, r3
	movs	r1, #1
	bl	sub_080d170c
	movs	r1, #168
	lsls	r1, r1, #6
	adds	r1, #1
	adds	r2, r7, r1
	movs	r3, #1
.L_080d68f4:
	strb	r3, [r2, #0]
	movs	r2, #168
	lsls	r2, r2, #6
	adds	r2, #2
	movs	r1, #0
	adds	r3, r7, r2
	strb	r1, [r3, #0]
.L_080d6902:
	pop	{r5, r6, r7, pc}
	.4byte 0x080d682c
	.2byte 0x02a0
	.2byte 0x8400
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #20
	movs	r0, #116
	sub	sp, #8
	bl	sub_08014d00
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	adds	r5, r0, #0
	ldr	r3, [r3, #0]
	movs	r0, #170
	mov	r8, r3
	bl	sub_080d2c64
	movs	r3, #128
	movs	r6, #0
	adds	r7, r5, #0
	add	r0, sp, #4
	lsls	r3, r3, #19
	adds	r7, #8
	str	r6, [r0, #0]
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #144]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014d00
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #128]
	bl	sub_0801591c
	bl	sub_080143ac
	movs	r1, #192
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	adds	r2, r6, #0
	bl	sub_080142d4
	str	r0, [r5, #4]
	movs	r0, #56
	bl	sub_0801314c
	movs	r5, #0
.L_080d6978:
	movs	r4, #0
	adds	r6, r7, #0
	stmia	r6!, {r4}
	ldr	r3, [pc, #96]
	movs	r0, #0
	stmia	r6!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r6, #0]
	mov	r3, r8
	ldr	r2, [r3, #8]
	ldr	r1, [r3, #0]
	str	r2, [r7, #20]
	str	r1, [r7, #12]
	str	r4, [sp, #0]
	bl	sub_080201c0
	ldr	r2, [pc, #56]
	adds	r3, r5, #0
	ands	r3, r2
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	ldr	r4, [sp, #0]
	adds	r7, #32
	cmp	r5, #63
	bls.n	.L_080d6978
	movs	r3, #128
	movs	r2, #252
	lsls	r3, r3, #19
	lsls	r2, r2, #6
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #8
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r4, [r3, #0]
	ldr	r0, [pc, #24]
	movs	r1, #144
	lsls	r1, r1, #3
	b.n	.L_080d69e8
	movs	r0, r0
	.4byte 0x0000000f
	.4byte 0x85000205
	.4byte 0x080f3798
	.4byte 0x40000400
	.2byte 0x64f1
	.2byte 0x080d
.L_080d69e8:
	bl	sub_080145a8
	add	sp, #8
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
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #116]
	sub	sp, #16
	mov	r9, r3
	movs	r1, #0
	ldr	r3, [pc, #200]
	movs	r2, #4
	str	r1, [sp, #0]
	mov	r5, r9
	add	r2, sp
	adds	r5, #8
	mov	sl, r2
	mov	fp, r3
.L_080d6a1e:
	ldrh	r1, [r5, #28]
	mov	r8, r1
	cmp	r1, #0
	bne.n	.L_080d6a5c
	bl	sub_08014878
	movs	r3, #200
	muls	r3, r0
	movs	r2, #144
	lsls	r2, r2, #17
	adds	r6, r3, r2
	bl	sub_08014878
	lsls	r3, r0, #2
	adds	r3, r3, r0
	movs	r2, #160
	lsls	r3, r3, #2
	lsls	r2, r2, #15
	subs	r7, r2, r3
	str	r6, [r5, #12]
	str	r7, [r5, #20]
	movs	r0, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080201c0
	movs	r3, #120
	strh	r3, [r5, #28]
	mov	r3, r8
	str	r0, [r5, #16]
	str	r3, [r5, #24]
.L_080d6a5c:
	ldr	r2, [r5, #24]
	ldr	r3, [r5, #12]
	mov	r1, sl
	adds	r3, r3, r2
	str	r3, [r1, #0]
	mov	r0, sl
	ldr	r3, [r5, #16]
	str	r3, [r1, #4]
	ldr	r3, [r5, #20]
	str	r3, [r1, #8]
	bl	sub_080dc390
	mov	r3, sl
	movs	r2, #2
	ldrsh	r6, [r3, r2]
	ldrh	r2, [r5, #28]
	movs	r1, #10
	ldrsh	r3, [r3, r1]
	subs	r3, r3, r2
	adds	r7, r3, #0
	bl	sub_08014878
	ldr	r3, [r5, #24]
	movs	r2, #128
	adds	r3, r3, r0
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r5, #24]
	adds	r3, r6, #0
	adds	r3, #16
	subs	r7, #8
	cmp	r3, #255
	bhi.n	.L_080d6b66
	movs	r3, #32
	negs	r3, r3
	cmp	r7, r3
	blt.n	.L_080d6b66
	cmp	r7, #159
	bgt.n	.L_080d6b66
	ldrh	r3, [r5, #28]
	cmp	r3, #59
	bhi.n	.L_080d6ac2
	mov	r1, r9
	ldr	r2, [r1, #4]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	adds	r2, #16
	ands	r2, r3
	ldrh	r3, [r5, #8]
	b.n	.L_080d6aea
.L_080d6ac2:
	cmp	r3, #89
	bhi.n	.L_080d6adc
	mov	r3, r9
	ldr	r2, [r3, #4]
	movs	r1, #192
	ldrh	r3, [r5, #8]
	lsls	r1, r1, #2
	adds	r1, #255
	adds	r2, #8
	b.n	.L_080d6ae8
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0xffff
.L_080d6adc:
	mov	r3, r9
	ldr	r2, [r3, #4]
	movs	r1, #192
	ldrh	r3, [r5, #8]
	lsls	r1, r1, #2
	adds	r1, #255
.L_080d6ae8:
	ands	r2, r1
.L_080d6aea:
	mov	r1, fp
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldr	r3, [pc, #100]
	ldrh	r1, [r5, #8]
	ldr	r2, [r3, #0]
	movs	r3, #1
	lsrs	r2, r2, #3
	ands	r2, r3
	lsls	r3, r1, #22
	lsls	r2, r2, #2
	lsrs	r3, r3, #22
	adds	r3, r3, r2
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #255
	ands	r3, r2
	mov	r2, fp
	ands	r1, r2
	orrs	r1, r3
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #60]
	ands	r6, r3
	ldrh	r3, [r5, #6]
	strh	r1, [r5, #8]
	ands	r3, r2
	orrs	r3, r6
	strh	r3, [r5, #6]
	ldrb	r2, [r5, #5]
	movs	r3, #63
	adds	r1, r3, #0
	ands	r1, r2
	ldrb	r2, [r5, #7]
	strb	r7, [r5, #4]
	ands	r3, r2
	movs	r2, #64
	orrs	r3, r2
	strb	r3, [r5, #7]
	ldrb	r3, [r5, #9]
	movs	r2, #12
	orrs	r3, r2
	movs	r2, #13
	negs	r2, r2
	strb	r3, [r5, #9]
	adds	r3, r2, #0
	ands	r1, r3
	movs	r3, #4
	orrs	r1, r3
	b.n	.L_080d6b5c
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x122c
	.2byte 0x0300
.L_080d6b5c:
	strb	r1, [r5, #5]
	adds	r0, r5, #0
	movs	r1, #240
	bl	sub_080140d8
.L_080d6b66:
	ldrh	r3, [r5, #28]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	strh	r3, [r5, #28]
	ldr	r2, [sp, #0]
	adds	r5, #32
	adds	r2, #1
	str	r2, [sp, #0]
	cmp	r2, #63
	bhi.n	.L_080d6b80
	b.n	.L_080d6a1e
.L_080d6b80:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
