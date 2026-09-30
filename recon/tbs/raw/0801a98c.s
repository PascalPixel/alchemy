.syntax unified
	.thumb
	.global MenuSelection_DrawFrame
	.thumb_func
MenuSelection_DrawFrame:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0801aa20
	ldr r3, [r3]
	movs r0, #192
	mov r10, r3
	lsls r0, r0, #2
	movs r3, #0
	sub sp, #24
	add r0, r10
	mov r11, r3
	movs r3, #210
	str r0, [sp, #8]
	lsls r3, r3, #2
	movs r1, #182
	movs r2, #195
	add r3, r10
	lsls r1, r1, #2
	lsls r2, r2, #2
	ldr r7, [r3]
	add r1, r10
	add r2, r10
	mov r8, r1
	mov r9, r2
	cmp r7, #0
	bne .L_0801a9cc
	b .L_0801ab1c
.L_0801a9cc:
	adds r6, r7, #0
	adds r6, #40
	movs r4, #4
	ldrb r3, [r6, #5]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	movs r5, #63
	strb r3, [r6, #5]
	negs r5, r5
	ldrb r3, [r6, #7]
	adds r2, r5, #0
	ands r3, r2
	strb r3, [r6, #7]
	ldrh r1, [r7, #16]
	ldr r3, .L_0801aa18
	ldr r2, .L_0801aa1c
	ands r1, r3
	ldrh r3, [r6, #6]
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #6]
	ldrh r3, [r7, #18]
	movs r0, #240
	strb r3, [r6, #4]
	adds r1, r3, #0
	movs r3, #232
	str r0, [sp, #4]
	lsls r3, r3, #2
	add r3, r10
	ldrh r2, [r3]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0801aa24
	adds r3, r2, r1
	strb r3, [r6, #4]
	b .L_0801aac0
	.2byte 0x0000
.L_0801aa18:
	.4byte 0x000001ff
.L_0801aa1c:
	.4byte 0xfffffe00
.L_0801aa20:
	.4byte Data_03001e98
.L_0801aa24:
	movs r1, #16
	ldrsh r2, [r7, r1]
	movs r3, #24
	ldrsh r1, [r7, r3]
	ldrh r5, [r7, #16]
	ldrh r4, [r7, #24]
	cmp r2, r1
	beq .L_0801aa78
	ldrh r0, [r7, #20]
	mov r12, r0
	movs r0, #20
	ldrsh r3, [r7, r0]
	cmp r3, #0
	ble .L_0801aa4c
	adds r3, r2, r3
	cmp r3, r1
	bgt .L_0801aa52
	mov r1, r12
	adds r3, r5, r1
	b .L_0801aa5a
.L_0801aa4c:
	adds r3, r2, r3
	cmp r3, r1
	bge .L_0801aa56
.L_0801aa52:
	strh r4, [r7, #16]
	b .L_0801aa5c
.L_0801aa56:
	mov r2, r12
	adds r3, r5, r2
.L_0801aa5a:
	strh r3, [r7, #16]
.L_0801aa5c:
	ldrh r1, [r7, #16]
	ldr r3, .L_0801aa70
	ldr r2, .L_0801aa74
	ands r1, r3
	ldrh r3, [r6, #6]
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #6]
	b .L_0801aac0
	.2byte 0x0000
.L_0801aa70:
	.4byte 0x000001ff
.L_0801aa74:
	.4byte 0xfffffe00
.L_0801aa78:
	ldr r3, .L_0801ad50
	add r3, r10
	ldrh r3, [r3]
	cmp r11, r3
	bne .L_0801aac0
	movs r3, #241
	str r3, [sp, #4]
	mov r4, r8
	ldrh r3, [r4, #10]
	cmp r3, #0
	beq .L_0801aac0
	add r5, sp, #12
	adds r1, r5, #0
	ldrh r0, [r7, #8]
	bl BattleMotion_ProjectScaledPositionFar
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_0801aac0
	ldr r2, [r5]
	mov r3, r8
	strh r2, [r3, #24]
	mov r4, r8
	ldr r1, [r5, #4]
	movs r5, #34
	ldrsh r3, [r4, r5]
	strh r1, [r4, #26]
	cmp r3, #0
	bne .L_0801aac0
	mov r0, r8
	strh r2, [r0, #16]
	movs r3, #1
	mov r2, r8
	strh r1, [r2, #18]
	strh r3, [r4, #34]
.L_0801aac0:
	movs r5, #34
	ldrsh r3, [r7, r5]
	cmp r3, #0
	beq .L_0801ab10
	ldr r0, .L_0801ad54
	bl Func_080770c0
	cmp r0, #0
	beq .L_0801ab08
	ldr r3, .L_0801ad58
	add r3, r10
	ldrh r3, [r3]
	cmp r3, #1
	bne .L_0801aaea
	ldrb r3, [r6, #5]
	movs r0, #13
	negs r0, r0
	ands r3, r0
	movs r2, #4
	orrs r3, r2
	b .L_0801aaf2
.L_0801aaea:
	ldrb r3, [r6, #5]
	movs r1, #13
	negs r1, r1
	ands r3, r1
.L_0801aaf2:
	strb r3, [r6, #5]
	ldrh r3, [r7, #10]
	cmp r3, #1
	bne .L_0801ab08
	ldrb r3, [r6, #5]
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #5]
.L_0801ab08:
	adds r0, r6, #0
	ldr r1, [sp, #4]
	bl Runtime_PushSlotEntry
.L_0801ab10:
	ldr r7, [r7, #4]
	movs r3, #1
	add r11, r3
	cmp r7, #0
	beq .L_0801ab1c
	b .L_0801a9cc
.L_0801ab1c:
	mov r4, r9
	ldrh r3, [r4, #10]
	cmp r3, #0
	bne .L_0801ab26
	b .L_0801ac36
.L_0801ab26:
	mov r0, r10
	bl NodeChain_GetNodeAtCount
	mov r5, r9
	adds r5, #40
	movs r6, #13
	ldrb r3, [r5, #5]
	negs r6, r6
	adds r2, r6, #0
	ands r2, r3
	movs r3, #4
	ldrb r1, [r5, #7]
	negs r3, r3
	ands r2, r3
	subs r3, #59
	ands r3, r1
	movs r1, #17
	negs r1, r1
	ands r2, r1
	movs r1, #32
	adds r7, r0, #0
	orrs r2, r1
	movs r0, #63
	ands r2, r0
	ands r3, r0
	movs r1, #128
	strb r2, [r5, #5]
	orrs r3, r1
	ldrb r2, [r5, #9]
	strb r3, [r5, #7]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r5, #9]
	mov r0, r9
	ldrh r3, [r0, #14]
	ldr r2, .L_0801ad5c
	ldrh r1, [r5, #8]
	ands r2, r3
	ldr r3, .L_0801ad60
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	ldrh r2, [r7, #16]
	ldr r1, .L_0801ad64
	subs r2, #4
	ands r2, r1
	ldr r3, .L_0801ad68
	ldrh r1, [r5, #6]
	mov r11, r3
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r3, .L_0801ad6c
	ldr r3, [r3]
	ldr r0, .L_0801ad70
	movs r2, #15
	lsrs r3, r3, #1
	ands r3, r2
	ldrb r3, [r0, r3]
	ldrb r1, [r7, #18]
	lsls r3, r3, #24
	asrs r3, r3, #25
	adds r1, r1, r3
	subs r1, #4
	strb r1, [r5, #4]
	mov r4, r9
	movs r0, #34
	ldrsh r2, [r4, r0]
	movs r0, #38
	ldrsh r3, [r4, r0]
	ldrh r1, [r4, #34]
	cmp r2, r3
	beq .L_0801ac18
	movs r0, #208
	lsls r0, r0, #2
	add r0, r10
	strh r1, [r0]
	ldr r3, .L_0801ad74
	ldrh r2, [r4, #34]
	add r3, r10
	strh r2, [r3]
	movs r2, #209
	lsls r2, r2, #2
	add r2, r10
	movs r3, #0
	strh r3, [r2]
	bl AffineMatrix_BuildForEffect
	movs r3, #31
	ldrb r2, [r5, #7]
	ands r0, r3
	movs r3, #63
	negs r3, r3
	lsls r0, r0, #1
	ands r3, r2
	orrs r3, r0
	strb r3, [r5, #7]
	ldrb r3, [r5, #5]
	ldrh r1, [r5, #6]
	movs r2, #3
	orrs r3, r2
	strb r3, [r5, #5]
	ldr r2, .L_0801ad78
	lsls r3, r1, #23
	lsrs r3, r3, #23
	ldr r4, .L_0801ad64
	adds r3, r3, r2
	mov r2, r11
	ands r3, r4
	ands r2, r1
	orrs r2, r3
	ldrb r3, [r5, #4]
	adds r3, #240
	strh r2, [r5, #6]
	strb r3, [r5, #4]
	mov r0, r9
	ldrh r3, [r0, #34]
	ldrh r2, [r0, #36]
	mov r1, r9
	adds r3, r3, r2
	strh r3, [r1, #34]
.L_0801ac18:
	ldr r0, .L_0801ad54
	bl Func_080770c0
	cmp r0, #0
	beq .L_0801ac2e
	ldrb r3, [r5, #5]
	adds r2, r6, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r5, #5]
.L_0801ac2e:
	adds r0, r5, #0
	movs r1, #248
	bl Runtime_PushSlotEntry
.L_0801ac36:
	mov r0, r10
	movs r1, #0
	bl MenuSelection_DrawSideMarker
	mov r0, r10
	movs r1, #1
	bl MenuSelection_DrawSideMarker
	movs r3, #211
	lsls r3, r3, #2
	add r3, r10
	ldr r7, [r3]
	cmp r7, #0
	bne .L_0801ac54
	b .L_0801adaa
.L_0801ac54:
	movs r2, #208
	lsls r2, r2, #2
	movs r3, #13
	add r2, r10
	negs r3, r3
	mov r9, r2
	mov r11, r3
.L_0801ac62:
	movs r4, #16
	ldrsh r2, [r7, r4]
	movs r5, #24
	ldrsh r3, [r7, r5]
	adds r6, r7, #0
	adds r6, #40
	ldrh r1, [r7, #16]
	cmp r2, r3
	beq .L_0801ac7a
	ldrh r3, [r7, #20]
	adds r3, r1, r3
	strh r3, [r7, #16]
.L_0801ac7a:
	movs r0, #18
	ldrsh r2, [r7, r0]
	movs r4, #26
	ldrsh r3, [r7, r4]
	ldrh r1, [r7, #18]
	cmp r2, r3
	beq .L_0801ac8e
	ldrh r3, [r7, #22]
	adds r3, r1, r3
	strh r3, [r7, #18]
.L_0801ac8e:
	ldr r4, .L_0801ad64
	ldrh r3, [r7, #16]
	ldr r5, .L_0801ad68
	ldrh r1, [r6, #6]
	adds r2, r4, #0
	ands r2, r3
	adds r3, r5, #0
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #6]
	ldrh r3, [r7, #18]
	strb r3, [r6, #4]
	movs r3, #34
	ldrsh r2, [r7, r3]
	mov lr, r2
	movs r3, #38
	ldrsh r2, [r7, r3]
	mov r12, r2
	movs r0, #0
	ldrh r1, [r7, #34]
	cmp lr, r12
	beq .L_0801ad16
	ldrh r3, [r7, #36]
	adds r3, r1, r3
	mov r1, r9
	strh r3, [r7, #34]
	strh r3, [r1]
	ldr r3, .L_0801ad74
	ldrh r2, [r7, #34]
	add r3, r10
	strh r2, [r3]
	movs r3, #209
	lsls r3, r3, #2
	add r3, r10
	strh r0, [r3]
	mov r0, r9
	str r4, [sp, #0]
	bl AffineMatrix_BuildForEffect
	movs r3, #31
	movs r1, #63
	ands r0, r3
	negs r1, r1
	ldrb r3, [r6, #7]
	adds r2, r1, #0
	ands r3, r2
	lsls r0, r0, #1
	orrs r3, r0
	strb r3, [r6, #7]
	ldrb r3, [r6, #5]
	ldrh r1, [r6, #6]
	movs r2, #3
	orrs r3, r2
	strb r3, [r6, #5]
	lsls r2, r1, #23
	ldr r3, .L_0801ad7c
	lsrs r2, r2, #23
	ldr r4, [sp, #0]
	adds r2, r2, r3
	adds r3, r5, #0
	ands r2, r4
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #6]
	ldrb r3, [r6, #4]
	adds r3, #248
	strb r3, [r6, #4]
	b .L_0801ad2e
.L_0801ad16:
	movs r4, #4
	ldrb r3, [r6, #5]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	movs r5, #63
	strb r3, [r6, #5]
	negs r5, r5
	ldrb r3, [r6, #7]
	adds r2, r5, #0
	ands r3, r2
	strb r3, [r6, #7]
.L_0801ad2e:
	ldr r0, .L_0801ad54
	bl Func_080770c0
	cmp r0, #0
	beq .L_0801ad9a
	ldr r3, .L_0801ad58
	add r3, r10
	ldrh r3, [r3]
	cmp r3, #1
	bne .L_0801ad80
	ldrb r3, [r6, #5]
	mov r0, r11
	ands r3, r0
	movs r2, #4
	orrs r3, r2
	b .L_0801ad86
	.2byte 0x0000
.L_0801ad50:
	.4byte 0x0000039e
.L_0801ad54:
	.4byte 0x00000103
.L_0801ad58:
	.4byte 0x000002e2
.L_0801ad5c:
	.4byte 0x000003ff
.L_0801ad60:
	.4byte 0xfffffc00
.L_0801ad64:
	.4byte 0x000001ff
.L_0801ad68:
	.4byte 0xfffffe00
.L_0801ad6c:
	.4byte gFrameTick
.L_0801ad70:
	.4byte Data_08036740
.L_0801ad74:
	.4byte 0x00000342
.L_0801ad78:
	.4byte 0x0000fff0
.L_0801ad7c:
	.4byte 0x0000fff8
.L_0801ad80:
	ldrb r3, [r6, #5]
	mov r1, r11
	ands r3, r1
.L_0801ad86:
	strb r3, [r6, #5]
	ldrh r3, [r7, #10]
	cmp r3, #1
	bne .L_0801ad9a
	ldrb r3, [r6, #5]
	mov r2, r11
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #5]
.L_0801ad9a:
	adds r0, r6, #0
	movs r1, #240
	bl Runtime_PushSlotEntry
	ldr r7, [r7, #4]
	cmp r7, #0
	beq .L_0801adaa
	b .L_0801ac62
.L_0801adaa:
	mov r4, r8
	ldrh r3, [r4, #10]
	cmp r3, #0
	bne .L_0801adb4
	b .L_0801aecc
.L_0801adb4:
	ldr r3, .L_0801ae04
	ldr r2, [r3]
	ldr r5, .L_0801ae08
	movs r3, #15
	lsrs r2, r2, #2
	ands r2, r3
	mov r11, r5
	lsls r2, r2, #8
	movs r1, #128
	add r2, r11
	ldrh r0, [r4, #12]
	lsls r1, r1, #1
	bl VramBlock_LoadCached
	ldr r3, .L_0801ae00
	ldr r1, [sp, #8]
	ands r0, r3
	ldrh r2, [r1, #8]
	ldr r3, .L_0801ae0c
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #8]
	mov r3, r8
	ldrh r0, [r3, #24]
	movs r4, #24
	ldrsh r2, [r3, r4]
	ldrh r1, [r3, #16]
	movs r5, #16
	ldrsh r3, [r3, r5]
	cmp r2, r3
	beq .L_0801ae1c
	subs r3, r2, r3
	asrs r3, r3, #1
	cmp r3, #0
	beq .L_0801ae18
	b .L_0801ae10
	.2byte 0x0000
.L_0801ae00:
	.4byte 0x000003ff
.L_0801ae04:
	.4byte gFrameTick
.L_0801ae08:
	.4byte Data_080346f8
.L_0801ae0c:
	.4byte 0xfffffc00
.L_0801ae10:
	adds r3, r1, r3
	mov r0, r8
	strh r3, [r0, #16]
	b .L_0801ae1c
.L_0801ae18:
	mov r1, r8
	strh r0, [r1, #16]
.L_0801ae1c:
	mov r2, r8
	mov r5, r8
	ldrh r1, [r2, #26]
	movs r3, #26
	ldrsh r2, [r2, r3]
	movs r4, #18
	ldrsh r3, [r5, r4]
	adds r0, r3, #0
	cmp r2, r3
	beq .L_0801ae48
	subs r3, r2, r3
	asrs r3, r3, #1
	cmp r3, #0
	beq .L_0801ae42
	adds r3, r0, r3
	mov r0, r8
	strh r3, [r0, #18]
	adds r0, r3, #0
	b .L_0801ae48
.L_0801ae42:
	mov r2, r8
	strh r1, [r2, #18]
	adds r0, r1, #0
.L_0801ae48:
	ldr r3, .L_0801ae90
	ldr r3, [r3]
	ldr r1, .L_0801ae94
	movs r2, #15
	lsrs r3, r3, #2
	ands r3, r2
	ldrb r3, [r1, r3]
	ldr r4, [sp, #8]
	adds r3, r3, r0
	subs r3, #32
	strb r3, [r4, #4]
	mov r5, r8
	ldrh r2, [r5, #16]
	ldr r3, .L_0801ae8c
	subs r2, #4
	ands r2, r3
	ldrh r1, [r4, #6]
	ldr r3, .L_0801ae98
	ldr r0, [sp, #8]
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r0, .L_0801ae9c
	bl Func_080770c0
	cmp r0, #0
	beq .L_0801aec4
	ldr r3, .L_0801aea0
	add r3, r10
	ldrh r3, [r3]
	cmp r3, #1
	bne .L_0801aeb6
	b .L_0801aea4
	.2byte 0x0000
.L_0801ae8c:
	.4byte 0x000001ff
.L_0801ae90:
	.4byte gFrameTick
.L_0801ae94:
	.4byte Data_08036740
.L_0801ae98:
	.4byte 0xfffffe00
.L_0801ae9c:
	.4byte 0x00000103
.L_0801aea0:
	.4byte 0x000002e2
.L_0801aea4:
	ldr r1, [sp, #8]
	movs r2, #13
	ldrb r3, [r1, #5]
	negs r2, r2
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r1, #5]
	b .L_0801aec4
.L_0801aeb6:
	ldr r3, [sp, #8]
	ldrb r2, [r3, #5]
	movs r3, #13
	negs r3, r3
	ldr r4, [sp, #8]
	ands r3, r2
	strb r3, [r4, #5]
.L_0801aec4:
	ldr r0, [sp, #8]
	movs r1, #248
	bl Runtime_PushSlotEntry
.L_0801aecc:
	ldr r2, .L_0801aee8
	add r2, r10
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0801aee8:
	.4byte 0x000003a2
