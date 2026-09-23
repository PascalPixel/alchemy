.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08123534, 0x08123534
	.global Func_0811843c
	.thumb_func
Func_0811843c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r0, [pc, #128]
	ldr	r1, [pc, #128]
	ldr	r6, [pc, #132]
	movs	r2, #148
	sub	sp, #4
	mov	r8, r0
	movs	r7, #0
	mov	r9, r1
	movs	r4, #0
	mov	sl, r2
.L_0811845a:
	mov	r3, r9
	ldrb	r0, [r3, r7]
	str	r4, [sp, #0]
	bl	sub_08016ca4
	movs	r3, #0
	adds	r5, r0, #0
	mov	r0, r8
	strb	r3, [r0, r7]
	mov	r1, r9
	ldrb	r0, [r1, r7]
	bl	sub_08123534
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_0811847c
	b.n	.L_081185a6
.L_0811847c:
	movs	r3, #1
	mov	r2, r8
	strb	r3, [r2, r7]
	cmp	r7, #2
	ble.n	.L_081184a4
	movs	r0, #56
	ldrsh	r2, [r5, r0]
	mov	r1, sl
	ldrh	r3, [r5, #56]
	cmp	r2, #0
	bgt.n	.L_08118494
	ldr	r3, [pc, #48]
.L_08118494:
	mov	r2, r8
	ldr	r0, [pc, #60]
	strh	r3, [r2, r1]
	ldrh	r2, [r5, #58]
	adds	r3, r4, r0
	strh	r2, [r3, #2]
	cmp	r7, #2
	bgt.n	.L_081184d8
.L_081184a4:
	mov	r1, r8
	adds	r3, r4, r1
	adds	r1, r3, #0
	movs	r3, #150
	lsls	r3, r3, #1
	adds	r1, #8
	adds	r2, r5, r3
	movs	r0, #3
.L_081184b4:
	ldrb	r3, [r2, #0]
	subs	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, #0
	bge.n	.L_081184b4
	b.n	.L_081184d8
	.4byte 0x00000001
	.4byte 0x0200ff58
	.4byte 0x0812a16c
	.4byte 0x0200ff6c
	.2byte 0xffec
	.2byte 0x0200
.L_081184d8:
	movs	r0, #152
	lsls	r0, r0, #1
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	movs	r1, #50
	adds	r1, #255
	strb	r3, [r6, #0]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	movs	r2, #153
	lsls	r2, r2, #1
	strb	r3, [r6, #1]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	adds	r0, #3
	strb	r3, [r6, #2]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	adds	r1, #3
	strb	r3, [r6, #3]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	adds	r2, #3
	strb	r3, [r6, #4]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	adds	r0, #3
	strb	r3, [r6, #5]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	adds	r1, #3
	strb	r3, [r6, #6]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	adds	r2, #3
	strb	r3, [r6, #7]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	adds	r0, #3
	strb	r3, [r6, #8]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	adds	r1, #3
	strb	r3, [r6, #9]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	adds	r2, #3
	strb	r3, [r6, #10]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	adds	r0, #3
	strb	r3, [r6, #11]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	adds	r1, #3
	strb	r3, [r6, #12]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	adds	r2, #3
	strb	r3, [r6, #13]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	adds	r0, #3
	strb	r3, [r6, #14]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	adds	r1, #3
	strb	r3, [r6, #15]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	adds	r2, #3
	strb	r3, [r6, #16]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	adds	r0, #3
	strb	r3, [r6, #17]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	adds	r1, #3
	strb	r3, [r6, #18]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	adds	r2, #3
	strb	r3, [r6, #19]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	adds	r0, #3
	strb	r3, [r6, #20]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	adds	r1, #3
	strb	r3, [r6, #21]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	adds	r2, #3
	strb	r3, [r6, #22]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	adds	r0, #3
	strb	r3, [r6, #23]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	strb	r3, [r6, #24]
.L_081185a6:
	movs	r1, #4
	adds	r7, #1
	adds	r4, #4
	add	sl, r1
	adds	r6, #28
	cmp	r7, #4
	bgt.n	.L_081185b6
	b.n	.L_0811845a
.L_081185b6:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.align 2, 0
