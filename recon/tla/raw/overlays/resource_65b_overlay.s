.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008281, 0x02008039, 0x02008045, 0x0200804d, 0x020080c1, 0x02008041, 0x02008295
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8348
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8378
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8390
	.2byte 0x0200
	push	{lr}
	bl 0x020082b8
	movs	r0, #0
	bl 0x02008340
	ldr	r0, [pc, #84]
	bl 0x02008300
	movs	r1, #0
	movs	r0, #8
	bl 0x02008308
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl 0x020082c8
	cmp	r0, #0
	bne.n	.L_02000096
	movs	r0, #8
	movs	r1, #3
	bl 0x020082f0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #2
	bl 0x02008310
	b.n	.L_020000b2
.L_02000096:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #8
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #2
	bl 0x02008310
.L_020000b2:
	bl 0x020082c0
	pop	{pc}
	.4byte 0x0000190a
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x83d8
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #2
	sub	sp, #8
	bl 0x02008298
	cmp	r0, #0
	beq.n	.L_020000da
	b.n	.L_02000216
.L_020000da:
	bl 0x020082b8
	movs	r0, #0
	bl 0x02008340
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008298
	cmp	r0, #0
	bne.n	.L_02000122
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x02008328
	movs	r0, #164
	movs	r1, #1
	movs	r2, #130
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x02008330
	ldr	r3, [pc, #264]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #158
	subs	r2, #216
	bl 0x020082d8
.L_02000122:
	movs	r0, #8
	bl 0x020082d0
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x020082d0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #148
	ands	r5, r3
	strb	r5, [r0, #0]
	movs	r2, #252
	movs	r0, #9
	bl 0x020082d8
	movs	r1, #170
	movs	r2, #252
	movs	r0, #8
	bl 0x020082e0
	movs	r0, #9
	bl 0x020082e8
	movs	r0, #8
	bl 0x020082d0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x020082d0
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #128
	orrs	r5, r3
	strb	r5, [r0, #0]
	lsls	r1, r1, #7
	movs	r0, #8
	movs	r2, #0
	bl 0x02008320
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x02008320
	ldr	r5, [pc, #136]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x020082e8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x02008320
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x020082a0
	bl 0x02008338
	movs	r3, #8
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #11
	movs	r1, #9
	movs	r2, #4
	movs	r3, #1
	bl 0x020082a8
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008298
	cmp	r0, #0
	bne.n	.L_02000216
	ldr	r0, [pc, #68]
	bl 0x02008300
	movs	r0, #10
	bl 0x020082b0
	movs	r0, #8
	movs	r1, #1
	bl 0x020082f8
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x02008310
	movs	r0, #9
	movs	r1, #3
	bl 0x020082f0
	movs	r0, #9
	movs	r1, #0
	bl 0x02008318
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x020082a0
	bl 0x020082c0
.L_02000216:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1908
	.2byte 0x0000
	push	{lr}
	bl 0x020082b8
	movs	r0, #0
	bl 0x02008340
	ldr	r0, [pc, #72]
	bl 0x02008300
	movs	r0, #164
	movs	r1, #1
	movs	r2, #130
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl 0x02008330
	bl 0x02008338
	movs	r0, #8
	movs	r1, #3
	bl 0x020082f0
	movs	r0, #8
	movs	r1, #0
	bl 0x02008318
	movs	r0, #9
	movs	r1, #3
	bl 0x020082f0
	movs	r1, #0
	movs	r0, #9
	bl 0x02008318
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #22
	bl 0x020082a0
	bl 0x020082c0
	pop	{pc}
	.2byte 0x1910
	.2byte 0x0000
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r0, #0
	bx	lr
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x080201e9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c80d1, 0x080c80d9, 0x080c80f1, 0x080c8129, 0x080c8149, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c84e1
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0xffff0000
	.4byte 0x00000098
	.4byte 0x400000b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000035
	.4byte 0x1010a002
	.4byte 0xffffffff
	.4byte 0x1020b002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x00bc0000
	.4byte 0x00000000
	.4byte 0x01040000
	.4byte 0x00014000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x01040000
	.4byte 0x00014000
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
	.4byte 0x00000002
	.4byte 0x0913000a
	.4byte 0x020080c9
	.4byte 0x00000002
	.4byte 0x0916000a
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0x09130008
	.4byte 0x02008055
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001912
	.4byte 0x00000000
	.4byte 0x09130009
	.4byte 0x0000190d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001913
	.4byte 0x00008d15
	.4byte 0x09130008
	.4byte 0x0000190e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001914
	.4byte 0x00008d15
	.4byte 0x09130009
	.4byte 0x0000190f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001915
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
