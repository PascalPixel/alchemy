.syntax unified
	.thumb
	.set sub_0200033e, 0x0200033e
	.set sub_02000368, 0x02000368
	.set sub_02000378, 0x02000378
	.set sub_0200038a, 0x0200038a
	.set sub_020003a6, 0x020003a6
	.set sub_020003ba, 0x020003ba
	.set sub_020003c0, 0x020003c0
	.set sub_020003c2, 0x020003c2
	.set sub_020003ca, 0x020003ca
	.set sub_020003d2, 0x020003d2
	.set sub_020003ee, 0x020003ee
	.set sub_02000402, 0x02000402
	.set sub_02000408, 0x02000408
	.set sub_0200040a, 0x0200040a
	.set sub_02000412, 0x02000412
	.set sub_0200041a, 0x0200041a
	.set sub_02000436, 0x02000436
	.set sub_0200044a, 0x0200044a
	.set sub_02000450, 0x02000450
	.set sub_02000452, 0x02000452
	.set sub_0200045a, 0x0200045a
	.set sub_02000462, 0x02000462
	.set sub_0200047e, 0x0200047e
	.set sub_02000492, 0x02000492
	.set sub_020004a2, 0x020004a2
	.set sub_020004ae, 0x020004ae
	.set sub_020004ce, 0x020004ce
	.set sub_020004d0, 0x020004d0
	.set sub_020004e0, 0x020004e0
	.set sub_020004f0, 0x020004f0
	.set sub_02000514, 0x02000514
	.set sub_02000524, 0x02000524
	.set sub_02000534, 0x02000534
	.set sub_02000576, 0x02000576
	.set sub_02000580, 0x02000580
	.set sub_0200058a, 0x0200058a
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x020081c5
	.4byte 0x47204c00
	.4byte 0x0200805d
	.4byte 0x47204c00
	.4byte 0x02008069
	.4byte 0x47204c00
	.4byte 0x02008071
	.4byte 0x47204c00
	.4byte 0x020081bd
	.4byte 0x47204c00
	.2byte 0x8065
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #0
	movs	r5, #2
	movs	r4, #1
	movs	r1, #105
	movs	r2, #110
