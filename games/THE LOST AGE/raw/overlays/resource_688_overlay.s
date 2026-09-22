.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_0200058c, 0x0200058c
	.set sub_020005d4, 0x020005d4
	.set sub_02000616, 0x02000616
	.set sub_0200062c, 0x0200062c
	.set sub_02000654, 0x02000654
	.set sub_02000656, 0x02000656
	.set sub_0200065a, 0x0200065a
	.set sub_02000662, 0x02000662
	.set sub_0200068c, 0x0200068c
	.set sub_020006a0, 0x020006a0
	.set sub_020006a4, 0x020006a4
	.set sub_020006a6, 0x020006a6
	.set sub_020006aa, 0x020006aa
	.set sub_020006b2, 0x020006b2
	.set sub_020006c4, 0x020006c4
	.set sub_020006c8, 0x020006c8
	.set sub_020006e4, 0x020006e4
	.set sub_020006ea, 0x020006ea
	.set sub_020006ee, 0x020006ee
	.set sub_020006f0, 0x020006f0
	.set sub_020006f4, 0x020006f4
	.set sub_020006f6, 0x020006f6
	.set sub_02000702, 0x02000702
	.set sub_02000722, 0x02000722
	.set sub_02000724, 0x02000724
	.set sub_0200072a, 0x0200072a
	.set sub_0200072e, 0x0200072e
	.set sub_02000732, 0x02000732
	.set sub_02000734, 0x02000734
	.set sub_0200073a, 0x0200073a
	.set sub_02000742, 0x02000742
	.set sub_02000746, 0x02000746
	.set sub_0200074a, 0x0200074a
	.set sub_02000752, 0x02000752
	.set sub_0200075a, 0x0200075a
	.set sub_0200075e, 0x0200075e
	.set sub_02000762, 0x02000762
	.set sub_0200076a, 0x0200076a
	.set sub_0200076c, 0x0200076c
	.set sub_02000772, 0x02000772
	.set sub_02000776, 0x02000776
	.set sub_02000778, 0x02000778
	.set sub_0200077a, 0x0200077a
	.set sub_02000782, 0x02000782
	.set sub_02000784, 0x02000784
	.set sub_02000786, 0x02000786
	.set sub_0200078a, 0x0200078a
	.set sub_0200078e, 0x0200078e
	.set sub_02000790, 0x02000790
	.set sub_02000792, 0x02000792
	.set sub_0200079a, 0x0200079a
	.set sub_0200079c, 0x0200079c
	.set sub_020007a2, 0x020007a2
	.set sub_020007a8, 0x020007a8
	.set sub_020007aa, 0x020007aa
	.set sub_020007b2, 0x020007b2
	.set sub_020007b4, 0x020007b4
	.set sub_020007ba, 0x020007ba
	.set sub_020007c0, 0x020007c0
	.set sub_020007c2, 0x020007c2
	.set sub_020007cc, 0x020007cc
	.set sub_020007d8, 0x020007d8
	.set sub_020007ea, 0x020007ea
	.set sub_02000808, 0x02000808
	.set sub_02000838, 0x02000838
	.set sub_0200086a, 0x0200086a
	.set sub_0200087e, 0x0200087e
	.set sub_0200089e, 0x0200089e
	.set sub_020008a0, 0x020008a0
	.set sub_020008ba, 0x020008ba
	.set sub_020008d6, 0x020008d6
	.set sub_020008dc, 0x020008dc
	.set sub_020008de, 0x020008de
	.set sub_02000906, 0x02000906
	.set sub_0200090a, 0x0200090a
	.set sub_0200093a, 0x0200093a
	.set sub_02000958, 0x02000958
	.set sub_0200096a, 0x0200096a
	.set sub_0200097a, 0x0200097a
	.set sub_020009a6, 0x020009a6
	.set sub_020009b0, 0x020009b0
	.set sub_02000a12, 0x02000a12
	.set sub_02000a3c, 0x02000a3c
	.set sub_02000a50, 0x02000a50
	.set sub_02000a92, 0x02000a92
	.set sub_02000a98, 0x02000a98
	.set sub_02000aa4, 0x02000aa4
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008191, 0x02008045, 0x02008051, 0x02008059, 0x02008119, 0x0200804d, 0x0200830d
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x23006d02
	.4byte 0x20017693
	.2byte 0x4770
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x873c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x876c
	.2byte 0x0200
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
	bl	sub_0200058c
	movs	r0, #123
	bl	sub_0200065a
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
	bl	sub_02000656
	bl	sub_02000662
	movs	r3, #4
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	sub_02000654
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
	bl	sub_020005d4
	movs	r0, #123
	bl	sub_020006aa
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
	bl	sub_020006a6
	bl	sub_020006b2
	movs	r3, #4
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	sub_020006a4
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #9
	bl	sub_02000616
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl	sub_020006a0
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88d4
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl	sub_0200062c
	cmp	r0, #0
	beq.n	.L_02000168
	bl	sub_0200068c
	movs	r0, #0
	bl	sub_02000702
	bl	sub_020006ee
	bl	sub_02000702
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
	bl	sub_020006f4
	bl	sub_020006c8
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
	bl	sub_02000724
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #19
	bl	sub_02000734
	movs	r1, #0
	bl	sub_020006ea
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #7
	bl	sub_020006e4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #180
	bl	sub_020006f6
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #9
	bl	sub_020006f0
	cmp	r0, #0
	beq.n	.L_020001fc
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl	sub_02000786
.L_020001fc:
	movs	r0, #9
	bl	sub_0200076c
	movs	r1, #0
	bl	sub_02000722
	movs	r0, #10
	bl	sub_02000778
	movs	r1, #0
	bl	sub_0200072e
	movs	r0, #11
	bl	sub_02000784
	movs	r1, #0
	bl	sub_0200073a
	movs	r0, #12
	bl	sub_02000790
	movs	r1, #0
	bl	sub_02000746
	movs	r0, #13
	bl	sub_0200079c
	movs	r1, #0
	bl	sub_02000752
	movs	r0, #14
	bl	sub_020007a8
	movs	r1, #0
	bl	sub_0200075e
	movs	r0, #15
	bl	sub_020007b4
	movs	r1, #0
	bl	sub_0200076a
	movs	r0, #16
	bl	sub_020007c0
	movs	r1, #0
	bl	sub_02000776
	movs	r0, #17
	bl	sub_020007cc
	movs	r1, #0
	bl	sub_02000782
	movs	r0, #18
	bl	sub_020007d8
	movs	r1, #0
	bl	sub_0200078e
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
	bl	sub_02000838
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
	bl	sub_0200086a
	bl	sub_0200087e
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
	bl	sub_020007ea
	cmp	r0, #0
	beq.n	.L_020002fa
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #107
	bl	sub_02000808
	bl	sub_020006c4
