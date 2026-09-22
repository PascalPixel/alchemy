.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_08009020, 0x08009020
	.set sub_08015068, 0x08015068
	.set sub_08015080, 0x08015080
	.set sub_08077008, 0x08077008
	.set sub_08077080, 0x08077080
	.set sub_080770c0, 0x080770c0
	.set sub_080a10d0, 0x080a10d0
	.set sub_080a112c, 0x080a112c
	.set sub_080a17c4, 0x080a17c4
	.set sub_080a1804, 0x080a1804
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1fd4, 0x080a1fd4
	.set sub_080a65e4, 0x080a65e4
	.set sub_080a68a8, 0x080a68a8
	.set sub_080a68ec, 0x080a68ec
	.set sub_080a6a00, 0x080a6a00
	.set sub_080a6a98, 0x080a6a98
	.set sub_080a6b64, 0x080a6b64
	.set sub_080a735c, 0x080a735c
	.set sub_080f9010, 0x080f9010
	.global Overlay_080a6ccc
Overlay_080a6ccc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r0, [sp, #48]
	ldr	r1, [sp, #48]
	ldr	r3, [pc, #644]
	lsls	r1, r1, #2
	ldr	r7, [r3, #0]
	movs	r0, #0
	adds	r3, r1, #0
	str	r0, [sp, #44]
	str	r0, [sp, #28]
	str	r0, [sp, #24]
	str	r1, [sp, #20]
	adds	r3, #20
	ldr	r2, [r7, r3]
	movs	r3, #13
	strb	r3, [r2, #5]
	adds	r5, r7, #0
	movs	r3, #14
	str	r3, [sp, #0]
	adds	r5, #52
	movs	r3, #2
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #3
	movs	r3, #17
	bl	sub_080a10d0
	ldr	r5, [r5, #0]
	movs	r2, #0
	mov	r8, r5
	str	r2, [sp, #36]
	adds	r3, r7, #2
	ldr	r0, [sp, #48]
	lsls	r0, r0, #1
	str	r3, [sp, #16]
	add	r4, sp, #52
	str	r0, [sp, #12]
	b.n	.L_080a72cc
.L_080a6d28:
	ldr	r1, [sp, #48]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r1, [sp, #16]
	ldrb	r0, [r1, r3]
	str	r4, [sp, #8]
	bl	sub_08077008
	movs	r2, #154
	str	r0, [sp, #32]
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080a6db2
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r1, r7, r3
	movs	r2, #1
	b.n	.L_080a6dbe
.L_080a6d54:
	movs	r0, #130
	b.n	.L_080a707e
.L_080a6d58:
	movs	r0, #113
	str	r4, [sp, #8]
	bl	sub_080f9010
	movs	r0, #1
	negs	r0, r0
	movs	r1, #1
	str	r0, [sp, #44]
	str	r1, [sp, #36]
	ldr	r4, [sp, #8]
	b.n	.L_080a72cc
.L_080a6d6e:
	ldr	r3, [r4, #24]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	str	r3, [sp, #44]
	movs	r3, #154
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	str	r3, [sp, #36]
	b.n	.L_080a72cc
.L_080a6d8a:
	movs	r0, #130
	str	r4, [sp, #8]
	bl	sub_080f9010
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #24]
	movs	r0, #228
	lsls	r3, r3, #1
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	movs	r1, #154
	lsls	r1, r1, #2
	str	r3, [sp, #44]
	adds	r2, r7, r1
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r2, #1
	str	r2, [sp, #36]
	b.n	.L_080a72cc
.L_080a6db2:
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldr	r0, [sp, #32]
	movs	r2, #2
	str	r4, [sp, #8]
.L_080a6dbe:
	bl	sub_080a68ec
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	ldr	r4, [sp, #8]
	movs	r2, #228
	lsls	r2, r2, #1
	adds	r0, r7, r2
	str	r4, [sp, #8]
	bl	sub_080a68a8
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #48]
	adds	r0, r4, #0
	bl	sub_080a6a00
	movs	r3, #1
	str	r3, [sp, #40]
	mov	r9, r3
	ldr	r3, [sp, #20]
	ldr	r2, [pc, #384]
	adds	r3, #20
	ldr	r3, [r7, r3]
	movs	r1, #4
	mov	r0, r9
	mov	sl, r1
	mov	fp, r2
	strb	r0, [r3, #5]
	ldr	r4, [sp, #8]
	b.n	.L_080a72b8
.L_080a6dfe:
	ldr	r1, [r4, #16]
	lsls	r1, r1, #4
	adds	r1, #36
	movs	r0, #88
	str	r4, [sp, #8]
	bl	sub_080a1a40
	mov	r3, r9
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080a6ece
	ldr	r1, [sp, #28]
	movs	r2, #228
	lsls	r3, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	movs	r0, #0
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_080a6e34
	lsls	r3, r1, #2
	adds	r3, #72
	ldr	r0, [r7, r3]
	bl	sub_080a17c4
	ldr	r4, [sp, #8]
.L_080a6e34:
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq.n	.L_080a6e54
	movs	r0, #0
	str	r0, [sp, #40]
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_080030f8
	ldr	r4, [sp, #8]
	mov	r0, r8
	adds	r2, r4, #0
	movs	r1, #0
	bl	sub_080a6b64
	ldr	r4, [sp, #8]
.L_080a6e54:
	adds	r2, r4, #0
	add	r1, sp, #80
	mov	r0, r8
	str	r4, [sp, #8]
	bl	sub_080a6a98
	movs	r3, #188
	ldr	r1, [sp, #12]
	ldr	r4, [sp, #8]
	lsls	r3, r3, #1
	adds	r2, r1, r3
	ldr	r3, [r4, #24]
	movs	r0, #228
	lsls	r0, r0, #1
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	movs	r1, #135
	strh	r3, [r7, r2]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r2, [r4, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_080a6ea0
	lsls	r3, r2, #2
	adds	r3, #72
	ldr	r0, [r7, r3]
	movs	r3, #9
	strb	r3, [r0, #5]
	movs	r3, #250
	strh	r6, [r0, #12]
	strb	r3, [r0, #15]
.L_080a6ea0:
	ldr	r3, [pc, #204]
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	movs	r5, #0
	cmp	r6, r3
	bcs.n	.L_080a6ece
	adds	r6, r2, #0
.L_080a6eae:
	movs	r0, #138
	lsls	r3, r5, #2
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r7, r3]
	movs	r1, #1
	str	r4, [sp, #8]
	bl	sub_08009020
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	ldrb	r3, [r6, #0]
	ldr	r4, [sp, #8]
	cmp	r5, r3
	bcc.n	.L_080a6eae
.L_080a6ece:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_080030f8
	ldr	r4, [sp, #8]
	ldr	r1, [r4, #24]
	ldr	r3, [pc, #152]
	str	r1, [sp, #28]
	ldr	r3, [r3, #0]
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080a6efc
	add	r3, sp, #60
	ldr	r1, [r4, #20]
	movs	r0, #0
	str	r3, [sp, #0]
	movs	r2, #5
	add	r3, sp, #68
	bl	sub_080a1fd4
	ldr	r4, [sp, #8]
	b.n	.L_080a6f00
.L_080a6efc:
	movs	r0, #1
	negs	r0, r0
.L_080a6f00:
	cmp	r0, #1
	bne.n	.L_080a6f0a
	movs	r3, #1
	str	r3, [sp, #40]
	mov	r9, r3
.L_080a6f0a:
	cmp	r0, #0
	bne.n	.L_080a6f12
	movs	r1, #1
	mov	r9, r1
.L_080a6f12:
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_080a6f1e
	movs	r3, #0
	mov	r9, r3
.L_080a6f1e:
	movs	r0, #154
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080a6f2c
	b.n	.L_080a7010
.L_080a6f2c:
	mov	r1, fp
	ldr	r3, [r1, #0]
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a6fb0
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne.n	.L_080a6fb0
	ldr	r3, [r4, #24]
	subs	r0, #160
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	ldr	r0, [pc, #24]
	ands	r0, r3
	str	r4, [sp, #8]
	bl	sub_08077080
	ldrb	r3, [r0, #12]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080a6f78
	movs	r0, #114
	bl	sub_080f9010
	b.n	.L_080a6fae
	movs	r0, r0
	.4byte 0x00003fff
	.4byte 0x03001f2c
	.4byte 0x03001c94
	.4byte 0x00000219
	.2byte 0x1ae8
	.2byte 0x0300
.L_080a6f78:
	movs	r0, #174
	str	r4, [sp, #8]
	bl	sub_080f9010
	movs	r1, #1
	movs	r2, #136
	str	r1, [sp, #24]
	lsls	r2, r2, #2
	adds	r1, r7, r2
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #48]
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #96
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #0
	movs	r2, #88
	movs	r3, #120
	bl	sub_08015068
	ldr	r0, [pc, #32]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #88
	bl	sub_08015080
.L_080a6fae:
	ldr	r4, [sp, #8]
.L_080a6fb0:
	ldr	r3, [pc, #20]
	ldr	r3, [r3, #0]
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080a7010
	b.n	.L_080a6fcc
	movs	r0, r0
	.4byte 0x00000002
	.4byte 0x00000ae1
	.2byte 0x1ae8
	.2byte 0x0300
.L_080a6fcc:
	ldr	r1, [sp, #24]
	cmp	r1, #1
	bne.n	.L_080a7010
	movs	r2, #0
	movs	r3, #136
	str	r2, [sp, #24]
	lsls	r3, r3, #2
	adds	r1, r7, r3
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #40]
	ands	r2, r3
	movs	r3, #96
	strh	r2, [r1, #0]
	mov	r0, r8
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #88
	movs	r3, #120
	str	r4, [sp, #8]
	bl	sub_08015068
	ldr	r0, [pc, #20]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #88
	bl	sub_08015080
	ldr	r4, [sp, #8]
	b.n	.L_080a7010
	movs	r0, r0
	.4byte 0x0000fffd
	.2byte 0x0b89
	.2byte 0x0000
.L_080a7010:
	mov	r0, fp
	ldr	r2, [r0, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a709a
	movs	r1, #154
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080a702a
	b.n	.L_080a6d54
.L_080a702a:
	ldr	r3, [r4, #24]
	movs	r0, #228
	lsls	r3, r3, #1
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldrh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_080a709a
	adds	r0, r3, #0
	str	r4, [sp, #8]
	bl	sub_080a735c
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080a704a
	b.n	.L_080a727c
.L_080a704a:
	ldr	r3, [r4, #24]
	movs	r1, #228
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r3, [r7, r3]
	ldr	r0, [pc, #32]
	ands	r0, r3
	str	r4, [sp, #8]
	bl	sub_08077080
	ldr	r1, [sp, #32]
	ldrb	r2, [r0, #9]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	ldr	r4, [sp, #8]
	cmp	r2, r3
	ble.n	.L_080a707c
	movs	r0, #114
	bl	sub_080f9010
	ldr	r4, [sp, #8]
	b.n	.L_080a709a
	.2byte 0x3fff
	.2byte 0x0000
.L_080a707c:
	movs	r0, #173
.L_080a707e:
	str	r4, [sp, #8]
	bl	sub_080f9010
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #24]
	movs	r2, #228
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	str	r3, [sp, #44]
	movs	r3, #1
	str	r3, [sp, #36]
	b.n	.L_080a72cc
.L_080a709a:
	mov	r0, fp
	ldr	r2, [r0, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a70a8
	b.n	.L_080a6d58
.L_080a70a8:
	ldr	r1, [pc, #116]
	ldr	r2, [r1, #0]
	adds	r3, #254
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080a70c2
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080a70c2
	b.n	.L_080a71c8
.L_080a70c2:
	ldr	r3, [pc, #96]
	ldr	r3, [r3, #0]
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080a70d0
	b.n	.L_080a71c8
.L_080a70d0:
	movs	r2, #154
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	negs	r5, r3
	orrs	r5, r3
	movs	r0, #111
	movs	r3, #2
	lsrs	r5, r5, #31
	subs	r5, r3, r5
	str	r4, [sp, #8]
	bl	sub_080f9010
	movs	r1, #134
	ldr	r0, [sp, #48]
	ldr	r2, [sp, #16]
	lsls	r1, r1, #2
	adds	r3, r0, r1
	ldr	r4, [sp, #8]
	ldrb	r3, [r2, r3]
	movs	r0, #152
	lsls	r0, r0, #2
	ldr	r2, [r4, #24]
	adds	r3, r3, r0
	strb	r2, [r7, r3]
	ldr	r1, [sp, #48]
	adds	r1, #28
	ldrsb	r6, [r7, r1]
	mov	r9, r1
	lsls	r5, r5, #24
.L_080a710c:
	ldr	r3, [pc, #16]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a7128
	adds	r6, #1
	b.n	.L_080a712a
	movs	r0, r0
	.4byte 0x03001b04
	.2byte 0x1ae8
	.2byte 0x0300
.L_080a7128:
	subs	r6, #1
.L_080a712a:
	ldr	r2, [pc, #136]
	adds	r3, r7, r2
	ldrb	r1, [r3, #0]
	adds	r0, r6, r1
	str	r4, [sp, #8]
	bl	sub_080022fc
	adds	r6, r0, #0
	lsls	r3, r6, #1
	movs	r2, #130
	mov	sl, r3
	lsls	r2, r2, #2
	add	r2, sl
	ldrh	r3, [r7, r2]
	ldr	r0, [pc, #112]
	str	r3, [r7, #8]
	ldrh	r2, [r7, r2]
	adds	r3, r7, r0
	strb	r2, [r3, #0]
	ldrb	r0, [r3, #0]
	bl	sub_08077008
	movs	r2, #228
	lsls	r2, r2, #1
	adds	r1, r7, r2
	asrs	r2, r5, #24
	bl	sub_080a68ec
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	lsls	r0, r0, #24
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080a710c
	mov	r2, r9
	strb	r6, [r7, r2]
	ldr	r2, [pc, #52]
	movs	r5, #0
	subs	r1, #212
.L_080a717c:
	lsls	r3, r5, #1
	adds	r3, r3, r1
	strh	r2, [r7, r3]
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #3
	bls.n	.L_080a717c
	movs	r2, #162
	ldr	r3, [pc, #32]
	lsls	r2, r2, #1
	add	r2, sl
	movs	r5, #130
	strh	r3, [r7, r2]
	lsls	r5, r5, #2
	add	r5, sl
	ldr	r0, [r7, #36]
	ldrh	r1, [r7, r5]
	movs	r2, #0
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_080a112c
	b.n	.L_080a71bc
	.4byte 0x0000001e
	.4byte 0x0000001a
	.4byte 0x00000219
	.2byte 0x021a
	.2byte 0x0000
.L_080a71bc:
	ldrh	r1, [r7, r5]
	adds	r0, r7, #0
	bl	sub_080a1804
	ldr	r4, [sp, #8]
	b.n	.L_080a72cc
.L_080a71c8:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7244
	ldr	r3, [pc, #56]
	ldr	r3, [r3, #0]
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080a7244
	ldr	r3, [r4, #24]
	movs	r1, #228
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r3, [r7, r3]
	ldr	r0, [pc, #28]
	ands	r0, r3
	str	r4, [sp, #8]
	bl	sub_08077080
	ldrb	r3, [r0, #12]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080a7214
	movs	r0, #114
	bl	sub_080f9010
	ldr	r4, [sp, #8]
	b.n	.L_080a7244
	movs	r0, r0
	.4byte 0x00003fff
	.2byte 0x1ae8
	.2byte 0x0300
.L_080a7214:
	movs	r0, #130
	str	r4, [sp, #8]
	bl	sub_080f9010
	movs	r0, #134
	ldr	r2, [sp, #48]
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #8]
	lsls	r0, r0, #2
	adds	r3, r2, r0
	ldrb	r0, [r1, r3]
	ldr	r3, [r4, #24]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r1, [r7, r3]
	movs	r2, #0
	bl	sub_080a65e4
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080a7244
	b.n	.L_080a6d6e
.L_080a7244:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a72b8
	ldr	r3, [pc, #56]
	ldr	r3, [r3, #0]
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080a72b8
	ldr	r3, [r4, #24]
	movs	r1, #228
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r3, [r7, r3]
	ldr	r0, [pc, #28]
	ands	r0, r3
	str	r4, [sp, #8]
	bl	sub_08077080
	ldrb	r3, [r0, #12]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080a7290
.L_080a727c:
	movs	r0, #114
	bl	sub_080f9010
	ldr	r4, [sp, #8]
	b.n	.L_080a72b8
	movs	r0, r0
	.4byte 0x00003fff
	.2byte 0x1ae8
	.2byte 0x0300
.L_080a7290:
	ldr	r2, [sp, #48]
	movs	r0, #134
	ldr	r1, [sp, #16]
	lsls	r0, r0, #2
	adds	r3, r2, r0
	ldrb	r0, [r1, r3]
	ldr	r3, [r4, #24]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r1, [r7, r3]
	movs	r2, #1
	str	r4, [sp, #8]
	bl	sub_080a65e4
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080a72b8
	b.n	.L_080a6d8a
.L_080a72b8:
	movs	r0, #168
	lsls	r0, r0, #1
	str	r4, [sp, #8]
	bl	sub_080770c0
	adds	r6, r0, #0
	ldr	r4, [sp, #8]
	cmp	r6, #0
	bne.n	.L_080a72cc
	b.n	.L_080a6dfe
.L_080a72cc:
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne.n	.L_080a72e4
	movs	r0, #168
	lsls	r0, r0, #1
	str	r4, [sp, #8]
	bl	sub_080770c0
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L_080a72e4
	b.n	.L_080a6d28
.L_080a72e4:
	movs	r0, #136
	lsls	r0, r0, #2
	adds	r1, r7, r0
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #104]
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r0, [r7, #68]
	str	r4, [sp, #8]
	bl	sub_080a17c4
	movs	r3, #186
	ldr	r1, [sp, #12]
	ldr	r4, [sp, #8]
	lsls	r3, r3, #1
	adds	r2, r1, r3
	ldr	r3, [r4, #24]
	strh	r3, [r7, r2]
	ldr	r0, [sp, #48]
	movs	r1, #134
	ldr	r2, [sp, #16]
	lsls	r1, r1, #2
	adds	r3, r0, r1
	ldrb	r3, [r2, r3]
	movs	r0, #152
	ldr	r2, [r4, #24]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	strb	r2, [r7, r3]
	ldr	r1, [sp, #12]
	movs	r2, #188
	add	r0, sp, #44
	ldrh	r0, [r0, #0]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	strh	r0, [r7, r3]
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a733e
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #44]
.L_080a733e:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #44]
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000fffd
