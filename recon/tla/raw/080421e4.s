.syntax unified
	.thumb
	.global UiText_DrawNumber
	.thumb_func
UiText_DrawNumber:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #16
	ldr r7, [sp, #36]
	adds r4, r0, #0
	adds r5, r1, #0
	mov r8, r3
	adds r1, r4, #0
	adds r6, r2, #0
	mov r0, sp
	adds r2, r5, #0
	bl UiText_FormatNumber
	adds r1, r6, #0
	mov r2, r8
	adds r3, r7, #0
	bl UiText_DrawString
	add sp, #16
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
