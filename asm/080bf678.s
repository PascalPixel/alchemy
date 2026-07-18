@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bf678
	.thumb_func
Func_080bf678:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #660]
	ldr	r2, [r1, #0]
	sub	sp, #48
	str	r2, [sp, #8]
	adds	r2, #68
	str	r2, [sp, #4]
	ldrb	r2, [r2, #0]
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	mov	fp, r3
	movs	r2, #0
	movs	r3, #1
	add	fp, r3
	mov	r9, r2
	cmp	r9, fp
	blt.n	.L0
	b.n	.L1
.L0:
	mov	r0, r9
	bl	Func_08077000
	movs	r1, #8
	adds	r3, r0, #0
	adds	r1, r1, r3
	mov	sl, r1
	movs	r1, #132
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	ldr	r3, [r3, #0]
	mov	r8, r2
	cmp	r8, r3
	bge.n	.L2
	mov	r5, sl
.L4:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	ble.n	.L3
	ldrb	r0, [r5, #2]
	bl	Func_080b7dd0
	cmp	r0, #0
	beq.n	.L3
	ldrb	r0, [r5, #2]
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	beq.n	.L3
	ldrb	r3, [r5, #3]
	subs	r3, #1
	strb	r3, [r5, #3]
.L3:
	movs	r3, #1
	add	r8, r3
	adds	r3, #255
	add	r3, sl
	ldr	r3, [r3, #0]
	adds	r5, #4
	cmp	r8, r3
	blt.n	.L4
.L2:
	movs	r3, #128
	lsls	r3, r3, #1
	add	r3, sl
	movs	r1, #0
	ldr	r3, [r3, #0]
	mov	r8, r1
	cmp	r8, r3
	bge.n	.L5
	mov	r6, sl
.L8:
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L6
	ldrb	r7, [r6, #2]
	adds	r0, r7, #0
	bl	Func_080b7dd0
	cmp	r0, #0
	beq.n	.L7
	bl	Func_080bdfec
	movs	r0, #30
	bl	Func_080bd808
	adds	r1, r7, #0
	movs	r0, #0
	bl	Func_080bbabc
	ldrb	r3, [r6, #0]
	lsls	r1, r3, #2
	adds	r1, r1, r3
	ldrb	r3, [r6, #1]
	lsls	r1, r1, #2
	movs	r2, #150
	adds	r1, r1, r3
	lsls	r2, r2, #1
	adds	r1, r1, r2
	movs	r0, #3
	bl	Func_080bbabc
	movs	r1, #175
	movs	r0, #14
	bl	Func_080bbabc
	movs	r1, #0
	movs	r0, #10
	bl	Func_080bbabc
	ldr	r1, [pc, #444]
	movs	r0, #4
	bl	Func_080bbabc
	adds	r1, r7, #0
	movs	r0, #11
	bl	Func_080bbabc
	movs	r0, #212
	bl	Func_080f9010
	adds	r0, r7, #0
	bl	Func_080b7dd0
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	Func_08009080
	adds	r0, r7, #0
	bl	Func_080b7dd0
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	Func_08009088
	ldrb	r5, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	Func_080771b0
	ldrb	r1, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r0, r7, #0
	bl	Func_080771c0
	adds	r0, r7, #0
	bl	Func_08077010
	adds	r1, r5, #0
	movs	r2, #3
	movs	r3, #0
	adds	r0, r7, #0
	bl	Func_080c1798
	bl	Func_080be02c
	b.n	.L7
.L6:
	movs	r3, #1
	adds	r6, #4
	add	r8, r3
.L7:
	movs	r3, #128
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r8, r3
	blt.n	.L8
.L5:
	movs	r1, #1
	add	r9, r1
	cmp	r9, fp
	bge.n	.L9
	b.n	.L0
.L9:
	ldr	r1, [pc, #316]
.L1:
	ldr	r3, [r1, #0]
	movs	r2, #201
	lsls	r2, r2, #3
	adds	r3, r3, r2
	ldrh	r1, [r3, #0]
	movs	r0, #2
	movs	r2, #0
	bl	Func_080c0774
	ldr	r3, [pc, #304]
	ldr	r1, [sp, #4]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #12]
	str	r4, [sp, #16]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L10
	ldr	r3, [sp, #8]
	adds	r3, #80
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L11
	movs	r3, #2
	mov	r2, sp
	str	r3, [sp, #12]
	adds	r2, #12
	movs	r3, #1
	str	r2, [sp, #0]
	str	r3, [r2, #4]
	b.n	.L12
.L10:
	mov	r3, sp
	adds	r3, #12
	str	r3, [sp, #0]
	b.n	.L12
.L11:
	mov	r1, sp
	adds	r1, #12
	str	r1, [sp, #0]
.L12:
	movs	r3, #20
	movs	r2, #0
	add	r3, sp
	mov	r9, r2
	mov	fp, r3
.L37:
	mov	r1, r9
	ldr	r2, [sp, #0]
	lsls	r3, r1, #2
	ldr	r0, [r3, r2]
	mov	r1, fp
	bl	Func_080b6c08
	movs	r3, #0
	mov	sl, r0
	mov	r8, r3
	cmp	r8, sl
	blt.n	.L13
	b.n	.L14
.L13:
	mov	r1, r8
	lsls	r3, r1, #1
	mov	r2, fp
	ldrh	r5, [r2, r3]
	adds	r0, r5, #0
	bl	Func_08077008
	movs	r3, #162
	lsls	r3, r3, #1
	adds	r7, r0, #0
	adds	r1, r7, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L15
	adds	r3, #255
	strb	r3, [r1, #0]
.L15:
	movs	r2, #56
	ldrsh	r1, [r7, r2]
	cmp	r1, #0
	beq.n	.L16
	adds	r0, r7, #0
	adds	r0, #68
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L17
	movs	r3, #52
	ldrsh	r2, [r7, r3]
	cmp	r1, r2
	beq.n	.L17
	ldrb	r6, [r0, #0]
	adds	r3, r1, r6
	cmp	r3, r2
	ble.n	.L18
	subs	r6, r2, r1
.L18:
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	Func_08077118
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	movs	r1, #5
	adds	r0, r6, #0
	bl	Func_08015120
	movs	r1, #56
	ldrsh	r2, [r7, r1]
	movs	r1, #52
	ldrsh	r3, [r7, r1]
	cmp	r2, r3
	bne.n	.L19
	ldr	r0, [pc, #108]
	bl	Func_080151c8
	b.n	.L20
.L19:
	ldr	r0, [pc, #104]
	bl	Func_080151c8
.L20:
	movs	r0, #175
	bl	Func_080f9010
	bl	Func_080bb65c
.L17:
	adds	r0, r7, #0
	adds	r0, #69
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L16
	movs	r2, #58
	ldrsh	r1, [r7, r2]
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	cmp	r1, r2
	beq.n	.L16
	ldrb	r6, [r0, #0]
	adds	r3, r1, r6
	cmp	r3, r2
	ble.n	.L21
	subs	r6, r2, r1
.L21:
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	Func_08077120
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	movs	r1, #5
	adds	r0, r6, #0
	bl	Func_08015120
	movs	r1, #58
	ldrsh	r2, [r7, r1]
	movs	r1, #54
	ldrsh	r3, [r7, r1]
	cmp	r2, r3
	bne.n	.L22
	ldr	r0, [pc, #24]
	bl	Func_080151c8
	b.n	.L23
	.4byte 0x03001e74
	.4byte 0x00000897
	.4byte 0x080c35bc
	.4byte 0x00000820
	.4byte 0x0000081d
	.4byte 0x00000821
.L22:
	ldr	r0, [pc, #568]
	bl	Func_080151c8
.L23:
	movs	r0, #175
	bl	Func_080f9010
	bl	Func_080bb65c
.L16:
	adds	r0, r5, #0
	bl	Func_080bf574
	cmp	r0, #0
	beq.n	.L24
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #524]
	bl	Func_080151c8
	bl	Func_080bb65c
.L24:
	adds	r0, r5, #0
	bl	Func_080bf250
	cmp	r0, #0
	beq.n	.L25
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #488]
	bl	Func_080151c8
	bl	Func_080bb65c
.L25:
	adds	r0, r5, #0
	bl	Func_080bf2b4
	cmp	r0, #0
	beq.n	.L26
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #448]
	bl	Func_080151c8
	bl	Func_080bb65c
.L26:
	adds	r0, r5, #0
	bl	Func_080bf318
	cmp	r0, #0
	beq.n	.L27
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #412]
	bl	Func_080151c8
	bl	Func_080bb65c
.L27:
	adds	r0, r5, #0
	bl	Func_080bf37c
	cmp	r0, #0
	beq.n	.L28
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #372]
	bl	Func_080151c8
	bl	Func_080bb65c
.L28:
	adds	r0, r5, #0
	bl	Func_080bf3bc
	cmp	r0, #0
	beq.n	.L29
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #336]
	bl	Func_080151c8
	bl	Func_080bb65c
.L29:
	adds	r0, r5, #0
	bl	Func_080bf400
	cmp	r0, #0
	beq.n	.L30
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #296]
	bl	Func_080151c8
	bl	Func_080bb65c
.L30:
	adds	r0, r5, #0
	bl	Func_080bf440
	cmp	r0, #0
	beq.n	.L31
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08015120
	adds	r0, r5, #0
	bl	Func_080b7aac
	ldr	r0, [pc, #252]
	bl	Func_080151c8
	bl	Func_080bb65c
.L31:
	adds	r0, r5, #0
	bl	Func_080bf484
	cmp	r0, #0
	beq.n	.L32
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08015120
	adds	r0, r5, #0
	bl	Func_080b7aac
	ldr	r0, [pc, #208]
	bl	Func_080151c8
	bl	Func_080bb65c
.L32:
	adds	r0, r5, #0
	bl	Func_080bf4c4
	cmp	r0, #0
	beq.n	.L33
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #172]
	bl	Func_080151c8
	bl	Func_080bb65c
.L33:
	adds	r0, r5, #0
	bl	Func_080bf524
	cmp	r0, #0
	beq.n	.L34
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #132]
	bl	Func_080151c8
	bl	Func_080bb65c
.L34:
	adds	r0, r5, #0
	bl	Func_080bf54c
	cmp	r0, #0
	beq.n	.L35
	adds	r0, r5, #0
	bl	Func_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #96]
	bl	Func_080151c8
	bl	Func_080bb65c
.L35:
	movs	r2, #1
	add	r8, r2
	cmp	r8, sl
	bge.n	.L14
	b.n	.L13
.L14:
	movs	r3, #1
	add	r9, r3
	mov	r1, r9
	cmp	r1, #1
	bgt.n	.L36
	b.n	.L37
.L36:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000081e
	.4byte 0x00000889
	.4byte 0x00000887
	.4byte 0x00000888
	.4byte 0x00000886
	.4byte 0x0000088b
	.4byte 0x0000088a
	.4byte 0x0000088e
	.4byte 0x0000088d
	.4byte 0x00000883
	.4byte 0x0000088c
	.4byte 0x00000891
	.4byte 0x00000892
