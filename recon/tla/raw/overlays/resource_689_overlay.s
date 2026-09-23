.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000490, 0x02000490
	.set sub_020004ec, 0x020004ec
	.set sub_02000510, 0x02000510
	.set sub_0200053a, 0x0200053a
	.set sub_0200054e, 0x0200054e
	.set sub_02000568, 0x02000568
	.set sub_0200056c, 0x0200056c
	.set sub_02000580, 0x02000580
	.set sub_0200059c, 0x0200059c
	.set sub_0200059e, 0x0200059e
	.set sub_020005a8, 0x020005a8
	.set sub_020005aa, 0x020005aa
	.set sub_020005be, 0x020005be
	.set sub_020005ca, 0x020005ca
	.set sub_020005ce, 0x020005ce
	.set sub_020005d2, 0x020005d2
	.set sub_020005da, 0x020005da
	.set sub_020005dc, 0x020005dc
	.set sub_020005f0, 0x020005f0
	.set sub_02000602, 0x02000602
	.set sub_02000610, 0x02000610
	.set sub_0200061a, 0x0200061a
	.set sub_0200061e, 0x0200061e
	.set sub_02000636, 0x02000636
	.set sub_02000652, 0x02000652
	.set sub_02000664, 0x02000664
	.set sub_0200066c, 0x0200066c
	.set sub_02000678, 0x02000678
	.set sub_0200067c, 0x0200067c
	.set sub_0200068e, 0x0200068e
	.set sub_02000690, 0x02000690
	.set sub_02000694, 0x02000694
	.set sub_020006a0, 0x020006a0
	.set sub_020006a2, 0x020006a2
	.set sub_020006c0, 0x020006c0
	.set sub_020006c8, 0x020006c8
	.set sub_020006ca, 0x020006ca
	.set sub_020006d8, 0x020006d8
	.set sub_02000702, 0x02000702
	.set sub_02000706, 0x02000706
	.set sub_02000722, 0x02000722
	.set sub_02000724, 0x02000724
	.set sub_02000732, 0x02000732
	.set sub_02000734, 0x02000734
	.set sub_02000746, 0x02000746
	.set sub_02000752, 0x02000752
	.set sub_02000762, 0x02000762
	.set sub_02000766, 0x02000766
	.set sub_02000788, 0x02000788
	.set sub_020007ae, 0x020007ae
	.set sub_020007ce, 0x020007ce
	.set sub_020007d2, 0x020007d2
	.set sub_020007de, 0x020007de
	.set sub_020007e6, 0x020007e6
	.set sub_020007e8, 0x020007e8
	.set sub_020007ea, 0x020007ea
	.set sub_02000802, 0x02000802
	.set sub_0200081c, 0x0200081c
	.set sub_0200084a, 0x0200084a
	.set sub_0200085e, 0x0200085e
	.set sub_0200086e, 0x0200086e
	.set sub_02000878, 0x02000878
	.set sub_02000880, 0x02000880
	.set sub_02000890, 0x02000890
	.set sub_020008ac, 0x020008ac
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
	bl	sub_02000490
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
	bl	sub_020004ec
	movs	r0, #0
	bl	sub_020005aa
	movs	r0, #8
	bl	sub_02000510
	ldr	r3, [r0, #8]
	cmp	r3, #0
	bne.n	.L_020000de
	movs	r2, #16
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl	sub_020005d2
	movs	r2, #16
	movs	r3, #192
	lsls	r3, r3, #6
	movs	r1, #0
	negs	r2, r2
	movs	r0, #8
	bl	sub_020005da
	movs	r0, #8
	bl	sub_02000568
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #148]
	adds	r2, #153
	bl	sub_0200054e
	movs	r1, #236
	movs	r2, #196
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl	sub_0200056c
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl	sub_020005ce
	b.n	.L_02000148
.L_020000de:
	movs	r0, #4
	movs	r1, #0
	movs	r2, #16
	bl	sub_0200061a
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl	sub_02000580
	movs	r1, #220
	movs	r2, #220
	movs	r0, #8
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl	sub_0200059e
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #8
	ldr	r1, [pc, #68]
	bl	sub_0200059c
	movs	r0, #8
	movs	r1, #2
	bl	sub_020005dc
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl	sub_020005a8
	cmp	r0, #0
	beq.n	.L_02000138
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl	sub_020005ca
.L_02000138:
	movs	r0, #8
	bl	sub_020005f0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl	sub_02000602
.L_02000148:
	bl	sub_020005be
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
	bl	sub_02000652
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #30
	bl	sub_0200068e
.L_0200018c:
	ldr	r0, [pc, #192]
	bl	sub_0200066c
	movs	r1, #0
	movs	r0, #8
	bl	sub_0200067c
	bl	sub_020006d8
	movs	r1, #0
	bl	sub_0200061e
	cmp	r0, #0
	bne.n	.L_0200022c
	movs	r0, #15
	bl	sub_02000610
	movs	r1, #0
	movs	r0, #8
	bl	sub_020006a0
	movs	r0, #5
	bl	sub_0200061e
	movs	r2, #0
	movs	r0, #8
	movs	r1, #4
	bl	sub_02000690
	movs	r1, #0
	movs	r0, #8
	bl	sub_020006c0
	movs	r0, #5
	bl	sub_02000636
	ldr	r5, [pc, #116]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl	sub_02000664
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
	bl	sub_02000694
	b.n	.L_02000208
.L_020001fc:
	movs	r1, #236
	movs	r2, #204
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl	sub_020006a2
.L_02000208:
	movs	r0, #4
	movs	r1, #0
	bl	sub_02000702
	movs	r0, #10
	bl	sub_02000678
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r3, r2
	movs	r2, #1
	strh	r2, [r3, #0]
	movs	r0, #1
	bl	sub_02000734
	b.n	.L_02000248
.L_0200022c:
	movs	r0, #20
	bl	sub_02000694
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #8
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl	sub_02000732
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
	bl	sub_020006ca
	movs	r0, #0
	bl	sub_02000788
	movs	r0, #158
	bl	sub_020007ae
	ldrh	r1, [r5, #4]
	ldrh	r2, [r5, #6]
	ldr	r0, [r5, #0]
	bl	sub_020006c8
	ldr	r5, [pc, #96]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl	sub_02000706
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl	sub_02000722
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl	sub_02000762
	ldr	r0, [r5, #0]
	cmp	r6, #0
	bne.n	.L_020002b2
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	bl	sub_02000752
	b.n	.L_020002bc
.L_020002b2:
	movs	r2, #8
	movs	r1, #0
	negs	r2, r2
	bl	sub_02000766
.L_020002bc:
	movs	r0, #10
	bl	sub_02000724
	adds	r0, r7, #0
	bl	sub_020007d2
	bl	sub_020007de
	bl	sub_020007ea
	bl	sub_02000746
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r1, r0, #0
	movs	r2, #0
	ldr	r0, [pc, #8]
	bl	sub_0200053a
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
	bl	sub_02000802
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r0, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #56]
	bl	sub_020007ce
	movs	r0, #48
	adds	r0, #255
	bl	sub_020007e6
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #10
	bl	sub_020007e8
	cmp	r0, #0
	beq.n	.L_020003b4
	movs	r0, #9
	movs	r1, #1
	bl	sub_020008ac
	b.n	.L_020003be
.L_020003b4:
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl	sub_02000878
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
	bl	sub_0200081c
	cmp	r0, #0
	bne.n	.L_02000436
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #57
	movs	r1, #52
	movs	r2, #64
	movs	r3, #15
	bl	sub_0200084a
	movs	r5, #3
	movs	r6, #6
	movs	r0, #60
	movs	r1, #53
	movs	r2, #67
	movs	r3, #16
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_0200085e
	movs	r0, #50
	movs	r1, #52
	movs	r2, #16
	movs	r3, #16
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_0200086e
	movs	r6, #10
	movs	r0, #50
	movs	r1, #61
	movs	r2, #16
	movs	r3, #28
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_02000880
	movs	r0, #61
	movs	r1, #61
	movs	r2, #16
	movs	r3, #63
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_02000890
.L_02000436:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.irp EntryTarget, 0x080000d1, 0x080003c9, 0x080003d9, 0x08020171, 0x08020179, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8139, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85e9, 0x080c85f9, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
