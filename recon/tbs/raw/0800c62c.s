@ Uncredited camera suffix, re-derived from the current owned TBS
@ English ROM and current physical source names. The two camera C drafts
@ omit unread frame reservations and retain their measured two-byte gaps.
	.syntax unified
	.thumb
	.text
	.balign 4
	.global ObjectSystem_UpdateCamera
	.type ObjectSystem_UpdateCamera, %function
	.thumb_func
ObjectSystem_UpdateCamera:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .LCamera0
	ldr r0, [r6]
	sub sp, #80
	str r0, [sp, #12]
	adds r2, r0, #0
	adds r2, #228
	ldr r1, [r2]
	ldr r3, .LCamera1
	ands r1, r3
	str r1, [sp, #8]
	ldr r2, [r2, #4]
	ands r2, r3
	str r2, [sp, #4]
	adds r3, r6, #0
	subs r3, #8
	ldr r3, [r3]
	str r3, [sp, #0]
	ldr r5, .LCamera2
	movs r0, #52
	adds r1, r5, #0
	bl Runtime_AllocateHeapBlock
	movs r2, #132
	lsrs r5, r5, #2
	lsls r2, r2, #24
	adds r1, r0, #0
	ldr r3, .LCamera3
	ldr r0, .LCamera4
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #0
	subs r6, #12
	ldr r6, [r6]
	ldr r2, [sp, #0]
	mov r10, r6
	strh r3, [r2]
	movs r4, #63
	ldr r3, .LCamera5
	movs r0, #84
	add r0, r10
	mov r7, r10
	str r4, [sp, #16]
	mov r11, r3
	mov r8, r0
	adds r7, #8
.LCamera27:
	mov r1, r10
	ldr r3, [r1]
	cmp r3, #0
	bne .LCamera6
	b .LCamera7
.LCamera6:
	ldr r1, [r7]
	cmp r1, #0
	bne .LCamera8
	ldr r3, [r7, #8]
	cmp r3, #0
	bne .LCamera8
	b .LCamera9
.LCamera8:
	mov r2, r8
	ldrb r3, [r2]
	movs r6, #15
	ands r6, r3
	cmp r6, #0
	bne .LCamera10
	b .LCamera7
.LCamera10:
	cmp r6, #1
	beq .LCamera11
	b .LCamera7
.LCamera11:
	ldr r0, [sp, #0]
	movs r4, #4
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .LCamera12
	ldrb r3, [r2, #8]
	cmp r3, #0
	bne .LCamera12
	ldr r5, [r7, #72]
	ldrb r0, [r5, #28]
	adds r5, #37
	bl Resource_ActivateEntry
	strb r6, [r5]
	b .LCamera7
.LCamera12:
	ldr r3, [sp, #4]
	ldr r0, [r7, #8]
	ldr r2, [sp, #8]
	subs r6, r0, r3
	ldr r3, [r7, #4]
	subs r2, r1, r2
	mov r9, r2
	subs r2, r6, r3
	ldr r3, .LCamera13
	ldr r4, .LCamera14
	add r3, r9
	ldr r5, [r7, #72]
	cmp r3, r4
	bls .LCamera15
	b .LCamera16
.LCamera15:
	ldr r3, .LCamera17
	cmp r2, r3
	ble .LCamera16
	ldr r4, .LCamera18
	cmp r2, r4
	bgt .LCamera16
	ldrb r3, [r7, #26]
	movs r2, #34
	add r2, r10
	mov r12, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	movs r3, #152
	lsls r3, r3, #1
	lsls r2, r2, #4
	adds r2, r2, r3
	ldr r4, [sp, #12]
	asrs r3, r0, #20
	lsls r3, r3, #7
	asrs r1, r1, #20
	adds r1, r1, r3
	ldr r3, [r4, r2]
	lsls r1, r1, #2
	ldrb r2, [r7, #27]
	adds r1, r3, r1
	movs r0, #35
	movs r3, #1
	add r0, r10
	ands r3, r2
	mov lr, r0
	cmp r3, #0
	beq .LCamera19
	ldr r4, [r1]
	lsls r3, r4, #16
	lsrs r0, r3, #30
	cmp r0, #0
	beq .LCamera20
	movs r2, #13
	ldrb r1, [r5, #9]
	negs r2, r2
	adds r3, r2, #0
	lsls r0, r0, #2
	ands r3, r1
	orrs r3, r0
	strb r3, [r5, #9]
	ldrb r3, [r5, #21]
	ands r2, r3
	orrs r2, r0
	strb r2, [r5, #21]
	b .LCamera20
.LCamera19:
	ldr r4, [r1]
.LCamera20:
	lsls r3, r4, #18
	lsrs r1, r3, #30
	cmp r1, #0
	beq .LCamera21
	adds r3, r1, #0
	adds r3, #255
	mov r1, r12
	strb r3, [r1]
.LCamera21:
	ldr r0, [r7, #16]
	ldr r1, [r5, #24]
	mov r12, pc
	bx r11
	str r0, [sp, #20]
	ldr r0, [r7, #20]
	ldr r1, [r5, #24]
	movs r0, r0
	mov r12, pc
	bx r11
	movs r2, #20
	add r1, sp, #28
	add r2, sp
	mov r3, r9
	str r0, [r2, #4]
	str r3, [r1]
	mov r12, r2
	ldr r2, [r7, #4]
	str r6, [r1, #8]
	str r2, [r1, #4]
	ldr r4, [r7, #12]
	str r4, [r1, #12]
	mov r3, lr
	ldrb r0, [r3]
	movs r3, #2
	ands r3, r0
	cmp r3, #0
	beq .LCamera22
	ldr r3, .LCamera23
	adds r2, r2, r3
	str r2, [r1, #4]
	adds r2, r6, r3
	adds r3, r4, r3
	str r2, [r1, #8]
	str r3, [r1, #12]
	mov r4, lr
	ldrb r0, [r4]
.LCamera22:
	movs r3, #4
	ands r3, r0
	cmp r3, #0
	beq .LCamera24
	ldr r3, [r1, #4]
	movs r2, #160
	lsls r2, r2, #17
	adds r3, r3, r2
	str r3, [r1, #4]
	ldr r3, [r1, #8]
	adds r3, r3, r2
	str r3, [r1, #8]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
.LCamera24:
	mov r0, r10
	ldrh r3, [r0, #6]
	mov r2, r12
	adds r0, r5, #0
	bl Render_ApplyProjectedPlacement
	b .LCamera7
.LCamera16:
	mov r1, r8
	ldrb r3, [r1, #8]
	cmp r3, #0
	bne .LCamera7
	ldrb r2, [r5, #29]
	movs r6, #1
	b .LCamera25
.LCamera9:
	mov r2, r8
	ldrb r3, [r2]
	movs r6, #15
	ands r6, r3
	cmp r6, #1
	bne .LCamera7
	ldrb r3, [r2, #8]
	ldr r5, [r7, #72]
	cmp r3, #0
	bne .LCamera7
	ldrb r2, [r5, #29]
.LCamera25:
	adds r3, r6, #0
	ands r3, r2
	cmp r3, #0
	bne .LCamera7
	ldrb r0, [r5, #28]
	bl Resource_ActivateEntry
	adds r3, r5, #0
	adds r3, #37
	strb r6, [r3]
.LCamera7:
	ldr r3, [sp, #16]
	movs r4, #112
	subs r3, #1
	str r3, [sp, #16]
	add r8, r4
	adds r7, #112
	add r10, r4
	cmp r3, #0
	blt .LCamera26
	b .LCamera27
.LCamera26:
	movs r0, #52
	bl Runtime_ReleaseHeapBlock
	add sp, #80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.balign 4, 0
.LCamera0:
	.4byte gObjectSlots + 0x0000000c
.LCamera1:
	.4byte 0xffff0000
.LCamera2:
	.4byte Render_DecodeFrameCodeSize
.LCamera3:
	.4byte 0x040000d4 @ DMA3 source/destination/control register block
.LCamera4:
	.4byte Render_DecodeFrame
.LCamera5:
	.4byte IwramMulQ16ReturnIp
.LCamera13:
	.4byte 0x001fffff
.LCamera14:
	.4byte 0x012ffffe
.LCamera17:
	.4byte 0xffe00000
.LCamera18:
	.4byte 0x00dfffff
.LCamera23:
	.4byte 0xfec00000
	.size ObjectSystem_UpdateCamera, .-ObjectSystem_UpdateCamera
	.global ObjectCamera_ReturnTrue
	.type ObjectCamera_ReturnTrue, %function
	.thumb_func
ObjectCamera_ReturnTrue:
	movs r0, #1
	bx lr
	.size ObjectCamera_ReturnTrue, .-ObjectCamera_ReturnTrue
	.global ObjectSystem_UpdateCameraFixed
	.type ObjectSystem_UpdateCameraFixed, %function
	.thumb_func
ObjectSystem_UpdateCameraFixed:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .LCamera28
	ldr r7, [r3]
	subs r3, #24
	ldr r3, [r3]
	sub sp, #52
	mov r8, r3
	ldr r5, .LCamera29
	movs r0, #52
	adds r1, r5, #0
	bl Runtime_AllocateHeapBlock
	movs r2, #132
	lsrs r5, r5, #2
	lsls r2, r2, #24
	adds r1, r0, #0
	ldr r3, .LCamera30
	ldr r0, .LCamera31
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, r7, #0
	ldr r3, [r5, #24]
	adds r6, r5, #0
	adds r6, #12
	cmp r3, #0
	beq .LCamera32
	adds r5, r3, #0
.LCamera32:
	ldr r3, [r7, #28]
	cmp r3, #0
	beq .LCamera33
	adds r6, r3, #0
.LCamera33:
	ldr r3, [r6]
	ldr r0, [r5]
	ldr r1, [r5, #8]
	subs r0, r0, r3
	ldr r3, [r6, #8]
	subs r1, r1, r3
	asrs r1, r1, #16
	asrs r0, r0, #16
	bl ArcTan2
	movs r3, #0
	mov r2, r8
	lsls r0, r0, #16
	asrs r0, r0, #16
	strh r3, [r2]
	mov r9, r0
	bl Render_ResetTransformState
	ldr r0, .LCamera34
	bl GameFlag_TestFar
	cmp r0, #0
	beq .LCamera35
	ldr r3, .LCamera36
	ldr r0, .LCamera37
	add r9, r3
	ldr r3, .LCamera38
	bl _call_via_r3
	adds r0, r5, #0
	adds r1, r6, #0
	bl Graphics_PrepareTransferAndRun
	b .LCamera39
.LCamera35:
	adds r0, r5, #0
	adds r1, r6, #0
	bl Graphics_PrepareTransferInIwramWork
.LCamera39:
	ldr r3, .LCamera40
	ldr r3, [r3]
	ldr r2, .LCamera41
	mov r8, r3
	add r8, r2
	movs r3, #128
	movs r2, #63
	lsls r3, r3, #9
	mov r7, r8
	str r2, [sp, #8]
	mov r11, r3
	adds r7, #24
.LCamera49:
	mov r2, r8
	ldr r3, [r2]
	cmp r3, #0
	beq .LCamera42
	mov r3, r8
	adds r3, #84
	ldrb r3, [r3]
	movs r2, #15
	ands r2, r3
	cmp r2, #1
	beq .LCamera43
	cmp r2, #1
	ble .LCamera42
	cmp r2, #2
	beq .LCamera44
	b .LCamera42
.LCamera43:
	movs r3, #8
	add r3, r8
	mov r10, r3
	ldr r3, [r7]
	str r3, [sp, #20]
	ldr r3, [r7, #4]
	add r6, sp, #20
	str r3, [r6, #4]
	ldr r0, .LCamera34
	ldr r5, [r7, #56]
	bl GameFlag_TestFar
	cmp r0, #0
	beq .LCamera45
	mov r2, r11
	str r2, [sp, #20]
	str r2, [r6, #4]
.LCamera45:
	mov r2, r8
	ldrh r3, [r2, #6]
	ldrb r2, [r7, #10]
	add r3, r9
	str r2, [sp, #0]
	adds r0, r5, #0
	mov r1, r10
	adds r2, r6, #0
	bl Render_PlaceProjectedSprite
	b .LCamera42
.LCamera44:
	ldr r3, [r7]
	str r3, [sp, #12]
	ldr r3, [r7, #4]
	add r4, sp, #12
	str r3, [r4, #4]
	ldr r0, .LCamera34
	str r4, [sp, #4]
	bl GameFlag_TestFar
	ldr r4, [sp, #4]
	cmp r0, #0
	beq .LCamera46
	mov r3, r11
	str r3, [sp, #12]
	str r3, [r4, #4]
.LCamera46:
	movs r2, #3
	ldr r6, [r7, #56]
	mov r10, r2
.LCamera48:
	ldmia r6!, {r5}
	cmp r5, #0
	beq .LCamera47
	mov r2, r8
	ldrh r3, [r2, #6]
	ldrb r2, [r7, #10]
	mov r1, r8
	str r2, [sp, #0]
	add r3, r9
	adds r2, r4, #0
	adds r0, r5, #0
	adds r1, #8
	str r4, [sp, #4]
	bl Render_PlaceProjectedSprite
	ldr r4, [sp, #4]
.LCamera47:
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r2, r10
	cmp r2, #0
	bge .LCamera48
.LCamera42:
	ldr r3, [sp, #8]
	movs r2, #112
	subs r3, #1
	negs r2, r2
	str r3, [sp, #8]
	subs r7, #112
	add r8, r2
	cmp r3, #0
	bge .LCamera49
	movs r0, #52
	bl Runtime_ReleaseHeapBlock
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.balign 4, 0
.LCamera28:
	.4byte gObjectSlots + 0x0000001c
.LCamera29:
	.4byte Render_DecodeFrameCodeSize
.LCamera30:
	.4byte 0x040000d4 @ DMA3 source/destination/control register block
.LCamera31:
	.4byte Render_DecodeFrame
.LCamera34:
	.4byte 0x0000016b
.LCamera36:
	.4byte 0xffffe000
.LCamera37:
	.4byte Camera_FixedViewMatrix
.LCamera38:
	.4byte IwramTransformMatrix
.LCamera40:
	.4byte gObjectSlots
.LCamera41:
	.4byte 0x00001b90
	.size ObjectSystem_UpdateCameraFixed, .-ObjectSystem_UpdateCameraFixed
