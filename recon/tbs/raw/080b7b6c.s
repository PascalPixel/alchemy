.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080072f0, 0x080072f0
	.set sub_08009030, 0x08009030
	.set sub_08009048, 0x08009048
	.set sub_08009070, 0x08009070
	.set sub_080b770c, 0x080b770c
	.set sub_080b78e4, 0x080b78e4
	.set sub_080b7aac, 0x080b7aac
	.set sub_080b7b30, 0x080b7b30
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_08185000, 0x08185000
	.global BattleActor_SpawnObjectsForList
	.global Func_080b7b6c
	.thumb_func
BattleActor_SpawnObjectsForList:
Func_080b7b6c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r1, [sp, #20]
	str	r0, [sp, #24]
	movs	r1, #0
	mov	fp, r1
.L_080b7b84:
	ldr	r0, [sp, #24]
	mov	r1, fp
	bl	sub_080b770c
	cmp	r0, #0
	bne.n	.L_080b7ba0
	mov	r0, fp
	mov	r2, fp
	adds	r0, #120
	cmp	r2, #7
	bgt.n	.L_080b7b9c
	mov	r0, fp
.L_080b7b9c:
	bl	sub_080b7b30
.L_080b7ba0:
	movs	r3, #1
	add	fp, r3
	mov	r1, fp
	cmp	r1, #13
	ble.n	.L_080b7b84
	ldr	r3, [pc, #144]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b7bb8
	movs	r0, #1
	bl	sub_080030f8
.L_080b7bb8:
	ldr	r3, [sp, #24]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, #255
	bne.n	.L_080b7bca
	b.n	.L_080b7d62
.L_080b7bca:
	ldr	r1, [sp, #24]
	str	r1, [sp, #8]
.L_080b7bce:
	ldr	r2, [sp, #16]
	cmp	r2, #254
	bne.n	.L_080b7bd6
	b.n	.L_080b7d46
.L_080b7bd6:
	adds	r0, r2, #0
	bl	sub_080b7dd0
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_080b7be4
	b.n	.L_080b7d46
.L_080b7be4:
	ldr	r0, [sp, #16]
	adds	r1, r7, #0
	bl	sub_080b78e4
	ldr	r3, [r7, #0]
	mov	r8, r3
	cmp	r3, #0
	bne.n	.L_080b7bf6
	b.n	.L_080b7d46
.L_080b7bf6:
	mov	r1, r8
	adds	r1, #84
	ldrb	r2, [r1, #0]
	str	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_080b7c04
	b.n	.L_080b7d46
.L_080b7c04:
	ldrh	r2, [r7, #4]
	ldr	r3, [pc, #48]
	ands	r3, r2
	movs	r2, #238
	lsls	r2, r2, #1
	cmp	r3, r2
	beq.n	.L_080b7c18
	adds	r2, #7
	cmp	r3, r2
	bne.n	.L_080b7cb4
.L_080b7c18:
	ldr	r3, [pc, #36]
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #24]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	r9, r2
	movs	r3, #8
	add	r3, r9
	mov	sl, r3
	movs	r3, #2
	strb	r3, [r1, #0]
	mov	r2, r8
	ldrh	r5, [r7, #4]
	mov	r1, sl
	b.n	.L_080b7c44
	movs	r0, r0
	.4byte 0x00000fff
	.4byte 0x03001a10
	.2byte 0x1e68
	.2byte 0x0300
.L_080b7c44:
	str	r1, [r2, #80]
	ldr	r3, [pc, #376]
	movs	r1, #16
	mov	r0, sl
	bl	sub_080072f0
	adds	r0, r5, #0
	bl	sub_08009030
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080b7c82
	ldr	r0, [r6, #24]
	ldr	r1, [r7, #24]
	ldr	r3, [pc, #352]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r6, #24]
	adds	r0, r5, #0
	bl	sub_08185000
	ldrb	r3, [r0, #9]
	mov	r1, r8
	lsrs	r3, r3, #1
	mov	r2, sl
	strh	r3, [r1, #32]
	movs	r3, #12
	add	r3, r9
	str	r6, [r2, #0]
	mov	sl, r3
.L_080b7c82:
	add	r1, sp, #12
	adds	r3, r6, #0
	ldrb	r1, [r1, #0]
	ldr	r2, [pc, #316]
	adds	r3, #38
	strb	r1, [r3, #0]
	adds	r0, r5, r2
	bl	sub_08009030
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080b7caa
	ldr	r0, [r6, #24]
	ldr	r1, [r7, #24]
	ldr	r3, [pc, #292]
	mov	ip, pc
	bx	r3
	mov	r1, sl
	str	r0, [r6, #24]
	str	r6, [r1, #0]
.L_080b7caa:
	add	r2, sp, #12
	adds	r3, r6, #0
	ldrb	r2, [r2, #0]
	adds	r3, #38
	b.n	.L_080b7d3e
.L_080b7cb4:
	ldrh	r0, [r7, #4]
	str	r1, [sp, #4]
	bl	sub_08009030
	adds	r6, r0, #0
	ldr	r1, [sp, #4]
	cmp	r6, #0
	beq.n	.L_080b7d40
	movs	r4, #1
	strb	r4, [r1, #0]
	mov	r3, r8
	str	r6, [r3, #80]
	ldr	r0, [r6, #24]
	ldr	r1, [r7, #24]
	ldr	r3, [pc, #240]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r6, #24]
	ldr	r5, [r6, #40]
	ldr	r3, [r7, #20]
	strb	r4, [r5, #6]
	strb	r3, [r5, #5]
	ldrh	r5, [r7, #6]
	cmp	r5, #0
	beq.n	.L_080b7cf8
	adds	r1, r5, #0
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl	sub_08009048
	ldr	r4, [sp, #0]
	adds	r5, r0, #0
	strb	r4, [r5, #6]
.L_080b7cf8:
	ldrh	r5, [r7, #8]
	cmp	r5, #0
	beq.n	.L_080b7d14
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_08009048
	adds	r5, r0, #0
	str	r5, [r7, #32]
	movs	r1, #0
	bl	sub_08009070
	movs	r3, #3
	strb	r3, [r5, #6]
.L_080b7d14:
	ldrh	r5, [r7, #10]
	cmp	r5, #0
	beq.n	.L_080b7d40
	adds	r3, r6, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	cmp	r3, #32
	bne.n	.L_080b7d26
	ldr	r5, [pc, #164]
.L_080b7d26:
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_08009048
	add	r1, sp, #12
	ldrb	r1, [r1, #0]
	adds	r3, r6, #0
	adds	r5, r0, #0
	adds	r3, #38
	adds	r2, r1, #0
	str	r5, [r7, #36]
	strb	r1, [r5, #6]
.L_080b7d3e:
	strb	r2, [r3, #0]
.L_080b7d40:
	ldr	r0, [sp, #16]
	bl	sub_080b7aac
.L_080b7d46:
	ldr	r3, [sp, #8]
	movs	r1, #1
	add	fp, r1
	adds	r3, #2
	mov	r2, fp
	str	r3, [sp, #8]
	cmp	r2, #13
	bgt.n	.L_080b7d62
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [sp, #16]
	cmp	r3, #255
	beq.n	.L_080b7d62
	b.n	.L_080b7bce
.L_080b7d62:
	ldr	r2, [sp, #20]
	cmp	r2, #0
	beq.n	.L_080b7dae
	ldr	r2, [sp, #24]
	movs	r1, #0
	ldrsh	r5, [r2, r1]
	movs	r3, #0
	mov	fp, r3
	cmp	r5, #255
	beq.n	.L_080b7dae
.L_080b7d76:
	ldr	r1, [sp, #24]
	ldrsh	r3, [r3, r1]
	cmp	r3, #254
	beq.n	.L_080b7d9a
	adds	r0, r5, #0
	bl	sub_080b7dd0
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_080b7d9a
	ldr	r7, [r7, #0]
	mov	r8, r7
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_080b7d9a
	adds	r0, r5, #0
	bl	sub_080b7aac
.L_080b7d9a:
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #13
	bgt.n	.L_080b7dae
	ldr	r1, [sp, #24]
	lsls	r3, r2, #1
	ldrsh	r5, [r3, r1]
	cmp	r5, #255
	bne.n	.L_080b7d76
.L_080b7dae:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03000164
	.4byte 0x03000118
	.4byte 0x00002001
	.4byte 0x000001ff
