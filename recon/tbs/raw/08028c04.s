.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016478, 0x08016478
	.set sub_0801e74c, 0x0801e74c
	.set sub_080284dc, 0x080284dc
	.set sub_0802851c, 0x0802851c
	.set sub_08028574, 0x08028574
	.set sub_080287a8, 0x080287a8
	.set sub_08028808, 0x08028808
	.global Func_08028c04
	.thumb_func
Func_08028c04:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	sub	sp, #4
	bl	sub_080284dc
	ldr	r3, [pc, #320]
	ldr	r7, [r3, #0]
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_08028c2c
	movs	r0, #44
	bl	sub_080287a8
	movs	r0, #45
	bl	sub_080287a8
	b.n	.L_08028c3e
.L_08028c2c:
	movs	r0, #46
	bl	sub_080287a8
	movs	r0, #47
	bl	sub_080287a8
	movs	r0, #48
	bl	sub_080287a8
.L_08028c3e:
	movs	r0, #17
	movs	r1, #7
	movs	r2, #0
	bl	sub_08028808
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_08028cbc
	ldr	r1, [pc, #268]
	ldr	r0, [pc, #268]
	bl	sub_080041d8
	ldr	r3, [pc, #268]
	adds	r2, r7, #0
	adds	r2, #150
	strh	r3, [r2, #0]
	movs	r6, #2
	movs	r1, #0
	movs	r2, #17
	movs	r3, #4
	movs	r0, #7
	str	r6, [sp, #0]
	bl	sub_080162d4
	ldr	r5, [pc, #248]
	movs	r3, #128
	adds	r1, r0, #0
	adds	r3, r3, r7
	str	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r2, #0
	mov	r8, r3
	movs	r3, #4
	bl	sub_0801e74c
	movs	r1, #4
	movs	r2, #25
	movs	r3, #12
	movs	r0, #3
	str	r6, [sp, #0]
	bl	sub_080162d4
	adds	r1, r0, #0
	str	r1, [r7, #124]
	adds	r0, r5, #1
	movs	r2, #8
	movs	r3, #0
	bl	sub_0801e74c
	adds	r0, r5, #2
	ldr	r1, [r7, #124]
	movs	r2, #8
	movs	r3, #11
	adds	r5, #3
	bl	sub_0801e74c
	ldr	r1, [r7, #124]
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #22
	bl	sub_0801e74c
	b.n	.L_08028d00
.L_08028cbc:
	ldr	r1, [pc, #156]
	ldr	r0, [pc, #172]
	bl	sub_080041d8
	ldr	r3, [pc, #156]
	adds	r2, r7, #0
	adds	r2, #150
	strh	r3, [r2, #0]
	movs	r5, #2
	movs	r1, #0
	movs	r2, #18
	movs	r3, #4
	movs	r0, #6
	str	r5, [sp, #0]
	bl	sub_080162d4
	movs	r3, #128
	adds	r1, r0, #0
	adds	r3, r3, r7
	str	r1, [r3, #0]
	ldr	r0, [pc, #136]
	movs	r2, #2
	mov	r8, r3
	movs	r3, #4
	bl	sub_0801e74c
	movs	r0, #1
	movs	r1, #5
	movs	r2, #28
	movs	r3, #7
	str	r5, [sp, #0]
	bl	sub_080162d4
	str	r0, [r7, #124]
.L_08028d00:
	movs	r0, #0
	bl	sub_08028574
	mov	r3, sl
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_08028d16
	ldr	r0, [pc, #80]
	bl	sub_08004278
	b.n	.L_08028d1c
.L_08028d16:
	ldr	r0, [pc, #84]
	bl	sub_08004278
.L_08028d1c:
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl	sub_08016478
	ldr	r0, [r7, #124]
	bl	sub_08016478
	mov	r3, r8
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl	sub_08016418
	movs	r1, #2
	ldr	r0, [r7, #124]
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_0802851c
	adds	r0, r5, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f38
	.4byte 0x00000c76
	.4byte 0x08028aa9
	.4byte 0x0000ffff
	.4byte 0x00000c77
	.4byte 0x08028b81
	.4byte 0x00000c76
