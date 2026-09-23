.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d00, 0x08014d00
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_08039430, 0x08039430
	.set sub_08039454, 0x08039454
	.set sub_0803a7ac, 0x0803a7ac
	.set sub_0803b880, 0x0803b880
	.set sub_0803dd98, 0x0803dd98
	.set sub_0803df00, 0x0803df00
	.set sub_0803e5a8, 0x0803e5a8
	.set sub_0803e6d8, 0x0803e6d8
	.set sub_0803e7ac, 0x0803e7ac
	.set sub_0803e998, 0x0803e998
	.set sub_08041c54, 0x08041c54
	.set sub_08041fa4, 0x08041fa4
	.set sub_08042010, 0x08042010
	.set sub_08042314, 0x08042314
	.set sub_08044460, 0x08044460
	.set sub_08044f88, 0x08044f88
	.set sub_08045018, 0x08045018
	.set sub_08045048, 0x08045048
	.set sub_080450ac, 0x080450ac
	.set sub_080450fc, 0x080450fc
	.set sub_080ad040, 0x080ad040
	.set sub_080c8648, 0x080c8648
	.set sub_080f8028, 0x080f8028
	.set sub_08108030, 0x08108030
	.set sub_08108038, 0x08108038
	.set sub_08108040, 0x08108040
	.set sub_08108048, 0x08108048
	.set sub_081c0010, 0x081c0010
	.global Func_0803f82c
	.thumb_func
