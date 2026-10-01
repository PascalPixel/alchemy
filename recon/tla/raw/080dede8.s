.syntax unified
	.thumb
	.global Func_080dede8
	.thumb_func
Func_080dede8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #52
	ldr r0, [r3, #20]
	ldr r5, [r3, #16]
	mov r10, r0
	movs r0, #150
	movs r1, #0
	mov r8, r3
	lsls r0, r0, #1
	movs r2, #0
	movs r3, #0
	str r1, [sp, #12]
	bl Object_Spawn
	adds r7, r0, #0
	movs r6, #0
	cmp r7, #0
	bne .L_080dee22
	b .L_080df156
.L_080dee22:
	bl BattleEffect_InitializeSharedScene
	movs r0, #138
	bl Audio_PlayCue
	mov r2, r8
	ldr r3, [r2, #20]
	cmp r3, #0
	bne .L_080dee48
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r2, #4]
	ldrh r1, [r2]
	ldr r3, [r5, #16]
	lsls r0, r0, #13
	str r3, [r2, #12]
	adds r2, #4
	bl Vector_AddPolarOffset
.L_080dee48:
	mov r3, sp
	adds r3, #28
	str r3, [sp, #8]
	ldr r4, [sp, #8]
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r4]
	lsls r0, r0, #13
	ldr r3, [r5, #12]
	mov r1, sp
	adds r3, r3, r0
	str r3, [r4, #4]
	adds r1, #16
	ldr r3, [r5, #16]
	mov r2, r8
	str r3, [r4, #8]
	str r1, [sp, #4]
	movs r4, #128
	ldr r3, [r2, #4]
	lsls r4, r4, #14
	str r3, [r1]
	mov r0, r8
	ldr r2, [r2, #8]
	adds r3, r2, r4
	str r3, [r1, #4]
	ldr r3, [r0, #12]
	str r3, [r1, #8]
	mov r3, r8
	adds r3, #52
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080dee96
	movs r1, #160
	lsls r1, r1, #15
	adds r3, r2, r1
	ldr r2, [sp, #4]
	str r3, [r2, #4]
.L_080dee96:
	mov r0, r10
	bl Func_08020330
	movs r3, #190
	lsls r3, r3, #1
	cmp r0, r3
	bne .L_080deeb4
	ldr r4, [sp, #4]
	movs r0, #128
	ldr r3, [r4, #4]
	lsls r0, r0, #14
	adds r3, r3, r0
	movs r1, #1
	str r3, [r4, #4]
	str r1, [sp, #12]
.L_080deeb4:
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	mov r9, r2
	mov r10, r3
.L_080deebc:
	mov r4, r10
	mov r0, r9
	ldr r3, [r4]
	ldr r5, [r0]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	adds r5, r5, r0
	str r5, [r7, #8]
	mov r2, r9
	mov r1, r10
	ldr r5, [r2, #4]
	ldr r3, [r1, #4]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	adds r5, r5, r0
	str r5, [r7, #12]
	mov r4, r10
	mov r0, r9
	ldr r3, [r4, #8]
	ldr r5, [r0, #8]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	movs r3, #192
	lsls r3, r3, #8
	adds r5, r5, r0
	movs r1, #10
	adds r0, r6, #0
	muls r0, r3
	str r5, [r7, #16]
	bl Math_Div
	movs r1, #128
	lsls r1, r1, #7
	adds r0, r0, r1
	str r0, [r7, #24]
	str r0, [r7, #28]
	adds r6, #1
	movs r0, #1
	bl WaitFrames
	cmp r6, #11
	blt .L_080deebc
	movs r0, #10
	bl WaitFrames
	mov r3, r8
	adds r3, #65
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080df038
	mov r3, r8
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #10
	mov r11, r2
	cmp r3, #0
	bne .L_080def52
	movs r3, #24
	mov r11, r3
.L_080def52:
	movs r4, #0
	mov r10, r4
	cmp r10, r11
	bge .L_080df030
	mov r0, r11
	subs r0, #1
	str r0, [sp, #0]
	add r6, sp, #40
	mov r9, r6
.L_080def64:
	ldr r3, [r7, #8]
	mov r1, r9
	str r3, [r1]
	ldr r3, [r7, #12]
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	str r3, [r1, #8]
	bl Random16
	movs r2, #192
	lsls r5, r0, #2
	lsls r2, r2, #10
	adds r5, r5, r0
	adds r5, r5, r2
	bl Random16
	mov r2, r9
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	ldr r3, [sp, #0]
	cmp r10, r3
	bne .L_080defa6
	movs r0, #20
	bl WaitFrames
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
.L_080defa6:
	movs r0, #46
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	adds r0, #255
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080deff0
	mov r4, r8
	ldr r2, [r4, #20]
	cmp r2, #0
	beq .L_080defd6
	ldr r3, [r2, #12]
	str r3, [r5, #20]
	ldr r0, [sp, #12]
	cmp r0, #1
	bne .L_080defde
	ldr r3, [r2, #12]
	movs r1, #128
	lsls r1, r1, #14
	adds r3, r3, r1
	b .L_080defdc
.L_080defd6:
	ldr r3, [r6, #4]
	ldr r2, .L_080df164
	adds r3, r3, r2
.L_080defdc:
	str r3, [r5, #20]
.L_080defde:
	ldr r3, .L_080df168
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	subs r2, #50
	movs r3, #5
	strb r3, [r2]
.L_080deff0:
	movs r0, #132
	bl Audio_PlayCue
	movs r0, #6
	bl WaitFrames
	mov r3, r10
	cmp r3, #12
	bne .L_080df028
	movs r0, #5
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #160
	lsls r0, r0, #23
	adds r0, #5
	movs r1, #5
	bl Func_080ce458
	cmp r0, #0
	beq .L_080df028
	mov r2, r8
	movs r4, #24
	ldrsh r1, [r2, r4]
	movs r3, #26
	ldrsh r2, [r2, r3]
	bl Func_080ceafc
.L_080df028:
	movs r4, #1
	add r10, r4
	cmp r10, r11
	blt .L_080def64
.L_080df030:
	movs r0, #10
	bl WaitFrames
	b .L_080df0d8
.L_080df038:
	mov r3, r8
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r0, #10
	mov r11, r0
	cmp r3, #0
	bne .L_080df04e
	movs r1, #30
	mov r11, r1
.L_080df04e:
	mov r2, r11
	cmp r2, #0
	beq .L_080df0d2
	add r6, sp, #40
	mov r10, r11
.L_080df058:
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
	bl Random16
	movs r3, #192
	lsls r5, r0, #2
	lsls r3, r3, #10
	adds r5, r5, r0
	adds r5, r5, r3
	bl Random16
	adds r2, r6, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	movs r0, #168
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	lsls r0, r0, #2
	bl Object_Spawn
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080df0c0
	ldr r3, .L_080df16c
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r4, #13
	ldr r1, [r5, #80]
	negs r4, r4
	ldrb r2, [r1, #9]
	adds r3, r4, #0
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	strb r2, [r1, #9]
	movs r1, #8
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #7
	bl Animation_ApplyChildValuesFar
.L_080df0c0:
	movs r0, #6
	bl WaitFrames
	movs r0, #1
	negs r0, r0
	add r10, r0
	mov r1, r10
	cmp r1, #0
	bne .L_080df058
.L_080df0d2:
	movs r0, #70
	bl WaitFrames
.L_080df0d8:
	ldr r2, [sp, #4]
	ldr r3, [sp, #8]
	movs r6, #0
	mov r10, r2
	mov r8, r3
.L_080df0e2:
	mov r4, r8
	mov r0, r10
	ldr r3, [r4]
	ldr r5, [r0]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	adds r5, r5, r0
	str r5, [r7, #8]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #4]
	ldr r3, [r1, #4]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	adds r5, r5, r0
	str r5, [r7, #12]
	mov r4, r8
	mov r0, r10
	ldr r3, [r4, #8]
	ldr r5, [r0, #8]
	movs r1, #10
	subs r3, r3, r5
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	ldr r3, .L_080df170
	adds r5, r5, r0
	movs r1, #10
	adds r0, r6, #0
	muls r0, r3
	str r5, [r7, #16]
	bl Math_Div
	movs r1, #128
	lsls r1, r1, #9
	adds r0, r0, r1
	str r0, [r7, #24]
	str r0, [r7, #28]
	adds r6, #1
	movs r0, #1
	bl WaitFrames
	cmp r6, #11
	blt .L_080df0e2
	adds r0, r7, #0
	bl Object_Destroy
	bl BattleFx_PrepareBufferInterpolation
.L_080df156:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080df164:
	.4byte 0xffe00000
.L_080df168:
	.4byte Func_080ded84
.L_080df16c:
	.4byte BattleFx_UpdateDriftingFallObject
.L_080df170:
	.4byte 0xffff4000
