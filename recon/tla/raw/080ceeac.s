.syntax unified
	.thumb
	.global Func_080ceeac
	.thumb_func
Func_080ceeac:
	push {r5, lr}
	movs r5, #0
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080ceec2
	ldr r0, [r0, #80]
	cmp r0, #0
	beq .L_080ceec2
	ldrb r3, [r0, #5]
	lsrs r5, r3, #6
.L_080ceec2:
	adds r0, r5, #0
	pop {r5, pc}
	.2byte 0x0000
