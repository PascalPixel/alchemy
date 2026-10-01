.syntax unified
	.thumb
	.global Func_08013d84
	.thumb_func
Func_08013d84:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r6, .L_08013dd8
	mov r7, sp
	ldrh r5, [r6]
	cmp r5, #0
	beq .L_08013dce
	ldr r3, .L_08013ddc
	movs r1, #0
	adds r2, r3, #1
	adds r3, #4
	lsrs r3, r3, #2
	mov r10, r1
	lsls r3, r3, #2
	mov r1, sp
	subs r1, r1, r3
	movs r4, #132
	movs r3, #128
	lsls r4, r4, #24
	lsrs r2, r2, #2
	lsls r3, r3, #19
	mov r8, sp
	adds r3, #212
	mov sp, r1
	ldr r0, .L_08013de0
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	adds r1, r5, #0
	mov lr, sp
	.2byte 0xf800
	mov r2, r10
	strh r2, [r6]
	mov sp, r8
.L_08013dce:
	mov sp, r7
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08013dd8:
	.4byte gIoWriteQueue
.L_08013ddc:
	.4byte 0x0000006b
.L_08013de0:
	.4byte Resource_ExecuteTransferCode
