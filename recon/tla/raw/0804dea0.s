.syntax unified
	.thumb
	.global Menu_DrawFlagBitTable
	.thumb_func
Menu_DrawFlagBitTable:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r0
	sub sp, #36
	adds r5, r1, #0
	bl RenderOutput_PrepareForRedraw
	mov r1, r10
	movs r2, #48
	movs r3, #0
	ldr r0, .L_0804df5c
	bl UiText_DrawStringInWindow
	add r2, sp, #8
	mov r8, r2
	mov r2, sp
	adds r2, #33
	movs r1, #0
	movs r3, #28
	str r2, [sp, #0]
	str r1, [sp, #4]
	add r3, sp
	movs r1, #16
	lsls r7, r5, #8
	mov r11, r3
	mov r9, r1
.L_0804dede:
	mov r3, r11
.L_0804dee0:
	movs r1, #0
	strb r1, [r3]
	ldr r2, [sp, #0]
	adds r3, #1
	cmp r3, r2
	bne .L_0804dee0
	adds r0, r7, #0
	movs r1, #3
	mov r2, r11
	bl Text_FormatHex
	mov r0, r11
	mov r1, r10
	movs r2, #0
	mov r3, r9
	bl UiText_DrawStringInWindow
	ldr r0, .L_0804df60
	mov r1, r10
	movs r2, #32
	mov r3, r9
	bl UiText_DrawStringInWindow
	mov r6, r8
	mov r5, r8
	adds r6, #15
.L_0804df14:
	adds r0, r7, #0
	bl GameFlag_Test
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	adds r3, #48
	strb r3, [r5]
	adds r5, #1
	adds r7, #1
	cmp r5, r6
	ble .L_0804df14
	movs r3, #16
	movs r2, #0
	mov r1, r8
	strb r2, [r1, r3]
	mov r0, r8
	mov r3, r9
	mov r1, r10
	movs r2, #48
	bl UiText_DrawStringInWindow
	ldr r1, [sp, #4]
	movs r3, #8
	adds r1, #1
	add r9, r3
	str r1, [sp, #4]
	cmp r1, #16
	bne .L_0804dede
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804df5c:
	.4byte Menu_HexDigitsString
.L_0804df60:
	.4byte Menu_ColonString
