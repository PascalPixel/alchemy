.syntax unified
	.thumb
	.global Map_CopyCellsRect
	.thumb_func
Map_CopyCellsRect:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	ldr r5, [sp, #36]
	ldr r6, [sp, #40]
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r9, r2
	mov r11, r3
	adds r2, r5, #0
	adds r3, r6, #0
	mov r8, r0
	mov r10, r1
	bl Map_CopyMetatileIndicesRect
	mov r0, r8
	mov r1, r10
	mov r2, r9
	mov r3, r11
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Map_CopyCellAttributeRect
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