.L_020002fa:
	movs	r0, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x00c7
	.2byte 0x0000
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
	bl	sub_0200090a
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
	bl	sub_0200089e
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
	bl	sub_020008ba
	adds	r2, r5, #0
	adds	r0, r6, #0
	movs	r1, #0
	movs	r3, #5
	bl	sub_020008d6
	adds	r5, r0, #0
	b.n	.L_020003a6
.L_020003a0:
	movs	r0, #1
	bl	sub_020008a0
.L_020003a6:
	bl	sub_020008dc
	cmp	r0, #0
	beq.n	.L_020003a0
	mov	r0, sl
	bl	sub_02000906
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_020008de
	bl	sub_0200090a
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
	bl	sub_0200093a
	movs	r0, #0
	bl	sub_020009b0
	movs	r1, #138
	movs	r2, #136
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r2, #143
	movs	r0, #8
	adds	r1, #30
	bl	sub_0200096a
	ldr	r1, [pc, #248]
	movs	r0, #8
	bl	sub_0200097a
	movs	r0, #80
	bl	sub_02000958
	ldr	r0, [pc, #240]
	bl	sub_020009a6
	movs	r0, #6
	movs	r1, #90
	bl	sub_02000722
	movs	r0, #7
	movs	r1, #90
	bl	sub_0200072a
	movs	r0, #6
	movs	r1, #180
	bl	sub_02000732
	movs	r0, #7
	movs	r1, #90
	bl	sub_0200073a
	movs	r0, #6
	movs	r1, #250
	bl	sub_02000742
	movs	r0, #7
	movs	r1, #180
	bl	sub_0200074a
	movs	r0, #5
	movs	r1, #250
	bl	sub_02000752
	movs	r0, #8
	movs	r1, #90
	bl	sub_0200075a
	movs	r0, #7
	movs	r1, #180
	bl	sub_02000762
	movs	r0, #8
	movs	r1, #180
	bl	sub_0200076a
	movs	r0, #7
	movs	r1, #180
	bl	sub_02000772
	movs	r0, #6
	movs	r1, #90
	bl	sub_0200077a
	movs	r0, #7
	movs	r1, #250
	bl	sub_02000782
	movs	r0, #5
	movs	r1, #250
	bl	sub_0200078a
	movs	r0, #7
	movs	r1, #250
	bl	sub_02000792
	movs	r0, #6
	movs	r1, #90
	bl	sub_0200079a
	movs	r0, #5
	movs	r1, #90
	bl	sub_020007a2
	movs	r0, #8
	movs	r1, #90
	bl	sub_020007aa
	movs	r0, #7
	movs	r1, #250
	bl	sub_020007b2
	movs	r0, #4
	movs	r1, #90
	bl	sub_020007ba
	movs	r1, #250
	movs	r0, #7
	bl	sub_020007c2
	movs	r0, #8
	bl	sub_02000a3c
	movs	r0, #10
	bl	sub_02000a12
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
	bl	sub_02000a98
	bl	sub_02000aa4
	movs	r0, #1
	bl	sub_02000a92
	strb	r6, [r5, #0]
	bl	sub_02000a50
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x020085e4
	.4byte 0x00002881
	.irp EntryTarget, 0x080000c1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020219, 0x08038019, 0x08038039, 0x08038049, 0x080380f9, 0x08038109, 0x08038141, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80f9, 0x080c8119, 0x080c8181, 0x080c8269, 0x080c8279, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85c9, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
