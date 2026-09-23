.syntax unified
	.thumb
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_08003b70, 0x08003b70
	.set sub_08003c3c, 0x08003c3c
	.set sub_08003ce0, 0x08003ce0
	.set sub_080040e8, 0x080040e8
	.set sub_08004760, 0x08004760
	.set sub_0800479c, 0x0800479c
	.set sub_08005340, 0x08005340
	.global Unnamed_080f2d54
	.global Func_080f2d54
	.thumb_func
Unnamed_080f2d54:
Func_080f2d54:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r2, [pc, #92]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r6, [pc, #88]
	bl	sub_080040e8
	movs	r0, #1
	bl	sub_08003b70
	bl	sub_08004760
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #64]
	movs	r5, #0
	strh	r5, [r3, #10]
	ldr	r5, [pc, #60]
	adds	r0, r6, #0
	mov	r8, r3
	bl	sub_08002f40
	adds	r1, r5, #0
	bl	sub_08005340
	adds	r6, r5, #0
	movs	r1, #160
	ldr	r3, [pc, #44]
	adds	r0, r6, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #44]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r6, r6, r3
	b.n	.L_080f2dd4
	.4byte 0x00000685
	.4byte 0x00001440
	.4byte 0x03001d18
	.4byte 0x00000019
	.4byte 0x0400000c
	.4byte 0x03001ad0
	.4byte 0x02010000
	.4byte 0x040000d4
	.2byte 0x0070
	.2byte 0x8400
.L_080f2dd4:
	adds	r0, r6, #0
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r6, r6, r3
	adds	r0, r6, #0
	ldr	r3, [pc, #88]
	ldr	r1, [pc, #100]
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r6, r6, r3
	movs	r5, #0
	movs	r2, #0
	mov	r3, r8
.L_080f2dfe:
	adds	r5, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r5, #3
	bls.n	.L_080f2dfe
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #72]
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #76]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_0800479c
	bl	sub_08004760
	movs	r0, #1
	bl	sub_08003c3c
	bl	sub_08003ce0
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	ldr	r0, [r3, #0]
	movs	r3, #3
	lsrs	r0, r0, #3
	ands	r0, r3
	lsls	r0, r0, #10
	movs	r5, #0
	b.n	.L_080f2e80
	.4byte 0x00001540
	.4byte 0x040000d4
	.4byte 0x06003000
	.4byte 0x84000200
	.4byte 0x06004000
	.4byte 0x84001000
	.4byte 0x03001ad0
	.4byte 0x04000010
	.4byte 0x84000004
	.2byte 0x1e40
	.2byte 0x0300
.L_080f2e68:
	movs	r0, #1
	adds	r5, #1
	bl	sub_080030f8
	cmp	r5, #119
	bhi.n	.L_080f2e98
	ldr	r3, [pc, #44]
	ldr	r0, [r3, #0]
	movs	r3, #3
	lsrs	r0, r0, #3
	ands	r0, r3
	lsls	r0, r0, #10
.L_080f2e80:
	ldr	r3, [pc, #36]
	adds	r0, r0, r6
	ldr	r1, [pc, #36]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #36]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f2e68
.L_080f2e98:
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001e40
	.4byte 0x040000d4
	.4byte 0x06004100
	.4byte 0x840000d0
	.4byte 0x03001c94
