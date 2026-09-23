.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.global Func_081192d0
	.thumb_func
Func_081192d0:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #36]
	sub	sp, #4
	adds	r3, r1, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_0811936c
	adds	r3, r1, #0
	adds	r3, #80
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #64]
	lsls	r2, r2, #3
	adds	r7, r2, r3
	adds	r3, r1, #0
	adds	r3, #82
	ldrb	r3, [r3, #0]
	ldr	r5, [pc, #56]
	cmp	r3, #0
	bne.n	.L_08119366
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #36]
	strh	r3, [r5, #0]
	strh	r3, [r5, #4]
	ldr	r3, [pc, #36]
	strh	r2, [r5, #2]
	strh	r3, [r5, #6]
	movs	r6, #0
.L_08119316:
	ldr	r3, [pc, #40]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_08119344
	adds	r4, #1
	cmp	r4, #24
	ble.n	.L_08119356
	b.n	.L_08119366
	movs	r0, r0
	.4byte 0x00000045
	.4byte 0x00000058
	.4byte 0x00000043
	.4byte 0x02003874
	.4byte 0x02003a74
	.2byte 0x124c
	.2byte 0x0300
.L_08119344:
	ldrh	r2, [r5, #4]
	ldrh	r3, [r7, #4]
	movs	r4, #0
	cmp	r2, r3
	bne.n	.L_08119356
	ldrh	r2, [r5, #6]
	ldrh	r3, [r7, #6]
	cmp	r2, r3
	beq.n	.L_0811936c
.L_08119356:
	movs	r0, #1
	str	r4, [sp, #0]
	bl	sub_08013560
	adds	r6, #1
	ldr	r4, [sp, #0]
	cmp	r6, #29
	ble.n	.L_08119316
.L_08119366:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811936e
.L_0811936c:
	movs	r0, #0
.L_0811936e:
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.align 2, 0
