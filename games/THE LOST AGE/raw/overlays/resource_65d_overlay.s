.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000604, 0x02000604
	.set sub_020006ee, 0x020006ee
	.set sub_020006fa, 0x020006fa
	.set sub_02000714, 0x02000714
	.set sub_02000766, 0x02000766
	.set sub_02000768, 0x02000768
	.set sub_0200076a, 0x0200076a
	.set sub_0200077c, 0x0200077c
	.set sub_0200078e, 0x0200078e
	.set sub_0200079c, 0x0200079c
	.set sub_020007a4, 0x020007a4
	.set sub_020007c6, 0x020007c6
	.set sub_020007d4, 0x020007d4
	.set sub_020007ea, 0x020007ea
	.set sub_020007f6, 0x020007f6
	.set sub_02000802, 0x02000802
	.set sub_02000804, 0x02000804
	.set sub_0200080c, 0x0200080c
	.set sub_0200081a, 0x0200081a
	.set sub_02000826, 0x02000826
	.set sub_0200082e, 0x0200082e
	.set sub_02000832, 0x02000832
	.set sub_02000838, 0x02000838
	.set sub_02000842, 0x02000842
	.set sub_02000850, 0x02000850
	.set sub_02000854, 0x02000854
	.set sub_02000856, 0x02000856
	.set sub_0200085c, 0x0200085c
	.set sub_02000862, 0x02000862
	.set sub_02000866, 0x02000866
	.set sub_02000876, 0x02000876
	.set sub_0200087a, 0x0200087a
	.set sub_0200088e, 0x0200088e
	.set sub_0200089c, 0x0200089c
	.set sub_020008b0, 0x020008b0
	.set sub_020008c4, 0x020008c4
	.set sub_020008d6, 0x020008d6
	.set sub_020008e8, 0x020008e8
	.set sub_020008f4, 0x020008f4
	.set sub_020008fc, 0x020008fc
	.set sub_0200090c, 0x0200090c
	.set sub_02000948, 0x02000948
	.set sub_02000952, 0x02000952
	.set sub_02000976, 0x02000976
	.set sub_02000978, 0x02000978
	.set sub_0200097c, 0x0200097c
	.set sub_02000982, 0x02000982
	.set sub_02000988, 0x02000988
	.set sub_02000990, 0x02000990
	.set sub_02000992, 0x02000992
	.set sub_02000996, 0x02000996
	.set sub_020009a2, 0x020009a2
	.set sub_020009c8, 0x020009c8
	.set sub_020009dc, 0x020009dc
	.set sub_020009f0, 0x020009f0
	.set sub_02000a02, 0x02000a02
	.set sub_02000a08, 0x02000a08
	.set sub_02000a14, 0x02000a14
	.set sub_02000a1c, 0x02000a1c
	.set sub_02000a28, 0x02000a28
	.set sub_02000a32, 0x02000a32
	.set sub_02000a38, 0x02000a38
	.set sub_02000aa4, 0x02000aa4
	.set sub_02000aa8, 0x02000aa8
	.set sub_02000ace, 0x02000ace
	.set sub_02000ae0, 0x02000ae0
	.set sub_02000b02, 0x02000b02
	.set sub_02000b0e, 0x02000b0e
	.set sub_02000b12, 0x02000b12
	.set sub_02000b16, 0x02000b16
	.set sub_02000b24, 0x02000b24
	.set sub_02000b2e, 0x02000b2e
	.set sub_02000b38, 0x02000b38
	.set sub_02000b3a, 0x02000b3a
	.set sub_02000b40, 0x02000b40
	.set sub_02000b48, 0x02000b48
	.set sub_02000b58, 0x02000b58
	.set sub_02000b60, 0x02000b60
	.set sub_02000b90, 0x02000b90
	.set sub_02000b92, 0x02000b92
	.set sub_02000bba, 0x02000bba
	.set sub_02000bc4, 0x02000bc4
	.set sub_02000bc6, 0x02000bc6
	.set sub_02000bca, 0x02000bca
	.set sub_02000be2, 0x02000be2
	.set sub_02000bec, 0x02000bec
	.set sub_02000bee, 0x02000bee
	.set sub_02000c0a, 0x02000c0a
	.set sub_02000c0c, 0x02000c0c
	.set sub_02000c1a, 0x02000c1a
	.set sub_02000c1e, 0x02000c1e
	.set sub_02000c20, 0x02000c20
	.set sub_02000c3c, 0x02000c3c
	.set sub_02000c48, 0x02000c48
	.set sub_02000c66, 0x02000c66
	.set sub_02000c78, 0x02000c78
	.set sub_02000c86, 0x02000c86
	.set sub_02000c94, 0x02000c94
	.set sub_02000ca0, 0x02000ca0
	.set sub_02000cae, 0x02000cae
	.set sub_02000cca, 0x02000cca
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020082f9, 0x02008039, 0x02008069, 0x02008071, 0x0200815d, 0x02008041, 0x0200830d
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8764
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000058
	ldr	r0, [pc, #12]
.L_02000058:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000038
	.2byte 0x8794
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x87c4
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	cmp	r2, r3
	bne.n	.L_02000088
	ldr	r0, [pc, #12]
	b.n	.L_0200008a
.L_02000088:
	ldr	r0, [pc, #12]
.L_0200008a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000037
	.4byte 0x02008fa0
	.2byte 0x90c0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl	sub_020006ee
	movs	r0, #0
	bl	sub_0200077c
	movs	r5, #8
.L_020000b0:
	adds	r0, r5, #0
	bl	sub_02000714
	cmp	r0, #0
	beq.n	.L_020000c2
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_020000c2:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_020000b0
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r7, r6, r2
	movs	r3, #0
	ldrsh	r5, [r7, r3]
	movs	r0, #158
	bl	sub_020007c6
	subs	r5, #1
	ldr	r0, [pc, #104]
	lsls	r4, r5, #3
	adds	r3, r4, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r4]
	bl	sub_020006fa
	ldr	r3, [pc, #88]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	ldr	r0, [r6, #0]
	lsls	r2, r2, #7
	bl	sub_02000768
	ldr	r0, [r6, #0]
	bl	sub_02000766
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #2
	ldr	r0, [r6, #0]
	bl	sub_020007a4
	cmp	r5, #4
	beq.n	.L_0200012c
	movs	r2, #8
	ldr	r0, [r6, #0]
	movs	r1, #2
	negs	r2, r2
	bl	sub_0200079c
	movs	r0, #10
	bl	sub_0200076a
.L_0200012c:
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	bl	sub_020007ea
	bl	sub_020007f6
	bl	sub_02000802
	bl	sub_0200078e
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200873c
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #18
	movs	r1, #2
	movs	r2, #8
	bl	sub_0200082e
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
	bne.n	.L_02000174
	ldr	r0, [pc, #12]
	b.n	.L_02000176
.L_02000174:
	ldr	r0, [pc, #12]
.L_02000176:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000038
	.4byte 0x020092ac
	.2byte 0x90d8
	.2byte 0x0200
	push	{r5, lr}
	bl	sub_020007d4
	movs	r0, #0
	bl	sub_02000862
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl	sub_02000832
	movs	r1, #0
	movs	r0, #9
	bl	sub_02000842
	bl	sub_0200088e
	movs	r1, #0
	bl	sub_02000804
	cmp	r0, #0
	bne.n	.L_020001c0
	movs	r0, #10
	bl	sub_020007f6
	adds	r0, r5, #1
	bl	sub_02000854
	b.n	.L_020001cc
.L_020001c0:
	movs	r0, #20
	bl	sub_02000804
	adds	r0, r5, #2
	bl	sub_02000862
.L_020001cc:
	movs	r0, #9
	movs	r1, #0
	bl	sub_0200087a
	bl	sub_02000826
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x191f
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #1
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #5
	movs	r1, #49
	movs	r2, #5
	movs	r3, #41
	bl	sub_0200080c
	movs	r3, #4
	movs	r2, #40
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #48
	movs	r2, #3
	movs	r3, #2
	movs	r0, #4
	bl	sub_02000838
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #91
	bl	sub_0200081a
	add	sp, #8
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #128
	lsls	r0, r0, #2
	sub	sp, #8
	bl	sub_02000826
	cmp	r0, #0
	bne.n	.L_020002ee
	movs	r0, #0
	bl	sub_02000850
	movs	r0, #1
	bl	sub_02000856
	movs	r0, #2
	bl	sub_0200085c
	movs	r5, #16
	movs	r0, #56
	movs	r1, #46
	movs	r2, #30
	movs	r3, #46
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_02000866
	movs	r0, #56
	movs	r1, #174
	movs	r2, #30
	movs	r3, #174
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_02000876
	movs	r2, #15
	movs	r3, #23
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r2
	movs	r0, #28
	movs	r1, #23
	movs	r2, #16
	movs	r3, #16
	bl	sub_0200089c
	movs	r3, #21
	movs	r2, #88
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #2
	bl	sub_020008b0
	movs	r3, #89
	str	r3, [sp, #4]
	movs	r6, #17
	movs	r0, #28
	movs	r1, #90
	movs	r2, #4
	movs	r3, #4
	str	r6, [sp, #0]
	bl	sub_020008c4
	movs	r5, #91
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl	sub_020008d6
	movs	r3, #19
	str	r3, [sp, #0]
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #2
	str	r5, [sp, #4]
	bl	sub_020008e8
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r5, #92
	movs	r0, #28
	movs	r1, #92
	movs	r2, #2
	movs	r3, #2
	str	r5, [sp, #4]
	bl	sub_020008fc
	movs	r0, #30
	movs	r1, #92
	movs	r2, #2
	movs	r3, #2
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_0200090c
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_020008f4
.L_020002ee:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
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
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #16
	sub	sp, #8
	bl	sub_02000978
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	ldr	r5, [pc, #268]
	strb	r3, [r0, #0]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r5, r1
	ldr	r0, [r3, #0]
	bl	sub_02000990
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r2, #240
	lsls	r2, r2, #1
	strb	r3, [r0, #0]
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #236]
	cmp	r2, r3
	bne.n	.L_02000414
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_02000952
	cmp	r0, #0
	beq.n	.L_02000426
	movs	r0, #0
	bl	sub_0200097c
	movs	r0, #1
	bl	sub_02000982
	movs	r0, #2
	bl	sub_02000988
	movs	r5, #16
	movs	r0, #56
	movs	r1, #46
	movs	r2, #30
	movs	r3, #46
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_02000992
	movs	r0, #56
	movs	r1, #174
	movs	r2, #30
	movs	r3, #174
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_020009a2
	movs	r2, #15
	movs	r3, #23
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r2
	movs	r0, #28
	movs	r1, #23
	movs	r2, #16
	movs	r3, #16
	bl	sub_020009c8
	movs	r3, #21
	movs	r2, #88
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #2
	bl	sub_020009dc
	movs	r3, #89
	str	r3, [sp, #4]
	movs	r6, #17
	movs	r0, #28
	movs	r1, #90
	movs	r2, #4
	movs	r3, #4
	str	r6, [sp, #0]
	bl	sub_020009f0
	movs	r5, #91
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #1
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl	sub_02000a02
	movs	r3, #19
	str	r3, [sp, #0]
	movs	r0, #28
	movs	r1, #90
	movs	r2, #2
	movs	r3, #2
	str	r5, [sp, #4]
	bl	sub_02000a14
	mov	r3, r8
	movs	r5, #92
	str	r3, [sp, #0]
	movs	r0, #28
	movs	r1, #92
	movs	r2, #2
	movs	r3, #2
	str	r5, [sp, #4]
	bl	sub_02000a28
	movs	r0, #30
	movs	r1, #92
	movs	r2, #2
	movs	r3, #2
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_02000a38
	b.n	.L_02000426
.L_02000414:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #91
	bl	sub_02000a1c
	cmp	r0, #0
	beq.n	.L_02000426
	bl	sub_02000604
.L_02000426:
	movs	r0, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0038
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #168]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #160]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000454
	movs	r6, #12
	movs	r5, #17
	b.n	.L_02000458
.L_02000454:
	movs	r6, #3
	movs	r5, #9
.L_02000458:
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl	sub_02000b12
	movs	r0, #10
	bl	sub_02000aa8
	movs	r0, #4
	bl	sub_02000ace
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #1
	movs	r3, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #14
	movs	r0, #96
	movs	r1, #14
	movs	r2, #72
	bl	sub_02000aa4
	movs	r2, #4
	movs	r1, #0
	movs	r0, #4
	bl	sub_02000b16
	movs	r0, #4
	bl	sub_02000b24
	movs	r0, #4
	bl	sub_02000b02
	movs	r1, #0
	bl	sub_02000ae0
	movs	r0, #4
	movs	r1, #13
	bl	sub_02000b40
	adds	r2, r5, #0
	movs	r1, #0
	movs	r0, #4
	bl	sub_02000b3a
	movs	r0, #4
	bl	sub_02000b48
	movs	r1, #10
	movs	r0, #4
	bl	sub_02000b58
	movs	r0, #10
	bl	sub_02000b0e
	movs	r0, #123
	bl	sub_02000bc4
	adds	r0, r6, #0
	bl	sub_02000b92
	bl	sub_02000b2e
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0037
	.2byte 0x0000
	push	{lr}
	bl	sub_02000b38
	movs	r0, #0
	bl	sub_02000bc6
	movs	r2, #14
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl	sub_02000be2
	movs	r0, #4
	movs	r1, #16
	movs	r2, #0
	bl	sub_02000bec
	bl	sub_02000948
	pop	{pc}
	push	{lr}
	bl	sub_02000b60
	movs	r0, #0
	bl	sub_02000bee
	movs	r1, #2
	movs	r2, #6
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl	sub_02000c0c
	movs	r1, #14
	movs	r2, #8
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl	sub_02000c1a
	bl	sub_02000976
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	bl	sub_02000b90
	movs	r0, #0
	bl	sub_02000c1e
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl	sub_02000c0a
	bl	sub_02000996
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #4
	bl	sub_02000bca
	movs	r3, #10
	ldrsh	r5, [r0, r3]
	bl	sub_02000bba
	movs	r0, #0
	bl	sub_02000c48
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_02000582
	adds	r3, #15
.L_02000582:
	asrs	r3, r3, #4
	lsls	r3, r3, #4
	subs	r3, r5, r3
	cmp	r3, #7
	bgt.n	.L_020005aa
	movs	r1, #4
	movs	r2, #4
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl	sub_02000c78
	movs	r1, #4
	movs	r2, #8
	movs	r0, #4
	negs	r1, r1
	negs	r2, r2
	bl	sub_02000c86
	b.n	.L_020005c2
.L_020005aa:
	movs	r2, #4
	movs	r0, #4
	movs	r1, #4
	negs	r2, r2
	bl	sub_02000c94
	movs	r2, #8
	movs	r0, #4
	movs	r1, #4
	negs	r2, r2
	bl	sub_02000ca0
.L_020005c2:
	movs	r1, #212
	movs	r0, #4
	lsls	r1, r1, #1
	movs	r2, #99
	bl	sub_02000c3c
	bl	sub_02000a08
	pop	{r5, pc}
	push	{lr}
	bl	sub_02000c20
	movs	r0, #0
	bl	sub_02000cae
	movs	r2, #4
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl	sub_02000cca
	movs	r1, #212
	movs	r0, #4
	lsls	r1, r1, #1
	movs	r2, #99
	bl	sub_02000c66
	bl	sub_02000a32
	pop	{pc}
	.2byte 0x0000
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x08020171, 0x08020179, 0x080201a1, 0x080201e1, 0x080201e9, 0x08020219, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c8119, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c8581, 0x080c85f9, 0x080c8779, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00320051
	.4byte 0x00020001
	.4byte 0x00500006
	.4byte 0x00010032
	.4byte 0x00060002
	.4byte 0x004fffff
	.4byte 0x00010032
	.4byte 0x00060002
	.4byte 0x0032004e
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x0034004e
	.4byte 0x00020002
	.4byte 0x004e0006
	.4byte 0x00020035
	.4byte 0x00060002
	.4byte 0x0000ffff
	.4byte 0x020086f8
	.4byte 0x001d0054
	.4byte 0x020086f8
	.4byte 0x0022004d
	.4byte 0x0200870e
	.4byte 0x00260056
	.4byte 0x020086f8
	.4byte 0x0029004b
	.4byte 0x02008724
	.4byte 0x001b0045
	.4byte 0xffff0000
	.4byte 0x000001c8
	.4byte 0x40000248
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001c0044
	.4byte 0x004c0194
	.4byte 0x019c0024
	.4byte 0x0001ffff
	.4byte 0x001c00f4
	.4byte 0x00fc0064
	.4byte 0x006c0024
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000037
	.4byte 0x10101039
	.4byte 0xffffffff
	.4byte 0x10202039
	.4byte 0xffffffff
	.4byte 0x10303039
	.4byte 0xffffffff
	.4byte 0x10404039
	.4byte 0xffffffff
	.4byte 0x1050103a
	.4byte 0xffffffff
	.4byte 0x1060e002
	.4byte 0xffffffff
	.4byte 0x10709037
	.4byte 0xffffffff
	.4byte 0x1080a037
	.4byte 0xffffffff
	.4byte 0x10907037
	.4byte 0xffffffff
	.4byte 0x10a08037
	.4byte 0xffffffff
	.4byte 0x10b04038
	.4byte 0xffffffff
	.4byte 0x10c01038
	.4byte 0xffffffff
	.4byte 0x00000038
	.4byte 0x1010c037
	.4byte 0xffffffff
	.4byte 0x10203038
	.4byte 0xffffffff
	.4byte 0x10302038
	.4byte 0xffffffff
	.4byte 0x1040b037
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfc72f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfcb2f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfcf2f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfd4ef001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfd8ef001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfdcef001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfe46f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfe86f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfec6f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xff22f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xff62f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffa2f001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00014000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00034000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0000c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00015000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0001e000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x0001a000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00012000
	.4byte 0xffff00cb
	.4byte 0x02008850
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0xffff0051
	.4byte 0x02008bf8
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00008000
	.4byte 0x006000f5
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200809d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200809d
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x0200809d
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200809d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x0200809d
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
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte 0x02008545
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte 0x020084ed
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte 0x02008515
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000191e
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008189
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001922
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001923
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001924
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001925
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001926
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001927
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001938
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001939
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001928
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001929
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000192a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000192b
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000192c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000192d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000192e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000192f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000193a
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000193b
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0200814d
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303e
	.4byte 0x50008805
	.4byte 0xffff0032
	.4byte 0x020081e1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte 0x02008545
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte 0x020084ed
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte 0x02008515
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x02008565
	.4byte 0x0000c602
	.4byte 0xffff0022
	.4byte 0x020085d5
	.4byte 0x50008a05
	.4byte 0x0200003c
	.4byte 0x02008219
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
