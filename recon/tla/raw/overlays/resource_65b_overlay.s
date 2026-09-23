.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000310, 0x02000310
	.set sub_02000344, 0x02000344
	.set sub_02000364, 0x02000364
	.set sub_0200036a, 0x0200036a
	.set sub_02000374, 0x02000374
	.set sub_02000378, 0x02000378
	.set sub_02000384, 0x02000384
	.set sub_02000394, 0x02000394
	.set sub_0200039e, 0x0200039e
	.set sub_020003a2, 0x020003a2
	.set sub_020003c0, 0x020003c0
	.set sub_020003f6, 0x020003f6
	.set sub_020003f8, 0x020003f8
	.set sub_02000408, 0x02000408
	.set sub_02000422, 0x02000422
	.set sub_02000424, 0x02000424
	.set sub_02000430, 0x02000430
	.set sub_02000434, 0x02000434
	.set sub_0200043e, 0x0200043e
	.set sub_02000440, 0x02000440
	.set sub_02000442, 0x02000442
	.set sub_02000452, 0x02000452
	.set sub_0200046c, 0x0200046c
	.set sub_02000472, 0x02000472
	.set sub_02000486, 0x02000486
	.set sub_02000494, 0x02000494
	.set sub_020004a4, 0x020004a4
	.set sub_020004b0, 0x020004b0
	.set sub_020004ca, 0x020004ca
	.set sub_020004d4, 0x020004d4
	.set sub_020004de, 0x020004de
	.set sub_020004e0, 0x020004e0
	.set sub_020004e4, 0x020004e4
	.set sub_020004ee, 0x020004ee
	.set sub_02000506, 0x02000506
	.set sub_02000514, 0x02000514
	.set sub_0200051e, 0x0200051e
	.set sub_02000534, 0x02000534
	.set sub_02000538, 0x02000538
	.set sub_02000542, 0x02000542
	.set sub_02000552, 0x02000552
	.set sub_0200056e, 0x0200056e
	.set sub_02000572, 0x02000572
	.set sub_02000576, 0x02000576
	.set sub_02000582, 0x02000582
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
	bl	sub_02000310
	movs	r0, #0
	bl	sub_0200039e
	ldr	r0, [pc, #84]
	bl	sub_02000364
	movs	r1, #0
	movs	r0, #8
	bl	sub_02000374
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	sub_02000344
	cmp	r0, #0
	bne.n	.L_02000096
	movs	r0, #8
	movs	r1, #3
	bl	sub_02000378
	movs	r0, #8
	movs	r1, #0
	movs	r2, #2
	bl	sub_020003a2
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
	bl	sub_020003c0
.L_020000b2:
	bl	sub_02000374
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
	bl	sub_0200036a
	cmp	r0, #0
	beq.n	.L_020000da
	b.n	.L_02000216
.L_020000da:
	bl	sub_02000394
	movs	r0, #0
	bl	sub_02000422
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02000384
	cmp	r0, #0
	bne.n	.L_02000122
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl	sub_02000424
	movs	r0, #164
	movs	r1, #1
	movs	r2, #130
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl	sub_0200043e
	ldr	r3, [pc, #264]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #158
	subs	r2, #216
	bl	sub_020003f8
.L_02000122:
	movs	r0, #8
	bl	sub_020003f6
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #9
	bl	sub_02000408
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #148
	ands	r5, r3
	strb	r5, [r0, #0]
	movs	r2, #252
	movs	r0, #9
	bl	sub_02000422
	movs	r1, #170
	movs	r2, #252
	movs	r0, #8
	bl	sub_02000434
	movs	r0, #9
	bl	sub_02000442
	movs	r0, #8
	bl	sub_02000430
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #9
	bl	sub_02000440
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #128
	orrs	r5, r3
	strb	r5, [r0, #0]
	lsls	r1, r1, #7
	movs	r0, #8
	movs	r2, #0
	bl	sub_020004a4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl	sub_020004b0
	ldr	r5, [pc, #136]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl	sub_02000486
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl	sub_020004ca
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_02000452
	bl	sub_020004ee
	movs	r3, #8
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #11
	movs	r1, #9
	movs	r2, #4
	movs	r3, #1
	bl	sub_02000472
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_0200046c
	cmp	r0, #0
	bne.n	.L_02000216
	ldr	r0, [pc, #68]
	bl	sub_020004de
	movs	r0, #10
	bl	sub_02000494
	movs	r0, #8
	movs	r1, #1
	bl	sub_020004e4
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl	sub_02000506
	movs	r0, #9
	movs	r1, #3
	bl	sub_020004ee
	movs	r0, #9
	movs	r1, #0
	bl	sub_0200051e
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_020004b0
	bl	sub_020004d4
.L_02000216:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x1908
	.2byte 0x0000
	push	{lr}
	bl	sub_020004e0
	movs	r0, #0
	bl	sub_0200056e
	ldr	r0, [pc, #72]
	bl	sub_02000534
	movs	r0, #164
	movs	r1, #1
	movs	r2, #130
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #16
	negs	r1, r1
	bl	sub_02000576
	bl	sub_02000582
	movs	r0, #8
	movs	r1, #3
	bl	sub_02000542
	movs	r0, #8
	movs	r1, #0
	bl	sub_02000572
	movs	r0, #9
	movs	r1, #3
	bl	sub_02000552
	movs	r1, #0
	movs	r0, #9
	bl	sub_02000582
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #22
	bl	sub_02000514
	bl	sub_02000538
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
