.syntax unified
	.thumb
	.global Func_0801591c
	.thumb_func
Func_0801591c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	mov r8, r0
	mov r10, r1
	ldr r5, .L_08015964
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08015968
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r8
	mov r1, r10
	mov lr, r6
	.2byte 0xf800
	adds r5, r0, #0
	adds r0, r6, #0
	bl Sys_Free
	adds r0, r5, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08015964:
	.4byte 0x0000025c
.L_08015968:
	.4byte Resource_DecodeByteLzCode
