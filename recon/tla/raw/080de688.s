.syntax unified
	.thumb
	.global Func_080de688
	.thumb_func
Func_080de688:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #12
	ldr r2, [r3, #20]
	mov r8, r3
	mov r9, r2
	bl BattleEffect_InitializeSharedScene
	movs r0, #115
	bl Audio_PlayCue
	movs r3, #15
	mov r7, sp
	mov r10, r3
.L_080de6b2:
	movs r0, #213
	lsls r0, r0, #1
	adds r0, #255
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_Spawn
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080de742
	bl Random16
	movs r4, #128
	lsls r4, r4, #8
	lsrs r0, r0, #1
	adds r0, r0, r4
	str r0, [r6, #28]
	str r0, [r6, #24]
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #0
	beq .L_080de6e8
	ldr r3, .L_080de80c
	b .L_080de6ea
.L_080de6e8:
	ldr r3, .L_080de810
.L_080de6ea:
	str r3, [r6, #108]
	bl Random16
	adds r2, r6, #0
	adds r2, #100
	movs r3, #60
	strh r0, [r6, #6]
	strh r3, [r2]
	bl Random16
	adds r3, r6, #0
	adds r3, #102
	movs r1, #9
	strh r0, [r3]
	adds r0, r6, #0
	bl Animation_ApplyChildValuesFar
	mov r2, r8
	ldr r3, [r2, #4]
	str r3, [r7]
	ldr r3, [r2, #8]
	str r3, [r7, #4]
	ldr r3, [r2, #12]
	str r3, [r7, #8]
	bl Random16
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #10
	lsls r5, r5, #2
	adds r5, r5, r3
	bl Random16
	adds r2, r7, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	ldr r3, [r7]
	str r3, [r6, #56]
	ldr r3, [r7, #4]
	str r3, [r6, #60]
	ldr r3, [r7, #8]
	str r3, [r6, #64]
.L_080de742:
	movs r0, #3
	bl WaitFrames
	movs r4, #1
	negs r4, r4
	add r10, r4
	mov r2, r10
	cmp r2, #0
	bge .L_080de6b2
	movs r0, #10
	bl WaitFrames
	movs r0, #115
	bl Audio_PlayCue
	movs r0, #50
	bl WaitFrames
	mov r3, r9
	cmp r3, #0
	beq .L_080de7fa
	mov r3, r8
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080de7fa
	movs r0, #212
	bl Audio_PlayCue
	movs r4, #7
	mov r10, r4
.L_080de784:
	movs r1, #7
	mov r0, r9
	bl Animation_ApplyChildValuesFar
	movs r0, #1
	bl WaitFrames
	mov r0, r9
	movs r1, #0
	bl Animation_ApplyChildValuesFar
	movs r0, #4
	bl WaitFrames
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	cmp r3, #0
	bge .L_080de784
	mov r3, r8
	adds r3, #52
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080de7c8
	movs r0, #220
	bl Audio_PlayCue
	mov r0, r9
	movs r1, #2
	bl Object_SetMode
.L_080de7c8:
	ldr r3, .L_080de814
	mov r4, r9
	str r3, [r4, #108]
	movs r0, #6
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #160
	lsls r0, r0, #23
	adds r0, #5
	movs r1, #6
	bl Func_080ce458
	cmp r0, #0
	beq .L_080de7f4
	mov r3, r8
	movs r2, #24
	ldrsh r1, [r3, r2]
	movs r4, #26
	ldrsh r2, [r3, r4]
	bl Func_080ceafc
.L_080de7f4:
	movs r0, #20
	bl WaitFrames
.L_080de7fa:
	bl BattleFx_PrepareBufferInterpolation
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080de80c:
	.4byte Func_080de5a4
.L_080de810:
	.4byte Func_080de5fc
.L_080de814:
	.4byte Func_080de550
