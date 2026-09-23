.syntax unified
	.thumb
	.set sub_080037d4, 0x080037d4
	.set sub_080fa324, 0x080fa324
	.set sub_080fa4bc, 0x080fa4bc
	.set sub_080faa58, 0x080faa58
	.set sub_080fb2cc, 0x080fb2cc
	.global Func_080f9080
	.thumb_func
Func_080f9080:
	push	{r5, r6, r7, lr}
	movs	r5, #240
	ldr	r3, [pc, #172]
	adds	r6, r0, #0
	lsls	r5, r5, #8
	ands	r5, r6
	ands	r6, r3
	cmp	r6, #17
	bne.n	.L_080f90b0
	ldr	r5, [pc, #164]
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080f909c
	b.n	.L_080f91e0
.L_080f909c:
	ldr	r0, [pc, #156]
	movs	r1, #7
	bl	sub_080fa4bc
	ldrb	r3, [r5, #0]
	ldr	r2, [pc, #152]
	adds	r3, #1
	strb	r3, [r5, #0]
	movs	r3, #19
	b.n	.L_080f91c2
.L_080f90b0:
	ldr	r3, [pc, #144]
	cmp	r6, r3
	bne.n	.L_080f90c6
	ldr	r3, [pc, #144]
	movs	r2, #0
	strh	r2, [r3, #6]
	ldr	r0, [pc, #140]
	movs	r1, #3
	bl	sub_080fa4bc
	b.n	.L_080f91e0
.L_080f90c6:
	cmp	r6, #99
	ble.n	.L_080f9108
	ldr	r7, [pc, #132]
	lsls	r4, r6, #3
	adds	r3, r4, #4
	ldrh	r2, [r7, r3]
	cmp	r2, #7
	bne.n	.L_080f90f2
	ldr	r1, [pc, #124]
.L_080f90d8:
	lsls	r5, r2, #1
	adds	r3, r5, r2
	lsls	r3, r3, #2
	ldr	r3, [r1, r3]
	ldrb	r3, [r3, #4]
	cmp	r3, #0
	beq.n	.L_080f90f6
	subs	r2, #1
	cmp	r2, #3
	bgt.n	.L_080f90d8
	movs	r2, #7
	movs	r5, #14
	b.n	.L_080f90f6
.L_080f90f2:
	ldr	r1, [pc, #96]
	lsls	r5, r2, #1
.L_080f90f6:
	adds	r3, r5, r2
	lsls	r3, r3, #2
	ldr	r0, [r1, r3]
	ldr	r1, [r7, r4]
	bl	sub_080faa58
	ldr	r3, [pc, #68]
	strh	r6, [r3, r5]
	b.n	.L_080f91e0
.L_080f9108:
	cmp	r6, #79
	ble.n	.L_080f9164
	ldr	r0, [pc, #44]
	movs	r1, #255
	movs	r2, #0
	bl	sub_080fb2cc
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #60]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #60]
	lsls	r0, r6, #16
	strh	r2, [r3, #0]
	lsrs	r0, r0, #16
	bl	sub_080fa324
	ldr	r2, [pc, #52]
	movs	r3, #10
	b.n	.L_080f91c2
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000fff
	.4byte 0x02003014
	.4byte 0x02004290
	.4byte 0x0200303c
	.4byte 0x00000121
	.4byte 0x02003020
	.4byte 0x02004360
	.4byte 0x080fc684
	.4byte 0x080fc624
	.4byte 0x02003034
	.4byte 0x02003008
	.2byte 0x3000
	.2byte 0x0200
.L_080f9164:
	cmp	r6, #18
	beq.n	.L_080f91e0
	ldr	r2, [pc, #56]
	ldrb	r3, [r2, #0]
	cmp	r6, r3
	beq.n	.L_080f91e0
	strb	r6, [r2, #0]
	cmp	r6, #70
	beq.n	.L_080f9180
	cmp	r6, #75
	beq.n	.L_080f9180
	movs	r0, #2
	cmp	r6, #67
	bne.n	.L_080f9182
.L_080f9180:
	movs	r0, #3
.L_080f9182:
	bl	sub_080037d4
	lsls	r0, r6, #16
	lsrs	r0, r0, #16
	bl	sub_080fa324
	movs	r3, #128
	lsls	r3, r3, #5
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080f91ac
	ldr	r2, [pc, #12]
	ldr	r3, [pc, #4]
	b.n	.L_080f91b0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0200303c
	.2byte 0x3008
	.2byte 0x0200
.L_080f91ac:
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #24]
.L_080f91b0:
	strh	r3, [r2, #0]
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #28]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #28]
	movs	r3, #0
.L_080f91c2:
	strb	r3, [r2, #0]
	b.n	.L_080f91e0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000004
	.4byte 0x02003008
	.4byte 0x02003034
	.4byte 0x02003010
	.2byte 0x3014
	.2byte 0x0200
.L_080f91e0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
