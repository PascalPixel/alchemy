.syntax unified
	.thumb
	.global Func_0810106c
	.thumb_func
Func_0810106c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r1, #0
	movs r4, #0
	mov r8, r1
	ldr r1, [r7, #20]
	mov r10, r4
	movs r3, #13
	mov r2, r10
	sub sp, #8
	strb r3, [r1, #5]
	strh r2, [r1, #12]
	str r4, [sp, #0]
	bl Func_08101638
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #0]
	movs r5, #2
.L_081010a0:
	cmp r5, #15
	bls .L_081010a6
	b .L_08101598
.L_081010a6:
	ldr r2, .L_081013ac
	lsls r3, r5, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_081010b0:
	.4byte .L_081010f0
	.4byte .L_08101598
	.4byte .L_08101102
	.4byte .L_081011b6
	.4byte .L_08101598
	.4byte .L_08101514
	.4byte .L_081013b0
	.4byte .L_081012be
	.4byte .L_08101598
	.4byte .L_08101416
	.4byte .L_08101142
	.4byte .L_08101544
	.4byte .L_081013e2
	.4byte .L_081012f0
	.4byte .L_08101448
	.4byte .L_0810118a
.L_081010f0:
	cmp r4, #0
	blt .L_081010f6
	b .L_08101510
.L_081010f6:
	movs r3, #1
	negs r3, r3
	movs r1, #1
	mov r10, r3
	mov r8, r1
	b .L_08101510
.L_08101102:
	movs r0, #0
	bl Func_08100e34
	movs r0, #1
	movs r1, #0
	movs r2, #200
	bl Func_08105300
	movs r0, #0
	bl Func_08102008
	adds r4, r0, #0
	movs r5, #15
	cmp r4, #10
	bne .L_08101122
	b .L_0810159c
.L_08101122:
	movs r5, #0
	cmp r4, #0
	bge .L_0810112a
	b .L_0810159c
.L_0810112a:
	movs r3, #28
	ldrsb r3, [r7, r3]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r7, r1
	strh r3, [r2]
	movs r5, #10
	cmp r4, #7
	bne .L_0810113e
	b .L_0810159c
.L_0810113e:
	movs r5, #3
	b .L_0810159c
.L_08101142:
	movs r3, #28
	ldrsb r3, [r7, r3]
	movs r2, #129
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r2, [r7, r3]
	movs r1, #128
	str r2, [r7, #8]
	ldrh r2, [r7, r3]
	lsls r1, r1, #2
	adds r1, #22
	adds r3, r7, r1
	strb r2, [r3]
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r1, [r3]
	movs r2, #13
	strb r2, [r1, #5]
	movs r1, #190
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	strb r2, [r1, #5]
	bl Func_08104540
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	beq .L_08101184
	b .L_08101510
.L_08101184:
	movs r3, #1
	mov r8, r3
	b .L_08101510
.L_0810118a:
	movs r1, #188
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	movs r2, #13
	strb r2, [r1, #5]
	movs r1, #190
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	strb r2, [r1, #5]
	bl Func_08101d5c
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	beq .L_081011b0
	b .L_08101510
.L_081011b0:
	movs r3, #1
	mov r8, r3
	b .L_08101510
.L_081011b6:
	movs r0, #8
	negs r0, r0
	bl Func_08100e34
	movs r3, #28
	ldrsb r3, [r7, r3]
	movs r1, #129
	lsls r1, r1, #2
	lsls r3, r3, #1
	adds r3, r3, r1
	ldrh r2, [r7, r3]
	adds r1, #18
	str r2, [r7, #8]
	ldrh r2, [r7, r3]
	adds r3, r7, r1
	strb r2, [r3]
	movs r0, #0
	movs r3, #28
	ldrsb r3, [r7, r3]
	movs r2, #54
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #3
	adds r1, #48
	bl Func_08105300
	movs r0, #1
	bl Func_08102008
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r7, r2
	ldrb r3, [r3]
	movs r1, #0
	adds r4, r0, #0
	cmp r1, r3
	bge .L_08101216
	adds r0, r7, r2
	subs r2, #237
.L_08101206:
	ldrh r3, [r2, r7]
	adds r1, #1
	adds r3, #8
	strh r3, [r2, r7]
	adds r2, #2
	ldrb r3, [r0]
	cmp r1, r3
	blt .L_08101206
.L_08101216:
	movs r3, #2
	negs r3, r3
	cmp r4, r3
	bne .L_08101222
	movs r1, #1
	mov r8, r1
.L_08101222:
	cmp r4, #0
	bge .L_08101228
	b .L_08101510
.L_08101228:
	subs r3, r4, #3
	cmp r3, #1
	bls .L_08101236
	cmp r4, #8
	beq .L_08101236
	cmp r4, #9
	bne .L_0810124e
.L_08101236:
	movs r3, #29
	ldrsb r3, [r7, r3]
	movs r2, #129
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	movs r1, #140
	ldrh r2, [r7, r3]
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r7, r1
	strb r2, [r3]
.L_0810124e:
	cmp r4, #0
	bge .L_08101254
	b .L_08101510
.L_08101254:
	cmp r4, #1
	bne .L_0810125c
	movs r5, #5
	b .L_0810159c
.L_0810125c:
	cmp r4, #2
	bne .L_08101264
	movs r5, #6
	b .L_0810159c
.L_08101264:
	cmp r4, #3
	bne .L_08101276
	movs r3, #135
	lsls r3, r3, #2
	adds r2, r7, r3
	movs r3, #2
	strh r3, [r2]
	movs r5, #7
	b .L_0810159c
.L_08101276:
	cmp r4, #4
	bne .L_08101288
	movs r1, #135
	lsls r1, r1, #2
	adds r2, r7, r1
	movs r3, #2
	strh r3, [r2]
	movs r5, #9
	b .L_0810159c
.L_08101288:
	cmp r4, #5
	bne .L_08101290
	movs r5, #11
	b .L_0810159c
.L_08101290:
	cmp r4, #6
	bne .L_08101298
	movs r5, #12
	b .L_0810159c
.L_08101298:
	cmp r4, #8
	bne .L_081012aa
	movs r3, #135
	lsls r3, r3, #2
	adds r2, r7, r3
	movs r3, #2
	strh r3, [r2]
	movs r5, #13
	b .L_0810159c
.L_081012aa:
	cmp r4, #9
	beq .L_081012b0
	b .L_0810159c
.L_081012b0:
	movs r1, #135
	lsls r1, r1, #2
	adds r2, r7, r1
	movs r3, #2
	strh r3, [r2]
	movs r5, #14
	b .L_0810159c
.L_081012be:
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r1, [r3]
	movs r2, #13
	strb r2, [r1, #5]
	movs r1, #190
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	movs r0, #1
	strb r2, [r1, #5]
	bl Func_081039fc
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	bne .L_081012e8
	movs r3, #1
	mov r8, r3
.L_081012e8:
	movs r5, #3
	cmp r4, #0
	bge .L_081012f0
	b .L_0810159c
.L_081012f0:
	movs r0, #126
	bl Audio_PlayCue
	movs r2, #128
	movs r1, #128
	lsls r2, r2, #2
	adds r2, #90
	lsls r1, r1, #2
	adds r3, r7, r2
	adds r1, #22
	subs r2, #2
	adds r6, r7, r1
	ldrb r1, [r3]
	adds r3, r7, r2
	ldrb r2, [r3]
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r7, r3
	ldrb r3, [r5]
	ldrb r0, [r6]
	bl Djinn_TransferFar
	adds r4, r0, #0
	ldrb r0, [r6]
	str r4, [sp, #0]
	bl Owner_RecalculateStatsFar
	ldrb r0, [r5]
	bl Owner_RecalculateStatsFar
	ldr r2, [r7, #20]
	movs r3, #13
	strb r3, [r2, #5]
	ldr r0, [r7, #52]
	bl RenderOutput_ClearListFar
	movs r1, #192
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r0, [r3]
	bl Func_081019a4
	movs r2, #181
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r0, [r3]
	movs r1, #10
	bl __umodsi3
	movs r1, #182
	movs r3, #0
	lsls r1, r1, #1
	mov r12, r3
	adds r3, r7, r1
	ldrb r6, [r3]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	movs r1, #0
	adds r5, r0, #0
	adds r5, #160
	ldr r4, [sp, #0]
	b .L_08101370
.L_0810136e:
	adds r1, #1
.L_08101370:
	movs r2, #192
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r2, [r3]
	ldrsb r3, [r2, r5]
	cmp r1, r3
	bge .L_08101390
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	ldrb r3, [r2, r3]
	cmp r6, r3
	bne .L_0810136e
	mov r12, r1
.L_08101390:
	mov r1, r12
	lsls r3, r1, #2
	add r3, r12
	movs r1, #180
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r0, r3
	adds r2, r7, r1
	strh r3, [r2]
	ldr r2, [r7, #20]
	movs r3, #1
	strb r3, [r2, #5]
	movs r5, #0
	b .L_0810159c
.L_081013ac:
	.4byte .L_081010b0
.L_081013b0:
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r1, [r3]
	movs r2, #13
	strb r2, [r1, #5]
	movs r1, #190
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	movs r0, #2
	strb r2, [r1, #5]
	bl Func_081039fc
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	bne .L_081013da
	movs r3, #1
	mov r8, r3
.L_081013da:
	movs r5, #3
	cmp r4, #0
	bge .L_081013e2
	b .L_0810159c
.L_081013e2:
	movs r0, #175
	bl Audio_PlayCue
	movs r2, #128
	movs r1, #128
	lsls r2, r2, #2
	adds r2, #90
	lsls r1, r1, #2
	adds r3, r7, r2
	adds r1, #22
	subs r2, #2
	adds r6, r7, r1
	adds r5, r7, r2
	ldrb r1, [r3]
	ldrb r2, [r5]
	ldrb r0, [r6]
	str r3, [sp, #4]
	bl Djinn_DeactivateFar
	ldr r3, [sp, #4]
	ldrb r2, [r5]
	ldrb r1, [r3]
	ldrb r0, [r6]
	bl Trade_AddOfferFar
	b .L_08101576
.L_08101416:
	movs r1, #188
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	movs r2, #13
	strb r2, [r1, #5]
	movs r1, #190
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	movs r0, #0
	strb r2, [r1, #5]
	bl Func_081039fc
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	bne .L_08101440
	movs r3, #1
	mov r8, r3
.L_08101440:
	movs r5, #3
	cmp r4, #0
	bge .L_08101448
	b .L_0810159c
.L_08101448:
	movs r0, #126
	bl Audio_PlayCue
	movs r2, #128
	movs r1, #128
	lsls r2, r2, #2
	adds r2, #90
	lsls r1, r1, #2
	adds r3, r7, r2
	adds r1, #22
	subs r2, #2
	adds r6, r7, r1
	ldrb r1, [r3]
	adds r3, r7, r2
	ldrb r2, [r3]
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r7, r3
	ldrb r3, [r5]
	ldrb r0, [r6]
	bl Djinn_TransferFar
	movs r1, #174
	lsls r1, r1, #1
	movs r2, #173
	adds r1, #255
	lsls r2, r2, #1
	adds r3, r7, r1
	adds r2, #255
	ldrb r1, [r3]
	adds r3, r7, r2
	ldrb r2, [r3]
	ldrb r0, [r5]
	ldrb r3, [r6]
	bl Djinn_TransferFar
	adds r4, r0, #0
	ldrb r0, [r6]
	str r4, [sp, #0]
	bl Owner_RecalculateStatsFar
	ldrb r0, [r5]
	bl Owner_RecalculateStatsFar
	movs r1, #192
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r0, [r3]
	bl Func_081019a4
	movs r2, #181
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r0, [r3]
	movs r1, #10
	bl __umodsi3
	movs r1, #182
	movs r3, #0
	lsls r1, r1, #1
	mov r12, r3
	adds r3, r7, r1
	ldrb r6, [r3]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	movs r1, #0
	adds r5, r0, #0
	adds r5, #160
	ldr r4, [sp, #0]
	b .L_081014d8
.L_081014d6:
	adds r1, #1
.L_081014d8:
	movs r2, #192
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r2, [r3]
	ldrsb r3, [r2, r5]
	cmp r1, r3
	bge .L_081014f8
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	ldrb r3, [r2, r3]
	cmp r6, r3
	bne .L_081014d6
	mov r12, r1
.L_081014f8:
	mov r1, r12
	lsls r3, r1, #2
	add r3, r12
	movs r1, #180
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r0, r3
	adds r2, r7, r1
	strh r3, [r2]
	ldr r2, [r7, #20]
	movs r3, #1
	strb r3, [r2, #5]
.L_08101510:
	movs r5, #2
	b .L_0810159c
.L_08101514:
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r1, [r3]
	movs r2, #13
	strb r2, [r1, #5]
	movs r1, #190
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r1, [r3]
	movs r0, #3
	strb r2, [r1, #5]
	bl Func_081039fc
	movs r2, #2
	adds r4, r0, #0
	negs r2, r2
	cmp r4, r2
	bne .L_0810153e
	movs r3, #1
	mov r8, r3
.L_0810153e:
	movs r5, #3
	cmp r4, #0
	blt .L_0810159c
.L_08101544:
	movs r0, #139
	bl Audio_PlayCue
	movs r2, #128
	movs r1, #128
	lsls r2, r2, #2
	adds r2, #90
	lsls r1, r1, #2
	adds r3, r7, r2
	adds r1, #22
	subs r2, #2
	adds r6, r7, r1
	adds r5, r7, r2
	ldrb r1, [r3]
	ldrb r2, [r5]
	ldrb r0, [r6]
	str r3, [sp, #4]
	bl Djinn_ActivateFar
	ldr r3, [sp, #4]
	ldrb r2, [r5]
	ldrb r1, [r3]
	ldrb r0, [r6]
	bl Trade_RemoveOfferFar
.L_08101576:
	adds r4, r0, #0
	ldrb r0, [r6]
	str r4, [sp, #0]
	bl Owner_RecalculateStatsFar
	ldr r2, [r7, #20]
	movs r3, #13
	strb r3, [r2, #5]
	ldr r0, [r7, #52]
	bl RenderOutput_ClearListFar
	ldr r2, [r7, #20]
	movs r3, #1
	strb r3, [r2, #5]
	movs r5, #2
	ldr r4, [sp, #0]
	b .L_0810159c
.L_08101598:
	movs r3, #1
	mov r8, r3
.L_0810159c:
	mov r1, r8
	cmp r1, #0
	bne .L_081015a4
	b .L_081010a0
.L_081015a4:
	mov r0, r10
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
