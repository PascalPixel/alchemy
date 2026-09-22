.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002df0, 0x08002df0
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_08015120, 0x08015120
	.set sub_080151c8, 0x080151c8
	.set sub_08077000, 0x08077000
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077118, 0x08077118
	.set sub_08077120, 0x08077120
	.set sub_080771b0, 0x080771b0
	.set sub_080771b8, 0x080771b8
	.set sub_080771c0, 0x080771c0
	.set sub_080b6b40, 0x080b6b40
	.set sub_080b6c08, 0x080b6c08
	.set sub_080b78e4, 0x080b78e4
	.set sub_080b7aac, 0x080b7aac
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080bb65c, 0x080bb65c
	.set sub_080bb938, 0x080bb938
	.set sub_080bbabc, 0x080bbabc
	.set sub_080bd808, 0x080bd808
	.set sub_080bdfec, 0x080bdfec
	.set sub_080be02c, 0x080be02c
	.set sub_080bf250, 0x080bf250
	.set sub_080bf2b4, 0x080bf2b4
	.set sub_080bf318, 0x080bf318
	.set sub_080bf37c, 0x080bf37c
	.set sub_080bf3bc, 0x080bf3bc
	.set sub_080bf400, 0x080bf400
	.set sub_080bf440, 0x080bf440
	.set sub_080bf484, 0x080bf484
	.set sub_080bf4c4, 0x080bf4c4
	.set sub_080bf524, 0x080bf524
	.set sub_080bf54c, 0x080bf54c
	.set sub_080bf574, 0x080bf574
	.set sub_080c0774, 0x080c0774
	.set sub_080c1798, 0x080c1798
	.set sub_080f9010, 0x080f9010
	.global Overlay_080bf678
Overlay_080bf678:
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
	blt.n	.L_080bf6aa
	b.n	.L_080bf7de
.L_080bf6aa:
	mov	r0, r9
	bl	sub_08077000
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
	bge.n	.L_080bf700
	mov	r5, sl
