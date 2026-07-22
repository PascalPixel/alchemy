.syntax unified
	.thumb
	.set sub_020002e2, 0x020002e2
	.set sub_02000332, 0x02000332
	.set sub_02000334, 0x02000334
	.set sub_02000342, 0x02000342
	.set sub_02000352, 0x02000352
	.set sub_0200035a, 0x0200035a
	.set sub_0200035e, 0x0200035e
	.set sub_02000372, 0x02000372
	.set sub_0200037a, 0x0200037a
	.set sub_0200037c, 0x0200037c
	.set sub_0200038a, 0x0200038a
	.set sub_0200039a, 0x0200039a
	.set sub_0200039e, 0x0200039e
	.set sub_020003a2, 0x020003a2
	.set sub_020003b4, 0x020003b4
	.set sub_020003ba, 0x020003ba
	.set sub_020003e2, 0x020003e2
	.set sub_02000438, 0x02000438
	.set sub_02000446, 0x02000446
	.set sub_02000452, 0x02000452
	.set sub_02000456, 0x02000456
	.set sub_020004a6, 0x020004a6
	.set sub_020004ac, 0x020004ac
	.set sub_020004ba, 0x020004ba
	.set sub_020004c6, 0x020004c6
	.set sub_020004e2, 0x020004e2
	.set sub_020004f6, 0x020004f6
	.set sub_02000526, 0x02000526
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x0200822d
	.4byte 0x47204c00
	.4byte 0x02008031
	.4byte 0x47204c00
	.4byte 0x0200803d
	.4byte 0x47204c00
	.4byte 0x02008045
	.4byte 0x47204c00
	.4byte 0x02008209
	.4byte 0x47204c00
	.4byte 0x02008039
	.4byte 0x47704800
	.4byte 0x020082f0
	.4byte 0x47702000
	.4byte 0x47704800
	.2byte 0x83c8
	.2byte 0x0200
