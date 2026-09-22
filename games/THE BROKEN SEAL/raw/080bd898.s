.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002322, 0x08002322
	.set sub_0800393c, 0x0800393c
	.set sub_080039fc, 0x080039fc
	.set sub_08003dec, 0x08003dec
	.set sub_080040d0, 0x080040d0
	.set sub_08009020, 0x08009020
	.set sub_08009080, 0x08009080
	.set sub_080090f8, 0x080090f8
	.set sub_08015048, 0x08015048
	.set sub_08015118, 0x08015118
	.set sub_08015120, 0x08015120
	.set sub_08015130, 0x08015130
	.set sub_080151d0, 0x080151d0
	.set sub_080152b8, 0x080152b8
	.set sub_08077008, 0x08077008
	.set sub_080b6cd0, 0x080b6cd0
	.set sub_080b78e4, 0x080b78e4
	.set sub_080b7aac, 0x080b7aac
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080b7e60, 0x080b7e60
	.set sub_080b7f70, 0x080b7f70
	.set sub_080ba918, 0x080ba918
	.set sub_080bac6c, 0x080bac6c
	.set sub_080bb588, 0x080bb588
	.set sub_080bb8e8, 0x080bb8e8
	.set sub_080bb928, 0x080bb928
	.set sub_080bbb0c, 0x080bbb0c
	.set sub_080bd850, 0x080bd850
	.set sub_080c2368, 0x080c2368
	.set sub_080c24f0, 0x080c24f0
	.set sub_080f9010, 0x080f9010
	.global Overlay_080bd898
