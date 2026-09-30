.syntax unified
	.thumb
	.global UiText_DrawString
	.thumb_func
UiText_DrawString:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	mov r8, r2
	mov r10, r3
	adds r7, r1, #0
	bl Runtime_BumpAllocateAlternatePool
	ldrb r3, [r5]
	adds r6, r0, #0
	adds r2, r6, #0
	cmp r3, #0
	beq .L_080420d8
.L_080420ca:
	ldrb r3, [r5]
	adds r5, #1
	strh r3, [r2]
	adds r2, #2
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_080420ca
.L_080420d8:
	ldr r3, .L_080420f0
	adds r0, r6, #0
	strh r3, [r2]
	adds r1, r7, #0
	mov r2, r8
	mov r3, r10
	bl UiText_RenderWideStringAtOffset
	adds r0, r6, #0
	bl Sys_Free
	b .L_080420f4
.L_080420f0:
	.4byte 0x00000000
.L_080420f4:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
