.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072fc, 0x080072fc
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e155c, 0x080e155c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080d3854
	.global Func_080d3854
	.thumb_func
Unnamed_080d3854:
Func_080d3854:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #92]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #52
	str	r1, [sp, #40]
	ldr	r2, [pc, #84]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #72]
	strh	r3, [r2, #0]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #64]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	ldr	r1, [sp, #40]
	adds	r0, r5, #0
	bl	sub_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #28]
	movs	r1, #7
	str	r3, [sp, #44]
	movs	r2, #7
	b.n	.L_080d38d4
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000ce
	.2byte 0x1388
	.2byte 0x0300
.L_080d38d4:
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #32]
	mov	r0, sp
	adds	r0, #44
	str	r0, [sp, #24]
	str	r3, [r0, #4]
	ldr	r2, [sp, #40]
	movs	r3, #225
	movs	r1, #0
	lsls	r3, r3, #7
	mov	r8, r1
	adds	r5, r2, r3
.L_080d38f4:
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	r8, r0
	negs	r3, r3
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_080d38f4
	ldr	r3, [sp, #40]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r3, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #40]
	ldr	r3, [pc, #76]
	adds	r2, r1, r3
	movs	r3, #50
	movs	r1, #144
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #68]
	bl	sub_080041d8
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #52]
	movs	r0, #1
	strh	r3, [r2, #0]
	bl	sub_080030f8
	movs	r0, #141
	bl	sub_080f9010
	ldr	r2, [pc, #52]
	ldr	r1, [sp, #40]
	adds	r2, r1, r2
	movs	r0, #0
	str	r2, [sp, #28]
	mov	fp, r0
.L_080d3960:
	mov	r3, fp
	lsls	r0, r3, #10
	bl	sub_08002322
	lsls	r0, r0, #4
	str	r0, [sp, #32]
	mov	r0, fp
	cmp	r0, #32
	bne.n	.L_080d3990
	movs	r0, #133
	bl	sub_080b50e8
	b.n	.L_080d3990
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x04000052
	.2byte 0x7828
	.2byte 0x0000
.L_080d3990:
	movs	r1, #0
	ldr	r6, [pc, #56]
	mov	r9, r1
	movs	r5, #16
.L_080d3998:
	cmp	fp, r5
	bne.n	.L_080d39a8
	movs	r1, #128
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #44]
	bl	sub_080072fc
.L_080d39a8:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r5, #8
	cmp	r3, #7
	bne.n	.L_080d3998
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d39d4
	ldr	r1, [sp, #32]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r1, r1, r2
	str	r1, [sp, #32]
	b.n	.L_080d39dc
	movs	r0, r0
	.4byte 0x03000168
	.2byte 0x0808
	.2byte 0x0808
.L_080d39d4:
	ldr	r3, [sp, #32]
	ldr	r0, [pc, #68]
	adds	r3, r3, r0
	str	r3, [sp, #32]
.L_080d39dc:
	mov	r1, fp
	cmp	r1, #16
	bgt.n	.L_080d39ec
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #56]
	mov	r3, fp
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_080d39ec:
	mov	r2, fp
	cmp	r2, #63
	ble.n	.L_080d3a00
	ldr	r2, [pc, #36]
	mov	r0, fp
	ldr	r1, [pc, #28]
	ldr	r3, [pc, #36]
	subs	r2, r2, r0
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_080d3a00:
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	ldr	r0, [pc, #28]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r0, r3]
	movs	r1, #0
	mov	r9, r1
	b.n	.L_080d3a28
	.4byte 0x00001000
	.4byte 0x0000004f
	.4byte 0xffe00000
	.4byte 0x04000052
	.2byte 0xe1ca
	.2byte 0x080e
.L_080d3a28:
	cmp	r3, #0
	bne.n	.L_080d3a2e
	b.n	.L_080d3b88
.L_080d3a2e:
	mov	r1, fp
	mov	r2, fp
	ldr	r3, [sp, #40]
	asrs	r1, r1, #31
	lsls	r2, r2, #11
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
.L_080d3a3e:
	ldr	r0, [sp, #16]
	bl	sub_08002322
	ldr	r1, [sp, #28]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #516]
	adds	r3, #1
	ldrb	r3, [r2, r3]
	muls	r3, r0
	ldr	r0, [sp, #32]
	adds	r3, r3, r0
	asrs	r3, r3, #16
	ldr	r0, [sp, #16]
	adds	r3, #40
	mov	sl, r3
	bl	sub_0800231c
	ldr	r1, [sp, #20]
	lsls	r0, r0, #1
	asrs	r7, r0, #16
	lsrs	r0, r1, #31
	add	r0, fp
	movs	r1, #3
	asrs	r0, r0, #1
	bl	sub_080022fc
	lsls	r5, r0, #2
	ldr	r2, [sp, #40]
	adds	r5, r5, r0
	lsls	r6, r5, #9
	ldr	r3, [pc, #468]
	adds	r6, r2, r6
	adds	r1, r6, r3
	movs	r0, #40
	movs	r2, #32
	adds	r3, r7, #0
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	adds	r3, #16
	mov	r2, sl
	bl	sub_080072f4
	ldr	r3, [sp, #40]
	ldr	r0, [pc, #444]
	lsls	r5, r5, #8
	adds	r5, r3, r5
	adds	r5, r5, r0
	movs	r1, #40
	movs	r2, #32
	adds	r3, r7, #0
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	adds	r3, #48
	ldr	r0, [sp, #36]
	adds	r1, r5, #0
	mov	r2, sl
	bl	sub_080072f4
	ldr	r3, [pc, #416]
	movs	r0, #40
	adds	r6, r6, r3
	movs	r1, #32
	adds	r3, r7, #0
	adds	r3, #80
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	mov	r2, sl
	adds	r1, r6, #0
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	bl	sub_080072f4
	movs	r0, #225
	ldr	r3, [sp, #12]
	movs	r2, #0
	lsls	r0, r0, #7
	mov	r8, r2
	adds	r6, r3, r0
.L_080d3ae6:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_080d3b32
	mov	r1, r8
	lsrs	r2, r1, #31
	add	r2, r8
	asrs	r2, r2, #1
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	asrs	r4, r4, #1
	movs	r2, #1
	mov	r5, r8
	adds	r4, r4, r3
	ands	r5, r2
	ldr	r0, [pc, #348]
	ldr	r2, [pc, #348]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldrb	r0, [r0, r4]
	ldr	r3, [sp, #40]
	ldr	r2, [r6, #0]
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #336]
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r5, r5, #2
	adds	r3, r3, r7
	ldr	r4, [r5, r0]
	add	r2, sl
	ldr	r0, [sp, #36]
	bl	sub_080072f4
	ldr	r3, [r6, #24]
.L_080d3b32:
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #6
	bne.n	.L_080d3b54
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	str	r3, [r6, #0]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r6, #4]
	movs	r3, #0
	str	r3, [r6, #24]
.L_080d3b54:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #4
	bne.n	.L_080d3ae6
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #12]
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r3, r3, r0
	adds	r1, #112
	str	r1, [sp, #12]
	str	r3, [sp, #16]
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	movs	r2, #1
	add	r9, r2
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #216]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r1, r3]
	cmp	r9, r3
	beq.n	.L_080d3b88
	b.n	.L_080d3a3e
.L_080d3b88:
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #20]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_080d3bec
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #212]
	movs	r3, #0
	adds	r7, r1, r2
	mov	sl, r3
	movs	r4, #36
.L_080d3ba2:
	movs	r0, #0
	mov	r5, sl
	mov	r9, r0
	adds	r6, r4, #0
	adds	r5, #16
.L_080d3bac:
	cmp	fp, r5
	bne.n	.L_080d3bd0
	ldr	r3, [r7, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	mov	r3, r8
	str	r4, [sp, #8]
	bl	sub_080d6888
	ldr	r3, [r7, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
	ldr	r4, [sp, #8]
.L_080d3bd0:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	adds	r5, #8
	cmp	r0, #7
	bne.n	.L_080d3bac
	add	r8, r3
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #20]
	movs	r1, #3
	add	sl, r1
	adds	r4, #2
	cmp	r8, r3
	bne.n	.L_080d3ba2
.L_080d3bec:
	ldr	r2, [sp, #40]
	ldr	r0, [pc, #132]
	movs	r1, #1
	adds	r3, r2, r0
	str	r1, [r3, #0]
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #80]
	adds	r3, #2
	ldrb	r0, [r1, r3]
	lsls	r1, r0, #1
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r0, [pc, #100]
	ldr	r2, [sp, #40]
	movs	r1, #1
	adds	r3, r2, r0
	str	r1, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #80
	beq.n	.L_080d3c2c
	b.n	.L_080d3960
.L_080d3c2c:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #64]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080ee1ca
	.4byte 0x00000c56
	.4byte 0x00002a56
	.4byte 0x00001156
	.4byte 0x080edeca
	.4byte 0x080edebe
	.4byte 0x080eded0
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