Overlay_080bd898:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #788]
	ldr	r3, [r3, #0]
	sub	sp, #44
	movs	r1, #215
	movs	r2, #128
	str	r3, [sp, #8]
	lsls	r1, r1, #3
	lsls	r2, r2, #4
	adds	r7, r3, r1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080bd8c2
	b.n	.L_080bdfb4
.L_080bd8c2:
	movs	r3, #164
	lsls	r3, r3, #1
	adds	r5, r7, r3
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bne.n	.L_080bd8d0
	b.n	.L_080bdfb4
.L_080bd8d0:
	cmp	r3, #1
	bne.n	.L_080bd91e
	ldr	r2, [pc, #744]
	movs	r4, #160
	ldr	r1, [sp, #8]
	lsls	r4, r4, #1
	adds	r3, r1, r2
	adds	r6, r7, r4
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [r6, #0]
	cmp	r3, r2
	bge.n	.L_080bd918
	adds	r4, #4
	movs	r1, #166
	movs	r2, #0
	adds	r3, r7, r4
	lsls	r1, r1, #1
	str	r2, [r3, #0]
	adds	r4, #12
	adds	r3, r7, r1
	str	r2, [r3, #0]
	adds	r3, r7, r4
	str	r2, [r3, #0]
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #704]
	adds	r0, r1, r2
	ldr	r1, [r6, #0]
	bl	sub_080bbb0c
	ldr	r3, [r6, #0]
	adds	r3, #1
	str	r3, [r6, #0]
	movs	r3, #2
	str	r3, [r5, #0]
	b.n	.L_080bd8c2
.L_080bd918:
	movs	r3, #4
	str	r3, [r5, #0]
	b.n	.L_080bd8c2
.L_080bd91e:
	cmp	r3, #2
	beq.n	.L_080bd924
	b.n	.L_080bdb7a
.L_080bd924:
	movs	r4, #166
	movs	r1, #162
	lsls	r4, r4, #1
	lsls	r1, r1, #1
	adds	r3, r7, r4
	adds	r2, r7, r1
	ldr	r5, [r3, #0]
	ldr	r3, [r2, #0]
	cmp	r5, r3
	blt.n	.L_080bd93a
	b.n	.L_080bdb66
.L_080bd93a:
	adds	r6, r5, #0
.L_080bd93c:
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bd94e
	subs	r3, #1
	str	r3, [r2, #0]
	b.n	.L_080bdfb4
.L_080bd94e:
	ldrb	r3, [r7, r6]
	cmp	r3, #14
	bls.n	.L_080bd956
	b.n	.L_080bdb3e
.L_080bd956:
	ldr	r2, [pc, #624]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bd9b6
	.4byte 0x080bd9c4
	.4byte 0x080bd9d2
	.4byte 0x080bd9de
	.4byte 0x080bd9fa
	.4byte 0x080bda16
	.4byte 0x080bd9f0
	.4byte 0x080bda30
	.4byte 0x080bda42
	.4byte 0x080bda82
	.4byte 0x080bdb02
	.4byte 0x080bdb10
	.4byte 0x080bda36
	.4byte 0x080bd9a8
	.2byte 0xd99c
	.2byte 0x080b
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	bl	sub_080f9010
	b.n	.L_080bdb3e
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r1, [r7, r3]
	adds	r0, r7, #0
	bl	sub_080bb928
	b.n	.L_080bdb3e
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r1, #1
	bl	sub_08015120
	b.n	.L_080bdb3e
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	movs	r1, #5
	bl	sub_08015120
	b.n	.L_080bdb3e
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	ldr	r3, [pc, #496]
	movs	r1, #2
	b.n	.L_080bd9e8
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	ldr	r3, [pc, #488]
	movs	r1, #4
.L_080bd9e8:
	ands	r0, r3
	bl	sub_08015120
	b.n	.L_080bdb3e
	ldr	r3, [pc, #480]
	ldr	r2, [r3, #0]
	movs	r3, #1
	str	r3, [r2, #8]
	b.n	.L_080bdb3e
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	cmp	r0, #0
	blt.n	.L_080bda08
	bl	sub_080151d0
.L_080bda08:
	movs	r4, #164
	lsls	r4, r4, #1
	adds	r2, r7, r4
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #452]
	b.n	.L_080bda7c
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	cmp	r0, #0
	blt.n	.L_080bda24
	bl	sub_080151d0
.L_080bda24:
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r2, r7, r1
	movs	r3, #13
	str	r3, [r2, #0]
	b.n	.L_080bdb3e
	bl	sub_08015118
	b.n	.L_080bdb3e
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	bl	sub_080bb8e8
	b.n	.L_080bdb3e
	movs	r2, #180
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	cmp	r0, #0
	ble.n	.L_080bda52
	bl	sub_080f9010
.L_080bda52:
	movs	r3, #178
	lsls	r3, r3, #1
	adds	r2, r7, r3
	lsls	r3, r6, #2
	adds	r3, #64
	ldr	r0, [r7, r3]
	str	r0, [r2, #0]
	bl	sub_080b7dd0
	movs	r1, #5
	ldr	r0, [r0, #0]
	bl	sub_08009080
	movs	r4, #164
	lsls	r4, r4, #1
	movs	r1, #168
	adds	r2, r7, r4
	movs	r3, #10
	lsls	r1, r1, #1
	str	r3, [r2, #0]
	adds	r2, r7, r1
.L_080bda7c:
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_080bdb3e
	lsls	r3, r6, #2
	adds	r3, #64
	movs	r2, #178
	lsls	r2, r2, #1
	ldr	r0, [r7, r3]
	adds	r5, r7, r2
	movs	r4, #182
	lsls	r4, r4, #1
	str	r0, [r5, #0]
	adds	r3, r7, r4
	ldr	r1, [r3, #0]
	bl	sub_080c24f0
	ldr	r0, [r5, #0]
	bl	sub_080bb588
	ldr	r0, [r5, #0]
	bl	sub_08077008
	movs	r5, #0
	adds	r6, r0, #0
	b.n	.L_080bdaca
.L_080bdaae:
	movs	r1, #149
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_080bdac2
	movs	r1, #4
	bl	sub_08009020
	b.n	.L_080bdac8
.L_080bdac2:
	movs	r1, #5
	bl	sub_08009020
.L_080bdac8:
	adds	r5, #1
.L_080bdaca:
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	sub_080b7dd0
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	sub_080b7f70
	cmp	r0, #0
	bne.n	.L_080bdaae
	movs	r4, #149
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080bdb3e
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #11
	str	r2, [r3, #0]
	movs	r2, #168
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	b.n	.L_080bdb3e
	ldr	r3, [pc, #184]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_08015130
	b.n	.L_080bdb3e
	lsls	r5, r6, #2
	adds	r5, #64
	ldr	r0, [r7, r5]
	bl	sub_080b7dd0
	adds	r1, r0, #0
	ldr	r0, [r7, r5]
	bl	sub_080b78e4
	ldr	r0, [r7, r5]
	bl	sub_080b7dd0
	adds	r6, r0, #0
	ldr	r0, [r7, r5]
	bl	sub_080b6cd0
	adds	r1, r0, #0
	ldr	r0, [r6, #0]
	bl	sub_080ba918
	ldr	r0, [r7, r5]
	bl	sub_080b7aac
.L_080bdb3e:
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	movs	r4, #162
	adds	r5, r3, #1
	str	r5, [r2, #0]
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	cmp	r5, r3
	bge.n	.L_080bdb66
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	adds	r6, r5, #0
	cmp	r3, #2
	bne.n	.L_080bdb66
	b.n	.L_080bd93c
.L_080bdb66:
	movs	r3, #164
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	cmp	r3, #2
	beq.n	.L_080bdb74
	b.n	.L_080bd8c2
.L_080bdb74:
	movs	r3, #1
	str	r3, [r2, #0]
	b.n	.L_080bd8c2
.L_080bdb7a:
	cmp	r3, #3
	beq.n	.L_080bdb82
	cmp	r3, #13
	bne.n	.L_080bdbe0
.L_080bdb82:
	bl	sub_08015048
	cmp	r0, #0
	bne.n	.L_080bdb8c
	b.n	.L_080bdfb4
.L_080bdb8c:
	ldr	r3, [r5, #0]
	cmp	r3, #13
	bne.n	.L_080bdb9e
	movs	r4, #168
	movs	r3, #2
	lsls	r4, r4, #1
	str	r3, [r5, #0]
	adds	r2, r7, r4
	b.n	.L_080bdd1c
.L_080bdb9e:
	movs	r1, #176
	movs	r3, #5
	lsls	r1, r1, #1
	str	r3, [r5, #0]
	adds	r2, r7, r1
	subs	r3, #6
	str	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	b.n	.L_080bd8c2
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x00000655
	.4byte 0x00000654
	.4byte 0x080bd960
	.4byte 0x000001ff
	.4byte 0x00003fff
	.4byte 0x03001ee4
	.4byte 0x03001af8
	.2byte 0x1800
	.2byte 0x0300
.L_080bdbe0:
	cmp	r3, #5
	beq.n	.L_080bdbe6
	b.n	.L_080bdd2c
.L_080bdbe6:
	ldr	r2, [pc, #164]
	ldr	r3, [r2, #0]
	movs	r2, #7
	lsrs	r3, r3, #2
	ldr	r1, [pc, #160]
	ands	r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r1
	mov	sl, r3
	ldr	r3, [pc, #152]
	movs	r4, #170
	ldr	r3, [r3, #0]
	lsls	r4, r4, #1
	adds	r4, r4, r7
	mov	r9, r4
	ldr	r4, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r2, #176
	str	r3, [sp, #4]
	lsls	r2, r2, #1
	adds	r6, r7, r2
	mov	fp, r4
	ldr	r3, [r6, #0]
	movs	r4, #1
	movs	r1, #0
	negs	r4, r4
	mov	r8, r1
	cmp	r3, r4
	bne.n	.L_080bdc26
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #84]
	str	r3, [r6, #0]
.L_080bdc26:
	ldr	r5, [pc, #112]
	bl	sub_08015118
	adds	r0, r5, #0
	movs	r1, #4
	bl	sub_080039fc
	adds	r0, r5, #0
	movs	r1, #16
	bl	sub_0800393c
	movs	r3, #160
	mov	r2, r9
	lsls	r3, r3, #8
	str	r3, [r2, #4]
	mov	r3, r8
	str	r3, [r2, #8]
	mov	r1, sl
	ldr	r0, [r6, #0]
	bl	sub_080040d0
	ldr	r3, [pc, #48]
	mov	r4, r9
	ldrh	r2, [r4, #8]
	ands	r0, r3
	ldr	r3, [pc, #64]
	ands	r3, r2
	orrs	r3, r0
	mov	r1, r9
	strh	r3, [r1, #8]
	ldr	r4, [sp, #4]
	mov	r3, fp
	ldrh	r2, [r3, #12]
	ldrh	r3, [r4, #4]
	lsls	r2, r2, #3
	lsrs	r3, r3, #8
	adds	r2, r2, r3
	adds	r2, #4
	mov	r8, r2
	ldr	r3, [pc, #16]
	mov	r1, r8
	ands	r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #6]
	ldr	r3, [pc, #32]
	ands	r3, r2
	b.n	.L_080bdca4
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x03001e40
	.4byte 0x080c3734
	.4byte 0x03001ee4
	.4byte 0x0400004a
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_080bdca4:
	orrs	r3, r1
	ldr	r1, [pc, #800]
	ldr	r0, [r1, #0]
	mov	r4, r9
	strh	r3, [r4, #6]
	lsls	r0, r0, #12
	bl	sub_08002322
	cmp	r0, #0
	bge.n	.L_080bdcbc
	ldr	r2, [pc, #784]
	adds	r0, r0, r2
.L_080bdcbc:
	mov	r4, fp
	ldrh	r3, [r4, #14]
	ldr	r1, [sp, #4]
	asrs	r2, r0, #15
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldrh	r3, [r1, #6]
	lsrs	r3, r3, #8
	adds	r3, r3, r2
	ldr	r0, [pc, #768]
	adds	r3, #6
	mov	r2, r9
	strb	r3, [r2, #4]
	ldr	r3, [r0, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080bdd06
	ldr	r3, [pc, #752]
	ldr	r1, [pc, #756]
	ldr	r3, [r3, #0]
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080bdd06
	movs	r4, #168
	ldr	r3, [pc, #748]
	lsls	r4, r4, #1
	adds	r2, r7, r4
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r3, r3, r2
	cmp	r3, #10
	bls.n	.L_080bdd22
	ldr	r3, [r0, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080bdd22
.L_080bdd06:
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r2, r7, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
.L_080bdd1c:
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_080bd8c2
.L_080bdd22:
	mov	r0, r9
	movs	r1, #240
	bl	sub_08003dec
	b.n	.L_080bdfb4
.L_080bdd2c:
	cmp	r3, #10
	bne.n	.L_080bddb8
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080bdd96
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080bdd6e
	add	r1, sp, #28
	mov	r9, r1
	mov	r2, r9
	movs	r3, #255
	strh	r3, [r2, #0]
	adds	r3, #101
	adds	r5, r7, r3
	ldr	r0, [r5, #0]
	bl	sub_080b7dd0
	adds	r6, r0, #0
	ldr	r0, [r5, #0]
	bl	sub_080b6cd0
	adds	r1, r0, #0
	ldr	r0, [r6, #0]
	bl	sub_080ba918
	b.n	.L_080bdd90
.L_080bdd6e:
	movs	r1, #178
	lsls	r1, r1, #1
	movs	r4, #28
	adds	r3, r7, r1
	add	r4, sp
	ldr	r0, [r3, #0]
	mov	r9, r4
	mov	r2, r9
	movs	r3, #255
	strh	r0, [r2, #0]
	strh	r3, [r4, #2]
	bl	sub_080b7dd0
	movs	r1, #7
	ldr	r0, [r0, #0]
	bl	sub_080ba918
.L_080bdd90:
	mov	r0, r9
	bl	sub_080152b8
.L_080bdd96:
	movs	r2, #168
	lsls	r2, r2, #1
	adds	r1, r7, r2
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #8
	bgt.n	.L_080bdda8
	b.n	.L_080bdfb4
.L_080bdda8:
	movs	r4, #164
	lsls	r4, r4, #1
	adds	r3, r7, r4
	movs	r2, #2
	str	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r1, #0]
	b.n	.L_080bd8c2
.L_080bddb8:
	cmp	r3, #11
	beq.n	.L_080bddbe
	b.n	.L_080bd8c2
.L_080bddbe:
	movs	r1, #168
	lsls	r1, r1, #1
	adds	r5, r7, r1
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080bddd2
	movs	r2, #128
	lsls	r2, r2, #3
	cmp	r3, r2
	blt.n	.L_080bdeca
.L_080bddd2:
	movs	r4, #6
	mov	sl, r4
	cmp	r3, #0
	bne.n	.L_080bde1c
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bde1c
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	sub_08077008
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r0, [r0, #0]
	bl	sub_080c2368
	cmp	r0, #0
	blt.n	.L_080bde10
	subs	r0, #1
	cmp	r0, #0
	bge.n	.L_080bde0a
	movs	r0, #0
.L_080bde0a:
	adds	r0, #146
	bl	sub_080f9010
.L_080bde10:
	movs	r4, #168
	lsls	r4, r4, #1
	movs	r3, #128
	adds	r2, r7, r4
	lsls	r3, r3, #3
	str	r3, [r2, #0]
.L_080bde1c:
	movs	r1, #168
	lsls	r1, r1, #1
	adds	r2, r7, r1
	ldr	r3, [r2, #0]
	ldr	r4, [pc, #440]
	cmp	r3, r4
	ble.n	.L_080bde2e
	movs	r3, #0
	str	r3, [r2, #0]
.L_080bde2e:
	cmp	r3, #0
	bne.n	.L_080bde54
	movs	r1, #178
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	sub_08077008
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r0, r0, r2
	ldrb	r0, [r0, #0]
	bl	sub_080c2368
	cmp	r0, #0
	blt.n	.L_080bde54
	adds	r0, #146
	bl	sub_080f9010
.L_080bde54:
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	movs	r1, #128
	ldr	r3, [r3, #0]
	lsls	r1, r1, #3
	cmp	r3, r1
	blt.n	.L_080bde7c
	ldr	r2, [pc, #380]
	adds	r0, r3, r2
	cmp	r0, #0
	bge.n	.L_080bde70
	ldr	r4, [pc, #376]
	adds	r0, r3, r4
.L_080bde70:
	asrs	r0, r0, #3
	movs	r1, #5
	bl	sub_080022fc
	adds	r0, #1
	mov	sl, r0
.L_080bde7c:
	mov	r1, sl
	cmp	r1, #6
	beq.n	.L_080bde94
	movs	r2, #168
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080bde94
	b.n	.L_080bdfa2
.L_080bde94:
	movs	r3, #255
	movs	r6, #0
	add	r5, sp, #12
	mov	r8, r3
	b.n	.L_080bdeb0
.L_080bde9e:
	ldr	r2, [r0, #40]
	ldrb	r3, [r2, #22]
	mov	r1, r8
	mov	r4, sl
	orrs	r3, r1
	stmia	r5!, {r0}
	strb	r4, [r2, #5]
	strb	r3, [r2, #22]
	adds	r6, #1
.L_080bdeb0:
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	sub_080b7dd0
	adds	r1, r6, #0
	ldr	r0, [r0, #0]
	bl	sub_080b7f70
	cmp	r0, #0
	bne.n	.L_080bde9e
	b.n	.L_080bdfa2
.L_080bdeca:
	cmp	r3, #4
	bne.n	.L_080bdede
	movs	r4, #178
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	bl	sub_080bac6c
	ldr	r3, [r5, #0]
	b.n	.L_080bdfb0
.L_080bdede:
	cmp	r3, #4
	ble.n	.L_080bdfb0
	movs	r1, #178
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	sub_080b7dd0
	movs	r3, #1
	adds	r6, r0, #0
	movs	r2, #0
	add	r5, sp, #12
	strh	r3, [r6, #42]
	b.n	.L_080bdefe
.L_080bdefa:
	stmia	r5!, {r0}
	adds	r2, #1
.L_080bdefe:
	adds	r1, r2, #0
	ldr	r0, [r6, #0]
	str	r2, [sp, #0]
	bl	sub_080b7f70
	ldr	r2, [sp, #0]
	cmp	r0, #0
	bne.n	.L_080bdefa
	movs	r4, #168
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r1, #20
	lsls	r3, r3, #2
	negs	r1, r1
	adds	r1, r1, r3
	mov	r8, r1
	cmp	r1, #127
	ble.n	.L_080bdf60
	cmp	r2, #0
	ble.n	.L_080bdf3e
	add	r6, sp, #12
	adds	r5, r2, #0
.L_080bdf2c:
	add	r2, sp, #44
	ldmia	r6!, {r0}
	mov	r9, r2
	movs	r1, #0
	subs	r5, #1
	bl	sub_080bd850
	cmp	r5, #0
	bne.n	.L_080bdf2c
.L_080bdf3e:
	movs	r4, #178
	lsls	r4, r4, #1
	adds	r3, r7, r4
	ldr	r0, [r3, #0]
	bl	sub_080b7e60
	movs	r1, #164
	lsls	r1, r1, #1
	adds	r2, r7, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #0
	str	r3, [r2, #0]
	b.n	.L_080bdfb4
.L_080bdf60:
	cmp	r2, #0
	ble.n	.L_080bdfa2
	movs	r4, #19
	movs	r1, #18
	negs	r4, r4
	negs	r1, r1
	adds	r4, r4, r3
	adds	r1, r1, r3
	subs	r3, #17
	mov	fp, r4
	mov	r9, r1
	mov	sl, r3
	adds	r6, r2, #0
	add	r5, sp, #12
.L_080bdf7c:
	ldr	r0, [r5, #0]
	mov	r1, r8
	bl	sub_080090f8
	ldr	r0, [r5, #0]
	mov	r1, fp
	bl	sub_080090f8
	ldr	r0, [r5, #0]
	mov	r1, r9
	bl	sub_080090f8
	subs	r6, #1
	ldmia	r5!, {r0}
	mov	r1, sl
	bl	sub_080090f8
	cmp	r6, #0
	bne.n	.L_080bdf7c
.L_080bdfa2:
	movs	r3, #168
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_080bdfb4
.L_080bdfb0:
	adds	r3, #1
	str	r3, [r5, #0]
.L_080bdfb4:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e40
	.4byte 0x00007fff
	.4byte 0x03001ae8
	.4byte 0x03001af8
	.4byte 0x00000303
	.4byte 0x03001800
	.4byte 0x0000041d
	.4byte 0xfffffc00
	.4byte 0xfffffc07
