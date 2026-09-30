.syntax unified
	.thumb
	.global UiWork_StepChannelScript
	.thumb_func
UiWork_StepChannelScript:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08016a5c
	ldr r1, .L_08016a60
	ldr r3, [r3]
	adds r6, r0, #0
	mov r8, r3
	movs r0, #131
	ldr r3, [r1]
	ldr r3, .L_08016a64
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrb r3, [r3]
	ldr r2, .L_08016a68
	ldrb r2, [r2, r3]
	sub sp, #52
	ldr r3, .L_08016a6c
	str r2, [sp, #32]
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08016944
	ldr r3, .L_08016a70
	ldrh r3, [r3]
	adds r2, r3, #0
	cmp r2, #0
	bge .L_08016936
	movs r2, #0
.L_08016936:
	cmp r2, #2
	ble .L_0801693c
	movs r2, #2
.L_0801693c:
	lsls r3, r2, #2
	adds r3, r3, r2
	adds r3, #3
	str r3, [sp, #32]
.L_08016944:
	ldrh r3, [r6, #28]
	cmp r3, #0
	beq .L_08016958
	movs r0, #1
	bl UiWork_ShiftPanelRowsLeft
	ldrh r3, [r6, #28]
	subs r3, #1
	strh r3, [r6, #28]
	b .L_08016f18
.L_08016958:
	ldr r3, [r1]
	cmp r3, #0
	bne .L_08016972
	ldrh r2, [r6, #34]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_08016972
	ldr r1, .L_08016a74
	adds r3, r2, r1
	strh r3, [r6, #34]
	b .L_08016f18
.L_0801696e:
	movs r0, #9
	b .L_08016f1a
.L_08016972:
	ldrh r3, [r6, #32]
	movs r7, #0
	cmp r3, #0
	bne .L_08016988
	ldrh r3, [r6, #18]
	movs r2, #235
	lsls r3, r3, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r4, r8
	ldrh r7, [r4, r3]
.L_08016988:
	cmp r7, #30
	bls .L_0801698e
	b .L_08016d76
.L_0801698e:
	ldr r2, .L_08016a78
	lsls r3, r7, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08016998:
	.4byte .L_08016d58
	.4byte .L_08016a80
	.4byte .L_08016c2c
	.4byte .L_08016a14
	.4byte .L_08016c7c
	.4byte .L_08016c58
	.4byte .L_08016c62
	.4byte .L_08016d18
	.4byte .L_08016c90
	.4byte .L_08016cd4
	.4byte .L_08016cfc
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d30
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d64
	.4byte .L_08016d58
.L_08016a14:
	ldrh r3, [r6, #30]
	strh r3, [r6, #4]
	ldr r3, [r6]
	ldrh r2, [r3, #22]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_08016a42
	ldrh r2, [r6, #6]
	ldr r3, .L_08016a7c
	cmp r2, r3
	bls .L_08016a38
	adds r0, r6, #0
	bl UiWork_ResetChannelTransition
	movs r0, #1
	str r0, [sp, #32]
	b .L_08016d64
.L_08016a38:
	movs r1, #208
	lsls r1, r1, #4
	adds r3, r2, r1
	strh r3, [r6, #6]
	b .L_08016d64
.L_08016a42:
	ldrh r3, [r6, #6]
	movs r2, #240
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r6, #16]
	strh r3, [r6, #6]
	adds r3, r2, #0
	cmp r3, #2
	bls .L_08016a56
	b .L_08016d64
.L_08016a56:
	adds r3, r2, #1
	strh r3, [r6, #16]
	b .L_08016d64
.L_08016a5c:
	.4byte Data_03001e8c
.L_08016a60:
	.4byte Data_03001ae8
.L_08016a64:
	.4byte gCell
.L_08016a68:
	.4byte Data_0807380b
.L_08016a6c:
	.4byte 0x00000ea5
.L_08016a70:
	.4byte gLagFramesShown
.L_08016a74:
	.4byte 0x0000ffff
.L_08016a78:
	.4byte .L_08016998
.L_08016a7c:
	.4byte 0x00000cff
.L_08016a80:
	ldr r3, .L_08016ad0
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08016a9a
	movs r4, #225
	ldrh r3, [r6, #20]
	lsls r4, r4, #2
	cmp r3, r4
	bcs .L_08016a9a
	ldr r2, .L_08016ad4
	movs r3, #0
	str r3, [r2]
.L_08016a9a:
	ldr r3, .L_08016ad8
	adds r0, r6, #0
	ldr r7, .L_08016acc
	strh r3, [r6, #20]
	bl UiWork_CheckCancelByModeInput
	cmp r0, #0
	bne .L_08016afa
	ldr r0, [r6]
	ldrh r3, [r0, #8]
	cmp r3, #0
	bne .L_08016ab4
	b .L_08016d64
.L_08016ab4:
	ldrh r3, [r0, #10]
	cmp r3, #0
	bne .L_08016abc
	b .L_08016d64
.L_08016abc:
	ldr r7, .L_08016adc
	add r7, r8
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_08016ac8
	b .L_08016d64
.L_08016ac8:
	b .L_08016ae0
	.2byte 0x0000
.L_08016acc:
	.4byte 0x00000000
.L_08016ad0:
	.4byte 0x00000ea4
.L_08016ad4:
	.4byte gKeysPressedLatch
.L_08016ad8:
	.4byte 0x00000397
.L_08016adc:
	.4byte 0x000012f8
.L_08016ae0:
	ldrh r2, [r0, #8]
	ldrh r3, [r0, #10]
	lsls r2, r2, #2
	lsls r3, r3, #3
	movs r5, #1
	subs r2, #8
	subs r3, #16
	movs r1, #1
	str r5, [sp, #0]
	bl UiText_DrawGlyph
	strb r5, [r7]
	b .L_08016d64
.L_08016afa:
	ldr r5, [r6]
	ldrh r3, [r5, #12]
	ldrh r0, [r5, #8]
	str r3, [sp, #48]
	ldrh r3, [r5, #14]
	str r0, [sp, #28]
	str r3, [sp, #44]
	ldr r3, .L_08016cb8
	ldrh r1, [r5, #10]
	ldrh r4, [r6, #18]
	add r3, r8
	str r1, [sp, #24]
	adds r0, r5, #0
	strb r7, [r3]
	str r4, [sp, #12]
	bl RenderOutput_PrepareForRedraw
	ldrh r3, [r6, #36]
	ldr r4, [sp, #12]
	cmp r3, #0
	bne .L_08016b3c
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	orrs r3, r2
	cmp r3, #0
	beq .L_08016b3c
	ldrh r0, [r5, #12]
	ldrh r1, [r5, #14]
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	bl UiWindow_EraseBorderRect
	ldr r4, [sp, #12]
.L_08016b3c:
	ldr r3, .L_08016cbc
	adds r4, #1
	ands r4, r3
	movs r2, #235
	lsls r3, r4, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r0, r8
	ldrh r3, [r0, r3]
	cmp r3, #0
	beq .L_08016c12
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	orrs r3, r2
	cmp r3, #0
	beq .L_08016c12
	ldrh r7, [r6, #36]
	cmp r7, #0
	beq .L_08016b70
	ldrh r0, [r5, #12]
	ldrh r1, [r5, #14]
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	bl UiWindow_EraseBorderRect
	b .L_08016c06
.L_08016b70:
	add r1, sp, #48
	mov r11, r1
	movs r0, #36
	movs r1, #8
	add r0, sp
	adds r1, r1, r6
	mov r2, sp
	mov r3, sp
	adds r3, #40
	adds r2, #44
	str r0, [sp, #0]
	str r1, [sp, #4]
	mov r9, r0
	mov r10, r1
	adds r0, r4, #0
	mov r1, r11
	str r3, [sp, #16]
	str r4, [sp, #12]
	str r2, [sp, #20]
	str r7, [sp, #8]
	bl UiWindow_FitOnScreen
	ldrh r1, [r5, #22]
	movs r3, #128
	ands r3, r1
	ldr r4, [sp, #12]
	cmp r3, #0
	beq .L_08016bc0
	ldr r2, [sp, #36]
	ldr r3, [sp, #24]
	cmp r3, r2
	beq .L_08016bb8
	subs r2, r2, r3
	ldr r3, [sp, #44]
	subs r3, r3, r2
	str r3, [sp, #44]
.L_08016bb8:
	ldr r3, [sp, #44]
	cmp r3, #0
	bge .L_08016bc0
	str r7, [sp, #44]
.L_08016bc0:
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r1
	cmp r3, #0
	bne .L_08016bf6
	ldr r3, [sp, #40]
	ldr r0, [sp, #28]
	subs r3, r0, r3
	cmp r3, #0
	bge .L_08016bd6
	adds r3, #3
.L_08016bd6:
	ldr r2, [sp, #48]
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r1, r9
	movs r3, #2
	str r2, [sp, #48]
	mov r2, r10
	str r1, [sp, #0]
	str r2, [sp, #4]
	str r3, [sp, #8]
	adds r0, r4, #0
	mov r1, r11
	ldr r2, [sp, #20]
	ldr r3, [sp, #16]
	bl UiWindow_FitOnScreen
.L_08016bf6:
	ldr r3, [sp, #48]
	strh r3, [r5, #12]
	ldr r3, [sp, #44]
	strh r3, [r5, #14]
	ldr r3, [sp, #40]
	strh r3, [r5, #8]
	ldr r3, [sp, #36]
	strh r3, [r5, #10]
.L_08016c06:
	ldrh r0, [r5, #12]
	ldrh r1, [r5, #14]
	ldrh r2, [r5, #8]
	ldrh r3, [r5, #10]
	bl UiWindow_DrawFrame
.L_08016c12:
	ldrh r3, [r6, #30]
	movs r2, #0
	ldr r5, .L_08016cc0
	strh r3, [r6, #4]
	strh r2, [r6, #6]
	strh r2, [r6, #16]
	add r5, r8
	ldrh r0, [r5]
	bl Resource_ResetEntry
	movs r3, #99
	strh r3, [r5]
	b .L_08016d64
.L_08016c2c:
	ldr r3, .L_08016cc4
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08016c46
	movs r4, #225
	ldrh r3, [r6, #20]
	lsls r4, r4, #2
	cmp r3, r4
	bcs .L_08016c46
	ldr r2, .L_08016cc8
	movs r3, #0
	str r3, [r2]
.L_08016c46:
	adds r0, r6, #0
	bl UiWork_CheckCancelByModeInput
	cmp r0, #0
	beq .L_08016c52
	b .L_0801696e
.L_08016c52:
	ldr r3, .L_08016ccc
	strh r3, [r6, #20]
	b .L_08016d64
.L_08016c58:
	ldrh r3, [r6, #20]
	cmp r3, #0
	bne .L_08016c6c
	movs r3, #20
	b .L_08016c6a
.L_08016c62:
	ldrh r3, [r6, #20]
	cmp r3, #0
	bne .L_08016c6c
	movs r3, #120
.L_08016c6a:
	strh r3, [r6, #20]
.L_08016c6c:
	ldr r2, .L_08016cd0
	movs r3, #0
	add r2, r8
	strh r3, [r2]
	adds r0, r6, #0
	bl UiWork_CheckCancelByInput
	b .L_08016d64
.L_08016c7c:
	ldrh r3, [r6, #20]
	cmp r3, #0
	bne .L_08016c86
	movs r3, #60
	strh r3, [r6, #20]
.L_08016c86:
	ldr r2, .L_08016cd0
	movs r3, #0
	add r2, r8
	strh r3, [r2]
	b .L_08016d64
.L_08016c90:
	ldrh r3, [r6, #18]
	ldr r2, .L_08016cb4
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
	ldrh r3, [r6, #18]
	movs r0, #235
	lsls r0, r0, #4
	lsls r3, r3, #1
	adds r3, r3, r0
	mov r1, r8
	ldrh r3, [r1, r3]
	adds r0, r6, #0
	strh r3, [r6, #22]
	bl UiWork_CopyParamsToRenderWork
	b .L_08016d64
	.2byte 0x0000
.L_08016cb4:
	.4byte 0x000001ff
.L_08016cb8:
	.4byte 0x000012f8
.L_08016cbc:
	.4byte 0x000001ff
.L_08016cc0:
	.4byte 0x000012b6
.L_08016cc4:
	.4byte 0x00000ea4
.L_08016cc8:
	.4byte gKeysPressedLatch
.L_08016ccc:
	.4byte 0x00000397
.L_08016cd0:
	.4byte 0x000012f6
.L_08016cd4:
	ldrh r3, [r6, #18]
	ldr r2, .L_08016cf8
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
	ldrh r3, [r6, #18]
	movs r2, #235
	lsls r3, r3, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r4, r8
	ldrh r3, [r4, r3]
	adds r0, r6, #0
	strh r3, [r6, #24]
	bl UiWork_CopyParamsToRenderWork
	b .L_08016d64
	.2byte 0x0000
.L_08016cf8:
	.4byte 0x000001ff
.L_08016cfc:
	ldrh r3, [r6, #18]
	ldr r2, .L_08016d2c
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
	ldrh r3, [r6, #18]
	movs r0, #235
	lsls r0, r0, #4
	lsls r3, r3, #1
	adds r3, r3, r0
	mov r1, r8
	ldrh r3, [r1, r3]
	adds r0, r6, #0
	b .L_08016d24
.L_08016d18:
	movs r3, #0
	movs r2, #15
	strh r3, [r6, #24]
	adds r0, r6, #0
	movs r3, #10
	strh r2, [r6, #22]
.L_08016d24:
	strh r3, [r6, #26]
	bl UiWork_CopyParamsToRenderWork
	b .L_08016d64
.L_08016d2c:
	.4byte 0x000001ff
.L_08016d30:
	ldrh r3, [r6, #18]
	ldr r0, .L_08016d60
	adds r3, #1
	ands r3, r0
	strh r3, [r6, #18]
	ldrh r2, [r6, #18]
	movs r4, #235
	lsls r3, r2, #1
	lsls r4, r4, #4
	adds r3, r3, r4
	mov r4, r8
	ldrh r3, [r4, r3]
	ldr r1, [r6]
	adds r2, #1
	strh r3, [r1, #18]
	ands r2, r0
	movs r3, #10
	strh r3, [r6, #20]
	strh r2, [r6, #18]
	b .L_08016d64
.L_08016d58:
	movs r3, #1
	strh r3, [r6, #32]
	movs r0, #8
	b .L_08016f1a
.L_08016d60:
	.4byte 0x000001ff
.L_08016d64:
	ldr r3, .L_08016dd4
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08016d70
	b .L_08016ede
.L_08016d70:
	movs r0, #1
	str r0, [sp, #32]
	b .L_08016ede
.L_08016d76:
	ldrh r3, [r6, #4]
	adds r2, r3, #0
	adds r2, #128
	cmp r2, #0
	bge .L_08016d84
	ldr r1, .L_08016dd8
	adds r2, r3, r1
.L_08016d84:
	asrs r5, r2, #8
	ldrh r2, [r6, #6]
	adds r3, r2, #0
	adds r3, #128
	cmp r3, #0
	bge .L_08016d94
	ldr r4, .L_08016dd8
	adds r3, r2, r4
.L_08016d94:
	asrs r3, r3, #8
	mov r12, r3
	movs r0, #131
	ldr r3, .L_08016ddc
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrb r3, [r3]
	ldr r2, .L_08016de0
	ldrb r2, [r2, r3]
	ldr r3, .L_08016de4
	add r3, r8
	ldrb r3, [r3]
	mov r10, r2
	ldrh r2, [r6, #18]
	cmp r3, #0
	beq .L_08016db6
	adds r5, #8
.L_08016db6:
	adds r3, r2, #1
	ldr r2, .L_08016de8
	movs r1, #235
	ands r3, r2
	lsls r3, r3, #1
	lsls r1, r1, #4
	adds r3, r3, r1
	mov r0, r8
	ldrh r4, [r0, r3]
	cmp r4, #222
	bne .L_08016dec
	movs r3, #128
	lsls r3, r3, #7
	b .L_08016df4
	.2byte 0x0000
.L_08016dd4:
	.4byte 0x00000ea5
.L_08016dd8:
	.4byte 0x0000017f
.L_08016ddc:
	.4byte gCell
.L_08016de0:
	.4byte Data_0807380e
.L_08016de4:
	.4byte 0x00000ea4
.L_08016de8:
	.4byte 0x000001ff
.L_08016dec:
	cmp r4, #223
	bne .L_08016dfe
	movs r3, #128
	lsls r3, r3, #8
.L_08016df4:
	orrs r7, r3
	ldrh r3, [r6, #18]
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
.L_08016dfe:
	ldr r0, [r6]
	ldrh r2, [r0, #22]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	bne .L_08016e3e
	cmp r7, #32
	bls .L_08016e3e
	cmp r4, #32
	bls .L_08016e3e
	adds r3, r7, #0
	adds r2, r4, #0
	ldr r1, .L_08016e70
	subs r3, #32
	subs r2, #32
	lsls r3, r3, #5
	lsls r2, r2, #5
	ldrh r3, [r1, r3]
	ldrh r2, [r1, r2]
	movs r1, #240
	adds r3, r3, r2
	lsls r3, r3, #16
	lsls r1, r1, #12
	cmp r3, r1
	bhi .L_08016e3e
	lsls r3, r4, #8
	orrs r7, r3
	ldrh r3, [r6, #18]
	ldr r2, .L_08016e6c
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
.L_08016e3e:
	movs r3, #0
	str r3, [sp, #0]
	adds r2, r5, #0
	mov r3, r12
	adds r1, r7, #0
	bl UiText_DrawGlyph
	ldr r3, .L_08016e74
	adds r4, r0, #0
	movs r0, #131
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r2, .L_08016e78
	ldrb r3, [r3]
	ldrb r3, [r2, r3]
	strh r3, [r6, #34]
	cmp r4, #0
	beq .L_08016ecc
	ldr r1, .L_08016e7c
	add r1, r8
	ldrh r3, [r1]
	b .L_08016e80
	.2byte 0x0000
.L_08016e6c:
	.4byte 0x000001ff
.L_08016e70:
	.4byte UiText_Glyphs
.L_08016e74:
	.4byte gCell
.L_08016e78:
	.4byte Data_08073808
.L_08016e7c:
	.4byte 0x000012f4
.L_08016e80:
	cmp r3, #0
	beq .L_08016eb6
	ldr r5, .L_08016eac
	add r5, r8
	ldrh r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_08016eb0
	cmp r7, #32
	beq .L_08016eb6
	ldrh r0, [r1]
	movs r3, #3
	ands r3, r7
	adds r0, r0, r3
	str r4, [sp, #12]
	bl Func_080f9010
	mov r1, r10
	strh r1, [r5]
	ldr r4, [sp, #12]
	b .L_08016eb6
	.2byte 0x0000
.L_08016eac:
	.4byte 0x000012f6
.L_08016eb0:
	ldr r0, .L_08016f10
	adds r3, r2, r0
	strh r3, [r5]
.L_08016eb6:
	lsls r0, r4, #8
	cmp r7, #32
	bne .L_08016ec6
	ldrh r3, [r6, #16]
	lsls r3, r3, #1
	adds r3, #8
	ldrh r3, [r6, r3]
	adds r0, r0, r3
.L_08016ec6:
	ldrh r3, [r6, #4]
	adds r3, r3, r0
	strh r3, [r6, #4]
.L_08016ecc:
	cmp r7, #32
	bne .L_08016ede
	ldr r3, .L_08016f14
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_08016ede
	movs r1, #1
	str r1, [sp, #32]
.L_08016ede:
	ldrh r2, [r6, #20]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_08016ef2
	ldr r4, .L_08016f10
	adds r3, r2, r4
	strh r3, [r6, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_08016efc
.L_08016ef2:
	ldrh r3, [r6, #18]
	ldr r2, .L_08016f0c
	adds r3, #1
	ands r3, r2
	strh r3, [r6, #18]
.L_08016efc:
	ldr r0, [sp, #32]
	subs r0, #1
	str r0, [sp, #32]
	cmp r0, #0
	beq .L_08016f08
	b .L_08016972
.L_08016f08:
	b .L_08016f18
	.2byte 0x0000
.L_08016f0c:
	.4byte 0x000001ff
.L_08016f10:
	.4byte 0x0000ffff
.L_08016f14:
	.4byte 0x00000ea5
.L_08016f18:
	movs r0, #0
.L_08016f1a:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
