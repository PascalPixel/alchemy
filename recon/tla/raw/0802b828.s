.syntax unified
	.thumb
	.global Func_0802b828
	.thumb_func
Func_0802b828:
	push {r5, lr}
	ldr r3, .L_0802b864
	movs r1, #253
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, .L_0802b868
	lsls r3, r3, #2
	ldrh r0, [r3, r2]
	ldr r3, .L_0802b86c
	adds r0, r0, r3
	bl Resource_GetTableEntry
	adds r5, r0, #0
	ldr r3, [r5, #44]
	ldr r1, .L_0802b870
	adds r0, r5, r3
	bl Resource_DecodeType01
	bl Func_0802a5e4
	ldr r3, [r5, #48]
	ldr r1, .L_0802b874
	adds r0, r5, r3
	bl Resource_DecodeType01
	pop {r5, pc}
.L_0802b864:
	.4byte gPartyState
.L_0802b868:
	.4byte Data_0802f380
.L_0802b86c:
	.4byte 0x0000026c
.L_0802b870:
	.4byte gMapCellBuffer
.L_0802b874:
	.4byte Data_02024000
