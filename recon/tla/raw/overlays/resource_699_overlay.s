.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000500, 0x02000500
	.set sub_020007ca, 0x020007ca
	.set sub_02001066, 0x02001066
	.set sub_02001b84, 0x02001b84
	.set sub_02001c84, 0x02001c84
	.set sub_02001ce0, 0x02001ce0
	.set sub_02001cfe, 0x02001cfe
	.set sub_02001d04, 0x02001d04
	.set sub_02001d2e, 0x02001d2e
	.set sub_02001d8a, 0x02001d8a
	.set sub_02001daa, 0x02001daa
	.set sub_02001dae, 0x02001dae
	.set sub_02001db8, 0x02001db8
	.set sub_02001dca, 0x02001dca
	.set sub_02001dda, 0x02001dda
	.set sub_02001e02, 0x02001e02
	.set sub_02001e06, 0x02001e06
	.set sub_02001e0e, 0x02001e0e
	.set sub_02001e14, 0x02001e14
	.set sub_02001e22, 0x02001e22
	.set sub_02001e2c, 0x02001e2c
	.set sub_02001e88, 0x02001e88
	.set sub_02001e9e, 0x02001e9e
	.set sub_02001eac, 0x02001eac
	.set sub_02001f12, 0x02001f12
	.set sub_02001f20, 0x02001f20
	.set sub_02001f3c, 0x02001f3c
	.set sub_02001f44, 0x02001f44
	.set sub_02001f9a, 0x02001f9a
	.set sub_02001f9c, 0x02001f9c
	.set sub_02001fc4, 0x02001fc4
	.set sub_02001fc6, 0x02001fc6
	.set sub_02001fda, 0x02001fda
	.set sub_02001fe2, 0x02001fe2
	.set sub_02001ff8, 0x02001ff8
	.set sub_02002092, 0x02002092
	.set sub_020020ac, 0x020020ac
	.set sub_020020d2, 0x020020d2
	.set sub_020020d4, 0x020020d4
	.set sub_02002158, 0x02002158
	.set sub_020021a6, 0x020021a6
	.set sub_020021b6, 0x020021b6
	.set sub_020021c0, 0x020021c0
	.set sub_020021f8, 0x020021f8
	.set sub_02002218, 0x02002218
	.set sub_0200223c, 0x0200223c
	.set sub_0200225e, 0x0200225e
	.set sub_02002266, 0x02002266
	.set sub_0200228a, 0x0200228a
	.set sub_020022b2, 0x020022b2
	.set sub_020022e4, 0x020022e4
	.set sub_020022e6, 0x020022e6
	.set sub_020022ec, 0x020022ec
	.set sub_0200231a, 0x0200231a
	.set sub_0200231e, 0x0200231e
	.set sub_0200237a, 0x0200237a
	.set sub_020023a2, 0x020023a2
	.set sub_020023ae, 0x020023ae
	.set sub_020023f8, 0x020023f8
	.set sub_0200241c, 0x0200241c
	.set sub_02002424, 0x02002424
	.set sub_02002432, 0x02002432
	.set sub_02002442, 0x02002442
	.set sub_02002482, 0x02002482
	.set sub_020024b0, 0x020024b0
	.set sub_020024bc, 0x020024bc
	.set sub_0200252c, 0x0200252c
	.set sub_02002540, 0x02002540
	.set sub_02002566, 0x02002566
	.set sub_020025a2, 0x020025a2
	.set sub_020025ce, 0x020025ce
	.set sub_020025de, 0x020025de
	.set sub_0200260e, 0x0200260e
	.set sub_02002624, 0x02002624
	.set sub_02002626, 0x02002626
	.set sub_02002632, 0x02002632
	.set sub_0200263c, 0x0200263c
	.set sub_020026bc, 0x020026bc
	.set sub_020026e6, 0x020026e6
	.set sub_02002768, 0x02002768
	.set sub_020027aa, 0x020027aa
	.set sub_020027b2, 0x020027b2
	.set sub_020027b4, 0x020027b4
	.set sub_020027ba, 0x020027ba
	.set sub_020027e6, 0x020027e6
	.set sub_020027f4, 0x020027f4
	.set sub_020027fe, 0x020027fe
	.set sub_02002802, 0x02002802
	.set sub_0200280c, 0x0200280c
	.set sub_02002810, 0x02002810
	.set sub_0200282e, 0x0200282e
	.set sub_0200283a, 0x0200283a
	.set sub_02002860, 0x02002860
	.set sub_0200286c, 0x0200286c
	.set sub_02002878, 0x02002878
	.set sub_02002884, 0x02002884
	.set sub_020028aa, 0x020028aa
	.set sub_020028b6, 0x020028b6
	.set sub_020029ca, 0x020029ca
	.set sub_020029f0, 0x020029f0
	.set sub_020029f4, 0x020029f4
	.set sub_02002a06, 0x02002a06
	.set sub_02002a18, 0x02002a18
	.set sub_02002a2c, 0x02002a2c
	.set sub_02002a2e, 0x02002a2e
	.set sub_02002a36, 0x02002a36
	.set sub_02002a74, 0x02002a74
	.set sub_02002a86, 0x02002a86
	.set sub_02002a98, 0x02002a98
	.set sub_02002aac, 0x02002aac
	.set sub_02002aae, 0x02002aae
	.set sub_02002ab6, 0x02002ab6
	.set sub_02002af4, 0x02002af4
	.set sub_02002af8, 0x02002af8
	.set sub_02002b06, 0x02002b06
	.set sub_02002b18, 0x02002b18
	.set sub_02002b2c, 0x02002b2c
	.set sub_02002b2e, 0x02002b2e
	.set sub_02002b36, 0x02002b36
	.set sub_02002c5e, 0x02002c5e
	.set sub_02002c72, 0x02002c72
	.set sub_02002cd4, 0x02002cd4
	.set sub_02002cec, 0x02002cec
	.set sub_02002cee, 0x02002cee
	.set sub_02002d10, 0x02002d10
	.set sub_02002d24, 0x02002d24
	.set sub_02002d38, 0x02002d38
	.set sub_02002d46, 0x02002d46
	.set sub_02002d52, 0x02002d52
	.set sub_02002d60, 0x02002d60
	.set sub_02002dec, 0x02002dec
	.set sub_02002e04, 0x02002e04
	.set sub_02002e2e, 0x02002e2e
	.set sub_02002f1e, 0x02002f1e
	.set sub_02002f2a, 0x02002f2a
	.set sub_02002f2e, 0x02002f2e
	.set sub_0200305a, 0x0200305a
	.set sub_02003074, 0x02003074
	.set sub_02003078, 0x02003078
	.set sub_0200309e, 0x0200309e
	.set sub_020030de, 0x020030de
	.set sub_02003106, 0x02003106
	.set sub_02003114, 0x02003114
	.set sub_0200312c, 0x0200312c
	.set sub_0200313a, 0x0200313a
	.set sub_02003144, 0x02003144
	.set sub_02003180, 0x02003180
	.set sub_020031b8, 0x020031b8
	.set sub_0200323a, 0x0200323a
	.set sub_02003278, 0x02003278
	.set sub_02003296, 0x02003296
	.set sub_020032b0, 0x020032b0
	.set sub_020032b2, 0x020032b2
	.set sub_020032e4, 0x020032e4
	.set sub_020032fe, 0x020032fe
	.set sub_02003314, 0x02003314
	.set sub_02003320, 0x02003320
	.set sub_02003332, 0x02003332
	.set sub_0200335a, 0x0200335a
	.set sub_02003372, 0x02003372
	.set sub_02003382, 0x02003382
	.set sub_02003384, 0x02003384
	.set sub_02003390, 0x02003390
	.set sub_020033a8, 0x020033a8
	.set sub_020033d6, 0x020033d6
	.set sub_020033e4, 0x020033e4
	.set sub_02003406, 0x02003406
	.set sub_02003438, 0x02003438
	.set sub_0200345a, 0x0200345a
	.set sub_0200348a, 0x0200348a
	.set sub_02003490, 0x02003490
	.set sub_02003496, 0x02003496
	.set sub_020034a8, 0x020034a8
	.set sub_020034ac, 0x020034ac
	.set sub_020034e0, 0x020034e0
	.set sub_020034ec, 0x020034ec
	.set sub_020034fc, 0x020034fc
	.set sub_02003506, 0x02003506
	.set sub_02003556, 0x02003556
	.set sub_02003564, 0x02003564
	.set sub_0200358a, 0x0200358a
	.set sub_020035ba, 0x020035ba
	.set sub_020035ce, 0x020035ce
	.set sub_020035e2, 0x020035e2
	.set sub_020035f4, 0x020035f4
	.set sub_02003600, 0x02003600
	.set sub_0200360a, 0x0200360a
	.set sub_0200361c, 0x0200361c
	.set sub_02003626, 0x02003626
	.set sub_0200363a, 0x0200363a
	.set sub_02003642, 0x02003642
	.set sub_02003644, 0x02003644
	.set sub_02003656, 0x02003656
	.set sub_02003660, 0x02003660
	.set sub_02003662, 0x02003662
	.set sub_0200367a, 0x0200367a
	.set sub_02003684, 0x02003684
	.set sub_0200369c, 0x0200369c
	.set sub_020036b6, 0x020036b6
	.set sub_020036b8, 0x020036b8
	.set sub_020036c2, 0x020036c2
	.set sub_020036de, 0x020036de
	.set sub_020036ea, 0x020036ea
	.set sub_02003744, 0x02003744
	.set sub_0200375a, 0x0200375a
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008325, 0x02008039, 0x02008045, 0x0200804d, 0x02008131, 0x02008041, 0x020083dd
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa27c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa2ac
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa2d0
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #67
	movs	r2, #24
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #69
	movs	r1, #0
	movs	r2, #4
	movs	r3, #4
	bl	sub_02001d8a
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #89
	movs	r2, #28
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #4
	movs	r3, #3
	bl	sub_02001daa
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #89
	movs	r2, #28
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #64
	movs	r1, #0
	movs	r2, #4
	movs	r3, #3
	bl	sub_02001dca
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #184
	bl	sub_02001c84
	cmp	r0, #0
	bne.n	.L_020000f0
	movs	r1, #212
	movs	r2, #244
	movs	r0, #249
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl	sub_02001e0e
	ldr	r2, [pc, #36]
	movs	r3, #149
	lsls	r3, r3, #2
	adds	r1, r2, r3
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #184
	strh	r3, [r1, #0]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #106
	movs	r1, #3
	bl	sub_02001dda
.L_020000f0:
	add	sp, #12
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #108
	movs	r2, #31
	movs	r1, #1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #69
	movs	r2, #4
	movs	r3, #3
	str	r1, [sp, #8]
	bl	sub_02001e2c
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #4
	bl	sub_02001e02
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #6
	bl	sub_02001e0e
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000148
	ldr	r0, [pc, #12]
	b.n	.L_0200014a
.L_02000148:
	ldr	r0, [pc, #12]
.L_0200014a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000fd
	.4byte 0x0200a36c
	.2byte 0xa2e8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	sub	sp, #12
	adds	r7, r0, #0
	bl	sub_02001ce0
	movs	r3, #255
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_020001a8
	movs	r3, #200
	mov	r6, sp
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [pc, #92]
	str	r3, [r6, #4]
	movs	r3, #164
	lsls	r3, r3, #16
	str	r3, [r6, #8]
	bl	sub_02001cfe
	adds	r5, r0, #0
	bl	sub_02001d04
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #5
	adds	r5, r5, r3
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_02001d2e
	ldr	r3, [r6, #0]
	str	r3, [r7, #8]
	ldr	r3, [r6, #4]
	str	r3, [r7, #12]
	ldr	r3, [r6, #8]
	str	r3, [r7, #16]
.L_020001a8:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	adds	r1, r7, #0
	adds	r1, #102
	strh	r3, [r1, #0]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #212
	bne.n	.L_020001c6
	adds	r0, r7, #0
	movs	r1, #9
	bl	sub_02001dae
	b.n	.L_020001ce
.L_020001c6:
	adds	r0, r7, #0
	movs	r1, #10
	bl	sub_02001db8
.L_020001ce:
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb560
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #179
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	adds	r6, r0, #0
	cmp	r3, #0
	bne.n	.L_0200021a
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200021a
	movs	r1, #175
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200021a
	movs	r1, #180
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02000228
.L_0200021a:
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_02001e14
	bl	sub_02001e88
	b.n	.L_02000290
.L_02000228:
	adds	r0, r6, #0
	movs	r1, #16
	bl	sub_02001e22
	bl	sub_02001e9e
	adds	r0, r6, #0
	adds	r0, #99
	ldrb	r1, [r0, #0]
	adds	r3, r1, #0
	cmp	r3, #0
	beq.n	.L_02000254
	ldr	r3, [pc, #80]
	movs	r2, #7
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000290
	adds	r3, r1, #0
	adds	r3, #255
	strb	r3, [r0, #0]
	b.n	.L_02000290
.L_02000254:
	adds	r5, r6, #0
	adds	r5, #102
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	bne.n	.L_02000266
	adds	r0, r6, #0
	bl	.L_020003c0
.L_02000266:
	ldrh	r3, [r5, #0]
	subs	r3, #1
	strh	r3, [r5, #0]
	ldr	r0, [r6, #16]
	ldr	r3, [r6, #12]
	asrs	r0, r0, #14
	asrs	r3, r3, #15
	adds	r0, r0, r3
	lsls	r0, r0, #8
	bl	sub_02001e06
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r6, #8]
	asrs	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r6, #8]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
.L_02000290:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r6, #0
.L_0200029c:
	movs	r0, #168
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	lsls	r0, r0, #2
	bl	sub_02001eac
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #85
	movs	r7, #0
	strb	r7, [r3, #0]
	movs	r0, #128
	subs	r3, #50
	strb	r7, [r3, #0]
	lsls	r0, r0, #8
	lsls	r3, r6, #12
	adds	r3, r3, r0
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	ldr	r3, [pc, #88]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #99
	lsls	r3, r6, #3
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02001f12
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_02001f9a
	ldr	r1, [r5, #80]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	adds	r0, r5, #0
	strb	r3, [r1, #9]
	movs	r1, #1
	bl	sub_02001f3c
	movs	r2, #1
	adds	r1, r5, #0
	ands	r2, r6
	adds	r1, #100
	adds	r5, #102
	adds	r0, r6, #1
	cmp	r2, #0
	beq.n	.L_02000312
	movs	r3, #212
	strh	r3, [r1, #0]
	strh	r7, [r5, #0]
	b.n	.L_02000318
.L_02000312:
	movs	r3, #116
	strh	r3, [r1, #0]
	strh	r2, [r5, #0]
.L_02000318:
	adds	r6, r0, #0
	cmp	r6, #16
	bne.n	.L_0200029c
	pop	{r5, r6, r7, pc}
	.2byte 0x81d9
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	ldr	r5, [pc, #148]
	str	r2, [r3, #0]
	subs	r2, #36
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #140]
	cmp	r2, r3
	bne.n	.L_0200039a
	movs	r0, #1
	bl	.L_02000828
	movs	r0, #10
	adds	r0, #255
	bl	sub_02001f20
	cmp	r0, #0
	bne.n	.L_02000372
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #4
	bne.n	.L_02000372
	adds	r2, #50
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl	sub_020020ac
.L_02000372:
	movs	r0, #10
	adds	r0, #255
	bl	sub_02001f44
	cmp	r0, #0
	bne.n	.L_0200039a
	ldr	r2, [pc, #76]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #6
	bne.n	.L_0200039a
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl	sub_020020d4
.L_0200039a:
	ldr	r3, [pc, #48]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_020003d8
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #16]
	subs	r2, #2
	strh	r3, [r2, #0]
	bl	.L_02000656
.L_020003c0:
	b.n	.L_020003d8
	.2byte 0x0000
	.4byte 0x00001008
	.4byte 0x00003f10
	.4byte 0x02000240
	.4byte 0x000000fc
	.2byte 0x00fd
	.2byte 0x0000
.L_020003d8:
	movs	r0, #0
	pop	{r5, pc}
	push	{lr}
	movs	r0, #0
	bl	sub_020007ca
	movs	r0, #0
	pop	{pc}
	push	{lr}
	movs	r0, #20
	adds	r0, #255
	bl	sub_02001fc4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl	sub_02001fc6
	cmp	r0, #0
	beq.n	.L_02000410
	movs	r0, #98
	adds	r0, #255
	bl	sub_02001fda
	movs	r0, #162
	lsls	r0, r0, #1
	bl	sub_02001fe2
.L_02000410:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x60184b01
	.4byte 0x00004770
	.2byte 0xa388
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	sub	sp, #32
	ldr	r0, [r3, #0]
	cmp	r0, #0
	bge.n	.L_02000432
	adds	r0, #3
.L_02000432:
	asrs	r0, r0, #2
	movs	r1, #5
	bl	sub_02001f9c
	ldr	r3, [pc, #148]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000486
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrh	r1, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_02000466
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_020004c2
.L_02000466:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_020004c2
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_020004c2
.L_02000486:
	movs	r5, #0
	movs	r6, #4
.L_0200048a:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
	bl	sub_02001ff8
	ldr	r3, [pc, #60]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
	ble.n	.L_0200048a
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r7, #0
	ldr	r1, [pc, #36]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_020004c2:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200a384
	.4byte 0x0200a388
	.4byte 0x0200a3b0
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, r6, lr}
	ldr	r2, [pc, #88]
	movs	r3, #1
	adds	r6, r0, #0
	str	r3, [r2, #0]
	cmp	r6, #2
	beq.n	sub_02000500
	ldr	r1, [pc, #80]
	movs	r2, #32
	ldr	r0, [pc, #80]
	ldr	r5, [pc, #80]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4814
	ldr	r1, [pc, #80]
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	bl	sub_020020d2
	cmp	r0, #0
	bne.n	.L_02000510
	cmp	r6, #1
	bne.n	.L_02000522
.L_02000510:
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl	sub_02002092
	b.n	.L_02000536
.L_02000522:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_02000536:
	pop	{r5, r6, pc}
	.4byte 0x0200a388
	.4byte 0x05000180
	.4byte 0x0200a3b0
	.4byte 0x03000730
	.4byte 0x0200a3d0
	.4byte 0x050001a0
	.4byte 0x0200a384
	.4byte 0x02008421
	.4byte 0x0200a3d4
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, lr}
	bl	sub_020021f8
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #85
	movs	r3, #4
	strb	r3, [r1, #0]
	movs	r2, #0
	ldr	r3, [r5, #20]
	str	r2, [r5, #68]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r5, #12]
	subs	r1, #50
	ldrb	r2, [r1, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #34
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r3, #0]
	bl	sub_020021c0
	adds	r3, r0, #0
	asrs	r3, r3, #19
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	adds	r3, #6
	movs	r2, #0
	bl	sub_02002218
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl	sub_020022e4
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #244]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_02002266
	adds	r6, r0, #0
	ldr	r7, [r6, #104]
	bl	sub_0200225e
	movs	r0, #0
	bl	sub_020022ec
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	movs	r5, #0
	strh	r5, [r3, #0]
	movs	r3, #85
	adds	r3, r3, r6
	mov	r9, r3
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_0200223c
	movs	r3, #99
	adds	r3, r3, r7
	mov	r8, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000654
.L_0200060e:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #176]
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	cmp	r5, r2
	bgt.n	.L_0200062a
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_0200062a:
	ldr	r3, [pc, #156]
	adds	r1, r6, #0
	ldr	r2, [r3, #0]
	ldrb	r3, [r3, #0]
	adds	r1, #35
	lsls	r3, r3, #12
	strh	r3, [r6, #6]
	movs	r3, #1
	ands	r2, r3
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	movs	r0, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	bl	sub_020021b6
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_0200060e
.L_02000654:
	movs	r3, #208
.L_02000656:
	lsls	r3, r3, #4
	adds	r3, #68
	movs	r2, #1
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	strh	r2, [r3, #0]
	ldr	r3, [r7, #8]
	ldrh	r1, [r7, #6]
	subs	r2, #3
	asrs	r3, r3, #19
	ands	r3, r2
	asrs	r1, r1, #13
	adds	r3, r3, r1
	subs	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #8]
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #56]
	asrs	r3, r3, #19
	ands	r3, r2
	movs	r2, #2
	ands	r1, r2
	subs	r3, r3, r1
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #16]
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	adds	r3, r6, #0
	adds	r3, #35
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_020022e6
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	b.n	.L_020006cc
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x0003ffff
	.2byte 0x122c
	.2byte 0x0300
.L_020006cc:
	bl	sub_020023a2
	bl	sub_020023ae
	movs	r3, #128
	adds	r7, r0, #0
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r3, #128
	ldr	r5, [pc, #52]
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r0, #0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl	sub_0200231e
	ldr	r3, [r6, #16]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	adds	r0, r7, #0
	bl	sub_0200231a
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_02000732
	b.n	.L_02000718
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02000718:
	movs	r0, #1
	adds	r5, #1
	bl	sub_0200228a
	cmp	r5, #59
	bgt.n	.L_02000732
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	cmp	r2, r3
	bgt.n	.L_02000718
.L_02000732:
	movs	r0, #127
	bl	sub_02002482
	ldr	r3, [r6, #40]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02000752
.L_02000740:
	movs	r0, #1
	adds	r5, #1
	bl	sub_020022b2
	cmp	r5, #59
	bgt.n	.L_02000752
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02000740
.L_02000752:
	adds	r0, r7, #0
	bl	sub_0200237a
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl	sub_020023f8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl	sub_02002442
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #70
	add	r2, sl
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #170
	lsls	r3, r3, #1
	movs	r6, #0
	add	r3, sl
	strh	r6, [r3, #0]
	bl	sub_0200241c
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [r1, #0]
	ldr	r4, [r0, #0]
	ldr	r2, [r1, #8]
	subs	r4, r4, r3
	ldr	r3, [r0, #8]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r4, #0
	muls	r0, r4
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.2byte 0x0000
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #80]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_02002482
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L_0200082a
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200082a
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200082a
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
.L_02000828:
	beq.n	.L_02000838
.L_0200082a:
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_02002424
	b.n	.L_02000976
	.2byte 0x0240
	.2byte 0x0200
.L_02000838:
	adds	r0, r6, #0
	movs	r1, #16
	bl	sub_02002432
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #31
	ands	r3, r2
	cmp	r3, #31
	bne.n	.L_02000858
	movs	r0, #231
	bl	sub_020025a2
.L_02000858:
	ldr	r3, [r7, #80]
	ldr	r0, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r0, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	movs	r2, #2
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	bl	sub_020025a2
	cmp	r0, #255
	beq.n	.L_0200095a
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_020025a2
	ldr	r5, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	cmp	r5, r3
	bgt.n	.L_0200095a
	ldr	r2, [pc, #172]
	cmp	r5, r2
	blt.n	.L_0200095a
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02000920
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
	subs	r5, r2, r3
	cmp	r5, #0
	bge.n	.L_020008b8
	subs	r5, r3, r2
.L_020008b8:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r0, #8
	adds	r1, #8
	mov	r8, r2
	bl	sub_02001066
	cmp	r0, #12
	bgt.n	.L_020008d8
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r5, r3
	bge.n	.L_020008d8
	movs	r2, #1
	mov	r8, r2
.L_020008d8:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02000920
	movs	r0, #130
	lsls	r0, r0, #1
	bl	sub_020024b0
	cmp	r0, #0
	bne.n	.L_02000920
	ldrh	r3, [r6, #6]
	str	r6, [r7, #104]
	strh	r3, [r7, #6]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #181
	lsls	r2, r2, #1
	strb	r3, [r1, #0]
	add	r2, sl
	movs	r3, #200
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r2, #128
	ldr	r0, [pc, #56]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #99
	strb	r3, [r2, #0]
.L_02000920:
	ldrh	r0, [r6, #6]
	bl	sub_020024b0
	ldr	r1, [r6, #48]
	ldr	r5, [pc, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl	sub_020024bc
	ldr	r1, [r6, #48]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	b.n	.L_02000954
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_02000954:
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_02000976
.L_0200095a:
	adds	r3, r6, #0
	adds	r3, #99
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	str	r5, [r6, #108]
	bl	sub_02002566
	movs	r0, #228
	bl	sub_020026bc
	ldr	r3, [pc, #20]
	str	r5, [r3, #0]
.L_02000976:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200a38c
	.2byte 0xa3ac
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #222
	sub	sp, #68
	bl	sub_020026e6
	ldrh	r0, [r5, #6]
	bl	sub_0200252c
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r6, [pc, #152]
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68ab
	add	r2, sp, #56
	adds	r3, r3, r0
	str	r3, [r2, #0]
	mov	r8, r2
	ldrh	r0, [r5, #6]
	bl	sub_02002540
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x692b
	mov	r2, r8
	adds	r3, r3, r0
	str	r3, [r2, #8]
	movs	r0, #140
	ldr	r1, [r2, #0]
	lsls	r0, r0, #1
	ldr	r2, [r5, #12]
	bl	sub_020025de
	movs	r1, #2
	adds	r7, r0, #0
	bl	sub_020025ce
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_02002626
	adds	r3, r7, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	ldr	r2, [pc, #56]
	ldrh	r3, [r5, #6]
	add	r4, sp, #16
	strh	r3, [r7, #6]
	adds	r3, r7, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	subs	r3, #2
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #44]
	str	r3, [r7, #108]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #1
	str	r3, [r4, #0]
	movs	r3, #7
	str	r3, [r4, #4]
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldr	r2, [r3, #8]
	ldr	r3, [pc, #24]
	ldr	r1, [r5, #12]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	b.n	.L_02000a4c
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0x020087cd
	.2byte 0x0000
	.2byte 0xfffa
.L_02000a4c:
	.2byte 0x2300
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	bl	.L_02001aea
	adds	r0, r7, #0
	add	sp, #68
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	sub	sp, #56
	ldr	r7, [r3, #0]
	movs	r3, #7
	ands	r7, r3
	mov	sl, r0
	cmp	r7, #0
	bne.n	.L_02000af2
	add	r6, sp, #16
	movs	r3, #3
	str	r3, [r6, #0]
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	movs	r3, #14
	str	r3, [r6, #4]
	bl	sub_0200260e
	mov	r2, sl
	lsls	r3, r0, #3
	ldr	r2, [r2, #8]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	mov	r8, r2
	add	r8, r3
	bl	sub_02002624
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	lsrs	r3, r3, #16
	movs	r2, #32
	subs	r2, r2, r3
	mov	r3, sl
	ldr	r5, [r3, #12]
	lsls	r2, r2, #16
	adds	r5, r5, r2
	bl	sub_0200263c
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r2, #160
	lsls	r2, r2, #11
	lsls	r0, r0, #16
	adds	r0, r0, r2
	movs	r1, #10
	bl	sub_02002632
	mov	r3, sl
	ldr	r2, [r3, #16]
	movs	r3, #176
	lsls	r3, r3, #12
	str	r0, [sp, #0]
	str	r3, [sp, #8]
	mov	r0, r8
	adds	r1, r5, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl	sub_02001b84
.L_02000af2:
	movs	r0, #0
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	bl	sub_020027aa
	adds	r6, r0, #0
	movs	r0, #10
	bl	sub_020027b2
	adds	r7, r0, #0
	movs	r0, #23
	bl	sub_020027ba
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #128
	mov	r8, r2
	movs	r2, #248
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl	sub_020027e6
	movs	r1, #128
	movs	r2, #248
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl	sub_020027f4
	movs	r1, #236
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl	sub_02002802
	movs	r1, #138
	movs	r2, #128
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #18
	bl	sub_02002810
	movs	r0, #24
	bl	sub_020027fe
	movs	r3, #85
	movs	r2, #0
	adds	r3, r3, r5
	str	r2, [r0, #24]
	strb	r2, [r3, #0]
	mov	fp, r3
	ldr	r3, [r5, #20]
	movs	r0, #160
	str	r3, [r5, #12]
	movs	r3, #85
	adds	r3, r3, r6
	strb	r2, [r3, #0]
	mov	r9, r3
	ldr	r3, [r6, #20]
	lsls	r0, r0, #4
	str	r3, [r6, #12]
	movs	r3, #85
	adds	r3, r3, r7
	strb	r2, [r3, #0]
	mov	sl, r3
	ldr	r3, [r7, #20]
	adds	r0, #10
	str	r3, [r7, #12]
	bl	sub_02002768
	cmp	r0, #0
	beq.n	.L_02000be0
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #208]
	movs	r0, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r2, [pc, #204]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #200]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl	sub_02002860
	movs	r1, #4
	bl	sub_0200282e
	movs	r0, #11
	bl	sub_0200286c
	movs	r1, #4
	bl	sub_0200283a
.L_02000be0:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl	sub_020027b4
	cmp	r0, #0
	beq.n	.L_02000c2a
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #132]
	movs	r0, #10
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	ldr	r2, [pc, #120]
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl	sub_020028aa
	movs	r1, #4
	bl	sub_02002878
	movs	r0, #12
	bl	sub_020028b6
	movs	r1, #4
	bl	sub_02002884
.L_02000c2a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl	sub_020027fe
	cmp	r0, #0
	beq.n	.L_02000c6a
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl	sub_0200280c
	cmp	r0, #0
	beq.n	.L_02000c6a
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #56]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	mov	r2, sl
	strb	r3, [r2, #0]
	mov	r2, fp
	strb	r3, [r2, #0]
.L_02000c6a:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00066640
	.4byte 0x0001eb80
	.4byte 0xfffd70c0
	.4byte 0x00028f40
	.4byte 0xfffff800
	.4byte 0x00199900
	.2byte 0x8480
	.2byte 0x001b
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [pc, #380]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [pc, #372]
	mov	sl, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	mov	r8, r2
	ldr	r2, [pc, #364]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	sub	sp, #8
	lsrs	r3, r3, #5
	str	r3, [sp, #4]
	ldr	r6, [pc, #356]
	ldr	r3, [r1, #0]
	movs	r1, #0
	ldr	r3, [r3, #4]
	mov	r9, r1
	str	r3, [sp, #0]
	ldr	r3, [pc, #348]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r9, r3
	blt.n	.L_02000ce6
	b.n	.L_02000e1a
.L_02000ce6:
	ldr	r2, [pc, #340]
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r5, [r2, r3]
	cmp	r5, #0
	bne.n	.L_02000cf4
	b.n	.L_02000e0a
.L_02000cf4:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne.n	.L_02000cfc
	b.n	.L_02000e0a
.L_02000cfc:
	mov	r1, sl
	subs	r0, r3, r1
	ldr	r2, [sp, #0]
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r3, r3, r2
	ldr	r2, [r5, #16]
	lsls	r1, r1, #12
	adds	r3, r3, r1
	mov	r1, r8
	subs	r2, r2, r1
	ldr	r1, [sp, #0]
	subs	r2, r2, r1
	subs	r4, r2, r3
	adds	r3, r3, r2
	asrs	r3, r3, #16
	adds	r3, #58
	mov	fp, r3
	ldr	r3, [pc, #284]
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r3, r5, #0
	mov	ip, r2
	asrs	r1, r0, #16
	mov	r0, ip
	adds	r3, #100
	asrs	r2, r4, #16
	cmp	r0, #0
	bne.n	.L_02000d72
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #7
	movs	r1, #167
	adds	r4, r2, #0
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #16
	cmp	r3, r1
	bhi.n	.L_02000e0a
	movs	r2, #16
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_02000e0a
	cmp	r4, #239
	bgt.n	.L_02000e0a
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	mov	r3, ip
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #212]
	b.n	.L_02000dae
.L_02000d72:
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #0
	movs	r1, #175
	adds	r4, r2, #0
	adds	r3, #23
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #64
	cmp	r3, r1
	bhi.n	.L_02000e0a
	movs	r2, #64
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_02000e0a
	cmp	r4, #175
	bgt.n	.L_02000e0a
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #152]
.L_02000dae:
	movs	r2, #128
	orrs	r4, r3
	stmia	r1!, {r4}
	ldr	r0, [sp, #4]
	lsls	r3, r7, #3
	adds	r3, r0, r3
	lsls	r2, r2, #4
	orrs	r3, r2
	str	r3, [r1, #0]
	ldr	r3, [pc, #136]
	movs	r0, #1
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_02000dec
	adds	r0, r5, #0
	bl	sub_02002af8
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r6, #9]
	negs	r1, r1
	adds	r2, r1, #0
	lsls	r0, r0, #2
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r6, #9]
	b.n	.L_02000e00
.L_02000dec:
	movs	r3, #3
	ands	r3, r2
	movs	r0, #13
	ldrb	r2, [r6, #9]
	negs	r0, r0
	adds	r1, r0, #0
	lsls	r3, r3, #2
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_02000e00:
	adds	r0, r6, #0
	mov	r1, fp
	bl	sub_020029ca
	adds	r6, #12
.L_02000e0a:
	ldr	r3, [pc, #44]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r9, r1
	cmp	r9, r3
	bge.n	.L_02000e1a
	b.n	.L_02000ce6
.L_02000e1a:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x0200a3f0
	.4byte 0x020036e0
	.4byte 0x0200a434
	.4byte 0x0200a3f2
	.4byte 0x0200a3f4
	.4byte 0x0200a4f4
	.4byte 0x40002000
	.4byte 0xc000a000
	.2byte 0xa4f6
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl	sub_020029f4
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl	sub_02002a18
	ldr	r5, [pc, #76]
	bl	sub_02002a2e
	movs	r1, #192
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl	sub_02002a36
	adds	r0, r6, #0
	bl	sub_02002a2c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl	sub_02002a06
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200a3f4
	.4byte 0x02009d54
	.4byte 0x0200a3f0
	.4byte 0x02008c95
	.4byte 0x0200a3f2
	.4byte 0x0200a4f4
	.2byte 0xa4f6
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl	sub_02002a74
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl	sub_02002a98
	ldr	r5, [pc, #76]
	bl	sub_02002aae
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl	sub_02002ab6
	adds	r0, r6, #0
	bl	sub_02002aac
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl	sub_02002a86
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200a3f4
	.4byte 0x02009eb7
	.4byte 0x0200a3f0
	.4byte 0x02008c95
	.4byte 0x0200a3f2
	.4byte 0x0200a4f4
	.2byte 0xa4f6
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl	sub_02002af4
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #80]
	bl	sub_02002b18
	ldr	r5, [pc, #80]
	bl	sub_02002b2e
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl	sub_02002b36
	adds	r0, r6, #0
	bl	sub_02002b2c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl	sub_02002b06
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_02000fd4
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200a3f4
	.4byte 0x0200a0e6
	.4byte 0x0200a3f0
	.4byte 0x02008c95
	.4byte 0x0200a3f2
	.4byte 0x0200a4f4
	.2byte 0xa4f6
	.2byte 0x0200
.L_02000fd4:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_02002c72
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_02000ffe
	adds	r3, r4, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	ldr	r1, [pc, #16]
	ldr	r0, [pc, #20]
	ldrh	r2, [r1, #0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	adds	r2, #1
	lsls	r3, r3, #2
	str	r4, [r0, r3]
	strh	r2, [r1, #0]
.L_02000ffe:
	pop	{r5, pc}
	.4byte 0x0200a3f2
	.4byte 0x0200a3f4
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xa4f6
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02001058
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001058
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_02001058:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl	sub_02002d52
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_020010dc
	cmp	r7, #0
	beq.n	.L_020010dc
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_020010e4
.L_020010dc:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_020010e4:
	mov	r3, sl
	bl	sub_02002cec
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020010f2
	b.n	.L_0200123e
.L_020010f2:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl	sub_02002cee
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl	sub_02002d10
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_02002d60
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl	sub_02002158
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200123e
	cmp	r7, #0
	beq.n	.L_0200123e
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001174
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl	sub_02002e2e
.L_02001174:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001194
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl	sub_020021a6
.L_02001194:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020011a8
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020011a8:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020011ee
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020011d6
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl	sub_02002d24
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020011e8
.L_020011d6:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl	sub_02002d38
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020011e8:
	bl	sub_02002d46
	str	r0, [r6, #52]
.L_020011ee:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200120a
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_02002dec
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl	sub_02002e04
.L_0200120a:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200121c
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_0200121c:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200122e
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200122e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200123e
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_0200123e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a3a0
	.4byte 0x0200905d
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r4, [pc, #268]
	movs	r1, #1
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	negs	r1, r1
	sub	sp, #4
	cmp	r3, r1
	beq.n	.L_02001368
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, #32
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl	sub_02002f1e
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_020012a8
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_020012b0
.L_020012a8:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_020012b0:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02001368
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02001368
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	str	r4, [sp, #0]
	adds	r3, r2, #0
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r5, [r3, #4]
	ldr	r3, [r2, #0]
	ands	r0, r1
	ands	r5, r1
	ldr	r6, [r3, #4]
	movs	r1, #16
	ldrsh	r3, [r4, r1]
	ldr	r2, [pc, #156]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r3
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	lsls	r1, r1, #20
	subs	r7, r1, r0
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	lsls	r2, r2, #20
	bl	sub_02002f2e
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	subs	r0, r0, r6
	lsls	r3, r3, #20
	subs	r3, r3, r5
	subs	r3, r3, r6
	subs	r2, r3, r0
	asrs	r7, r7, #16
	adds	r0, r0, r3
	asrs	r0, r0, #16
	adds	r3, r7, #0
	movs	r5, #167
	asrs	r2, r2, #16
	adds	r1, r0, #0
	adds	r3, #15
	lsls	r5, r5, #1
	adds	r2, #14
	adds	r1, #58
	ldr	r4, [sp, #0]
	cmp	r3, r5
	bhi.n	.L_02001368
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02001368
	cmp	r2, #239
	bgt.n	.L_02001368
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r4, #20]
	lsls	r3, r7, #16
	orrs	r2, r3
	ldr	r3, [pc, #48]
	adds	r0, r4, #0
	orrs	r2, r3
	movs	r3, #128
	str	r2, [r4, #24]
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r4, #28]
	adds	r0, #20
	bl	sub_02002f2a
.L_02001368:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200a4f8
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x020036e0
	.2byte 0x8800
	.2byte 0x8000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r0, [sp, #44]
	ldr	r0, [pc, #540]
	str	r1, [sp, #40]
	mov	r8, r0
	movs	r1, #32
	add	r1, r8
	mov	r9, r1
	mov	ip, r9
	adds	r5, r2, #0
	mov	r2, ip
	adds	r6, r3, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #520]
	movs	r1, #4
	ldr	r7, [sp, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xa80b
	ldrh	r0, [r0, #0]
	mov	r1, r8
	strh	r0, [r1, #4]
	add	r1, sp, #40
	ldrh	r1, [r1, #0]
	mov	r3, r8
	strh	r1, [r3, #0]
	strh	r5, [r3, #2]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r5, r8
	mov	r0, r8
	adds	r3, #255
	mov	r1, r8
	strh	r6, [r5, #6]
	movs	r2, #0
	strh	r7, [r0, #8]
	strh	r3, [r1, #12]
	mov	r3, r8
	strh	r2, [r3, #10]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	mov	ip, r3
	lsls	r2, r2, #1
	mov	r1, ip
	add	r2, ip
	adds	r1, #236
	ldr	r0, [r1, #0]
	ldr	r3, [r2, #8]
	ldr	r5, [r2, #48]
	adds	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	adds	r1, #4
	ldr	r3, [r2, #12]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r2
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	adds	r2, r2, r0
	lsls	r2, r2, #2
	asrs	r3, r3, #20
	adds	r5, r5, r2
	movs	r0, #0
	str	r3, [sp, #20]
	str	r5, [sp, #36]
	str	r0, [sp, #12]
	cmp	r0, r3
	bge.n	.L_0200150c
.L_0200143c:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_02001500
.L_02001450:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_020014f0
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_020014f0
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_020014f0
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #32]
	mov	r0, r9
	adds	r7, r2, r3
	strh	r7, [r0, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #28]
	add	r0, sp, #40
	ldrh	r0, [r0, #0]
	adds	r6, r1, r2
	mov	r3, r9
	mov	r1, r9
	strh	r6, [r3, #2]
	strh	r0, [r1, #4]
	ldr	r1, [sp, #40]
	movs	r0, #10
	adds	r1, #1
	adds	r0, #255
	str	r1, [sp, #40]
	bl	sub_0200305a
	cmp	r0, #0
	bne.n	.L_020014a4
	cmp	r5, sl
	bne.n	.L_020014e2
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl	sub_02003074
	b.n	.L_020014e2
.L_020014a4:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl	sub_02003078
	cmp	r0, #0
	beq.n	.L_020014e2
	mov	r2, r8
	ldrh	r4, [r2, #6]
	ldrh	r5, [r2, #8]
	movs	r3, #8
	ldrsh	r1, [r2, r3]
	movs	r3, #6
	ldrsh	r0, [r2, r3]
	movs	r2, #64
	adds	r3, r2, #0
	ands	r3, r4
	ands	r2, r5
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	asrs	r3, r3, #16
	asrs	r2, r2, #16
	orrs	r7, r3
	orrs	r6, r2
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	bl	sub_02003114
.L_020014e2:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_020014f0:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02001450
.L_02001500:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_0200143c
.L_0200150c:
	movs	r0, #10
	adds	r0, #255
	bl	sub_020030de
	cmp	r0, #0
	beq.n	.L_02001564
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_020031b8
	ldr	r3, [r0, #8]
	movs	r2, #0
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	mov	r0, r8
	asrs	r1, r3, #20
	ldr	r3, [sp, #8]
	mov	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	cmp	r2, r3
	bge.n	.L_02001564
.L_0200153e:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02001554
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02001554
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02001554:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_0200153e
.L_02001564:
	movs	r0, #128
	lsls	r0, r0, #1
	bl	sub_02003106
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_02001572:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02001572
	bl	sub_0200313a
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl	sub_02003144
	adds	r0, r5, #0
	bl	sub_0200313a
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl	sub_02003114
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200a4f8
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0x925d
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #116]
	movs	r2, #133
	mov	r8, r1
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl	sub_02003278
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl	sub_02003296
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl	sub_020032b2
	movs	r0, #1
	bl	sub_02003180
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #20
	lsls	r6, r6, #20
	adds	r5, r5, r3
	mov	r1, sl
	adds	r6, r6, r3
	ldr	r2, [r1, #12]
	adds	r3, r6, #0
	adds	r1, r5, #0
	mov	r0, sl
	bl	sub_0200323a
	movs	r0, #4
	bl	sub_020032b0
	bl	sub_02003314
	ldr	r2, [pc, #20]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #100]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl	sub_020032fe
	mov	r2, r8
	ldrh	r1, [r2, #6]
	movs	r2, #64
	ldr	r6, [r0, #8]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	asrs	r6, r6, #20
	orrs	r6, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r0, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl	sub_02002c5e
	movs	r0, #161
	bl	sub_020033e4
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_020032e4
	movs	r0, #12
	bl	sub_02003332
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r1, r1, r2
	mov	r8, r0
	ldr	r0, [r1, #0]
	sub	sp, #8
	mov	sl, r1
	bl	sub_02003372
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #64
	asrs	r7, r3, #20
	mov	r3, r8
	ldrh	r1, [r3, #6]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	orrs	r7, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r6, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl	sub_02002cd4
	movs	r0, #229
	bl	sub_0200345a
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #2
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_0200335a
	movs	r0, #12
	bl	sub_020033a8
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl	sub_020033d6
	movs	r1, #0
	bl	sub_02003384
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #16]
	movs	r7, #0
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r2, sl
	b.n	.L_02001778
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_02001778:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl	sub_02003438
	movs	r0, #16
	bl	sub_02003406
.L_0200178c:
	cmp	r7, #5
	bne.n	.L_02001796
	movs	r0, #204
	bl	sub_020034e0
.L_02001796:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r7, #1
	bl	sub_02003320
	cmp	r7, #39
	ble.n	.L_0200178c
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_0200345a
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #14]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl	sub_020034e0
	bl	sub_020034ec
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl	sub_02003382
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02001834
	adds	r3, #15
.L_02001834:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl	sub_02003506
	adds	r7, r0, #0
	bl	sub_020034fc
	movs	r0, #0
	bl	sub_0200358a
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl	sub_02003564
	bl	sub_020034a8
	movs	r0, #1
	bl	sub_02003406
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl	sub_020035ba
	bl	sub_020035ce
	movs	r0, #204
	bl	sub_0200361c
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl	sub_02003556
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #256]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_020018f6:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl	sub_0200348a
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl	sub_02003490
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl	sub_02003496
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl	sub_020034ac
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #176]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #156]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl	sub_020029f0
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020018f6
	movs	r0, #188
	bl	sub_020036b8
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl	sub_02003642
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl	sub_0200363a
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl	sub_020035e2
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl	sub_020035f4
	bl	sub_02003600
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl	sub_0200367a
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl	sub_02003644
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	sub_02003684
	bl	sub_02003660
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02009805
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #156]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	sub_0200369c
	ldr	r3, [r0, #8]
	ldr	r6, [pc, #144]
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r0, #16]
	adds	r5, r6, #0
	asrs	r3, r3, #20
	mov	sl, r3
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	movs	r7, #0
	adds	r5, #32
	ldrh	r2, [r6, #10]
	cmp	r7, r3
	bge.n	.L_02001a90
.L_02001a28:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_02001a84
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_02001a84
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl	sub_0200360a
	cmp	r0, #0
	bne.n	.L_02001a58
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0200309e
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl	sub_02003626
	strh	r7, [r6, #12]
	b.n	.L_02001a90
.L_02001a58:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_02001a90
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl	sub_0200312c
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl	sub_02003656
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl	sub_02003662
	movs	r0, #1
	b.n	.L_02001a92
.L_02001a84:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_02001a28
.L_02001a90:
	movs	r0, #0
.L_02001a92:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xa4f8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	ldr	r3, [pc, #156]
	str	r2, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r1
	bl	sub_0200375a
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl	sub_020036b6
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl	sub_020036c2
	mov	r8, r0
.L_02001aea:
	cmp	r7, #0
	bne.n	.L_02001af2
	cmp	r0, #0
	beq.n	.L_02001b46
.L_02001af2:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl	sub_020036de
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl	sub_020036ea
	mov	r3, r9
	adds	r2, r7, r3
	mov	r3, r8
	movs	r1, #128
	add	r3, fp
	lsls	r1, r1, #12
	lsls	r3, r3, #20
	adds	r3, r3, r1
	str	r3, [r6, #16]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r2, r2, #20
	adds	r2, r2, r1
	ldr	r1, [r3, #0]
	str	r2, [r6, #8]
	str	r2, [r1, #8]
	ldr	r3, [r6, #16]
	str	r3, [r1, #16]
	bl	sub_02003744
	bl	sub_02003390
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_02001b46:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a4f8
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000f9, 0x08000119, 0x08000121, 0x08000129, 0x08000169, 0x08000179, 0x080001a9, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c9, 0x080003d1, 0x080003e9, 0x080003f1, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x080201c1, 0x080201e1, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x08020279, 0x080202d9, 0x080202e1, 0x08020361, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80c9, 0x080c80f9, 0x080c8119, 0x080c8171, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8259, 0x080c8279, 0x080c8281, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85c1, 0x080c8761, 0x080c87e9, 0x080c8831, 0x080c8849, 0x080c8881, 0x080c8949, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x06345d01
	.4byte 0x08003b01
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte 0x08071768
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte 0x020010df
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte 0x02007800
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte 0x0800200e
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte 0x08076918
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.4byte 0x00000002
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte 0x08e7ee5a
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte 0x087d19d7
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02d869f
	.4byte 0x3002fa45
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.4byte 0x01000000
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf0101c7d
	.4byte 0xe7c5f937
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000fc
	.4byte 0x001010fd
	.4byte 0x002030fc
	.4byte 0x003020fc
	.4byte 0x004040fb
	.4byte 0x006060fe
	.4byte 0x000000fd
	.4byte 0x001010fc
	.4byte 0x000001ff
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
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte 0x02008055
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte 0x02008075
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte 0x020080f9
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte 0x02008119
	.4byte 0x50008905
	.4byte 0xffff0022
	.4byte 0x02008125
	.4byte 0x50008905
	.4byte 0xffff0026
	.4byte 0x02008095
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x0200a1c8
	.4byte 0x0200a204
	.4byte 0x0200a240
