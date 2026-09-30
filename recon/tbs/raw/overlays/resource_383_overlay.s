.syntax unified
	.thumb
	.section .text.x02008690,"ax",%progbits
	.global Func_02000690
	.thumb_func
Func_02000690:
	push {r5, r6, r7, lr}
	movs r0, #25
	bl Object_GetById
	movs r7, #240
	ldrh r3, [r0, #6]
	adds r5, r0, #0
	adds r5, #100
	lsls r7, r7, #8
	ands r7, r3
	ldrh r3, [r5]
	lsls r3, r3, #16
	asrs r6, r3, #17
	bl Engine_EventBegin
	movs r1, #2
	movs r0, #25
	bl Engine_ActorRunRepeatedMotion
	ldr r0, .L_020087a8
	bl Engine_EventSetMessage
	movs r0, #25
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #224
	movs r2, #224
	movs r0, #25
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Engine_ActorSetSpeed
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #4
	bhi .L_0200878e
	ldr r2, .L_020087ac
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_020086e4:
	.4byte .L_0200871a
	.4byte .L_02008742
	.4byte .L_0200871a
	.4byte .L_02008742
	.4byte .L_020086f8
.L_020086f8:
	ldr r2, .L_020087b0
	ldr r0, .L_020087b4
	adds r3, r7, r2
	cmp r3, r0
	bhi .L_0200870e
	ldr r1, .L_020087b8
	movs r0, #25
	bl Engine_ActorEnableActionCallback
	movs r3, #2
	b .L_0200878c
.L_0200870e:
	ldr r1, .L_020087bc
	movs r0, #25
	bl Engine_ActorEnableActionCallback
	movs r3, #3
	b .L_0200878c
.L_0200871a:
	ldr r2, .L_020087b0
	ldr r0, .L_020087b4
	adds r3, r7, r2
	cmp r3, r0
	bhi .L_0200876a
	movs r0, #0
	ldrsh r2, [r5, r0]
	lsls r3, r6, #2
	adds r3, r3, r2
	ldr r1, .L_020087c0
	lsls r3, r3, #2
	ldr r1, [r1, r3]
	movs r0, #25
	bl Engine_ActorEnableActionCallback
	ldrh r3, [r5]
	lsls r2, r6, #1
	subs r3, r3, r2
	adds r3, #1
	b .L_0200878c
.L_02008742:
	ldr r0, .L_020087c4
	ldr r2, .L_020087b4
	adds r3, r7, r0
	cmp r3, r2
	bhi .L_0200876a
	movs r0, #0
	ldrsh r2, [r5, r0]
	lsls r3, r6, #2
	adds r3, r3, r2
	ldr r1, .L_020087c0
	lsls r3, r3, #2
	ldr r1, [r1, r3]
	movs r0, #25
	bl Engine_ActorEnableActionCallback
	ldrh r3, [r5]
	lsls r2, r6, #1
	subs r3, r3, r2
	adds r3, #1
	b .L_0200878c
.L_0200876a:
	movs r3, #1
	movs r0, #0
	ldrsh r2, [r5, r0]
	eors r3, r6
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r1, .L_020087c0
	lsls r3, r3, #2
	ldr r1, [r1, r3]
	movs r0, #25
	bl Engine_ActorEnableActionCallback
	ldrh r3, [r5]
	lsls r2, r6, #1
	subs r3, r3, r2
	ldr r2, .L_020087c8
	adds r3, r3, r2
.L_0200878c:
	strh r3, [r5]
.L_0200878e:
	ldrh r2, [r5]
	movs r3, #3
	ands r3, r2
	strh r3, [r5]
	movs r0, #25
	bl Object_RefreshSelectorById
	bl Engine_EventEnd
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_020087a8:
	.4byte 0x000012ad
.L_020087ac:
	.4byte .L_020086e4
.L_020087b0:
	.4byte 0xffffdfff
.L_020087b4:
	.4byte 0x00007ffe
.L_020087b8:
	.4byte KuupuappuHeya_PairScriptR
.L_020087bc:
	.4byte KuupuappuHeya_PairScriptP
.L_020087c0:
	.4byte Data_020064d8
.L_020087c4:
	.4byte 0xffff9fff
.L_020087c8:
	.4byte 0x0000ffff
	.section .text.x0200c8c6,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200c8c8,"ax",%progbits
	.global KuupuappuHeya_UpdateActorStops
	.thumb_func
KuupuappuHeya_UpdateActorStops:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #4
	bl Engine_ActorLookup
	ldr r3, .L_0200cb20
	ldr r3, [r3]
	movs r1, #0
	mov r8, r0
	movs r0, #2
	mov r9, r1
	mov r11, r3
	bl Engine_ActorLookup
	adds r7, r0, #0
	adds r5, r7, #0
	adds r5, #8
	adds r0, r5, #0
	bl SceneData_FindEntryAtPosition
	mov r10, r0
	cmp r0, #0
	beq .L_0200c9b8
	movs r2, #128
	ldr r3, [r7, #56]
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200c9b8
	mov r1, r8
	ldr r2, [r5]
	ldr r3, [r1, #8]
	subs r6, r2, r3
	ldr r2, [r7, #16]
	ldr r3, [r1, #16]
	subs r5, r2, r3
	movs r2, #6
	ldrsh r3, [r1, r2]
	movs r1, #2
	add r1, sp
	mov r8, r1
	mov r2, r8
	adds r1, r6, #0
	strh r3, [r2]
	adds r0, r5, #0
	bl ArcTan2
	movs r3, #206
	lsls r3, r3, #1
	add r3, r11
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r0, r0, #16
	asrs r0, r0, #16
	asrs r6, r6, #16
	asrs r5, r5, #16
	cmp r3, #0
	ble .L_0200c976
	adds r4, r6, #0
	muls r4, r6
	adds r1, r5, #0
	muls r1, r5
	movs r2, #200
	adds r3, r4, r1
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_0200c97e
	mov r3, r8
	ldrh r2, [r3]
	lsls r3, r0, #16
	lsrs r3, r3, #16
	subs r2, r2, r3
	lsls r2, r2, #16
	asrs r0, r2, #16
	ldr r2, .L_0200cb24
	cmp r0, r2
	ble .L_0200c97e
	movs r3, #128
	lsls r3, r3, #5
	cmp r0, r3
	bge .L_0200c97e
	b .L_0200c98c
.L_0200c976:
	adds r4, r6, #0
	muls r4, r6
	adds r1, r5, #0
	muls r1, r5
.L_0200c97e:
	adds r3, r4, r1
	cmp r3, #64
	ble .L_0200c98c
	movs r1, #6
	ldrsh r3, [r7, r1]
	mov r2, r8
	strh r3, [r2]
.L_0200c98c:
	mov r0, r10
	mov r1, r8
	bl KuupuappuHeya_SnapToNearestStop
	adds r5, r0, #0
	bl SceneActor_CheckTileFreeOfKinds
	cmp r0, #0
	bne .L_0200c9b0
	adds r0, r7, #0
	adds r1, r5, #0
	bl SceneActor_ApplyScaledBytePairPosition
	adds r0, r7, #0
	movs r1, #2
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_0200c9b8
.L_0200c9b0:
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_ApplyArgumentToChildren
.L_0200c9b8:
	movs r0, #24
	bl Engine_ActorLookup
	adds r7, r0, #0
	adds r0, #8
	bl SceneData_FindEntryAtPosition
	mov r10, r0
	cmp r0, #0
	beq .L_0200ca4c
	movs r1, #128
	ldr r3, [r7, #56]
	lsls r1, r1, #24
	cmp r3, r1
	bne .L_0200ca4c
	bl Random16
	lsls r0, r0, #1
	lsrs r0, r0, #16
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r1, #208
	lsls r1, r1, #24
	lsls r3, r3, #29
	ldrh r2, [r7, #6]
	adds r3, r3, r1
	mov r6, sp
	lsrs r3, r3, #16
	adds r6, #2
	adds r3, r3, r2
	strh r3, [r6]
	mov r0, r10
	adds r1, r6, #0
	bl KuupuappuHeya_SnapToNearestStop
	adds r5, r0, #0
	bl SceneActor_CheckTileFreeOfKinds
	cmp r0, #0
	beq .L_0200ca3c
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	strh r3, [r6]
	mov r0, r10
	adds r1, r6, #0
	bl KuupuappuHeya_SnapToNearestStop
	adds r5, r0, #0
	bl SceneActor_CheckTileFreeOfKinds
	cmp r0, #0
	bne .L_0200ca2e
	movs r0, #24
	movs r1, #2
	bl Engine_ActorSetAttachedEffect
	b .L_0200ca3c
.L_0200ca2e:
	adds r0, r7, #0
	movs r1, #4
	bl ObjectDispatch_ApplyArgumentToChildren
	movs r3, #1
	mov r9, r3
	b .L_0200ca4c
.L_0200ca3c:
	adds r0, r7, #0
	adds r1, r5, #0
	bl SceneActor_ApplyScaledBytePairPosition
	adds r0, r7, #0
	movs r1, #2
	bl ObjectDispatch_ApplyArgumentToChildren
.L_0200ca4c:
	movs r0, #25
	bl Engine_ActorLookup
	adds r7, r0, #0
	adds r0, #8
	bl SceneData_FindEntryAtPosition
	mov r10, r0
	cmp r0, #0
	beq .L_0200cae2
	movs r1, #128
	ldr r3, [r7, #56]
	lsls r1, r1, #24
	cmp r3, r1
	bne .L_0200cae2
	bl Random16
	lsls r2, r0, #1
	adds r2, r2, r0
	lsrs r2, r2, #16
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r1, #208
	lsls r1, r1, #24
	lsls r3, r3, #28
	ldrh r2, [r7, #6]
	adds r3, r3, r1
	mov r6, sp
	lsrs r3, r3, #16
	adds r6, #2
	adds r3, r3, r2
	strh r3, [r6]
	mov r0, r10
	adds r1, r6, #0
	bl KuupuappuHeya_SnapToNearestStop
	adds r5, r0, #0
	bl SceneActor_CheckTileFreeOfKinds
	cmp r0, #0
	beq .L_0200cad2
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	strh r3, [r6]
	mov r0, r10
	adds r1, r6, #0
	bl KuupuappuHeya_SnapToNearestStop
	adds r5, r0, #0
	bl SceneActor_CheckTileFreeOfKinds
	cmp r0, #0
	bne .L_0200cac4
	movs r0, #25
	movs r1, #2
	bl Engine_ActorSetAttachedEffect
	b .L_0200cad2
.L_0200cac4:
	adds r0, r7, #0
	movs r1, #4
	bl ObjectDispatch_ApplyArgumentToChildren
	movs r3, #2
	add r9, r3
	b .L_0200cae2
.L_0200cad2:
	adds r0, r7, #0
	adds r1, r5, #0
	bl SceneActor_ApplyScaledBytePairPosition
	adds r0, r7, #0
	movs r1, #2
	bl ObjectDispatch_ApplyArgumentToChildren
.L_0200cae2:
	mov r1, r9
	cmp r1, #0
	beq .L_0200cb08
	ldr r2, .L_0200cb28
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r2, #232
	lsls r3, r3, #16
	lsls r2, r2, #13
	cmp r3, r2
	bls .L_0200cb0e
	movs r2, #193
	mov r3, r9
	lsls r2, r2, #1
	adds r3, #200
	add r2, r11
	strh r3, [r2]
	b .L_0200cb0e
.L_0200cb08:
	ldr r3, .L_0200cb28
	mov r1, r9
	strh r1, [r3]
.L_0200cb0e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0200cb20:
	.4byte gEventWork
.L_0200cb24:
	.4byte 0xfffff000
.L_0200cb28:
	.4byte KuupuappuHeya_StopTimer
	.section .rodata.x0200cf2c,"a",%progbits
	.global KuupuappuHeya_Stops
KuupuappuHeya_Stops:
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0x0000801e
	.4byte 0x0000ffff
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0x00008001
	.4byte 0x0000401f
	.4byte 0x0000000a
	.4byte 0x00000004
	.4byte 0x00008002
	.4byte 0x0000ffff
	.4byte 0x0000000c
	.4byte 0x00000005
	.4byte 0x00008003
	.4byte 0x0000ffff
	.4byte 0x0000000e
	.4byte 0x00000006
	.4byte 0x00008004
	.4byte 0x0000ffff
	.4byte 0x00000010
	.4byte 0x00000007
	.4byte 0x00008005
	.4byte 0x00004022
	.4byte 0x00000012
	.4byte 0x00000008
	.4byte 0x00008006
	.4byte 0x0000ffff
	.4byte 0x00000014
	.4byte 0x00000009
	.4byte 0x00008007
	.4byte 0x0000ffff
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00008008
	.4byte 0x0000ffff
	.4byte 0x00000018
	.4byte 0x0000400b
	.4byte 0x00008009
	.4byte 0x0000ffff
	.4byte 0x00000218
	.4byte 0x0000400c
	.4byte 0x0000c00a
	.4byte 0x0000ffff
	.4byte 0x00000418
	.4byte 0x0000400d
	.4byte 0x0000c00b
	.4byte 0x0000ffff
	.4byte 0x00000618
	.4byte 0x0000400e
	.4byte 0x0000c00c
	.4byte 0x0000ffff
	.4byte 0x00000818
	.4byte 0x0000800f
	.4byte 0x0000c00d
	.4byte 0x0000ffff
	.4byte 0x00000816
	.4byte 0x00008010
	.4byte 0x0000000e
	.4byte 0x0000ffff
	.4byte 0x00000814
	.4byte 0x00008011
	.4byte 0x0000000f
	.4byte 0x0000ffff
	.4byte 0x00000812
	.4byte 0x00008012
	.4byte 0x00000010
	.4byte 0x0000ffff
	.4byte 0x00000810
	.4byte 0x00008013
	.4byte 0x00000011
	.4byte 0x0000c024
	.4byte 0x0000080e
	.4byte 0x00008014
	.4byte 0x00000012
	.4byte 0x0000ffff
	.4byte 0x0000080c
	.4byte 0x00008015
	.4byte 0x00000013
	.4byte 0x0000ffff
	.4byte 0x0000080a
	.4byte 0x00008016
	.4byte 0x00000014
	.4byte 0x0000ffff
	.4byte 0x00000808
	.4byte 0x00008017
	.4byte 0x00000015
	.4byte 0x0000c021
	.4byte 0x00000806
	.4byte 0x00008018
	.4byte 0x00000016
	.4byte 0x0000ffff
	.4byte 0x00000804
	.4byte 0x00008019
	.4byte 0x00000017
	.4byte 0x0000ffff
	.4byte 0x00000802
	.4byte 0x0000801a
	.4byte 0x00000018
	.4byte 0x0000ffff
	.4byte 0x00000800
	.4byte 0x0000c01b
	.4byte 0x00000019
	.4byte 0x0000ffff
	.4byte 0x00000600
	.4byte 0x0000c01c
	.4byte 0x0000401a
	.4byte 0x0000ffff
	.4byte 0x00000400
	.4byte 0x0000c01d
	.4byte 0x0000401b
	.4byte 0x0000ffff
	.4byte 0x00000200
	.4byte 0x0000001e
	.4byte 0x0000401c
	.4byte 0x0000ffff
	.4byte 0x00000102
	.4byte 0x00000000
	.4byte 0x0000801d
	.4byte 0x0000ffff
	.4byte 0x00000208
	.4byte 0x00004020
	.4byte 0x0000c002
	.4byte 0x0000ffff
	.4byte 0x00000408
	.4byte 0x00004021
	.4byte 0x0000c01f
	.4byte 0x0000ffff
	.4byte 0x00000608
	.4byte 0x00004016
	.4byte 0x0000c020
	.4byte 0x0000ffff
	.4byte 0x00000210
	.4byte 0x00004023
	.4byte 0x0000c006
	.4byte 0x0000ffff
	.4byte 0x00000410
	.4byte 0x00004024
	.4byte 0x0000c022
	.4byte 0x0000ffff
	.4byte 0x00000610
	.4byte 0x00004012
	.4byte 0x0000c023
	.4byte 0x0000ffff
	.global KuupuappuHeya_ActionTable
KuupuappuHeya_ActionTable:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x000000b4
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03200000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000010
	.global KuupuappuHeya_VaultScriptA
KuupuappuHeya_VaultScriptA:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global KuupuappuHeya_VaultScriptB
KuupuappuHeya_VaultScriptB:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global KuupuappuHeya_VaultScriptC
KuupuappuHeya_VaultScriptC:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.global KuupuappuHeya_VaultScriptD
KuupuappuHeya_VaultScriptD:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.global KuupuappuHeya_VaultScriptE
KuupuappuHeya_VaultScriptE:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptA
KuupuappuHeya_PairScriptA:
.L_0200d538:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptB
KuupuappuHeya_PairScriptB:
.L_0200d560:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptC
KuupuappuHeya_PairScriptC:
.L_0200d5b0:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptD
KuupuappuHeya_PairScriptD:
.L_0200d5d8:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptE
KuupuappuHeya_PairScriptE:
.L_0200d600:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptF
KuupuappuHeya_PairScriptF:
.L_0200d650:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptG
KuupuappuHeya_PairScriptG:
.L_0200d678:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptH
KuupuappuHeya_PairScriptH:
.L_0200d6c8:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptI
KuupuappuHeya_PairScriptI:
.L_0200d6f0:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptJ
KuupuappuHeya_PairScriptJ:
.L_0200d718:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptK
KuupuappuHeya_PairScriptK:
.L_0200d768:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptL
KuupuappuHeya_PairScriptL:
.L_0200d7a4:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptM
KuupuappuHeya_PairScriptM:
.L_0200d7cc:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptN
KuupuappuHeya_PairScriptN:
.L_0200d808:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptO
KuupuappuHeya_PairScriptO:
.L_0200d830:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptP
KuupuappuHeya_PairScriptP:
.L_0200d858:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptQ
KuupuappuHeya_PairScriptQ:
.L_0200d894:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_PairScriptR
KuupuappuHeya_PairScriptR:
.L_0200d8bc:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KuupuappuHeya_Scripts
KuupuappuHeya_Scripts:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000c0
	.4byte 0xc00000dc
	.4byte 0x00200000
	.4byte 0x01100000
	.4byte 0x000000f0
	.4byte 0xffff0006
	.4byte 0x000001b0
	.4byte 0xc00000fe
	.4byte 0x01200000
	.4byte 0x02100000
	.4byte 0x00000110
	.4byte 0xffff0007
	.4byte 0x000002d0
	.4byte 0xc000030e
	.4byte 0x01b00000
	.4byte 0x03200240
	.4byte 0x00000320
	.4byte 0xffff0008
	.4byte 0x00000270
	.4byte 0xc00000ec
	.4byte 0x02300000
	.4byte 0x03500020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000090
	.4byte 0xc00001ec
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000200
	.4byte 0xffff000a
	.4byte 0x000001a0
	.4byte 0xc000020e
	.4byte 0x01400000
	.4byte 0x02500120
	.4byte 0x00000220
	.4byte 0xffff000b
	.4byte 0x000002e8
	.4byte 0x40000280
	.4byte 0x01b00000
	.4byte 0x03200240
	.4byte 0x00000320
	.4byte 0xffff000c
	.4byte 0x00000166
	.4byte 0x40000278
	.4byte 0x00200000
	.4byte 0x01900240
	.4byte 0x00000308
	.4byte 0xffff000d
	.4byte 0x00000288
	.4byte 0x400001a8
	.4byte 0x02300000
	.4byte 0x03200140
	.4byte 0x000001e0
	.4byte 0xffff000e
	.4byte 0x000002c0
	.4byte 0x80000198
	.4byte 0x02300000
	.4byte 0x03200140
	.4byte 0x000001e0
	.4byte 0xffff000f
	.4byte 0x00000300
	.4byte 0x00000198
	.4byte 0x02c80000
	.4byte 0x03b80120
	.4byte 0x000001f0
	.4byte 0xffff0010
	.4byte 0x00000318
	.4byte 0xc00001b0
	.4byte 0x02c80000
	.4byte 0x03b80120
	.4byte 0x000001f0
	.4byte 0xffff0011
	.4byte 0x00000318
	.4byte 0xc00001b0
	.4byte 0x02c80000
	.4byte 0x03b80120
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuHeya_Regions
KuupuappuHeya_Regions:
	.4byte 0x0021027f
	.4byte 0x02910197
	.4byte 0x01a90033
	.4byte 0x000dffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuHeya_Messages
KuupuappuHeya_Messages:
	.4byte 0x00000015
	.4byte 0x00505014
	.4byte 0x00606014
	.4byte 0x00707014
	.4byte 0x00808014
	.4byte 0x00909014
	.4byte 0x00a0a014
	.4byte 0x00b0b015
	.4byte 0x00c0c015
	.4byte 0x00d0d014
	.4byte 0x00e0e015
	.4byte 0x00f0f015
	.4byte 0x0100c009
	.4byte 0x000001ff
	.global KuupuappuHeya_SceneTableA
KuupuappuHeya_SceneTableA:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00013000
	.4byte 0x0000006a
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00008000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x0000006b
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0x00000067
	.4byte 0x00000002
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00004000
	.4byte 0x0000006b
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00010000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00018000
	.4byte 0x00000075
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01c30000
	.4byte 0x00004000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0000c000
	.4byte 0x0000006a
	.4byte 0x00000002
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x0000c000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00014000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00014000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00034000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00018000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00014000
	.4byte 0x0854003f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00030000
	.4byte 0x08540040
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00004000
	.4byte 0x000000df
	.4byte 0x00000001
	.4byte 0x02b40000
	.4byte 0x00000000
	.4byte 0x019b0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuHeya_SceneTableB
KuupuappuHeya_SceneTableB:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00005000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001b000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00004000
	.4byte 0xffff003f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00030000
	.4byte 0xffff0040
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00004000
	.4byte 0xffff0040
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00004000
	.4byte 0xffff00fb
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff00dd
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuHeyaEvents
gKuupuappuHeyaEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte SceneState_SetValue123Mode11
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x1856000a
	.4byte FieldScene_RunOpeningSequenceThird
	.4byte 0x00000002
	.4byte 0x1300000b
	.4byte KuupuappuHeya_PoseDialogueActors
	.4byte 0x00000002
	.4byte 0x1300000c
	.4byte KuupuappuHeya_PoseDialogueActors
	.4byte 0x00000002
	.4byte 0x1300000d
	.4byte KuupuappuHeya_PoseDialogueActors
	.4byte 0x00000002
	.4byte 0x1300000e
	.4byte KuupuappuHeya_PoseDialogueActors
	.4byte 0x00000002
	.4byte 0x1300000f
	.4byte KuupuappuHeya_PoseDialogueActors
	.4byte 0x00000002
	.4byte 0x13000010
	.4byte KuupuappuHeya_PoseDialogueActors
	.4byte 0x00000002
	.4byte 0x13000011
	.4byte KuupuappuHeya_PoseDialogueActors
	.4byte 0x00000002
	.4byte 0x13000012
	.4byte KuupuappuHeya_PoseDialogueActors
	.4byte 0x00000002
	.4byte 0x13000014
	.4byte FieldScene_RunScene383SequenceB
	.4byte 0x00000002
	.4byte 0x18520015
	.4byte FieldScene_RunSteps107And250
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte FieldScene_SelectActorPair
	.4byte 0x00000006
	.4byte 0xffff00ca
	.4byte FieldScene_SelectActorPair
	.4byte 0x00000006
	.4byte 0xffff00cb
	.4byte FieldScene_SelectActorPair
	.4byte 0x00000006
	.4byte 0xffff00fa
	.4byte SceneDialogue_ShowLine12BB
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte FieldScene_RunOpeningSequenceHead
	.4byte 0x00000202
	.4byte 0xffff0028
	.4byte ActorPresentation_SetSceneCellByAngle
	.4byte 0x00000000
	.4byte 0x12500002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte FieldScene_RunSetupSequence
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001242
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte SceneDialogue_RunActor9FlaggedLine
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001246
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte SceneDialogue_RunActorElevenDialogue
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte SceneDialogue_RunActorTwelveFlaggedDialogue
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000124d
	.4byte 0x00000000
	.4byte 0x0856000e
	.4byte 0x00001252
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000127a
	.4byte 0x00000000
	.4byte 0x0856000f
	.4byte FieldScene_RunScene383_02000428
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000127b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte FieldScene_RunFlag856DialogueBranch
	.4byte 0x00000000
	.4byte 0x08560011
	.4byte 0x00001251
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001279
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte SceneDialogue_ShowLine128E
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte FieldScene_RunActorNineteenAngleDialogue
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte FieldScene_RunActorTwentyAngleDialogue
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte SceneState_BranchOnSlotZeroFacingAndFlag855
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000128c
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte FieldScene_RunActorTwentyThreeAngleDialogue
	.4byte 0x00000000
	.4byte 0x12500018
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x12500019
	.4byte 0x00000000
	.4byte 0x00008d15
	.4byte 0x12500018
	.4byte 0x00000000
	.4byte 0x00008d15
	.4byte 0x12500019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x08530018
	.4byte 0x00001290
	.4byte 0x00000000
	.4byte 0x08530019
	.4byte 0x00001291
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte SceneActor_StepActor24AnimationByFacing
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte Func_02000690
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001244
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte SceneDialogue_RunActorNineFlaggedDialogue
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000124a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte SceneDialogue_RunActorElevenFlaggedDialogue
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte SceneDialogue_ShowLine124EOr135E
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000124f
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000127e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000127f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte SceneDialogue_RunActor16FlaggedLine
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000127d
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte SceneDialogue_RunActorEighteenBranchedDialogue
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001281
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001283
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001287
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001292
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001293
	.4byte 0x00008d15
	.4byte 0xffff0418
	.4byte SceneActor_StepActor24AnimationByFacing
	.4byte 0x00008d15
	.4byte 0xffff0419
	.4byte Func_02000690
	.4byte 0x00008c15
	.4byte 0x0859001a
	.4byte FieldScene_RunObjectTwentySixPositionCheck
	.4byte 0x00000003
	.4byte 0xffff0029
	.4byte SceneDialogue_ShowEmptyBarrel
	.4byte 0x00000023
	.4byte 0x0f4b0064
	.4byte 0x00200007
	.4byte 0x00000033
	.4byte 0x0f4c0065
	.4byte 0x00200004
	.4byte 0x00000033
	.4byte 0x0f4d0066
	.4byte 0x001000e3
	.4byte 0x000000d3
	.4byte 0x0f4e0067
	.4byte 0x001000c3
	.4byte 0x0000c4f3
	.4byte 0xffff00c8
	.4byte 0x004029d1
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029d2
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuHeyaEventsEntrances15To17
gKuupuappuHeyaEventsEntrances15To17:
	.4byte 0x00000002
	.4byte 0xffff001b
	.4byte FieldScene_RunOpeningSequenceSecond
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunVaultClosingSequence
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte SceneDialogue_ShowEmptyChest
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte SceneDialogue_ShowEmptyChest
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte SceneDialogue_ShowEmptyChest
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000012c4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuHeyaEventsFlag855
gKuupuappuHeyaEventsFlag855:
	.4byte 0x00000003
	.4byte 0xffff0029
	.4byte FieldScene_RunActorEighteenConditionalScene
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte FieldScene_RunOpeningSequenceHead
	.4byte 0x00000002
	.4byte 0xffff001b
	.4byte FieldScene_RunOpeningSequenceSecond
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001352
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte SceneDialogue_RunActor9FlaggedLine
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte SceneDialogue_RunActor10Line
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte SceneDialogue_RunActor11Line
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte SceneDialogue_RunActorTwelveFlaggedDialogue
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000135d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte SceneDialogue_RunActor14Line
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000136b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte FieldScene_RunScene383SequenceC
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001367
	.4byte 0x00000000
	.4byte 0x02500012
	.4byte FieldScene_RunScene383_0200091c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte FieldScene_RunActorNineteenAngleDialogue
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte FieldScene_RunActorTwentyAngleDialogue
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte SceneState_BranchOnSlotZeroFacingAndFlag855
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0000137a
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte FieldScene_RunActorTwentyThreeAngleDialogue
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001354
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte SceneDialogue_RunActorNineFlaggedDialogue
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000135a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte SceneDialogue_RunActorElevenFlaggedDialogue
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte SceneDialogue_ShowLine124EOr135E
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000135f
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000136e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000136f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte SceneDialogue_RunActor16FlaggedLine
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000136d
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte SceneDialogue_RunActorEighteenBranchedDialogue
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001371
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001373
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001375
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001380
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001381
	.4byte 0x00000023
	.4byte 0x0f4b0064
	.4byte 0x00200007
	.4byte 0x00000033
	.4byte 0x0f4c0065
	.4byte 0x00200004
	.4byte 0x00000033
	.4byte 0x0f4d0066
	.4byte 0x001000e3
	.4byte 0x000000d3
	.4byte 0x0f4e0067
	.4byte 0x001000c3
	.4byte 0x0000c4f3
	.4byte 0xffff00c8
	.4byte 0x004029d1
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x004029d2
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuHeya_StepActions
KuupuappuHeya_StepActions:
	.4byte .L_0200d5b0
	.4byte .L_0200d5d8
	.4byte .L_0200d718
	.4byte .L_0200d6c8
	.4byte .L_0200d650
	.4byte .L_0200d560
	.global KuupuappuHeya_IdleActions
KuupuappuHeya_IdleActions:
	.4byte .L_0200d678
	.4byte .L_0200d538
	.4byte .L_0200d5b0
	.4byte .L_0200d600
	.4byte .L_0200d6f0
	.4byte .L_0200d6c8
	.global Data_020064d8
Data_020064d8:
	.4byte .L_0200d7cc
	.4byte .L_0200d894
	.4byte .L_0200d858
	.4byte .L_0200d7a4
	.4byte .L_0200d830
	.4byte .L_0200d768
	.4byte .L_0200d808
	.4byte .L_0200d8bc
	.section .bss,"aw",%nobits
	.global KuupuappuHeya_StopTimer
KuupuappuHeya_StopTimer:
	.space 4
