.syntax unified
	.thumb
	.global BattleEffect_RunImpactBurst
	.thumb_func
BattleEffect_RunImpactBurst:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	ldr r5, .L_080e6f28
	str r2, [sp, #28]
	str r0, [sp, #32]
	adds r3, r5, #0
	ldmia r3!, {r0}
	mov r11, r1
	ldr r3, [r3]
	mov r2, r11
	movs r4, #160
	str r3, [sp, #24]
	lsls r4, r4, #14
	adds r3, r2, #0
	adds r3, r3, r4
	mov r11, r3
	lsrs r3, r3, #31
	add r3, r11
	asrs r3, r3, #1
	ldr r1, [r5, #8]
	mov r11, r3
	str r2, [sp, #8]
	ldr r3, .L_080e6f20
	ldr r2, .L_080e6f2c
	str r1, [sp, #12]
	strh r3, [r2]
	adds r2, #8
	movs r3, #0
	str r3, [r2]
	ldr r3, .L_080e6f24
	adds r2, #40
	mov r10, r0
	strh r3, [r2]
	movs r6, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl Unnamed_080ed408
	ldr r0, [r5, #28]
	movs r3, #3
	str r0, [sp, #16]
	movs r1, #7
	movs r2, #7
	movs r0, #47
	str r3, [sp, #0]
	bl Unnamed_080ed408
	ldr r5, [r5, #32]
	b .L_080e6f30
.L_080e6f20:
	.4byte 0x00000080
.L_080e6f24:
	.4byte 0x00003f46
.L_080e6f28:
	.4byte gBattleFxWork
.L_080e6f2c:
	.4byte 0x04000020
.L_080e6f30:
	ldr r1, [sp, #12]
	ldr r0, .L_080e7238
	movs r2, #0
	movs r3, #0
	str r5, [sp, #20]
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e723c
	mov r1, r10
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r1, .L_080e7240
	ldr r0, .L_080e7244
	add r1, r10
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080e7248
	add r3, r10
	str r6, [r3]
	add r2, r10
	movs r3, #50
	movs r1, #144
	lsls r1, r1, #3
	str r3, [r2]
	ldr r0, .L_080e724c
	movs r7, #225
	bl Scheduler_AddOrUpdateCallback
	lsls r7, r7, #7
	movs r1, #0
	mov r8, r1
	add r7, r10
.L_080e6f7c:
	bl Random16
	movs r6, #255
	movs r2, #128
	lsls r2, r2, #1
	ands r6, r0
	adds r6, r6, r2
	bl Random16
	ldr r3, .L_080e7250
	adds r5, r0, #0
	ands r5, r3
	mov r3, r11
	str r3, [r7]
	ldr r4, [sp, #28]
	adds r0, r5, #0
	str r4, [r7, #4]
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Func_0800231c
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	negs r3, r3
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r0, #1
	add r8, r0
	adds r3, #16
	mov r1, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #64
	bne .L_080e6f7c
	ldr r5, .L_080e7254
	movs r2, #0
	mov r8, r2
	movs r6, #0
	add r5, r10
.L_080e6fdc:
	mov r3, r11
	str r3, [r5]
	ldr r4, [sp, #28]
	adds r0, r6, #0
	str r4, [r5, #4]
	bl Trig_Sin
	lsls r0, r0, #5
	asrs r0, r0, #6
	str r0, [r5, #12]
	adds r0, r6, #0
	bl Func_0800231c
	lsls r0, r0, #5
	asrs r0, r0, #5
	negs r0, r0
	movs r1, #1
	str r0, [r5, #16]
	add r8, r1
	ldr r0, .L_080e7258
	mov r2, r8
	adds r6, r6, r0
	adds r5, #28
	cmp r2, #3
	bne .L_080e6fdc
	movs r3, #0
	ldr r7, .L_080e725c
	mov r8, r3
.L_080e7014:
	bl Random16
	movs r6, #255
	ands r6, r0
	bl Random16
	mov r4, r11
	str r4, [r7]
	ldr r3, .L_080e7250
	adds r5, r0, #0
	ldr r0, [sp, #28]
	ands r5, r3
	str r0, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Func_0800231c
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #5
	negs r3, r3
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	movs r1, #1
	ands r3, r0
	add r8, r1
	adds r3, #20
	mov r2, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #64
	bne .L_080e7014
	movs r7, #0
.L_080e7068:
	cmp r7, #4
	bne .L_080e7072
	movs r0, #154
	bl Func_080f9010
.L_080e7072:
	cmp r7, #32
	bne .L_080e707c
	movs r0, #212
	bl Func_080f9010
.L_080e707c:
	cmp r7, #47
	bgt .L_080e70be
	adds r0, r7, #0
	subs r0, #8
	movs r1, #5
	bl FixedPoint_Ratio
	adds r4, r0, #0
	cmp r4, #0
	bge .L_080e7092
	movs r4, #0
.L_080e7092:
	ldr r2, .L_080e7260
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	mov r3, r11
	asrs r2, r3, #16
	ldr r3, .L_080e7264
	ldrb r5, [r3, r4]
	ldr r0, [sp, #28]
	lsrs r3, r5, #1
	subs r2, r2, r3
	asrs r3, r0, #16
	ldr r0, .L_080e7268
	ldrb r4, [r0, r4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	add r1, r10
	str r5, [sp, #0]
	ldr r0, [sp, #24]
	ldr r4, [sp, #16]
	bl _call_via_r4
.L_080e70be:
	movs r6, #225
	movs r0, #0
	lsls r6, r6, #7
	mov r8, r0
	add r6, r10
.L_080e70c8:
	mov r1, r8
	lsrs r3, r1, #31
	add r3, r8
	asrs r3, r3, #1
	cmp r7, r3
	ble .L_080e711e
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_080e711e
	subs r3, #1
	str r3, [r6, #24]
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity2D
	ldr r4, [r6, #24]
	cmp r4, #0
	bge .L_080e70f0
	adds r4, #15
.L_080e70f0:
	asrs r4, r4, #4
	adds r4, #3
	movs r3, #2
	ldrsh r2, [r6, r3]
	lsls r5, r4, #1
	movs r0, #6
	ldrsh r3, [r6, r0]
	ldr r0, .L_080e726c
	subs r1, r5, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #12]
	adds r1, r0, r1
	lsrs r0, r4, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	subs r2, r2, r0
	subs r3, r3, r4
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #24]
	ldr r4, [sp, #20]
	bl _call_via_r4
.L_080e711e:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #30
	bne .L_080e70c8
	ldr r3, .L_080e726c
	movs r2, #0
	ldr r6, .L_080e725c
	mov r8, r2
	mov r9, r3
.L_080e7134:
	cmp r7, #35
	ble .L_080e7182
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_080e7182
	subs r3, #1
	str r3, [r6, #24]
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity2D
	ldr r4, [r6, #24]
	cmp r4, #0
	bge .L_080e7154
	adds r4, #15
.L_080e7154:
	asrs r4, r4, #4
	adds r4, #1
	lsls r5, r4, #1
	movs r0, #2
	ldrsh r2, [r6, r0]
	movs r1, #6
	ldrsh r3, [r6, r1]
	mov r0, r9
	subs r1, r5, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #12]
	adds r1, r0, r1
	lsrs r0, r4, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	subs r2, r2, r0
	subs r3, r3, r4
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #24]
	ldr r4, [sp, #20]
	bl _call_via_r4
.L_080e7182:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #60
	bne .L_080e7134
	ldr r5, .L_080e7254
	movs r2, #0
	adds r6, r7, #0
	mov r8, r2
	subs r6, #36
	add r5, r10
.L_080e719a:
	cmp r6, #27
	bhi .L_080e71d8
	movs r2, #0
	adds r0, r5, #0
	movs r1, #64
	bl EffectStep_AdvanceWithGravity2D
	movs r1, #7
	adds r0, r6, #0
	bl FixedPoint_Ratio
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #5
	ldr r0, .L_080e7240
	add r1, r10
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r1, r1, r0
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #12
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	subs r2, #6
	subs r3, #12
	ldr r0, [sp, #24]
	ldr r4, [sp, #16]
	bl _call_via_r4
.L_080e71d8:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #3
	bne .L_080e719a
	cmp r7, #35
	bgt .L_080e71f2
	ldr r0, [sp, #32]
	ldr r1, [sp, #8]
	ldr r2, [sp, #28]
	bl BattleFx_PlaceFormationObjects
.L_080e71f2:
	ldr r2, .L_080e7270
	movs r3, #1
	add r2, r10
	str r3, [r2]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #72
	beq .L_080e7208
	b .L_080e7068
.L_080e7208:
	ldr r0, .L_080e724c
	bl Scheduler_RemoveCallback
	movs r1, #128
	ldr r3, .L_080e7274
	lsls r1, r1, #7
	ldr r0, .L_080e7278
	bl _call_via_r3
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080e7238:
	.4byte 0x00000073
.L_080e723c:
	.4byte 0x0000005e
.L_080e7240:
	.4byte 0x000059d8
.L_080e7244:
	.4byte 0x0000005f
.L_080e7248:
	.4byte 0x00007784
.L_080e724c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e7250:
	.4byte 0x0000ffff
.L_080e7254:
	.4byte 0x0000772c
.L_080e7258:
	.4byte 0x00005555
.L_080e725c:
	.4byte gMapCellBuffer
.L_080e7260:
	.4byte Data_080eee66
.L_080e7264:
	.4byte Data_080eee56
.L_080e7268:
	.4byte Data_080eee5e
.L_080e726c:
	.4byte ParticleStreams_CellOffsets
.L_080e7270:
	.4byte 0x00007824
.L_080e7274:
	.4byte IwramClearWords
.L_080e7278:
	.4byte 0x06004000
