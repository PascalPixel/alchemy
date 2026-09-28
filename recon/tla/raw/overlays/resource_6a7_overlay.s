.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008619, 0x02008049, 0x02008055, 0x0200805d, 0x02008611, 0x02008051, 0x02008681
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x23006d02
	.4byte 0x30227693
	.4byte 0x70032301
	.2byte 0x2001
	.2byte 0x4770
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8b94
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8bf4
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8c0c
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #9
	str	r3, [sp, #4]
	movs	r5, #25
	movs	r0, #47
	movs	r1, #9
	movs	r2, #9
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x02008a40
	movs	r3, #69
	str	r3, [sp, #4]
	movs	r0, #47
	movs	r1, #69
	movs	r2, #9
	movs	r3, #10
	str	r5, [sp, #0]
	bl 0x02008a40
	movs	r3, #89
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #111
	movs	r1, #8
	movs	r2, #9
	movs	r3, #5
	bl 0x02008a38
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008a20
	cmp	r0, #0
	bne.n	.L_02000100
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008a28
	bl 0x02008a68
	movs	r0, #0
	bl 0x02008b30
	ldr	r0, [pc, #56]
	bl 0x02008ac0
	movs	r0, #8
	bl 0x02008a80
	bl 0x02008a30
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x02008ad8
	movs	r0, #5
	bl 0x02008a60
	movs	r0, #5
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	bl 0x02008a70
.L_02000100:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2f31
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #12
	bl 0x02008a20
	cmp	r0, #0
	bne.n	.L_020001d2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #12
	bl 0x02008a28
	bl 0x02008a68
	movs	r0, #0
	bl 0x02008b30
	ldr	r0, [pc, #164]
	bl 0x02008ac0
	movs	r0, #8
	bl 0x02008a80
	bl 0x02008a30
	movs	r0, #5
	bl 0x02008a60
	movs	r0, #5
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r0, #232
	movs	r1, #1
	movs	r2, #176
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	bl 0x02008ae8
	bl 0x02008af0
	movs	r0, #5
	bl 0x02008a60
	movs	r1, #204
	movs	r2, #144
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02008ab0
	movs	r1, #204
	movs	r2, #144
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02008ab0
	movs	r1, #204
	movs	r2, #144
	movs	r0, #6
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02008ab0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	bl 0x02008a70
.L_020001d2:
	pop	{pc}
	.2byte 0x2f33
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #9
	sub	sp, #24
	bl 0x02008a80
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x02008a80
	mov	r8, r0
	movs	r0, #8
	bl 0x02008a80
	adds	r7, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #14
	bl 0x02008a20
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0200020c
	b.n	.L_020005f2
.L_0200020c:
	bl 0x02008a68
	movs	r0, #0
	bl 0x02008b30
	ldr	r2, [pc, #272]
	mov	sl, r2
	mov	r0, sl
	bl 0x02008ac0
	movs	r1, #232
	movs	r2, #216
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x02008a90
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x02008ad8
	movs	r0, #5
	bl 0x02008a60
	movs	r0, #11
	bl 0x02008a80
	str	r5, [r0, #24]
	movs	r0, #7
	bl 0x02008a80
	str	r5, [r0, #24]
	movs	r0, #5
	bl 0x02008a80
	str	r5, [r0, #24]
	movs	r0, #6
	bl 0x02008a80
	str	r5, [r0, #24]
	movs	r0, #11
	bl 0x02008a80
	movs	r1, #0
	bl 0x02008a48
	movs	r0, #7
	bl 0x02008a80
	movs	r1, #0
	bl 0x02008a48
	movs	r0, #5
	bl 0x02008a80
	movs	r1, #0
	bl 0x02008a48
	movs	r0, #6
	bl 0x02008a80
	movs	r1, #0
	bl 0x02008a48
	movs	r1, #232
	movs	r2, #216
	movs	r0, #11
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02008ab0
	movs	r1, #232
	movs	r2, #216
	movs	r0, #7
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02008ab0
	movs	r1, #232
	movs	r2, #216
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02008ab0
	movs	r1, #232
	movs	r2, #216
	movs	r0, #6
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02008ab0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #13
	bl 0x02008a20
	cmp	r0, #0
	bne.n	.L_020002ea
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #13
	bl 0x02008a28
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
.L_020002ea:
	mov	r0, sl
	adds	r0, #1
	bl 0x02008ac0
	movs	r1, #0
	movs	r0, #11
	bl 0x02008ac8
	movs	r0, #4
	movs	r1, #0
	bl 0x02008a78
	cmp	r0, #0
	beq.n	.L_0200032c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #16
	bl 0x02008aa0
	adds	r0, r7, #0
	bl 0x02008a30
	movs	r0, #5
	bl 0x02008a60
	b.n	.L_020005c6
	.2byte 0x2f37
	.2byte 0x0000
.L_0200032c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #11
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r1, #192
	movs	r2, #192
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #8
	bl 0x02008a88
	movs	r0, #8
	bl 0x02008a80
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r2, #16
	movs	r0, #8
	bl 0x02008b38
	movs	r0, #144
	bl 0x02008b58
	movs	r0, #4
	bl 0x02008a60
	movs	r3, #192
	lsls	r3, r3, #9
	mov	r2, r8
	str	r3, [r2, #24]
	str	r3, [r2, #28]
	movs	r0, #10
	bl 0x02008a80
	movs	r1, #6
	bl 0x02008a58
	movs	r1, #232
	movs	r2, #200
	lsls	r2, r2, #16
	movs	r0, #10
	lsls	r1, r1, #17
	bl 0x02008ab0
	movs	r0, #10
	movs	r1, #1
	bl 0x02008ab8
	movs	r3, #128
	lsls	r3, r3, #8
	movs	r1, #128
	movs	r2, #128
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	movs	r0, #9
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x02008a88
	movs	r1, #232
	movs	r2, #216
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x02008ab0
	movs	r1, #232
	movs	r0, #9
	lsls	r1, r1, #1
	movs	r2, #144
	bl 0x02008a98
	movs	r5, #15
.L_020003da:
	ldr	r3, [r6, #24]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #28]
	subs	r5, #1
	bl 0x020089b8
	cmp	r5, #0
	bge.n	.L_020003da
	movs	r0, #236
	movs	r1, #1
	movs	r2, #176
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x02008ae8
	bl 0x02008af0
	movs	r0, #9
	bl 0x02008aa8
	movs	r0, #145
	bl 0x02008b58
	ldr	r3, [r6, #8]
	ldr	r2, [pc, #480]
	add	r0, sp, #12
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r2, [pc, #476]
	ldr	r3, [r6, #12]
	mov	r1, sp
	str	r3, [r0, #4]
	movs	r5, #192
	ldr	r3, [r6, #16]
	lsls	r5, r5, #18
	adds	r3, r3, r2
	str	r3, [r0, #8]
	movs	r3, #200
	lsls	r3, r3, #17
	str	r3, [r1, #0]
	movs	r3, #0
	str	r3, [r1, #4]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r1, #8]
	ldr	r2, [pc, #444]
	bl 0x020086b0
	movs	r0, #20
	bl 0x02008a60
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	ldr	r2, [r5, #108]
	movs	r6, #218
	movs	r3, #100
	lsls	r6, r6, #1
	movs	r0, #128
	str	r3, [r2, r6]
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x02008b10
	bl 0x02008b20
	bl 0x02008b28
	ldr	r2, [r5, #108]
	movs	r0, #128
	movs	r3, #8
	lsls	r0, r0, #6
	str	r3, [r2, r6]
	adds	r0, #5
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x02008ad0
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x02008a88
	movs	r1, #232
	movs	r2, #176
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x02008a90
	movs	r0, #8
	movs	r1, #1
	bl 0x02008ae0
	movs	r1, #232
	movs	r0, #8
	lsls	r1, r1, #1
	movs	r2, #152
	bl 0x02008a90
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #153
	adds	r2, #204
	bl 0x02008a88
	ldr	r5, [pc, #276]
	ldr	r3, [r7, #28]
	movs	r2, #16
	adds	r3, r3, r5
	str	r3, [r7, #28]
	str	r3, [r7, #24]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #8
	bl 0x02008aa0
	movs	r0, #8
	bl 0x02008aa8
	ldr	r3, [r7, #28]
	movs	r2, #16
	adds	r3, r3, r5
	str	r3, [r7, #28]
	str	r3, [r7, #24]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #8
	bl 0x02008aa0
	movs	r0, #8
	bl 0x02008aa8
	ldr	r3, [r7, #28]
	movs	r2, #16
	adds	r3, r3, r5
	str	r3, [r7, #28]
	str	r3, [r7, #24]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #8
	bl 0x02008aa0
	movs	r0, #8
	bl 0x02008aa8
	ldr	r3, [r7, #28]
	movs	r2, #16
	adds	r3, r3, r5
	str	r3, [r7, #28]
	str	r3, [r7, #24]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #8
	bl 0x02008aa0
	movs	r0, #8
	bl 0x02008aa8
	ldr	r3, [r7, #28]
	movs	r2, #16
	adds	r3, r3, r5
	str	r3, [r7, #28]
	str	r3, [r7, #24]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #8
	bl 0x02008aa0
	movs	r0, #8
	bl 0x02008aa8
	movs	r1, #204
	movs	r2, #200
	lsls	r1, r1, #6
	lsls	r2, r2, #5
	movs	r0, #8
	adds	r1, #51
	adds	r2, #153
	bl 0x02008a88
	movs	r2, #12
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x02008aa0
	movs	r5, #19
.L_02000594:
	ldr	r3, [r7, #28]
	ldr	r2, [pc, #116]
	movs	r0, #7
	adds	r3, r3, r2
	str	r3, [r7, #28]
	str	r3, [r7, #24]
	subs	r5, #1
	bl 0x020089b8
	cmp	r5, #0
	bge.n	.L_02000594
	movs	r3, #0
	str	r3, [r7, #28]
	str	r3, [r7, #24]
	movs	r0, #10
	bl 0x02008a60
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #14
	bl 0x02008a28
	movs	r0, #4
	bl 0x02008b00
.L_020005c6:
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x02008ab0
	bl 0x02008a70
.L_020005f2:
	add	sp, #24
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffe0000
	.4byte 0xfff80000
	.4byte 0x02008065
	.4byte 0xfffff000
	.2byte 0xf700
	.2byte 0xffff
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8ccc
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [pc, #80]
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02008a80
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r2, #241
	lsls	r2, r2, #1
	strb	r3, [r0, #0]
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #90
	bne.n	.L_0200066e
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x02008a28
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #118
	adds	r3, r5, r2
	movs	r2, #1
	strh	r2, [r3, #0]
	ldr	r0, [pc, #20]
	movs	r1, #4
	bl 0x02008af8
.L_0200066e:
	bl 0x02008b08
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x010c
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_02000696
	bl 0x02008064
.L_02000696:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #14
	bl 0x02008a20
	cmp	r0, #0
	beq.n	.L_020006a8
	bl 0x02008064
.L_020006a8:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r1
	movs	r1, #240
	lsls	r1, r1, #5
	sub	sp, #28
	adds	r1, #88
	movs	r0, #220
	str	r2, [sp, #20]
	bl 0x020089c8
	movs	r1, #0
	mov	r9, r0
	ldr	r0, [pc, #548]
	str	r1, [sp, #8]
	bl 0x02008a18
	mov	r1, r9
	bl 0x020089f0
	bl 0x02008a08
	movs	r1, #160
	mov	r2, r9
	lsls	r1, r1, #3
	str	r0, [sp, #16]
	bl 0x02008a00
	movs	r7, #192
	lsls	r7, r7, #5
	movs	r6, #128
.L_020006f8:
	str	r0, [sp, #12]
	movs	r2, #0
	adds	r7, #112
	lsls	r6, r6, #5
	mov	r8, r2
	add	r7, r9
	add	r6, r9
.L_02000706:
	ldr	r2, [sp, #12]
	mov	r1, r8
.L_0200070a:
	movs	r3, #3
	ands	r3, r1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	str	r3, [sp, #0]
	movs	r3, #128
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	lsls	r3, r3, #23
	bl 0x02008b40
	ldrb	r1, [r6, #9]
	movs	r3, #0
	movs	r2, #13
	mov	fp, r3
	negs	r2, r2
	movs	r3, #250
	strh	r3, [r6, #30]
	adds	r3, r2, #0
	ands	r1, r3
	ldrb	r3, [r6, #5]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r1, r3
	strb	r1, [r6, #9]
	mov	r0, r8
	movs	r1, #6
	bl 0x020089a8
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r5, r0, #0
	lsls	r2, r5, #20
	adds	r3, r3, r2
	movs	r1, #6
	str	r3, [r7, #0]
	mov	r0, r8
	bl 0x020089b0
	mov	r2, sl
	ldr	r3, [r2, #4]
	lsls	r0, r0, #20
	subs	r3, r3, r0
	str	r3, [r7, #4]
	subs	r5, #4
	ldr	r3, [r2, #8]
	adds	r6, #40
	str	r3, [r7, #8]
	bl 0x020089c0
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r3, r3, #1
	muls	r3, r5
	str	r3, [r7, #12]
	bl 0x020089c0
	movs	r3, #128
	movs	r2, #1
	lsls	r3, r3, #10
	lsls	r0, r0, #2
	add	r8, r2
	adds	r0, r0, r3
	mov	r1, fp
	mov	r3, r8
	str	r0, [r7, #16]
	str	r1, [r7, #20]
	str	r1, [r7, #24]
	adds	r7, #28
	cmp	r3, #53
	ble.n	.L_02000706
	movs	r0, #192
	lsls	r0, r0, #3
	bl 0x020089e0
	movs	r5, #128
	lsls	r5, r5, #5
	mov	sl, r0
	adds	r0, r5, #0
	bl 0x020089d8
	adds	r5, r0, #0
	ldr	r0, [pc, #328]
	bl 0x02008a18
	adds	r1, r5, #0
	bl 0x020089f0
	bl 0x02008a08
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r2, r5, #0
	mov	fp, r0
	bl 0x02008a00
	str	r0, [sp, #4]
	adds	r0, r5, #0
	bl 0x020089e8
	mov	r5, sl
	movs	r1, #95
	adds	r5, #12
	mov	r8, r1
.L_020007e0:
	bl 0x020089c0
	movs	r2, #1
	lsls	r0, r0, #5
	negs	r2, r2
	lsrs	r0, r0, #16
	add	r8, r2
	negs	r0, r0
	mov	r3, r8
	str	r0, [r5, #0]
	adds	r5, #16
	cmp	r3, #0
	bge.n	.L_020007e0
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #252]
	ldr	r1, [pc, #256]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r6, #0
.L_02000810:
	movs	r3, #31
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_0200081e
	movs	r0, #145
	bl 0x02008b58
.L_0200081e:
	cmp	r6, #60
	bne.n	.L_0200083a
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x02008b10
	movs	r0, #90
	bl 0x02008b18
	movs	r0, #141
	bl 0x02008b58
.L_0200083a:
	cmp	r6, #105
	bne.n	.L_0200085e
	ldr	r1, [sp, #20]
	mov	lr, r1
	.2byte 0xf800
	.2byte 0x2201
	str	r2, [sp, #8]
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x02008a50
	movs	r0, #228
	bl 0x02008b58
.L_0200085e:
	cmp	r6, #130
	bne.n	.L_02000866
	movs	r3, #1
	str	r3, [sp, #8]
.L_02000866:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	beq.n	.L_020008ac
	movs	r7, #192
	lsls	r7, r7, #5
	movs	r5, #128
	adds	r7, #112
	lsls	r5, r5, #5
	movs	r2, #53
	add	r7, r9
	add	r5, r9
	mov	r8, r2
.L_0200087e:
	ldr	r3, [r7, #24]
	cmp	r3, #59
	bhi.n	.L_0200089c
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl 0x02008b48
	adds	r0, r7, #0
	movs	r1, #63
	ldr	r2, [pc, #120]
	bl 0x02008b50
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
.L_0200089c:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r1, r8
	adds	r5, #40
	adds	r7, #28
	cmp	r1, #0
	bge.n	.L_0200087e
.L_020008ac:
	mov	r5, sl
.L_020008ae:
	ldr	r2, [r5, #12]
	cmp	r2, #0
	bne.n	.L_02000910
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	bl 0x020089c0
	ldr	r3, [pc, #48]
	lsls	r0, r0, #7
	lsrs	r0, r0, #16
	adds	r0, #52
	ands	r0, r3
	ldr	r2, [pc, #44]
	ldrh	r3, [r5, #6]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #6]
	bl 0x020089c0
	ldrb	r3, [r5, #9]
	movs	r1, #13
	movs	r2, #240
	lsls	r0, r0, #6
	negs	r1, r1
	orrs	r3, r2
	lsrs	r0, r0, #16
	adds	r2, r1, #0
	adds	r0, #16
	ands	r3, r2
	strb	r0, [r5, #4]
	strb	r3, [r5, #9]
	b.n	.L_02000910
	.2byte 0x0000
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000001f2
	.4byte 0x000001e8
	.4byte 0x02008b60
	.4byte 0x050003e0
	.2byte 0xd000
	.2byte 0xffff
.L_02000910:
	.2byte 0x68eb
	cmp	r3, #0
	blt.n	.L_02000934
	ldr	r2, [sp, #4]
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	adds	r3, r2, r3
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #48]
	ands	r3, r2
	ldrh	r2, [r5, #8]
	adds	r0, r5, #0
	ands	r2, r1
	orrs	r2, r3
	strh	r2, [r5, #8]
	movs	r1, #100
	bl 0x02008a10
.L_02000934:
	ldr	r3, [r5, #12]
	adds	r3, #1
	str	r3, [r5, #12]
	cmp	r3, #31
	ble.n	.L_02000958
	bl 0x020089c0
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsrs	r3, r3, #16
	negs	r3, r3
	str	r3, [r5, #12]
	b.n	.L_02000958
	.2byte 0x0000
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_02000958:
	movs	r3, #190
	lsls	r3, r3, #3
	adds	r5, #16
	add	r3, sl
	cmp	r5, r3
	ble.n	.L_020008ae
	movs	r0, #1
	adds	r6, #1
	bl 0x020089b8
	cmp	r6, #149
	bgt.n	.L_02000972
	b.n	.L_02000810
.L_02000972:
	mov	r0, fp
	bl 0x020089f8
	mov	r0, sl
	bl 0x020089e8
	movs	r0, #40
	bl 0x02008a60
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [sp, #16]
	bl 0x020089f8
	movs	r0, #220
	bl 0x020089d0
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000f9, 0x08000141, 0x08000151, 0x08000169, 0x08000171, 0x08000179, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x080001e9, 0x08000291, 0x080003c9, 0x080003d1, 0x08020151, 0x080201e9, 0x080201f1, 0x08020219, 0x08020229, 0x08020279, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80c9, 0x080c80d1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81d9, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8269, 0x080c8279, 0x080c8331, 0x080c8379, 0x080c8391, 0x080c83a9, 0x080c83b9, 0x080c84e1, 0x080c85f1, 0x080c87c1, 0x080c87d1, 0x080c87e1, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x575a0260
	.4byte 0x46754ad7
	.4byte 0x31cf3a32
	.4byte 0x28ea294c
	.4byte 0x00750009
	.4byte 0x01bf011f
	.4byte 0x031f027f
	.4byte 0x7fff03ff
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
	.4byte 0xffff0002
	.4byte 0x000001d6
	.4byte 0xc00000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000001d6
	.4byte 0xc00000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000010c
	.4byte 0x1013c002
	.4byte 0xffffffff
	.4byte 0x1043d002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0008
	.4byte 0x02008b80
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff011d
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00020000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
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
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x190a0014
	.4byte 0x02008109
	.4byte 0x00000002
	.4byte 0x190a0015
	.4byte 0x020081d9
	.4byte 0x00000002
	.4byte 0x090a0016
	.4byte 0x020080a9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
