.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008361, 0x02008039, 0x02008045, 0x02008065, 0x020082f1, 0x02008041, 0x020083cd
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x854c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{lr}
	movs	r0, #150
	lsls	r0, r0, #4
	bl 0x02008444
	cmp	r0, #0
	bne.n	.L_02000056
	ldr	r0, [pc, #8]
	b.n	.L_02000058
.L_02000056:
	ldr	r0, [pc, #8]
.L_02000058:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200857c
	.2byte 0x859c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x85bc
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #224]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #2
	bne.n	.L_0200014c
	bl 0x0200846c
	movs	r0, #0
	bl 0x02008524
	movs	r0, #8
	bl 0x02008484
	ldr	r3, [r0, #8]
	cmp	r3, #0
	bne.n	.L_020000de
	movs	r2, #16
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x02008534
	movs	r2, #16
	movs	r3, #192
	lsls	r3, r3, #6
	movs	r1, #0
	negs	r2, r2
	movs	r0, #8
	bl 0x0200852c
	movs	r0, #8
	bl 0x020084b4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #148]
	adds	r2, #153
	bl 0x0200848c
	movs	r1, #236
	movs	r2, #196
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200849c
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x020084f4
	b.n	.L_02000148
.L_020000de:
	movs	r0, #4
	movs	r1, #0
	movs	r2, #16
	bl 0x02008534
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200848c
	movs	r1, #220
	movs	r2, #220
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200849c
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #8
	ldr	r1, [pc, #68]
	bl 0x0200848c
	movs	r0, #8
	movs	r1, #2
	bl 0x020084c4
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02008484
	cmp	r0, #0
	beq.n	.L_02000138
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x02008494
.L_02000138:
	movs	r0, #8
	bl 0x020084b4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x020084bc
.L_02000148:
	bl 0x02008474
.L_0200014c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, lr}
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #108]
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200018c
	ldr	r3, [pc, #220]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #8
	movs	r2, #0
	bl 0x020084d4
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #30
	bl 0x02008504
.L_0200018c:
	ldr	r0, [pc, #192]
	bl 0x020084dc
	movs	r1, #0
	movs	r0, #8
	bl 0x020084e4
	bl 0x0200853c
	movs	r1, #0
	bl 0x0200847c
	cmp	r0, #0
	bne.n	.L_0200022c
	movs	r0, #15
	bl 0x02008464
	movs	r1, #0
	movs	r0, #8
	bl 0x020084ec
	movs	r0, #5
	bl 0x02008464
	movs	r2, #0
	movs	r0, #8
	movs	r1, #4
	bl 0x020084cc
	movs	r1, #0
	movs	r0, #8
	bl 0x020084f4
	movs	r0, #5
	bl 0x02008464
	ldr	r5, [pc, #116]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x02008484
	ldr	r3, [r0, #16]
	movs	r2, #196
	lsls	r2, r2, #17
	ldr	r0, [r5, #0]
	cmp	r3, r2
	bge.n	.L_020001fc
	movs	r1, #236
	movs	r2, #188
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200849c
	b.n	.L_02000208
.L_020001fc:
	movs	r1, #236
	movs	r2, #204
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200849c
.L_02000208:
	movs	r0, #4
	movs	r1, #0
	bl 0x020084f4
	movs	r0, #10
	bl 0x02008464
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r3, r2
	movs	r2, #1
	strh	r2, [r3, #0]
	movs	r0, #1
	bl 0x0200850c
	b.n	.L_02000248
.L_0200022c:
	movs	r0, #20
	bl 0x02008464
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #8
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x020084ec
.L_02000248:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x289a
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r1, #0
	adds	r6, r2, #0
	bl 0x0200846c
	movs	r0, #0
	bl 0x02008524
	movs	r0, #158
	bl 0x02008544
	ldrh	r1, [r5, #4]
	ldrh	r2, [r5, #6]
	ldr	r0, [r5, #0]
	bl 0x02008454
	ldr	r5, [pc, #96]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x02008484
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200848c
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x020084c4
	ldr	r0, [r5, #0]
	cmp	r6, #0
	bne.n	.L_020002b2
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	bl 0x020084a4
	b.n	.L_020002bc
.L_020002b2:
	movs	r2, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x020084ac
.L_020002bc:
	movs	r0, #10
	bl 0x02008464
	adds	r0, r7, #0
	bl 0x0200850c
	bl 0x02008514
	bl 0x0200851c
	bl 0x02008474
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r1, r0, #0
	movs	r2, #0
	ldr	r0, [pc, #8]
	bl 0x02008254
	pop	{pc}
	.2byte 0x0000
	.2byte 0x8610
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8618
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #80]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200034a
	ldr	r3, [pc, #72]
	movs	r1, #9
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000310:
	ldr	r4, [pc, #64]
	lsls	r3, r1, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #4
	bne.n	.L_02000310
	ldr	r3, [pc, #48]
	movs	r1, #15
	strh	r0, [r3, #0]
	adds	r3, #22
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000332:
	ldr	r4, [pc, #32]
	lsls	r3, r1, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #12
	bne.n	.L_02000332
	ldr	r3, [pc, #20]
	strh	r0, [r3, #0]
.L_0200034a:
	pop	{pc}
	.4byte 0x0300122c
	.4byte 0x05000172
	.4byte 0x05000160
	.4byte 0x05000168
	.2byte 0x0178
	.2byte 0x0500
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	ldr	r3, [pc, #80]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02008484
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r0, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #56]
	bl 0x0200843c
	movs	r0, #48
	adds	r0, #255
	bl 0x0200844c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x02008444
	cmp	r0, #0
	beq.n	.L_020003b4
	movs	r0, #9
	movs	r1, #1
	bl 0x020084fc
	b.n	.L_020003be
.L_020003b4:
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x020084bc
.L_020003be:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x82f9
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #252
	lsls	r0, r0, #3
	adds	r0, #255
	sub	sp, #8
	bl 0x02008444
	cmp	r0, #0
	bne.n	.L_02000436
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #57
	movs	r1, #52
	movs	r2, #64
	movs	r3, #15
	bl 0x0200845c
	movs	r5, #3
	movs	r6, #6
	movs	r0, #60
	movs	r1, #53
	movs	r2, #67
	movs	r3, #16
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200845c
	movs	r0, #50
	movs	r1, #52
	movs	r2, #16
	movs	r3, #16
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200845c
	movs	r6, #10
	movs	r0, #50
	movs	r1, #61
	movs	r2, #16
	movs	r3, #28
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200845c
	movs	r0, #61
	movs	r1, #61
	movs	r2, #16
	movs	r3, #63
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200845c
.L_02000436:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.irp EntryTarget, 0x080000d1, 0x080003c9, 0x080003d9, 0x08020171, 0x08020179, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8139, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85e9, 0x080c85f9, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
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
	.4byte 0x000000c8
	.4byte 0x101030c7
	.4byte 0xffffffff
	.4byte 0x102020c9
	.4byte 0xffffffff
	.4byte 0x10301028
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x000000c8
	.4byte 0x101050c7
	.4byte 0xffffffff
	.4byte 0x102050c9
	.4byte 0xffffffff
	.4byte 0x10301028
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff01a4
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00260033
	.4byte 0x00010001
	.4byte 0xffff0006
	.4byte 0x02008604
	.4byte 0x00160046
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020082dd
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x0200806d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008159
	.4byte 0x00008d15
	.4byte 0xffff0408
	.4byte 0x02008159
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
