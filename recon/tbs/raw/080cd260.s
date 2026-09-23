.syntax unified
	.thumb
	.set sub_0800543c, 0x0800543c
	.set sub_08005490, 0x08005490
	.set sub_080054e4, 0x080054e4
	.set sub_08005534, 0x08005534
	.set sub_080072f0, 0x080072f0
	.global BattlePresentation_ProcessPendingGraphicsTransfer
	.global Func_080cd260
	.thumb_func
BattlePresentation_ProcessPendingGraphicsTransfer:
Func_080cd260:
	push	{r5, r6, lr}
	ldr	r0, [pc, #212]
	ldr	r2, [pc, #212]
	ldr	r6, [r0, #0]
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080cd326
	subs	r2, #164
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	ldr	r5, [r0, #4]
	cmp	r3, #4
	bhi.n	.L_080cd316
	ldr	r2, [pc, #192]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080cd298
	.4byte 0x080cd2a8
	.4byte 0x080cd2ca
	.4byte 0x080cd2f0
	.2byte 0xd304
	.2byte 0x080c
	movs	r2, #128
	ldr	r3, [pc, #168]
	ldr	r0, [pc, #168]
	adds	r1, r5, #0
	lsls	r2, r2, #7
	bl	sub_080072f0
	b.n	.L_080cd316
	movs	r2, #128
	ldr	r3, [pc, #152]
	adds	r1, r5, #0
	lsls	r2, r2, #7
	ldr	r0, [pc, #148]
	bl	sub_080072f0
	ldr	r2, [pc, #148]
	movs	r1, #128
	adds	r3, r6, r2
	ldr	r2, [r3, #0]
	adds	r0, r5, #0
	ldr	r3, [pc, #140]
	lsls	r1, r1, #7
	bl	sub_080072f0
	b.n	.L_080cd316
	ldr	r2, [pc, #128]
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #50
	bne.n	.L_080cd2e2
	movs	r2, #128
	ldr	r1, [pc, #112]
	lsls	r2, r2, #7
	adds	r0, r5, #0
	bl	sub_08005534
	b.n	.L_080cd316
.L_080cd2e2:
	movs	r2, #128
	ldr	r1, [pc, #96]
	lsls	r2, r2, #7
	adds	r0, r5, #0
	bl	sub_080054e4
	b.n	.L_080cd316
	ldr	r2, [pc, #88]
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	movs	r3, #128
	ldr	r2, [pc, #76]
	lsls	r3, r3, #7
	adds	r0, r5, #0
	bl	sub_08005490
	b.n	.L_080cd316
	ldr	r2, [pc, #68]
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	adds	r0, r5, #0
	bl	sub_0800543c
.L_080cd316:
	ldr	r3, [pc, #36]
	adds	r2, r6, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, r6, r3
	movs	r3, #1
	b.n	.L_080cd32e
.L_080cd326:
	ldr	r3, [pc, #44]
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
.L_080cd32e:
	str	r3, [r2, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007824
	.4byte 0x080cd284
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x00007784
	.4byte 0x03000168
	.4byte 0x00007820