.L_080bf6ca:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	ble.n	.L_080bf6f0
	ldrb	r0, [r5, #2]
	bl	sub_080b7dd0
	cmp	r0, #0
	beq.n	.L_080bf6f0
	ldrb	r0, [r5, #2]
	bl	sub_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	beq.n	.L_080bf6f0
	ldrb	r3, [r5, #3]
	subs	r3, #1
	strb	r3, [r5, #3]
.L_080bf6f0:
	movs	r3, #1
	add	r8, r3
	adds	r3, #255
	add	r3, sl
	ldr	r3, [r3, #0]
	adds	r5, #4
	cmp	r8, r3
	blt.n	.L_080bf6ca
.L_080bf700:
	movs	r3, #128
	lsls	r3, r3, #1
	add	r3, sl
	movs	r1, #0
	ldr	r3, [r3, #0]
	mov	r8, r1
	cmp	r8, r3
	bge.n	.L_080bf7d2
	mov	r6, sl
.L_080bf712:
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_080bf7c0
	ldrb	r7, [r6, #2]
	adds	r0, r7, #0
	bl	sub_080b7dd0
	cmp	r0, #0
	beq.n	.L_080bf7c6
	bl	sub_080bdfec
	movs	r0, #30
	bl	sub_080bd808
	adds	r1, r7, #0
	movs	r0, #0
	bl	sub_080bbabc
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
	bl	sub_080bbabc
	movs	r1, #175
	movs	r0, #14
	bl	sub_080bbabc
	movs	r1, #0
	movs	r0, #10
	bl	sub_080bbabc
	ldr	r1, [pc, #444]
	movs	r0, #4
	bl	sub_080bbabc
	adds	r1, r7, #0
	movs	r0, #11
	bl	sub_080bbabc
	movs	r0, #212
	bl	sub_080f9010
	adds	r0, r7, #0
	bl	sub_080b7dd0
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	sub_08009080
	adds	r0, r7, #0
	bl	sub_080b7dd0
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	sub_08009088
	ldrb	r5, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	sub_080771b0
	ldrb	r1, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r0, r7, #0
	bl	sub_080771c0
	adds	r0, r7, #0
	bl	sub_08077010
	adds	r1, r5, #0
	movs	r2, #3
	movs	r3, #0
	adds	r0, r7, #0
	bl	sub_080c1798
	bl	sub_080be02c
	b.n	.L_080bf7c6
.L_080bf7c0:
	movs	r3, #1
	adds	r6, #4
	add	r8, r3
.L_080bf7c6:
	movs	r3, #128
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r8, r3
	blt.n	.L_080bf712
.L_080bf7d2:
	movs	r1, #1
	add	r9, r1
	cmp	r9, fp
	bge.n	.L_080bf7dc
	b.n	.L_080bf6aa
.L_080bf7dc:
	ldr	r1, [pc, #316]
.L_080bf7de:
	ldr	r3, [r1, #0]
	movs	r2, #201
	lsls	r2, r2, #3
	adds	r3, r3, r2
	ldrh	r1, [r3, #0]
	movs	r0, #2
	movs	r2, #0
	bl	sub_080c0774
	ldr	r3, [pc, #304]
	ldr	r1, [sp, #4]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #12]
	str	r4, [sp, #16]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080bf81c
	ldr	r3, [sp, #8]
	adds	r3, #80
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bf824
	movs	r3, #2
	mov	r2, sp
	str	r3, [sp, #12]
	adds	r2, #12
	movs	r3, #1
	str	r2, [sp, #0]
	str	r3, [r2, #4]
	b.n	.L_080bf82a
.L_080bf81c:
	mov	r3, sp
	adds	r3, #12
	str	r3, [sp, #0]
	b.n	.L_080bf82a
.L_080bf824:
	mov	r1, sp
	adds	r1, #12
	str	r1, [sp, #0]
.L_080bf82a:
	movs	r3, #20
	movs	r2, #0
	add	r3, sp
	mov	r9, r2
	mov	fp, r3
.L_080bf834:
	mov	r1, r9
	ldr	r2, [sp, #0]
	lsls	r3, r1, #2
	ldr	r0, [r3, r2]
	mov	r1, fp
	bl	sub_080b6c08
	movs	r3, #0
	mov	sl, r0
	mov	r8, r3
	cmp	r8, sl
	blt.n	.L_080bf84e
	b.n	.L_080bfb52
.L_080bf84e:
	mov	r1, r8
	lsls	r3, r1, #1
	mov	r2, fp
	ldrh	r5, [r2, r3]
	adds	r0, r5, #0
	bl	sub_08077008
	movs	r3, #162
	lsls	r3, r3, #1
	adds	r7, r0, #0
	adds	r1, r7, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080bf870
	adds	r3, #255
	strb	r3, [r1, #0]
.L_080bf870:
	movs	r2, #56
	ldrsh	r1, [r7, r2]
	cmp	r1, #0
	beq.n	.L_080bf944
	adds	r0, r7, #0
	adds	r0, #68
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_080bf8d0
	movs	r3, #52
	ldrsh	r2, [r7, r3]
	cmp	r1, r2
	beq.n	.L_080bf8d0
	ldrb	r6, [r0, #0]
	adds	r3, r1, r6
	cmp	r3, r2
	ble.n	.L_080bf894
	subs	r6, r2, r1
.L_080bf894:
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_08077118
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	movs	r1, #5
	adds	r0, r6, #0
	bl	sub_08015120
	movs	r1, #56
	ldrsh	r2, [r7, r1]
	movs	r1, #52
	ldrsh	r3, [r7, r1]
	cmp	r2, r3
	bne.n	.L_080bf8c0
	ldr	r0, [pc, #108]
	bl	sub_080151c8
	b.n	.L_080bf8c6
.L_080bf8c0:
	ldr	r0, [pc, #104]
	bl	sub_080151c8
.L_080bf8c6:
	movs	r0, #175
	bl	sub_080f9010
	bl	sub_080bb65c
.L_080bf8d0:
	adds	r0, r7, #0
	adds	r0, #69
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_080bf944
	movs	r2, #58
	ldrsh	r1, [r7, r2]
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	cmp	r1, r2
	beq.n	.L_080bf944
	ldrb	r6, [r0, #0]
	adds	r3, r1, r6
	cmp	r3, r2
	ble.n	.L_080bf8f0
	subs	r6, r2, r1
.L_080bf8f0:
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_08077120
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	movs	r1, #5
	adds	r0, r6, #0
	bl	sub_08015120
	movs	r1, #58
	ldrsh	r2, [r7, r1]
	movs	r1, #54
	ldrsh	r3, [r7, r1]
	cmp	r2, r3
	bne.n	.L_080bf934
	ldr	r0, [pc, #24]
	bl	sub_080151c8
	b.n	.L_080bf93a
	.4byte 0x03001e74
	.4byte 0x00000897
	.4byte 0x080c35bc
	.4byte 0x00000820
	.4byte 0x0000081d
	.2byte 0x0821
	.2byte 0x0000
.L_080bf934:
	ldr	r0, [pc, #568]
	bl	sub_080151c8
.L_080bf93a:
	movs	r0, #175
	bl	sub_080f9010
	bl	sub_080bb65c
.L_080bf944:
	adds	r0, r5, #0
	bl	sub_080bf574
	cmp	r0, #0
	beq.n	.L_080bf96e
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #524]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bf96e:
	adds	r0, r5, #0
	bl	sub_080bf250
	cmp	r0, #0
	beq.n	.L_080bf998
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #488]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bf998:
	adds	r0, r5, #0
	bl	sub_080bf2b4
	cmp	r0, #0
	beq.n	.L_080bf9c2
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #448]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bf9c2:
	adds	r0, r5, #0
	bl	sub_080bf318
	cmp	r0, #0
	beq.n	.L_080bf9ec
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #412]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bf9ec:
	adds	r0, r5, #0
	bl	sub_080bf37c
	cmp	r0, #0
	beq.n	.L_080bfa16
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #372]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bfa16:
	adds	r0, r5, #0
	bl	sub_080bf3bc
	cmp	r0, #0
	beq.n	.L_080bfa40
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #336]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bfa40:
	adds	r0, r5, #0
	bl	sub_080bf400
	cmp	r0, #0
	beq.n	.L_080bfa6a
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #296]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bfa6a:
	adds	r0, r5, #0
	bl	sub_080bf440
	cmp	r0, #0
	beq.n	.L_080bfa9a
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08015120
	adds	r0, r5, #0
	bl	sub_080b7aac
	ldr	r0, [pc, #252]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bfa9a:
	adds	r0, r5, #0
	bl	sub_080bf484
	cmp	r0, #0
	beq.n	.L_080bfaca
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08015120
	adds	r0, r5, #0
	bl	sub_080b7aac
	ldr	r0, [pc, #208]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bfaca:
	adds	r0, r5, #0
	bl	sub_080bf4c4
	cmp	r0, #0
	beq.n	.L_080bfaf4
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #172]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bfaf4:
	adds	r0, r5, #0
	bl	sub_080bf524
	cmp	r0, #0
	beq.n	.L_080bfb1e
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #132]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bfb1e:
	adds	r0, r5, #0
	bl	sub_080bf54c
	cmp	r0, #0
	beq.n	.L_080bfb48
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080b78e4
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015120
	ldr	r0, [pc, #96]
	bl	sub_080151c8
	bl	sub_080bb65c
.L_080bfb48:
	movs	r2, #1
	add	r8, r2
	cmp	r8, sl
	bge.n	.L_080bfb52
	b.n	.L_080bf84e
.L_080bfb52:
	movs	r3, #1
	add	r9, r3
	mov	r1, r9
	cmp	r1, #1
	bgt.n	.L_080bfb5e
	b.n	.L_080bf834
.L_080bfb5e:
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
	.2byte 0x0892
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r0, [sp, #12]
	ldrb	r0, [r0, #0]
	movs	r1, #0
	mov	r8, r0
	str	r1, [sp, #4]
	bl	sub_08077008
	mov	r2, r8
	str	r0, [sp, #8]
	movs	r0, #0
	cmp	r2, #7
	bls.n	.L_080bfbce
	movs	r0, #1
.L_080bfbce:
	bl	sub_08077000
	adds	r3, r0, #0
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r6, r3, #0
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #4]
	adds	r6, #8
	movs	r7, #0
	cmp	r1, r3
	bge.n	.L_080bfc18
	movs	r2, #1
	negs	r2, r2
	mov	sl, r2
	adds	r5, r6, #0
.L_080bfbf0:
	ldrb	r3, [r5, #2]
	cmp	r3, r8
	bne.n	.L_080bfc08
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, sl
	bne.n	.L_080bfc08
	ldrb	r1, [r5, #0]
	ldrb	r2, [r5, #1]
	mov	r0, r8
	bl	sub_080771b8
.L_080bfc08:
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	adds	r7, #1
	adds	r5, #4
	cmp	r7, r3
	blt.n	.L_080bfbf0
.L_080bfc18:
	movs	r0, #1
	movs	r1, #0
	bl	sub_080b6b40
	cmp	r0, #0
	beq.n	.L_080bfc34
	movs	r0, #2
	movs	r1, #0
	bl	sub_080b6b40
	cmp	r0, #0
	beq.n	.L_080bfc34
	movs	r1, #1
	str	r1, [sp, #4]
.L_080bfc34:
	mov	r2, r8
	movs	r0, #0
	cmp	r2, #7
	bls.n	.L_080bfc3e
	movs	r0, #1
.L_080bfc3e:
	bl	sub_08077000
	mov	r3, sp
	adds	r3, #16
	str	r3, [sp, #0]
	adds	r6, r0, #0
	ldr	r0, [sp, #0]
	adds	r6, #8
	movs	r2, #0
	add	r3, sp, #28
	mov	ip, r0
.L_080bfc54:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L_080bfc54
	movs	r3, #128
	lsls	r3, r3, #1
	movs	r1, #2
	adds	r7, r6, r3
	negs	r1, r1
	mov	r9, r1
	mov	fp, r7
.L_080bfc6a:
	movs	r2, #1
	ldr	r3, [r7, #0]
	negs	r2, r2
	movs	r4, #0
	mov	ip, r2
	cmp	r4, r3
	bge.n	.L_080bfc9e
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, r9
	bne.n	.L_080bfc86
	ldrb	r3, [r6, #2]
	mov	ip, r3
	b.n	.L_080bfc9e
.L_080bfc86:
	ldr	r3, [r7, #0]
	adds	r4, #1
	cmp	r4, r3
	bge.n	.L_080bfc9e
	lsls	r3, r4, #2
	adds	r2, r6, r3
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, r9
	bne.n	.L_080bfc86
	ldrb	r2, [r2, #2]
	mov	ip, r2
.L_080bfc9e:
	movs	r3, #1
	negs	r3, r3
	cmp	ip, r3
	beq.n	.L_080bfd18
	adds	r5, r3, #0
	ldr	r3, [r7, #0]
	cmp	r3, #0
	ble.n	.L_080bfccc
	mov	r0, fp
	ldr	r4, [r0, #0]
	adds	r2, r6, #0
.L_080bfcb4:
	ldrb	r3, [r2, #2]
	cmp	r3, ip
	bne.n	.L_080bfcc4
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, r5
	ble.n	.L_080bfcc4
	adds	r5, r3, #0
.L_080bfcc4:
	subs	r4, #1
	adds	r2, #4
	cmp	r4, #0
	bne.n	.L_080bfcb4
.L_080bfccc:
	adds	r5, #1
	cmp	r5, #1
	bgt.n	.L_080bfcd4
	movs	r5, #2
.L_080bfcd4:
	ldr	r3, [r7, #0]
	movs	r4, #0
	cmp	r4, r3
	bge.n	.L_080bfc6a
	movs	r2, #128
	movs	r1, #2
	lsls	r2, r2, #1
	negs	r1, r1
	adds	r2, r2, r6
	ldr	r0, [sp, #0]
	mov	sl, r1
	mov	lr, r2
	adds	r1, r6, #0
.L_080bfcee:
	ldrb	r3, [r1, #2]
	cmp	r3, ip
	bne.n	.L_080bfd0a
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, sl
	bne.n	.L_080bfd0a
	ldrb	r2, [r1, #0]
	strb	r5, [r1, #3]
	lsls	r2, r2, #2
	ldr	r3, [r0, r2]
	adds	r3, #1
	str	r3, [r0, r2]
	adds	r5, #1
.L_080bfd0a:
	mov	r2, lr
	ldr	r3, [r2, #0]
	adds	r4, #1
	adds	r1, #4
	cmp	r4, r3
	blt.n	.L_080bfcee
	b.n	.L_080bfc6a
.L_080bfd18:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L_080bfd20
	b.n	.L_080bff78
.L_080bfd20:
	movs	r5, #166
	movs	r0, #0
	lsls	r5, r5, #1
	mov	sl, r0
	adds	r0, r5, #0
	bl	sub_08004970
	adds	r2, r5, #0
	ldr	r3, [pc, #604]
	ldr	r1, [sp, #8]
	mov	r9, r0
	bl	sub_080072f0
	movs	r7, #1
	ldr	r2, [sp, #0]
	negs	r7, r7
	movs	r6, #0
.L_080bfd42:
	ldmia	r2!, {r3}
	cmp	r3, sl
	ble.n	.L_080bfd4c
	mov	sl, r3
	adds	r7, r6, #0
.L_080bfd4c:
	adds	r6, #1
	cmp	r6, #3
	ble.n	.L_080bfd42
	cmp	r7, #0
	blt.n	.L_080bfd68
	movs	r1, #150
	lsls	r1, r1, #1
	ldr	r0, [sp, #8]
	adds	r2, r7, r1
	ldrsb	r3, [r0, r2]
	cmp	r3, sl
	bge.n	.L_080bfd68
	mov	r1, sl
	strb	r1, [r0, r2]
.L_080bfd68:
	mov	r0, r8
	bl	sub_08077010
	movs	r6, #0
	movs	r7, #72
.L_080bfd72:
	ldr	r3, [sp, #8]
	mov	r1, r9
	ldrsh	r2, [r7, r3]
	ldrsh	r3, [r7, r1]
	subs	r5, r2, r3
	cmp	r5, #0
	ble.n	.L_080bfde8
	bl	sub_080bdfec
	movs	r0, #25
	bl	sub_080bd808
	mov	r1, r8
	movs	r0, #0
	bl	sub_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	sub_080bbabc
	movs	r1, #175
	movs	r0, #14
	bl	sub_080bbabc
	ldr	r1, [pc, #496]
	movs	r0, #4
	adds	r1, r6, r1
	bl	sub_080bbabc
	mov	r1, r8
	movs	r0, #11
	bl	sub_080bbabc
	movs	r0, #212
	bl	sub_080f9010
	mov	r0, r8
	bl	sub_080b7dd0
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	sub_08009080
	mov	r0, r8
	bl	sub_080b7dd0
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	sub_08009088
	mov	r3, sl
	adds	r1, r6, #0
	movs	r2, #2
	subs	r3, #1
	mov	r0, r8
	bl	sub_080c1798
	bl	sub_080be02c
.L_080bfde8:
	adds	r6, #1
	adds	r7, #4
	cmp	r6, #3
	ble.n	.L_080bfd72
	mov	r0, r9
	bl	sub_08002df0
	ldr	r1, [sp, #4]
	cmp	r1, #0
	bne.n	.L_080bfdfe
	b.n	.L_080bff78
.L_080bfdfe:
	bl	sub_080bdfec
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #96]
	cmp	r3, #0
	beq.n	.L_080bfe68
	mov	r1, r8
	movs	r0, #8
	bl	sub_080bbabc
	mov	r1, r8
	movs	r0, #0
	bl	sub_080bbabc
	ldr	r3, [sp, #12]
	movs	r0, #1
	ldr	r1, [r3, #96]
	bl	sub_080bbabc
	ldr	r1, [pc, #368]
	movs	r0, #4
	bl	sub_080bbabc
	ldr	r0, [sp, #12]
	ldr	r1, [r0, #96]
	mov	r0, r8
	negs	r1, r1
	bl	sub_08077118
	cmp	r0, #0
	bne.n	.L_080bfe60
	mov	r1, r8
	movs	r0, #9
	bl	sub_080bbabc
	mov	r1, r8
	movs	r0, #0
	bl	sub_080bbabc
	mov	r1, r8
	cmp	r1, #7
	bhi.n	.L_080bfe56
	ldr	r1, [pc, #328]
	b.n	.L_080bfe58
.L_080bfe56:
	ldr	r1, [pc, #328]
.L_080bfe58:
	movs	r0, #4
	bl	sub_080bbabc
	b.n	.L_080bfe68
.L_080bfe60:
	movs	r0, #11
	mov	r1, r8
	bl	sub_080bbabc
.L_080bfe68:
	bl	sub_080bb938
	bl	sub_080bdfec
	ldr	r3, [pc, #304]
	ldr	r2, [sp, #8]
	adds	r6, r2, r3
	movs	r0, #0
	ldrsb	r0, [r6, r0]
	cmp	r0, #0
	beq.n	.L_080bff04
	movs	r1, #52
	ldrsh	r3, [r2, r1]
	movs	r1, #10
	muls	r0, r3
	bl	sub_080022ec
	ldr	r3, [pc, #284]
	adds	r7, r0, #0
	mov	r1, r8
	movs	r0, #8
	ldr	r5, [r3, #0]
	bl	sub_080bbabc
	mov	r1, r8
	movs	r0, #0
	bl	sub_080bbabc
	adds	r1, r7, #0
	movs	r0, #1
	bl	sub_080bbabc
	ldr	r1, [pc, #256]
	movs	r0, #4
	bl	sub_080bbabc
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_080bfec2
	movs	r3, #130
	lsls	r3, r3, #4
	adds	r2, r5, r3
	movs	r3, #134
	b.n	.L_080bfeca
.L_080bfec2:
	movs	r0, #130
	lsls	r0, r0, #4
	adds	r2, r5, r0
	movs	r3, #133
.L_080bfeca:
	str	r3, [r2, #0]
	negs	r1, r7
	mov	r0, r8
	bl	sub_08077118
	cmp	r0, #0
	bne.n	.L_080bfefc
	mov	r1, r8
	movs	r0, #9
	bl	sub_080bbabc
	mov	r1, r8
	movs	r0, #0
	bl	sub_080bbabc
	mov	r1, r8
	cmp	r1, #7
	bhi.n	.L_080bfef2
	ldr	r1, [pc, #172]
	b.n	.L_080bfef4
.L_080bfef2:
	ldr	r1, [pc, #172]
.L_080bfef4:
	movs	r0, #4
	bl	sub_080bbabc
	b.n	.L_080bff04
.L_080bfefc:
	movs	r0, #11
	mov	r1, r8
	bl	sub_080bbabc
.L_080bff04:
	bl	sub_080bb938
	bl	sub_080bdfec
	ldr	r3, [pc, #160]
	ldr	r2, [sp, #8]
	adds	r1, r2, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080bff74
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080bff74
	movs	r1, #192
	lsls	r1, r1, #24
	mov	r0, r8
	bl	sub_08077118
	cmp	r0, #0
	bne.n	.L_080bff74
	mov	r1, r8
	movs	r0, #0
	bl	sub_080bbabc
	ldr	r5, [pc, #120]
	movs	r0, #4
	adds	r1, r5, #0
	bl	sub_080bbabc
	mov	r1, r8
	movs	r0, #8
	bl	sub_080bbabc
	mov	r1, r8
	movs	r0, #9
	bl	sub_080bbabc
	movs	r0, #0
	mov	r1, r8
	bl	sub_080bbabc
	mov	r0, r8
	cmp	r0, #7
	bhi.n	.L_080bff6c
	subs	r1, r5, #3
	movs	r0, #4
	bl	sub_080bbabc
	b.n	.L_080bff74
.L_080bff6c:
	adds	r1, r5, #3
	movs	r0, #4
	bl	sub_080bbabc
.L_080bff74:
	bl	sub_080bb938
.L_080bff78:
	mov	r0, r8
	bl	sub_08077010
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001388
	.4byte 0x00000879
	.4byte 0x0000084b
	.4byte 0x00000825
	.4byte 0x0000082b
	.4byte 0x00000131
	.4byte 0x03001e74
	.4byte 0x00000851
	.4byte 0x00000141
	.4byte 0x00000828
