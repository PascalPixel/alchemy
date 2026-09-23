.syntax unified
	.thumb
	.set sub_081c18e0, 0x081c18e0
	.set sub_081c2f40, 0x081c2f40
	.set sub_081c2f68, 0x081c2f68
	.set sub_081c2fd0, 0x081c2fd0
	.global Func_081c0e30
	.thumb_func
Func_081c0e30:
	push	{r5, r6, lr}
	ldr	r1, [pc, #72]
	ldrb	r3, [r1, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	beq.n	.L_081c0e56
	cmp	r2, #1
	bne.n	.L_081c0e52
	ldr	r3, [pc, #60]
	ldrb	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_081c0e56
	strb	r3, [r1, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
	b.n	.L_081c0e56
.L_081c0e52:
	adds	r3, #255
	strb	r3, [r1, #0]
.L_081c0e56:
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #44]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	ldrh	r4, [r1, #0]
	cmp	r2, r3
	beq.n	.L_081c0eba
	subs	r0, r2, r3
	cmp	r0, #0
	ble.n	.L_081c0e90
	ldr	r3, [pc, #28]
	ldrh	r3, [r3, #0]
	adds	r3, r4, r3
	b.n	.L_081c0e96
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x02005800
	.4byte 0x02006a10
	.4byte 0x02005838
	.4byte 0x02005808
	.2byte 0x5810
	.2byte 0x0200
.L_081c0e90:
	ldr	r3, [pc, #140]
	ldrh	r3, [r3, #0]
	subs	r3, r4, r3
.L_081c0e96:
	strh	r3, [r1, #0]
	ldr	r3, [pc, #136]
	ldr	r1, [pc, #140]
	ldrh	r4, [r3, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	subs	r3, r3, r2
	eors	r3, r0
	cmp	r3, #0
	bge.n	.L_081c0eb0
	strh	r4, [r1, #0]
.L_081c0eb0:
	ldrh	r2, [r1, #0]
	ldr	r0, [pc, #120]
	movs	r1, #255
	bl	sub_081c2f68
.L_081c0eba:
	ldr	r3, [pc, #116]
	ldr	r1, [pc, #116]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	ldrh	r4, [r1, #0]
	cmp	r2, r3
	beq.n	.L_081c0f1e
	subs	r0, r2, r3
	cmp	r0, #0
	ble.n	.L_081c0eda
	ldr	r3, [pc, #100]
	ldrh	r3, [r3, #0]
	adds	r3, r4, r3
	b.n	.L_081c0ee0
.L_081c0eda:
	ldr	r3, [pc, #92]
	ldrh	r3, [r3, #0]
	subs	r3, r4, r3
.L_081c0ee0:
	strh	r3, [r1, #0]
	ldr	r3, [pc, #76]
	ldr	r6, [pc, #76]
	ldrh	r1, [r3, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r4, #0
	ldrsh	r2, [r6, r4]
	subs	r3, r3, r2
	eors	r3, r0
	cmp	r3, #0
	bge.n	.L_081c0efa
	strh	r1, [r6, #0]
.L_081c0efa:
	ldr	r5, [pc, #48]
	ldrh	r1, [r6, #0]
	adds	r0, r5, #0
	bl	sub_081c2f40
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	movs	r1, #255
	lsls	r2, r3, #1
	adds	r2, r2, r3
	movs	r3, #244
	lsls	r2, r2, #18
	lsls	r3, r3, #24
	adds	r2, r2, r3
	asrs	r2, r2, #16
	adds	r0, r5, #0
	bl	sub_081c2fd0
.L_081c0f1e:
	pop	{r5, r6, pc}
	.4byte 0x02005810
	.4byte 0x02005838
	.4byte 0x02005808
	.4byte 0x02006a90
	.4byte 0x02005834
	.4byte 0x0200583c
	.2byte 0x580c
	.2byte 0x0200
	push	{lr}
	bl	sub_081c18e0
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	lsls	r1, r1, #16
	ldr	r0, [pc, #8]
	lsrs	r1, r1, #16
	bl	sub_081c2f40
	pop	{pc}
	.2byte 0x6a90
	.2byte 0x0200
	push	{lr}
	adds	r2, r0, #0
	lsls	r2, r2, #16
	ldr	r0, [pc, #12]
	asrs	r2, r2, #16
	movs	r1, #255
	bl	sub_081c2fd0
	pop	{pc}
	movs	r0, r0
	.4byte 0x02006a90
