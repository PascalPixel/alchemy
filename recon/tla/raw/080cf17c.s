.syntax unified
	.thumb
	.global BattleFx_StartRandomParticleEmitter
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r6, #192
	lsls r6, r6, #18
	adds r5, r0, #0
	ldr r0, [r6, #108]
	lsls r3, r5, #2
	adds r3, #20
	ldr r7, [r0, r3]
	mov r10, r0
	sub sp, #12
	mov r9, r1
	movs r0, #0
	cmp r7, #0
	bne .L_080cf1a2
	b .L_080cf32e
.L_080cf1a2:
	ldr r3, [r7, #8]
	mov r8, sp
	str r3, [sp, #0]
	ldr r3, [r7, #12]
	str r3, [sp, #4]
	ldr r3, [r7, #16]
	str r3, [sp, #8]
	bl EventRuntime_GetControlledOwner
	cmp r5, r0
	bne .L_080cf1de
	movs r0, #128
	ldrh r1, [r7, #6]
	mov r2, r8
	lsls r0, r0, #13
	bl Vector_AddPolarOffset
	mov r2, r8
	ldr r3, [r2]
	ldr r1, .L_080cf2e4
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	mov r4, r8
	adds r3, r3, r2
	str r3, [r4]
	ldr r3, [r4, #8]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r4, #8]
.L_080cf1de:
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #164
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080cf21e
	ldr r5, [r6, #20]
	movs r6, #63
.L_080cf1f4:
	ldr r1, [r5]
	cmp r1, #0
	beq .L_080cf216
	ldr r2, [r5, #108]
	ldr r3, .L_080cf2e8
	cmp r2, r3
	bne .L_080cf20a
	adds r0, r5, #0
	bl Object_Destroy
	ldr r1, [r5]
.L_080cf20a:
	ldr r3, .L_080cf2ec
	cmp r1, r3
	bne .L_080cf216
	adds r0, r5, #0
	bl Object_Destroy
.L_080cf216:
	subs r6, #1
	adds r5, #128
	cmp r6, #0
	bge .L_080cf1f4
.L_080cf21e:
	movs r0, #3
	bl WaitFrames
	mov r2, r8
	ldr r1, [r2]
	ldr r2, [r2, #4]
	movs r3, #128
	lsls r3, r3, #13
	mov r4, r8
	movs r0, #234
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r4, #8]
	bl Func_080200c0
	adds r7, r0, #0
	movs r0, #0
	cmp r7, #0
	beq .L_080cf32e
	ldr r1, .L_080cf2f0
	adds r0, r7, #0
	bl ObjectDispatch_InitializeFar
	ldr r6, [r7, #80]
	movs r3, #0
	ldrb r2, [r6, #5]
	strb r3, [r6, #26]
	strb r3, [r6, #27]
	subs r3, #33
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r7, #0
	strb r3, [r6, #9]
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	mov r0, r8
	ldr r3, [r0, #4]
	movs r1, #193
	str r3, [r7, #20]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #40]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #72]
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlock
	adds r5, r0, #0
	mov r0, r9
	bl Func_08038248
	movs r2, #128
	lsls r2, r2, #3
	adds r5, r5, r2
	movs r1, #128
	adds r2, r5, #0
	ldrb r0, [r6, #16]
	bl VramBlock_LoadCached
	ldr r3, .L_080cf2e0
	ldrh r2, [r6, #8]
	ands r0, r3
	ldr r3, .L_080cf2f4
	ands r3, r2
	orrs r3, r0
	strh r3, [r6, #8]
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_080cf2f8
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #164
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080cf32c
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #20]
	movs r1, #0
	b .L_080cf300
	.2byte 0x0000
.L_080cf2e0:
	.4byte 0x000003ff
.L_080cf2e4:
	.4byte 0xfff00000
.L_080cf2e8:
	.4byte BattleFx_SpawnRandomParticleAtPosition
.L_080cf2ec:
	.4byte BattleFx_ParticleScript
.L_080cf2f0:
	.4byte Data_080effd8
.L_080cf2f4:
	.4byte 0xfffffc00
.L_080cf2f8:
	.4byte BattleFx_EmitRandomParticleFromEmitter
.L_080cf2fc:
	adds r1, #1
	adds r0, #128
.L_080cf300:
	cmp r1, #63
	bgt .L_080cf32c
	ldr r2, [r0]
	cmp r2, #0
	beq .L_080cf2fc
	ldr r3, .L_080cf33c
	cmp r2, r3
	bne .L_080cf2fc
	ldr r2, [r0, #8]
	ldr r3, [r7, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_080cf2fc
	ldr r2, [r0, #16]
	ldr r3, [r7, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_080cf2fc
	bl Object_Destroy
.L_080cf32c:
	adds r0, r7, #0
.L_080cf32e:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cf33c:
	.4byte Data_080f01b8
