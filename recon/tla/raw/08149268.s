.syntax unified
	.thumb
	.global Func_08149268
	.thumb_func
Func_08149268:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r9, r0
	ldr r0, [r3, #92]
	sub sp, #40
	str r0, [sp, #28]
	movs r0, #0
	ldr r3, [r3, #96]
	movs r7, #239
	str r3, [sp, #24]
	bl Func_081435e0
	ldr r3, .L_081492cc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r1, [sp, #28]
	movs r2, #224
	lsls r2, r2, #3
	adds r5, r1, r2
	adds r1, r5, #0
	ldr r0, .L_081492d0
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	adds r1, r5, #0
	movs r2, #1
	ldr r0, .L_081492d4
	movs r3, #1
	bl Func_08157cf4
	mov r4, sp
	adds r4, #32
	mov r3, r9
	ldr r0, [r3, #4]
	adds r1, r4, #0
	str r4, [sp, #16]
	bl Func_08144aac
	ldr r5, [sp, #28]
	b .L_081492d8
.L_081492cc:
	.4byte 0x00001010
.L_081492d0:
	.4byte 0x0000014d
.L_081492d4:
	.4byte 0x00000193
.L_081492d8:
	movs r0, #238
	lsls r7, r7, #7
	lsls r0, r0, #7
	adds r2, r5, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	movs r1, #200
	adds r2, r5, r0
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08149618
	bl Func_080145a8
	ldr r2, .L_0814961c
	movs r1, #0
	str r2, [sp, #12]
	mov r3, r9
	mov r8, r1
	ldr r1, [r3, #24]
	lsls r3, r1, #1
	adds r3, r3, r1
	ldrb r3, [r2, r3]
	cmp r3, #0
	beq .L_08149384
	ldr r5, [sp, #28]
	movs r4, #31
	movs r0, #63
	mov r11, r4
	movs r7, #0
	mov r10, r0
.L_08149318:
	bl Random16
	ldr r3, .L_08149620
	mov r1, r9
	str r3, [r5, #4]
	ldr r3, [r1, #4]
	cmp r3, #1
	bne .L_0814933e
	bl Random16
	mov r2, r11
	ands r0, r2
	adds r0, #80
	lsls r6, r0, #16
	bl Random16
	mov r3, r10
	ands r0, r3
	b .L_08149354
.L_0814933e:
	bl Random16
	mov r4, r11
	ands r0, r4
	adds r0, #8
	lsls r6, r0, #16
	bl Random16
	mov r1, r10
	ands r0, r1
	negs r0, r0
.L_08149354:
	lsls r0, r0, #12
	str r0, [r5, #12]
	ldr r2, [r5, #12]
	str r7, [r5, #24]
	lsls r3, r2, #3
	adds r3, r3, r2
	lsls r3, r3, #1
	subs r3, r6, r3
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #16]
	str r3, [r5, #8]
	mov r3, r9
	ldr r1, [r3, #24]
	ldr r4, .L_0814961c
	lsls r3, r1, #1
	adds r3, r3, r1
	ldrb r3, [r4, r3]
	movs r2, #1
	add r8, r2
	adds r5, #28
	adds r7, #8
	cmp r8, r3
	bne .L_08149318
.L_08149384:
	adds r2, r1, #0
	movs r5, #0
	str r5, [sp, #20]
	lsls r3, r2, #1
	ldr r7, [sp, #12]
	adds r3, r3, r2
	adds r3, #1
	ldrb r3, [r7, r3]
	cmp r3, #0
	bne .L_0814939a
	b .L_0814969a
.L_0814939a:
	cmp r2, #2
	bne .L_081493e2
	ldr r0, [sp, #20]
	cmp r0, #103
	bgt .L_081493e2
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #48]
	movs r2, #192
	cmp r0, #95
	ble .L_081493be
	ldr r2, [sp, #20]
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r2, #156
	lsls r3, r3, #3
	lsls r2, r2, #4
	subs r2, r2, r3
.L_081493be:
	mov r4, r9
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_081493d4
	ldrh r3, [r1, #54]
	ldr r5, .L_0814961c
	subs r3, r3, r2
	strh r3, [r1, #54]
	str r5, [sp, #12]
	ldr r1, [r4, #24]
	b .L_081493e2
.L_081493d4:
	ldrh r3, [r1, #54]
	ldr r7, .L_0814961c
	adds r3, r3, r2
	strh r3, [r1, #54]
	str r7, [sp, #12]
	mov r0, r9
	ldr r1, [r0, #24]
.L_081493e2:
	lsls r3, r1, #1
	ldr r2, [sp, #12]
	adds r3, r3, r1
	adds r3, #2
	ldrb r3, [r2, r3]
	ldr r4, [sp, #20]
	cmp r4, r3
	bne .L_081493fc
	movs r0, #134
	mov r5, r9
	bl Func_08118088 + 0x60
	ldr r1, [r5, #24]
.L_081493fc:
	lsls r3, r1, #1
	ldr r7, [sp, #12]
	adds r3, r3, r1
	adds r3, #1
	ldrb r3, [r7, r3]
	ldr r0, [sp, #20]
	subs r3, #8
	cmp r0, r3
	bne .L_0814942a
	ldr r1, [sp, #28]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r4, #238
	movs r3, #3
	str r3, [r2]
	lsls r4, r4, #7
	ldr r3, .L_08149624
	adds r4, #132
	adds r2, r1, r4
	str r3, [r2]
	mov r5, r9
	ldr r1, [r5, #24]
.L_0814942a:
	lsls r3, r1, #1
	ldr r7, [sp, #12]
	adds r2, r3, r1
	adds r3, r2, #1
	ldrb r3, [r7, r3]
	ldr r0, [sp, #20]
	subs r3, #8
	cmp r0, r3
	ble .L_0814943e
	b .L_08149656
.L_0814943e:
	movs r3, #0
	mov r11, r3
	ldrb r3, [r7, r2]
	cmp r3, #0
	bne .L_0814944a
	b .L_08149656
.L_0814944a:
	ldr r4, [sp, #28]
	mov r10, r4
.L_0814944e:
	mov r5, r10
	ldr r3, [r5, #8]
	cmp r3, #1
	bne .L_08149504
	mov r0, r11
	lsls r2, r0, #4
	lsls r3, r0, #7
	ldr r1, .L_08149628
	subs r3, r3, r2
	movs r7, #0
	lsls r3, r3, #2
	mov r8, r7
	adds r7, r3, r1
.L_08149468:
	movs r1, #5
	mov r0, r8
	bl Math_Mod
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #96
	ldr r0, [r7, #24]
	bl __divsi3
	movs r1, #3
	bl Math_Mod
	movs r2, #4
	mov r3, r8
	adds r6, r7, #0
	adds r4, r5, r0
	mov r12, r2
	cmp r3, #2
	ble .L_08149494
	movs r5, #0
	mov r12, r5
.L_08149494:
	ldr r2, .L_0814962c
	lsls r3, r4, #2
	ldr r1, [r2, r3]
	ldr r0, [sp, #28]
	movs r2, #240
	adds r1, r0, r1
	lsls r2, r2, #4
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r6, r3]
	ldr r3, .L_08149630
	ldrb r5, [r3, r4]
	lsrs r3, r5, #1
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r6, r0]
	ldr r0, .L_08149634
	ldrb r4, [r0, r4]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #16]
	mov r5, r12
	ldr r4, [r5, r0]
	ldr r0, [sp, #24]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	lsls r2, r2, #6
	adds r0, r6, #0
	movs r1, #64
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #24]
	ldr r2, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #24]
	cmp r2, #1
	ble .L_081494f2
	ldr r1, [sp, #20]
	movs r3, #1
	ands r3, r1
	cmp r3, #0
	beq .L_081494f2
	subs r3, r2, #1
	str r3, [r6, #8]
.L_081494f2:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r7, #28
	cmp r3, #16
	bne .L_08149468
	mov r4, r9
	ldr r1, [r4, #24]
	b .L_08149640
.L_08149504:
	mov r5, r10
	ldr r3, [r5, #24]
	ldr r7, [sp, #20]
	cmp r7, r3
	bge .L_08149510
	b .L_08149640
.L_08149510:
	mov r3, r10
	movs r1, #2
	ldrsh r2, [r3, r1]
	movs r1, #32
	movs r7, #6
	ldrsh r4, [r3, r7]
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	ldr r3, [sp, #28]
	ldr r1, [sp, #16]
	movs r5, #1
	mov r6, r11
	ands r6, r5
	movs r7, #224
	mov r12, r4
	lsls r0, r6, #2
	lsls r7, r7, #3
	ldr r4, [r0, r1]
	subs r2, #16
	adds r1, r3, r7
	ldr r0, [sp, #24]
	mov r3, r12
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	mov r0, r10
	lsls r2, r2, #9
	movs r1, #64
	bl BattleFxKernels_IntegrateVector2
	mov r0, r10
	ldr r3, [r0, #4]
	movs r2, #224
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_08149610
	mov r3, r11
	str r2, [r0, #4]
	ldr r4, .L_08149628
	lsls r2, r3, #4
	lsls r3, r3, #7
	subs r3, r3, r2
	ldr r7, .L_08149638
	movs r1, #0
	lsls r3, r3, #2
	str r5, [r0, #8]
	mov r8, r1
	adds r5, r3, r4
	movs r1, #127
.L_08149574:
	ldrb r3, [r7]
	mov r0, r10
	ldr r2, [r0]
	subs r3, #40
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r5]
	ldrb r3, [r7, #1]
	str r1, [sp, #8]
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ldr r1, [sp, #8]
	ands r0, r1
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ldr r1, [sp, #8]
	ands r0, r1
	negs r0, r0
	lsls r3, r0, #11
	str r3, [r5, #16]
	cmp r6, #0
	beq .L_081495b4
	ldr r3, [r5, #12]
	lsls r3, r3, #1
	str r3, [r5, #12]
	lsls r3, r0, #12
	str r3, [r5, #16]
.L_081495b4:
	movs r3, #32
	movs r2, #1
	str r3, [r5, #8]
	add r8, r2
	movs r3, #0
	str r3, [r5, #24]
	mov r3, r8
	adds r7, #2
	adds r5, #28
	cmp r3, #16
	bne .L_08149574
	ldr r4, [sp, #28]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r3, r4, r5
	movs r2, #8
	str r2, [r3]
	movs r0, #144
	bl Audio_PlayCue
	mov r0, r9
	ldr r3, [r0, #20]
	movs r7, #0
	mov r8, r7
	cmp r3, #0
	beq .L_0814963c
	movs r6, #4
	movs r5, #36
.L_081495ee:
	mov r1, r9
	ldrsh r0, [r5, r1]
	mov r3, r8
	movs r1, #7
	movs r2, #5
	str r6, [sp, #0]
	bl Func_0814cd48
	movs r3, #1
	mov r4, r9
	add r8, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_081495ee
	ldr r1, [r4, #24]
	b .L_08149640
.L_08149610:
	mov r5, r9
	ldr r1, [r5, #24]
	b .L_08149640
	.2byte 0x0000
.L_08149618:
	.4byte Func_08143000
.L_0814961c:
	.4byte Data_081979b7
.L_08149620:
	.4byte 0xffc00000
.L_08149624:
	.4byte 0x06060606
.L_08149628:
	.4byte gMapCellBuffer
.L_0814962c:
	.4byte Data_08197834
.L_08149630:
	.4byte Data_0819781a
.L_08149634:
	.4byte Data_08197826
.L_08149638:
	.4byte Data_081977f8
.L_0814963c:
	mov r7, r9
	ldr r1, [r7, #24]
.L_08149640:
	ldr r4, .L_081496c0
	lsls r3, r1, #1
	adds r3, r3, r1
	ldrb r3, [r4, r3]
	movs r2, #1
	movs r0, #28
	add r11, r2
	add r10, r0
	cmp r11, r3
	beq .L_08149656
	b .L_0814944e
.L_08149656:
	lsls r0, r1, #1
	lsls r1, r1, #2
	adds r1, #8
	adds r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r7, #240
	ldr r5, [sp, #28]
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r5, r7
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #20]
	ldr r1, .L_081496c0
	adds r0, #1
	str r1, [sp, #12]
	str r0, [sp, #20]
	mov r3, r9
	ldr r2, [r3, #24]
	ldr r4, .L_081496c0
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #1
	ldrb r3, [r4, r3]
	adds r1, r2, #0
	cmp r0, r3
	beq .L_0814969a
	b .L_0814939a
.L_0814969a:
	ldr r0, .L_081496c4
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081496c0:
	.4byte Data_081979b7
.L_081496c4:
	.4byte Func_08143000
