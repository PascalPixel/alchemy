.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077080, 0x08077080
	.set sub_080770c0, 0x080770c0
	.set sub_08077120, 0x08077120
	.set sub_080a112c, 0x080a112c
	.set sub_080a1d08, 0x080a1d08
	.set sub_080a3cf8, 0x080a3cf8
	.set sub_080a5fe0, 0x080a5fe0
	.set sub_080a602c, 0x080a602c
	.set sub_080a63e4, 0x080a63e4
	.set sub_080a65e4, 0x080a65e4
	.set sub_080a6ccc, 0x080a6ccc
	.set sub_080a9cbc, 0x080a9cbc
	.set sub_080a9f10, 0x080a9f10
	.set sub_080aa460, 0x080aa460
	.set sub_080f9010, 0x080f9010
	.global Overlay_080a5cc0
Overlay_080a5cc0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	movs	r5, #0
	str	r0, [sp, #8]
	str	r2, [sp, #4]
	str	r5, [sp, #0]
	ldr	r3, [pc, #360]
	ldr	r7, [r3, #0]
	mov	fp, r5
	b.n	.L_080a5fa4
.L_080a5ce0:
	cmp	r5, #4
	bls.n	.L_080a5ce6
	b.n	.L_080a5fa0
.L_080a5ce6:
	ldr	r2, [pc, #352]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080a5d04
	.4byte 0x080a5d34
	.4byte 0x080a5e22
	.4byte 0x080a5dfa
	.2byte 0x5e92
	.2byte 0x080a
	movs	r3, #186
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [pc, #316]
	movs	r0, #0
	bl	sub_080a3cf8
	movs	r0, #0
	bl	sub_080a602c
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080a5d2a
	movs	r2, #1
	str	r2, [sp, #0]
	mov	fp, r3
.L_080a5d2a:
	ldr	r0, [r7, #44]
	bl	sub_08015270
	movs	r5, #1
	b.n	.L_080a5fa4
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #276]
	adds	r3, r7, r2
	ldrb	r0, [r3, #0]
	bl	sub_08077008
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_080a5d54
	b.n	.L_080a5fa4
.L_080a5d54:
	movs	r2, #154
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_080a5d74
	cmp	r3, #1
	bgt.n	.L_080a5d6a
	cmp	r3, #0
	beq.n	.L_080a5d70
	b.n	.L_080a5d86
.L_080a5d6a:
	cmp	r3, #2
	beq.n	.L_080a5d7e
	b.n	.L_080a5d86
.L_080a5d70:
	ldr	r1, [pc, #224]
	b.n	.L_080a5d76
.L_080a5d74:
	ldr	r1, [pc, #224]
.L_080a5d76:
	movs	r0, #0
	bl	sub_080a3cf8
	b.n	.L_080a5d86
.L_080a5d7e:
	ldr	r1, [pc, #220]
	movs	r0, #0
	bl	sub_080a3cf8
.L_080a5d86:
	bl	sub_080a9cbc
	ldr	r3, [pc, #196]
	adds	r6, r7, r3
	ldrb	r1, [r6, #0]
	movs	r2, #0
	ldr	r0, [r7, #36]
	movs	r3, #0
	bl	sub_080a112c
	movs	r0, #0
	bl	sub_080a6ccc
	movs	r2, #1
	negs	r2, r2
	adds	r1, r0, #0
	mov	r8, r2
	movs	r5, #0
	cmp	r1, r8
	bne.n	.L_080a5db0
	b.n	.L_080a5fa4
.L_080a5db0:
	movs	r2, #154
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r5, #2
	cmp	r3, #0
	bne.n	.L_080a5dc0
	b.n	.L_080a5fa4
.L_080a5dc0:
	cmp	r3, #1
	bne.n	.L_080a5dde
	movs	r2, #0
	ldrb	r0, [r6, #0]
	bl	sub_080a65e4
	ldr	r0, [r7, #44]
	bl	sub_08015278
	ldr	r0, [pc, #140]
	mov	r1, r8
	mov	r2, r8
	bl	sub_080a1d08
	b.n	.L_080a5df6
.L_080a5dde:
	movs	r2, #1
	ldrb	r0, [r6, #0]
	bl	sub_080a65e4
	ldr	r0, [r7, #44]
	bl	sub_08015278
	ldr	r0, [pc, #116]
	mov	r1, r8
	mov	r2, r8
	bl	sub_080a1d08
.L_080a5df6:
	movs	r5, #0
	b.n	.L_080a5fa4
	ldr	r1, [pc, #108]
	movs	r0, #0
	bl	sub_080a3cf8
	movs	r0, #0
	bl	sub_080a63e4
	movs	r3, #1
	mov	sl, r0
	negs	r3, r3
	movs	r5, #4
	cmp	sl, r3
	beq.n	.L_080a5e16
	b.n	.L_080a5fa4
.L_080a5e16:
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r1, r7, r2
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #32]
	b.n	.L_080a5f58
	bl	sub_080a5fe0
	cmp	r0, #1
	bne.n	.L_080a5e2e
.L_080a5e2a:
	movs	r5, #3
	b.n	.L_080a5fa4
.L_080a5e2e:
	cmp	r0, #2
	bne.n	.L_080a5e70
	ldr	r3, [pc, #56]
	adds	r2, r7, r3
	movs	r3, #9
	strb	r3, [r2, #0]
	movs	r5, #4
	b.n	.L_080a5fa4
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001f2c
	.4byte 0x080a5cf0
	.4byte 0x00000ae9
	.4byte 0x0000021a
	.4byte 0x00000aea
	.4byte 0x00000af1
	.4byte 0x00000af0
	.4byte 0x00000ae2
	.4byte 0x00000ae3
	.4byte 0x00000aeb
	.2byte 0x021b
	.2byte 0x0000
.L_080a5e70:
	movs	r2, #1
	str	r2, [sp, #0]
	mov	fp, r2
	ldr	r2, [pc, #272]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	ldr	r2, [sp, #8]
	str	r3, [r2, #0]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #256]
	ands	r3, r2
	ldr	r2, [sp, #4]
	str	r3, [r2, #0]
	b.n	.L_080a5fa4
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r2, r2, r7
	movs	r3, #0
	ldrh	r0, [r2, #0]
	mov	sl, r3
	mov	r8, r2
	ldr	r3, [pc, #228]
	ldr	r2, [pc, #236]
	adds	r5, r7, r3
	adds	r6, r7, r2
	movs	r3, #0
	ldrb	r1, [r5, #0]
	ldrb	r2, [r6, #0]
	bl	sub_080a9f10
	ldrb	r3, [r6, #0]
	mov	fp, r0
	cmp	r3, #9
	bne.n	.L_080a5ec2
	ldrb	r3, [r5, #0]
	strb	r3, [r6, #0]
	movs	r3, #9
	mov	sl, r3
.L_080a5ec2:
	movs	r2, #1
	negs	r2, r2
	mov	r9, r2
	cmp	fp, r9
	beq.n	.L_080a5ee4
	mov	r2, r8
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #184]
	ands	r0, r3
	bl	sub_08077080
	ldrb	r3, [r5, #0]
	ldrb	r1, [r0, #9]
	adds	r0, r3, #0
	negs	r1, r1
	bl	sub_08077120
.L_080a5ee4:
	ldrb	r0, [r5, #0]
	bl	sub_08077010
	cmp	fp, r9
	beq.n	.L_080a5f22
	ldrb	r1, [r6, #0]
	ldr	r0, [r7, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080a112c
	mov	r2, r8
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #140]
	ands	r0, r3
	bl	sub_080aa460
	ldr	r0, [r7, #44]
	bl	sub_08015278
	ldr	r2, [pc, #132]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #128]
	movs	r1, #0
	adds	r0, r0, r3
	mov	r2, r9
	bl	sub_080a1d08
	b.n	.L_080a5f42
.L_080a5f22:
	movs	r0, #114
	bl	sub_080f9010
	ldr	r0, [r7, #44]
	bl	sub_08015278
	ldr	r2, [pc, #100]
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #96]
	mov	r1, fp
	adds	r0, r0, r3
	mov	r2, fp
	bl	sub_080a1d08
.L_080a5f42:
	movs	r3, #1
	negs	r3, r3
	cmp	fp, r3
	beq.n	.L_080a5f60
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r1, r7, r3
	movs	r2, #1
	mov	fp, r2
	ldr	r3, [pc, #44]
	ldrh	r2, [r1, #0]
.L_080a5f58:
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r5, #1
	b.n	.L_080a5fa4
.L_080a5f60:
	ldr	r3, [pc, #56]
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	mov	r2, sl
	ldr	r1, [pc, #24]
	cmp	r2, #9
	beq.n	.L_080a5f72
	b.n	.L_080a5e2a
.L_080a5f72:
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	orrs	r3, r1
	strh	r3, [r2, #0]
	movs	r5, #1
	b.n	.L_080a5fa4
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x0000021a
	.4byte 0x00003fff
	.4byte 0x0000021b
	.4byte 0x0000025a
	.4byte 0x00000bef
	.2byte 0x0222
	.2byte 0x0000
.L_080a5fa0:
	movs	r2, #1
	str	r2, [sp, #0]
.L_080a5fa4:
	ldr	r3, [sp, #0]
	cmp	r3, #0
	bne.n	.L_080a5fb8
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a5fb8
	b.n	.L_080a5ce0
.L_080a5fb8:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a5fca
	movs	r2, #1
	negs	r2, r2
	mov	fp, r2
.L_080a5fca:
	mov	r0, fp
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
