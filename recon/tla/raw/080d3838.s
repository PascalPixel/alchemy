.syntax unified
	.thumb
	.global Func_080d3838
	.thumb_func
Func_080d3838:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r2, #0
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080d3858
	adds r3, r0, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_080d385c
	bl Object_SetCallback
	adds r0, r6, #0
	bl Battle_WaitMode0
.L_080d3858:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080d385c:
	.4byte Data_080f3300
