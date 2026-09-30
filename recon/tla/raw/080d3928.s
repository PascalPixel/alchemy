.syntax unified
	.thumb
	.global Func_080d3928
	.thumb_func
Func_080d3928:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	ldr r3, [r0, #80]
	cmp r0, #0
	beq .L_080d393c
	cmp r3, #0
	beq .L_080d393c
	strh r5, [r3, #18]
.L_080d393c:
	pop {r5, pc}
	.2byte 0x0000
