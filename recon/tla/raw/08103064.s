.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08103218, 0x08103218
	.global Func_08103064
	.thumb_func
Func_08103064:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	mov	fp, r0
	adds	r0, r5, #0
	sub	sp, #4
	adds	r6, r2, #0
	bl	sub_08016ca4
	mov	sl, r0
	movs	r0, #1
	negs	r0, r0
	movs	r7, #0
	cmp	r6, r0
	bne.n	.L_081030fc
	lsls	r5, r5, #8
	movs	r6, #132
	str	r5, [sp, #0]
	movs	r1, #0
	lsls	r6, r6, #1
	mov	r8, r1
	add	r6, sl
	mov	r9, r1
.L_0810309c:
	ldr	r2, [r6, #0]
	mov	r3, r8
	lsls	r1, r3, #5
	mov	r0, fp
	lsls	r3, r7, #1
	movs	r5, #0
	mov	lr, r9
	mov	ip, r2
	adds	r4, r3, r0
.L_081030ae:
	movs	r2, #1
	lsls	r2, r5
	mov	r3, ip
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081030c8
	ldr	r2, [pc, #168]
	ldr	r0, [sp, #0]
	adds	r3, r1, #0
	orrs	r3, r5
	orrs	r3, r2
	orrs	r3, r0
	b.n	.L_081030de
.L_081030c8:
	mov	r3, lr
	adds	r3, #248
	mov	r0, sl
	ldr	r3, [r0, r3]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081030e4
	ldr	r2, [sp, #0]
	adds	r3, r1, #0
	orrs	r3, r5
	orrs	r3, r2
.L_081030de:
	strh	r3, [r4, #0]
	adds	r7, #1
	adds	r4, #2
.L_081030e4:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L_081030ae
	movs	r0, #1
	add	r8, r0
	movs	r3, #4
	mov	r1, r8
	adds	r6, #4
	add	r9, r3
	cmp	r1, #3
	ble.n	.L_0810309c
	b.n	.L_08103152
.L_081030fc:
	movs	r0, #132
	lsls	r3, r6, #2
	lsls	r0, r0, #1
	adds	r2, r3, r0
	mov	r1, sl
	ldr	r2, [r1, r2]
	lsls	r4, r6, #5
	mov	ip, r2
	mov	r6, fp
	lsls	r2, r7, #1
	adds	r0, r2, r6
	ldr	r2, [pc, #80]
	movs	r1, #1
	adds	r3, #248
	movs	r5, #0
	mov	lr, r1
	mov	r9, r2
	mov	r8, r3
.L_08103120:
	mov	r1, lr
	lsls	r1, r5
	mov	r3, ip
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08103136
	adds	r3, r4, #0
	orrs	r3, r5
	mov	r6, r9
	orrs	r3, r6
	b.n	.L_08103146
.L_08103136:
	mov	r2, sl
	mov	r6, r8
	ldr	r3, [r2, r6]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0810314c
	adds	r3, r4, #0
	orrs	r3, r5
.L_08103146:
	strh	r3, [r0, #0]
	adds	r7, #1
	adds	r0, #2
.L_0810314c:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L_08103120
.L_08103152:
	adds	r0, r7, #0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x8000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	adds	r4, r0, #0
	sub	sp, #24
	mov	r8, r1
	ldr	r7, [r3, #0]
	cmp	r4, #0
	bne.n	.L_081031c6
	movs	r2, #175
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	ldr	r0, [r7, #56]
	movs	r5, #1
	movs	r6, #2
	str	r1, [sp, #12]
	movs	r2, #0
	movs	r1, #0
	str	r4, [sp, #4]
	str	r4, [sp, #20]
	str	r5, [sp, #0]
	str	r6, [sp, #8]
	str	r5, [sp, #16]
	bl	sub_08103218
	movs	r2, #151
	lsls	r2, r2, #2
	ldr	r4, [sp, #20]
	adds	r3, r7, r2
	mov	r2, r8
	ldr	r0, [r7, #40]
	ldrb	r3, [r3, #0]
	movs	r1, #0
	str	r2, [sp, #12]
	movs	r2, #0
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	str	r6, [sp, #8]
	str	r4, [sp, #16]
	bl	sub_08103218
	b.n	.L_0810320c
.L_081031c6:
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r7, r2
	movs	r2, #4
	ldrb	r3, [r3, #0]
	ldr	r0, [r7, #56]
	str	r2, [sp, #8]
	mov	r2, r8
	movs	r5, #0
	movs	r6, #1
	str	r2, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	str	r6, [sp, #16]
	bl	sub_08103218
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r7, r2
	mov	r2, r8
	ldr	r0, [r7, #40]
	ldrb	r3, [r3, #0]
	movs	r1, #0
	str	r2, [sp, #12]
	movs	r2, #0
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	str	r6, [sp, #8]
	str	r5, [sp, #16]
	bl	sub_08103218
.L_0810320c:
	movs	r0, #1
	add	sp, #24
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
