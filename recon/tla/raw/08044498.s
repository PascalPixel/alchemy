.syntax unified
	.thumb
	.global Func_08044498
	.thumb_func
Func_08044498:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #60]
	ldrb r3, [r0]
	sub sp, #8
	movs r1, #0
	cmp r3, #0
	beq .L_080444c0
	movs r3, #244
	lsls r3, r3, #4
	adds r2, r4, r3
.L_080444b0:
	ldrb r3, [r0]
	adds r0, #1
	strh r3, [r2]
	adds r1, #1
	ldrb r3, [r0]
	adds r2, #2
	cmp r3, #0
	bne .L_080444b0
.L_080444c0:
	movs r2, #244
	lsls r3, r1, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldr r2, .L_080444e0
	add r1, sp, #4
	strh r2, [r4, r3]
	movs r0, #0
	mov r2, sp
	movs r3, #0
	bl UiText_MeasureEntryDimensions
	ldr r0, [sp, #4]
	add sp, #8
	b .L_080444e4
	.2byte 0x0000
.L_080444e0:
	.4byte 0x00000000
.L_080444e4:
	pop {pc}
	.2byte 0x0000
