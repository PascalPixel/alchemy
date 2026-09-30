.syntax unified
	.thumb
	.global Func_080ceafc
	.thumb_func
Func_080ceafc:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r2, #0
	adds r0, r1, #0
	cmp r5, #0
	beq .L_080ceb50
	ldr r3, [r5, #8]
	cmp r3, #0
	beq .L_080ceb34
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	bge .L_080ceb2e
	bl Func_080d22a8
	ldr r0, [r5, #8]
	bl Func_080d3be8
	adds r0, r6, #0
	movs r1, #0
	bl Func_080d407c
	bl Func_080d2350
	b .L_080ceb34
.L_080ceb2e:
	adds r1, r6, #0
	mov lr, r3
	.2byte 0xf800
.L_080ceb34:
	movs r0, #161
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ceb50
	bl Func_080d22a8
	ldr r0, .L_080ceb54
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	bl Func_080d2350
.L_080ceb50:
	movs r0, #0
	pop {r5, r6, pc}
.L_080ceb54:
	.4byte 0x00000dc3