.L_0200003c:
	strb	r5, [r0, #22]
	str	r4, [r0, #4]
	strh	r1, [r0, #0]
	cmp	r3, #4
	beq.n	.L_0200004a
	cmp	r3, #7
	bne.n	.L_0200004c
.L_0200004a:
	strh	r2, [r0, #0]
.L_0200004c:
	adds	r3, #1
	adds	r0, #24
	cmp	r3, #14
	bls.n	.L_0200003c
	pop	{r5}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x47704800
	.4byte 0x02008318
	.4byte 0x47702000
	.4byte 0x47704800
	.2byte 0x8498
	.2byte 0x0200
	push	{r5, lr}
	ldr	r0, [pc, #32]
	bl	sub_0200033e
	cmp	r0, #0
	bne.n	.L_02000082
	ldr	r0, [pc, #24]
	bl	.L_020000b0
.L_02000082:
	ldr	r5, [pc, #20]
	adds	r0, r5, #0
	bl	sub_02000368
	adds	r0, r5, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x00000845
	.2byte 0x84d8
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #0
	bl	sub_0200038a
	ldrh	r5, [r0, #6]
	bl	sub_02000378
	ldr	r3, [pc, #44]
	adds	r5, r5, r3
	ldr	r3, [pc, #44]
.L_020000b0:
	cmp	r5, r3
	bhi.n	.L_020000be
	movs	r0, #13
	movs	r1, #16
	bl	sub_020003c2
	b.n	.L_020000cc
.L_020000be:
	ldr	r0, [pc, #32]
	bl	sub_020003ba
	movs	r0, #16
	movs	r1, #0
	bl	sub_020003ca
.L_020000cc:
	bl	sub_020003a6
	pop	{r5}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.2byte 0x16ad
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #0
	bl	sub_020003d2
	ldrh	r5, [r0, #6]
	bl	sub_020003c0
	ldr	r3, [pc, #44]
	adds	r5, r5, r3
	ldr	r3, [pc, #44]
	cmp	r5, r3
	bhi.n	.L_02000106
	movs	r0, #14
	movs	r1, #17
	bl	sub_0200040a
	b.n	.L_02000114
.L_02000106:
	ldr	r0, [pc, #32]
	bl	sub_02000402
	movs	r0, #17
	movs	r1, #0
	bl	sub_02000412
.L_02000114:
	bl	sub_020003ee
	pop	{r5}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.2byte 0x16af
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #0
	bl	sub_0200041a
	ldrh	r5, [r0, #6]
	bl	sub_02000408
	ldr	r3, [pc, #44]
	adds	r5, r5, r3
	ldr	r3, [pc, #44]
	cmp	r5, r3
	bhi.n	.L_0200014e
	movs	r0, #15
	movs	r1, #18
	bl	sub_02000452
	b.n	.L_0200015c
.L_0200014e:
	ldr	r0, [pc, #32]
	bl	sub_0200044a
	movs	r0, #18
	movs	r1, #0
	bl	sub_0200045a
.L_0200015c:
	bl	sub_02000436
	pop	{r5}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.2byte 0x16b1
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #0
	bl	sub_02000462
	ldrh	r5, [r0, #6]
	bl	sub_02000450
	ldr	r3, [pc, #44]
	adds	r5, r5, r3
	ldr	r3, [pc, #44]
	cmp	r5, r3
	bhi.n	.L_02000196
	movs	r0, #3
	movs	r1, #19
	bl	sub_020004a2
	b.n	.L_020001a4
.L_02000196:
	ldr	r0, [pc, #32]
	bl	sub_02000492
	movs	r0, #19
	movs	r1, #0
	bl	sub_020004a2
.L_020001a4:
	bl	sub_0200047e
	pop	{r5}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x000016b7
	.4byte 0x47704800
	.2byte 0x8658
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #224]
	movs	r1, #224
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	ldr	r2, [pc, #220]
	adds	r3, r3, r1
	str	r2, [r3, #0]
	ldr	r0, [pc, #216]
	sub	sp, #8
	bl	sub_020004a2
	cmp	r0, #0
	bne.n	.L_020001f4
	movs	r5, #8
.L_020001e2:
	adds	r0, r5, #0
	bl	sub_020004ce
	adds	r5, #1
	movs	r1, #0
	bl	sub_020004ae
	cmp	r5, #22
	bls.n	.L_020001e2
.L_020001f4:
	ldr	r3, [pc, #188]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #7
	bne.n	.L_0200023c
	movs	r5, #13
	movs	r6, #8
	movs	r0, #34
	movs	r1, #34
	movs	r2, #18
	movs	r3, #16
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl	sub_020004d0
	movs	r0, #34
	movs	r1, #94
	movs	r2, #18
	movs	r3, #76
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl	sub_020004e0
	movs	r0, #94
	movs	r1, #34
	movs	r2, #78
	movs	r3, #16
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl	sub_020004f0
	b.n	.L_0200029c
.L_0200023c:
	adds	r3, r2, #0
	subs	r3, #8
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_0200029c
	movs	r5, #11
	movs	r6, #8
	movs	r0, #34
	movs	r1, #43
	movs	r2, #19
	movs	r3, #23
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl	sub_02000514
	movs	r0, #34
	movs	r1, #94
	movs	r2, #19
	movs	r3, #83
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl	sub_02000524
	movs	r3, #23
	movs	r0, #94
	movs	r1, #34
	movs	r2, #79
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl	sub_02000534
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl	sub_02000576
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl	sub_02000580
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl	sub_0200058a
.L_0200029c:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x00000845
	.4byte 0x02000240
	.4byte 0x47204c00
	.4byte 0x08009181
	.4byte 0x47204c00
	.4byte 0x080091e1
	.4byte 0x47204c00
	.4byte 0x080770c1
	.4byte 0x47204c00
	.4byte 0x0808a019
	.4byte 0x47204c00
	.4byte 0x0808a021
	.4byte 0x47204c00
	.4byte 0x0808a039
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a0f1
	.4byte 0x47204c00
	.4byte 0x0808a171
	.4byte 0x47204c00
	.4byte 0x0808a181
	.4byte 0x47204c00
	.4byte 0x080b0009
	.4byte 0x47204c00
	.4byte 0x080b0019
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000080
	.4byte 0x000000c0
	.4byte 0x00100000
	.4byte 0x01000038
	.4byte 0x000000d8
	.4byte 0xffff0002
	.4byte 0x000000c0
	.4byte 0x600000a8
	.4byte 0x00100000
	.4byte 0x01000038
	.4byte 0x000000d8
	.4byte 0xffff0003
	.4byte 0x00000188
	.4byte 0xc00000c8
	.4byte 0x01200000
	.4byte 0x02000040
	.4byte 0x000000e8
	.4byte 0xffff0004
	.4byte 0x00000288
	.4byte 0xc00000c8
	.4byte 0x02100000
	.4byte 0x03000040
	.4byte 0x000000e8
	.4byte 0xffff0005
	.4byte 0x000002c0
	.4byte 0x600000a8
	.4byte 0x02100000
	.4byte 0x03000040
	.4byte 0x000000e0
	.4byte 0xffff0006
	.4byte 0x00000080
	.4byte 0x000001a0
	.4byte 0x00100000
	.4byte 0x01000118
	.4byte 0x000001b8
	.4byte 0xffff0007
	.4byte 0x00000188
	.4byte 0xc00001d8
	.4byte 0x01200000
	.4byte 0x02000108
	.4byte 0x000001f8
	.4byte 0xffff0008
	.4byte 0x00000188
	.4byte 0x40000148
	.4byte 0x01200000
	.4byte 0x02000108
	.4byte 0x000001f0
	.4byte 0xffff0009
	.4byte 0x00000188
	.4byte 0xc0000158
	.4byte 0x01200000
	.4byte 0x02000108
	.4byte 0x000001f0
	.4byte 0xffff000a
	.4byte 0x00000288
	.4byte 0xc00001a8
	.4byte 0x02100000
	.4byte 0x03000120
	.4byte 0x000001c8
	.4byte 0xffff000b
	.4byte 0x000002c0
	.4byte 0x60000188
	.4byte 0x02100000
	.4byte 0x03000120
	.4byte 0x000001c0
	.4byte 0xffff000c
	.4byte 0x00000080
	.4byte 0x000002b0
	.4byte 0x00100000
	.4byte 0x01000228
	.4byte 0x000002c8
	.4byte 0xffff000d
	.4byte 0x000000c0
	.4byte 0x60000298
	.4byte 0x00100000
	.4byte 0x01000228
	.4byte 0x000002c8
	.4byte 0xffff000e
	.4byte 0x00000190
	.4byte 0xc00002b0
	.4byte 0x01200000
	.4byte 0x02100228
	.4byte 0x000002c8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000025
	.4byte 0x0010b025
	.4byte 0x0020e025
	.4byte 0x00304024
	.4byte 0x00403024
	.4byte 0x0050c025
	.4byte 0x0060d025
	.4byte 0x00705024
	.4byte 0x00807024
	.4byte 0x00906024
	.4byte 0x00a01025
	.4byte 0x00b05025
	.4byte 0x00c06025
	.4byte 0x00d02025
	.4byte 0x00e01026
	.4byte 0x000001ff
	.4byte 0x00000067
	.4byte 0x00000002
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00030000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00008000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x0000006b
	.4byte 0x00000003
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00004000
	.4byte 0x00000074
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00004000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00008000
	.4byte 0x0000006b
	.4byte 0x00000002
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00008000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x02700000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00014000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00014000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x02700000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00000074
	.4byte 0x00000002
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x00004000
	.4byte 0x000000ab
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00004000
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
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x18450008
	.4byte 0x0000169d
	.4byte 0x00000000
	.4byte 0x18450009
	.4byte 0x0000169e
	.4byte 0x00000000
	.4byte 0x1845000a
	.4byte 0x000016a1
	.4byte 0x00000000
	.4byte 0x1845000b
	.4byte 0x000016a2
	.4byte 0x00000000
	.4byte 0x1845000c
	.4byte 0x000016a3
	.4byte 0x00000000
	.4byte 0x1845000d
	.4byte 0x000016a7
	.4byte 0x00000000
	.4byte 0x1845000e
	.4byte 0x000016a8
	.4byte 0x00000000
	.4byte 0x1845000f
	.4byte 0x000016a9
	.4byte 0x00000000
	.4byte 0x18450010
	.4byte 0x0200809d
	.4byte 0x00000000
	.4byte 0x18450011
	.4byte 0x020080e5
	.4byte 0x00000000
	.4byte 0x18450012
	.4byte 0x0200812d
	.4byte 0x00000000
	.4byte 0x18450013
	.4byte 0x02008175
	.4byte 0x00000000
	.4byte 0x18450014
	.4byte 0x000016b8
	.4byte 0x00000000
	.4byte 0x18450015
	.4byte 0x000016b9
	.4byte 0x00000000
	.4byte 0x18450016
	.4byte 0x000016ba
	.4byte 0x00008d15
	.4byte 0x18450008
	.4byte 0x0000169f
	.4byte 0x00008d15
	.4byte 0x18450009
	.4byte 0x000016a0
	.4byte 0x00008d15
	.4byte 0x1845000a
	.4byte 0x000016a4
	.4byte 0x00008d15
	.4byte 0x1845000b
	.4byte 0x000016a5
	.4byte 0x00008d15
	.4byte 0x1845000c
	.4byte 0x000016a6
	.4byte 0x00008d15
	.4byte 0x1845000d
	.4byte 0x000016aa
	.4byte 0x00008d15
	.4byte 0x1845000e
	.4byte 0x000016ab
	.4byte 0x00008d15
	.4byte 0x1845000f
	.4byte 0x000016ac
	.4byte 0x00008d15
	.4byte 0x18450010
	.4byte 0x000016ae
	.4byte 0x00008d15
	.4byte 0x18450011
	.4byte 0x000016b0
	.4byte 0x00008d15
	.4byte 0x18450012
	.4byte 0x000016b2
	.4byte 0x00008d15
	.4byte 0x18450013
	.4byte 0x000016bb
	.4byte 0x00008d15
	.4byte 0x18450014
	.4byte 0x000016bc
	.4byte 0x00008d15
	.4byte 0x18450015
	.4byte 0x000016bd
	.4byte 0x00008d15
	.4byte 0x18450016
	.4byte 0x000016be
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000167d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000167e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000167f
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001680
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001681
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001682
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001683
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001684
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001685
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001686
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001687
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001689
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000168a
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0000168b
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000168c
	.4byte 0x00000023
	.4byte 0x0f590064
	.4byte 0x001000e5
	.4byte 0x00000033
	.4byte 0x0f5a0065
	.4byte 0x00200007
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x004029d4
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029d5
	.4byte 0x000000f3
	.4byte 0xffff00ca
	.4byte 0x004029d6
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
