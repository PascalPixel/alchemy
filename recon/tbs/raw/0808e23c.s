.syntax unified
	.thumb
	.set sub_080072f0, 0x080072f0
	.set sub_08015040, 0x08015040
	.set sub_08015120, 0x08015120
	.set sub_08015140, 0x08015140
	.set sub_08077008, 0x08077008
	.set sub_08077018, 0x08077018
	.set sub_08077058, 0x08077058
	.set sub_080770c0, 0x080770c0
	.set sub_080770c8, 0x080770c8
	.set sub_080770d0, 0x080770d0
	.set sub_08077148, 0x08077148
	.set sub_0808ddec, 0x0808ddec
	.set sub_0808e14c, 0x0808e14c
	.set sub_080916b0, 0x080916b0
	.set sub_08091750, 0x08091750
	.set sub_08091d84, 0x08091d84
	.set sub_08092b94, 0x08092b94
	.set sub_08092f84, 0x08092f84
	.set sub_08096810, 0x08096810
	.set sub_08096fb0, 0x08096fb0
	.set sub_08097194, 0x08097194
	.global BattleCommand_ExecuteSelectedItem
	.global Func_0808e23c
	.thumb_func
BattleCommand_ExecuteSelectedItem:
Func_0808e23c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #568]
	movs	r2, #1
	sub	sp, #8
	mov	fp, r3
	negs	r2, r2
	str	r2, [sp, #0]
	mov	r2, fp
	ands	r2, r0
	movs	r3, #15
	asrs	r0, r0, #10
	ands	r0, r3
	mov	fp, r2
	mov	sl, r0
	str	r1, [sp, #4]
	bl	sub_08077148
	mov	r2, sl
	movs	r7, #0
	mov	r8, r0
	cmp	r2, #15
	bne.n	.L_0808e2be
	movs	r3, #0
	mov	sl, r3
	movs	r6, #0
	cmp	sl, r8
	bge.n	.L_0808e2de
	ldr	r3, [pc, #520]
	ldr	r0, [pc, #520]
	movs	r2, #252
	lsls	r2, r2, #1
	mov	r9, r0
	adds	r5, r3, r2
.L_0808e28a:
	ldrb	r0, [r5, #0]
	bl	sub_08077008
	movs	r4, #0
	adds	r0, #216
	movs	r1, #14
.L_0808e296:
	ldrh	r2, [r0, #0]
	mov	r3, r9
	ands	r3, r2
	adds	r0, #2
	cmp	r3, fp
	bne.n	.L_0808e2a4
	adds	r4, #1
.L_0808e2a4:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L_0808e296
	cmp	r7, r4
	bge.n	.L_0808e2b4
	ldrb	r3, [r5, #0]
	adds	r7, r4, #0
	mov	sl, r3
.L_0808e2b4:
	adds	r6, #1
	adds	r5, #1
	cmp	r6, r8
	blt.n	.L_0808e28a
	b.n	.L_0808e2de
.L_0808e2be:
	mov	r0, sl
	bl	sub_08077008
	ldr	r4, [pc, #452]
	adds	r0, #216
	movs	r1, #14
.L_0808e2ca:
	ldrh	r2, [r0, #0]
	adds	r3, r4, #0
	ands	r3, r2
	adds	r0, #2
	cmp	r3, fp
	bne.n	.L_0808e2d8
	adds	r7, #1
.L_0808e2d8:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L_0808e2ca
.L_0808e2de:
	cmp	r7, #0
	bne.n	.L_0808e2f0
	ldr	r0, [pc, #428]
	movs	r1, #1
	bl	sub_08015040
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0808e470
.L_0808e2f0:
	mov	r0, fp
	bl	sub_0808e14c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0808e376
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq.n	.L_0808e376
	ldr	r0, [pc, #400]
	bl	sub_080770d0
	movs	r0, #161
	lsls	r0, r0, #1
	bl	sub_080770d0
	ldrh	r2, [r6, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0808e334
	mov	r0, sl
	movs	r1, #1
	bl	sub_08015120
	mov	r0, fp
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #360]
	movs	r1, #1
	bl	sub_08015040
.L_0808e334:
	movs	r0, #128
	ldr	r3, [r6, #8]
	lsls	r0, r0, #9
	cmp	r3, r0
	bge.n	.L_0808e366
	ldr	r3, [pc, #328]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_0808ddec
	adds	r5, r0, #0
	bl	sub_080916b0
	ldr	r0, [r6, #8]
	bl	sub_08092b94
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08092f84
	bl	sub_08091750
	b.n	.L_0808e370
.L_0808e366:
	mov	r0, fp
	mov	r1, sl
	ldr	r2, [sp, #4]
	bl	sub_080072f0
.L_0808e370:
	movs	r3, #0
	str	r3, [sp, #0]
	b.n	.L_0808e448
.L_0808e376:
	movs	r7, #161
	ldr	r0, [pc, #280]
	lsls	r7, r7, #1
	bl	sub_080770d0
	adds	r0, r7, #0
	bl	sub_080770c8
	mov	r0, fp
	bl	sub_08077018
	ldr	r3, [pc, #268]
	ldrh	r5, [r0, #40]
	ldr	r3, [r3, #0]
	mov	r8, r3
	cmp	r5, #0
	beq.n	.L_0808e448
	ldr	r0, [pc, #260]
	bl	sub_080770c8
	adds	r0, r7, #0
	bl	sub_080770d0
	cmp	r5, #149
	bne.n	.L_0808e3fc
	movs	r0, #162
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_0808e3fc
	mov	r0, fp
	movs	r1, #2
	bl	sub_08015120
	movs	r1, #13
	ldr	r0, [pc, #228]
	bl	sub_08015040
	movs	r0, #1
	bl	sub_08091d84
	adds	r6, r0, #0
	bl	sub_08015140
	movs	r0, #0
	cmp	r6, #0
	bne.n	.L_0808e470
	ldr	r1, [pc, #176]
	movs	r0, #144
	lsls	r0, r0, #2
	adds	r3, r1, r0
	ldrh	r2, [r3, #0]
	subs	r0, #128
	adds	r3, r1, r0
	strh	r2, [r3, #0]
	ldr	r2, [pc, #192]
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	adds	r0, #2
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	movs	r2, #184
	lsls	r2, r2, #1
	ldr	r3, [pc, #180]
	add	r2, r8
	strh	r3, [r2, #0]
.L_0808e3fc:
	mov	r0, sl
	movs	r1, #1
	bl	sub_08015120
	mov	r0, fp
	movs	r1, #2
	bl	sub_08015120
	ldr	r6, [pc, #160]
	ldr	r0, [pc, #136]
	movs	r1, #1
	bl	sub_08015040
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_08096fb0
	add	r6, r8
	movs	r2, #0
	movs	r5, #1
	mov	r8, r2
	strb	r5, [r6, #0]
	bl	sub_08096810
	mov	r3, r8
	strb	r3, [r6, #0]
	bl	sub_08097194
	mov	r0, fp
	bl	sub_08077018
	ldrb	r3, [r0, #12]
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L_0808e448
	ldr	r0, [pc, #80]
	bl	sub_080770c8
.L_0808e448:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808e45c
	ldr	r0, [pc, #56]
	movs	r1, #1
	bl	sub_08015040
.L_0808e45c:
	ldr	r0, [pc, #52]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0808e46e
	mov	r0, sl
	ldr	r1, [sp, #4]
	bl	sub_08077058
.L_0808e46e:
	ldr	r0, [sp, #0]
.L_0808e470:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x02000240
	.4byte 0x000001ff
	.4byte 0x00000927
	.4byte 0x00000143
	.4byte 0x0000091c
	.4byte 0x03001ebc
	.4byte 0x00000145
	.4byte 0x00000924
	.4byte 0x00000242
	.4byte 0x000003e7
	.4byte 0x00000cc6
