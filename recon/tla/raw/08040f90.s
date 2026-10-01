.syntax unified
	.thumb
	.global Func_08040f90
	.thumb_func
Func_08040f90:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #36
	bl GameFlag_SetBit
	movs r0, #190
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #197
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl GameFlag_SetBit
	movs r0, #156
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #193
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #194
	bl GameFlag_SetBit
	ldr r0, .L_08040fe0
	movs r1, #1
	bl Event_SetPairWork1c0Far
	pop {pc}
	.2byte 0x0000
.L_08040fe0:
	.4byte 0x0000010e
