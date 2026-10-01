.syntax unified
	.thumb
	.global Func_081514bc
	.thumb_func
Func_081514bc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #96
	str r0, [sp, #48]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	movs r6, #0
	str r0, [sp, #44]
	movs r0, #1
	ldr r1, [r3, #96]
	mov r8, r6
	str r1, [sp, #40]
	ldr r3, [r3, #100]
	str r3, [sp, #32]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08151524
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r2, [sp, #44]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08151528
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_0815152c
	ldr r1, [sp, #32]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	mov r4, sp
	adds r4, #52
	movs r0, #0
	adds r1, r4, #0
	str r4, [sp, #28]
	bl Func_08144aac
	ldr r5, [sp, #44]
	b .L_08151530
	.2byte 0x0000
.L_08151524:
	.4byte 0x00000100
.L_08151528:
	.4byte 0x0000017c
.L_0815152c:
	.4byte 0x0000017e
.L_08151530:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #56
	str r3, [r5, #8]
	bl Random16
	movs r3, #31
	ands r3, r0
	movs r0, #1
	subs r3, #64
	add r8, r0
	lsls r3, r3, #16
	mov r1, r8
	str r3, [r5, #4]
	adds r5, #28
	cmp r1, #64
	bne .L_08151530
	ldr r3, [sp, #44]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r6, [sp, #44]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r6, r0
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0815183c
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #48]
	ldr r3, [r1, #4]
	cmp r3, #1
	bne .L_0815159a
	movs r2, #128
	ldr r3, .L_08151840
	lsls r2, r2, #19
	adds r2, #40
	str r3, [r2]
.L_0815159a:
	movs r2, #0
	str r2, [sp, #36]
	ldr r3, [sp, #48]
	ldr r1, .L_08151844
	ldr r2, [r3, #24]
	movs r4, #50
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	ldrb r3, [r1, r3]
	negs r4, r4
	cmp r3, r4
	bne .L_081515b6
	b .L_08151b00
.L_081515b6:
	mov r6, sp
	adds r6, #60
	str r6, [sp, #8]
.L_081515bc:
	movs r0, #240
	movs r3, #0
	lsls r0, r0, #15
	str r3, [sp, #16]
	str r0, [sp, #20]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	ldrb r3, [r1, r3]
	ldr r4, [sp, #36]
	adds r3, #11
	cmp r4, r3
	bne .L_081515dc
	movs r0, #133
	bl Func_081180e8
.L_081515dc:
	ldr r0, [sp, #8]
	ldr r6, [sp, #16]
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r0, #8]
	str r6, [r0]
	str r6, [r0, #4]
	bl Func_08014de4
	ldr r0, [sp, #8]
	bl SceneTransform_ApplyPosition
	ldr r1, [sp, #36]
	subs r1, #36
	str r1, [sp, #12]
	cmp r1, #27
	bhi .L_0815160e
	ldr r2, [sp, #36]
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	bne .L_0815160e
	movs r0, #115
	bl Audio_PlayCue
.L_0815160e:
	ldr r3, [sp, #36]
	cmp r3, #85
	bne .L_0815161a
	movs r0, #136
	bl Audio_PlayCue
.L_0815161a:
	ldr r6, [sp, #48]
	movs r4, #0
	ldr r3, [r6, #20]
	mov r8, r4
	cmp r3, #0
	beq .L_08151652
	movs r6, #36
	movs r5, #40
.L_0815162a:
	ldr r0, [sp, #36]
	cmp r0, r5
	bne .L_08151646
	ldr r1, [sp, #48]
	movs r3, #0
	ldrsh r0, [r6, r1]
	str r3, [sp, #0]
	movs r1, #9
	subs r3, #1
	movs r2, #5
	bl Func_0814cd48
	ldr r4, [sp, #48]
	ldr r3, [r4, #20]
.L_08151646:
	movs r0, #1
	add r8, r0
	adds r6, #2
	adds r5, #4
	cmp r8, r3
	bne .L_0815162a
.L_08151652:
	ldr r1, [sp, #48]
	ldr r0, .L_08151844
	ldr r5, [r1, #24]
	ldr r6, [sp, #36]
	lsls r3, r5, #1
	adds r2, r3, r5
	adds r3, r2, #2
	ldrb r3, [r0, r3]
	movs r4, #16
	adds r1, r5, #0
	str r4, [sp, #24]
	cmp r6, r3
	bge .L_08151670
	ldrb r2, [r0, r2]
	str r2, [sp, #24]
.L_08151670:
	lsls r3, r1, #1
	adds r3, r3, r1
	adds r3, #2
	ldrb r3, [r0, r3]
	ldr r0, [sp, #36]
	adds r3, #35
	cmp r0, r3
	blt .L_08151682
	b .L_081517b2
.L_08151682:
	ldr r2, [sp, #24]
	movs r1, #0
	mov r8, r1
	cmp r2, #0
	bne .L_0815168e
	b .L_081517b2
.L_0815168e:
	ldr r6, [sp, #44]
	movs r3, #84
	movs r4, #72
	add r3, sp
	add r4, sp
	mov r11, r3
	mov r9, r4
	mov r10, r6
.L_0815169e:
	ldr r0, [sp, #36]
	cmp r0, r8
	ble .L_0815179e
	mov r1, r8
	cmp r1, #0
	bge .L_081516ac
	adds r1, #7
.L_081516ac:
	asrs r7, r1, #3
	lsls r3, r7, #3
	mov r1, r8
	subs r7, r1, r3
	lsrs r3, r1, #31
	add r3, r8
	asrs r3, r3, #1
	mov r6, r10
	movs r2, #48
	subs r2, r2, r3
	ldr r3, [r6, #4]
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_08151734
	ldr r2, .L_08151848
	cmp r3, r2
	ble .L_08151734
	ldr r0, [r6]
	bl Trig_Sin
	ldr r3, [r6, #8]
	mov r4, r11
	muls r3, r0
	str r3, [r4]
	ldr r3, [r6, #4]
	str r3, [r4, #4]
	ldr r0, [r6]
	bl Trig_Cos
	ldr r3, [r6, #8]
	mov r1, r9
	muls r3, r0
	mov r0, r11
	str r3, [r0, #8]
	bl Func_0815e1ec
	mov r1, r9
	ldr r2, [r1]
	movs r4, #6
	ldrsh r3, [r1, r4]
	ldr r0, .L_0815184c
	asrs r2, r2, #17
	adds r2, #64
	adds r3, #60
	str r2, [r1]
	str r3, [r1, #4]
	lsls r1, r7, #1
	ldrh r1, [r0, r1]
	ldr r0, [sp, #44]
	movs r4, #224
	adds r1, r0, r1
	ldr r0, .L_08151850
	lsls r4, r4, #3
	ldrb r5, [r0, r7]
	adds r1, r1, r4
	lsrs r0, r5, #1
	subs r2, r2, r0
	ldr r0, .L_08151854
	ldrb r4, [r0, r7]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #28]
	ldr r4, [r0, #4]
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
.L_08151734:
	ldr r1, [sp, #48]
	ldr r2, .L_08151844
	ldr r5, [r1, #24]
	ldr r4, [sp, #36]
	lsls r3, r5, #1
	adds r3, r3, r5
	adds r3, #2
	ldrb r3, [r2, r3]
	cmp r4, r3
	bge .L_08151776
	mov r3, r8
	adds r3, #16
	cmp r4, r3
	ble .L_0815179e
	ldr r3, [r6, #8]
	cmp r3, #4
	ble .L_0815175a
	subs r3, #2
	str r3, [r6, #8]
.L_0815175a:
	ldr r3, [r6, #4]
	ldr r0, .L_08151858
	cmp r3, r0
	bgt .L_0815176a
	movs r1, #160
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r6, #4]
.L_0815176a:
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r6]
	b .L_0815179e
.L_08151776:
	ldr r3, [r6, #8]
	mov r0, r8
	adds r3, #8
	str r3, [r6, #8]
	movs r1, #5
	bl Math_Mod
	ldr r3, [r6, #4]
	adds r0, #2
	lsls r0, r0, #16
	subs r3, r3, r0
	str r3, [r6, #4]
	ldr r4, [sp, #20]
	cmp r4, r3
	ble .L_08151796
	str r3, [sp, #20]
.L_08151796:
	ldr r6, [sp, #16]
	cmp r6, r3
	bge .L_0815179e
	str r3, [sp, #16]
.L_0815179e:
	ldr r2, [sp, #24]
	movs r1, #1
	movs r0, #28
	add r8, r1
	add r10, r0
	cmp r8, r2
	beq .L_081517ae
	b .L_0815169e
.L_081517ae:
	ldr r3, [sp, #48]
	ldr r5, [r3, #24]
.L_081517b2:
	ldr r4, [sp, #20]
	ldr r6, [sp, #16]
	movs r3, #128
	lsls r3, r3, #15
	adds r4, r4, r3
	adds r6, r6, r3
	str r4, [sp, #20]
	lsls r3, r5, #1
	str r6, [sp, #16]
	ldr r0, .L_08151844
	adds r2, r3, r5
	adds r3, r2, #2
	ldrb r3, [r0, r3]
	ldr r1, [sp, #36]
	cmp r1, r3
	bge .L_081518aa
	movs r3, #0
	mov r8, r3
	adds r3, r2, #1
	ldrb r3, [r0, r3]
	cmp r3, #0
	beq .L_081518aa
	ldr r4, .L_0815185c
	ldr r6, .L_08151860
	mov r10, r4
	mov r9, r6
	movs r7, #0
.L_081517e8:
	ldr r0, [sp, #12]
	movs r1, #3
	bl Math_Div
	cmp r8, r0
	bge .L_08151894
	movs r1, #3
	mov r0, r8
	bl Math_Mod
	lsls r3, r5, #1
	adds r4, r0, #0
	ldr r0, .L_08151844
	adds r3, r3, r5
	adds r3, #2
	ldrb r3, [r0, r3]
	ldr r1, [sp, #36]
	subs r3, #7
	cmp r1, r3
	blt .L_08151868
	lsls r3, r4, #1
	mov r2, r10
	ldrh r1, [r2, r3]
	ldr r3, [sp, #44]
	movs r0, #224
	adds r1, r3, r1
	lsls r0, r0, #3
	adds r1, r1, r0
	ldr r0, .L_08151864
	mov r3, r9
	ldrb r4, [r0, r4]
	movs r0, #32
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #1]
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #28]
	subs r3, r3, r4
	ldr r4, [r0, #4]
	ldr r0, [sp, #40]
	b .L_0815188c
	.2byte 0x0000
.L_0815183c:
	.4byte Func_08143000
.L_08151840:
	.4byte 0xffff9000
.L_08151844:
	.4byte Data_08198348
.L_08151848:
	.4byte 0xffd00000
.L_0815184c:
	.4byte Data_08198362
.L_08151850:
	.4byte Data_08198351
.L_08151854:
	.4byte Data_08198359
.L_08151858:
	.4byte 0x002fffff
.L_0815185c:
	.4byte Data_08198372
.L_08151860:
	.4byte Data_08198322
.L_08151864:
	.4byte Data_08198378
.L_08151868:
	lsls r3, r4, #1
	mov r2, r10
	ldrh r1, [r2, r3]
	ldr r3, [sp, #44]
	movs r0, #224
	adds r1, r3, r1
	lsls r0, r0, #3
	adds r1, r1, r0
	ldr r0, .L_08151b24
	ldrb r3, [r6, #1]
	ldrb r4, [r0, r4]
	movs r0, #32
	ldrb r2, [r6]
	subs r3, r3, r4
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #40]
	ldr r4, [sp, #52]
.L_0815188c:
	mov lr, r4
	.2byte 0xf800
	ldr r1, [sp, #48]
	ldr r5, [r1, #24]
.L_08151894:
	lsls r3, r5, #1
	ldr r4, .L_08151b28
	adds r3, r3, r5
	adds r3, #1
	ldrb r3, [r4, r3]
	movs r2, #1
	add r8, r2
	adds r6, #2
	adds r7, #2
	cmp r8, r3
	bne .L_081517e8
.L_081518aa:
	lsls r3, r5, #1
	ldr r6, .L_08151b28
	adds r3, r3, r5
	adds r3, #2
	ldrb r3, [r6, r3]
	ldr r0, [sp, #36]
	cmp r0, r3
	bne .L_08151910
	ldr r5, .L_08151b2c
	movs r1, #0
	mov r8, r1
	movs r6, #15
.L_081518c2:
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	ands r0, r6
	adds r0, #80
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	movs r3, #63
	ands r3, r0
	subs r3, #32
	lsls r3, r3, #12
	str r3, [r5, #8]
	bl Random16
	negs r0, r0
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl Random16
	movs r2, #1
	ands r0, r6
	add r8, r2
	adds r0, #16
	mov r3, r8
	str r0, [r5, #24]
	adds r5, #28
	cmp r3, #32
	bne .L_081518c2
	ldr r4, [sp, #48]
	ldr r5, [r4, #24]
.L_08151910:
	lsls r3, r5, #1
	ldr r6, .L_08151b28
	adds r3, r3, r5
	adds r3, #2
	ldrb r3, [r6, r3]
	ldr r0, [sp, #36]
	cmp r0, r3
	blt .L_081519a2
	ldr r5, .L_08151b2c
	movs r1, #0
	mov r8, r1
.L_08151926:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_08151980
	mov r3, r8
	cmp r3, #0
	bge .L_08151934
	adds r3, #7
.L_08151934:
	asrs r4, r3, #3
	lsls r3, r4, #3
	mov r2, r8
	subs r4, r2, r3
	ldr r2, .L_08151b30
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	movs r0, #2
	ldrsh r2, [r5, r0]
	ldr r0, .L_08151b34
	ldr r3, [sp, #44]
	ldrb r0, [r0, r4]
	movs r6, #224
	adds r1, r3, r1
	lsls r6, r6, #3
	adds r1, r1, r6
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	ldr r0, .L_08151b38
	ldrb r0, [r0, r4]
	str r0, [sp, #4]
	ldr r0, [sp, #28]
	ldr r4, [r0, #4]
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08151980:
	ldr r3, [r5, #4]
	ldr r1, [sp, #20]
	cmp r1, r3
	ble .L_0815198a
	str r3, [sp, #20]
.L_0815198a:
	ldr r2, [sp, #16]
	cmp r2, r3
	bge .L_08151992
	str r3, [sp, #16]
.L_08151992:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #28
	cmp r4, #24
	bne .L_08151926
	ldr r6, [sp, #48]
	ldr r5, [r6, #24]
.L_081519a2:
	ldr r0, [sp, #20]
	ldr r1, [sp, #16]
	asrs r0, r0, #16
	asrs r1, r1, #16
	str r0, [sp, #20]
	str r1, [sp, #16]
	cmp r1, r0
	bgt .L_081519b6
	adds r0, #1
	str r0, [sp, #16]
.L_081519b6:
	lsls r3, r5, #1
	ldr r2, .L_08151b28
	adds r3, r3, r5
	adds r3, #2
	ldrb r3, [r2, r3]
	ldr r4, [sp, #36]
	cmp r4, r3
	bne .L_08151a18
	ldr r5, [sp, #44]
	movs r6, #0
	mov r8, r6
.L_081519cc:
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5, #12]
	ldr r0, [sp, #16]
	ldr r1, [sp, #20]
	cmp r0, r1
	bne .L_081519e6
	lsls r3, r1, #16
	str r3, [r5, #16]
	b .L_081519fc
.L_081519e6:
	bl Random16
	ldr r2, [sp, #16]
	ldr r3, [sp, #20]
	subs r1, r2, r3
	bl Math_ModU
	ldr r4, [sp, #20]
	adds r0, r0, r4
	lsls r0, r0, #16
	str r0, [r5, #16]
.L_081519fc:
	bl Random16
	movs r6, #1
	movs r3, #15
	ands r3, r0
	add r8, r6
	adds r3, #20
	mov r0, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #32
	bne .L_081519cc
	ldr r1, [sp, #48]
	ldr r5, [r1, #24]
.L_08151a18:
	lsls r3, r5, #1
	ldr r2, .L_08151b28
	adds r3, r3, r5
	adds r3, #2
	ldrb r3, [r2, r3]
	ldr r4, [sp, #36]
	cmp r4, r3
	blt .L_08151ad0
	subs r3, r4, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r7, .L_08151b28
	ldr r5, [sp, #44]
	asrs r3, r3, #1
	movs r6, #0
	mov r10, r3
	mov r8, r6
.L_08151a3a:
	ldr r3, [r5, #24]
	cmp r3, #17
	bhi .L_08151a7a
	movs r0, #17
	subs r0, r0, r3
	lsrs r3, r0, #31
	ldr r2, .L_08151b3c
	adds r0, r0, r3
	asrs r0, r0, #1
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #32]
	adds r1, r2, r1
	movs r3, #14
	ldrsh r2, [r5, r3]
	ldr r3, .L_08151b40
	ldrb r4, [r3, r0]
	movs r6, #18
	ldrsh r3, [r5, r6]
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r6, [sp, #28]
	lsrs r0, r4, #1
	subs r2, r2, r0
	subs r3, r3, r0
	mov r0, r10
	subs r3, r3, r0
	ldr r4, [r6, #4]
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
.L_08151a7a:
	movs r0, #1
	subs r3, #1
	negs r0, r0
	str r3, [r5, #24]
	cmp r3, r0
	beq .L_08151a8a
	cmp r3, #17
	bne .L_08151ac4
.L_08151a8a:
	ldr r1, [sp, #48]
	ldr r3, [r1, #24]
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r2, #2
	ldrb r3, [r7, r2]
	ldr r2, [sp, #36]
	adds r3, #35
	cmp r2, r3
	bge .L_08151ac4
	movs r3, #17
	str r3, [r5, #24]
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5, #12]
	bl Random16
	ldr r3, [sp, #16]
	ldr r4, [sp, #20]
	subs r1, r3, r4
	bl Math_ModU
	ldr r6, [sp, #20]
	adds r0, r0, r6
	lsls r0, r0, #16
	str r0, [r5, #16]
.L_08151ac4:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #32
	bne .L_08151a3a
.L_08151ad0:
	ldr r3, [sp, #44]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #36]
	ldr r0, [sp, #48]
	adds r6, #1
	str r6, [sp, #36]
	ldr r1, .L_08151b28
	ldr r2, [r0, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	ldrb r3, [r1, r3]
	adds r3, #50
	cmp r6, r3
	beq .L_08151b00
	b .L_081515bc
.L_08151b00:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08151b44
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08151b24:
	.4byte Data_08198378
.L_08151b28:
	.4byte Data_08198348
.L_08151b2c:
	.4byte gMapCellBuffer
.L_08151b30:
	.4byte Data_0819838c
.L_08151b34:
	.4byte Data_0819837b
.L_08151b38:
	.4byte Data_08198383
.L_08151b3c:
	.4byte Data_0819744c
.L_08151b40:
	.4byte Data_0819745e
.L_08151b44:
	.4byte Func_08143000
