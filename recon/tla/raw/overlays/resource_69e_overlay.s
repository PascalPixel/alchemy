.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02002fc0
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_02002ff0
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_0200301c
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	ldr r0, .L_02008060
	bl Func_020029d0
	pop {pc}
	.2byte 0x0000
.L_02008060:
	.4byte Data_02002fb8
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	adds r0, r7, #0
	bl Object_GetById
	adds r5, r0, #0
	cmp r7, #11
	bne .L_020080c2
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080c6
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #13
	bne .L_020080c6
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #34
	bne .L_020080c6
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r6, #15
.L_0200809a:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #1
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_0200809a
	adds r0, r7, #0
	bl Func_02000e04
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #9
	bl GameFlag_SetBit
	b .L_020080c6
.L_020080c2:
	bl Func_020029d8
.L_020080c6:
	pop {r5, r6, r7, pc}
	.section .text.x020080c8,"ax",%progbits
	.global Func_020000c8
	.thumb_func
Func_020000c8:
	push {lr}
	ldr r1, [r0, #80]
	adds r0, #100
	ldrh r3, [r0]
	movs r2, #3
	ands r2, r3
	ldr r4, [r1, #40]
	cmp r2, #1
	beq .L_020080fc
	cmp r2, #1
	bgt .L_020080e4
	cmp r2, #0
	beq .L_020080ee
	b .L_02008118
.L_020080e4:
	cmp r2, #2
	beq .L_02008100
	cmp r2, #3
	beq .L_0200810e
	b .L_02008118
.L_020080ee:
	movs r3, #7
	strb r3, [r4, #5]
	movs r3, #1
	strb r3, [r1, #25]
	movs r3, #2
	strb r3, [r1, #26]
	b .L_02008118
.L_020080fc:
	movs r3, #0
	b .L_02008108
.L_02008100:
	movs r2, #7
	movs r3, #0
	strb r2, [r4, #5]
	movs r2, #1
.L_02008108:
	strb r2, [r1, #25]
	strb r3, [r1, #26]
	b .L_02008118
.L_0200810e:
	movs r2, #0
	movs r3, #1
	strb r2, [r4, #5]
	strb r3, [r1, #25]
	strb r2, [r1, #26]
.L_02008118:
	ldrh r3, [r0]
	adds r3, #1
	strh r3, [r0]
	pop {pc}
	.section .text.x02008120,"ax",%progbits
	.global Func_02000120
	.thumb_func
Func_02000120:
	push {r5, lr}
	ldr r3, .L_02008144
	adds r5, r0, #0
	ldr r0, [r3]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_02008140
	movs r1, #6
	lsrs r0, r0, #1
	bl Engine_MathModulo
	adds r1, r0, #0
	adds r0, r5, #0
	bl Animation_ApplyChildValues
.L_02008140:
	pop {r5, pc}
	.2byte 0x0000
.L_02008144:
	.4byte Data_0300122c
	.section .text.x02008148,"ax",%progbits
	.global Func_02000148
	.thumb_func
Func_02000148:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r6, r5, #0
	adds r6, #100
	ldrh r1, [r6]
	mov r10, r0
	mov r8, r1
	mov r0, r8
	bl Math_Cosine
	ldr r3, [r5, #48]
	mov r1, r10
	adds r3, #3
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r1, #8]
	mov r0, r8
	adds r3, r3, r2
	str r3, [r5, #8]
	bl Math_Sine
	mov r2, r10
	ldr r3, [r2, #16]
	ldr r2, [r5, #8]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r3, [r5, #16]
	str r2, [r5, #56]
	str r3, [r5, #64]
	ldr r1, .L_020081a0
	ldrh r3, [r6]
	adds r3, r3, r1
	strh r3, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081a0:
	.4byte 0xfffff800
	.section .text.x020081a4,"ax",%progbits
	.global Func_020001a4
	.thumb_func
Func_020001a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081c6
	ldr r3, .L_020081e8
	movs r1, #3
	ldr r0, [r3]
	bl Engine_MathModulo
	cmp r0, #0
	bne .L_0200828c
.L_020081c6:
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081ec
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	b .L_020081f6
.L_020081e8:
	.4byte Data_0300122c
.L_020081ec:
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #6
.L_020081f6:
	lsrs r2, r2, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	ldr r3, .L_0200827c
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	bl Func_02002828
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200828c
	ldr r1, .L_02008280
	adds r0, r7, #0
	ldr r6, [r7, #80]
	bl Func_02002820
	movs r1, #1
	adds r0, r7, #0
	bl Object_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_02008284
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_02008288
	ldr r1, .L_02008278
	str r3, [r7, #108]
	mov r8, r1
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	mov r3, r8
	ldrb r2, [r6, #9]
	strb r3, [r6, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
	b .L_0200828c
	.2byte 0x0000
.L_02008278:
	.4byte 0x00000000
.L_0200827c:
	.4byte 0xffe40000
.L_02008280:
	.4byte Data_02003214
.L_02008284:
	.4byte 0x0ffff000
.L_02008288:
	.4byte Func_02000148
.L_0200828c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008294,"ax",%progbits
	.global Func_02000294
	.thumb_func
Func_02000294:
	push {lr}
	ldr r3, .L_020082b0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_020082a4
	movs r0, #201
	bl Func_02002a40
.L_020082a4:
	ldr r3, .L_020082b4
	ldr r0, [r3]
	bl Func_02001650
	pop {pc}
	.2byte 0x0000
.L_020082b0:
	.4byte Data_02003278
.L_020082b4:
	.4byte Data_02003274
	.section .text.x020082b8,"ax",%progbits
	.global Func_020002b8
	.thumb_func
Func_020002b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008570
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	ldr r0, [r5]
	sub sp, #52
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #13
	bl Object_GetById
	str r0, [sp, #24]
	movs r0, #14
	bl Object_GetById
	str r0, [sp, #20]
	movs r0, #15
	bl Object_GetById
	str r0, [sp, #16]
	movs r0, #16
	bl Object_GetById
	str r0, [sp, #12]
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #42
	beq .L_02008302
	b .L_0200869e
.L_02008302:
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r3, #41
	beq .L_0200830c
	b .L_0200869e
.L_0200830c:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #0
	bl Func_020029a8
	movs r1, #192
	lsls r1, r1, #8
	ldr r0, [r5]
	bl Func_02002940
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_02008574
	movs r0, #30
	str r3, [r7, #108]
	bl Battle_WaitMode0
	movs r0, #220
	bl Func_02002a40
	movs r1, #64
	adds r0, r7, #0
	bl ObjectDispatch_ApplyValueToChildren
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #178
	bl Func_02002a40
	adds r2, r7, #0
	adds r2, #100
	adds r3, r2, #0
	movs r5, #0
	str r2, [sp, #8]
	strh r5, [r3]
	ldr r3, .L_02008578
	movs r0, #30
	str r3, [r7, #108]
	bl Battle_WaitMode0
	ldr r3, [r7, #8]
	add r0, sp, #40
	str r3, [r0]
	ldr r3, [r7, #12]
	movs r1, #160
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	add r1, sp, #28
	str r3, [r0, #8]
	movs r3, #170
	lsls r3, r3, #18
	str r3, [r1]
	ldr r3, .L_0200857c
	str r5, [r1, #4]
	str r3, [r1, #8]
	bl Func_02001718
	ldr r3, .L_02008580
	ldr r2, .L_02008584
	str r5, [r3]
	movs r1, #144
	movs r3, #1
	str r3, [r2]
	ldr r0, .L_02008588
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_0200839e:
	ldr r2, .L_02008580
	movs r0, #20
	movs r3, #0
	subs r0, r0, r5
	str r3, [r2]
	mov r11, r3
	lsrs r3, r0, #31
	adds r0, r0, r3
	mov r9, r2
	asrs r0, r0, #1
	bl WaitFrames
	movs r1, #1
	mov r2, r9
	str r1, [r2]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #19
	ble .L_0200839e
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02002a40
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #9
	bl Func_02002870
	movs r3, #1
	mov r1, r9
	str r3, [r1]
	movs r0, #90
	bl Battle_WaitMode0
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02002870
	movs r2, #3
	mov r3, r9
	str r2, [r3]
	ldr r3, .L_02008584
	mov r1, r11
	str r1, [r3]
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #23
	bl ObjectMotion_SetActionVariant
	movs r0, #23
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #23
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #85
	mov r1, r11
	strb r1, [r3]
	adds r1, r2, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r5, #2
	orrs r3, r5
	strb r3, [r1]
	movs r6, #200
	ldr r3, .L_0200858c
	ldr r1, .L_02008590
	lsls r6, r6, #5
	adds r6, #153
	str r3, [r2, #28]
	str r6, [r2, #24]
	movs r0, #23
	mov r10, r3
	mov r8, r1
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #2
	movs r0, #24
	bl ObjectMotion_SetActionVariant
	movs r0, #24
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #24
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #85
	mov r1, r11
	strb r1, [r3]
	adds r1, r2, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r0, #24
	orrs r5, r3
	mov r3, r10
	strb r5, [r1]
	str r3, [r2, #28]
	str r6, [r2, #24]
	mov r1, r8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008594
	bl Scheduler_AddOrUpdateCallback
	movs r0, #144
	bl Func_02002a40
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_02002870
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02002980
	movs r1, #0
	ldr r0, .L_02008598
	bl Func_02002978
	movs r0, #60
	bl Func_02002988
	movs r0, #60
	bl WaitFrames
	movs r0, #144
	bl Func_02002a40
	movs r0, #30
	bl WaitFrames
	movs r0, #144
	bl Func_02002a40
	movs r0, #30
	bl WaitFrames
	movs r0, #144
	bl Func_02002a40
	movs r0, #30
	bl WaitFrames
	movs r3, #41
	movs r2, #99
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #58
	movs r1, #99
	movs r2, #3
	movs r3, #3
	bl Func_02002860
	movs r3, #106
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #5
	movs r1, #32
	movs r2, #1
	movs r0, #123
	bl Func_02002860
	movs r0, #2
	bl Func_02000d80
	movs r0, #144
	bl Func_02002a40
	movs r0, #30
	bl WaitFrames
	ldr r1, .L_0200856c
	ldr r2, [sp, #8]
	movs r0, #1
	strh r1, [r2]
	bl WaitFrames
	mov r3, r11
	str r3, [r7, #108]
	adds r0, r7, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	ldr r3, .L_02008570
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	ldr r5, .L_0200859c
	movs r0, #23
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #24
	b .L_020085a0
	.2byte 0x0000
.L_0200856c:
	.4byte 0x00000003
.L_02008570:
	.4byte gPartyState
.L_02008574:
	.4byte Func_02000120
.L_02008578:
	.4byte Func_020000c8
.L_0200857c:
	.4byte 0x023e0000
.L_02008580:
	.4byte Data_02003274
.L_02008584:
	.4byte Data_02003278
.L_02008588:
	.4byte Func_02000294
.L_0200858c:
	.4byte 0xffff0000
.L_02008590:
	.4byte Data_02002f70
.L_02008594:
	.4byte Func_020001a4
.L_02008598:
	.4byte 0x004063ff
.L_0200859c:
	.4byte Data_02002f94
.L_020085a0:
	bl ObjectMotion_EnableActionAndSetCallback
	mov r2, r9
	movs r3, #2
	str r3, [r2]
	movs r0, #30
	bl Battle_WaitMode0
	mov r1, r9
	movs r3, #1
	str r3, [r1]
	movs r0, #30
	bl Battle_WaitMode0
	mov r3, r9
	mov r2, r11
	str r2, [r3]
	movs r0, #23
	bl Object_RefreshSelectorById
	movs r0, #24
	bl Object_RefreshSelectorById
	movs r0, #23
	bl Object_GetById
	mov r1, r11
	str r1, [r0, #24]
	movs r0, #24
	bl Object_GetById
	mov r2, r11
	str r2, [r0, #24]
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02002978
	movs r0, #30
	bl Func_02002988
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02002870
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #9
	movs r0, #0
	bl Func_02002870
	ldr r0, .L_02008648
	bl Scheduler_RemoveCallbackFar
	bl Func_0200185c
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, [sp, #24]
	ldr r6, .L_02008644
	adds r3, #85
	strb r6, [r3]
	ldr r3, [sp, #20]
	movs r5, #15
	adds r3, #85
	strb r6, [r3]
	ldr r3, [sp, #16]
	adds r3, #85
	strb r6, [r3]
	ldr r3, [sp, #12]
	adds r3, #85
	strb r6, [r3]
	b .L_0200864c
	.2byte 0x0000
.L_02008644:
	.4byte 0x00000000
.L_02008648:
	.4byte Func_02000294
.L_0200864c:
	ldr r1, [sp, #24]
	movs r2, #128
	ldr r3, [r1, #12]
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r1, [sp, #20]
	movs r0, #1
	ldr r3, [r1, #12]
	subs r5, #1
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r1, [sp, #16]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r1, [sp, #12]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
	bl WaitFrames
	cmp r5, #0
	bge .L_0200864c
	movs r0, #13
	bl Func_02000e04
	movs r0, #14
	bl Func_02000e04
	movs r0, #15
	bl Func_02000e04
	movs r0, #16
	bl Func_02000e04
	movs r0, #80
	bl Func_02002a40
	bl AudioCommand_WaitForStateByteClear
.L_0200869e:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x020086ac,"ax",%progbits
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push {lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #14
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200871e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #14
	bl GameFlag_SetBit
	bl Func_020028b0
	movs r0, #0
	bl Func_020029a8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02002980
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02002978
	movs r0, #30
	bl Func_02002988
	bl Event_WaitValue1c8Frames
	movs r1, #1
	movs r0, #0
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #2
	negs r2, r2
	ldr r0, .L_02008720
	movs r1, #0
	bl Func_02002898
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02002978
	movs r0, #30
	bl Func_02002988
	bl Func_020028b8
.L_0200871e:
	pop {pc}
.L_02008720:
	.4byte 0x00002b49
	.section .text.x02008724,"ax",%progbits
	.global Func_02000724
	.thumb_func
Func_02000724:
	push {lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #14
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008734,"ax",%progbits
	.global Func_02000734
	.thumb_func
Func_02000734:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #22
	sub sp, #56
	bl Object_GetById
	mov r10, r0
	movs r0, #190
	bl Func_02002a40
	movs r0, #22
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #1
	add r6, sp, #16
	str r3, [r6]
	movs r3, #5
	str r3, [r6, #4]
	movs r3, #168
	lsls r3, r3, #2
	strh r3, [r6, #24]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6, #12]
	movs r2, #0
	mov r8, r2
.L_0200877a:
	movs r0, #1
	bl Battle_WaitMode0
	movs r7, #1
	mov r3, r8
	ands r7, r3
	cmp r7, #0
	bne .L_020087d0
	bl Random16Far
	lsls r3, r0, #1
	mov r2, r10
	adds r3, r3, r0
	ldr r5, [r2, #8]
	lsls r3, r3, #3
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, r5, r3
	ldr r3, .L_02008808
	adds r5, r5, r3
	bl Random16Far
	mov r2, r10
	ldr r1, [r2, #12]
	lsls r0, r0, #5
	lsrs r0, r0, #16
	lsls r0, r0, #16
	movs r3, #128
	adds r1, r1, r0
	lsls r3, r3, #14
	adds r1, r1, r3
	ldr r3, .L_0200880c
	ldr r2, [r2, #16]
	str r3, [sp, #0]
	movs r3, #216
	lsls r3, r3, #13
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_02001c88
.L_020087d0:
	mov r2, r8
	cmp r2, #20
	bne .L_020087e0
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #1
	bl Func_02002918
.L_020087e0:
	movs r3, #1
	add r8, r3
	mov r2, r8
	cmp r2, #31
	bls .L_0200877a
	movs r1, #0
	movs r0, #22
	bl Func_02002918
	movs r0, #22
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008808:
	.4byte 0xfff40000
.L_0200880c:
	.4byte 0xfffc0000
	.section .text.x02008810,"ax",%progbits
	.global Func_02000810
	.thumb_func
Func_02000810:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	adds r6, r1, #0
	bl Func_02002a00
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #2
	bl Func_02002a18
	movs r0, #201
	bl Func_02002a40
	movs r0, #50
	bl WaitFrames
	movs r0, #82
	bl Func_02002a40
	movs r5, #0
.L_0200883a:
	adds r0, r6, #0
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r0, #6]
	adds r0, r6, #0
	bl Object_GetById
	movs r3, #4
	ands r3, r5
	movs r1, #7
	cmp r3, #0
	bne .L_0200885c
	movs r1, #0
.L_0200885c:
	bl Object_SetPartAttribute
	adds r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #119
	ble .L_0200883a
	adds r0, r6, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r0, #6]
	adds r0, r6, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Func_02002a18
	bl Func_02002a10
	bl Func_02002a08
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetModeById
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020088a4,"ax",%progbits
	.global Func_020008a4
	.thumb_func
Func_020008a4:
	push {lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #36
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020088b6
	b .L_02008ada
.L_020088b6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #35
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088c6
	b .L_02008ada
.L_020088c6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088d6
	b .L_02008ada
.L_020088d6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #34
	bl GameFlag_SetBit
	bl Func_020028b0
	movs r0, #0
	bl Func_020029a8
	ldr r0, .L_02008adc
	bl Func_02002928
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02002938
	movs r2, #140
	movs r0, #4
	movs r1, #68
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #136
	lsls r2, r2, #1
	movs r0, #4
	movs r1, #68
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02002940
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02002940
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #4
	bl Func_02002940
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02002940
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #22
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r3, #128
	movs r1, #128
	movs r2, #225
	lsls r3, r3, #7
	lsls r1, r1, #15
	lsls r2, r2, #16
	movs r0, #22
	bl Func_02002900
	bl Func_02000734
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #136
	movs r1, #1
	movs r2, #240
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	movs r1, #0
	movs r0, #22
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020089d0
	movs r0, #22
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02002938
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020089fa
.L_020089d0:
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #22
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02002938
.L_020089fa:
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02002938
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02002950
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02002938
	movs r0, #22
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02002938
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02002938
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #136
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #16
	movs r1, #0
	movs r0, #22
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #22
	bl Func_02000810
	movs r0, #1
	bl Func_020028a0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r0, [r3]
	movs r1, #5
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #5
	movs r0, #22
	bl Func_02002938
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_SetBit
	movs r0, #98
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_020028b8
.L_02008ada:
	pop {pc}
.L_02008adc:
	.4byte 0x00002a7a
	.section .text.x02008ae0,"ax",%progbits
	.global Func_02000ae0
	.thumb_func
Func_02000ae0:
	ldr r0, .L_02008ae4
	bx lr
.L_02008ae4:
	.4byte Data_0200327c
	.section .text.x02008ae8,"ax",%progbits
	.global Func_02000ae8
	.thumb_func
Func_02000ae8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #160
	adds r3, r3, r2
	lsls r0, r0, #4
	adds r2, #88
	str r2, [r3]
	adds r0, #35
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b18
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #34
	bl GameFlag_ClearBit
.L_02008b18:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b38
	movs r3, #128
	movs r1, #128
	movs r2, #225
	lsls r3, r3, #7
	movs r0, #22
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02002900
.L_02008b38:
	ldr r0, .L_02008be8
	bl Func_020029c8
	movs r0, #64
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	movs r0, #23
	bl Object_GetById
	adds r3, r0, #0
	movs r5, #0
	adds r3, #85
	str r5, [r0, #24]
	strb r5, [r3]
	movs r3, #170
	lsls r3, r3, #18
	str r3, [r0, #8]
	mov r8, r3
	movs r6, #145
	movs r3, #128
	lsls r3, r3, #14
	lsls r6, r6, #18
	str r3, [r0, #12]
	str r6, [r0, #16]
	movs r0, #23
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #24
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	str r5, [r0, #24]
	strb r5, [r3]
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #15
	str r2, [r0, #8]
	str r3, [r0, #12]
	str r6, [r0, #16]
	movs r0, #24
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008bde
	movs r3, #41
	movs r2, #99
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #58
	movs r1, #99
	movs r2, #3
	movs r3, #3
	bl Func_02002860
	movs r3, #106
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #123
	movs r1, #32
	movs r2, #1
	movs r3, #5
	bl Func_02002860
	movs r1, #144
	ldr r0, .L_02008bec
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_02008bde:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008be8:
	.4byte Data_02002fb8
.L_02008bec:
	.4byte Func_020001a4
	.section .text.x02008bf0,"ax",%progbits
	.global Func_02000bf0
	.thumb_func
Func_02000bf0:
	push {lr}
	movs r0, #0
	bl Func_02000c8c
	movs r0, #0
	bl Func_02000d80
	ldr r3, .L_02008c88
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c32
	movs r0, #208
	movs r1, #168
	lsls r0, r0, #16
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #236
	bl Func_02002a28
.L_02008c32:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c62
	movs r0, #10
	bl Func_02000e04
	movs r0, #12
	bl Func_02000e04
	movs r0, #13
	bl Func_02000e04
	movs r0, #14
	bl Func_02000e04
	movs r0, #15
	bl Func_02000e04
	movs r0, #16
	bl Func_02000e04
.L_02008c62:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #9
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c84
	movs r1, #216
	movs r2, #138
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_020028f8
	movs r0, #11
	bl Func_02000e04
.L_02008c84:
	movs r0, #0
	pop {pc}
.L_02008c88:
	.4byte gPartyState
	.section .text.x02008c8c,"ax",%progbits
	.global Func_02000c8c
	.thumb_func
Func_02000c8c:
	push {lr}
	movs r0, #20
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008cb4
	movs r0, #98
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_02008cb4:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008cb8,"ax",%progbits
	.global Func_02000cb8
	.thumb_func
Func_02000cb8:
	ldr r3, .L_02008cc0
	str r0, [r3]
	bx lr
	.2byte 0x0000
.L_02008cc0:
	.4byte Data_020033dc
	.section .text.x02008cc4,"ax",%progbits
	.global Func_02000cc4
	.thumb_func
Func_02000cc4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008d70
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_02008cd6
	adds r0, #3
.L_02008cd6:
	asrs r0, r0, #2
	movs r1, #5
	bl Engine_MathRemainder
	ldr r3, .L_02008d74
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008d2a
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_02008d0a
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_02008d66
.L_02008d0a:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_02008d66
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008d66
.L_02008d2a:
	movs r5, #0
	movs r6, #4
.L_02008d2e:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl Engine_MathRemainder
	ldr r3, .L_02008d78
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_02008d2e
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_02008d7c
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02008d70
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008d66:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d70:
	.4byte Data_020033d8
.L_02008d74:
	.4byte Data_020033dc
.L_02008d78:
	.4byte gOverlayArea + 0x3404
.L_02008d7c:
	.4byte 0x05000184
	.section .text.x02008d80,"ax",%progbits
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {r5, r6, lr}
	ldr r2, .L_02008ddc
	movs r3, #1
	adds r6, r0, #0
	str r3, [r2]
	cmp r6, #2
	beq .L_02008da4
	ldr r1, .L_02008de0
	movs r2, #32
	ldr r0, .L_02008de4
	ldr r5, .L_02008de8
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008dec
	ldr r1, .L_02008df0
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
.L_02008da4:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008db4
	cmp r6, #1
	bne .L_02008dc6
.L_02008db4:
	ldr r3, .L_02008df4
	movs r2, #0
	movs r1, #144
	str r2, [r3]
	ldr r0, .L_02008df8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_02008dda
.L_02008dc6:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008dfc
	ldr r1, .L_02008e00
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_02008dda:
	pop {r5, r6, pc}
.L_02008ddc:
	.4byte Data_020033dc
.L_02008de0:
	.4byte 0x05000180
.L_02008de4:
	.4byte gOverlayArea + 0x3404
.L_02008de8:
	.4byte IwramCopyWords
.L_02008dec:
	.4byte gOverlayArea + 0x3424
.L_02008df0:
	.4byte 0x050001a0
.L_02008df4:
	.4byte Data_020033d8
.L_02008df8:
	.4byte Func_02000cc4
.L_02008dfc:
	.4byte gOverlayArea + 0x3428
.L_02008e00:
	.4byte 0x05000184
	.section .text.x02008e04,"ax",%progbits
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #85
	movs r3, #4
	strb r3, [r1]
	movs r2, #0
	ldr r3, [r5, #20]
	str r2, [r5, #68]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r5, #12]
	subs r1, #50
	ldrb r2, [r1]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r3, r0, #0
	asrs r3, r3, #19
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	adds r3, #6
	movs r2, #0
	bl Func_02002888
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_02002a30
	pop {r5, pc}
	.section .text.x02008e58,"ax",%progbits
	.global Func_02000e58
	.thumb_func
Func_02000e58:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_02008f64
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_020028b0
	movs r0, #0
	bl Func_020029a8
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02008ef8
.L_02008eb2:
	ldr r3, [r7, #8]
	ldr r2, .L_02008f68
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_02008ece
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_02008ece:
	ldr r3, .L_02008f6c
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_02008eb2
.L_02008ef8:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_02008f60
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_02008f70
.L_02008f60:
	.4byte 0x00000001
.L_02008f64:
	.4byte gPartyState
.L_02008f68:
	.4byte 0x0003ffff
.L_02008f6c:
	.4byte Data_0300122c
.L_02008f70:
	bl Motion_CamBounds
	bl Func_02002970
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_02008fb8
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02002840
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_02008fd6
	b .L_02008fbc
	.2byte 0x0000
.L_02008fb8:
	.4byte 0x00000000
.L_02008fbc:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008fd6
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_02008fbc
.L_02008fd6:
	movs r0, #127
	bl Func_02002a40
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_02008ff6
.L_02008fe4:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008ff6
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008fe4
.L_02008ff6:
	adds r0, r7, #0
	bl Func_02002848
	ldr r5, .L_02009040
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Object_AttachWorkTargetToObject
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_020028b8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009040:
	.4byte gPartyState
	.section .text.x02009044,"ax",%progbits
	.global Func_02001044
	.thumb_func
Func_02001044:
	push {lr}
	ldr r3, [r1]
	ldr r4, [r0]
	ldr r2, [r1, #8]
	subs r4, r4, r3
	ldr r3, [r0, #8]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r2, r3, #0
	muls r2, r3
	adds r0, r4, #0
	muls r0, r4
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_0200906c
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_0200906c:
	.4byte IwramFillWords + 0x74
	.section .text.x02009070,"ax",%progbits
	.global Func_02001070
	.thumb_func
Func_02001070:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_020090d8
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_020090ce
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020090ce
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020090ce
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020090dc
.L_020090ce:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_0200921a
.L_020090d8:
	.4byte gPartyState
.L_020090dc:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_020090fc
	movs r0, #231
	bl Func_02002a40
.L_020090fc:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_02002a20
	cmp r0, #255
	beq .L_020091fe
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_020029b0
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_020091fe
	ldr r2, .L_020091ec
	cmp r5, r2
	blt .L_020091fe
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_020091c4
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_0200915c
	subs r5, r3, r2
.L_0200915c:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02001044
	cmp r0, #12
	bgt .L_0200917c
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_0200917c
	movs r2, #1
	mov r8, r2
.L_0200917c:
	mov r3, r8
	cmp r3, #0
	beq .L_020091c4
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020091c4
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_020091f0
	movs r2, #128
	ldr r0, .L_020091e8
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_020091c4:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_020091f4
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_020091f8
.L_020091e8:
	.4byte 0x00000000
.L_020091ec:
	.4byte 0xffe00000
.L_020091f0:
	.4byte gPartyState
.L_020091f4:
	.4byte IwramMulQ16
.L_020091f8:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_0200921a
.L_020091fe:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_02009228
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02002820
	movs r0, #228
	bl Func_02002a40
	ldr r3, .L_0200922c
	str r5, [r3]
.L_0200921a:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009228:
	.4byte Data_020033e0
.L_0200922c:
	.4byte gOverlayArea + 0x3400
	.section .text.x02009230,"ax",%progbits
	.global Func_02001230
	.thumb_func
Func_02001230:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_02002a40
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_020092e4
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_02002828
	movs r1, #2
	adds r7, r0, #0
	bl Func_02002810
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_020092e0
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_020092e8
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_020092ec
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_020092f0
.L_020092e0:
	.4byte 0x00000000
.L_020092e4:
	.4byte IwramMulQ16
.L_020092e8:
	.4byte Func_02001070
.L_020092ec:
	.4byte 0xfffa0000
.L_020092f0:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02001c88
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009308,"ax",%progbits
	.global Func_02001308
	.thumb_func
Func_02001308:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020093a4
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02009396
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl Engine_MathDivide
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_02001c88
.L_02009396:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020093a4:
	.4byte Data_0300122c
	.section .text.x020093a8,"ax",%progbits
	.global Func_020013a8
	.thumb_func
Func_020013a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020028f8
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020028f8
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020028f8
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_020028f8
	movs r0, #24
	bl Object_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009484
	ldr r3, [r6, #12]
	ldr r2, .L_0200951c
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02009520
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009524
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02009484:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094ce
	ldr r3, [r7, #12]
	ldr r2, .L_0200951c
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009520
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009528
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_0200952c
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_020094ce:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200950e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200950e
	ldr r3, [r6, #12]
	ldr r2, .L_02009530
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009534
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_0200950e:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200951c:
	.4byte 0x00066640
.L_02009520:
	.4byte 0x0001eb80
.L_02009524:
	.4byte 0xfffd70c0
.L_02009528:
	.4byte 0x00028f40
.L_0200952c:
	.4byte 0xfffff800
.L_02009530:
	.4byte 0x00199900
.L_02009534:
	.4byte 0x001b8480
	.section .text.x02009538,"ax",%progbits
	.global Func_02001538
	.thumb_func
Func_02001538:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #16
	mov r10, r3
	movs r2, #232
	movs r3, #95
	movs r7, #128
	lsls r2, r2, #4
	str r3, [sp, #0]
	lsls r7, r7, #3
	add r2, r10
	add r7, r10
	mov r11, r2
.L_02009564:
	ldr r2, [r7, #20]
	ldr r0, [r7, #24]
	mov r9, r2
	cmp r0, #0
	blt .L_02009632
	cmp r0, r9
	bgt .L_02009632
	mov r1, r9
	lsls r0, r0, #15
	bl Engine_MathDivide
	bl Math_Sine
	movs r3, #232
	movs r2, #236
	lsls r3, r3, #5
	lsls r2, r2, #5
	adds r3, #140
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	mov r8, r0
	mov r1, r9
	adds r0, r2, #0
	muls r0, r3
	add r6, sp, #4
	bl Engine_MathDivide
	ldr r3, [r7, #12]
	adds r5, r5, r0
	mov r2, r8
	muls r2, r3
	adds r3, r2, #0
	adds r5, r5, r3
	str r5, [r6]
	ldr r0, [r7, #24]
	mov r1, r9
	lsls r0, r0, #15
	bl Engine_MathDivide
	bl Math_Sine
	movs r2, #232
	movs r3, #232
	lsls r2, r2, #5
	lsls r3, r3, #5
	adds r2, #132
	adds r3, #144
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	mov r8, r0
	mov r1, r9
	adds r0, r2, #0
	muls r0, r3
	bl Engine_MathDivide
	ldr r3, [r7, #16]
	adds r5, r5, r0
	mov r2, r8
	muls r2, r3
	adds r3, r2, #0
	adds r5, r5, r3
	movs r2, #232
	movs r3, #232
	str r5, [r6, #4]
	lsls r2, r2, #5
	lsls r3, r3, #5
	adds r2, #136
	adds r3, #148
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	adds r0, r2, #0
	muls r0, r3
	mov r1, r9
	bl Engine_MathDivide
	adds r5, r5, r0
	adds r0, r6, #0
	str r5, [r6, #8]
	bl Func_020029b0
	ldr r3, [r6]
	mov r2, r11
	str r3, [r2, #12]
	mov r0, r11
	ldr r3, [r6, #8]
	str r3, [r2, #16]
	bl Func_020029e8
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_02009632:
	ldr r2, [sp, #0]
	movs r3, #40
	subs r2, #1
	add r11, r3
	str r2, [sp, #0]
	adds r7, #28
	cmp r2, #0
	bge .L_02009564
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x02009650,"ax",%progbits
	.global Func_02001650
	.thumb_func
Func_02001650:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #4
	str r3, [sp, #0]
	cmp r0, #0
	ble .L_02009704
	adds r7, r0, #0
.L_02009670:
	ldr r1, [sp, #0]
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #152
	adds r1, r1, r2
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r9, r1
	lsls r6, r3, #3
	subs r6, r6, r3
	ldr r3, [sp, #0]
	lsls r6, r6, #2
	movs r1, #128
	lsls r1, r1, #3
	adds r6, r3, r6
	adds r6, r6, r1
	bl Random16Far
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r2, #192
	movs r3, #128
	lsls r3, r3, #15
	lsls r2, r2, #7
	lsrs r5, r5, #2
	mov r11, r3
	adds r5, r5, r2
	bl Random16Far
	mov r8, r0
	mov r1, r8
	lsls r1, r1, #4
	lsrs r1, r1, #16
	movs r2, #16
	mov r8, r1
	adds r0, r5, #0
	add r8, r2
	bl Math_Cosine
	ldr r3, .L_02009714
	mov r1, r11
	mov r10, r3
	mov lr, r10
	.2byte 0xf800
	asrs r0, r0, #16
	str r0, [r6, #12]
	adds r0, r5, #0
	bl Math_Sine
	mov r1, r11
	mov lr, r10
	.2byte 0xf800
	negs r0, r0
	asrs r0, r0, #16
	mov r1, r8
	movs r3, #0
	str r1, [r6, #20]
	str r3, [r6, #24]
	str r0, [r6, #16]
	mov r2, r9
	ldrh r0, [r2]
	mov r3, r9
	adds r0, #1
	strh r0, [r3]
	lsls r0, r0, #16
	movs r1, #96
	asrs r0, r0, #16
	bl Engine_MathRemainder
	subs r7, #1
	mov r1, r9
	strh r0, [r1]
	cmp r7, #0
	bne .L_02009670
.L_02009704:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009714:
	.4byte IwramMulQ16
	.section .text.x02009718,"ax",%progbits
	.global Func_02001718
	.thumb_func
Func_02001718:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r1, #237
	adds r5, r0, #0
	lsls r1, r1, #5
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlockFar
	ldr r3, .L_02009850
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r7, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #236
	lsls r3, r3, #5
	adds r2, r7, r3
	ldr r3, [r5]
	movs r1, #232
	str r3, [r2]
	lsls r1, r1, #5
	ldr r3, [r5, #4]
	adds r1, #132
	adds r2, r7, r1
	str r3, [r2]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #136
	adds r2, r7, r3
	ldr r3, [r5, #8]
	adds r1, #8
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6]
	adds r1, #8
	str r3, [r2]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #144
	adds r2, r7, r3
	ldr r3, [r6, #4]
	mov r11, r0
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6, #8]
	ldr r0, .L_02009854
	str r3, [r2]
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020027b8
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	adds r2, r7, #0
	adds r5, r0, #0
	bl VramBlock_LoadCached
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #154
	mov r9, r0
	adds r3, r7, r2
	mov r1, r9
	strh r1, [r3]
	adds r2, #2
	movs r1, #128
	adds r3, r7, r2
	lsls r1, r1, #3
	movs r2, #232
	strh r5, [r3]
	adds r1, r1, r7
	movs r3, #0
	lsls r2, r2, #4
	mov r10, r3
	mov r8, r1
	adds r6, r7, r2
.L_020097c8:
	movs r5, #15
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #1
	add r3, r9
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_020029e0
	ldrb r3, [r6, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r6, #5]
	ldrb r3, [r6, #9]
	mov r0, r11
	ands r5, r3
	strb r5, [r6, #9]
	bl Func_020029f0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r6, #9]
	mov r0, r11
	bl Func_020029f8
	movs r3, #1
	mov r2, r8
	negs r3, r3
	subs r0, #1
	strh r0, [r6, #30]
	str r3, [r2, #24]
	movs r3, #1
	add r10, r3
	movs r1, #28
	mov r2, r10
	adds r6, #40
	add r8, r1
	cmp r2, #95
	ble .L_020097c8
	movs r1, #232
	lsls r1, r1, #5
	adds r1, #152
	adds r2, r7, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_02009858
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009850:
	.4byte gPartyState
.L_02009854:
	.4byte 0x000001ef
.L_02009858:
	.4byte Func_02001538
	.section .text.x0200985c,"ax",%progbits
	.global Func_0200185c
	.thumb_func
Func_0200185c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_02009884
	ldr r5, [r3]
	bl Scheduler_RemoveCallbackFar
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #156
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
.L_02009884:
	.4byte Func_02001538
	.section .text.x02009888,"ax",%progbits
	.global Func_02001888
	.thumb_func
Func_02001888:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02009a1c
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02009a20
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02009a24
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02009a28
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009a2c
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_020098da
	b .L_02009a0e
.L_020098da:
	ldr r2, .L_02009a30
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_020098e8
	b .L_020099fe
.L_020098e8:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_020098f0
	b .L_020099fe
.L_020098f0:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_02009a34
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02009966
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_020099fe
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_020099fe
	cmp r4, #239
	bgt .L_020099fe
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009a38
	b .L_020099a2
.L_02009966:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_020099fe
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_020099fe
	cmp r4, #175
	bgt .L_020099fe
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009a3c
.L_020099a2:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009a40
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_020099e0
	adds r0, r5, #0
	bl Func_020029f0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_020099f4
.L_020099e0:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_020099f4:
	adds r0, r6, #0
	mov r1, r11
	bl Func_020027d8
	adds r6, #12
.L_020099fe:
	ldr r3, .L_02009a2c
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02009a0e
	b .L_020098da
.L_02009a0e:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009a1c:
	.4byte 0xffff0000
.L_02009a20:
	.4byte gOverlayArea + 0x3444
.L_02009a24:
	.4byte ResourceTableEntries
.L_02009a28:
	.4byte gOverlayArea + 0x3488
.L_02009a2c:
	.4byte gOverlayArea + 0x3446
.L_02009a30:
	.4byte gOverlayArea + 0x3448
.L_02009a34:
	.4byte gOverlayArea + 0x3548
.L_02009a38:
	.4byte 0x40002000
.L_02009a3c:
	.4byte 0xc000a000
.L_02009a40:
	.4byte gOverlayArea + 0x354a
	.section .text.x02009a44,"ax",%progbits
	.global Func_02001a44
	.thumb_func
Func_02001a44:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02009aa4
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02009aa8
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02009aac
	bl Func_020027b8
	ldr r5, .L_02009ab0
	bl Resource_FindFreeEntry
	movs r1, #192
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009ab4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02009ab8
	ldr r2, .L_02009a9c
	strh r2, [r3]
	ldr r3, .L_02009abc
	strh r2, [r3]
	ldr r2, .L_02009ac0
	ldr r3, .L_02009aa0
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009a9c:
	.4byte 0x00000000
.L_02009aa0:
	.4byte 0xffffffff
.L_02009aa4:
	.4byte IwramClearWords
.L_02009aa8:
	.4byte gOverlayArea + 0x3448
.L_02009aac:
	.4byte Data_02002a48
.L_02009ab0:
	.4byte gOverlayArea + 0x3444
.L_02009ab4:
	.4byte Func_02001888
.L_02009ab8:
	.4byte gOverlayArea + 0x3446
.L_02009abc:
	.4byte gOverlayArea + 0x3548
.L_02009ac0:
	.4byte gOverlayArea + 0x354a
	.section .text.x02009ac4,"ax",%progbits
	.global Func_02001ac4
	.thumb_func
Func_02001ac4:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02009b24
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02009b28
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02009b2c
	bl Func_020027b8
	ldr r5, .L_02009b30
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009b34
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02009b38
	ldr r2, .L_02009b1c
	strh r2, [r3]
	ldr r3, .L_02009b3c
	strh r2, [r3]
	ldr r2, .L_02009b40
	ldr r3, .L_02009b20
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009b1c:
	.4byte 0x00000000
.L_02009b20:
	.4byte 0xffffffff
.L_02009b24:
	.4byte IwramClearWords
.L_02009b28:
	.4byte gOverlayArea + 0x3448
.L_02009b2c:
	.4byte Data_02002baa + 0x1
.L_02009b30:
	.4byte gOverlayArea + 0x3444
.L_02009b34:
	.4byte Func_02001888
.L_02009b38:
	.4byte gOverlayArea + 0x3446
.L_02009b3c:
	.4byte gOverlayArea + 0x3548
.L_02009b40:
	.4byte gOverlayArea + 0x354a
	.section .text.x02009b44,"ax",%progbits
	.global Func_02001b44
	.thumb_func
Func_02001b44:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02009ba8
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02009bac
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02009bb0
	bl Func_020027b8
	ldr r5, .L_02009bb4
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009bb8
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_02009bbc
	ldr r3, .L_02009b9c
	strh r3, [r2]
	ldr r2, .L_02009bc0
	ldr r3, .L_02009ba0
	strh r3, [r2]
	ldr r2, .L_02009bc4
	ldr r3, .L_02009ba4
	strh r3, [r2]
	b .L_02009bc8
.L_02009b9c:
	.4byte 0x00000000
.L_02009ba0:
	.4byte 0x00000001
.L_02009ba4:
	.4byte 0xffffffff
.L_02009ba8:
	.4byte IwramClearWords
.L_02009bac:
	.4byte gOverlayArea + 0x3448
.L_02009bb0:
	.4byte Data_02002dda
.L_02009bb4:
	.4byte gOverlayArea + 0x3444
.L_02009bb8:
	.4byte Func_02001888
.L_02009bbc:
	.4byte gOverlayArea + 0x3446
.L_02009bc0:
	.4byte gOverlayArea + 0x3548
.L_02009bc4:
	.4byte gOverlayArea + 0x354a
.L_02009bc8:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009bcc,"ax",%progbits
	.global Func_02001bcc
	.thumb_func
Func_02001bcc:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	adds r4, r0, #0
	cmp r4, #0
	beq .L_02009bf2
	adds r3, r4, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_02009bf4
	ldr r0, .L_02009bf8
	ldrh r2, [r1]
	movs r5, #0
	ldrsh r3, [r1, r5]
	adds r2, #1
	lsls r3, r3, #2
	str r4, [r0, r3]
	strh r2, [r1]
.L_02009bf2:
	pop {r5, pc}
.L_02009bf4:
	.4byte gOverlayArea + 0x3446
.L_02009bf8:
	.4byte gOverlayArea + 0x3448
	.section .text.x02009bfc,"ax",%progbits
	.global Func_02001bfc
	.thumb_func
Func_02001bfc:
	ldr r3, .L_02009c04
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_02009c04:
	.4byte gOverlayArea + 0x354a
	.section .text.x02009c4e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009c50,"ax",%progbits
	.global Func_02001c50
	.thumb_func
Func_02001c50:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x02009c88,"ax",%progbits
	.global Func_02001c88
	.thumb_func
Func_02001c88:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02009e40
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_02009cd0
	cmp r7, #0
	beq .L_02009cd0
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02009cd8
.L_02009cd0:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02009cd8:
	mov r3, r10
	bl Func_02002828
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02009ce6
	b .L_02009e32
.L_02009ce6:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02002810
	ldr r2, .L_02009e44
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02002820
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02009e48
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_02009e4c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009e32
	cmp r7, #0
	beq .L_02009e32
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02009d68
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02009d68:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009d88
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_02009d88:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02009d9c
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02009d9c:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009de2
	ldr r3, .L_02009e44
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02009dca
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02009ddc
.L_02009dca:
	ldr r2, .L_02009e4c
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02009e4c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02009ddc:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_02009de2:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009dfe
	adds r0, r6, #0
	movs r1, #1
	bl Func_02002810
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02002820
.L_02009dfe:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009e10
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02009e10:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009e22
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02009e22:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009e32
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02009e32:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009e40:
	.4byte gPartyState
.L_02009e44:
	.4byte Data_020033f4
.L_02009e48:
	.4byte Func_02001c50
.L_02009e4c:
	.4byte 0xffff0000
	.section .text.x02009e50,"ax",%progbits
	.global Func_02001e50
	.thumb_func
Func_02001e50:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_02009f68
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_02009f5c
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_02009f6c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_02009e9c
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_02009ea4
.L_02009e9c:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_02009ea4:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_02009f70
	cmp r3, r2
	beq .L_02009f5c
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_02009f5c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_02009f74
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_02009f5c
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_02009f5c
	cmp r2, #239
	bgt .L_02009f5c
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_02009f78
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_020027d8
.L_02009f5c:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009f68:
	.4byte gOverlayArea + 0x354c
.L_02009f6c:
	.4byte gPartyState
.L_02009f70:
	.4byte 0xffff0000
.L_02009f74:
	.4byte ResourceTableEntries
.L_02009f78:
	.4byte 0x80008800
	.section .text.x02009f7c,"ax",%progbits
	.global Func_02001f7c
	.thumb_func
Func_02001f7c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200a1ac
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200a1b0
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200a100
.L_0200a030:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200a0f4
.L_0200a044:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200a0e4
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200a0e4
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200a0e4
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a098
	cmp r5, r10
	bne .L_0200a0d6
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200a0d6
.L_0200a098:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0d6
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002858
.L_0200a0d6:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200a0e4:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200a044
.L_0200a0f4:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200a030
.L_0200a100:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a158
	ldr r3, .L_0200a1b4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200a158
.L_0200a132:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200a148
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200a148
	mov r0, r8
	strh r2, [r0, #12]
.L_0200a148:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200a132
.L_0200a158:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200a166:
	ldr r3, .L_0200a1b8
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200a166
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a1bc
	bl Scheduler_AddOrUpdateCallback
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a1ac:
	.4byte gOverlayArea + 0x354c
.L_0200a1b0:
	.4byte IwramClearWords
.L_0200a1b4:
	.4byte gPartyState
.L_0200a1b8:
	.4byte 0x11111111
.L_0200a1bc:
	.4byte Func_02001e50
	.section .text.x0200a1c0,"ax",%progbits
	.global Func_020021c0
	.thumb_func
Func_020021c0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_0200a240
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_02002970
	ldr r2, .L_0200a244
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a240:
	.4byte gPartyState
.L_0200a244:
	.4byte 0xfff80000
	.section .text.x0200a248,"ax",%progbits
	.global Func_02002248
	.thumb_func
Func_02002248:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200a2b4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_020021c0
	movs r0, #161
	bl Func_02002a40
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002858
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200a2b4:
	.4byte gPartyState
	.section .text.x0200a2b8,"ax",%progbits
	.global Func_020022b8
	.thumb_func
Func_020022b8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200a368
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_020021c0
	movs r0, #229
	bl Func_02002a40
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002858
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200a360
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a364
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200a36c
	.2byte 0x0000
.L_0200a360:
	.4byte 0x00000000
.L_0200a364:
	.4byte 0x00008000
.L_0200a368:
	.4byte gPartyState
.L_0200a36c:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_0200a380:
	cmp r7, #5
	bne .L_0200a38a
	movs r0, #204
	bl Func_02002a40
.L_0200a38a:
	ldr r3, [r6, #24]
	ldr r1, .L_0200a3e8
	ldr r2, .L_0200a3ec
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200a3f0
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200a380
	ldr r3, .L_0200a3f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a3e8:
	.4byte 0xfffffc00
.L_0200a3ec:
	.4byte 0xfffffd00
.L_0200a3f0:
	.4byte 0xffff6667
.L_0200a3f4:
	.4byte gPartyState
	.section .text.x0200a3f8,"ax",%progbits
	.global Func_020023f8
	.thumb_func
Func_020023f8:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200a428
	adds r3, #15
.L_0200a428:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a450,"ax",%progbits
	.global Func_02002450
	.thumb_func
Func_02002450:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a5d4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020028b0
	movs r0, #0
	bl Func_020029a8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02002838
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_02002a40
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200a5d8
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200a4ea:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200a5dc
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200a5e0
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200a5e4
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02001c88
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200a4ea
	movs r0, #188
	bl Func_02002a40
	ldr r5, .L_0200a5d4
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02002958
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002870
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002870
	bl Func_02002878
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002958
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_020028b8
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a5d4:
	.4byte gPartyState
.L_0200a5d8:
	.4byte Func_020023f8
.L_0200a5dc:
	.4byte 0xffffa000
.L_0200a5e0:
	.4byte 0xffffd000
.L_0200a5e4:
	.4byte 0x01090001
	.section .text.x0200a5e8,"ax",%progbits
	.global Func_020025e8
	.thumb_func
Func_020025e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a690
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200a694
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200a684
.L_0200a61c:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200a678
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200a678
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a64c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02002248
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200a684
.L_0200a64c:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200a684
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_020022b8
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200a686
.L_0200a678:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200a61c
.L_0200a684:
	movs r0, #0
.L_0200a686:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a690:
	.4byte gPartyState
.L_0200a694:
	.4byte gOverlayArea + 0x354c
	.section .text.x0200a698,"ax",%progbits
	.global Func_02002698
	.thumb_func
Func_02002698:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200a748
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200a74c
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200a6e6
	cmp r0, #0
	beq .L_0200a73a
.L_0200a6e6:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_02002838
	bl Func_02002450
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200a73a:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a748:
	.4byte gPartyState
.L_0200a74c:
	.4byte gOverlayArea + 0x354c
	.section .rodata.x0200aa48,"a",%progbits
	.global Data_02002a48
Data_02002a48:
	.4byte 0x06345d01
	.4byte Runtime_ReciprocalTable + 0x1259
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte Text_MessageContexts + 0x10b38
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte Data_02001024 + 0xbb
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte Data_0200752c + 0x2d4
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte Resource_DecodeHalfwordLzCode + 0x12
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte Text_MessageContexts + 0x15ce8
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.2byte 0x0002
	.global Data_02002baa
Data_02002baa:
	.2byte 0x0000
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte Tileset_Set112TilesA + 0x21e
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte Battle_PurpleCaveBackdrop + 0x3607
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.2byte 0x0000
	.global Data_02002dda
Data_02002dda:
	.2byte 0x0100
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
.L_0200aebc:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200aef8:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200af34:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02002f70
Data_02002f70:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02002f94
Data_02002f94:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc40
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000008c
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02002fb8
Data_02002fb8:
	.4byte 0x00120011
	.4byte 0xffff0013
	.global Data_02002fc0
Data_02002fc0:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002ff0
Data_02002ff0:
	.4byte 0x00000102
	.4byte 0x00101101
	.4byte 0x00203102
	.4byte 0x00302102
	.4byte 0x00505107
	.4byte 0x00606107
	.4byte 0x00708102
	.4byte 0x00807102
	.4byte 0x00909103
	.4byte 0x00a0a103
	.4byte 0x000001ff
	.global Data_0200301c
Data_0200301c:
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x00026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x01022000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00022000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01022000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003214
Data_02003214:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02003274
Data_02003274:
	.4byte 0x00000000
	.global Data_02003278
Data_02003278:
	.4byte 0x00000001
	.global Data_0200327c
Data_0200327c:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x0a000015
	.4byte Func_020006ac
	.4byte 0x00000002
	.4byte 0x0a000016
	.4byte Func_02000724
	.4byte 0x00000002
	.4byte 0xffff0064
	.4byte Func_020008a4
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002a84
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002a85
	.4byte 0x00000008
	.4byte 0xffffffff
	.4byte Func_02000054
	.4byte 0x00000009
	.4byte 0xffffffff
	.4byte Func_02000064
	.4byte 0x10008c15
	.4byte 0xffff0011
	.4byte Func_02000054
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte Func_02000064
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte Func_02000054
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte Func_02000064
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte Func_02000054
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte Func_02000064
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte Func_02000054
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte Func_02000064
	.4byte 0x50009985
	.4byte 0x0a000000
	.4byte Func_020002b8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020033d8
Data_020033d8:
	.4byte 0xffffffff
	.global Data_020033dc
Data_020033dc:
	.4byte 0x00000001
	.global Data_020033e0
Data_020033e0:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_020033f4
Data_020033f4:
	.4byte .L_0200aebc
	.4byte .L_0200aef8
	.4byte .L_0200af34
