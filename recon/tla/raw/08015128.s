.syntax unified
	.thumb
	.global SceneTransform_ApplyPosition
	.thumb_func
SceneTransform_ApplyPosition:
	push {r5, r6, lr}
	sub sp, #48
	adds r6, r0, #0
	mov r5, sp
	adds r0, r5, #0
	movs r1, #128
	lsls r1, r1, #9
	movs r2, #0
	movs r3, #0
	movs r4, #0
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	ldr r3, [r6]
	adds r0, r5, #0
	str r3, [r5, #36]
	ldr r3, [r6, #4]
	str r3, [r5, #40]
	ldr r3, [r6, #8]
	str r3, [r5, #44]
	ldr r3, .L_0801515c
	mov lr, r3
	.2byte 0xf800
	add sp, #48
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0801515c:
	.4byte IwramTransformMatrix
