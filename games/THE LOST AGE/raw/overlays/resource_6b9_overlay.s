.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_020002ce, 0x020002ce
	.set sub_0200061c, 0x0200061c
	.set sub_02000626, 0x02000626
	.set sub_0200063a, 0x0200063a
	.set sub_02000648, 0x02000648
	.set sub_02000650, 0x02000650
	.set sub_02000654, 0x02000654
	.set sub_0200065a, 0x0200065a
	.set sub_0200065c, 0x0200065c
	.set sub_0200069c, 0x0200069c
	.set sub_020006ac, 0x020006ac
	.set sub_0200071a, 0x0200071a
	.set sub_0200071c, 0x0200071c
	.set sub_0200072c, 0x0200072c
	.set sub_0200074a, 0x0200074a
	.set sub_0200077e, 0x0200077e
	.set sub_02000796, 0x02000796
	.set sub_020007a0, 0x020007a0
	.set sub_020007b2, 0x020007b2
	.set sub_020007c0, 0x020007c0
	.set sub_02000800, 0x02000800
	.set sub_0200081e, 0x0200081e
	.set sub_02000838, 0x02000838
	.set sub_0200083a, 0x0200083a
	.set sub_0200083e, 0x0200083e
	.set sub_02000874, 0x02000874
	.set sub_0200089e, 0x0200089e
	.set sub_020008d4, 0x020008d4
	.set sub_020008de, 0x020008de
	.set sub_020008f4, 0x020008f4
	.set sub_02000914, 0x02000914
	.set sub_02000940, 0x02000940
	.set sub_0200097c, 0x0200097c
	.set sub_02000984, 0x02000984
	.set sub_02000998, 0x02000998
	.set sub_020009ec, 0x020009ec
	.set sub_020009f8, 0x020009f8
	.set sub_02000a08, 0x02000a08
	.set sub_02000a20, 0x02000a20
	.set sub_02000a46, 0x02000a46
	.set sub_02000a4c, 0x02000a4c
	.set sub_02000a72, 0x02000a72
	.set sub_02000ac6, 0x02000ac6
	.set sub_02000af0, 0x02000af0
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008105, 0x02008039, 0x02008045, 0x0200804d, 0x020080fd, 0x02008041, 0x02008159
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8608
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8668
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8674
	.2byte 0x0200
	push	{lr}
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl	sub_02000626
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	cmp	r0, #1
	bne.n	.L_02000074
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl	sub_0200063a
.L_02000074:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	movs	r0, #9
	movs	r1, #3
	bl	sub_02000654
	pop	{pc}
	push	{r5, lr}
	sub	sp, #8
	cmp	r0, #1
	bne.n	.L_020000b4
	movs	r5, #34
	str	r0, [sp, #4]
	movs	r1, #1
	movs	r0, #49
	movs	r2, #1
	movs	r3, #2
	str	r5, [sp, #0]
	bl	sub_02000650
	movs	r3, #3
	str	r3, [sp, #4]
	movs	r0, #49
	movs	r1, #1
	movs	r2, #1
	movs	r3, #2
	str	r5, [sp, #0]
	bl	sub_0200065a
.L_020000b4:
	add	sp, #8
	pop	{r5, pc}
	push	{lr}
	movs	r0, #136
	lsls	r0, r0, #2
	bl	sub_02000648
	cmp	r0, #0
	bne.n	.L_020000d6
	movs	r0, #136
	lsls	r0, r0, #2
	bl	sub_0200065c
	movs	r0, #14
	movs	r1, #0
	bl	sub_020006ac
.L_020000d6:
	pop	{pc}
	push	{r5, lr}
	bl	sub_0200069c
	adds	r5, r0, #0
	bl	.L_0200037e
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r2, [r5, #16]
	movs	r3, #20
	bl	sub_020002ce
	movs	r0, #20
	bl	sub_0200061c
	bl	.L_02000450
	pop	{r5, pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x86ec
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r1, r2, r3
	subs	r3, #172
	str	r3, [r1, #0]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #55
	adds	r3, r2, r1
	movs	r1, #0
	strb	r1, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #54
	adds	r2, r2, r3
	ldr	r3, [pc, #40]
	strb	r1, [r2, #0]
	movs	r1, #139
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #2
	strb	r2, [r3, #0]
	bl	sub_0200071a
	movs	r2, #11
	movs	r1, #10
	movs	r0, #0
	bl	sub_0200072c
	movs	r0, #9
	movs	r1, #4
	bl	sub_0200071c
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r3, #192
	adds	r5, r7, r2
	lsls	r3, r3, #4
	movs	r2, #63
	adds	r6, r7, r3
	mov	r8, r2
.L_0200017a:
	ldr	r3, [r5, #24]
	cmp	r3, #19
	bhi.n	.L_020001c8
	movs	r2, #176
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	ldrh	r1, [r1, #0]
	movs	r2, #7
	asrs	r3, r3, #2
	ands	r3, r2
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r6, #8]
	adds	r0, r6, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	adds	r1, r5, #0
	bl	sub_020007a0
	adds	r0, r5, #0
	movs	r1, #63
	ldr	r2, [pc, #20]
	bl	sub_020007b2
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	b.n	.L_020001c8
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x8000
	.2byte 0xffff
.L_020001c8:
	.2byte 0x2301
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r2, #0
	bge.n	.L_0200017a
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r2, [sp, #0]
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	adds	r2, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r2, #0
	ble.n	.L_0200028c
	adds	r7, r2, #0
.L_02000208:
	bl	sub_0200074a
	movs	r1, #176
	lsls	r1, r1, #5
	add	r1, fp
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	sl, r1
	lsls	r6, r3, #3
	subs	r6, r6, r3
	lsls	r6, r6, #2
	movs	r3, #160
	add	r6, fp
	lsls	r3, r3, #3
	adds	r5, r6, r3
	movs	r1, #0
	str	r1, [r5, #24]
	ldr	r2, [sp, #8]
	mov	r8, r1
	str	r2, [r5, #0]
	ldr	r3, [sp, #4]
	mov	r9, r0
	str	r3, [r5, #4]
	ldr	r1, [sp, #0]
	subs	r7, #1
	str	r1, [r5, #8]
	bl	sub_0200077e
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r0, r0, #3
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r5, #0
	bl	sub_02000796
	mov	r3, r8
	str	r3, [r5, #12]
	movs	r3, #160
	lsls	r3, r3, #11
	mov	r1, r8
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	bl	sub_020007a0
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #12
	movs	r2, #128
	adds	r6, r6, r3
	lsls	r2, r2, #10
	lsls	r0, r0, #1
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r6, #0
	bl	sub_020007c0
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #63
	adds	r3, #1
	ands	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	cmp	r7, #0
	bne.n	.L_02000208
.L_0200028c:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #8
	movs	r0, #220
	sub	sp, #4
	bl	sub_02000800
	adds	r6, r0, #0
	ldr	r0, [pc, #152]
	bl	sub_02000838
	adds	r1, r6, #0
	bl	sub_0200081e
	bl	sub_0200083a
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r2, r6, #0
	adds	r5, r0, #0
	bl	sub_0200083e
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #2
	mov	sl, r0
	adds	r3, r6, r1
	mov	r2, sl
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r2, #160
	movs	r3, #192
	lsls	r2, r2, #3
	lsls	r3, r3, #4
	movs	r1, #63
	adds	r7, r6, r2
	adds	r5, r6, r3
	mov	r8, r1
.L_020002f4:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl	sub_020008f4
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
	movs	r3, #15
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	subs	r3, #241
	add	r8, r3
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r5, #40
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_020002f4
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r2, r6, r1
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl	sub_02000874
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001f0
	.2byte 0x815d
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl	sub_0200089e
	movs	r3, #176
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_020008de
	movs	r0, #220
	bl	sub_020008d4
.L_0200037e:
	pop	{r5, pc}
	.2byte 0x815d
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	adds	r4, r3, #0
	cmp	r0, #1
	bne.n	.L_020003a2
	ldr	r3, [sp, #16]
	adds	r0, r1, #0
	str	r3, [sp, #0]
	ldr	r3, [sp, #20]
	adds	r1, r2, #0
	str	r3, [sp, #4]
	adds	r2, r4, #0
	ldr	r3, [sp, #12]
	bl	sub_02000940
.L_020003a2:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r1, #0
	mov	sl, r0
	adds	r0, r6, #0
	adds	r7, r2, #0
	mov	r8, r3
	bl	sub_0200097c
	mov	r2, sl
	adds	r5, r0, #0
	cmp	r2, #1
	bne.n	.L_020003d8
	mov	r3, r8
	lsls	r2, r3, #16
	lsls	r1, r7, #16
	adds	r0, r6, #0
	bl	sub_02000998
	movs	r3, #0
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_020003d8:
	mov	r2, sl
	cmp	r2, #2
	bne.n	.L_0200040e
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L_02000406
.L_020003e8:
	movs	r0, #1
	bl	sub_02000914
	ldr	r3, [r5, #24]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #30
	adds	r3, r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	cmp	r3, r2
	ble.n	.L_020003e8
.L_02000406:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_0200040e:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	mov	sl, r0
	adds	r0, r7, #0
	mov	r8, r3
	adds	r5, r2, #0
	bl	sub_020009ec
	mov	r3, sl
	adds	r6, r0, #0
	cmp	r3, #2
	bne.n	.L_0200045e
	mov	r3, r8
	lsls	r2, r3, #16
	adds	r0, r7, #0
	lsls	r1, r5, #16
	bl	sub_02000a08
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r6, #0
	str	r3, [r6, #12]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
.L_02000450:
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #10
	str	r3, [r6, #72]
	movs	r0, #50
	bl	sub_02000984
.L_0200045e:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	cmp	r6, #1
	bne.n	.L_0200049e
	movs	r0, #177
	lsls	r3, r2, #16
	lsls	r1, r7, #16
	lsls	r0, r0, #1
	movs	r2, #0
	bl	sub_02000a20
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200049e
	movs	r1, #0
	bl	sub_02000a4c
	adds	r2, r5, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
.L_0200049e:
	cmp	r6, #2
	bne.n	.L_0200051a
	mov	r2, r8
	lsls	r3, r2, #16
	lsls	r1, r7, #16
	movs	r0, #252
	movs	r2, #0
	bl	sub_02000a46
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200051a
	movs	r1, #0
	bl	sub_02000a72
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_020004cc:
	movs	r0, #1
	bl	sub_020009f8
	ldr	r2, [r5, #24]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #30
	adds	r2, r2, r3
	ldrh	r3, [r5, #6]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	strh	r3, [r5, #6]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	str	r2, [r5, #24]
	str	r2, [r5, #28]
	cmp	r2, r3
	ble.n	.L_020004cc
	adds	r3, #1
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r3, [pc, #40]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	sub_02000ac6
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	strh	r3, [r5, #6]
	mov	r0, sl
	ldr	r1, [sp, #24]
	bl	sub_02000af0
.L_0200051a:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000129, 0x08000141, 0x08000151, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x08000291, 0x080003c9, 0x080003d1, 0x080200c1, 0x080201e1, 0x080201e9, 0x080201f1, 0x08020219, 0x080c8089, 0x080c80f9, 0x080c8119, 0x080c8281, 0x080c86a9, 0x080c86b9, 0x080c87c1, 0x080c87d1, 0x080c87e1
	overlay_veneer \EntryTarget
	.endr
	.4byte 0xffff0000
	.4byte 0x00000198
	.4byte 0x40000058
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0xc0000198
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x000001f8
	.4byte 0x400000e8
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000142
	.4byte 0x01402142
	.4byte 0x000001ff
	.4byte 0xffff0132
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020080d9
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x020080b9
	.4byte 0x00002115
	.4byte 0xffff0008
	.4byte 0x02008055
	.4byte 0x50008615
	.4byte 0xffff0008
	.4byte 0x02008065
	.4byte 0x00008715
	.4byte 0xffff0009
	.4byte 0x02008079
	.4byte 0x00009815
	.4byte 0xffff0009
	.4byte 0x0200807d
	.4byte 0x00008515
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte 0x02008089
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
