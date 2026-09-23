.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_0801475c, 0x0801475c
	.set sub_0801587c, 0x0801587c
	.set sub_0801591c, 0x0801591c
	.set sub_0802c4d8, 0x0802c4d8
	.set sub_0802d088, 0x0802d088
	.global Func_0802c9c8
	.thumb_func
Func_0802c9c8:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r2, [pc, #244]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #32]
	mov	r9, r2
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldr	r5, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #19
	mov	r8, r3
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	ldr	r0, [r5, #0]
	mov	sl, r3
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801591c
	mov	r2, sl
	mov	r3, r9
	strh	r2, [r3, #0]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r0, r9
	mov	r1, r8
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [r5, #12]
	bl	sub_08013300
	ldr	r1, [pc, #176]
	bl	sub_0801587c
	ldr	r0, [r5, #16]
	bl	sub_08013300
	ldr	r1, [pc, #168]
	bl	sub_0801587c
	ldr	r0, [r5, #20]
	bl	sub_08013300
	ldr	r1, [pc, #160]
	bl	sub_0801587c
	ldr	r0, [r5, #24]
	bl	sub_08013300
	ldr	r1, [pc, #152]
	bl	sub_0801587c
	ldr	r0, [r5, #28]
	bl	sub_08013300
	ldr	r1, [pc, #144]
	bl	sub_0801587c
	ldr	r0, [r5, #32]
	bl	sub_08013300
	ldr	r1, [pc, #136]
	bl	sub_0801587c
	ldr	r2, [pc, #132]
	ldr	r3, [pc, #136]
	movs	r0, #1
	str	r3, [r2, #0]
	movs	r3, #130
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #131
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #159
	strh	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [pc, #108]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	ldr	r5, [pc, #56]
	bl	sub_0802c4d8
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r6, r2
	strb	r5, [r3, #0]
	ldr	r0, [pc, #88]
	bl	sub_0801475c
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #114
	adds	r3, r6, r2
	subs	r2, #1
	strb	r5, [r3, #0]
	adds	r3, r6, r2
	strb	r5, [r3, #0]
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #115
	adds	r6, r6, r3
	movs	r0, #0
	ldrsb	r0, [r6, r0]
	movs	r1, #0
	bl	sub_0802d088
	movs	r0, #1
	b.n	.L_0802caf8
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x02038000
	.4byte 0x0203a000
	.4byte 0x0203c000
	.4byte 0x0203e000
	.4byte 0x02028000
	.4byte 0x0202a000
	.4byte 0x030011f8
	.4byte 0x0802c98d
	.4byte 0x00000198
	.2byte 0xcb65
	.2byte 0x0802
.L_0802caf8:
	bl	sub_08013560
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.align 2, 0
