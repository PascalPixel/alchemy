.syntax unified
	.thumb
	.global Func_08015b24
	.thumb_func
Func_08015b24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r3, [r3]
	mov r10, r0
	mov r8, r3
	movs r3, #196
	lsls r3, r3, #6
	adds r3, #60
	add r3, r8
	ldr r5, [r3]
	cmp r5, #0
	beq .L_08015b4e
	movs r0, #0
	mov lr, r5
	.2byte 0xf800
.L_08015b4e:
	ldr r2, .L_08015c04
	mov r7, r8
	mov r9, r2
	mov r2, r10
	lsls r3, r2, #16
	lsrs r6, r3, #16
	mov r2, r9
	adds r7, #60
	ldr r3, [r2]
	adds r0, r6, #0
	adds r1, r7, #0
	mov lr, r3
	.2byte 0xf800
	lsls r0, r0, #16
	cmp r0, #0
	bne .L_08015bea
	adds r0, r6, #0
	adds r1, r7, #0
	bl Flash_VerifySector
	cmp r0, #0
	bne .L_08015bea
	cmp r5, #0
	beq .L_08015b84
	movs r0, #1
	mov lr, r5
	.2byte 0xf800
.L_08015b84:
	mov r3, r10
	movs r7, #128
	adds r3, #1
	lsls r7, r7, #5
	lsls r3, r3, #16
	adds r7, #60
	lsrs r6, r3, #16
	mov r2, r9
	add r7, r8
	ldr r3, [r2]
	adds r0, r6, #0
	adds r1, r7, #0
	mov lr, r3
	.2byte 0xf800
	lsls r0, r0, #16
	cmp r0, #0
	bne .L_08015bea
	adds r0, r6, #0
	adds r1, r7, #0
	bl Flash_VerifySector
	cmp r0, #0
	bne .L_08015bea
	cmp r5, #0
	beq .L_08015bbc
	movs r0, #1
	mov lr, r5
	.2byte 0xf800
.L_08015bbc:
	mov r3, r10
	movs r7, #128
	adds r3, #2
	lsls r7, r7, #6
	lsls r3, r3, #16
	adds r7, #60
	lsrs r6, r3, #16
	mov r2, r9
	add r7, r8
	ldr r3, [r2]
	adds r0, r6, #0
	adds r1, r7, #0
	mov lr, r3
	.2byte 0xf800
	lsls r0, r0, #16
	cmp r0, #0
	bne .L_08015bea
	adds r0, r6, #0
	adds r1, r7, #0
	bl Flash_VerifySector
	cmp r0, #0
	beq .L_08015bee
.L_08015bea:
	movs r0, #1
	b .L_08015bfa
.L_08015bee:
	cmp r5, #0
	beq .L_08015bf8
	movs r0, #1
	mov lr, r5
	.2byte 0xf800
.L_08015bf8:
	movs r0, #0
.L_08015bfa:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08015c04:
	.4byte Flash_Handler0
