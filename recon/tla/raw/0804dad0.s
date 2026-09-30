.syntax unified
	.thumb
	.global Func_0804dad0
	.thumb_func
Func_0804dad0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	adds r5, r1, #0
	lsls r5, r5, #16
	asrs r5, r5, #16
	adds r6, r0, #0
	movs r3, #0
	ldrsh r1, [r2, r3]
	adds r0, r5, #0
	sub sp, #4
	mov r9, r2
	bl Func_080c8648
	ldr r3, .L_0804db68
	mov r10, r0
	adds r0, r6, #0
	add r10, r3
	bl RenderOutput_PrepareForRedraw
	movs r2, #14
	str r2, [sp, #0]
	mov r8, r2
	adds r0, r5, #0
	adds r2, r6, #0
	movs r3, #0
	movs r1, #3
	bl UiText_DrawNumber
	mov r2, r9
	movs r3, #0
	ldrsh r0, [r2, r3]
	mov r3, r8
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r1, #3
	movs r3, #82
	bl UiText_DrawNumber
	ldr r2, .L_0804db6c
	adds r1, r6, #0
	mov r8, r2
	mov r0, r8
	movs r2, #74
	movs r3, #0
	bl UiText_DrawString
	ldr r3, .L_0804db70
	adds r1, r6, #0
	adds r5, r5, r3
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawResource
	mov r0, r8
	adds r1, r6, #0
	movs r2, #74
	movs r3, #14
	bl UiText_DrawString
	mov r0, r10
	adds r1, r6, #0
	movs r2, #82
	movs r3, #0
	bl UiText_DrawResource
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0804db68:
	.4byte 0x00000e58
.L_0804db6c:
	.4byte Data_0805f8d8
.L_0804db70:
	.4byte 0x00000eba
