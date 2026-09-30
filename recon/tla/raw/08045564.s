.syntax unified
	.thumb
	.balign 4
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r0
	ldr	r5, [pc, #56]
	adds	r0, r5, #0
	bl	Func_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #40]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	add	r3, r8
	ldr	r0, [r3, #0]
	mov	r1, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	Func_08013164
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x0000027c
	.4byte 0x08038b14