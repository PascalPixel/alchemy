.syntax unified
	.thumb
	.global Func_08043cd8
	.thumb_func
Func_08043cd8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	movs r1, #192
	lsls r1, r1, #4
	mov r8, r0
	adds r1, #236
	movs r0, #220
	sub sp, #40
	bl Runtime_AllocateHeapBlock
	movs r3, #192
	lsls r3, r3, #18
	movs r1, #0
	adds r3, #204
	movs r2, #1
	ldr r7, [r3]
	adds r5, r0, #0
	str r1, [sp, #32]
	str r1, [sp, #28]
	str r1, [sp, #24]
	str r2, [sp, #12]
	bl Func_080ad2c8
	mov r3, r8
	str r0, [sp, #8]
	cmp r3, #0
	bge .L_08043d1e
	movs r1, #0
	mov r8, r1
.L_08043d1e:
	mov r2, r10
	cmp r2, #1
	bne .L_08043d70
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r4, #0
	b .L_08043d5a
.L_08043d36:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #2
	ble .L_08043d44
	movs r1, #0
	mov r8, r1
.L_08043d44:
	adds r4, #1
	cmp r4, #2
	ble .L_08043d4c
	b .L_08043e9c
.L_08043d4c:
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
.L_08043d5a:
	cmp r3, #0
	beq .L_08043d36
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_08043d36
	b .L_08043e9c
.L_08043d70:
	mov r2, r10
	cmp r2, #4
	bne .L_08043dc0
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r4, #0
	b .L_08043daa
.L_08043d88:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #3
	bne .L_08043d96
	movs r1, #0
	mov r8, r1
.L_08043d96:
	adds r4, #1
	cmp r4, #2
	bgt .L_08043e9c
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
.L_08043daa:
	cmp r3, #0
	beq .L_08043d88
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08043d88
	b .L_08043e9c
.L_08043dc0:
	mov r2, r10
	cmp r2, #5
	bne .L_08043e10
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r4, #0
	b .L_08043dfa
.L_08043dd8:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #2
	ble .L_08043de6
	movs r1, #0
	mov r8, r1
.L_08043de6:
	adds r4, #1
	cmp r4, #2
	bgt .L_08043e9c
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
.L_08043dfa:
	cmp r3, #0
	beq .L_08043dd8
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08043dd8
	b .L_08043e9c
.L_08043e10:
	mov r2, r10
	cmp r2, #6
	bne .L_08043e60
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r4, #0
	b .L_08043e4a
.L_08043e28:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #2
	ble .L_08043e36
	movs r1, #0
	mov r8, r1
.L_08043e36:
	adds r4, #1
	cmp r4, #2
	bgt .L_08043e9c
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
.L_08043e4a:
	cmp r3, #0
	beq .L_08043e28
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_08043e28
	b .L_08043e9c
.L_08043e60:
	mov r2, r10
	cmp r2, #0
	beq .L_08043eb2
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	movs r4, #0
	cmp r3, #0
	bne .L_08043e9c
	adds r3, r2, r7
	adds r2, r3, r1
.L_08043e7e:
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r2, #64
	cmp r3, #2
	ble .L_08043e90
	movs r3, #0
	adds r2, r7, r1
	mov r8, r3
.L_08043e90:
	adds r4, #1
	cmp r4, #2
	bgt .L_08043e9c
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08043e7e
.L_08043e9c:
	cmp r4, #3
	bne .L_08043eb2
	movs r5, #2
	negs r5, r5
	b .L_08044330
.L_08043ea6:
	movs r0, #113
	movs r5, #1
	bl Audio_PlayCue
	negs r5, r5
	b .L_080442de
.L_08043eb2:
	add r0, sp, #36
	movs r3, #0
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_0804420c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Func_080439c4
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #2
	movs r2, #28
	movs r0, #1
	movs r3, #7
	bl UiWindow_Create
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #60
	adds r1, r7, r1
	movs r2, #192
	lsls r2, r2, #6
	str r1, [sp, #20]
	movs r4, #0
	adds r2, #88
	mov r9, r0
	mov r11, r4
	adds r6, r7, r2
.L_08043ef2:
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_08043f04
	ldr r0, .L_08044210
	mov r1, r9
	movs r2, #10
	mov r3, r11
	str r4, [sp, #4]
	b .L_08043f7e
.L_08043f04:
	ldrh r3, [r6, #26]
	ldr r1, [sp, #8]
	cmp r3, r1
	bcs .L_08043f18
	ldr r0, .L_08044214
	mov r1, r9
	movs r2, #10
	mov r3, r11
	str r4, [sp, #4]
	b .L_08043f7e
.L_08043f18:
	ldr r2, [r6, #4]
	ldr r3, [r6, #28]
	cmp r2, r3
	beq .L_08043f2c
	ldr r0, .L_08044218
	mov r1, r9
	movs r2, #10
	mov r3, r11
	str r4, [sp, #4]
	b .L_08043f7e
.L_08043f2c:
	mov r2, r10
	cmp r2, #5
	bne .L_08043f46
	movs r3, #21
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bne .L_08043f46
	ldr r0, .L_0804421c
	mov r1, r9
	movs r2, #10
	mov r3, r11
	str r4, [sp, #4]
	b .L_08043f7e
.L_08043f46:
	mov r3, r10
	cmp r3, #6
	bne .L_08043f60
	movs r3, #23
	ldrsb r3, [r6, r3]
	cmp r3, #0
	beq .L_08043f60
	ldr r0, .L_08044220
	mov r1, r9
	movs r2, #10
	mov r3, r11
	str r4, [sp, #4]
	b .L_08043f7e
.L_08043f60:
	ldr r0, [sp, #20]
	mov r5, r11
	adds r0, #16
	mov r1, r9
	movs r2, #12
	adds r3, r5, #0
	str r4, [sp, #4]
	bl UiText_DrawString
	ldr r3, .L_08044224
	ldrh r0, [r6, #2]
	mov r1, r9
	adds r0, r0, r3
	movs r2, #72
	adds r3, r5, #0
.L_08043f7e:
	bl UiText_DrawResource
	ldr r4, [sp, #4]
	ldr r2, [sp, #20]
	movs r1, #16
	adds r2, #64
	adds r4, #1
	add r11, r1
	adds r6, #64
	str r2, [sp, #20]
	cmp r4, #2
	ble .L_08043ef2
	movs r3, #24
	negs r3, r3
	mov r1, r9
	movs r2, #72
	mov r0, r10
	bl Func_08044f88
	movs r5, #144
	str r0, [sp, #16]
	bl Func_080f8070
	movs r4, #0
.L_08043fae:
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #128
	str r4, [sp, #4]
	bl Func_080f8090
	ldr r4, [sp, #4]
	adds r5, #24
	adds r4, #1
	cmp r4, #3
	ble .L_08043fae
	movs r0, #0
	bl Func_080f8080
	movs r0, #1
	bl WaitFrames
	movs r3, #2
	mov r11, r3
.L_08043fd4:
	ldr r1, [sp, #12]
	cmp r1, #0
	bne .L_08043fdc
	b .L_08044142
.L_08043fdc:
	movs r2, #0
	movs r1, #192
	mov r3, r8
	str r2, [sp, #12]
	lsls r1, r1, #6
	lsls r5, r3, #6
	adds r1, #88
	adds r3, r5, r1
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_080440c6
	movs r2, #192
	lsls r2, r2, #6
	adds r2, #112
	adds r3, r5, r2
	ldrb r0, [r7, r3]
	adds r3, r7, r3
	ldrb r1, [r3, #1]
	bl Func_0803f9c0
	ldr r3, [sp, #32]
	cmp r3, #0
	bne .L_0804401c
	mov r1, r11
	str r1, [sp, #0]
	movs r0, #1
	movs r1, #10
	movs r2, #14
	movs r3, #9
	bl UiWindow_Create
	str r0, [sp, #32]
.L_0804401c:
	movs r2, #192
	lsls r2, r2, #6
	adds r3, r7, r5
	adds r2, #60
	adds r6, r3, r2
	ldr r0, [sp, #32]
	adds r1, r6, #0
	bl StatusMenu_DrawCharacterSummary
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #28]
	cmp r3, #0
	bne .L_0804404c
	mov r1, r11
	str r1, [sp, #0]
	movs r0, #16
	movs r1, #10
	movs r2, #13
	movs r3, #3
	bl UiWindow_Create
	str r0, [sp, #28]
.L_0804404c:
	bl Func_08043b34
	movs r1, #0
	ldr r0, [sp, #28]
	movs r2, #0
	adds r3, r6, #0
	bl Func_08043a64
	movs r0, #1
	bl WaitFrames
	movs r2, #192
	lsls r2, r2, #6
	movs r1, #192
	adds r2, #100
	lsls r1, r1, #6
	adds r3, r5, r2
	adds r1, #101
	ldrsb r2, [r7, r3]
	adds r3, r5, r1
	ldrsb r3, [r7, r3]
	adds r1, #1
	adds r2, r2, r3
	adds r3, r5, r1
	ldrsb r3, [r7, r3]
	adds r1, #1
	adds r2, r2, r3
	adds r3, r5, r1
	ldrsb r3, [r7, r3]
	cmn r2, r3
	beq .L_080440b2
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_080440a2
	mov r3, r11
	str r3, [sp, #0]
	movs r0, #16
	movs r1, #14
	movs r2, #13
	movs r3, #5
	bl UiWindow_Create
	str r0, [sp, #24]
.L_080440a2:
	ldr r0, [sp, #24]
	adds r1, r6, #0
	bl UiText_DrawFourNumbersInRow
	movs r0, #1
	bl Func_080f8080
	b .L_0804410a
.L_080440b2:
	movs r0, #0
	bl Func_080f8080
	movs r1, #2
	ldr r0, [sp, #24]
	bl UiWork_Finalize
	movs r1, #0
	str r1, [sp, #24]
	b .L_0804410a
.L_080440c6:
	ldr r2, .L_08044228
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r2, r1
	ldrb r0, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #38
	adds r2, r2, r3
	ldrb r1, [r2]
	bl Func_0803f9c0
	movs r0, #0
	bl Func_080f8080
	bl Func_08043b34
	movs r1, #2
	ldr r0, [sp, #24]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #28]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #32]
	bl UiWork_Finalize
	movs r1, #0
	str r1, [sp, #24]
	str r1, [sp, #28]
	str r1, [sp, #32]
.L_0804410a:
	mov r0, r9
	bl RenderOutput_RedrawSavedRect
	mov r2, r11
	str r2, [sp, #0]
	mov r0, r9
	movs r1, #0
	movs r2, #2
	movs r3, #27
	bl UiWindow_DrawDividerLine
	movs r3, #4
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #0
	movs r2, #4
	movs r3, #27
	bl UiWindow_DrawDividerLine
	mov r3, r8
	lsls r2, r3, #1
	movs r3, #1
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #0
	movs r3, #26
	bl Func_080439e8
.L_08044142:
	ldr r0, [sp, #16]
	bl Func_08045018
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0804422c
	movs r3, #64
	ldr r2, [r1, #12]
	ands r2, r3
	cmp r2, #0
	beq .L_080441f4
	movs r0, #111
	bl Audio_PlayCue
	movs r1, #1
	str r1, [sp, #12]
	b .L_080441e0
.L_08044166:
	movs r1, #192
	mov r3, r8
	lsls r1, r1, #6
	lsls r2, r3, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_080441e0
	mov r3, r10
	cmp r3, #1
	bne .L_0804418e
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080441e0
.L_0804418e:
	mov r3, r10
	cmp r3, #4
	bne .L_080441a8
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #108
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080441e0
.L_080441a8:
	mov r3, r10
	cmp r3, #5
	bne .L_080441c2
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #108
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080441e0
.L_080441c2:
	mov r3, r10
	cmp r3, #6
	beq .L_080441ca
	b .L_08043fd4
.L_080441ca:
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #108
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080441e0
	b .L_08043fd4
.L_080441e0:
	mov r0, r8
	adds r0, #2
	movs r1, #3
	bl Math_Mod
	mov r2, r10
	mov r8, r0
	cmp r2, #0
	bne .L_08044166
	b .L_08043fd4
.L_080441f4:
	ldr r2, [r1, #12]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_080442be
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	str r3, [sp, #12]
	b .L_080442aa
	.2byte 0x0000
.L_0804420c:
	.4byte 0x8500033b
.L_08044210:
	.4byte 0x00000000
.L_08044214:
	.4byte 0x00000001
.L_08044218:
	.4byte 0x00000003
.L_0804421c:
	.4byte 0x00000002
.L_08044220:
	.4byte 0x00000004
.L_08044224:
	.4byte 0x00000e58
.L_08044228:
	.4byte gPartyState
.L_0804422c:
	.4byte gInput
.L_08044230:
	mov r1, r8
	lsls r2, r1, #6
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #88
	adds r3, r2, r1
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_080442aa
	mov r3, r10
	cmp r3, #1
	bne .L_08044258
	adds r1, #20
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080442aa
.L_08044258:
	mov r3, r10
	cmp r3, #4
	bne .L_08044272
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #108
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080442aa
.L_08044272:
	mov r3, r10
	cmp r3, #5
	bne .L_0804428c
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #108
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080442aa
.L_0804428c:
	mov r3, r10
	cmp r3, #6
	beq .L_08044294
	b .L_08043fd4
.L_08044294:
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #108
	adds r3, r2, r1
	adds r3, r7, r3
	ldrb r3, [r3, #3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080442aa
	b .L_08043fd4
.L_080442aa:
	mov r0, r8
	adds r0, #4
	movs r1, #3
	bl Math_Mod
	mov r2, r10
	mov r8, r0
	cmp r2, #0
	bne .L_08044230
	b .L_08043fd4
.L_080442be:
	ldr r3, [r1, #4]
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L_080442ca
	b .L_08043ea6
.L_080442ca:
	ldr r3, [r1, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080442d6
	b .L_08043fd4
.L_080442d6:
	movs r0, #112
	bl Audio_PlayCue
	mov r5, r8
.L_080442de:
	bl Func_080f8078
	bl Func_08043b34
	movs r1, #2
	ldr r0, [sp, #24]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #28]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #32]
	bl UiWork_Finalize
	movs r1, #2
	mov r0, r9
	bl UiWork_Finalize
	bl Func_080439d8
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_08044340
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_0803f9c0
	movs r0, #1
	bl WaitFrames
.L_08044330:
	adds r0, r5, #0
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08044340:
	.4byte gPartyState
