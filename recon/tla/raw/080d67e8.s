.syntax unified
	.thumb
	.global Func_080d67e8
	.thumb_func
Func_080d67e8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #120]
	movs r1, #252
	lsls r1, r1, #5
	adds r3, #128
	adds r5, r6, r1
	ldr r7, [r3]
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bge .L_080d6804
	b .L_080d6902
.L_080d6804:
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080d6814
	movs r3, #128
	strh r3, [r5]
.L_080d6814:
	ldrh r3, [r5]
	subs r2, r3, #1
	lsls r3, r3, #16
	asrs r3, r3, #16
	strh r2, [r5]
	cmp r3, #11
	bhi .L_080d6902
	ldr r2, .L_080d6904
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080d682c:
	.4byte .L_080d685c
	.4byte .L_080d68de
	.4byte .L_080d6902
	.4byte .L_080d6902
	.4byte .L_080d6902
	.4byte .L_080d68b2
	.4byte .L_080d68de
	.4byte .L_080d6902
	.4byte .L_080d6902
	.4byte .L_080d6902
	.4byte .L_080d68b2
	.4byte .L_080d68de
.L_080d685c:
	movs r1, #248
	lsls r1, r1, #5
	adds r1, #130
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080d68b2
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r3, r5, #1
	adds r3, r3, r5
	movs r2, #100
	lsls r3, r3, #3
	muls r2, r0
	adds r3, r3, r5
	lsls r3, r3, #4
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	movs r1, #252
	subs r3, r3, r2
	lsls r1, r1, #5
	adds r2, r6, r1
	adds r3, #150
	strh r3, [r2]
	movs r2, #248
	lsls r2, r2, #5
	adds r2, #132
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080d68ac
	movs r0, #172
	bl Audio_PlayCue
	b .L_080d68b2
.L_080d68ac:
	movs r0, #171
	bl Audio_PlayCue
.L_080d68b2:
	adds r0, r6, #0
	movs r1, #1
	bl Func_080d170c
	movs r2, #168
	lsls r2, r2, #5
	adds r0, r6, r2
	movs r3, #128
	movs r2, #196
	lsls r2, r2, #5
	lsls r3, r3, #19
	adds r1, r7, r2
	adds r3, #212
	ldr r2, .L_080d6908
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #168
	lsls r3, r3, #6
	adds r3, #1
	adds r2, r7, r3
	movs r3, #12
	b .L_080d68f4
.L_080d68de:
	movs r3, #168
	lsls r3, r3, #4
	adds r0, r6, r3
	movs r1, #1
	bl Func_080d170c
	movs r1, #168
	lsls r1, r1, #6
	adds r1, #1
	adds r2, r7, r1
	movs r3, #1
.L_080d68f4:
	strb r3, [r2]
	movs r2, #168
	lsls r2, r2, #6
	adds r2, #2
	movs r1, #0
	adds r3, r7, r2
	strb r1, [r3]
.L_080d6902:
	pop {r5, r6, r7, pc}
.L_080d6904:
	.4byte .L_080d682c
.L_080d6908:
	.4byte 0x840002a0
