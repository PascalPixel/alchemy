.syntax unified
	.thumb
	.global Func_08157cf4
	.thumb_func
Func_08157cf4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r1
	adds r7, r2, #0
	adds r5, r3, #0
	bl Resource_GetTableEntry
	adds r6, r0, #0
	cmp r5, #0
	beq .L_08157d18
	movs r0, #160
	ldr r3, .L_08157d2c
	lsls r0, r0, #19
	adds r1, r6, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08157d18:
	cmp r7, #0
	beq .L_08157d1e
	adds r6, #128
.L_08157d1e:
	adds r0, r6, #0
	mov r1, r8
	bl Func_0801587c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_08157d2c:
	.4byte IwramCopyWords
