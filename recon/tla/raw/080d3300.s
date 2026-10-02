.syntax unified
	.thumb
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	push {r5, lr}
	adds r5, r1, #0
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080d3324
	cmp r5, #0
	ble .L_080d3324
	cmp r5, #3
	ble .L_080d3316
	movs r5, #3
.L_080d3316:
	movs r1, #3
	ldr r3, .L_080d3328
	subs r1, r1, r5
	lsls r1, r1, #7
	adds r1, r1, r3
	bl ObjectDispatch_InitializeFar
.L_080d3324:
	pop {r5, pc}
	.2byte 0x0000
.L_080d3328:
	.4byte Data_080f06d8
