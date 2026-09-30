.syntax unified
	.thumb
	.global Func_08044460
	.thumb_func
Func_08044460:
	push {r5, lr}
	bl Resource_FindFreeEntry
	ldr r2, .L_08044474
	movs r1, #128
	adds r5, r0, #0
	bl VramBlock_LoadResourceFar
	adds r0, r5, #0
	pop {r5, pc}
.L_08044474:
	.4byte 0x000001fa
