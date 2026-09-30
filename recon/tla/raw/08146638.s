.syntax unified
	.thumb
	.global Func_08146638
	.thumb_func
Func_08146638:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #120
	str r0, [sp, #80]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	ldr r4, [sp, #80]
	str r0, [sp, #76]
	ldr r1, [r5, #96]
	str r1, [sp, #72]
	ldr r2, [r5, #100]
	str r2, [sp, #52]
	ldr r3, [r5, #48]
	str r3, [sp, #48]
	ldr r0, [r4, #8]
	bl Func_08118088 + 0x10
	ldr r6, [r0]
	movs r0, #1
	bl Func_081435e0
	ldr r0, .L_081469ac
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081469b0
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #0
	ldr r1, [sp, #52]
	movs r3, #0
	ldr r0, .L_081469b4
	bl Func_08157cf4
	adds r0, r6, #0
	movs r1, #2
	bl Object_SetMode
	adds r0, r6, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r0, [sp, #76]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	ldr r5, [r5, #104]
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_081469b8
	str r5, [sp, #60]
	bl Func_080145a8
	ldr r4, [sp, #80]
	ldr r0, [r4, #8]
	bl Func_08118088 + 0x10
	ldr r2, [sp, #80]
	ldr r6, [r0]
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl Func_08118088 + 0x10
	movs r3, #0
	ldr r0, [r0]
	ldr r5, [sp, #76]
	str r3, [sp, #56]
	mov r11, r0
.L_081466ea:
	ldr r2, [r6, #8]
	movs r4, #160
	str r2, [r5]
	lsls r4, r4, #14
	ldr r3, [r6, #12]
	adds r3, r3, r4
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	ldr r0, [sp, #56]
	cmp r0, #0
	bne .L_0814670a
	mov r1, r11
	ldr r0, [r1, #8]
	movs r1, #12
	b .L_08146712
.L_0814670a:
	mov r3, r11
	ldr r0, [r3, #8]
	movs r1, #12
	lsls r0, r0, #1
.L_08146712:
	subs r0, r0, r2
	bl __divsi3
	str r0, [r5, #12]
	mov r4, r11
	ldr r0, [r4, #12]
	ldr r3, [r5, #4]
	movs r1, #160
	subs r0, r0, r3
	lsls r1, r1, #14
	adds r0, r0, r1
	movs r1, #12
	bl __divsi3
	str r0, [r5, #16]
	mov r2, r11
	ldr r0, [r2, #16]
	ldr r3, [r5, #8]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	movs r3, #0
	str r0, [r5, #20]
	str r3, [r5, #24]
	ldr r3, [sp, #56]
	adds r5, #28
	adds r3, #1
	str r3, [sp, #56]
	cmp r3, #3
	bne .L_081466ea
	movs r4, #0
	str r4, [sp, #68]
.L_08146754:
	ldr r0, [sp, #68]
	cmp r0, #47
	bgt .L_08146788
	movs r2, #128
	cmp r0, #39
	ble .L_0814676a
	ldr r1, [sp, #68]
	movs r3, #192
	lsls r2, r1, #4
	lsls r3, r3, #2
	subs r2, r3, r2
.L_0814676a:
	ldr r4, [sp, #80]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0814677e
	ldr r0, [sp, #48]
	ldrh r3, [r0, #54]
	adds r1, r0, #0
	subs r3, r3, r2
	strh r3, [r1, #54]
	b .L_08146788
.L_0814677e:
	ldr r4, [sp, #48]
	ldrh r3, [r4, #54]
	adds r0, r4, #0
	adds r3, r3, r2
	strh r3, [r0, #54]
.L_08146788:
	ldr r2, [sp, #76]
	movs r1, #0
	str r1, [sp, #56]
	str r1, [sp, #24]
	str r2, [sp, #20]
	str r1, [sp, #16]
.L_08146794:
	ldr r3, [sp, #68]
	ldr r4, [sp, #16]
	cmp r3, r4
	bge .L_0814679e
	b .L_081469f8
.L_0814679e:
	ldr r0, [sp, #20]
	subs r5, r3, r4
	mov r10, r0
	adds r3, r5, #0
	cmp r5, #0
	bge .L_081467ac
	adds r3, r5, #3
.L_081467ac:
	asrs r3, r3, #2
	adds r3, #2
	mov r8, r3
	cmp r3, #10
	ble .L_081467ba
	movs r1, #10
	mov r8, r1
.L_081467ba:
	bl Func_08014de4
	ldr r0, [sp, #48]
	adds r1, r0, #0
	adds r1, #12
	bl Func_080156e8
	ldr r0, [sp, #20]
	bl Func_08015128
	movs r2, #0
	str r2, [sp, #44]
	movs r0, #128
	ldr r2, [sp, #24]
	lsls r3, r5, #12
	lsls r0, r0, #5
	adds r0, r3, r0
	str r0, [sp, #36]
	lsls r3, r2, #3
	ldr r0, .L_081469bc
	movs r1, #96
	subs r3, r3, r2
	add r1, sp
	lsls r3, r3, #2
	str r5, [sp, #40]
	movs r4, #0
	add r7, sp, #84
	add r6, sp, #108
	mov r9, r1
	adds r5, r3, r0
.L_081467f6:
	str r4, [sp, #12]
	bl Func_08014e38
	ldr r1, [sp, #40]
	lsls r0, r1, #10
	bl Func_080150e4
	movs r0, #128
	lsls r0, r0, #7
	bl Func_08015068
	ldr r2, [sp, #36]
	movs r3, #128
	lsls r3, r3, #9
	str r2, [r7]
	ldr r4, [sp, #12]
	cmp r2, r3
	ble .L_0814681c
	str r3, [r7]
.L_0814681c:
	ldr r3, [r7]
	adds r0, r7, #0
	str r3, [r7, #4]
	str r3, [r7, #8]
	str r4, [sp, #12]
	bl Func_080151ac
	movs r3, #200
	ldr r4, [sp, #12]
	lsls r3, r3, #5
	adds r3, #154
	adds r0, r4, #0
	muls r0, r3
	bl Func_080150e4
	ldr r4, [sp, #12]
	movs r3, #1
	ands r3, r4
	lsls r0, r3, #1
	adds r0, r0, r3
	ldr r3, .L_081469c0
	lsls r0, r0, #2
	adds r0, r0, r3
	adds r1, r6, #0
	bl Func_0815e1ec
	ldr r3, [sp, #44]
	ldr r4, [sp, #12]
	cmp r3, r0
	bge .L_0814685a
	str r0, [sp, #44]
.L_0814685a:
	ldr r2, [r6]
	mov r0, r9
	asrs r2, r2, #1
	str r2, [r6]
	str r4, [sp, #12]
	ldr r3, [r0]
	adds r2, r2, r3
	str r2, [r5, #12]
	ldr r3, [r0, #4]
	ldr r2, [r6, #4]
	adds r2, r2, r3
	str r2, [r5, #16]
	ldr r3, [r6]
	str r3, [r5, #12]
	ldr r3, [r6, #4]
	str r3, [r5, #16]
	bl Func_08014ea8
	ldr r4, [sp, #12]
	adds r5, #28
	adds r4, #1
	cmp r4, #10
	bne .L_081467f6
	ldr r1, [sp, #44]
	ldr r2, .L_081469c4
	cmp r1, r2
	bgt .L_08146926
	ldr r3, [sp, #24]
	mov r1, r8
	str r3, [sp, #32]
	lsrs r3, r1, #31
	add r3, r8
	asrs r3, r3, #1
	mov r0, r8
	str r3, [sp, #28]
	lsls r0, r0, #1
	movs r4, #0
	mov r9, r0
.L_081468a6:
	ldr r3, [sp, #32]
	ldr r0, .L_081469bc
	adds r2, r4, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r4, #1
	adds r7, r3, r0
	movs r1, #10
	adds r0, r4, #0
	str r4, [sp, #8]
	bl Math_Mod
	ldr r1, [sp, #32]
	ldr r2, .L_081469bc
	adds r0, r0, r1
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	adds r6, r3, r2
	movs r5, #0
.L_081468d0:
	ldr r3, [r6, #12]
	ldr r2, [r7, #12]
	subs r3, r3, r2
	muls r3, r5
	cmp r3, #0
	bge .L_081468de
	adds r3, #15
.L_081468de:
	asrs r3, r3, #4
	adds r0, r2, r3
	ldr r3, [r6, #16]
	ldr r2, [r7, #16]
	subs r3, r3, r2
	muls r3, r5
	cmp r3, #0
	bge .L_081468f0
	adds r3, #15
.L_081468f0:
	asrs r3, r3, #4
	adds r2, r2, r3
	ldr r3, .L_081469c8
	mov r12, r2
	mov r2, r9
	subs r2, #2
	ldrh r1, [r3, r2]
	ldr r4, [sp, #52]
	ldr r3, [sp, #28]
	adds r1, r4, r1
	subs r2, r0, r3
	mov r4, r12
	mov r0, r8
	subs r3, r4, r0
	mov r4, r9
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #72]
	ldr r4, [sp, #60]
	adds r5, #1
	mov lr, r4
	.2byte 0xf800
	cmp r5, #16
	bne .L_081468d0
	ldr r4, [sp, #8]
	cmp r4, #10
	bne .L_081468a6
.L_08146926:
	mov r0, r10
	ldr r3, [r0]
	ldr r2, [r0, #12]
	adds r3, r3, r2
	str r3, [r0]
	ldr r2, [r0, #16]
	ldr r3, [r0, #4]
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r2, [r0, #20]
	ldr r3, [r0, #8]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r1, [sp, #16]
	ldr r2, [sp, #56]
	ldr r4, [sp, #68]
	adds r3, r1, r2
	adds r3, #10
	cmp r4, r3
	bne .L_081469f8
	movs r3, #128
	mov r0, r11
	lsls r3, r3, #10
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #48]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	mov r2, r11
	str r3, [r0, #72]
	adds r2, #90
	movs r3, #0
	strb r3, [r2]
	bl Object_ResetMotion
	mov r2, r11
	ldr r1, [r2, #8]
	cmp r1, #0
	bge .L_0814698e
	ldr r3, .L_081469cc
	mov r0, r11
	adds r1, r1, r3
	ldr r3, [r2, #16]
	movs r2, #0
	bl Object_SetPosition
	b .L_0814699e
.L_0814698e:
	movs r4, #160
	lsls r4, r4, #14
	mov r0, r11
	adds r1, r1, r4
	ldr r3, [r0, #16]
	movs r2, #0
	bl Object_SetPosition
.L_0814699e:
	ldr r1, [sp, #56]
	cmp r1, #2
	bne .L_081469d0
	movs r0, #134
	bl Func_08118088 + 0x60
	b .L_081469ea
.L_081469ac:
	.4byte 0x0000013a
.L_081469b0:
	.4byte IwramCopyWords
.L_081469b4:
	.4byte 0x00000134
.L_081469b8:
	.4byte Func_08143000
.L_081469bc:
	.4byte gMapCellBuffer
.L_081469c0:
	.4byte Data_08197924
.L_081469c4:
	.4byte 0x00061a7f
.L_081469c8:
	.4byte Data_08197410
.L_081469cc:
	.4byte 0xffd80000
.L_081469d0:
	movs r0, #134
	bl Audio_PlayCue
	ldr r3, [sp, #80]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_081469ea:
	ldr r4, [sp, #76]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r4, r0
	movs r3, #4
	str r3, [r2]
.L_081469f8:
	ldr r1, [sp, #24]
	ldr r2, [sp, #20]
	ldr r3, [sp, #16]
	ldr r4, [sp, #56]
	adds r1, #10
	adds r2, #28
	adds r3, #12
	adds r4, #1
	str r1, [sp, #24]
	str r2, [sp, #20]
	str r3, [sp, #16]
	str r4, [sp, #56]
	cmp r4, #3
	beq .L_08146a16
	b .L_08146794
.L_08146a16:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #68]
	adds r2, #1
	str r2, [sp, #68]
	cmp r2, #60
	beq .L_08146a42
	b .L_08146754
.L_08146a42:
	ldr r0, .L_08146a68
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #120
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08146a68:
	.4byte Func_08143000
