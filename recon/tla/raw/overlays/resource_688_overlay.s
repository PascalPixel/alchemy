.syntax unified
	.thumb
	.4byte 0x23006d02
	.4byte 0x20017693
	.2byte 0x4770
	.2byte 0x0000
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x873c
	.2byte 0x0200
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	movs	r0, #0
	bx	lr
	.global Func_02000050
	.thumb_func
Func_02000050:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x876c
	.2byte 0x0200
	.global Func_02000058
	.thumb_func
Func_02000058:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x879c
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #72]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #118
	adds	r2, r5, r3
	adds	r6, r0, #0
	movs	r3, #0
	movs	r0, #150
	strh	r3, [r2, #0]
	lsls	r0, r0, #4
	bl 0x02008514
	movs	r0, #123
	bl 0x020085dc
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r3, #8
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	subs	r2, #104
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x020085bc
	bl 0x020085c4
	movs	r3, #4
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl 0x020085ac
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #72]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #118
	adds	r2, r5, r3
	adds	r6, r0, #0
	movs	r3, #0
	movs	r0, #150
	strh	r3, [r2, #0]
	lsls	r0, r0, #4
	bl 0x0200850c
	movs	r0, #123
	bl 0x020085dc
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r3, #8
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	subs	r2, #104
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x020085bc
	bl 0x020085c4
	movs	r3, #4
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl 0x020085ac
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #9
	bl 0x0200850c
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200858c
	pop	{pc}
	.global Func_02000118
	.thumb_func
Func_02000118:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88d4
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x02008504
	cmp	r0, #0
	beq.n	.L_02000168
	bl 0x0200855c
	movs	r0, #0
	bl 0x020085cc
	bl 0x020085b4
	bl 0x020085c4
	ldr	r2, [pc, #72]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r1, r2, r3
	movs	r3, #6
	strb	r3, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #118
	adds	r2, r2, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #8
	movs	r1, #7
	bl 0x02008594
	bl 0x02008564
	b.n	.L_02000188
.L_02000168:
	ldr	r2, [pc, #32]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r1, r2, r3
	movs	r3, #4
	str	r3, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	adds	r3, r2, r1
	strb	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #118
	adds	r2, r2, r3
	strh	r0, [r2, #0]
.L_02000188:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.global Func_02000190
	.thumb_func
Func_02000190:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	mov	r8, r2
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r6, [pc, #348]
	adds	r2, #88
	str	r2, [r3, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r7, r6, r3
	ldr	r0, [r7, #0]
	mov	sl, r2
	bl 0x0200856c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #7
	bl 0x0200850c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #180
	bl 0x02008514
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #9
	bl 0x02008504
	cmp	r0, #0
	beq.n	.L_020001fc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200858c
.L_020001fc:
	movs	r0, #9
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #10
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #11
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #12
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #13
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #14
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #15
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #16
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #17
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r0, #18
	bl 0x0200856c
	movs	r1, #0
	bl 0x0200851c
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r5, r6, r2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #90
	bne.n	.L_02000296
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r6, r2
	movs	r2, #1
	strh	r2, [r3, #0]
	ldr	r0, [pc, #120]
	movs	r1, #1
	bl 0x020085a4
.L_02000296:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	beq.n	.L_020002a2
	cmp	r3, #5
	bne.n	.L_020002ce
.L_020002a2:
	movs	r3, #8
	mov	r2, r8
	str	r3, [r7, #0]
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x020085b4
	bl 0x020085c4
	movs	r3, #4
	mov	r2, r8
	str	r3, [r7, #0]
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r2, sl
	str	r2, [r3, #0]
.L_020002ce:
	ldr	r3, [pc, #52]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_020002fa
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x02008504
	cmp	r0, #0
	beq.n	.L_020002fa
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #107
	bl 0x02008514
	bl 0x020083cc
.L_020002fa:
	movs	r0, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x00c7
	.2byte 0x0000
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #20
	ldr	r7, [r3, #108]
	adds	r5, r0, #0
	movs	r3, #0
	mov	sl, r1
	str	r3, [sp, #16]
	str	r3, [sp, #12]
	movs	r6, #56
	cmp	r5, #8
	beq.n	.L_02000332
	adds	r6, r5, #0
.L_02000332:
	adds	r0, r6, #0
	bl 0x020085d4
	mov	r8, r0
	cmp	r5, #7
	bne.n	.L_02000366
	movs	r3, #10
	str	r3, [sp, #16]
	str	r3, [sp, #12]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	add	r4, sp, #4
	add	r2, sp, #12
	add	r3, sp, #8
	add	r1, sp, #16
	str	r4, [sp, #0]
	bl 0x02008544
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #8]
	adds	r3, r3, r2
	subs	r5, r3, #5
	b.n	.L_02000370
.L_02000366:
	movs	r3, #5
	str	r3, [sp, #16]
	movs	r3, #10
	str	r3, [sp, #12]
	movs	r5, #5
.L_02000370:
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldrh	r0, [r2, #0]
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	mov	r2, r8
	lsls	r3, r2, #16
	lsls	r0, r0, #16
	movs	r2, #2
	orrs	r3, r2
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	asrs	r0, r0, #16
	bl 0x0200852c
	adds	r2, r5, #0
	adds	r0, r6, #0
	movs	r1, #0
	movs	r3, #5
	bl 0x0200853c
	adds	r5, r0, #0
	b.n	.L_020003a6
.L_020003a0:
	movs	r0, #1
	bl 0x020084fc
.L_020003a6:
	bl 0x02008534
	cmp	r0, #0
	beq.n	.L_020003a0
	mov	r0, sl
	bl 0x02008554
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x02008524
	bl 0x0200854c
	add	sp, #20
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #288]
	movs	r3, #139
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r3, #2
	ldrb	r6, [r5, #0]
	strb	r3, [r5, #0]
	bl 0x0200855c
	movs	r0, #0
	bl 0x020085cc
	movs	r1, #138
	movs	r2, #136
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r2, #143
	movs	r0, #8
	adds	r1, #30
	bl 0x02008574
	ldr	r1, [pc, #248]
	movs	r0, #8
	bl 0x0200857c
	movs	r0, #80
	bl 0x02008554
	ldr	r0, [pc, #240]
	bl 0x0200859c
	movs	r0, #6
	movs	r1, #90
	bl 0x02008310
	movs	r0, #7
	movs	r1, #90
	bl 0x02008310
	movs	r0, #6
	movs	r1, #180
	bl 0x02008310
	movs	r0, #7
	movs	r1, #90
	bl 0x02008310
	movs	r0, #6
	movs	r1, #250
	bl 0x02008310
	movs	r0, #7
	movs	r1, #180
	bl 0x02008310
	movs	r0, #5
	movs	r1, #250
	bl 0x02008310
	movs	r0, #8
	movs	r1, #90
	bl 0x02008310
	movs	r0, #7
	movs	r1, #180
	bl 0x02008310
	movs	r0, #8
	movs	r1, #180
	bl 0x02008310
	movs	r0, #7
	movs	r1, #180
	bl 0x02008310
	movs	r0, #6
	movs	r1, #90
	bl 0x02008310
	movs	r0, #7
	movs	r1, #250
	bl 0x02008310
	movs	r0, #5
	movs	r1, #250
	bl 0x02008310
	movs	r0, #7
	movs	r1, #250
	bl 0x02008310
	movs	r0, #6
	movs	r1, #90
	bl 0x02008310
	movs	r0, #5
	movs	r1, #90
	bl 0x02008310
	movs	r0, #8
	movs	r1, #90
	bl 0x02008310
	movs	r0, #7
	movs	r1, #250
	bl 0x02008310
	movs	r0, #4
	movs	r1, #90
	bl 0x02008310
	movs	r1, #250
	movs	r0, #7
	bl 0x02008310
	movs	r0, #8
	bl 0x02008584
	movs	r0, #10
	bl 0x02008554
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #30
	str	r3, [r2, #0]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #88
	str	r3, [r2, #0]
	bl 0x020085bc
	bl 0x020085c4
	movs	r0, #1
	bl 0x020085ac
	strb	r6, [r5, #0]
	bl 0x02008564
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x020085e4
	.4byte 0x00002881
	.section .rodata,"a",%progbits
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000027
	.4byte 0x00000007
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000001b8
	.4byte 0x40000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000c7
	.4byte 0x1012d002
	.4byte 0xffffffff
	.4byte 0x102040c7
	.4byte 0xffffffff
	.4byte 0x103010c8
	.4byte 0xffffffff
	.4byte 0x104020c7
	.4byte 0xffffffff
	.4byte 0x105010c8
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0008
	.4byte 0x02008728
	.4byte 0x000e0000
	.4byte 0x00000000
	.4byte 0x00040000
	.4byte 0x0002c000
	.4byte 0xffff0156
	.4byte 0x00000001
	.4byte 0x027c0000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00028000
	.4byte 0xffff0156
	.4byte 0x00000001
	.4byte 0x029c0000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x01020000
	.4byte 0xffff0156
	.4byte 0x00000001
	.4byte 0x029c0000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0xffff0156
	.4byte 0x00000001
	.4byte 0x02dc0000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01028000
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x02e00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x01024000
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x02c00000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x01024000
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x00340000
	.4byte 0x00000000
	.4byte 0x031c0000
	.4byte 0x00028000
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x00340000
	.4byte 0x00000000
	.4byte 0x033c0000
	.4byte 0x01028000
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x00340000
	.4byte 0x00000000
	.4byte 0x035c0000
	.4byte 0x01028000
	.4byte 0xffff01a8
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008061
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x020080b1
	.4byte 0x00009815
	.4byte 0xffff0013
	.4byte 0x02008101
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
