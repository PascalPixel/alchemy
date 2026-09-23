.syntax unified
	.thumb
	.set sub_08002096, 0x08002096
	.set sub_08013164, 0x08013164
	.set sub_0801401c, 0x0801401c
	.set sub_080140d8, 0x080140d8
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_0801587c, 0x0801587c
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020090, 0x08020090
	.set sub_080cad84, 0x080cad84
	.set sub_080cd91c, 0x080cd91c
	.set sub_080d8d68, 0x080d8d68
	.set sub_080d9104, 0x080d9104
	.set sub_080d99b8, 0x080d99b8
	.set sub_080d9f1c, 0x080d9f1c
	.set sub_080da060, 0x080da060
	.global Func_080d9ad0
	.thumb_func
Func_080d9ad0:
	push	{r5, r6, r7, lr}
	adds	r7, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #156
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	adds	r6, r2, #0
	lsls	r2, r5, #5
	adds	r3, r3, r2
	strh	r1, [r3, #20]
	adds	r0, r1, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080d9afc
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080d9104
	b.n	.L_080d9b06
.L_080d9afc:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080d8d68
.L_080d9b06:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #236
	movs	r0, #160
	sub	sp, #4
	bl	sub_08014d00
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	bl	sub_08014d78
	movs	r3, #0
	adds	r6, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_080143ac
	adds	r1, r6, #0
	str	r0, [r5, #8]
	ldr	r0, [pc, #48]
	bl	sub_0801587c
	adds	r2, r6, #0
	ldr	r0, [r5, #8]
	movs	r1, #64
	bl	sub_080142d4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #32]
	bl	sub_080145a8
	movs	r1, #228
	lsls	r1, r1, #2
	adds	r1, #255
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	adds	r0, r6, #0
	bl	sub_08013164
	add	sp, #4
	pop	{r5, r6, pc}
	.4byte 0x8500043b
	.4byte 0x080f0c04
	.4byte 0x080d9b81
	.2byte 0x9d41
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r3, r1, #0
	sub	sp, #48
	movs	r2, #0
	adds	r3, #160
	ldr	r0, [r3, #0]
	str	r2, [sp, #32]
	movs	r3, #0
	mov	fp, r3
	ldr	r3, [r0, #8]
	mov	sl, r2
	ldr	r2, [pc, #380]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	adds	r0, #16
	lsrs	r3, r3, #5
	str	r3, [sp, #28]
	ldr	r3, [pc, #368]
	ldr	r1, [r1, #32]
	movs	r7, #0
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r4, [r2, #0]
	mov	r9, r0
	ands	r4, r3
	str	r4, [sp, #24]
	ldr	r2, [r2, #4]
	ands	r2, r3
	str	r2, [sp, #20]
	movs	r2, #7
	ldr	r3, [r1, #0]
	mov	r1, sp
	ldr	r3, [r3, #4]
	adds	r1, #40
	str	r3, [sp, #16]
	str	r1, [sp, #8]
	str	r2, [sp, #36]
.L_080d9bdc:
	mov	r3, r9
	ldr	r2, [r3, #0]
	ldr	r5, [r3, #20]
	cmp	r2, #0
	beq.n	.L_080d9c1c
	ldr	r4, [sp, #8]
	ldr	r1, [pc, #316]
	ldr	r3, [r4, #4]
	lsls	r2, r2, #16
	lsrs	r2, r2, #16
	ands	r3, r1
	orrs	r3, r2
	str	r3, [r4, #4]
	ldr	r3, [sp, #40]
	movs	r2, #128
	ands	r3, r1
	lsls	r2, r2, #1
	orrs	r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #17
	orrs	r3, r2
	ldr	r0, [sp, #8]
	str	r3, [sp, #40]
	bl	sub_0801401c
	movs	r6, #1
	str	r0, [sp, #12]
	b.n	.L_080d9c20
.L_080d9c1c:
	movs	r6, #0
	str	r6, [sp, #12]
.L_080d9c20:
	cmp	r5, #0
	beq.n	.L_080d9cfa
	str	r6, [sp, #4]
.L_080d9c26:
	ldr	r6, [r5, #8]
	ldr	r0, [r5, #12]
	ldr	r1, [pc, #256]
	mov	r2, fp
	ldr	r4, [r5, #4]
	mov	r8, r0
	adds	r6, r6, r1
	cmp	r2, #0
	beq.n	.L_080d9cea
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #24]
	adds	r3, r0, r4
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r2, sl
	subs	r0, r3, r1
	adds	r3, r2, r6
	lsrs	r2, r3, #31
	ldr	r1, [sp, #16]
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r2, r8
	subs	r1, r3, r1
	adds	r3, r7, r2
	lsrs	r2, r3, #31
	ldr	r7, [sp, #20]
	mov	sl, r1
	adds	r3, r3, r2
	ldr	r1, [sp, #16]
	asrs	r3, r3, #1
	subs	r3, r3, r7
	subs	r7, r3, r1
	ldr	r1, [pc, #196]
	mov	r3, sl
	subs	r2, r7, r3
	adds	r3, r0, r1
	ldr	r1, [pc, #192]
	cmp	r3, r1
	bhi.n	.L_080d9cea
	ldr	r3, [pc, #192]
	cmp	r2, r3
	ble.n	.L_080d9cea
	ldr	r1, [pc, #188]
	cmp	r2, r1
	bgt.n	.L_080d9cea
	movs	r3, #128
	asrs	r0, r0, #16
	lsls	r3, r3, #1
	adds	r3, #255
	subs	r0, #4
	asrs	r2, r2, #16
	ands	r0, r3
	subs	r2, #4
	movs	r3, #255
	ands	r2, r3
	mov	r3, sl
	adds	r1, r3, r7
	movs	r3, #0
	str	r3, [r5, #20]
	lsls	r0, r0, #16
	movs	r3, #128
	orrs	r2, r0
	lsls	r3, r3, #6
	orrs	r2, r3
	str	r2, [r5, #24]
	ldr	r7, [sp, #28]
	movs	r3, #128
	lsls	r3, r3, #4
	orrs	r3, r7
	str	r3, [r5, #28]
	movs	r0, #4
	ldrb	r3, [r5, #25]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	ldr	r2, [sp, #4]
	movs	r7, #63
	orrs	r3, r2
	strb	r3, [r5, #25]
	ldr	r2, [sp, #12]
	movs	r3, #31
	ands	r2, r3
	ldrb	r3, [r5, #27]
	negs	r7, r7
	adds	r0, r7, #0
	lsls	r2, r2, #1
	ands	r3, r0
	orrs	r3, r2
	asrs	r1, r1, #16
	adds	r0, r5, #0
	adds	r1, #58
	strb	r3, [r5, #27]
	adds	r0, #20
	str	r4, [sp, #0]
	bl	sub_080140d8
	ldr	r4, [sp, #0]
.L_080d9cea:
	str	r4, [sp, #32]
	movs	r0, #1
	ldr	r5, [r5, #0]
	mov	sl, r6
	mov	r7, r8
	mov	fp, r0
	cmp	r5, #0
	bne.n	.L_080d9c26
.L_080d9cfa:
	ldr	r4, [sp, #36]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #28
	subs	r4, #1
	str	r1, [sp, #32]
	mov	sl, r1
	movs	r7, #0
	mov	fp, r2
	add	r9, r3
	str	r4, [sp, #36]
	cmp	r4, #0
	blt.n	.L_080d9d16
	b.n	.L_080d9bdc
.L_080d9d16:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x020036e0
	.4byte 0xffff0000
	.4byte 0xfffc0000
	.4byte 0x001fffff
	.4byte 0x012ffffe
	.4byte 0xffe00000
	.2byte 0xffff
	.2byte 0x00df
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r0, #130
	adds	r3, #160
	lsls	r0, r0, #1
	mov	r8, r1
	ldr	r6, [r3, #0]
	movs	r7, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080d9e06
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #12
	ble.n	.L_080d9e06
	ldr	r2, [pc, #252]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080d9df0
	adds	r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl	sub_080cd91c
	movs	r2, #1
	adds	r5, r6, #0
	negs	r2, r2
	adds	r5, #12
	cmp	r0, r2
	beq.n	.L_080d9e0a
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r3, r0
	beq.n	.L_080d9dc0
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	cmp	r3, r0
	beq.n	.L_080d9dc4
	adds	r2, r5, #0
.L_080d9da6:
	adds	r7, #1
	adds	r2, #28
	cmp	r7, #7
	bgt.n	.L_080d9e0a
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, r0
	beq.n	.L_080d9dc4
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	cmp	r3, r0
	bne.n	.L_080d9da6
	b.n	.L_080d9dc4
.L_080d9dc0:
	adds	r5, r6, #0
	adds	r5, #12
.L_080d9dc4:
	cmp	r7, #7
	bgt.n	.L_080d9e0a
	lsls	r3, r7, #3
	subs	r3, r3, r7
	lsls	r3, r3, #2
	adds	r3, #12
	ldrsh	r0, [r6, r3]
	adds	r3, r6, r3
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	bl	sub_080d99b8
	cmp	r0, #0
	beq.n	.L_080d9e0a
	movs	r2, #181
	movs	r3, #252
	lsls	r2, r2, #1
	lsls	r3, r3, #8
	add	r2, r8
	adds	r3, #131
	strh	r3, [r2, #0]
	b.n	.L_080d9e0a
.L_080d9df0:
	cmp	r3, #4
	bne.n	.L_080d9e06
	movs	r2, #181
	movs	r3, #252
	lsls	r2, r2, #1
	lsls	r3, r3, #8
	add	r2, r8
	adds	r3, #132
	adds	r5, r6, #0
	strh	r3, [r2, #0]
	b.n	.L_080d9e08
.L_080d9e06:
	adds	r5, r6, #0
.L_080d9e08:
	adds	r5, #12
.L_080d9e0a:
	ldr	r3, [pc, #100]
	movs	r7, #0
	mov	r8, r3
.L_080d9e10:
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq.n	.L_080d9e5c
	ldr	r0, [r5, #16]
	bl	sub_08002096
	ldr	r1, [r5, #8]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x692b
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	movs	r1, #243
	lsls	r1, r1, #8
	adds	r6, r0, #0
	str	r3, [r5, #16]
	ldr	r0, [r5, #8]
	adds	r1, #51
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x22c8
	lsls	r2, r2, #5
	adds	r2, #152
	str	r0, [r5, #8]
	cmp	r0, r2
	bgt.n	.L_080d9e4c
	movs	r3, #0
	str	r3, [r5, #8]
	str	r3, [r5, #20]
.L_080d9e4c:
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	adds	r0, r7, #0
	adds	r3, r6, #0
	bl	sub_080da060
.L_080d9e5c:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #7
	ble.n	.L_080d9e10
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	movs	r2, #0
	adds	r0, r3, #0
	adds	r0, #236
.L_080d9e84:
	movs	r3, #16
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_080d9e96
	adds	r2, #1
	adds	r0, #32
	cmp	r2, #127
	ble.n	.L_080d9e84
	movs	r0, #0
.L_080d9e96:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	mov	r8, r1
	adds	r7, r0, #0
	mov	r0, r8
	mov	sl, r2
	sub	sp, #12
	mov	r9, r3
	bl	sub_080cad84
	adds	r5, r0, #0
	mov	r0, sl
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r5, #0
	beq.n	.L_080d9f0e
	cmp	r6, #0
	beq.n	.L_080d9f0e
	lsls	r3, r7, #3
	subs	r3, r3, r7
	lsls	r3, r3, #2
	add	r3, r9
	adds	r3, #12
	mov	r2, r8
	strh	r2, [r3, #0]
	mov	r2, sl
	strh	r2, [r3, #2]
	movs	r4, #128
	ldr	r0, [r6, #8]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	str	r0, [sp, #0]
	lsls	r4, r4, #12
	ldr	r0, [r6, #12]
	adds	r2, r2, r4
	adds	r0, r0, r4
	str	r0, [sp, #4]
	ldr	r0, [r6, #16]
	str	r0, [sp, #8]
	adds	r0, r7, #0
	bl	sub_080d9f1c
	adds	r0, r5, #0
	movs	r1, #4
	bl	sub_08020090
	adds	r0, r6, #0
	movs	r1, #4
	bl	sub_08020090
.L_080d9f0e:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
