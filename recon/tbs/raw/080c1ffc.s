.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08004458, 0x08004458
	.set sub_080072f8, 0x080072f8
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_08077140, 0x08077140
	.set sub_080c1afc, 0x080c1afc
	.set sub_080c1c54, 0x080c1c54
	.set sub_080c1df4, 0x080c1df4
	.set sub_080c1f50, 0x080c1f50
	.set sub_080c23c0, 0x080c23c0
	.global BattleFormation_BuildEnemyList
	.global Func_080c1ffc
	.thumb_func
BattleFormation_BuildEnemyList:
Func_080c1ffc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #616]
	sub	sp, #124
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	mov	r0, sp
	str	r3, [sp, #24]
	movs	r2, #0
	adds	r0, #96
	adds	r3, #64
	str	r2, [sp, #32]
	strb	r2, [r3, #0]
	str	r0, [sp, #28]
	ldr	r0, [pc, #596]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080c2034
	add	r0, sp, #32
	bl	sub_080c1afc
	adds	r5, r0, #0
.L_080c2034:
	movs	r1, #190
	lsls	r1, r1, #1
	cmp	r5, r1
	bcc.n	.L_080c203e
	movs	r5, #1
.L_080c203e:
	ldr	r2, [pc, #572]
	lsls	r3, r5, #4
	adds	r3, r3, r2
	adds	r4, r3, #0
	str	r3, [sp, #20]
	adds	r4, #2
	ldrb	r3, [r4, #4]
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L_080c2064
	ldr	r2, [sp, #20]
	adds	r2, #6
.L_080c2056:
	adds	r1, #1
	cmp	r1, #4
	bgt.n	.L_080c2064
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080c2056
.L_080c2064:
	cmp	r1, #5
	bne.n	.L_080c2070
	ldr	r2, [pc, #532]
	adds	r4, r2, #0
	str	r2, [sp, #20]
	adds	r4, #2
.L_080c2070:
	ldr	r0, [sp, #20]
	adds	r0, #1
	movs	r5, #0
	movs	r3, #6
	str	r0, [sp, #8]
	str	r5, [sp, #16]
	mov	fp, r3
	movs	r7, #0
	adds	r5, r4, #4
.L_080c2082:
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080c20aa
	ldr	r1, [sp, #8]
	ldrb	r0, [r1, r7]
	adds	r0, #8
	str	r4, [sp, #0]
	bl	sub_080c23c0
	negs	r3, r0
	orrs	r3, r0
	movs	r2, #2
	lsrs	r3, r3, #31
	subs	r3, r2, r3
	ldrb	r2, [r5, #0]
	muls	r3, r2
	mov	r2, fp
	subs	r2, r2, r3
	ldr	r4, [sp, #0]
	mov	fp, r2
.L_080c20aa:
	adds	r7, #1
	adds	r5, #1
	cmp	r7, #4
	bls.n	.L_080c2082
	mov	r3, sp
	ldr	r5, [sp, #20]
	adds	r3, #56
	adds	r5, #11
	adds	r4, #4
	movs	r0, #0
	str	r3, [sp, #12]
	movs	r7, #0
	add	r6, sp, #76
	mov	r9, r5
	mov	sl, r4
	mov	r8, r0
.L_080c20ca:
	mov	r1, sl
	ldrb	r2, [r1, #0]
	movs	r3, #1
	mov	r1, r9
	ldr	r0, [sp, #12]
	mov	r5, r8
	add	sl, r3
	ldrb	r3, [r1, #0]
	str	r2, [r5, r0]
	movs	r5, #1
	add	r9, r5
	subs	r5, r3, r2
	cmp	r5, #0
	ble.n	.L_080c2116
	ldr	r1, [sp, #8]
	ldrb	r0, [r1, r7]
	adds	r0, #8
	bl	sub_080c23c0
	negs	r1, r0
	orrs	r1, r0
	lsrs	r1, r1, #31
	movs	r3, #2
	subs	r1, r3, r1
	mov	r0, fp
	bl	sub_080022ec
	cmp	r0, r5
	bge.n	.L_080c2106
	adds	r5, r0, #0
.L_080c2106:
	bl	sub_08004458
	adds	r3, r5, #1
	muls	r3, r0
	mov	r2, r8
	lsrs	r3, r3, #16
	str	r3, [r2, r6]
	b.n	.L_080c211c
.L_080c2116:
	movs	r3, #0
	mov	r5, r8
	str	r3, [r5, r6]
.L_080c211c:
	movs	r0, #4
	adds	r7, #1
	add	r8, r0
	cmp	r7, #4
	bls.n	.L_080c20ca
	ldr	r4, [sp, #12]
	adds	r1, r6, #0
.L_080c212a:
	movs	r2, #0
	mov	r8, r2
	movs	r7, #0
	movs	r5, #0
.L_080c2132:
	ldr	r0, [sp, #8]
	ldrb	r3, [r0, r7]
	adds	r0, r3, #0
	ldr	r3, [r5, r6]
	adds	r0, #8
	cmp	r3, #0
	beq.n	.L_080c2178
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_080c23c0
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	movs	r3, #2
	subs	r2, r3, r2
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r2, fp
	ble.n	.L_080c2162
	movs	r3, #0
	str	r3, [r5, r6]
	b.n	.L_080c2178
.L_080c2162:
	ldr	r3, [r5, r4]
	adds	r3, #1
	str	r3, [r5, r4]
	ldr	r3, [r5, r1]
	subs	r3, #1
	str	r3, [r5, r1]
	mov	r3, fp
	subs	r3, r3, r2
	movs	r0, #1
	mov	fp, r3
	mov	r8, r0
.L_080c2178:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #4
	bls.n	.L_080c2132
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080c212a
	ldr	r5, [sp, #20]
	ldr	r2, [sp, #24]
	ldrb	r3, [r5, #0]
	adds	r2, #66
	strb	r3, [r2, #0]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080c219e
	cmp	r3, #1
	beq.n	.L_080c221a
	movs	r7, #0
	b.n	.L_080c22b0
.L_080c219e:
	add	r0, sp, #36
	mov	r8, r0
	movs	r7, #0
	mov	r3, r8
.L_080c21a6:
	stmia	r3!, {r7}
	adds	r7, #1
	cmp	r7, #4
	bls.n	.L_080c21a6
	movs	r7, #0
	mov	r6, r8
.L_080c21b2:
	bl	sub_08004458
	lsls	r5, r0, #2
	adds	r5, r5, r0
	bl	sub_08004458
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsrs	r5, r5, #16
	lsrs	r3, r3, #16
	lsls	r5, r5, #2
	lsls	r3, r3, #2
	ldr	r1, [r6, r5]
	ldr	r2, [r6, r3]
	adds	r7, #1
	str	r2, [r6, r5]
	str	r1, [r6, r3]
	cmp	r7, #9
	bls.n	.L_080c21b2
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #16]
	ldr	r6, [sp, #12]
	movs	r7, #0
	mov	r4, r8
	mov	ip, r1
	lsls	r0, r2, #1
.L_080c21e6:
	ldr	r2, [r4, #0]
	lsls	r1, r2, #2
	ldr	r3, [r6, r1]
	cmp	r3, #0
	ble.n	.L_080c2210
	mov	r5, ip
	ldrb	r3, [r5, r2]
	ldr	r2, [sp, #12]
	ldr	r5, [sp, #28]
	ldr	r1, [r2, r1]
	adds	r3, #8
	adds	r2, r0, r5
.L_080c21fe:
	strh	r3, [r2, #0]
	ldr	r5, [sp, #16]
	subs	r1, #1
	adds	r5, #1
	adds	r2, #2
	adds	r0, #2
	str	r5, [sp, #16]
	cmp	r1, #0
	bne.n	.L_080c21fe
.L_080c2210:
	adds	r7, #1
	adds	r4, #4
	cmp	r7, #4
	bls.n	.L_080c21e6
	b.n	.L_080c22b4
.L_080c221a:
	ldr	r1, [sp, #16]
	movs	r0, #36
	ldr	r2, [sp, #28]
	add	r0, sp
	lsls	r3, r1, #1
	ldr	r4, [sp, #12]
	mov	r8, r0
	adds	r6, r3, r2
.L_080c222a:
	movs	r5, #0
	movs	r7, #0
	adds	r1, r4, #0
	add	r2, sp, #36
.L_080c2232:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L_080c223c
	stmia	r2!, {r7}
	adds	r5, #1
.L_080c223c:
	adds	r7, #1
	cmp	r7, #4
	bls.n	.L_080c2232
	cmp	r5, #0
	beq.n	.L_080c22b4
	str	r4, [sp, #0]
	bl	sub_08004458
	adds	r3, r5, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	mov	r5, r8
	ldr	r2, [r5, r3]
	ldr	r0, [sp, #8]
	ldrb	r3, [r0, r2]
	adds	r3, #8
	strh	r3, [r6, #0]
	ldr	r1, [sp, #16]
	adds	r1, #1
	str	r1, [sp, #16]
	ldr	r4, [sp, #0]
	lsls	r2, r2, #2
	ldr	r3, [r4, r2]
	subs	r3, #1
	adds	r6, #2
	str	r3, [r4, r2]
	b.n	.L_080c222a
	.4byte 0x03001e74
	.4byte 0x00000173
	.4byte 0x080c5c38
	.2byte 0x5c48
	.2byte 0x080c
.L_080c2284:
	ldr	r5, [sp, #12]
	lsls	r3, r7, #2
	ldr	r2, [r5, r3]
	cmp	r2, #0
	ble.n	.L_080c22ae
	ldr	r0, [sp, #8]
	ldr	r5, [sp, #16]
	ldrb	r3, [r0, r7]
	ldr	r0, [sp, #28]
	adds	r1, r3, #0
	lsls	r3, r5, #1
	adds	r1, #8
	adds	r3, r3, r0
.L_080c229e:
	strh	r1, [r3, #0]
	ldr	r5, [sp, #16]
	subs	r2, #1
	adds	r5, #1
	adds	r3, #2
	str	r5, [sp, #16]
	cmp	r2, #0
	bne.n	.L_080c229e
.L_080c22ae:
	adds	r7, #1
.L_080c22b0:
	cmp	r7, #4
	bls.n	.L_080c2284
.L_080c22b4:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #28]
	ldr	r3, [pc, #20]
	lsls	r2, r0, #1
	strh	r3, [r2, r1]
	ldr	r5, [sp, #24]
	movs	r3, #6
	strh	r3, [r5, #60]
	ldr	r0, [sp, #24]
	movs	r2, #0
	strh	r2, [r0, #62]
	ldr	r5, [pc, #8]
	movs	r7, #128
	b.n	.L_080c22d8
	.4byte 0x00000000
	.2byte 0x0164
	.2byte 0x0300
.L_080c22d8:
	adds	r0, r7, #0
	bl	sub_08077008
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r7, #1
	bl	sub_080072f8
	cmp	r7, #133
	bls.n	.L_080c22d8
	ldr	r1, [sp, #28]
	ldrh	r3, [r1, #0]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080c234e
	movs	r2, #0
.L_080c22f8:
	ldr	r3, [sp, #28]
	adds	r6, r2, r3
	ldrh	r0, [r6, #0]
	movs	r1, #1
	bl	sub_080c1df4
	movs	r3, #128
	adds	r4, r0, #0
	lsls	r3, r3, #8
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080c231a
	ldrh	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	sub_080c1f50
	ldr	r4, [sp, #0]
.L_080c231a:
	adds	r5, r7, #0
	ldr	r2, [pc, #68]
	adds	r5, #128
	ldrh	r1, [r6, #0]
	ands	r2, r4
	adds	r0, r5, #0
	bl	sub_08077140
	adds	r0, r5, #0
	bl	sub_08077008
	ldr	r1, [sp, #32]
	cmp	r1, #0
	beq.n	.L_080c233c
	adds	r0, r5, #0
	bl	sub_080c1c54
.L_080c233c:
	adds	r7, #1
	cmp	r7, #5
	bhi.n	.L_080c234e
	lsls	r3, r7, #1
	ldr	r5, [sp, #28]
	adds	r2, r3, #0
	ldrh	r3, [r2, r5]
	cmp	r3, #0
	bne.n	.L_080c22f8
.L_080c234e:
	adds	r0, r7, #0
	add	sp, #124
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00007fff
