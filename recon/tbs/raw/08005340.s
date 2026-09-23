.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_08004938, 0x08004938
	.set sub_080072fc, 0x080072fc
	.global Func_08005340
	.thumb_func
Func_08005340:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [pc, #56]
	adds	r0, r5, #0
	bl	sub_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #44]
	ldr	r0, [pc, #48]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	mov	r1, sl
	bl	sub_080072fc
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08002df0
	adds	r0, r5, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000002c4
	.4byte 0x040000d4
	.2byte 0x2544
	.2byte 0x0800
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [pc, #56]
	adds	r0, r5, #0
	bl	sub_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #44]
	ldr	r0, [pc, #48]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	mov	r1, sl
	bl	sub_080072fc
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08002df0
	adds	r0, r5, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000004ec
	.4byte 0x040000d4
	.4byte 0x08002808
