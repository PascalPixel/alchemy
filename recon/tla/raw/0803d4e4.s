.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014cc0, 0x08014cc0
	.set sub_0803d5c4, 0x0803d5c4
	.set sub_0803db54, 0x0803db54
	.set sub_080ad078, 0x080ad078
	.global Func_0803d4e4
	.thumb_func
Func_0803d4e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	movs	r1, #193
	sub	sp, #4
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	str	r3, [sp, #0]
	mov	fp, r2
	bl	sub_08014cc0
	ldr	r3, [pc, #132]
	lsls	r5, r5, #2
	movs	r2, #192
	ldr	r3, [r3, r5]
	lsls	r2, r2, #3
	adds	r7, r0, #0
	adds	r2, #4
	adds	r2, r2, r7
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, r3, r7
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #2
	adds	r3, r3, r7
	movs	r5, #2
	mov	r8, r3
	mov	r9, r2
	mov	r2, sl
	strh	r5, [r2, #0]
	mov	r2, r8
	strh	r5, [r2, #0]
	movs	r1, #0
	bl	sub_0803db54
	ldr	r3, [pc, #80]
	lsls	r6, r6, #2
	ldr	r3, [r3, r6]
	mov	r2, r9
	str	r3, [r2, #0]
	mov	r3, sl
	mov	r2, r8
	strh	r5, [r3, #0]
	adds	r0, r7, #0
	strh	r5, [r2, #0]
	movs	r1, #1
	bl	sub_0803db54
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne.n	.L_0803d564
	bl	sub_080143ac
	mov	r2, fp
	str	r0, [r2, #0]
.L_0803d564:
	mov	r3, fp
	ldr	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r2, r7, r3
	movs	r1, #128
	bl	sub_080142d4
	ldr	r2, [sp, #0]
	str	r0, [r2, #0]
	movs	r0, #68
	bl	sub_0801314c
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0804e684
	.2byte 0xea74
	.2byte 0x0804
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #4
	ldr	r5, [sp, #24]
	adds	r6, r1, #0
	mov	r8, r2
	mov	sl, r3
	bl	sub_080ad078
	adds	r1, r6, #0
	ldrh	r0, [r0, #4]
	mov	r2, r8
	mov	r3, sl
	str	r5, [sp, #0]
	bl	sub_0803d5c4
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
