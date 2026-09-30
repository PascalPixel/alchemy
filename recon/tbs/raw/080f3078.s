.syntax unified
	.thumb
	.global Unnamed_080f3078
	.thumb_func
Unnamed_080f3078:
	.global Graphics_TransformPaletteBuffer
Graphics_TransformPaletteBuffer:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	mov r8, r2
	movs r1, #128
	adds r2, r3, #0
	movs r3, #128
	sub sp, #40
	lsls r1, r1, #2
	lsls r3, r3, #8
	str r1, [sp, #36]
	cmp r0, r3
	bne .L_080f30a0
	mov r1, r10
	ldrh r0, [r1]
.L_080f30a0:
	cmp r2, #1
	bne .L_080f30ac
	movs r3, #128
	lsls r3, r3, #1
	str r3, [sp, #36]
	b .L_080f30c2
.L_080f30ac:
	cmp r2, #2
	bne .L_080f30c2
	movs r1, #192
	lsls r1, r1, #3
	add r8, r1
	movs r1, #128
	movs r3, #128
	lsls r1, r1, #1
	lsls r3, r3, #2
	str r1, [sp, #36]
	add r10, r3
.L_080f30c2:
	movs r3, #128
	lsls r3, r3, #8
	cmp r0, r3
	bcs .L_080f311a
	ldr r2, .L_080f3104
	adds r3, r0, #0
	ands r3, r2
	movs r2, #2
	mov r1, r8
	add r8, r2
	ldr r2, .L_080f3108
	strh r3, [r1]
	adds r3, r0, #0
	ands r3, r2
	mov r1, r8
	lsls r3, r3, #5
	strh r3, [r1]
	ldr r3, .L_080f310c
	movs r2, #2
	add r8, r2
	ands r0, r3
	lsls r3, r0, #10
	mov r1, r8
	strh r3, [r1]
	ldr r3, [sp, #36]
	subs r3, #1
	add r8, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #1
	movs r4, #128
	lsls r4, r4, #24
	b .L_080f3110
.L_080f3104:
	.4byte 0x00007c00
.L_080f3108:
	.4byte 0x000003e0
.L_080f310c:
	.4byte 0x0000001f
.L_080f3110:
	mov r0, r8
	lsrs r2, r2, #1
	ldr r3, .L_080f3450
	subs r0, #6
	b .L_080f375e
.L_080f311a:
	movs r3, #128
	lsls r3, r3, #13
	cmp r0, r3
	bcc .L_080f3124
	b .L_080f3546
.L_080f3124:
	ldr r1, .L_080f3454
	adds r0, r0, r1
	cmp r0, #6
	bls .L_080f312e
	b .L_080f34f8
.L_080f312e:
	ldr r2, .L_080f3458
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080f3138:
	.4byte .L_080f3154
	.4byte .L_080f31a2
	.4byte .L_080f323e
	.4byte .L_080f32bc
	.4byte .L_080f3356
	.4byte .L_080f33d8
	.4byte .L_080f346c
.L_080f3154:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_080f3160
	b .L_080f3766
.L_080f3160:
	ldr r6, .L_080f345c
	mov r5, r8
.L_080f3164:
	mov r1, r10
	ldrh r4, [r1]
	movs r3, #248
	lsls r0, r4, #11
	lsls r3, r3, #8
	movs r2, #2
	ands r0, r3
	movs r3, #248
	lsls r3, r3, #9
	add r10, r2
	lsls r2, r4, #7
	ands r2, r3
	movs r3, #248
	lsls r3, r3, #7
	ands r3, r4
	adds r0, r0, r2
	adds r0, r0, r3
	movs r1, #7
	bl _call_via_r6
	adds r4, r0, #0
	strh r4, [r5]
	strh r4, [r5, #2]
	strh r4, [r5, #4]
	movs r3, #1
	ldr r1, [sp, #36]
	add r9, r3
	adds r5, #6
	cmp r9, r1
	bcc .L_080f3164
	b .L_080f3766
.L_080f31a2:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_080f31ae
	b .L_080f3766
.L_080f31ae:
	movs r1, #31
	ldr r2, .L_080f3460
	mov r11, r1
.L_080f31b4:
	mov r3, r10
	ldrh r4, [r3]
	movs r1, #2
	adds r7, r4, #0
	mov r3, r11
	lsrs r0, r4, #5
	ands r7, r3
	ands r0, r3
	add r10, r1
	lsrs r3, r4, #10
	mov r1, r11
	ands r3, r1
	adds r0, r7, r0
	adds r0, r0, r3
	str r2, [sp, #0]
	ldr r3, .L_080f345c
	movs r1, #10
	bl _call_via_r3
	adds r4, r0, #0
	lsls r3, r4, #2
	adds r7, r3, #5
	lsls r3, r4, #1
	adds r3, r3, r4
	adds r5, r3, #5
	adds r6, r5, #0
	ldr r2, [sp, #0]
	cmp r7, #7
	bgt .L_080f31f0
	movs r7, #8
.L_080f31f0:
	cmp r5, #7
	bgt .L_080f31fc
	movs r6, #8
	cmp r5, #7
	bgt .L_080f31fc
	movs r5, #8
.L_080f31fc:
	cmp r7, #28
	ble .L_080f3202
	movs r7, #28
.L_080f3202:
	cmp r6, #28
	ble .L_080f3208
	movs r6, #28
.L_080f3208:
	cmp r5, #28
	ble .L_080f320e
	movs r5, #28
.L_080f320e:
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	lsls r3, r6, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	movs r1, #1
	ldr r3, [sp, #36]
	add r9, r1
	cmp r9, r3
	bcc .L_080f31b4
	b .L_080f3766
.L_080f323e:
	movs r1, #0
	ldr r2, [sp, #36]
	mov r9, r1
	cmp r9, r2
	bcc .L_080f324a
	b .L_080f3766
.L_080f324a:
	movs r3, #31
	mov r11, r3
.L_080f324e:
	mov r1, r10
	ldrh r4, [r1]
	mov r3, r11
	adds r7, r4, #0
	lsrs r6, r4, #5
	lsrs r5, r4, #10
	ands r6, r3
	ands r7, r3
	movs r2, #2
	movs r1, #3
	ands r5, r3
	adds r0, r6, #0
	lsrs r3, r7, #1
	subs r7, r7, r3
	add r10, r2
	bl FixedPoint_Ratio
	adds r7, #6
	subs r6, r6, r0
	adds r0, r7, #0
	bl Graphics_ClampRgb555Channel
	adds r6, #4
	adds r7, r0, #0
	adds r0, r6, #0
	bl Graphics_ClampRgb555Channel
	subs r5, #6
	adds r6, r0, #0
	adds r0, r5, #0
	bl Graphics_ClampRgb555Channel
	ldr r2, .L_080f3464
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r2, .L_080f3460
	lsls r3, r6, #1
	ldrh r3, [r2, r3]
	mov r2, r8
	strh r3, [r2, #2]
	ldr r2, .L_080f3468
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	strh r3, [r1, #4]
	ldr r1, [sp, #36]
	movs r3, #1
	movs r2, #6
	add r9, r3
	add r8, r2
	cmp r9, r1
	bcc .L_080f324e
	b .L_080f3766
.L_080f32bc:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_080f32c8
	b .L_080f3766
.L_080f32c8:
	ldr r1, .L_080f3468
	mov r11, r1
.L_080f32cc:
	mov r2, r10
	ldrh r4, [r2]
	movs r1, #31
	adds r7, r4, #0
	movs r3, #2
	lsrs r6, r4, #5
	lsrs r5, r4, #10
	ands r7, r1
	add r10, r3
	ands r6, r1
	ands r5, r1
	cmp r7, #9
	bgt .L_080f32e8
	movs r7, #10
.L_080f32e8:
	cmp r6, #15
	bgt .L_080f32ee
	movs r6, #16
.L_080f32ee:
	cmp r5, #15
	bgt .L_080f32f4
	movs r5, #16
.L_080f32f4:
	cmp r7, #28
	ble .L_080f32fa
	movs r7, #28
.L_080f32fa:
	cmp r6, #24
	ble .L_080f3300
	movs r6, #24
.L_080f3300:
	cmp r5, #26
	ble .L_080f3306
	movs r5, #26
.L_080f3306:
	adds r0, r7, #0
	bl Graphics_ClampRgb555Channel
	adds r6, #2
	adds r7, r0, #0
	adds r0, r6, #0
	bl Graphics_ClampRgb555Channel
	adds r5, #2
	adds r6, r0, #0
	adds r0, r5, #0
	bl Graphics_ClampRgb555Channel
	adds r5, r0, #0
	mov r2, r11
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r2, #2
	mov r1, r11
	lsls r3, r6, #1
	ldrh r3, [r1, r3]
	add r8, r2
	mov r2, r8
	strh r3, [r2]
	movs r3, #2
	add r8, r3
	lsls r3, r7, #1
	ldrh r3, [r1, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r1, [sp, #36]
	movs r3, #1
	movs r2, #2
	add r9, r3
	add r8, r2
	cmp r9, r1
	bcc .L_080f32cc
	b .L_080f3766
.L_080f3356:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_080f3362
	b .L_080f3766
.L_080f3362:
	ldr r1, .L_080f3464
	mov r11, r1
.L_080f3366:
	mov r2, r10
	ldrh r4, [r2]
	movs r1, #31
	adds r7, r4, #0
	lsrs r6, r4, #5
	lsrs r5, r4, #10
	ands r7, r1
	ands r6, r1
	ands r5, r1
	adds r0, r7, r6
	movs r1, #3
	movs r3, #2
	adds r0, r0, r5
	add r10, r3
	bl FixedPoint_Ratio
	bl Graphics_ClampRgb555Channel
	asrs r3, r7, #1
	adds r7, r3, r0
	asrs r3, r6, #1
	adds r6, r3, r0
	asrs r3, r5, #1
	adds r5, r3, r0
	adds r0, r7, #0
	bl Graphics_ClampRgb555Channel
	adds r7, r0, #0
	adds r0, r6, #0
	bl Graphics_ClampRgb555Channel
	adds r6, r0, #0
	adds r0, r5, #0
	bl Graphics_ClampRgb555Channel
	adds r5, r0, #0
	mov r2, r11
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	lsls r3, r6, #1
	ldrh r3, [r2, r3]
	mov r2, r8
	strh r3, [r2, #2]
	mov r1, r11
	lsls r3, r7, #1
	ldrh r3, [r1, r3]
	strh r3, [r2, #4]
	movs r1, #1
	ldr r2, [sp, #36]
	movs r3, #6
	add r9, r1
	add r8, r3
	cmp r9, r2
	bcc .L_080f3366
	b .L_080f3766
.L_080f33d8:
	movs r3, #0
	ldr r1, [sp, #36]
	mov r9, r3
	cmp r9, r1
	bcc .L_080f33e4
	b .L_080f3766
.L_080f33e4:
	movs r2, #31
	mov r11, r2
.L_080f33e8:
	mov r3, r10
	ldrh r4, [r3]
	mov r2, r11
	lsrs r6, r4, #5
	lsrs r5, r4, #10
	adds r7, r4, #0
	ands r6, r2
	ands r5, r2
	ands r7, r2
	asrs r3, r6, #3
	asrs r2, r5, #3
	adds r3, r3, r2
	adds r7, r7, r3
	movs r1, #2
	adds r0, r7, #0
	add r10, r1
	bl Graphics_ClampRgb555Channel
	movs r1, #3
	adds r7, r0, #0
	adds r0, r6, #0
	bl FixedPoint_Ratio
	movs r1, #3
	subs r6, r6, r0
	adds r0, r5, #0
	bl FixedPoint_Ratio
	ldr r1, .L_080f3468
	subs r5, r5, r0
	lsls r3, r5, #1
	ldrh r3, [r1, r3]
	mov r2, r8
	strh r3, [r2]
	lsls r3, r6, #1
	ldrh r3, [r1, r3]
	mov r1, r8
	strh r3, [r1, #2]
	ldr r2, .L_080f3460
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	mov r2, r8
	strh r3, [r2, #4]
	movs r1, #1
	ldr r2, [sp, #36]
	movs r3, #6
	add r9, r1
	add r8, r3
	cmp r9, r2
	bcc .L_080f33e8
	b .L_080f3766
	.2byte 0x0000
.L_080f3450:
	.4byte 0x040000d4
.L_080f3454:
	.4byte 0xfffeffff
.L_080f3458:
	.4byte .L_080f3138
.L_080f345c:
	.4byte IwramSignedDivideArm
.L_080f3460:
	.4byte Data_080f3a2e
.L_080f3464:
	.4byte Data_080f3a6e
.L_080f3468:
	.4byte Data_080f39ee
.L_080f346c:
	movs r3, #0
	ldr r1, [sp, #36]
	mov r9, r3
	cmp r9, r1
	bcc .L_080f3478
	b .L_080f3766
.L_080f3478:
	movs r2, #31
	mov r11, r2
.L_080f347c:
	mov r3, r10
	ldrh r4, [r3]
	mov r2, r11
	adds r7, r4, #0
	lsrs r6, r4, #5
	ands r7, r2
	ands r6, r2
	movs r1, #2
	lsrs r3, r7, #1
	lsrs r5, r4, #10
	adds r0, r6, #0
	add r10, r1
	movs r1, #3
	ands r5, r2
	subs r7, r7, r3
	bl FixedPoint_Ratio
	adds r7, #6
	subs r6, r6, r0
	adds r0, r7, #0
	bl Graphics_ClampRgb555Channel
	adds r6, #4
	adds r7, r0, #0
	adds r0, r6, #0
	bl Graphics_ClampRgb555Channel
	subs r5, #6
	adds r6, r0, #0
	adds r0, r5, #0
	bl Graphics_ClampRgb555Channel
	ldr r2, .L_080f34ec
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r2, .L_080f34f0
	lsls r3, r6, #1
	ldrh r3, [r2, r3]
	mov r2, r8
	strh r3, [r2, #2]
	ldr r2, .L_080f34f4
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	strh r3, [r1, #4]
	ldr r1, [sp, #36]
	movs r3, #1
	movs r2, #6
	add r9, r3
	add r8, r2
	cmp r9, r1
	bcc .L_080f347c
	b .L_080f3766
	.2byte 0x0000
.L_080f34ec:
	.4byte Data_080f3a6e
.L_080f34f0:
	.4byte Data_080f3a2e
.L_080f34f4:
	.4byte Data_080f39ee
.L_080f34f8:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_080f3504
	b .L_080f3766
.L_080f3504:
	ldr r5, .L_080f3510
	ldr r0, .L_080f3514
	ldr r2, .L_080f3518
	mov r1, r8
	b .L_080f351c
	.2byte 0x0000
.L_080f3510:
	.4byte 0x00007c00
.L_080f3514:
	.4byte 0x000003e0
.L_080f3518:
	.4byte 0x0000001f
.L_080f351c:
	mov r3, r10
	ldrh r4, [r3]
	movs r3, #2
	add r10, r3
	adds r3, r4, #0
	ands r3, r5
	strh r3, [r1]
	adds r3, r4, #0
	ands r3, r0
	lsls r3, r3, #5
	ands r4, r2
	strh r3, [r1, #2]
	lsls r3, r4, #10
	strh r3, [r1, #4]
	movs r3, #1
	add r9, r3
	ldr r3, [sp, #36]
	adds r1, #6
	cmp r9, r3
	bcc .L_080f351c
	b .L_080f3766
.L_080f3546:
	movs r3, #128
	lsls r3, r3, #14
	ands r3, r0
	cmp r3, #0
	beq .L_080f35e6
	movs r3, #31
	str r0, [sp, #32]
	adds r1, r0, #0
	lsrs r2, r0, #5
	lsrs r0, r0, #10
	ands r1, r3
	mov r11, r0
	ands r2, r3
	str r1, [sp, #32]
	mov r1, r11
	ands r1, r3
	str r2, [sp, #28]
	ldr r3, [sp, #36]
	movs r2, #0
	mov r9, r2
	mov r11, r1
	cmp r9, r3
	bcc .L_080f3576
	b .L_080f3766
.L_080f3576:
	mov r1, r10
	ldrh r4, [r1]
	movs r3, #248
	lsls r0, r4, #11
	lsls r3, r3, #8
	movs r2, #2
	ands r0, r3
	movs r3, #248
	lsls r3, r3, #9
	add r10, r2
	lsls r2, r4, #7
	ands r2, r3
	movs r3, #248
	lsls r3, r3, #7
	ands r3, r4
	adds r0, r0, r2
	adds r0, r0, r3
	movs r1, #96
	ldr r3, .L_080f36e4
	bl _call_via_r3
	ldr r1, [sp, #32]
	adds r4, r0, #0
	adds r7, r1, #0
	muls r7, r4
	ldr r2, [sp, #28]
	adds r0, r7, #0
	adds r6, r2, #0
	muls r6, r4
	mov r5, r11
	muls r5, r4
	bl Graphics_ClampRgb555Component
	adds r7, r0, #0
	adds r0, r6, #0
	bl Graphics_ClampRgb555Component
	adds r6, r0, #0
	adds r0, r5, #0
	bl Graphics_ClampRgb555Component
	mov r3, r8
	mov r1, r8
	mov r2, r8
	adds r5, r0, #0
	strh r5, [r3]
	strh r6, [r1, #2]
	strh r7, [r2, #4]
	movs r1, #1
	ldr r2, [sp, #36]
	movs r3, #6
	add r9, r1
	add r8, r3
	cmp r9, r2
	bcc .L_080f3576
	b .L_080f3766
.L_080f35e6:
	movs r3, #128
	lsls r3, r3, #15
	ands r3, r0
	cmp r3, #0
	bne .L_080f35f2
	b .L_080f36f0
.L_080f35f2:
	movs r3, #31
	str r0, [sp, #24]
	adds r1, r0, #0
	lsrs r2, r0, #5
	lsrs r0, r0, #10
	ands r1, r3
	mov r11, r0
	ands r2, r3
	str r1, [sp, #24]
	mov r1, r11
	ands r1, r3
	str r2, [sp, #20]
	ldr r3, [sp, #36]
	movs r2, #0
	mov r9, r2
	mov r11, r1
	cmp r9, r3
	bcc .L_080f3618
	b .L_080f3766
.L_080f3618:
	ldr r2, [sp, #20]
	ldr r1, [sp, #24]
	ldr r3, [sp, #24]
	adds r1, r1, r2
	str r1, [sp, #4]
	lsls r1, r2, #16
	mov r2, r11
	lsls r3, r3, #16
	lsls r2, r2, #16
	str r3, [sp, #16]
	str r1, [sp, #12]
	str r2, [sp, #8]
.L_080f3630:
	mov r3, r10
	ldrh r4, [r3]
	movs r2, #31
	adds r7, r4, #0
	lsrs r0, r4, #5
	ands r7, r2
	ands r0, r2
	lsrs r3, r4, #10
	movs r1, #2
	ands r3, r2
	add r10, r1
	adds r0, r7, r0
	ldr r1, [sp, #4]
	adds r0, r0, r3
	add r1, r11
	ldr r3, .L_080f36e4
	lsls r0, r0, #4
	bl _call_via_r3
	ldr r3, [sp, #24]
	adds r4, r0, #0
	adds r0, r3, #0
	muls r0, r4
	ldr r2, [sp, #16]
	lsrs r0, r0, #4
	lsls r0, r0, #16
	asrs r1, r2, #4
	ldr r3, .L_080f36e8
	mov r12, pc
	bx r3
	ldr r1, [sp, #20]
	adds r7, r0, #0
	adds r0, r1, #0
	muls r0, r4
	ldr r2, [sp, #12]
	lsrs r0, r0, #4
	lsls r0, r0, #16
	asrs r1, r2, #4
	mov r12, pc
	bx r3
	adds r6, r0, #0
	mov r0, r11
	muls r0, r4
	ldr r3, [sp, #8]
	lsrs r0, r0, #4
	asrs r1, r3, #4
	lsls r0, r0, #16
	ldr r3, .L_080f36e8
	mov r12, pc
	bx r3
	adds r5, r0, #0
	lsrs r0, r7, #16
	bl Graphics_ClampRgb555Channel
	adds r7, r0, #0
	lsrs r0, r6, #16
	bl Graphics_ClampRgb555Channel
	adds r6, r0, #0
	lsrs r0, r5, #16
	bl Graphics_ClampRgb555Channel
	ldr r1, .L_080f36ec
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r3, [r1, r3]
	mov r2, r8
	strh r3, [r2]
	movs r3, #2
	add r8, r3
	lsls r3, r6, #1
	ldrh r3, [r1, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r1, .L_080f36ec
	movs r2, #2
	lsls r3, r7, #1
	ldrh r3, [r1, r3]
	add r8, r2
	mov r2, r8
	strh r3, [r2]
	movs r1, #1
	ldr r2, [sp, #36]
	movs r3, #2
	add r9, r1
	add r8, r3
	cmp r9, r2
	bcc .L_080f3630
	b .L_080f3766
	.2byte 0x0000
.L_080f36e4:
	.4byte IwramSignedDivideArm
.L_080f36e8:
	.4byte IwramMulQ16ReturnIp
.L_080f36ec:
	.4byte Data_080f39ee
.L_080f36f0:
	movs r3, #128
	lsls r3, r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_080f3746
	movs r3, #0
	ldr r1, [sp, #36]
	mov r9, r3
	cmp r9, r1
	bcs .L_080f3766
	ldr r5, .L_080f3710
	ldr r0, .L_080f3714
	ldr r2, .L_080f3718
	mov r1, r8
	b .L_080f371c
	.2byte 0x0000
.L_080f3710:
	.4byte 0x00007c00
.L_080f3714:
	.4byte 0x000003e0
.L_080f3718:
	.4byte 0x0000001f
.L_080f371c:
	mov r3, r10
	ldrh r4, [r3]
	movs r3, #2
	add r10, r3
	adds r3, r4, #0
	ands r3, r5
	strh r3, [r1]
	adds r3, r4, #0
	ands r3, r0
	lsls r3, r3, #5
	ands r4, r2
	strh r3, [r1, #2]
	lsls r3, r4, #10
	strh r3, [r1, #4]
	movs r3, #1
	add r9, r3
	ldr r3, [sp, #36]
	adds r1, #6
	cmp r9, r3
	bcc .L_080f371c
	b .L_080f3766
.L_080f3746:
	cmp r2, #2
	bne .L_080f3750
	movs r1, #192
	lsls r1, r1, #3
	adds r0, r0, r1
.L_080f3750:
	ldr r3, [sp, #36]
	lsls r2, r3, #1
	adds r2, r2, r3
	movs r4, #132
	lsls r4, r4, #24
	lsrs r2, r2, #1
	ldr r3, .L_080f3778
.L_080f375e:
	mov r1, r8
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_080f3766:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080f3778:
	.4byte 0x040000d4
