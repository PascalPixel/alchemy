.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08005268, 0x08005268
	.set sub_08015038, 0x08015038
	.set sub_08015048, 0x08015048
	.set sub_080150f8, 0x080150f8
	.set sub_08015108, 0x08015108
	.set sub_08015110, 0x08015110
	.set sub_080151e0, 0x080151e0
	.set sub_0808ba1c, 0x0808ba1c
	.set sub_080915ac, 0x080915ac
	.set sub_08092ba8, 0x08092ba8
	.set sub_08094154, 0x08094154
	.global UiText_OpenMessageAtObject
	.global Func_08092c40
	.thumb_func
UiText_OpenMessageAtObject:
Func_08092c40:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #804]
	ldr	r1, [r3, #0]
	sub	sp, #64
	str	r1, [sp, #32]
	ldr	r3, [r3, #48]
	movs	r2, #0
	adds	r6, r0, #0
	str	r3, [sp, #28]
	mov	sl, r2
	mov	r9, r2
	bl	sub_08092ba8
	movs	r2, #240
	lsls	r2, r2, #8
	movs	r3, #0
	mov	fp, r2
	movs	r1, #4
	str	r3, [sp, #20]
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	mov	r3, fp
	ldr	r1, [sp, #28]
	movs	r2, #236
	lsls	r2, r2, #1
	str	r0, [sp, #24]
	ands	r3, r6
	mov	fp, r3
	adds	r3, r1, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #748]
	ands	r6, r3
	movs	r4, #0
	adds	r0, r6, #0
	mov	r8, r1
	str	r4, [sp, #4]
	bl	sub_0808ba1c
	movs	r2, #250
	ldr	r1, [sp, #28]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	str	r6, [r3, #0]
	subs	r2, #40
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r5, #0
	movs	r7, #0
	ldr	r4, [sp, #4]
	cmp	r3, #0
	beq.n	.L_08092cb6
	b.n	.L_08092f3c
.L_08092cb6:
	cmp	r0, #0
	beq.n	.L_08092ce6
	subs	r2, #46
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #3
	bne.n	.L_08092cde
	add	r5, sp, #52
	adds	r1, r5, #0
	adds	r0, #8
	bl	sub_08005268
	ldr	r3, [r5, #0]
	asrs	r4, r3, #3
	ldr	r3, [r5, #4]
	asrs	r5, r3, #3
	movs	r7, #1
	subs	r5, #2
	b.n	.L_08092d38
.L_08092cde:
	add	r5, sp, #52
	adds	r1, r5, #0
	adds	r0, r6, #0
	b.n	.L_08092d24
.L_08092ce6:
	cmp	r6, #7
	bgt.n	.L_08092d38
	ldr	r3, [pc, #656]
	movs	r2, #250
	lsls	r2, r2, #1
	str	r6, [sp, #24]
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl	sub_0808ba1c
	movs	r2, #207
	ldr	r1, [sp, #28]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #3
	bne.n	.L_08092d1e
	add	r5, sp, #52
	adds	r1, r5, #0
	adds	r0, #8
	bl	sub_08005268
	ldr	r3, [r5, #0]
	asrs	r4, r3, #3
	ldr	r3, [r5, #4]
	movs	r7, #1
	b.n	.L_08092d36
.L_08092d1e:
	ldr	r0, [r5, #0]
	add	r5, sp, #52
	adds	r1, r5, #0
.L_08092d24:
	bl	sub_08094154
	mvns	r0, r0
	negs	r3, r0
	orrs	r3, r0
	lsrs	r7, r3, #31
	ldr	r3, [r5, #0]
	asrs	r4, r3, #3
	ldr	r3, [r5, #4]
.L_08092d36:
	asrs	r5, r3, #3
.L_08092d38:
	cmp	r7, #0
	bne.n	.L_08092d44
	movs	r3, #15
	str	r3, [sp, #48]
	movs	r3, #10
	b.n	.L_08092d94
.L_08092d44:
	movs	r3, #0
	add	r0, sp, #36
	str	r3, [sp, #48]
	str	r3, [sp, #44]
	add	r2, sp, #44
	add	r3, sp, #40
	str	r0, [sp, #0]
	add	r1, sp, #48
	mov	r0, r8
	str	r4, [sp, #4]
	bl	sub_08015110
	ldr	r3, [sp, #40]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r4, [sp, #4]
	asrs	r3, r3, #1
	subs	r3, r4, r3
	str	r3, [sp, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08092d7e
	ldr	r3, [sp, #36]
	subs	r3, r5, r3
	subs	r3, #1
	b.n	.L_08092d94
.L_08092d7e:
	mov	r1, fp
	lsrs	r3, r1, #15
	cmp	r3, #0
	bne.n	.L_08092d92
	cmp	r5, #8
	bgt.n	.L_08092d92
	ldr	r3, [sp, #36]
	subs	r3, r5, r3
	subs	r3, #1
	b.n	.L_08092d94
.L_08092d92:
	adds	r3, r5, #4
.L_08092d94:
	str	r3, [sp, #44]
	ldr	r2, [sp, #32]
	ldr	r1, [pc, #484]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08092da6
	movs	r2, #5
	str	r2, [sp, #12]
.L_08092da6:
	movs	r3, #128
	lsls	r3, r3, #5
	mov	r1, fp
	ands	r3, r1
	adds	r6, r4, #0
	cmp	r3, #0
	beq.n	.L_08092dc2
	ldr	r2, [sp, #12]
	subs	r3, r6, r2
	subs	r6, r3, #2
	cmp	r6, #0
	bge.n	.L_08092dfe
	movs	r6, #0
	b.n	.L_08092dfe
.L_08092dc2:
	movs	r3, #128
	lsls	r3, r3, #6
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08092dde
	ldr	r2, [sp, #12]
	adds	r6, #2
	adds	r3, r6, r2
	cmp	r3, #29
	ble.n	.L_08092dfe
	movs	r3, #29
	subs	r6, r3, r2
	b.n	.L_08092dfe
.L_08092dde:
	cmp	r6, #15
	bgt.n	.L_08092df0
	ldr	r1, [sp, #12]
	subs	r3, r6, r1
	subs	r6, r3, #2
	cmp	r6, #0
	bge.n	.L_08092dfe
	adds	r6, r4, #2
	b.n	.L_08092dfe
.L_08092df0:
	ldr	r2, [sp, #12]
	adds	r6, #2
	adds	r3, r6, r2
	cmp	r3, #29
	ble.n	.L_08092dfe
	subs	r3, r4, r2
	subs	r6, r3, #2
.L_08092dfe:
	ldr	r0, [sp, #24]
	bl	sub_080151e0
	movs	r3, #1
	negs	r3, r3
	adds	r7, r0, #0
	mov	sl, r3
	cmp	r7, sl
	beq.n	.L_08092e80
	mov	r3, sp
	movs	r1, #48
	movs	r2, #44
	add	r1, sp
	add	r2, sp
	adds	r3, #40
	add	r7, sp, #36
	mov	r0, r8
	mov	r9, r1
	mov	fp, r2
	str	r3, [sp, #8]
	str	r7, [sp, #0]
	bl	sub_08015110
	ldr	r2, [sp, #44]
	subs	r1, r2, #5
	mov	r8, sl
	str	r1, [sp, #16]
	cmp	r2, r5
	bgt.n	.L_08092e3e
	ldr	r3, [sp, #36]
	adds	r3, r2, r3
	str	r3, [sp, #16]
.L_08092e3e:
	ldr	r3, [sp, #16]
	cmp	r3, #0
	bge.n	.L_08092e4c
	ldr	r3, [sp, #36]
	adds	r3, r2, r3
	str	r3, [sp, #16]
	b.n	.L_08092e58
.L_08092e4c:
	ldr	r3, [sp, #16]
	adds	r3, #5
	cmp	r3, #19
	ble.n	.L_08092e58
	subs	r1, r2, #5
	str	r1, [sp, #16]
.L_08092e58:
	ldr	r3, [sp, #16]
	cmp	r2, r3
	bge.n	.L_08092ea2
	movs	r0, #1
	mov	r1, r9
	ldr	r3, [sp, #8]
	negs	r0, r0
	mov	r2, fp
	ldr	r5, [sp, #36]
	str	r7, [sp, #0]
	bl	sub_08015108
	ldr	r3, [sp, #36]
	movs	r1, #1
	subs	r5, r5, r3
	negs	r1, r1
	adds	r5, #1
	mov	r8, r1
	str	r5, [sp, #20]
	b.n	.L_08092ea2
.L_08092e80:
	ldr	r3, [sp, #44]
	cmp	r3, r5
	bge.n	.L_08092ea2
	add	r0, sp, #36
	add	r3, sp, #40
	str	r0, [sp, #0]
	add	r1, sp, #48
	mov	r0, r8
	add	r2, sp, #44
	ldr	r5, [sp, #36]
	bl	sub_08015108
	ldr	r3, [sp, #36]
	subs	r5, r5, r3
	adds	r5, #1
	str	r5, [sp, #20]
	mov	r8, r7
.L_08092ea2:
	cmp	r6, #0
	bge.n	.L_08092eaa
	movs	r6, #0
	b.n	.L_08092eb6
.L_08092eaa:
	ldr	r2, [sp, #12]
	adds	r3, r6, r2
	cmp	r3, #29
	ble.n	.L_08092eb6
	movs	r3, #29
	subs	r6, r3, r2
.L_08092eb6:
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #196]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08092ee6
	movs	r0, #8
	bl	sub_080030f8
	ldr	r3, [sp, #20]
	cmp	r3, #0
	beq.n	.L_08092edc
	ldr	r2, [sp, #44]
	adds	r2, r2, r3
	ldr	r1, [sp, #48]
	subs	r2, #1
	mov	r0, r8
	movs	r3, #18
	b.n	.L_08092f10
.L_08092edc:
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #44]
	mov	r0, r8
	movs	r3, #2
	b.n	.L_08092f10
.L_08092ee6:
	ldr	r0, [sp, #24]
	bl	sub_080915ac
	ldr	r1, [sp, #20]
	cmp	r1, #0
	beq.n	.L_08092f04
	ldr	r3, [sp, #20]
	ldr	r2, [sp, #44]
	adds	r2, r2, r3
	lsls	r3, r0, #16
	movs	r0, #17
	orrs	r3, r0
	ldr	r1, [sp, #48]
	subs	r2, #1
	b.n	.L_08092f0e
.L_08092f04:
	lsls	r3, r0, #16
	movs	r0, #1
	orrs	r3, r0
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #44]
.L_08092f0e:
	mov	r0, r8
.L_08092f10:
	bl	sub_08015038
	mov	sl, r0
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #100]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	ldr	r0, [sp, #24]
	movs	r1, #0
	adds	r2, r6, #0
	ldr	r3, [sp, #16]
	bl	sub_080150f8
	mov	r9, r0
	b.n	.L_08092f34
.L_08092f2e:
	movs	r0, #1
	bl	sub_080030f8
.L_08092f34:
	bl	sub_08015048
	cmp	r0, #0
	beq.n	.L_08092f2e
.L_08092f3c:
	ldr	r1, [sp, #28]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r3, r1, r2
	mov	r1, sl
	str	r1, [r3, #0]
	ldr	r2, [sp, #28]
	movs	r1, #254
	lsls	r1, r1, #1
	adds	r3, r2, r1
	mov	r2, r9
	str	r2, [r3, #0]
	ldr	r3, [sp, #28]
	subs	r1, #36
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	mov	r0, sl
	strh	r3, [r2, #0]
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x00000fff
	.4byte 0x02000240
	.4byte 0x00000ea4
