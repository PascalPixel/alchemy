.syntax unified
	.thumb
	.global Func_081b2008
	.thumb_func
Func_081b2008:
	push {lr}
	ldr r3, .L_081b202c
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_081b2030
	ldr r2, .L_081b2034
	ldr r3, [r3, #4]
	movs r0, #227
	lsls r0, r0, #1
	str r3, [r2]
	adds r0, #255
	bl Audio_PlayCue
	bl Func_081b34a8
	movs r0, #0
	b .L_081b2038
.L_081b202c:
	.4byte 0x00000040
.L_081b2030:
	.4byte gPartyState
.L_081b2034:
	.4byte gRandomState
.L_081b2038:
	pop {pc}
	.2byte 0x0000