AlchemyC_02000044:
	.space 0x24
	push	{r5, r6, lr}
	ldr	r5, [pc, #64]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_02000332
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_02000342
	movs	r0, #0
	movs	r1, #0
	bl	sub_02000332
	cmp	r0, #0
	bne.n	.L_02000096
	movs	r0, #10
	bl	sub_02000334
	adds	r0, r5, #1
	bl	sub_02000352
	b.n	.L_0200009c
.L_02000096:
	adds	r0, r5, #2
	bl	sub_0200035a
.L_0200009c:
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_02000372
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0x25b8
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #64]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_0200037a
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_0200038a
	movs	r0, #0
	movs	r1, #0
	bl	sub_0200037a
	cmp	r0, #0
	bne.n	.L_020000de
	movs	r0, #10
	bl	sub_0200037c
	adds	r0, r5, #1
	bl	sub_0200039a
	b.n	.L_020000e4
.L_020000de:
	adds	r0, r5, #2
	bl	sub_020003a2
.L_020000e4:
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_020003ba
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0x25dc
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #4
	movs	r2, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #9
	movs	r2, #1
	movs	r3, #1
	movs	r0, #25
	bl	sub_0200039e
	ldr	r0, [pc, #8]
	bl	sub_020003b4
	add	sp, #8
	pop	{r0}
	bx	r0
	.2byte 0x0201
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #92]
	movs	r2, #182
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r8, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r2, [pc, #80]
	lsls	r3, r1, #2
	mov	sl, r1
	ldrsh	r6, [r2, r3]
	adds	r3, #2
	ldrsh	r5, [r2, r3]
	lsls	r6, r6, #16
	lsls	r5, r5, #16
	movs	r0, #158
	lsrs	r6, r6, #16
	lsrs	r5, r5, #16
	bl	sub_02000438
	adds	r1, r6, #0
	adds	r2, r5, #0
	ldr	r0, [pc, #56]
	bl	sub_020003e2
	movs	r2, #16
	movs	r0, #0
	movs	r1, #0
	negs	r2, r2
	bl	sub_02000446
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r1, #228
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #16
	str	r2, [r3, #0]
	mov	r0, sl
	bl	sub_02000452
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x020085d0
	.2byte 0x85e8
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #92]
	movs	r2, #182
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r8, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r2, [pc, #80]
	lsls	r3, r1, #2
	mov	sl, r1
	ldrsh	r6, [r2, r3]
	adds	r3, #2
	ldrsh	r5, [r2, r3]
	lsls	r6, r6, #16
	lsls	r5, r5, #16
	movs	r0, #158
	lsrs	r6, r6, #16
	lsrs	r5, r5, #16
	bl	sub_020004ac
	adds	r1, r6, #0
	adds	r2, r5, #0
	ldr	r0, [pc, #56]
	bl	sub_02000456
	movs	r2, #16
	movs	r0, #0
	movs	r1, #0
	negs	r2, r2
	bl	sub_020004ba
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r1, #228
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #16
	str	r2, [r3, #0]
	mov	r0, sl
	bl	sub_020004c6
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x020085d0
	.2byte 0x85fe
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #20]
	bl	sub_020004a6
	cmp	r0, #0
	beq.n	.L_02000218
	ldr	r0, [pc, #12]
	b.n	.L_0200021a
.L_02000218:
	ldr	r0, [pc, #12]
.L_0200021a:
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x0000096f
	.4byte 0x02008758
	.2byte 0x8614
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #72]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #90
	bne.n	.L_02000244
	ldr	r0, [pc, #60]
	bl	sub_020004e2
.L_02000244:
	ldr	r3, [pc, #56]
	ldr	r1, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	subs	r3, #192
	str	r3, [r2, #0]
	adds	r3, #200
	adds	r2, r1, r3
	movs	r3, #24
	str	r3, [r2, #0]
	ldr	r0, [pc, #40]
	bl	sub_020004f6
	cmp	r0, #0
	beq.n	.L_02000270
	bl	sub_0200035e
	movs	r0, #16
	movs	r1, #4
	bl	sub_02000526
.L_02000270:
	movs	r0, #0
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000096f
	.4byte 0x03001ebc
	.4byte 0x00000201
	.4byte 0x47204c00
	.4byte 0x08009179
	.4byte 0x47204c00
	.4byte 0x080091c1
	.4byte 0x47204c00
	.4byte 0x080770c1
	.4byte 0x47204c00
	.4byte 0x080770c9
	.4byte 0x47204c00
	.4byte 0x0808a011
	.4byte 0x47204c00
	.4byte 0x0808a071
	.4byte 0x47204c00
	.4byte 0x0808a101
	.4byte 0x47204c00
	.4byte 0x0808a171
	.4byte 0x47204c00
	.4byte 0x0808a179
	.4byte 0x47204c00
	.4byte 0x0808a181
	.4byte 0x47204c00
	.4byte 0x0808a249
	.4byte 0x47204c00
	.4byte 0x0808a579
	.4byte 0x47204c00
	.4byte 0x080f9011
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000100
	.4byte 0x40000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000058
	.4byte 0x40000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000000d8
	.4byte 0x40000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000128
	.4byte 0x40000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000078
	.4byte 0x00000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000028
	.4byte 0x000000b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000a7
	.4byte 0x001010a8
	.4byte 0x002020a8
	.4byte 0x003030a8
	.4byte 0x004040a8
	.4byte 0x005050a8
	.4byte 0x00a1b002
	.4byte 0x00b38002
	.4byte 0x00c39002
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00008000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00008000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00002000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0001c000
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00008000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00014000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00002000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00002000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00070008
	.4byte 0x0005000f
	.4byte 0x00100005
	.4byte 0x000f000d
	.4byte 0x00100012
	.4byte 0x00000019
	.4byte 0x00020001
	.4byte 0x001a0008
	.4byte 0x00010000
	.4byte 0x00010002
	.4byte 0x001affff
	.4byte 0x00020002
	.4byte 0x00080002
	.4byte 0x0002001c
	.4byte 0x00020002
	.4byte 0xffff0001
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008195
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008121
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025b7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008069
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000025bb
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025bc
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000025bd
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025be
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000025bf
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025c0
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025c1
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025c2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025c3
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025c4
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025c5
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025c6
	.4byte 0x00000013
	.4byte 0x0fad0064
	.4byte 0x001000c2
	.4byte 0x00000023
	.4byte 0x0fae0065
	.4byte 0x001000e5
	.4byte 0x00000023
	.4byte 0x0faf0066
	.4byte 0x00200005
	.4byte 0x00000c15
	.4byte 0x02010010
	.4byte 0x020080f9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008195
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008121
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025d7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025d8
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000025d9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000025da
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025db
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020080b1
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025df
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000025e0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025e1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025e2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025e3
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000025e4
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025e5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025e6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025e7
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025e8
	.4byte 0x00000013
	.4byte 0x0fad0064
	.4byte 0x001000c2
	.4byte 0x00000023
	.4byte 0x0fae0065
	.4byte 0x001000e5
	.4byte 0x00000023
	.4byte 0x0faf0066
	.4byte 0x00200005
	.4byte 0x00000c15
	.4byte 0x02010010
	.4byte 0x020080f9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