Func_0803f82c:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #72]
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #158
	adds	r2, r3, r1
	strh	r0, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #1
	strh	r2, [r3, #0]
	bl	sub_0803dd98
	bl	sub_0803e7ac
	movs	r1, #5
	movs	r0, #0
	bl	sub_0803e5a8
	bl	sub_0803df00
	movs	r0, #1
	bl	sub_0803e998
	adds	r5, r0, #0
	bl	sub_0803e6d8
	adds	r0, r5, #0
	pop	{r5, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #20
	ldr	r6, [r3, #108]
	ldr	r2, [pc, #124]
	movs	r3, #8
	str	r3, [sp, #16]
	str	r3, [sp, #12]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_080c8648
	ldr	r3, [pc, #96]
	adds	r5, r0, #0
	adds	r5, r5, r3
	add	r0, sp, #4
	add	r1, sp, #16
	add	r2, sp, #12
	add	r3, sp, #8
	str	r0, [sp, #0]
	adds	r0, r5, #0
	bl	sub_0803b880
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #10
	subs	r0, r0, r2
	subs	r1, r1, r3
	movs	r4, #2
	asrs	r1, r1, #1
	asrs	r0, r0, #1
	str	r1, [sp, #12]
	str	r4, [sp, #0]
	str	r0, [sp, #16]
	bl	sub_08039260
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r1, r0, #0
	adds	r3, r6, r2
	str	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08041fa4
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r2, r6, r3
	movs	r1, #144
	movs	r3, #90
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl	sub_080145a8
	add	sp, #20
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x00000e58
	.2byte 0xf901
	.2byte 0x0803
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r0, #136
	lsls	r0, r0, #2
	adds	r2, r1, r0
	ldrh	r3, [r2, #0]
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r3, r0
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0803f934
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl	sub_0803939c
	ldr	r0, [pc, #8]
	bl	sub_08014644
.L_0803f934:
	pop	{pc}
	movs	r0, r0
	.2byte 0xf901
	.2byte 0x0803
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	cmp	r0, #0
	beq.n	.L_0803f962
	ldrh	r3, [r0, #22]
	cmp	r3, #0
	beq.n	.L_0803f962
	movs	r1, #2
	bl	sub_0803939c
	ldr	r0, [pc, #4]
	bl	sub_08014644
.L_0803f962:
	pop	{pc}
	.2byte 0xf901
	.2byte 0x0803
	push	{lr}
	adds	r0, r1, #0
	bl	sub_080ad040
	movs	r0, #0
	pop	{pc}
.L_0803f974:
	push	{r5, lr}
	movs	r5, #0
	ldrsh	r4, [r0, r5]
	muls	r1, r4
	asrs	r4, r1, #16
	movs	r5, #2
	ldrsh	r1, [r0, r5]
	muls	r2, r1
	asrs	r1, r2, #16
	movs	r5, #4
	ldrsh	r2, [r0, r5]
	muls	r3, r2
	asrs	r3, r3, #16
	cmp	r4, #0
	bge.n	.L_0803f994
	movs	r4, #0
.L_0803f994:
	cmp	r4, #31
	ble.n	.L_0803f99a
	movs	r4, #31
.L_0803f99a:
	cmp	r1, #0
	bge.n	.L_0803f9a0
	movs	r1, #0
.L_0803f9a0:
	cmp	r1, #31
	ble.n	.L_0803f9a6
	movs	r1, #31
.L_0803f9a6:
	cmp	r3, #0
	bge.n	.L_0803f9ac
	movs	r3, #0
.L_0803f9ac:
	cmp	r3, #31
	ble.n	.L_0803f9b2
	movs	r3, #31
.L_0803f9b2:
	lsls	r2, r1, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	orrs	r4, r3
	lsls	r0, r4, #16
	lsrs	r0, r0, #16
	pop	{r5, pc}
.L_0803f9c0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r1, #0
	adds	r0, #12
	movs	r1, #24
	sub	sp, #8
	bl	sub_08002064
	adds	r5, r0, #0
	ldr	r2, [pc, #336]
	lsls	r5, r5, #18
	asrs	r5, r5, #16
	adds	r0, r5, #0
	movs	r1, #96
	mov	r8, r2
	bl	sub_08002064
	lsls	r0, r0, #16
	mov	r2, r8
	asrs	r0, r0, #16
	ldrb	r3, [r2, r0]
	subs	r6, #7
	lsls	r6, r6, #16
	asrs	r6, r6, #16
	adds	r3, r3, r6
	adds	r0, r5, #0
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	movs	r1, #96
	adds	r0, #32
	mov	sl, r3
	bl	sub_08002064
	mov	r2, r8
	ldrb	r3, [r2, r0]
	adds	r5, #64
	adds	r3, r3, r6
	lsls	r3, r3, #16
	adds	r0, r5, #0
	movs	r1, #96
	asrs	r7, r3, #16
	bl	sub_08002064
	mov	r2, r8
	ldrb	r3, [r2, r0]
	mov	r2, sl
	adds	r3, r3, r6
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r2, #0
	bge.n	.L_0803fa30
	movs	r2, #0
	mov	sl, r2
.L_0803fa30:
	mov	r2, sl
	cmp	r2, #31
	ble.n	.L_0803fa3a
	movs	r2, #31
	mov	sl, r2
.L_0803fa3a:
	cmp	r7, #0
	bge.n	.L_0803fa40
	movs	r7, #0
.L_0803fa40:
	cmp	r7, #31
	ble.n	.L_0803fa46
	movs	r7, #31
.L_0803fa46:
	cmp	r3, #0
	bge.n	.L_0803fa4c
	movs	r3, #0
.L_0803fa4c:
	cmp	r3, #31
	ble.n	.L_0803fa52
	movs	r3, #31
.L_0803fa52:
	mov	r5, sp
	mov	r2, sl
	strh	r2, [r5, #0]
	strh	r7, [r5, #2]
	strh	r3, [r5, #4]
	movs	r3, #238
	lsls	r3, r3, #8
	adds	r3, #238
	movs	r2, #204
	mov	sl, r3
	lsls	r2, r2, #8
	adds	r2, #204
	ldr	r3, [pc, #196]
	adds	r0, r5, #0
	mov	r1, sl
	mov	r9, r2
	bl	.L_0803f974
	ldr	r3, [pc, #188]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	strh	r0, [r3, #0]
	movs	r1, #213
	movs	r6, #187
	lsls	r6, r6, #8
	adds	r6, #187
	lsls	r1, r1, #8
	mov	r3, sl
	adds	r1, #85
	adds	r0, r5, #0
	adds	r2, r6, #0
	bl	.L_0803f974
	ldr	r3, [pc, #160]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	strh	r0, [r3, #0]
	movs	r3, #170
	lsls	r3, r3, #8
	adds	r3, #170
	mov	r8, r3
	adds	r0, r5, #0
	mov	r3, r9
	adds	r1, r6, #0
	mov	r2, r8
	bl	.L_0803f974
	ldr	r3, [pc, #136]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	strh	r0, [r3, #0]
	movs	r1, #162
	movs	r2, #153
	lsls	r1, r1, #8
	lsls	r2, r2, #8
	mov	r3, r8
	adds	r1, #33
	adds	r2, #153
	adds	r0, r5, #0
	bl	.L_0803f974
	ldr	r3, [pc, #112]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	strh	r0, [r3, #0]
	movs	r1, #132
	movs	r2, #221
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	adds	r1, #136
	adds	r2, #221
	ldr	r3, [pc, #96]
	adds	r0, r5, #0
	bl	.L_0803f974
	ldr	r3, [pc, #92]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	strh	r0, [r3, #0]
	movs	r1, #145
	lsls	r1, r1, #9
	adds	r1, #33
	ldr	r3, [pc, #84]
	adds	r0, r5, #0
	mov	r2, sl
	bl	.L_0803f974
	ldr	r3, [pc, #76]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	strh	r0, [r3, #0]
	ldr	r1, [pc, #72]
	movs	r2, #128
	lsls	r2, r2, #9
	ldr	r3, [pc, #72]
	adds	r0, r5, #0
	bl	.L_0803f974
	ldr	r3, [pc, #68]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	strh	r0, [r3, #0]
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0805ea1c
	.4byte 0x00011110
	.4byte 0x050001e8
	.4byte 0x050001ea
	.4byte 0x050001ec
	.4byte 0x050001ee
	.4byte 0x00013333
	.4byte 0x050001f0
	.4byte 0x00015555
	.4byte 0x050001f2
	.4byte 0x00013bbb
	.4byte 0x00017777
	.2byte 0x01f4
	.2byte 0x0500
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r5, [r3, #0]
	movs	r1, #160
	movs	r2, #160
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r1, #116
	adds	r2, #164
	adds	r3, r5, r1
	adds	r0, r5, r2
	ldrh	r6, [r3, #0]
	bl	sub_08108030
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #180
	adds	r0, r5, r3
	bl	sub_08108038
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #196
	adds	r0, r5, r1
	bl	sub_08108038
	cmp	r6, #0
	bne.n	.L_0803fbb0
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #124
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	movs	r3, #7
	ands	r3, r2
	ldr	r2, [pc, #136]
	adds	r0, r3, r2
	b.n	.L_0803fbb2
.L_0803fbb0:
	ldr	r0, [pc, #128]
.L_0803fbb2:
	bl	sub_08013300
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #180
	adds	r3, r5, r1
	ldr	r3, [r3, #0]
	movs	r1, #128
	adds	r2, r0, #0
	lsls	r1, r1, #1
	ldrb	r0, [r3, #14]
	bl	sub_080142d4
	cmp	r6, #1
	bne.n	.L_0803fbe4
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #124
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	movs	r3, #7
	ands	r3, r2
	ldr	r2, [pc, #84]
	adds	r0, r3, r2
	b.n	.L_0803fbe6
.L_0803fbe4:
	ldr	r0, [pc, #76]
.L_0803fbe6:
	bl	sub_08013300
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #196
	adds	r3, r5, r1
	ldr	r3, [r3, #0]
	movs	r1, #128
	adds	r2, r0, #0
	lsls	r1, r1, #1
	ldrb	r0, [r3, #14]
	bl	sub_080142d4
	cmp	r6, #1
	ble.n	.L_0803fc24
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #148
	adds	r2, r6, r1
	ldrsb	r2, [r5, r2]
	lsls	r3, r6, #1
	adds	r3, r3, r6
	adds	r3, r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	lsls	r3, r3, #2
	adds	r2, #212
	adds	r3, r3, r2
	ldr	r0, [r5, r3]
	bl	sub_080450fc
.L_0803fc24:
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #124
	adds	r2, r5, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x01cd
	.2byte 0x0000
.L_0803fc38:
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #208
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r4, #0
	ldr	r2, [pc, #176]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #172]
	movs	r0, #147
	lsls	r0, r0, #1
	adds	r0, #255
	adds	r3, r1, r0
	movs	r0, #160
	ldrb	r2, [r3, #0]
	lsls	r0, r0, #3
	adds	r0, #148
	adds	r3, r4, r0
	strb	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	movs	r0, #128
	adds	r3, #153
	lsls	r0, r0, #2
	adds	r2, r4, r3
	adds	r0, #38
	movs	r3, #24
	strb	r3, [r2, #0]
	adds	r3, r1, r0
	movs	r0, #160
	ldrb	r2, [r3, #0]
	lsls	r0, r0, #3
	adds	r0, #149
	adds	r3, r4, r0
	strb	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #154
	movs	r0, #139
	adds	r2, r4, r3
	lsls	r0, r0, #2
	movs	r3, #15
	strb	r3, [r2, #0]
	adds	r3, r1, r0
	movs	r0, #160
	ldrb	r2, [r3, #0]
	lsls	r0, r0, #3
	adds	r0, #150
	adds	r3, r4, r0
	strb	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	movs	r0, #128
	adds	r3, #155
	lsls	r0, r0, #2
	adds	r2, r4, r3
	adds	r0, #42
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r3, r1, r0
	movs	r0, #147
	ldrb	r2, [r3, #0]
	lsls	r0, r0, #3
	adds	r0, #255
	adds	r3, r4, r0
	strb	r2, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #156
	adds	r3, r4, r2
	movs	r0, #2
	strb	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #74
	adds	r1, r1, r3
	ldrb	r2, [r1, #0]
	movs	r1, #179
	lsls	r1, r1, #3
	adds	r3, r4, r1
	strb	r2, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #157
	adds	r4, r4, r2
	movs	r1, #144
	strb	r0, [r4, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	movs	r0, r0
	.4byte 0x8500018a
	.4byte 0x02000240
	.2byte 0xfb61
	.2byte 0x0803
.L_0803fd14:
	push	{lr}
	ldr	r0, [pc, #12]
	bl	sub_08014644
	movs	r0, #208
	bl	sub_0801314c
	pop	{pc}
	.2byte 0xfb61
	.2byte 0x0803
.L_0803fd28:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	sub	sp, #8
	movs	r5, #2
	mov	r9, r3
	movs	r1, #5
	movs	r2, #27
	movs	r3, #14
	movs	r0, #1
	str	r5, [sp, #0]
	bl	sub_08039260
	movs	r1, #0
	movs	r2, #2
	movs	r3, #26
	str	r5, [sp, #0]
	mov	fp, r0
	bl	sub_08041c54
	movs	r3, #4
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	movs	r2, #4
	movs	r3, #26
	bl	sub_08041c54
	movs	r3, #7
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	movs	r2, #7
	movs	r3, #26
	bl	sub_08041c54
	movs	r3, #10
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	movs	r2, #10
	movs	r3, #26
	bl	sub_08041c54
	ldr	r5, [pc, #804]
	mov	r1, fp
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #0
	adds	r5, #1
	bl	sub_08042010
	adds	r0, r5, #0
	mov	r1, fp
	movs	r2, #8
	movs	r3, #16
	bl	sub_08042010
	ldr	r5, [pc, #780]
	mov	r1, fp
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #32
	adds	r5, #1
	bl	sub_08042010
	adds	r0, r5, #0
	mov	r1, fp
	movs	r2, #32
	movs	r3, #40
	bl	sub_08042010
	ldr	r0, [pc, #756]
	mov	r1, fp
	movs	r2, #8
	movs	r3, #64
	bl	sub_08042010
	mov	r1, fp
	movs	r2, #8
	movs	r3, #88
	ldr	r0, [pc, #744]
	bl	sub_08042010
	bl	sub_08044460
	movs	r1, #0
	str	r1, [sp, #0]
	movs	r1, #128
	mov	r2, fp
	lsls	r1, r1, #23
	movs	r3, #0
	bl	sub_08042314
	adds	r4, r0, #0
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, r9
	str	r4, [r0, #0]
	mov	r1, fp
	movs	r2, #12
	ldrsh	r3, [r1, r2]
	lsls	r6, r3, #3
	movs	r2, #14
	ldrsh	r3, [r1, r2]
	adds	r1, r6, #0
	lsls	r3, r3, #3
	adds	r7, r3, #0
	adds	r7, #12
	adds	r2, r7, #0
	bl	sub_08108048
	bl	sub_080143ac
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L_0803feec
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #664]
	ldr	r1, [pc, #664]
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [pc, #656]
	adds	r0, r6, #0
	bl	sub_080142d4
	ldr	r3, [pc, #652]
	movs	r7, #0
	mov	r8, r3
	mov	r1, r8
	mov	r2, fp
	movs	r3, #134
	adds	r0, r6, #0
	str	r7, [sp, #0]
	bl	sub_08042314
	adds	r4, r0, #0
	ldrb	r2, [r4, #25]
	movs	r5, #15
	movs	r1, #224
	mov	sl, r1
	adds	r3, r5, #0
	ands	r3, r2
	mov	r2, sl
	orrs	r3, r2
	strb	r3, [r4, #25]
	mov	r1, r8
	mov	r2, fp
	movs	r3, #166
	adds	r0, r6, #0
	str	r7, [sp, #0]
	bl	sub_08042314
	adds	r4, r0, #0
	ldrh	r1, [r4, #24]
	movs	r3, #192
	lsls	r2, r1, #22
	lsrs	r2, r2, #22
	lsls	r3, r3, #2
	adds	r3, #255
	adds	r2, #4
	ands	r2, r3
	ldr	r3, [pc, #584]
	movs	r7, #16
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #24]
	mov	r1, sl
	ldrb	r3, [r4, #25]
	mov	r2, fp
	ands	r5, r3
	orrs	r5, r1
	strb	r5, [r4, #25]
	mov	r1, r8
	movs	r3, #134
	adds	r0, r6, #0
	str	r7, [sp, #0]
	bl	sub_08042314
	adds	r4, r0, #0
	ldrb	r3, [r4, #25]
	movs	r5, #240
	orrs	r3, r5
	strb	r3, [r4, #25]
	mov	r1, r8
	mov	r2, fp
	movs	r3, #166
	adds	r0, r6, #0
	str	r7, [sp, #0]
	bl	sub_08042314
	adds	r4, r0, #0
	ldrh	r2, [r4, #24]
	movs	r1, #192
	lsls	r3, r2, #22
	lsrs	r3, r3, #22
	lsls	r1, r1, #2
	adds	r1, #255
	adds	r3, #4
	ands	r3, r1
	ldr	r1, [pc, #508]
	ands	r1, r2
	orrs	r1, r3
	str	r1, [sp, #4]
	add	r2, sp, #4
	ldrh	r2, [r2, #0]
	strh	r2, [r4, #24]
	ldrb	r3, [r4, #25]
	orrs	r3, r5
	strb	r3, [r4, #25]
.L_0803feec:
	bl	sub_080143ac
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L_0803ff6a
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	bl	sub_080142d4
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r6, #0
	mov	r2, fp
	str	r3, [sp, #0]
	bl	sub_08042314
	adds	r4, r0, #0
	ldrb	r3, [r4, #21]
	movs	r5, #160
	lsls	r5, r5, #3
	movs	r2, #32
	adds	r5, #180
	orrs	r3, r2
	add	r5, r9
	strb	r3, [r4, #21]
	str	r4, [r5, #0]
	mov	r2, fp
	movs	r1, #12
	ldrsh	r3, [r2, r1]
	lsls	r3, r3, #3
	adds	r6, r3, #0
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #148
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r6, #140
	lsls	r0, r3, #4
	subs	r0, r0, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #153
	add	r3, r9
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	lsls	r0, r0, #2
	bl	sub_08002054
	mov	r2, fp
	movs	r1, #14
	ldrsh	r3, [r2, r1]
	adds	r6, r6, r0
	lsls	r3, r3, #3
	adds	r7, r3, #4
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_08108048
.L_0803ff6a:
	bl	sub_080143ac
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L_0803ffea
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	bl	sub_080142d4
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r6, #0
	mov	r2, fp
	str	r3, [sp, #0]
	bl	sub_08042314
	adds	r4, r0, #0
	ldrb	r3, [r4, #21]
	movs	r5, #160
	lsls	r5, r5, #3
	movs	r2, #32
	adds	r5, #196
	orrs	r3, r2
	add	r5, r9
	strb	r3, [r4, #21]
	str	r4, [r5, #0]
	mov	r2, fp
	movs	r1, #12
	ldrsh	r3, [r2, r1]
	lsls	r3, r3, #3
	adds	r6, r3, #0
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #149
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r6, #140
	lsls	r0, r3, #4
	subs	r0, r0, r3
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #154
	add	r3, r9
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	lsls	r0, r0, #2
	bl	sub_08002054
	mov	r2, fp
	movs	r1, #14
	ldrsh	r3, [r2, r1]
	adds	r6, r6, r0
	lsls	r3, r3, #3
	adds	r7, r3, #0
	adds	r7, #20
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_08108048
.L_0803ffea:
	ldr	r5, [pc, #240]
	movs	r7, #28
	movs	r0, #0
	ldrsb	r0, [r5, r0]
	mov	r2, fp
	movs	r1, #0
	movs	r3, #84
	str	r7, [sp, #0]
	bl	sub_080450ac
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #236
	add	r3, r9
	str	r0, [r3, #0]
	mov	r2, fp
	movs	r0, #1
	ldrsb	r0, [r5, r0]
	movs	r1, #0
	movs	r3, #108
	str	r7, [sp, #0]
	bl	sub_080450ac
	movs	r3, #190
	lsls	r3, r3, #3
	add	r3, r9
	str	r0, [r3, #0]
	mov	r2, fp
	movs	r0, #2
	ldrsb	r0, [r5, r0]
	movs	r1, #0
	movs	r3, #132
	str	r7, [sp, #0]
	bl	sub_080450ac
	movs	r3, #160
	ldr	r5, [pc, #172]
	lsls	r3, r3, #3
	adds	r3, #244
	add	r3, r9
	str	r0, [r3, #0]
	movs	r7, #52
	movs	r0, #0
	ldrsb	r0, [r5, r0]
	mov	r2, fp
	movs	r1, #0
	movs	r3, #100
	str	r7, [sp, #0]
	bl	sub_080450ac
	movs	r3, #191
	lsls	r3, r3, #3
	add	r3, r9
	str	r0, [r3, #0]
	mov	r2, fp
	movs	r0, #1
	ldrsb	r0, [r5, r0]
	movs	r1, #0
	movs	r3, #124
	str	r7, [sp, #0]
	bl	sub_080450ac
	movs	r3, #160
	ldr	r5, [pc, #120]
	lsls	r3, r3, #3
	adds	r3, #252
	add	r3, r9
	str	r0, [r3, #0]
	movs	r7, #76
	movs	r0, #0
	ldrsb	r0, [r5, r0]
	mov	r2, fp
	movs	r1, #0
	movs	r3, #100
	str	r7, [sp, #0]
	bl	sub_080450ac
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	add	r3, r9
	str	r0, [r3, #0]
	movs	r1, #0
	movs	r0, #1
	ldrsb	r0, [r5, r0]
	mov	r2, fp
	movs	r3, #124
	str	r7, [sp, #0]
	bl	sub_080450ac
	movs	r3, #193
	lsls	r3, r3, #3
	add	r3, r9
	str	r0, [r3, #0]
	add	sp, #8
	mov	r0, fp
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00001138
	.4byte 0x0000113e
	.4byte 0x00001140
	.4byte 0x00001143
	.4byte 0x080aa0e8
	.4byte 0x050003c0
	.4byte 0x0804e584
	.4byte 0x40004000
	.4byte 0xfffffc00
	.4byte 0x0805ea7c
	.4byte 0x0805ea7f
	.2byte 0xea81
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #1
	movs	r1, #0
	sub	sp, #20
	mov	fp, r0
	mov	r9, r1
	bl	.L_0803fc38
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r7, [r3, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #2
	movs	r2, #28
	movs	r3, #3
	movs	r0, #1
	bl	sub_08039260
	str	r0, [sp, #16]
	bl	.L_0803fd28
	movs	r3, #48
	mov	r8, r0
	negs	r3, r3
	movs	r2, #64
	mov	r1, r8
	movs	r0, #7
	bl	sub_08044f88
	str	r0, [sp, #12]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #160
	movs	r3, #160
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	adds	r2, #148
	adds	r3, #149
	adds	r2, r7, r2
	adds	r3, r7, r3
	str	r2, [sp, #8]
	str	r3, [sp, #4]
.L_0804014e:
	mov	r0, fp
	cmp	r0, #0
	bne.n	.L_08040156
	b.n	.L_080403b8
.L_08040156:
	movs	r1, #0
	mov	r0, r9
	mov	fp, r1
	adds	r0, #5
	movs	r1, #5
	bl	sub_08002064
	movs	r5, #160
	lsls	r5, r5, #3
	movs	r2, #179
	mov	r9, r0
	adds	r5, #148
	lsls	r2, r2, #3
	add	r2, r9
	adds	r3, r7, #1
	add	r5, r9
	ldrsb	r1, [r3, r2]
	ldrsb	r0, [r7, r5]
	adds	r0, r0, r1
	bl	sub_08002064
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #116
	strb	r0, [r7, r5]
	adds	r3, r7, r2
	mov	r0, r9
	strh	r0, [r3, #0]
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #24]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080401a0
	movs	r1, #179
	lsls	r1, r1, #3
	adds	r3, r7, r1
	strb	r2, [r3, #0]
.L_080401a0:
	movs	r6, #160
	lsls	r6, r6, #3
	movs	r5, #0
	adds	r6, #236
	b.n	.L_080401b4
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x1180
	.2byte 0x0300
.L_080401b4:
	ldr	r0, [r6, r7]
	movs	r3, #251
	strb	r3, [r0, #15]
	bl	sub_080f8028
	ldr	r3, [r6, r7]
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #150
	ldrb	r1, [r3, #14]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	cmp	r5, r3
	beq.n	.L_080401d8
	movs	r2, #1
.L_080401d8:
	ldr	r3, [pc, #868]
	adds	r6, #4
	ldrsb	r0, [r3, r5]
	adds	r5, #1
	bl	sub_08045048
	cmp	r5, #2
	ble.n	.L_080401b4
	movs	r6, #191
	movs	r5, #0
	lsls	r6, r6, #3
.L_080401ee:
	ldr	r0, [r6, r7]
	movs	r3, #251
	strb	r3, [r0, #15]
	bl	sub_080f8028
	ldr	r3, [r6, r7]
	movs	r0, #147
	lsls	r0, r0, #3
	adds	r0, #255
	ldrb	r1, [r3, #14]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	cmp	r5, r3
	beq.n	.L_08040212
	movs	r2, #1
.L_08040212:
	ldr	r3, [pc, #816]
	adds	r6, #4
	ldrsb	r0, [r3, r5]
	adds	r5, #1
	bl	sub_08045048
	cmp	r5, #1
	ble.n	.L_080401ee
	movs	r6, #192
	lsls	r6, r6, #3
	movs	r5, #0
	adds	r6, #4
.L_0804022a:
	ldr	r0, [r6, r7]
	movs	r3, #251
	strb	r3, [r0, #15]
	bl	sub_080f8028
	ldr	r3, [r6, r7]
	movs	r2, #0
	ldrb	r1, [r3, #14]
	movs	r3, #179
	lsls	r3, r3, #3
	adds	r3, r3, r7
	mov	sl, r3
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r5, r3
	beq.n	.L_0804024e
	movs	r2, #1
.L_0804024e:
	ldr	r3, [pc, #760]
	adds	r6, #4
	ldrsb	r0, [r3, r5]
	adds	r5, #1
	bl	sub_08045048
	cmp	r5, #1
	ble.n	.L_0804022a
	mov	r1, r8
	movs	r0, #12
	ldrsh	r3, [r1, r0]
	ldr	r2, [sp, #8]
	lsls	r3, r3, #3
	adds	r5, r3, #0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	movs	r1, #160
	lsls	r1, r1, #3
	lsls	r0, r3, #4
	adds	r1, #153
	subs	r0, r0, r3
	adds	r3, r7, r1
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	lsls	r0, r0, #2
	bl	sub_08002054
	adds	r5, #140
	adds	r5, r5, r0
	mov	r0, r8
	movs	r2, #14
	ldrsh	r3, [r0, r2]
	movs	r1, #160
	lsls	r1, r1, #3
	lsls	r3, r3, #3
	adds	r1, #180
	adds	r2, r3, #4
	adds	r0, r7, r1
	movs	r3, #1
	adds	r1, r5, #0
	bl	sub_08108040
	mov	r0, r8
	movs	r2, #12
	ldrsh	r3, [r0, r2]
	ldr	r1, [sp, #4]
	lsls	r3, r3, #3
	adds	r5, r3, #0
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #154
	lsls	r0, r3, #4
	subs	r0, r0, r3
	adds	r3, r7, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	lsls	r0, r0, #2
	bl	sub_08002054
	adds	r5, #140
	mov	r1, r8
	adds	r5, r5, r0
	movs	r0, #14
	ldrsh	r3, [r1, r0]
	adds	r1, r5, #0
	lsls	r3, r3, #3
	adds	r2, r3, #0
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #196
	adds	r0, r7, r3
	adds	r2, #20
	movs	r3, #1
	bl	sub_08108040
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #150
	adds	r3, r7, r0
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #596]
	mov	r0, r8
	adds	r5, r2, r3
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r1, #160
	movs	r2, #40
	movs	r3, #200
	bl	sub_08039454
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #160
	movs	r3, #40
	bl	sub_08042010
	movs	r1, #147
	lsls	r1, r1, #3
	adds	r1, #255
	adds	r3, r7, r1
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #556]
	mov	r0, r8
	adds	r5, r2, r3
	movs	r3, #72
	str	r3, [sp, #0]
	movs	r1, #160
	movs	r2, #64
	movs	r3, #184
	bl	sub_08039454
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #160
	movs	r3, #64
	bl	sub_08042010
	mov	r3, sl
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #524]
	mov	r0, r8
	adds	r5, r2, r3
	movs	r3, #96
	str	r3, [sp, #0]
	movs	r1, #160
	movs	r2, #88
	movs	r3, #184
	bl	sub_08039454
	adds	r0, r5, #0
	movs	r3, #88
	mov	r1, r8
	movs	r2, #160
	bl	sub_08042010
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #4]
	movs	r0, #0
	ldrsb	r0, [r1, r0]
	movs	r1, #0
	ldrsb	r1, [r2, r1]
	bl	.L_0803f9c0
	mov	r1, r8
	movs	r0, #12
	ldrsh	r3, [r1, r0]
	mov	r0, r9
	lsls	r5, r3, #3
	movs	r3, #14
	ldrsh	r2, [r1, r3]
	lsls	r3, r0, #1
	add	r3, r9
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r2, r3, #4
	cmp	r0, #0
	bne.n	.L_08040394
	adds	r2, #8
.L_08040394:
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #164
	adds	r0, r7, r1
	movs	r3, #3
	adds	r1, r5, #0
	bl	sub_08108040
	ldr	r0, [sp, #16]
	bl	sub_08039430
	ldr	r0, [pc, #428]
	ldr	r1, [sp, #16]
	add	r0, r9
	movs	r2, #0
	movs	r3, #0
	bl	sub_08041fa4
.L_080403b8:
	ldr	r0, [sp, #12]
	bl	sub_08045018
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [pc, #404]
	movs	r3, #4
	ldr	r2, [r5, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0804040c
	movs	r0, #112
	bl	sub_081c0010
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #126
	adds	r1, r7, r2
	ldrh	r3, [r1, #0]
	movs	r0, #1
	adds	r3, #1
	mov	fp, r0
	movs	r0, #160
	strh	r3, [r1, #0]
	lsls	r0, r0, #11
	lsls	r3, r3, #16
	movs	r2, #0
	cmp	r3, r0
	bls.n	.L_080403f6
	strh	r2, [r1, #0]
.L_080403f6:
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #356]
	ldrb	r3, [r2, r3]
	ldr	r2, [sp, #8]
	strb	r3, [r2, #0]
	ldr	r2, [pc, #352]
	ldrh	r3, [r1, #0]
	ldr	r0, [sp, #4]
	ldrb	r3, [r2, r3]
	strb	r3, [r0, #0]
	b.n	.L_0804014e
.L_0804040c:
	ldr	r2, [r5, #4]
	movs	r3, #9
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08040418
	b.n	.L_08040536
.L_08040418:
	ldr	r2, [r5, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08040424
	b.n	.L_0804052a
.L_08040424:
	ldr	r2, [r5, #12]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08040440
	movs	r0, #111
	bl	sub_081c0010
	movs	r1, #1
	negs	r1, r1
	movs	r2, #1
	add	r9, r1
	mov	fp, r2
	b.n	.L_0804014e
.L_08040440:
	ldr	r2, [r5, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08040458
	movs	r0, #111
	bl	sub_081c0010
	movs	r3, #1
	add	r9, r3
	mov	fp, r3
	b.n	.L_0804014e
.L_08040458:
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804047a
	movs	r0, #111
	bl	sub_081c0010
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #148
	add	r2, r9
	ldrb	r3, [r7, r2]
	movs	r0, #1
	subs	r3, #1
	strb	r3, [r7, r2]
	mov	fp, r0
.L_0804047a:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08040486
	b.n	.L_0804014e
.L_08040486:
	movs	r0, #111
	bl	sub_081c0010
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #148
	add	r2, r9
	ldrb	r3, [r7, r2]
	movs	r1, #1
	adds	r3, #1
	strb	r3, [r7, r2]
	mov	fp, r1
	b.n	.L_0804014e
.L_080404a0:
	ldr	r0, [sp, #16]
	movs	r1, #2
	bl	sub_0803939c
	mov	r0, r8
	movs	r1, #2
	bl	sub_0803939c
	cmp	r6, #0
	bne.n	.L_08040574
	ldr	r5, [pc, #176]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #74
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080404dc
	movs	r0, #179
	lsls	r0, r0, #3
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L_080404dc
	ldr	r0, [pc, #148]
	movs	r1, #1
	bl	sub_0803a7ac
.L_080404dc:
	cmp	r6, #0
	bne.n	.L_08040576
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #148
	adds	r3, r7, r1
	movs	r0, #147
	ldrb	r2, [r3, #0]
	lsls	r0, r0, #1
	adds	r0, #255
	adds	r3, r5, r0
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	ldrb	r2, [r3, #0]
	adds	r0, #1
	adds	r3, r5, r0
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	ldrb	r2, [r3, #0]
	adds	r0, #6
	adds	r3, r5, r0
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	ldrb	r2, [r3, #0]
	subs	r0, #2
	adds	r3, r5, r0
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	ldrb	r2, [r3, #0]
	adds	r0, #32
	adds	r3, r5, r0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #72]
	strb	r2, [r3, #0]
	b.n	.L_0804058e
.L_0804052a:
	movs	r6, #1
	movs	r0, #113
	negs	r6, r6
	bl	sub_081c0010
	b.n	.L_080404a0
.L_08040536:
	movs	r0, #112
	movs	r6, #0
	bl	sub_081c0010
	b.n	.L_080404a0
	.4byte 0x0805ea7c
	.4byte 0x0805ea7f
	.4byte 0x0805ea81
	.4byte 0x0000113b
	.4byte 0x00001141
	.4byte 0x00001144
	.4byte 0x00001146
	.4byte 0x03001150
	.4byte 0x0805ea83
	.4byte 0x0805ea89
	.4byte 0x02000240
	.4byte 0x00001161
	.2byte 0x1200
	.2byte 0x0300
.L_08040574:
	ldr	r5, [pc, #48]
.L_08040576:
	movs	r1, #147
	lsls	r1, r1, #1
	movs	r2, #128
	adds	r1, #255
	lsls	r2, r2, #2
	adds	r3, r5, r1
	adds	r2, #38
	ldrb	r0, [r3, #0]
	adds	r3, r5, r2
	ldrb	r1, [r3, #0]
	bl	.L_0803f9c0
.L_0804058e:
	bl	.L_0803fd14
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r6, #0
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r5, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #164
	adds	r0, r5, r2
	bl	sub_08108030
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #116
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	adds	r2, #156
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r0, [r5, r3]
	bl	sub_080450fc
	pop	{r5, pc}
	.align 2, 0
