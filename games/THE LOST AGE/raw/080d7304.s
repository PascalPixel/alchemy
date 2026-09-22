.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_080d0c50, 0x080d0c50
	.set sub_080d0e1c, 0x080d0e1c
	.global Overlay_080d7304
Overlay_080d7304:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r1, #248
	lsls	r1, r1, #5
	adds	r1, #136
	movs	r0, #120
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	mov	r8, r3
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #120]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #9
	mov	r1, r8
	adds	r2, r5, #0
	adds	r0, #3
	movs	r3, #1
	bl	sub_080d0e1c
	movs	r3, #168
	lsls	r3, r3, #4
	movs	r0, #128
	adds	r6, r5, r3
	lsls	r0, r0, #9
	mov	r1, r8
	adds	r2, r6, #0
	adds	r0, #5
	movs	r3, #1
	bl	sub_080d0e1c
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r2, r5, r3
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r3, #12
	bl	sub_080d0c50
	movs	r3, #224
	lsls	r3, r3, #4
	add	r8, r3
	adds	r0, r5, #0
	mov	r2, r8
	movs	r1, #0
	movs	r3, #1
	bl	sub_080d0e1c
	movs	r3, #252
	lsls	r3, r3, #5
	adds	r2, r5, r3
	movs	r3, #150
	lsls	r3, r3, #2
	strh	r3, [r2, #0]
	movs	r3, #248
	lsls	r3, r3, #5
	adds	r3, #130
	adds	r5, r5, r3
	movs	r1, #144
	movs	r3, #1
	strh	r3, [r5, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl	sub_080145a8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x850007e2
	.2byte 0x67e9
	.2byte 0x080d
	push	{lr}
	movs	r1, #248
	lsls	r1, r1, #5
	adds	r1, #136
	movs	r0, #120
	bl	sub_08014d00
	movs	r3, #252
	lsls	r3, r3, #5
	adds	r2, r0, r3
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #248
	lsls	r3, r3, #5
	adds	r3, #130
	movs	r1, #0
	adds	r0, r0, r3
	strh	r1, [r0, #0]
	pop	{pc}
