.syntax unified
	.thumb
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080162d4, 0x080162d4
	.set sub_0801e41c, 0x0801e41c
	.set sub_0801e74c, 0x0801e74c
	.set sub_0801eadc, 0x0801eadc
	.set sub_08021750, 0x08021750
	.set sub_080770c0, 0x080770c0
	.set sub_080b0038, 0x080b0038
	.global Menu_CreateWorkspaceWindows
	.global Func_0801d9d4
	.thumb_func
Menu_CreateWorkspaceWindows:
Func_0801d9d4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #364]
	movs	r0, #3
	ldr	r3, [r3, #0]
	mov	sl, r0
	movs	r0, #191
	lsls	r0, r0, #1
	sub	sp, #12
	mov	fp, r3
	bl	sub_080770c0
	movs	r2, #0
	mov	r9, r0
	str	r2, [sp, #8]
	cmp	r0, #0
	beq.n	.L_0801da08
	movs	r0, #2
	movs	r3, #1
	str	r0, [sp, #8]
	mov	sl, r3
.L_0801da08:
	ldr	r3, [pc, #328]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801da14
	movs	r2, #3
	add	sl, r2
.L_0801da14:
	movs	r3, #8
	mov	r0, sl
	subs	r1, r3, r0
	lsls	r3, r0, #1
	add	r3, sl
	adds	r4, r3, #1
	adds	r3, r1, r4
	cmp	r3, #19
	ble.n	.L_0801da2a
	movs	r1, #1
	movs	r4, #19
.L_0801da2a:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #20
	movs	r0, #5
	adds	r3, r4, #0
	bl	sub_080162d4
	mov	r2, sl
	mov	r8, r0
	cmp	r2, #1
	ble.n	.L_0801da5c
	mov	r5, sl
	movs	r6, #3
	subs	r5, #1
.L_0801da46:
	adds	r2, r6, #0
	mov	r0, r8
	movs	r1, #0
	movs	r3, #19
	subs	r5, #1
	str	r6, [sp, #0]
	bl	sub_0801e41c
	adds	r6, #3
	cmp	r5, #0
	bne.n	.L_0801da46
.L_0801da5c:
	mov	r3, r9
	movs	r7, #4
	cmp	r3, #0
	bne.n	.L_0801da82
	ldr	r5, [pc, #240]
	mov	r1, r8
	adds	r0, r5, #0
	movs	r2, #48
	movs	r3, #4
	adds	r5, #1
	bl	sub_0801e74c
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #48
	movs	r3, #28
	bl	sub_0801e74c
	movs	r7, #52
.L_0801da82:
	adds	r3, r7, #0
	ldr	r0, [pc, #212]
	mov	r1, r8
	movs	r2, #48
	bl	sub_0801e74c
	ldr	r3, [pc, #196]
	ldrb	r3, [r3, #0]
	adds	r7, #24
	cmp	r3, #0
	beq.n	.L_0801dac4
	ldr	r5, [pc, #196]
	adds	r3, r7, #0
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #48
	adds	r7, #24
	bl	sub_0801e74c
	adds	r0, r5, #1
	adds	r3, r7, #0
	mov	r1, r8
	movs	r2, #48
	adds	r7, #24
	adds	r5, #2
	bl	sub_0801e74c
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #48
	adds	r3, r7, #0
	bl	sub_0801e74c
.L_0801dac4:
	bl	sub_08004080
	adds	r5, r0, #0
	cmp	r5, #95
	bgt.n	.L_0801db02
	ldr	r2, [pc, #148]
	movs	r1, #128
	bl	sub_08003fa4
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	mov	r2, r8
	adds	r0, r5, #0
	str	r3, [sp, #0]
	bl	sub_0801eadc
	ldr	r2, [pc, #128]
	add	r2, fp
	str	r0, [r2, #0]
	mov	r0, r8
	ldrh	r3, [r0, #14]
	lsls	r3, r3, #3
	ldrh	r1, [r0, #12]
	adds	r7, r3, #0
	adds	r7, #16
	adds	r0, r2, #0
	lsls	r1, r1, #3
	adds	r2, r7, #0
	bl	sub_080b0038
.L_0801db02:
	movs	r7, #4
	mov	r2, sl
	negs	r7, r7
	cmp	r2, #0
	ble.n	.L_0801db3c
	ldr	r3, [pc, #92]
	movs	r4, #194
	ldr	r0, [sp, #8]
	lsls	r4, r4, #3
	add	r4, fp
	mov	r5, sl
	adds	r6, r0, r3
.L_0801db1a:
	ldrb	r0, [r6, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	movs	r1, #0
	mov	r2, r8
	movs	r3, #12
	str	r7, [sp, #0]
	str	r4, [sp, #4]
	bl	sub_08021750
	ldr	r4, [sp, #4]
	subs	r5, #1
	adds	r6, #1
	stmia	r4!, {r0}
	adds	r7, #24
	cmp	r5, #0
	bne.n	.L_0801db1a
.L_0801db3c:
	mov	r0, r8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001ea0
	.4byte 0x03001f54
	.4byte 0x00000c23
	.4byte 0x00000c25
	.4byte 0x00000c27
	.4byte 0x080310a4
	.4byte 0x000005a4
	.4byte 0x080367dc
