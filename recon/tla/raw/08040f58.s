.syntax unified
	.thumb
	.global Func_08040f58
	.thumb_func
Func_08040f58:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #76
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #225
	bl GameFlag_SetBit
	movs r0, #133
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	ldr r0, .L_08040f8c
	movs r1, #30
	bl Event_SetPairWork1c0Far
	pop {pc}
.L_08040f8c:
	.4byte 0x000000f4
