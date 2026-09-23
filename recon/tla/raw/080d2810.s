.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08038048, 0x08038048
	.set sub_08038100, 0x08038100
	.set sub_08038140, 0x08038140
	.set sub_08038378, 0x08038378
	.set sub_080cad84, 0x080cad84
	.set sub_080cccb8, 0x080cccb8
	.set sub_080d3240, 0x080d3240
	.global Func_080d2810
	.thumb_func
Func_080d2810:
.L_080d2810:
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080d283e
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080d283e
	ldr	r6, [r0, #80]
	movs	r5, #0
	b.n	.L_080d282e
.L_080d282c:
	adds	r5, #1
.L_080d282e:
	cmp	r5, #89
	bgt.n	.L_080d283e
	movs	r0, #1
	bl	sub_08013560
	ldrb	r3, [r6, #24]
	cmp	r7, r3
	beq.n	.L_080d282c
.L_080d283e:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	mov	r8, r0
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldr	r0, [r3, #0]
	mov	sl, r1
	bl	sub_080cccb8
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	movs	r0, #242
	mov	r9, r2
	lsls	r0, r0, #1
	movs	r2, #244
	adds	r3, r6, r0
	lsls	r2, r2, #1
	ldr	r7, [r3, #0]
	adds	r3, r6, r2
	ldr	r5, [r3, #0]
	movs	r3, #1
	mov	fp, r3
	b.n	.L_080d2886
.L_080d2880:
	movs	r0, #1
	bl	sub_08013560
.L_080d2886:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_080d2880
	ldr	r3, [pc, #200]
	movs	r0, #139
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrb	r2, [r3, #0]
	movs	r3, #2
	subs	r3, r3, r2
	lsls	r0, r3, #1
	adds	r0, r0, r3
	bl	sub_08013560
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080d28cc
	movs	r3, #14
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r7, #10]
	adds	r1, r2, r3
	cmp	r5, #0
	beq.n	.L_080d28c4
	movs	r0, #14
	ldrsh	r2, [r5, r0]
	ldrh	r3, [r5, #10]
	adds	r2, r2, r3
	cmp	r1, r2
	bge.n	.L_080d28c4
	adds	r1, r2, #0
.L_080d28c4:
	cmp	r1, #15
	ble.n	.L_080d28cc
	movs	r2, #0
	mov	fp, r2
.L_080d28cc:
	movs	r0, #203
	lsls	r0, r0, #4
	adds	r3, r6, r0
	adds	r0, #2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	adds	r3, r6, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r0, #192
	lsls	r0, r0, #4
	adds	r0, #180
	adds	r3, r6, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	mov	r0, fp
	bl	sub_08038378
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d2920
	movs	r1, #4
	mov	r0, r8
	bl	sub_080d3240
	mov	r0, r9
	bl	sub_08038100
	bl	sub_08038140
	ldr	r3, [pc, #76]
	movs	r2, #139
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	beq.n	.L_080d2948
	mov	r0, r8
	movs	r1, #4
	bl	.L_080d2810
	b.n	.L_080d2948
.L_080d2920:
	movs	r1, #3
	mov	r0, r8
	bl	sub_080d3240
	mov	r0, r9
	bl	sub_08038100
	bl	sub_08038140
	ldr	r3, [pc, #36]
	movs	r0, #139
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	beq.n	.L_080d2948
	mov	r0, r8
	movs	r1, #3
	bl	.L_080d2810
.L_080d2948:
	adds	r0, r5, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038378
	pop	{pc}
	.align 2, 0
