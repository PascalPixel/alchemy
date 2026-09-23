.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080233a8, 0x080233a8
	.global Func_08023510
	.thumb_func
Func_08023510:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	ldr	r1, [pc, #8]
	adds	r5, r0, #0
	bl	sub_080233a8
	str	r6, [r5, #104]
	pop	{r5, r6, pc}
	.2byte 0xf1e8
	.2byte 0x0802
	push	{r5, lr}
	ldr	r3, [pc, #144]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	adds	r5, r0, #0
	cmp	r3, #12
	bhi.n	.L_080235ae
	ldr	r2, [pc, #128]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08023574
	.4byte 0x08023578
	.4byte 0x0802357c
	.4byte 0x08023580
	.4byte 0x08023584
	.4byte 0x08023588
	.4byte 0x0802358c
	.4byte 0x08023590
	.4byte 0x08023594
	.4byte 0x08023598
	.4byte 0x080235a2
	.4byte 0x080235a6
	.2byte 0x35aa
	.2byte 0x0802
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	ldr	r1, [pc, #72]
	b.n	.L_080235b0
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #68]
	b.n	.L_080235b0
	ldr	r1, [pc, #68]
	b.n	.L_080235b0
	ldr	r1, [pc, #68]
	b.n	.L_080235b0
	ldr	r1, [pc, #68]
	b.n	.L_080235b0
.L_080235ae:
	ldr	r1, [pc, #68]
.L_080235b0:
	adds	r0, r5, #0
	bl	sub_080233a8
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x08023540
	.4byte 0x0802f140
	.4byte 0x0802f158
	.4byte 0x0802f0b0
	.4byte 0x0802f0c8
	.4byte 0x0802f0e0
	.4byte 0x0802f0f8
	.4byte 0x0802f170
	.4byte 0x0802f188
	.4byte 0x0802f1a0
	.4byte 0x0802f1b8
	.4byte 0x0802f110
	.4byte 0x0802f128
	.4byte 0x0802f200
	.4byte 0x0802f098
