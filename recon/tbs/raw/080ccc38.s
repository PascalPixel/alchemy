.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080ccc38
	.thumb_func
Func_080ccc38:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #56]
	adds	r3, r6, #0
	ldmia	r3!, {r7}
	ldr	r2, [pc, #52]
	ldr	r3, [r3, #0]
	adds	r5, r7, r2
	str	r0, [r5, #0]
	movs	r0, #0
	mov	r9, r3
	sub	sp, #32
	mov	sl, r1
	bl	sub_080cd594
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080ccc8c
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	b.n	.L_080ccc98
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x0020
	.2byte 0x0400
.L_080ccc8c:
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
.L_080ccc98:
	bl	sub_080ed408
	ldr	r6, [r6, #28]
	str	r6, [sp, #12]
	ldr	r0, [pc, #468]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r3, #0
	ldr	r0, [pc, #460]
	ldr	r1, [pc, #460]
	movs	r2, #1
	bl	sub_080e0524
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080cccd2
	ldr	r0, [pc, #452]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #444]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
.L_080cccd2:
	movs	r4, #239
	lsls	r4, r4, #7
	ldr	r0, [pc, #436]
	adds	r2, r7, r4
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r1, #144
	adds	r2, r7, r0
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #420]
	bl	sub_080041d8
	ldr	r2, [pc, #420]
	adds	r5, r7, r2
	ldr	r3, [r5, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	add	r6, sp, #20
	mov	r8, r0
	adds	r1, r6, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080e396c
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080ccd1c
	ldr	r2, [r6, #0]
	movs	r3, #16
	b.n	.L_080ccd20
.L_080ccd1c:
	ldr	r2, [r6, #0]
	movs	r3, #112
.L_080ccd20:
	ldr	r1, [pc, #372]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	movs	r3, #74
	mov	r4, sl
	str	r3, [sp, #8]
	cmp	r4, #1
	beq.n	.L_080ccd36
	movs	r0, #48
	str	r0, [sp, #8]
.L_080ccd36:
	ldr	r2, [sp, #8]
	movs	r5, #0
	cmp	r2, #0
	bne.n	.L_080ccd40
	b.n	.L_080cce54
.L_080ccd40:
	ldr	r3, [pc, #336]
	ldr	r4, [pc, #344]
	adds	r6, r7, r3
	mov	fp, r4
.L_080ccd48:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080ccd50
	adds	r3, r5, #3
.L_080ccd50:
	asrs	r4, r3, #2
	cmp	r4, #5
	bgt.n	.L_080ccdc2
	cmp	r4, #3
	bgt.n	.L_080ccd8e
	lsls	r3, r4, #1
	mov	r0, fp
	ldrh	r1, [r0, r3]
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r0, [pc, #308]
	lsls	r3, r3, #1
	adds	r3, r4, r3
	ldrb	r2, [r0, r3]
	ldr	r0, [pc, #304]
	ldr	r3, [pc, #308]
	ldrb	r0, [r0, r4]
	ldrsb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #304]
	ldrb	r0, [r0, r4]
	adds	r1, r7, r1
	str	r0, [sp, #4]
	adds	r3, #32
	mov	r0, r9
	ldr	r4, [sp, #12]
	bl	sub_080072f4
	b.n	.L_080ccdc2
.L_080ccd8e:
	lsls	r3, r4, #1
	mov	r0, fp
	ldrh	r1, [r0, r3]
	ldr	r2, [pc, #232]
	ldr	r3, [r6, #0]
	adds	r1, r1, r2
	ldr	r2, [r3, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r0, [pc, #252]
	lsls	r3, r3, #1
	adds	r3, r4, r3
	ldrb	r2, [r0, r3]
	ldr	r0, [pc, #248]
	ldr	r3, [pc, #252]
	ldrb	r0, [r0, r4]
	ldrsb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #248]
	ldrb	r0, [r0, r4]
	adds	r3, #32
	str	r0, [sp, #4]
	ldr	r4, [sp, #12]
	mov	r0, r9
	bl	sub_080072f4
.L_080ccdc2:
	cmp	r5, #8
	bne.n	.L_080cce02
	mov	r0, sl
	cmp	r0, #0
	bne.n	.L_080ccde0
	movs	r0, #133
	bl	sub_080b50e8
	ldr	r3, [r6, #0]
	movs	r1, #1
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5088
	b.n	.L_080ccdfa
.L_080ccde0:
	movs	r0, #134
	bl	sub_080f9010
	ldr	r3, [r6, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_080d6888
.L_080ccdfa:
	ldr	r0, [pc, #180]
	movs	r3, #8
	adds	r2, r7, r0
	str	r3, [r2, #0]
.L_080cce02:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_080cce30
	cmp	r5, #13
	bne.n	.L_080cce1e
	movs	r3, #192
	mov	r4, r8
	lsls	r3, r3, #12
	str	r3, [r4, #40]
	ldr	r3, [pc, #156]
	str	r3, [r4, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r4, #68]
.L_080cce1e:
	cmp	r5, #65
	bne.n	.L_080cce30
	ldr	r0, [pc, #140]
	movs	r3, #4
	adds	r2, r7, r0
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_080b50e8
.L_080cce30:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r3, [pc, #120]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r4, [sp, #8]
	adds	r5, #1
	cmp	r5, r4
	beq.n	.L_080cce54
	b.n	.L_080ccd48
.L_080cce54:
	ldr	r0, [pc, #56]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000071
	.4byte 0x00000072
	.4byte 0x02010000
	.4byte 0x000000a0
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0x080ee070
	.4byte 0x080ee07c
	.4byte 0x080ee064
	.4byte 0x080ee088
	.4byte 0x080ee06a
	.4byte 0x000077a8
	.4byte 0x00007851
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #96]
	mov	r8, r1
	mov	r3, r8
	subs	r3, #4
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #88]
	ldr	r2, [r1, #0]
	adds	r5, r7, r3
	str	r0, [r5, #0]
	movs	r0, #2
	sub	sp, #32
	mov	fp, r2
	bl	sub_080cd594
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	mov	sl, r1
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	mov	r2, sl
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	add	r6, sp, #20
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r6, #0
	bl	sub_080e396c
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #20]
	lsls	r3, r3, #1
	add	r5, sp, #8
	adds	r3, #34
	ldrsh	r0, [r2, r3]
	adds	r1, r5, #0
	bl	sub_080e396c
	ldr	r1, [r6, #0]
	ldr	r3, [r5, #0]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	b.n	.L_080ccf3c
	.4byte 0x00000100
	.4byte 0x00001000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000052
	.2byte 0x0020
	.2byte 0x0400
.L_080ccf3c:
	adds	r1, r1, r3
	movs	r3, #64
	ldr	r2, [pc, #112]
	subs	r3, r3, r1
	lsls	r3, r3, #8
	str	r1, [r6, #0]
	ldr	r0, [pc, #108]
	str	r3, [r2, #0]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r1, [pc, #96]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #0
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #80]
	bl	sub_080041d8
	movs	r0, #143
	bl	sub_080f9010
	movs	r2, #0
	movs	r3, #1
	movs	r1, #32
	mov	r9, r2
	mov	sl, r3
	mov	r8, r1
.L_080ccf84:
	mov	r2, r9
	cmp	r2, #8
	bgt.n	.L_080ccf94
	lsls	r3, r2, #1
	ldr	r2, [pc, #28]
	ldr	r1, [pc, #52]
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_080ccf94:
	mov	r2, r9
	cmp	r2, #53
	ble.n	.L_080ccfc8
	lsls	r3, r2, #1
	ldr	r2, [pc, #16]
	subs	r2, r2, r3
	ldr	r3, [pc, #8]
	orrs	r2, r3
	ldr	r3, [pc, #28]
	strh	r2, [r3, #0]
	b.n	.L_080ccfc8
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x0000007c
	.4byte 0x04000028
	.4byte 0x00000059
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x0052
	.2byte 0x0400
.L_080ccfc8:
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #276]
	adds	r1, r7, #0
	ldr	r4, [r3, #0]
	movs	r2, #33
	movs	r3, #41
	mov	r0, fp
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #7
	movs	r0, #46
	bl	sub_080ed408
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #232]
	adds	r1, r7, #0
	ldr	r4, [r3, #0]
	movs	r2, #64
	movs	r3, #41
	mov	r0, fp
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #11
	movs	r0, #46
	bl	sub_080ed408
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #188]
	adds	r1, r7, #0
	ldr	r4, [r3, #0]
	movs	r2, #33
	movs	r3, #72
	mov	r0, fp
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r1, sl
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #15
	movs	r0, #46
	bl	sub_080ed408
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #144]
	adds	r1, r7, #0
	ldr	r4, [r3, #0]
	movs	r2, #64
	movs	r3, #72
	mov	r0, fp
	bl	sub_080072f4
	movs	r0, #46
	bl	sub_08002dd8
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_080cd084
	movs	r0, #143
	bl	sub_080b50e8
.L_080cd084:
	ldr	r2, [pc, #112]
	ldr	r3, [r7, r2]
	ldr	r3, [r3, #20]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080cd0ba
	movs	r6, #36
.L_080cd092:
	mov	r3, r9
	cmp	r3, #10
	bne.n	.L_080cd0ac
	ldr	r3, [r7, r2]
	movs	r2, #1
	ldrsh	r0, [r3, r6]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	adds	r3, r5, #0
	bl	sub_080d6888
.L_080cd0ac:
	ldr	r2, [pc, #72]
	ldr	r3, [r7, r2]
	ldr	r3, [r3, #20]
	adds	r5, #1
	adds	r6, #2
	cmp	r5, r3
	bne.n	.L_080cd092
.L_080cd0ba:
	bl	sub_080cd52c
	ldr	r2, [pc, #60]
	mov	r1, sl
	adds	r3, r7, r2
	str	r1, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #63
	beq.n	.L_080cd0d8
	b.n	.L_080ccf84
.L_080cd0d8:
	ldr	r0, [pc, #36]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f08
	.4byte 0x00007828
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #136
	str	r1, [sp, #4]
	ldr	r3, [pc, #320]
	ldr	r1, [r3, #0]
	subs	r3, #4
	ldr	r3, [r3, #0]
	add	r2, sp, #8
	str	r3, [sp, #0]
	mov	fp, r2
	add	r3, sp, #136
	adds	r7, r0, #0
	mov	r9, r1
	movs	r6, #63
	mov	r5, fp
	mov	r8, r3
.L_080cd130:
	bl	sub_08004458
	ands	r0, r6
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r8
	bne.n	.L_080cd130
	cmp	r7, #1
	bne.n	.L_080cd1c0
	movs	r1, #0
	movs	r2, #1
	mov	r8, r1
	mov	sl, r2
	movs	r5, #0
.L_080cd14c:
	add	r8, sl
	movs	r3, #1
	add	sl, r3
	cmp	r5, r8
	beq.n	.L_080cd1a6
	ldr	r1, [sp, #4]
	mov	ip, fp
	movs	r4, #7
	subs	r7, r3, r1
.L_080cd15e:
	movs	r6, #0
	mov	r0, ip
.L_080cd162:
	ldrb	r3, [r0, #0]
	subs	r1, r5, r3
	adds	r0, #1
	cmp	r1, #0
	blt.n	.L_080cd19a
	cmp	r1, #127
	bgt.n	.L_080cd19a
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_080cd178
	adds	r2, r6, #7
.L_080cd178:
	asrs	r2, r2, #3
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_080cd182
	adds	r3, r1, #7
.L_080cd182:
	asrs	r3, r3, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r6, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	mov	r3, r9
	strb	r7, [r3, r2]
.L_080cd19a:
	adds	r6, #1
	cmp	r6, #128
	bne.n	.L_080cd162
	adds	r5, #1
	cmp	r5, r8
	bne.n	.L_080cd15e
.L_080cd1a6:
	ldr	r1, [sp, #0]
	ldr	r3, [pc, #176]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #128
	lsls	r1, r1, #1
	cmp	r8, r1
	ble.n	.L_080cd14c
	b.n	.L_080cd244
.L_080cd1c0:
	movs	r2, #0
	movs	r3, #1
	mov	r8, r2
	mov	sl, r3
	movs	r6, #0
.L_080cd1ca:
	mov	r1, sl
	lsrs	r3, r1, #31
	add	r3, sl
	asrs	r3, r3, #1
	movs	r2, #4
	add	r8, r3
	add	sl, r2
	cmp	r6, r8
	beq.n	.L_080cd22e
	ldr	r1, [sp, #4]
	movs	r3, #1
	mov	ip, fp
	movs	r4, #7
	subs	r7, r3, r1
.L_080cd1e6:
	movs	r5, #0
	mov	r0, ip
.L_080cd1ea:
	ldrb	r3, [r0, #0]
	subs	r1, r6, r3
	adds	r0, #1
	cmp	r1, #0
	blt.n	.L_080cd222
	cmp	r1, #127
	bgt.n	.L_080cd222
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_080cd200
	adds	r2, r1, #7
.L_080cd200:
	asrs	r2, r2, #3
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080cd20a
	adds	r3, r5, #7
.L_080cd20a:
	asrs	r3, r3, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	adds	r3, r5, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	mov	r3, r9
	strb	r7, [r3, r2]
.L_080cd222:
	adds	r5, #1
	cmp	r5, #128
	bne.n	.L_080cd1ea
	adds	r6, #1
	cmp	r6, r8
	bne.n	.L_080cd1e6
.L_080cd22e:
	ldr	r1, [sp, #0]
	ldr	r3, [pc, #40]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	mov	r1, r8
	cmp	r1, #191
	ble.n	.L_080cd1ca
.L_080cd244:
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ef0
	.4byte 0x00007824
