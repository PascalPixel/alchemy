.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072fc, 0x080072fc
	.set sub_08009038, 0x08009038
	.set sub_080b5070, 0x080b5070
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d6750, 0x080d6750
	.set sub_080d6888, 0x080d6888
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e3944, 0x080e3944
	.set sub_080e6d3c, 0x080e6d3c
	.set sub_080e6eac, 0x080e6eac
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Overlay_080d2d98
Overlay_080d2d98:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #100]
	ldr	r1, [r3, #0]
	sub	sp, #60
	str	r1, [sp, #48]
	subs	r2, r3, #4
	ldr	r2, [r2, #0]
	str	r2, [sp, #44]
	ldr	r7, [pc, #88]
	ldr	r3, [r3, #4]
	str	r3, [sp, #40]
	adds	r3, r2, r7
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r2, [pc, #76]
	mov	r0, sp
	ldr	r3, [pc, #60]
	adds	r0, #52
	strh	r3, [r2, #0]
	str	r0, [sp, #36]
	ldr	r1, [sp, #36]
	movs	r0, #0
	bl	sub_080cef64
	ldr	r0, [pc, #60]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #56]
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	ldr	r1, [sp, #44]
	adds	r0, r5, #0
	bl	sub_08005340
	ldr	r0, [pc, #36]
	bl	sub_08002f40
	ldr	r3, [pc, #36]
	ldr	r2, [sp, #44]
	adds	r5, r0, #0
	b.n	.L_080d2e28
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x0000006e
	.4byte 0x03001388
	.4byte 0x00000085
	.2byte 0x06e4
	.2byte 0x0000
.L_080d2e28:
	adds	r5, #128
	adds	r1, r2, r3
	adds	r0, r5, #0
	bl	sub_08005340
	ldr	r0, [pc, #888]
	bl	sub_08002f40
	ldr	r1, [sp, #40]
	bl	sub_08005340
	movs	r0, #239
	ldr	r7, [sp, #44]
	lsls	r0, r0, #7
	ldr	r1, [pc, #872]
	adds	r2, r7, r0
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #860]
	bl	sub_080041d8
	movs	r7, #176
	ldr	r0, [pc, #856]
	lsls	r7, r7, #15
	ldr	r1, [pc, #856]
	movs	r3, #128
	str	r7, [sp, #32]
	str	r0, [sp, #20]
	ldr	r7, [sp, #44]
	ldr	r0, [pc, #848]
	movs	r2, #0
	lsls	r3, r3, #17
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r3, [sp, #28]
	mov	r8, r2
	adds	r3, r7, r0
	subs	r2, #1
.L_080d2e7e:
	movs	r1, #1
	add	r8, r1
	mov	r7, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_080d2e7e
	ldr	r1, [sp, #44]
	ldr	r2, [pc, #820]
	movs	r0, #0
	mov	r8, r0
	adds	r5, r1, r2
.L_080d2e96:
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #56
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	negs	r3, r3
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r7, r8
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_080d2e96
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #764]
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #3
.L_080d2ed2:
	movs	r7, #1
	add	r8, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080d2ed2
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #744]
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	bl	sub_080d6750
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #190
	movs	r2, #2
	lsls	r1, r1, #1
	movs	r0, #12
	bl	sub_080dbb24
	movs	r2, #0
	mov	r9, r2
.L_080d2f00:
	ldr	r3, [pc, #716]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d2f1a
	mov	r3, r9
	cmp	r3, #32
	ble.n	.L_080d2f1a
	cmp	r3, #97
	bgt.n	.L_080d2f1a
	movs	r7, #98
	mov	r9, r7
.L_080d2f1a:
	mov	r0, r9
	cmp	r0, #120
	bne.n	.L_080d2f26
	movs	r0, #134
	bl	sub_080b50e8
.L_080d2f26:
	mov	r1, r9
	cmp	r1, #15
	bgt.n	.L_080d2f32
	ldr	r2, [sp, #16]
	adds	r2, #2
	str	r2, [sp, #16]
.L_080d2f32:
	mov	r3, r9
	cmp	r3, #99
	bgt.n	.L_080d2f7c
	ldr	r0, [sp, #28]
	ldr	r7, [sp, #20]
	adds	r7, r7, r0
	str	r7, [sp, #28]
	ldr	r7, [sp, #20]
	movs	r3, #58
	muls	r3, r7
	ldr	r2, [sp, #32]
	ldr	r1, [sp, #24]
	adds	r1, r1, r2
	str	r1, [sp, #32]
	cmp	r3, #0
	bge.n	.L_080d2f54
	adds	r3, #63
.L_080d2f54:
	ldr	r0, [sp, #24]
	asrs	r3, r3, #6
	str	r3, [sp, #20]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080d2f66
	adds	r3, #63
.L_080d2f66:
	ldr	r1, [sp, #28]
	ldr	r2, [pc, #616]
	asrs	r3, r3, #6
	str	r3, [sp, #24]
	cmp	r1, r2
	bgt.n	.L_080d2f7c
	ldr	r3, [sp, #20]
	movs	r7, #128
	lsls	r7, r7, #8
	adds	r7, r3, r7
	str	r7, [sp, #20]
.L_080d2f7c:
	movs	r0, #1
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #32]
	bl	sub_080e6d3c
	mov	r0, r9
	cmp	r0, #28
	bne.n	.L_080d300c
	movs	r1, #0
	movs	r2, #63
	ldr	r7, [pc, #580]
	mov	r8, r1
	mov	sl, r2
.L_080d2f96:
	movs	r0, #1
	ldr	r3, [r7, #24]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_080d2ffe
	bl	sub_08004458
	mov	r1, sl
	adds	r6, r0, #0
	ands	r6, r1
	bl	sub_08004458
	ldr	r3, [pc, #556]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r3, r3, #3
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r0, #192
	lsls	r0, r0, #15
	asrs	r3, r3, #2
	adds	r3, r3, r0
	str	r3, [r7, #4]
	bl	sub_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08004458
	mov	r2, sl
	ands	r0, r2
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	str	r0, [r7, #16]
	str	r3, [r7, #24]
.L_080d2ffe:
	movs	r3, #1
	movs	r0, #128
	add	r8, r3
	lsls	r0, r0, #1
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L_080d2f96
.L_080d300c:
	mov	r1, r9
	subs	r1, #32
	str	r1, [sp, #12]
	cmp	r1, #47
	bhi.n	.L_080d30a2
	movs	r2, #0
	movs	r3, #63
	ldr	r7, [pc, #444]
	mov	fp, r2
	mov	r8, r2
	mov	sl, r3
.L_080d3022:
	movs	r0, #1
	ldr	r3, [r7, #24]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_080d3094
	bl	sub_08004458
	mov	r1, sl
	adds	r6, r0, #0
	ands	r6, r1
	bl	sub_08004458
	ldr	r3, [pc, #416]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r3, r3, #3
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r0, #192
	lsls	r0, r0, #15
	asrs	r3, r3, #2
	adds	r3, r3, r0
	str	r3, [r7, #4]
	bl	sub_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08004458
	mov	r2, sl
	ands	r0, r2
	movs	r3, #0
	negs	r0, r0
	subs	r0, #8
	str	r3, [r7, #24]
	movs	r3, #1
	lsls	r0, r0, #13
	add	fp, r3
	str	r0, [r7, #16]
	mov	r0, fp
	cmp	r0, #16
	beq.n	.L_080d30a2
.L_080d3094:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_080d3022
.L_080d30a2:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_080d30ae
	movs	r0, #164
	bl	sub_080f9010
.L_080d30ae:
	mov	r7, r9
	cmp	r7, #32
	bne.n	.L_080d30ba
	movs	r0, #145
	bl	sub_080f9010
.L_080d30ba:
	mov	r0, r9
	cmp	r0, #80
	bne.n	.L_080d30c6
	movs	r0, #144
	bl	sub_080f9010
.L_080d30c6:
	ldr	r1, [sp, #12]
	cmp	r1, #47
	bhi.n	.L_080d3132
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #272]
	adds	r0, r0, r1
	movs	r2, #0
	mov	r7, r9
	mov	sl, r0
	ldr	r0, [pc, #264]
	mov	r8, r2
	lsls	r3, r7, #4
	movs	r2, #34
	ldr	r6, [pc, #260]
	mov	fp, r2
	adds	r7, r3, r0
.L_080d30e6:
	adds	r0, r7, #0
	movs	r1, #104
	bl	sub_080022fc
	ldrb	r3, [r6, #1]
	ldrb	r2, [r6, #0]
	adds	r5, r0, #0
	mov	r1, fp
	movs	r0, #104
	subs	r3, r3, r5
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #52]
	subs	r2, #17
	subs	r3, #104
	ldr	r0, [sp, #48]
	mov	r1, sl
	bl	sub_080072f4
	ldrb	r2, [r6, #0]
	ldrb	r3, [r6, #1]
	mov	r1, fp
	subs	r2, #17
	subs	r3, r3, r5
	str	r1, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #48]
	mov	r1, sl
	bl	sub_080072f4
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r6, #2
	adds	r7, #25
	cmp	r3, #3
	bne.n	.L_080d30e6
.L_080d3132:
	mov	r7, r9
	cmp	r7, #95
	bgt.n	.L_080d3172
	movs	r0, #0
	mov	r8, r0
	movs	r5, #32
	movs	r6, #120
.L_080d3140:
	mov	r2, r8
	lsls	r1, r2, #5
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L_080d314c
	adds	r2, #3
.L_080d314c:
	movs	r3, #31
	asrs	r2, r2, #2
	ands	r2, r3
	ldr	r7, [sp, #16]
	adds	r2, r1, r2
	subs	r2, #32
	ldr	r1, [sp, #44]
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #48]
	subs	r3, r6, r7
	bl	sub_080072f4
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #5
	bne.n	.L_080d3140
.L_080d3172:
	movs	r2, #0
	ldr	r5, [pc, #96]
	mov	r8, r2
.L_080d3178:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bge.n	.L_080d3180
	b.n	.L_080d32a8
.L_080d3180:
	mov	r0, r8
	movs	r1, #3
	bl	sub_080022fc
	ldr	r3, [r5, #16]
	adds	r4, r0, #2
	cmp	r3, #0
	ble.n	.L_080d3192
	adds	r4, #2
.L_080d3192:
	mov	r7, r9
	cmp	r7, #68
	ble.n	.L_080d319e
	cmp	r4, #5
	bgt.n	.L_080d319e
	movs	r4, #6
.L_080d319e:
	mov	r0, r9
	cmp	r0, #70
	ble.n	.L_080d31ec
	cmp	r4, #6
	bgt.n	.L_080d31ec
	movs	r4, #7
	b.n	.L_080d31ec
	.4byte 0x00000073
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0xfff00000
	.4byte 0xfffc0000
	.4byte 0x00007098
	.4byte 0x00007320
	.4byte 0x02010018
	.4byte 0x00007828
	.4byte 0x03001b04
	.4byte 0x0077ffff
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x000006e4
	.4byte 0xffffff00
	.2byte 0xe1ac
	.2byte 0x080e
.L_080d31ec:
	mov	r1, r9
	cmp	r1, #72
	ble.n	.L_080d31f8
	cmp	r4, #7
	bgt.n	.L_080d31f8
	movs	r4, #8
.L_080d31f8:
	mov	r2, r9
	cmp	r2, #74
	ble.n	.L_080d3204
	cmp	r4, #8
	bgt.n	.L_080d3204
	movs	r4, #9
.L_080d3204:
	mov	r7, r9
	cmp	r7, #76
	ble.n	.L_080d320c
	movs	r4, #10
.L_080d320c:
	movs	r6, #4
	cmp	r3, #0
	bgt.n	.L_080d3214
	movs	r6, #0
.L_080d3214:
	lsls	r0, r4, #1
	ldr	r2, [pc, #396]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #40]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #36]
	subs	r3, r3, r4
	ldr	r4, [r6, r0]
	ldr	r0, [sp, #48]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r1, [r5, #16]
	ldr	r3, [r5, #4]
	mov	r2, r9
	adds	r3, r3, r1
	str	r3, [r5, #4]
	cmp	r2, #80
	ble.n	.L_080d325c
	ldr	r7, [pc, #336]
	adds	r3, r1, r7
	b.n	.L_080d326a
.L_080d325c:
	movs	r2, #3
	mov	r0, r8
	ldr	r3, [pc, #328]
	ands	r2, r0
	lsls	r2, r2, #2
	ldr	r3, [r3, r2]
	adds	r3, r1, r3
.L_080d326a:
	str	r3, [r5, #16]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_080d327a
	adds	r3, #63
.L_080d327a:
	ldr	r2, [r5, #16]
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r2, r3, #1
	cmp	r2, #0
	bge.n	.L_080d328c
	adds	r2, #63
.L_080d328c:
	ldr	r3, [r5, #24]
	asrs	r2, r2, #6
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r2, #0
	ble.n	.L_080d32a8
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	cmp	r3, #104
	ble.n	.L_080d32a8
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080d32a8:
	movs	r2, #1
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #3
	adds	r5, #28
	cmp	r8, r3
	beq.n	.L_080d32b8
	b.n	.L_080d3178
.L_080d32b8:
	mov	r7, r9
	cmp	r7, #79
	bgt.n	.L_080d333c
	ldr	r3, [pc, #240]
	ldr	r1, [sp, #44]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_080d333c
	adds	r7, r2, #0
	movs	r4, #36
.L_080d32d4:
	mov	r0, r9
	cmp	r0, #29
	ble.n	.L_080d332e
	movs	r1, #12
	str	r4, [sp, #8]
	bl	sub_080022fc
	adds	r6, r0, #0
	ldr	r4, [sp, #8]
	cmp	r6, #0
	bne.n	.L_080d3314
	ldr	r3, [r7, #0]
	ldrsh	r0, [r3, r4]
	bl	sub_080b5098
	ldr	r3, [r7, #0]
	ldr	r4, [sp, #8]
	ldr	r5, [r0, #0]
	ldrsh	r0, [r3, r4]
	movs	r3, #1
	negs	r3, r3
	movs	r1, #7
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_080d6888
	movs	r3, #144
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r3, [pc, #164]
	ldr	r4, [sp, #8]
	str	r3, [r5, #72]
.L_080d3314:
	cmp	r6, #6
	bne.n	.L_080d332e
	ldr	r3, [r7, #0]
	ldrsh	r0, [r3, r4]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	str	r4, [sp, #8]
	bl	sub_080d6888
	ldr	r4, [sp, #8]
.L_080d332e:
	ldr	r3, [r7, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r4, #2
	cmp	r8, r3
	bne.n	.L_080d32d4
.L_080d333c:
	ldr	r3, [sp, #44]
	ldr	r7, [pc, #120]
	adds	r2, r3, r7
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_080030f8
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	cmp	r1, #124
	beq.n	.L_080d3358
	b.n	.L_080d2f00
.L_080d3358:
	ldr	r0, [pc, #96]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r2, [sp, #32]
	movs	r0, #1
	ldr	r1, [sp, #28]
	bl	sub_080e6eac
	ldr	r3, [sp, #44]
	movs	r2, #0
	subs	r7, #76
	mov	r8, r2
	adds	r5, r3, r7
.L_080d337e:
	ldmia	r5!, {r0}
	bl	sub_08009038
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #12
	bne.n	.L_080d337e
	bl	sub_080cdbc0
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080ede48
	.4byte 0xffff8000
	.4byte 0x080ee1b4
	.4byte 0x00007828
	.4byte 0x0000ab85
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
	ldr	r6, [pc, #148]
	adds	r3, r6, #0
	ldmia	r3!, {r7}
	ldr	r1, [pc, #144]
	ldr	r3, [r3, #0]
	sub	sp, #36
	adds	r1, r1, r7
	str	r3, [sp, #20]
	str	r0, [r1, #0]
	movs	r0, #1
	mov	r8, r1
	bl	sub_080cd594
	ldr	r0, [pc, #128]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #124]
	adds	r1, r5, #0
	movs	r2, #128
	adds	r5, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [r6, #28]
	movs	r1, #7
	str	r2, [sp, #12]
	movs	r3, #15
	movs	r2, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #48]
	mov	r4, r8
	strh	r3, [r2, #0]
	ldr	r3, [r4, #0]
	ldr	r6, [r6, #32]
	ldr	r0, [r3, #8]
	str	r6, [sp, #16]
	bl	sub_080b5098
	ldr	r6, [r0, #0]
	mov	r0, r8
	ldr	r3, [r0, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	ldr	r3, [r6, #12]
	adds	r3, r3, r0
	mov	r9, r3
	movs	r3, #225
	movs	r1, #0
	movs	r2, #255
	lsls	r3, r3, #7
	mov	r8, r1
	mov	sl, r2
	adds	r5, r7, r3
	b.n	.L_080d3478
	.4byte 0x00000f0f
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x000000cd
	.4byte 0x03001388
	.2byte 0x0052
	.2byte 0x0400
.L_080d3478:
	ldr	r3, [r6, #8]
	mov	r4, r9
	str	r4, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #127
	lsls	r0, r0, #16
	asrs	r0, r0, #5
	str	r0, [r5, #12]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	subs	r3, #16
	lsls	r3, r3, #16
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	bl	sub_08004458
	mov	r2, sl
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #16
	asrs	r0, r0, #5
	movs	r3, #1
	movs	r1, #1
	str	r0, [r5, #20]
	negs	r3, r3
	mov	r0, r8
	ldr	r4, [pc, #868]
	add	r8, r1
	str	r3, [r5, #24]
	mov	r2, r8
	movs	r3, #0
	strb	r3, [r4, r0]
	adds	r5, #28
	cmp	r2, #30
	bne.n	.L_080d3478
	ldr	r4, [pc, #856]
	movs	r3, #0
	adds	r2, r7, r4
	mov	r8, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080d3512
	movs	r0, #232
	lsls	r0, r0, #7
	adds	r6, r2, #0
	movs	r1, #36
	adds	r5, r7, r0
.L_080d34e8:
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r1]
	str	r1, [sp, #8]
	bl	sub_080b5098
	ldr	r2, [r0, #0]
	ldr	r3, [r2, #8]
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r1, #2
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_080d34e8
.L_080d3512:
	ldr	r4, [pc, #792]
	ldr	r0, [pc, #792]
	adds	r3, r7, r4
	movs	r2, #0
	movs	r5, #144
	str	r2, [r3, #0]
	lsls	r5, r5, #3
	adds	r3, r7, r0
	str	r2, [r3, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #780]
	bl	sub_080041d8
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r7, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [pc, #768]
	adds	r2, r7, r3
	movs	r3, #75
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #760]
	bl	sub_080041d8
	movs	r0, #164
	bl	sub_080f9010
	ldr	r0, [pc, #728]
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #744]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r2, r3]
	movs	r4, #0
	mov	fp, r4
	cmp	r3, #0
	bne.n	.L_080d3566
	b.n	.L_080d37f4
.L_080d3566:
	ldr	r3, [pc, #732]
	ldr	r6, [r3, #0]
	mov	r3, fp
	subs	r3, #17
	cmp	r3, #46
	bhi.n	.L_080d357c
	ldr	r1, [pc, #696]
	movs	r3, #192
	adds	r2, r7, r1
	lsls	r3, r3, #1
	b.n	.L_080d3582
.L_080d357c:
	ldr	r3, [pc, #684]
	adds	r2, r7, r3
	movs	r3, #0
.L_080d3582:
	str	r3, [r2, #0]
	ldr	r0, [pc, #672]
	adds	r5, r7, r0
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r4, [pc, #688]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r4, r3]
	subs	r3, #16
	mov	sl, r4
	cmp	fp, r3
	bne.n	.L_080d35a2
	movs	r0, #132
	bl	sub_080b50e8
.L_080d35a2:
	bl	sub_080049ac
	adds	r1, r6, #0
	adds	r1, #12
	adds	r0, r6, #0
	bl	sub_080051d8
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	mov	r2, sl
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_080d35c4
	b.n	.L_080d37ac
.L_080d35c4:
	movs	r3, #225
	lsls	r3, r3, #7
	mov	sl, r5
	mov	r9, r1
	adds	r6, r7, r3
.L_080d35ce:
	cmp	fp, r9
	ble.n	.L_080d3634
	ldr	r4, [pc, #592]
	mov	r0, r8
	ldrsb	r3, [r4, r0]
	cmp	r3, #0
	bne.n	.L_080d3634
	add	r5, sp, #24
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r3, #159
	bgt.n	.L_080d35f6
	movs	r3, #160
	str	r3, [r5, #8]
.L_080d35f6:
	ldr	r2, [pc, #592]
	cmp	r3, r2
	ble.n	.L_080d35fe
	str	r2, [r5, #8]
.L_080d35fe:
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	movs	r1, #12
	movs	r4, #192
	str	r1, [sp, #0]
	lsls	r4, r4, #4
	movs	r1, #24
	subs	r2, #6
	subs	r3, #12
	str	r1, [sp, #4]
	ldr	r0, [sp, #20]
	adds	r1, r7, r4
	ldr	r4, [sp, #12]
	bl	sub_080072f4
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L_080d3634:
	mov	r3, r9
	adds	r3, #48
	cmp	fp, r3
	ble.n	.L_080d3730
	ldr	r0, [pc, #484]
	mov	r1, r8
	ldrsb	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L_080d3730
	mov	r2, sl
	ldr	r3, [r2, #0]
	mov	r0, r8
	ldr	r1, [r3, #20]
	bl	sub_080022fc
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsls	r1, r1, #2
	movs	r3, #232
	lsls	r3, r3, #7
	adds	r1, r7, r1
	adds	r1, r1, r3
	ldr	r3, [r1, #0]
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	ldr	r2, [r6, #12]
	asrs	r3, r3, #9
	adds	r0, r2, r3
	str	r0, [r6, #12]
	ldr	r2, [r6, #4]
	ldr	r3, [r1, #4]
	subs	r3, r3, r2
	ldr	r2, [r6, #16]
	asrs	r3, r3, #9
	adds	r4, r2, r3
	str	r4, [r6, #16]
	ldr	r2, [r6, #8]
	ldr	r3, [r1, #8]
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	asrs	r3, r3, #9
	adds	r1, r2, r3
	mov	r3, r9
	adds	r3, #85
	str	r1, [r6, #20]
	cmp	fp, r3
	bge.n	.L_080d36c2
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080d369e
	adds	r2, #63
.L_080d369e:
	asrs	r3, r2, #6
	str	r3, [r6, #12]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080d36ae
	adds	r2, #63
.L_080d36ae:
	asrs	r3, r2, #6
	str	r3, [r6, #16]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080d36be
	adds	r2, #63
.L_080d36be:
	asrs	r3, r2, #6
	str	r3, [r6, #20]
.L_080d36c2:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_080d3730
	ldr	r4, [pc, #344]
	movs	r3, #1
	mov	r0, r8
	strb	r3, [r4, r0]
	movs	r3, #0
	str	r3, [r6, #24]
	add	r5, sp, #24
	ldr	r3, [r5, #0]
	str	r3, [r6, #0]
	bl	sub_08004458
	ldr	r3, [r5, #4]
	movs	r2, #31
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #16
	mov	r1, sl
	ldr	r5, [r1, #0]
	str	r3, [r6, #4]
	mov	r0, r8
	ldr	r1, [r5, #20]
	bl	sub_080022fc
	adds	r3, r0, #0
	lsls	r2, r3, #1
	adds	r2, #36
	movs	r1, #4
	ldrsh	r0, [r5, r2]
	str	r1, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	bl	sub_080d6888
	mov	r2, sl
	ldr	r5, [r2, #0]
	mov	r0, r8
	ldr	r1, [r5, #20]
	bl	sub_080022fc
	lsls	r0, r0, #1
	adds	r0, #36
	ldrsh	r0, [r5, r0]
	movs	r1, #0
	bl	sub_080b5088
	ldr	r4, [pc, #296]
	movs	r0, #4
	adds	r3, r7, r4
	str	r0, [r3, #0]
	movs	r0, #132
	bl	sub_080f9010
.L_080d3730:
	ldr	r3, [r6, #24]
	cmp	r3, #15
	bhi.n	.L_080d3790
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	movs	r1, #3
	asrs	r0, r0, #1
	bl	sub_080022fc
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r1, r0, #0
	movs	r4, #16
	movs	r0, #64
	lsls	r1, r1, #10
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #12]
	subs	r2, #16
	ldr	r0, [sp, #20]
	adds	r1, r7, r1
	subs	r3, #56
	bl	sub_080072f4
	ldr	r0, [r6, #24]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	movs	r1, #3
	asrs	r0, r0, #1
	bl	sub_080022fc
	ldr	r3, [r6, #4]
	adds	r1, r0, #0
	movs	r4, #64
	movs	r0, #16
	lsls	r1, r1, #10
	ldr	r2, [r6, #0]
	subs	r3, #56
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	adds	r1, r7, r1
	ldr	r0, [sp, #20]
	ldr	r4, [sp, #16]
	bl	sub_080072f4
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
.L_080d3790:
	mov	r4, sl
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #168]
	lsls	r3, r3, #1
	movs	r1, #1
	ldrb	r3, [r2, r3]
	movs	r0, #2
	add	r8, r1
	add	r9, r0
	adds	r6, #28
	cmp	r8, r3
	beq.n	.L_080d37ac
	b.n	.L_080d35ce
.L_080d37ac:
	ldr	r0, [pc, #120]
	adds	r5, r7, r0
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #24]
	lsls	r1, r1, #1
	adds	r1, #2
	adds	r0, r1, #0
	bl	sub_080e155c
	ldr	r1, [pc, #112]
	adds	r2, r7, r1
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080d37cc
	movs	r3, #1
	str	r3, [r2, #0]
.L_080d37cc:
	bl	sub_080cd52c
	ldr	r3, [pc, #124]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #92]
	lsls	r3, r3, #1
	adds	r3, #1
	movs	r4, #1
	ldrb	r3, [r2, r3]
	add	fp, r4
	cmp	fp, r3
	beq.n	.L_080d37f4
	b.n	.L_080d3566
.L_080d37f4:
	ldr	r0, [pc, #60]
	bl	sub_08004278
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x00007828
	.4byte 0x000077ac
	.4byte 0x000077b0
	.4byte 0x080d6505
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080ee1c4
	.4byte 0x03001e80
	.4byte 0x0000031f
	.4byte 0x000077a8
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #92]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #52
	str	r1, [sp, #40]
	ldr	r2, [pc, #84]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #72]
	strh	r3, [r2, #0]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #64]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	ldr	r1, [sp, #40]
	adds	r0, r5, #0
	bl	sub_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #28]
	movs	r1, #7
	str	r3, [sp, #44]
	movs	r2, #7
	b.n	.L_080d38d4
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000ce
	.2byte 0x1388
	.2byte 0x0300
.L_080d38d4:
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #32]
	mov	r0, sp
	adds	r0, #44
	str	r0, [sp, #24]
	str	r3, [r0, #4]
	ldr	r2, [sp, #40]
	movs	r3, #225
	movs	r1, #0
	lsls	r3, r3, #7
	mov	r8, r1
	adds	r5, r2, r3
.L_080d38f4:
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	r8, r0
	negs	r3, r3
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_080d38f4
	ldr	r3, [sp, #40]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r3, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #40]
	ldr	r3, [pc, #76]
	adds	r2, r1, r3
	movs	r3, #50
	movs	r1, #144
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #68]
	bl	sub_080041d8
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #52]
	movs	r0, #1
	strh	r3, [r2, #0]
	bl	sub_080030f8
	movs	r0, #141
	bl	sub_080f9010
	ldr	r2, [pc, #52]
	ldr	r1, [sp, #40]
	adds	r2, r1, r2
	movs	r0, #0
	str	r2, [sp, #28]
	mov	fp, r0
.L_080d3960:
	mov	r3, fp
	lsls	r0, r3, #10
	bl	sub_08002322
	lsls	r0, r0, #4
	str	r0, [sp, #32]
	mov	r0, fp
	cmp	r0, #32
	bne.n	.L_080d3990
	movs	r0, #133
	bl	sub_080b50e8
	b.n	.L_080d3990
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x04000052
	.2byte 0x7828
	.2byte 0x0000
.L_080d3990:
	movs	r1, #0
	ldr	r6, [pc, #56]
	mov	r9, r1
	movs	r5, #16
.L_080d3998:
	cmp	fp, r5
	bne.n	.L_080d39a8
	movs	r1, #128
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #44]
	bl	sub_080072fc
.L_080d39a8:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r5, #8
	cmp	r3, #7
	bne.n	.L_080d3998
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d39d4
	ldr	r1, [sp, #32]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r1, r1, r2
	str	r1, [sp, #32]
	b.n	.L_080d39dc
	movs	r0, r0
	.4byte 0x03000168
	.2byte 0x0808
	.2byte 0x0808
.L_080d39d4:
	ldr	r3, [sp, #32]
	ldr	r0, [pc, #68]
	adds	r3, r3, r0
	str	r3, [sp, #32]
.L_080d39dc:
	mov	r1, fp
	cmp	r1, #16
	bgt.n	.L_080d39ec
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #56]
	mov	r3, fp
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_080d39ec:
	mov	r2, fp
	cmp	r2, #63
	ble.n	.L_080d3a00
	ldr	r2, [pc, #36]
	mov	r0, fp
	ldr	r1, [pc, #28]
	ldr	r3, [pc, #36]
	subs	r2, r2, r0
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_080d3a00:
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	ldr	r0, [pc, #28]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r0, r3]
	movs	r1, #0
	mov	r9, r1
	b.n	.L_080d3a28
	.4byte 0x00001000
	.4byte 0x0000004f
	.4byte 0xffe00000
	.4byte 0x04000052
	.2byte 0xe1ca
	.2byte 0x080e
.L_080d3a28:
	cmp	r3, #0
	bne.n	.L_080d3a2e
	b.n	.L_080d3b88
.L_080d3a2e:
	mov	r1, fp
	mov	r2, fp
	ldr	r3, [sp, #40]
	asrs	r1, r1, #31
	lsls	r2, r2, #11
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
.L_080d3a3e:
	ldr	r0, [sp, #16]
	bl	sub_08002322
	ldr	r1, [sp, #28]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #516]
	adds	r3, #1
	ldrb	r3, [r2, r3]
	muls	r3, r0
	ldr	r0, [sp, #32]
	adds	r3, r3, r0
	asrs	r3, r3, #16
	ldr	r0, [sp, #16]
	adds	r3, #40
	mov	sl, r3
	bl	sub_0800231c
	ldr	r1, [sp, #20]
	lsls	r0, r0, #1
	asrs	r7, r0, #16
	lsrs	r0, r1, #31
	add	r0, fp
	movs	r1, #3
	asrs	r0, r0, #1
	bl	sub_080022fc
	lsls	r5, r0, #2
	ldr	r2, [sp, #40]
	adds	r5, r5, r0
	lsls	r6, r5, #9
	ldr	r3, [pc, #468]
	adds	r6, r2, r6
	adds	r1, r6, r3
	movs	r0, #40
	movs	r2, #32
	adds	r3, r7, #0
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	adds	r3, #16
	mov	r2, sl
	bl	sub_080072f4
	ldr	r3, [sp, #40]
	ldr	r0, [pc, #444]
	lsls	r5, r5, #8
	adds	r5, r3, r5
	adds	r5, r5, r0
	movs	r1, #40
	movs	r2, #32
	adds	r3, r7, #0
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	adds	r3, #48
	ldr	r0, [sp, #36]
	adds	r1, r5, #0
	mov	r2, sl
	bl	sub_080072f4
	ldr	r3, [pc, #416]
	movs	r0, #40
	adds	r6, r6, r3
	movs	r1, #32
	adds	r3, r7, #0
	adds	r3, #80
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	mov	r2, sl
	adds	r1, r6, #0
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	bl	sub_080072f4
	movs	r0, #225
	ldr	r3, [sp, #12]
	movs	r2, #0
	lsls	r0, r0, #7
	mov	r8, r2
	adds	r6, r3, r0
.L_080d3ae6:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_080d3b32
	mov	r1, r8
	lsrs	r2, r1, #31
	add	r2, r8
	asrs	r2, r2, #1
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	asrs	r4, r4, #1
	movs	r2, #1
	mov	r5, r8
	adds	r4, r4, r3
	ands	r5, r2
	ldr	r0, [pc, #348]
	ldr	r2, [pc, #348]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldrb	r0, [r0, r4]
	ldr	r3, [sp, #40]
	ldr	r2, [r6, #0]
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #336]
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r5, r5, #2
	adds	r3, r3, r7
	ldr	r4, [r5, r0]
	add	r2, sl
	ldr	r0, [sp, #36]
	bl	sub_080072f4
	ldr	r3, [r6, #24]
.L_080d3b32:
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #6
	bne.n	.L_080d3b54
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	str	r3, [r6, #0]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r6, #4]
	movs	r3, #0
	str	r3, [r6, #24]
.L_080d3b54:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #4
	bne.n	.L_080d3ae6
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #12]
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r3, r3, r0
	adds	r1, #112
	str	r1, [sp, #12]
	str	r3, [sp, #16]
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	movs	r2, #1
	add	r9, r2
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #216]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r1, r3]
	cmp	r9, r3
	beq.n	.L_080d3b88
	b.n	.L_080d3a3e
.L_080d3b88:
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #20]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_080d3bec
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #212]
	movs	r3, #0
	adds	r7, r1, r2
	mov	sl, r3
	movs	r4, #36
.L_080d3ba2:
	movs	r0, #0
	mov	r5, sl
	mov	r9, r0
	adds	r6, r4, #0
	adds	r5, #16
.L_080d3bac:
	cmp	fp, r5
	bne.n	.L_080d3bd0
	ldr	r3, [r7, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	mov	r3, r8
	str	r4, [sp, #8]
	bl	sub_080d6888
	ldr	r3, [r7, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
	ldr	r4, [sp, #8]
.L_080d3bd0:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	adds	r5, #8
	cmp	r0, #7
	bne.n	.L_080d3bac
	add	r8, r3
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #20]
	movs	r1, #3
	add	sl, r1
	adds	r4, #2
	cmp	r8, r3
	bne.n	.L_080d3ba2
.L_080d3bec:
	ldr	r2, [sp, #40]
	ldr	r0, [pc, #132]
	movs	r1, #1
	adds	r3, r2, r0
	str	r1, [r3, #0]
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #80]
	adds	r3, #2
	ldrb	r0, [r1, r3]
	lsls	r1, r0, #1
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r0, [pc, #100]
	ldr	r2, [sp, #40]
	movs	r1, #1
	adds	r3, r2, r0
	str	r1, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #80
	beq.n	.L_080d3c2c
	b.n	.L_080d3960
.L_080d3c2c:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #64]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080ee1ca
	.4byte 0x00000c56
	.4byte 0x00002a56
	.4byte 0x00001156
	.4byte 0x080edeca
	.4byte 0x080edebe
	.4byte 0x080eded0
	.4byte 0x00007828
	.4byte 0x000077a8
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
	ldr	r3, [pc, #88]
	ldmia	r3!, {r1}
	ldr	r5, [pc, #88]
	mov	r9, r1
	ldr	r3, [r3, #0]
	sub	sp, #40
	add	r5, r9
	str	r3, [sp, #28]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #72]
	strh	r3, [r2, #0]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #32
	ldr	r0, [r3, #4]
	adds	r1, r2, #0
	str	r2, [sp, #20]
	bl	sub_080cef64
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #36]
	movs	r3, #50
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #28]
	lsls	r1, r1, #3
	b.n	.L_080d3d00
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000cf
	.4byte 0x00007784
	.2byte 0xd261
	.2byte 0x080c
.L_080d3d00:
	bl	sub_080041d8
	ldr	r4, [pc, #536]
	str	r4, [sp, #12]
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #1
	ldrb	r3, [r4, r3]
	cmp	r3, #0
	beq.n	.L_080d3d98
	movs	r5, #31
	mov	fp, r5
	ldr	r7, [pc, #516]
	movs	r5, #225
	lsls	r5, r5, #7
	movs	r0, #0
	add	r5, r9
	mov	r8, r0
	add	r7, r9
.L_080d3d2a:
	bl	sub_08004458
	ldr	r3, [pc, #504]
	str	r3, [r5, #4]
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d3d50
	bl	sub_08004458
	mov	r1, fp
	ands	r0, r1
	adds	r0, #80
	lsls	r6, r0, #16
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	b.n	.L_080d3d66
.L_080d3d50:
	bl	sub_08004458
	mov	r2, fp
	ands	r0, r2
	adds	r0, #8
	lsls	r6, r0, #16
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	negs	r3, r3
.L_080d3d66:
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #3
	adds	r3, r3, r2
	lsls	r3, r3, #1
	subs	r3, r6, r3
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #16]
	str	r3, [r5, #8]
	mov	r3, r8
	str	r3, [r5, #24]
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #408]
	lsls	r3, r3, #1
	movs	r0, #1
	ldrb	r3, [r1, r3]
	movs	r4, #8
	add	sl, r0
	adds	r5, #28
	add	r8, r4
	cmp	sl, r3
	bne.n	.L_080d3d2a
.L_080d3d98:
	movs	r2, #0
	str	r2, [sp, #24]
	ldr	r2, [pc, #388]
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #24]
	ldr	r5, [sp, #12]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_080d3db2
	b.n	.L_080d40a6
.L_080d3db2:
	ldr	r0, [pc, #368]
	add	r0, r9
	str	r0, [sp, #16]
.L_080d3db8:
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	bne.n	.L_080d3e02
	ldr	r2, [sp, #24]
	cmp	r2, #103
	bgt.n	.L_080d3e02
	ldr	r3, [pc, #352]
	ldr	r1, [r3, #0]
	ldr	r3, [sp, #24]
	movs	r2, #192
	cmp	r3, #95
	ble.n	.L_080d3de2
	ldr	r4, [sp, #24]
	lsls	r3, r4, #1
	adds	r3, r3, r4
	movs	r2, #156
	lsls	r3, r3, #3
	lsls	r2, r2, #4
	subs	r2, r2, r3
.L_080d3de2:
	ldr	r5, [sp, #16]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080d3df8
	ldrh	r3, [r1, #54]
	ldr	r0, [pc, #304]
	subs	r3, r3, r2
	strh	r3, [r1, #54]
	str	r0, [sp, #12]
	b.n	.L_080d3e02
.L_080d3df8:
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
	strh	r3, [r1, #54]
	ldr	r1, [pc, #288]
	str	r1, [sp, #12]
.L_080d3e02:
	ldr	r5, [pc, #288]
	add	r5, r9
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #24]
	ldr	r4, [sp, #12]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r4, r3]
	ldr	r0, [sp, #24]
	subs	r3, #80
	cmp	r0, r3
	bne.n	.L_080d3e22
	movs	r0, #134
	bl	sub_080b50e8
	ldr	r2, [r5, #0]
.L_080d3e22:
	ldr	r3, [r2, #24]
	ldr	r1, [sp, #12]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r1, r3]
	ldr	r4, [sp, #24]
	subs	r3, #8
	cmp	r4, r3
	bne.n	.L_080d3e48
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #240]
	ldr	r3, [pc, #240]
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [r5, #0]
.L_080d3e48:
	ldr	r3, [r2, #24]
	ldr	r5, [sp, #12]
	lsls	r2, r3, #1
	adds	r3, r2, #1
	ldrb	r3, [r5, r3]
	ldr	r0, [sp, #24]
	subs	r3, #8
	cmp	r0, r3
	ble.n	.L_080d3e5c
	b.n	.L_080d4060
.L_080d3e5c:
	ldrb	r3, [r5, r2]
	movs	r1, #0
	mov	fp, r1
	cmp	r3, #0
	bne.n	.L_080d3e68
	b.n	.L_080d4060
.L_080d3e68:
	movs	r2, #225
	lsls	r2, r2, #7
	add	r2, r9
	mov	r8, r2
.L_080d3e70:
	mov	r4, r8
	ldr	r3, [r4, #8]
	cmp	r3, #1
	bne.n	.L_080d3f48
	mov	r0, fp
	lsls	r2, r0, #4
	lsls	r3, r0, #7
	subs	r3, r3, r2
	ldr	r1, [pc, #180]
	movs	r5, #0
	lsls	r3, r3, #2
	mov	sl, r5
	adds	r7, r3, r1
.L_080d3e8a:
	movs	r1, #5
	mov	r0, sl
	bl	sub_080022fc
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #96
	ldr	r0, [r7, #24]
	bl	sub_080022ec
	movs	r1, #3
	bl	sub_080022fc
	movs	r2, #4
	mov	r3, sl
	adds	r6, r7, #0
	adds	r4, r5, r0
	mov	ip, r2
	cmp	r3, #2
	ble.n	.L_080d3eb6
	movs	r5, #0
	mov	ip, r5
.L_080d3eb6:
	ldr	r2, [pc, #132]
	lsls	r3, r4, #2
	ldr	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #124]
	ldrb	r5, [r3, r4]
	movs	r0, #128
	lsls	r0, r0, #4
	lsrs	r3, r5, #1
	add	r1, r9
	adds	r1, r1, r0
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	ldr	r0, [pc, #108]
	ldrb	r4, [r0, r4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r5, [sp, #0]
	ldr	r0, [sp, #20]
	str	r4, [sp, #4]
	mov	r5, ip
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #28]
	bl	sub_080072f4
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r0, r6, #0
	movs	r1, #64
	bl	sub_080e3908
	ldr	r3, [r6, #24]
	ldr	r2, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	cmp	r2, #1
	ble.n	.L_080d3f12
	ldr	r1, [sp, #24]
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080d3f12
	subs	r3, r2, #1
	str	r3, [r6, #8]
.L_080d3f12:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_080d3e8a
	b.n	.L_080d4046
	.4byte 0x080ee1f5
	.4byte 0x00007828
	.4byte 0xffc00000
	.4byte 0x03001e80
	.4byte 0x00007784
	.4byte 0x06060606
	.4byte 0x02010000
	.4byte 0x080ee214
	.4byte 0x080ee1fb
	.2byte 0xe207
	.2byte 0x080e
.L_080d3f48:
	mov	r4, r8
	ldr	r3, [r4, #24]
	ldr	r5, [sp, #24]
	cmp	r5, r3
	blt.n	.L_080d4046
	movs	r1, #6
	ldrsh	r3, [r4, r1]
	movs	r1, #2
	ldrsh	r2, [r4, r1]
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r5, #1
	movs	r1, #64
	mov	r6, fp
	str	r1, [sp, #4]
	ands	r6, r5
	ldr	r1, [sp, #20]
	lsls	r0, r6, #2
	ldr	r4, [r0, r1]
	subs	r2, #16
	ldr	r0, [sp, #28]
	mov	r1, r9
	bl	sub_080072f4
	movs	r2, #128
	lsls	r2, r2, #9
	mov	r0, r8
	movs	r1, #64
	bl	sub_080e3908
	mov	r2, r8
	ldr	r3, [r2, #4]
	movs	r2, #224
	lsls	r2, r2, #14
	cmp	r3, r2
	ble.n	.L_080d4046
	mov	r3, r8
	str	r5, [r3, #8]
	mov	r5, fp
	str	r2, [r3, #4]
	lsls	r2, r5, #4
	lsls	r3, r5, #7
	subs	r3, r3, r2
	ldr	r0, [pc, #304]
	movs	r4, #0
	lsls	r3, r3, #2
	ldr	r7, [pc, #300]
	mov	sl, r4
	movs	r1, #127
	adds	r5, r3, r0
.L_080d3fac:
	ldrb	r3, [r7, #0]
	mov	r4, r8
	ldr	r2, [r4, #0]
	subs	r3, #40
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldrb	r3, [r7, #1]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	str	r1, [sp, #8]
	bl	sub_08004458
	ldr	r1, [sp, #8]
	ands	r0, r1
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ldr	r1, [sp, #8]
	ands	r0, r1
	negs	r0, r0
	lsls	r3, r0, #11
	str	r3, [r5, #16]
	cmp	r6, #0
	beq.n	.L_080d3fec
	ldr	r3, [r5, #12]
	lsls	r3, r3, #1
	str	r3, [r5, #12]
	lsls	r3, r0, #12
	str	r3, [r5, #16]
.L_080d3fec:
	movs	r0, #1
	movs	r3, #32
	add	sl, r0
	str	r3, [r5, #8]
	mov	r2, sl
	movs	r3, #0
	str	r3, [r5, #24]
	adds	r7, #2
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_080d3fac
	ldr	r2, [pc, #212]
	movs	r3, #8
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #144
	bl	sub_080f9010
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [pc, #196]
	mov	r4, r9
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080d4046
	ldr	r5, [pc, #184]
	movs	r6, #36
	add	r5, r9
.L_080d4026:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, sl
	movs	r1, #7
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	sl, r2
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L_080d4026
.L_080d4046:
	ldr	r5, [sp, #16]
	movs	r3, #28
	add	r8, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r0, [pc, #140]
	lsls	r3, r3, #1
	movs	r4, #1
	ldrb	r3, [r0, r3]
	add	fp, r4
	cmp	fp, r3
	beq.n	.L_080d4060
	b.n	.L_080d3e70
.L_080d4060:
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r1, [r3, #24]
	lsls	r0, r1, #1
	lsls	r1, r1, #2
	adds	r0, #4
	adds	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #108]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #24]
	ldr	r3, [pc, #88]
	adds	r2, #1
	str	r2, [sp, #24]
	str	r3, [sp, #12]
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #24]
	ldr	r5, [pc, #72]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r5, r3]
	ldr	r0, [sp, #24]
	ldr	r2, [pc, #60]
	cmp	r0, r3
	beq.n	.L_080d40a6
	b.n	.L_080d3db8
.L_080d40a6:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x080ee1d3
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x080ee1f5
	.4byte 0x00007824
	.4byte 0x080cd261
