.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_080149e0, 0x080149e0
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020218, 0x08020218
	.set sub_080cad84, 0x080cad84
	.set sub_080db9c0, 0x080db9c0
	.set sub_080db9cc, 0x080db9cc
	.set sub_080dc390, 0x080dc390
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_080ebc62, 0x080ebc62
	.set sub_080ebe94, 0x080ebe94
	.global Overlay_080eb824
Overlay_080eb824:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r1, #196
	movs	r0, #240
	ldr	r5, [r3, #60]
	bl	sub_08014cc0
	movs	r2, #128
	lsls	r2, r2, #5
	mov	r8, r2
	adds	r7, r0, #0
	mov	r1, r8
	movs	r0, #96
	bl	sub_08014cc0
	mov	sl, r0
	ldr	r6, [pc, #92]
	movs	r1, #196
	adds	r0, r7, #0
	movs	r2, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2398
	lsls	r3, r3, #5
	adds	r3, #72
	adds	r5, r5, r3
	ldrh	r3, [r5, #0]
	adds	r5, r7, #0
	adds	r5, #188
	strh	r3, [r5, #0]
	ldr	r2, [pc, #60]
	adds	r3, r7, #0
	movs	r1, #1
	adds	r3, #192
	strb	r1, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	adds	r2, #168
	movs	r3, #16
	str	r3, [r2, #0]
	adds	r3, r7, #0
	movs	r0, #0
	adds	r3, #172
	movs	r2, #128
	str	r0, [r3, #0]
	lsls	r2, r2, #9
	adds	r3, #4
	str	r2, [r3, #0]
	adds	r3, #4
	str	r2, [r3, #0]
	adds	r3, #11
	adds	r2, r7, #0
	strb	r1, [r3, #0]
	adds	r2, #190
	movs	r3, #250
	strb	r3, [r2, #0]
	mov	r1, r8
	mov	r0, sl
	movs	r2, #0
	b.n	.L_080eb8b0
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x0260
	.2byte 0x0300
.L_080eb8b0:
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x8829
	ldr	r2, [pc, #104]
	movs	r3, #128
	lsls	r1, r1, #5
	lsls	r3, r3, #19
	adds	r1, r1, r2
	adds	r3, #212
	mov	r0, sl
	ldr	r2, [pc, #92]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #180
	bl	sub_08014d00
	movs	r6, #0
	movs	r7, #7
.L_080eb8d8:
	asrs	r3, r6, #3
	adds	r4, r6, #0
	movs	r1, #0
	ands	r4, r7
	lsls	r5, r3, #3
.L_080eb8e2:
	asrs	r3, r1, #3
	adds	r3, r5, r3
	lsls	r3, r3, #3
	adds	r2, r1, #0
	adds	r3, r3, r4
	ands	r2, r7
	lsls	r3, r3, #3
	adds	r3, r3, r2
	adds	r1, #1
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r1, #63
	ble.n	.L_080eb8e2
	adds	r6, #1
	cmp	r6, #63
	ble.n	.L_080eb8d8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #32]
	bl	sub_080145a8
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #108
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x06010000
	.4byte 0x84000400
	.2byte 0xb6a1
	.2byte 0x080e
	push	{r0, r2, r4, r7, lr}
	lsrs	r6, r1, #32
	push	{lr}
	ldr	r0, [pc, #36]
	bl	sub_08014644
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #180
	bl	sub_0801314c
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #240
	bl	sub_0801314c
	pop	{pc}
	.2byte 0xb6a1
	.2byte 0x080e
	push	{r0, r2, r4, r7, lr}
	lsrs	r6, r1, #32
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r5, r0, #0
	movs	r0, #92
	sub	sp, #48
	bl	sub_08014cc0
	str	r0, [sp, #32]
	adds	r0, r5, #0
	bl	sub_080cad84
	mov	r8, r0
	bl	sub_080db9cc
	str	r0, [sp, #12]
	mov	r1, r8
	ldrh	r1, [r1, #6]
	ldr	r0, [pc, #368]
	str	r1, [sp, #8]
	bl	sub_08013300
	ldr	r1, [sp, #32]
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r1, r5, #0
	ldr	r2, [sp, #32]
	str	r0, [sp, #24]
	bl	sub_080142d4
	ldr	r3, [sp, #32]
	movs	r1, #176
	str	r0, [sp, #20]
	movs	r2, #0
	lsls	r1, r1, #4
	adds	r7, r3, r5
	mov	sl, r2
	movs	r6, #15
	adds	r5, r3, r1
.L_080eb9c2:
	ldr	r2, [sp, #20]
	mov	r3, sl
	ands	r3, r6
	lsls	r3, r3, #1
	adds	r3, r2, r3
	str	r3, [sp, #0]
	movs	r1, #4
	adds	r0, r5, #0
	movs	r2, #4
	movs	r3, #0
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	mov	r0, r8
	bl	sub_080db9cc
	strh	r0, [r5, #30]
	mov	r0, r8
	bl	sub_080db9c0
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r5, #9]
	movs	r2, #1
	movs	r3, #1
	negs	r3, r3
	add	sl, r2
	str	r3, [r7, #24]
	mov	r3, sl
	adds	r5, #40
	adds	r7, #28
	cmp	r3, #63
	ble.n	.L_080eb9c2
	mov	r1, r8
	movs	r3, #0
	str	r3, [r1, #24]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r1, #28]
	mov	r0, r8
	movs	r1, #0
	bl	sub_08020218
	movs	r3, #0
	movs	r2, #1
	str	r2, [sp, #16]
	str	r3, [sp, #28]
	mov	r9, r3
	mov	fp, r3
.L_080eba40:
	ldr	r1, [sp, #16]
	cmp	r1, #0
	beq.n	.L_080ebaba
	mov	sl, r1
.L_080eba48:
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r2, #192
	mov	r1, r9
	lsls	r2, r2, #12
	lsls	r5, r5, #1
	adds	r5, r5, r2
	lsls	r3, r1, #3
	ldr	r2, [sp, #32]
	subs	r3, r3, r1
	lsls	r3, r3, #2
	movs	r1, #128
	adds	r3, r2, r3
	lsls	r1, r1, #3
	adds	r7, r3, r1
	movs	r3, #0
	str	r3, [r7, #24]
	mov	r2, r8
	ldr	r3, [r2, #8]
	movs	r1, #160
	str	r3, [r7, #0]
	lsls	r1, r1, #13
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r7, #4]
	ldr	r3, [r2, #16]
	str	r3, [r7, #8]
	bl	sub_08014878
	adds	r2, r7, #0
	adds	r1, r6, #0
	bl	sub_0801489c
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	str	r6, [r7, #12]
	str	r5, [r7, #20]
	cmp	r3, #0
	bge.n	.L_080ebaa4
	adds	r3, #63
.L_080ebaa4:
	movs	r2, #1
	asrs	r3, r3, #6
	negs	r2, r2
	lsls	r3, r3, #6
	mov	r1, r9
	add	sl, r2
	subs	r1, r1, r3
	mov	r3, sl
	mov	r9, r1
	cmp	r3, #0
	bne.n	.L_080eba48
.L_080ebaba:
	ldr	r1, [sp, #32]
	movs	r2, #128
	movs	r3, #176
	lsls	r2, r2, #3
	lsls	r3, r3, #4
	adds	r7, r1, r2
	adds	r4, r1, r3
	movs	r1, #63
	mov	sl, r1
.L_080ebacc:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_080ebb7e
	adds	r6, r4, #0
	cmp	r3, #49
	bhi.n	.L_080ebb70
	ldr	r3, [r7, #0]
	add	r5, sp, #36
	ldr	r1, [r7, #12]
	ldr	r0, [r7, #20]
	str	r3, [r5, #0]
	ldr	r3, [r7, #4]
	adds	r2, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r7, #8]
	str	r4, [sp, #4]
	str	r3, [r5, #8]
	bl	sub_0801489c
	mov	r1, r8
	ldr	r2, [r5, #8]
	ldr	r3, [r1, #16]
	ldr	r4, [sp, #4]
	cmp	r2, r3
	ble.n	.L_080ebb08
	ldr	r3, [sp, #12]
	adds	r3, #2
	b.n	.L_080ebb0c
	.2byte 0x01ea
	.2byte 0x0000
.L_080ebb08:
	ldr	r3, [sp, #12]
	subs	r3, #2
.L_080ebb0c:
	strh	r3, [r4, #30]
	adds	r0, r5, #0
	str	r4, [sp, #4]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	ldr	r1, [pc, #56]
	str	r3, [r6, #12]
	adds	r0, r6, #0
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	ldr	r2, [r7, #24]
	movs	r3, #3
	ands	r2, r3
	ldr	r3, [sp, #20]
	lsls	r2, r2, #1
	adds	r2, r3, r2
	ldr	r3, [pc, #40]
	ands	r2, r3
	ldrh	r3, [r6, #8]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, #8]
	bl	sub_080eb01c
	ldr	r3, [r7, #12]
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r3, r3, r1
	str	r3, [r7, #12]
	ldr	r2, [pc, #16]
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #16]
	adds	r3, r3, r2
	b.n	.L_080ebb64
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.4byte 0xffffcccd
	.2byte 0xc000
	.2byte 0xffff
.L_080ebb64:
	.2byte 0x617b
	ldr	r3, [r7, #4]
	ldr	r4, [sp, #4]
	adds	r3, r3, r1
	str	r3, [r7, #4]
	ldr	r3, [r7, #24]
.L_080ebb70:
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #50
	bne.n	.L_080ebb7e
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
.L_080ebb7e:
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	adds	r4, #40
	adds	r7, #28
	cmp	r3, #0
	bge.n	.L_080ebacc
	ldr	r1, [sp, #28]
	cmp	r1, #0
	beq.n	.L_080ebb9a
	cmp	r1, #1
	beq.n	.L_080ebbac
	b.n	.L_080ebbfa
.L_080ebb9a:
	mov	r2, fp
	cmp	r2, #10
	bne.n	.L_080ebbfa
	movs	r1, #1
	movs	r3, #1
	negs	r1, r1
	str	r3, [sp, #28]
	mov	fp, r1
	b.n	.L_080ebbfa
.L_080ebbac:
	mov	r2, fp
	cmp	r2, #63
	bgt.n	.L_080ebbcc
	mov	r1, r8
	ldrh	r3, [r1, #6]
	ldr	r2, [pc, #116]
	adds	r3, r3, r2
	mov	r2, fp
	strh	r3, [r1, #6]
	lsls	r3, r2, #10
	movs	r2, #128
	lsls	r2, r2, #9
	str	r3, [r1, #24]
	cmp	r3, r2
	ble.n	.L_080ebbcc
	str	r2, [r1, #24]
.L_080ebbcc:
	mov	r3, fp
	cmp	r3, #64
	bne.n	.L_080ebbe2
	add	r1, sp, #8
	ldrh	r1, [r1, #0]
	mov	r2, r8
	strh	r1, [r2, #6]
	mov	r0, r8
	movs	r1, #1
	bl	sub_08020218
.L_080ebbe2:
	mov	r2, fp
	cmp	r2, #10
	bne.n	.L_080ebbec
	movs	r3, #0
	str	r3, [sp, #16]
.L_080ebbec:
	mov	r1, fp
	cmp	r1, #74
	bne.n	.L_080ebbfa
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	str	r2, [sp, #28]
.L_080ebbfa:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #186
	ldr	r1, [sp, #28]
	lsls	r2, r2, #2
	movs	r3, #1
	adds	r2, #255
	add	fp, r3
	cmp	r1, r2
	beq.n	.L_080ebc12
	b.n	.L_080eba40
.L_080ebc12:
	ldr	r0, [sp, #24]
	bl	sub_08014274
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #69
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_080ebc92
	ldrh	r3, [r5, #56]
	ldrh	r2, [r5, #58]
	adds	r3, #1
	strh	r3, [r5, #56]
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L_080ebc56
	subs	r3, r2, #1
	strh	r3, [r5, #58]
	b.n	sub_080ebc62
.L_080ebc56:
	ldr	r3, [r5, #52]
	cmp	r3, #0
	beq.n	sub_080ebc62
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_080ebc92
	adds	r3, r5, #0
	adds	r3, #67
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080ebc7e
	adds	r0, r5, #0
	bl	.L_080ebd24
.L_080ebc7e:
	adds	r3, r5, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080ebc92
	adds	r0, r5, #0
	bl	.L_080ebc94
.L_080ebc92:
	pop	{r5, r6, pc}
.L_080ebc94:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r3, #0
	mov	r8, r3
	adds	r3, r6, #0
	adds	r3, #71
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	sub	sp, #24
	ldr	r7, [r6, #0]
	cmp	r3, #0
	beq.n	.L_080ebcbc
	ldr	r2, [r6, #8]
	movs	r3, #254
	lsls	r3, r3, #17
	subs	r3, r3, r2
	mov	r8, r3
.L_080ebcbc:
	ldr	r5, [pc, #84]
	ldr	r1, [r7, #12]
	ldr	r0, [r6, #40]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9000
	ldr	r1, [r7, #12]
	ldr	r0, [r6, #44]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6872
	add	r1, sp, #8
	mov	r5, r8
	mov	r4, sp
	str	r0, [r4, #4]
	str	r2, [r1, #0]
	str	r5, [r1, #4]
	ldr	r0, [r6, #8]
	adds	r3, r0, r5
	str	r3, [r1, #8]
	movs	r3, #0
	str	r3, [r1, #12]
	ldr	r3, [pc, #44]
	ldr	r5, [pc, #48]
	adds	r2, r2, r3
	cmp	r2, r5
	bhi.n	.L_080ebd0a
	ldr	r3, [pc, #44]
	cmp	r0, r3
	ble.n	.L_080ebd0a
	movs	r5, #224
	lsls	r5, r5, #16
	cmp	r0, r5
	bge.n	.L_080ebd0a
	adds	r0, r7, #0
	adds	r2, r4, #0
	movs	r3, #0
	bl	sub_08020010
.L_080ebd0a:
	add	sp, #24
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.4byte 0x001fffff
	.4byte 0x012ffffe
	.2byte 0x0000
	.2byte 0xffe0
.L_080ebd24:
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #24
	sub	sp, #4
	cmp	r0, r2
	bne.n	.L_080ebd3e
	b.n	.L_080ebe5a
.L_080ebd3e:
	ldr	r3, [r7, #4]
	ldr	r2, [r7, #16]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	mov	sl, r0
	subs	r2, r2, r3
	adds	r3, r7, #0
	adds	r3, #65
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_080ebdc6
	cmp	r0, #0
	bge.n	.L_080ebd66
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	add	r0, sl
.L_080ebd66:
	mov	r3, r8
	asrs	r0, r0, #16
	cmp	r3, #0
	bge.n	.L_080ebd76
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	add	r3, r8
.L_080ebd76:
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r6, r0, #0
	muls	r6, r0
	adds	r3, r2, #0
	adds	r0, r6, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #224]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2380
	lsls	r0, r0, #16
	lsls	r3, r3, #16
	cmp	r0, r3
	bge.n	.L_080ebdb2
	ldr	r6, [pc, #212]
	mov	r1, sl
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4641
	mov	r9, r0
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4448
	str	r0, [sp, #0]
	bl	sub_080149e0
.L_080ebdb2:
	movs	r6, #128
	lsls	r6, r6, #12
	cmp	r0, r6
	bgt.n	.L_080ebdc6
	ldr	r1, [r7, #12]
	ldr	r2, [r7, #16]
	adds	r0, r7, #0
	bl	sub_080ebe94
	b.n	.L_080ebe5a
.L_080ebdc6:
	mov	r0, r8
	mov	r1, sl
	bl	sub_080148e8
	adds	r3, r7, #0
	adds	r3, #66
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	lsls	r0, r0, #16
	asrs	r5, r0, #16
	cmp	r3, #0
	beq.n	.L_080ebe18
	ldrh	r4, [r7, #48]
	subs	r3, r5, r4
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080ebdf0
	negs	r3, r2
.L_080ebdf0:
	movs	r6, #50
	ldrsh	r1, [r7, r6]
	ldrh	r0, [r7, #50]
	cmp	r3, r1
	blt.n	.L_080ebe18
	cmp	r2, #0
	bge.n	.L_080ebe0c
	negs	r3, r2
	cmp	r3, r1
	ble.n	.L_080ebe12
	negs	r3, r0
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	b.n	.L_080ebe12
.L_080ebe0c:
	cmp	r2, r1
	ble.n	.L_080ebe12
	adds	r2, r1, #0
.L_080ebe12:
	adds	r3, r2, r4
	lsls	r3, r3, #16
	asrs	r5, r3, #16
.L_080ebe18:
	lsls	r3, r5, #16
	lsrs	r0, r3, #16
	ldr	r2, [r7, #28]
	ldr	r3, [r7, #36]
	strh	r0, [r7, #48]
	adds	r6, r2, r3
	ldr	r3, [r7, #32]
	cmp	r6, r3
	ble.n	.L_080ebe2c
	adds	r6, r3, #0
.L_080ebe2c:
	lsls	r3, r0, #16
	asrs	r5, r3, #16
	str	r6, [r7, #28]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [pc, #48]
	adds	r1, r6, #0
	mov	r8, r2
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x687b
	adds	r3, r3, r0
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x68bb
	adds	r3, r3, r0
	str	r3, [r7, #8]
.L_080ebe5a:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x030002d4
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	adds	r3, r0, #0
	adds	r3, #65
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080ebe84
	movs	r0, #0
	b.n	.L_080ebe92
.L_080ebe84:
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #24
	eors	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
.L_080ebe92:
	pop	{pc}
