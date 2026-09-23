.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_080f8888, 0x080f8888
	.global Func_080f8bcc
	.thumb_func
Func_080f8bcc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	adds	r5, r3, #0
	adds	r5, #76
	mov	r9, r0
	mov	sl, r1
	mov	r8, r2
	movs	r6, #0
	adds	r7, r5, #0
.L_080f8bee:
	ldmia	r7!, {r3}
	cmp	r3, #0
	beq.n	.L_080f8c04
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	.L_080f8c18
.L_080f8c04:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, #31
	ble.n	.L_080f8bee
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
.L_080f8c18:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	mov	r8, r2
	adds	r7, r3, #0
	cmp	r6, #31
	ble.n	.L_080f8c2a
	movs	r6, #0
.L_080f8c2a:
	ldr	r1, [sp, #20]
	ldr	r5, [r0, #0]
	adds	r0, r6, #0
	bl	sub_08002054
	lsls	r0, r0, #4
	adds	r0, r0, r7
	strh	r0, [r5, #8]
	ldr	r1, [sp, #20]
	adds	r0, r6, #0
	bl	sub_08002064
	lsls	r0, r0, #4
	add	r0, r8
	strh	r0, [r5, #6]
	adds	r0, r5, #0
	bl	sub_080f8888
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_080f8c54:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	mov	r8, r2
	adds	r7, r3, #0
	cmp	r6, #15
	ble.n	.L_080f8c66
	movs	r6, #0
.L_080f8c66:
	ldr	r1, [sp, #20]
	ldr	r5, [r0, #0]
	adds	r0, r6, #0
	bl	sub_08002054
	lsls	r0, r0, #4
	adds	r0, r0, r7
	strh	r0, [r5, #8]
	ldr	r1, [sp, #20]
	adds	r0, r6, #0
	bl	sub_08002064
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	add	r3, r8
	strh	r3, [r5, #6]
	adds	r0, r5, #0
	bl	sub_080f8888
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	movs	r2, #56
	sub	sp, #4
	ldr	r3, [r3, #0]
	mov	r8, r2
	cmp	r0, #1
	beq.n	.L_080f8cb2
	movs	r2, #40
	mov	r8, r2
.L_080f8cb2:
	adds	r5, r3, #0
	adds	r5, #76
	movs	r3, #5
	movs	r6, #0
	adds	r7, r5, #0
	mov	sl, r3
.L_080f8cbe:
	ldmia	r7!, {r3}
	cmp	r3, #0
	beq.n	.L_080f8cd4
	mov	r2, sl
	str	r2, [sp, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #116
	mov	r3, r8
	bl	.L_080f8c54
.L_080f8cd4:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, #14
	ble.n	.L_080f8cbe
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
