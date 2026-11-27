/* Link battles: the parties and Djinn exchanged, and the turn handshake. Part
   of battle.c. */

/* What a link battle sends of each party member, and of the Djinn recovery
   list, in bytes. */
typedef enum LINK_PARTY_PACKET_SIZE {
	/* A UNIT (BATTLE_UNIT_SIZE, 332 bytes) and the 8 after it. */
	LINK_PARTY_UNIT_BYTES = 340,
	/* A DJINN_RECOVERY_TABLE and what follows it. */
	LINK_PARTY_OFFER_BYTES = 340
} LINK_PARTY_PACKET_SIZE;

/* The eight halfwords of a link signature, at the transfer payload's start. */
#define LINK_SIGNATURE_BYTES (8 * sizeof(Uint16))

/* Unused: receives the opponent's party into the enemy units, "Enemy" before
   each name, then its Djinn recovery list. Returns how many party members
   came. */
Sint32 btLinkBattleReceiveParty(void)
{
	Uint16 label[16];
	void  *buf;
	UNIT  *opponent;
	Sint32 memberCnt;
	Sint32 partySlot;
	Sint32 labelLength;
	Sint32 i;

	/* XXX (no evidence): what the block is for; each party member is received
	   straight into its enemy unit, and the block is only freed. */
	buf = syHeapAllocEWRAM(LINK_PARTY_UNIT_BYTES);
	memberCnt = 0;
	for(partySlot = 0; partySlot < LINK_PARTY_SIZE; partySlot++) {
		opponent = syUnitGet(partySlot + FIRST_ENEMY_UNIT);
		if(sySerialStartReceive(opponent) == -1) {
			break;
		}
		sySerialWaitForReceive();
		if(opponent->side != 0) {
			memberCnt += 1;
		}
		sySynch(2);

		/* Measure the label (at most BATTLE_ARENA_PREFIX_LAST + 1 characters),
		   shift the name right to make room for it, then write it in; the
		   name's end is cut. */
		mnMessageDecodeRawUnboundedStub((Sint32)MsgEnemyLabel, label);
		for(i = 0; i <= BATTLE_ARENA_PREFIX_LAST; i++) {
			if(label[i] == 0)
				break;
		}
#if SPACED_ENEMY_NAME_PREFIX
		label[i] = ' ';
		labelLength = i + 1;
#else
		labelLength = i;
#endif
		for(i = UNIT_NAME_LENGTH; i >= labelLength; i--) {
			opponent->name[i] = opponent->name[i - labelLength];
		}
		for(i = 0; i < labelLength; i++) {
			opponent->name[i] = label[i];
		}
		opponent->name[UNIT_NAME_LENGTH] = 0;
	}
	syHeapRelease(buf);

	/* The recovery packet follows all three party places. */
	buf = syHeapAllocEWRAM(LINK_PARTY_OFFER_BYTES);
	if(sySerialStartReceive(
		   gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_OPPONENT)) != -1) {
		sySerialWaitForReceive();
		sySynch(2);
	}
	syHeapRelease(buf);
	return memberCnt;
}

/* Unused: sends the party, then the Djinn recovery list, each unit under the id
   the opponent knows it by. */
Sint32 btLinkBattleSendParty(void)
{
	Uint8				*buf;
	BATTLE_STATE		*battle;
	Sint32				 j;
	DJINN_RECOVERY_LIST *list;
	Sint32				 size;
	Sint32				 cnt;
	Sint32				 i;
	Uint16				 unitIds[8];

	size = LINK_PARTY_UNIT_BYTES;
	buf = (Uint8 *)syHeapAllocEWRAM(size);
	{
		Sint32 noSlot = OPPONENT_SLOT_NONE;

		battle = RamHeapSlots->battleState;
		for(i = ARRAY_COUNT(battle->opponentSlots) - 1; i >= 0; i--)
			battle->opponentSlots[i] = noSlot;

		/* The party members, each marked as the opponent's. */
		cnt = btPartyPrepareMembers(unitIds);
		for(i = 0; i < cnt; i++) {
			syWordsCopyEntry(buf, syUnitGet(unitIds[i]), size);
			((UNIT *)buf)->side = BATTLE_SIDE_PARTY;
			battle->opponentSlots[unitIds[i]] = FIRST_ENEMY_UNIT + i;
			if(sySerialStartSend(buf, LINK_PARTY_UNIT_BYTES) == -1)
				break;
			sySerialWaitForSend();
			sySynch(2);
		}
		/* Empty places up to three. */
		for(; i < LINK_PARTY_SIZE; i++) {
			((UNIT *)buf)->side = BATTLE_SIDE_NONE;
			if(sySerialStartSend(buf, LINK_PARTY_UNIT_BYTES) == -1)
				break;
			sySerialWaitForSend();
			sySynch(2);
		}

		/* The Djinn recovery list, its units renumbered for the opponent. */
		size = LINK_PARTY_OFFER_BYTES;
		syHeapRelease(buf);
		buf = (Uint8 *)syHeapAllocEWRAM(size);
		IWRAM_COPY_WORDS(
			buf, gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY), size);
		list = &((DJINN_RECOVERY_TABLE *)buf)->list;
		for(j = 0; j < list->cnt; j++)
			list->entries[j].unitId =
				battle->opponentSlots[list->entries[j].unitId];
		if(sySerialStartSend(buf, LINK_PARTY_OFFER_BYTES) != -1) {
			sySerialWaitForSend();
			sySynch(1);
			sySynch(2);
		}
		syHeapRelease(buf);
	}
}

/* Copies a name's first four characters, an underscore for each zero, and
   returns a pointer to the end. UB: the copy is on the stack; UBFIX keeps
   it. */
Uint8 *btTextCopyFourCharsUnderscored(
	Uint32 unusedA, Uint32 unusedB, Uint16 *src)
{
#if FIX_RETURNED_STACK_BUFFER
	static Uint8 sBuffer[5];
	Uint8		*base = sBuffer;
#else
	Uint8  buf[5];
	Uint8 *base = buf;
#endif
	Uint32 fill = '_';
	Uint8 *cur = base;
	Uint8 *dst = cur;
	Sint32 cnt;

	for(cnt = 0; cnt < 4; cnt++) {
		Uint32 value = *src;

		*dst = value;
		src++;
		dst++;
		if((Uint8)value == 0)
			*cur = fill;
		cur++;
	}
	base[4] = 0;
	/* BUG: the original buffer is on the stack, gone once this returns. UBFIX
	   keeps the result until the next call in private scratch storage. */
	return dst;
}

/* In a link battle, shows the "EXEC" signature and waits up to 30 ticks for
   the other console to show it too. Returns -1 when the link fails, else 0. */
Sint32 btLinkBattleWaitSync(void)
{
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Uint16		 *peer;
	Uint16		 *signature;
	Sint32		  missed = 0;
	Sint32		  i;

	if(battle->isLink != 0) {
		Uint32 other = battle->linkSide ^ 1;

		/* LinkPeerSignatures[other]: the signature starts a peer's payload,
		   after its record's four bytes of block state. */
		peer = (Uint16 *)SerialPeerPayloads[other].payload;
		signature = (Uint16 *)SerialTransfer.payload;
		if(battle->linkPaused == 0) {
			/* The other console is in step once its second pair reads "EC". */
			signature[0] = 'E';
			signature[1] = 'X';
			signature[2] = 'E';
			signature[3] = 'C';

			for(i = 0; i < 30; i++) {
				if((LinkStatusWork & LINK_BOTH_CONNECTED) !=
					LINK_BOTH_CONNECTED) {
					missed++;
					if(missed > LINK_MAX_MISSED_TICKS)
						return -1;
				} else {
					missed = 0;
					if(signature[2] == peer[2] && signature[3] == peer[3])
						return 0;
				}
				sySynch(1);
			}
		}
		return -1;
	}
	return 0;
}

/* In a link battle, after each action and each turn, steps both consoles
   through the same handshake of signatures: "ex" and "TU", "rn", "EXEC", "tu",
   "RN". Returns -1 when the link fails, the steps disagree or no link is heard
   for more than LINK_MAX_MISSED_TICKS, else 0. */
Sint32 btLinkBattleSyncTurn(void)
{
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Uint16		 *peer;
	Uint16		 *signature;
	Sint32		  missed = 0;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Uint16 spare[10];

	if(battle->isLink != 0) {
		peer = LinkPeerSignatures[battle->linkSide ^ 1];
		signature = (Uint16 *)SerialTransfer.payload;
		if(battle->linkPaused != 0)
			return -1;

		/* Publish "ex"/"TU" while both sides still agree on the earlier
		   phase. */
		signature[0] = 'e';
		signature[1] = 'x';
		signature[4] = 'T';
		signature[5] = 'U';
		sySynch(1);
		for(;;) {
			if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
				if(++missed > LINK_MAX_MISSED_TICKS)
					return -1;
			} else {
				missed = 0;
				if(signature[2] != peer[2] || signature[3] != peer[3])
					return -1;
				if(signature[0] == peer[0] && signature[1] == peer[1] &&
					signature[4] == peer[4] && signature[5] == peer[5])
					break;
			}
			sySynch(1);
		}

		/* Advance to "rn" only after the peer has reached "TU". */
		signature[6] = 'r';
		signature[7] = 'n';
		for(;;) {
			if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
				if(++missed > LINK_MAX_MISSED_TICKS)
					return -1;
			} else {
				missed = 0;
				if(signature[4] != peer[4] || signature[5] != peer[5])
					return -1;
				if(signature[6] == peer[6] && signature[7] == peer[7])
					break;
			}
			sySynch(1);
		}

		/* Confirm "rn" before publishing the execution signature. */
		signature[0] = 'E';
		signature[1] = 'X';
		signature[2] = 'E';
		signature[3] = 'C';
		for(;;) {
			if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
				if(++missed > LINK_MAX_MISSED_TICKS)
					return -1;
			} else {
				missed = 0;
				if(signature[6] != peer[6] || signature[7] != peer[7])
					return -1;
				if(signature[0] == peer[0] && signature[1] == peer[1] &&
					signature[2] == peer[2] && signature[3] == peer[3])
					break;
			}
			sySynch(1);
		}

		/* Confirm the execution signature before acknowledging with "tu". */
		signature[4] = 't';
		signature[5] = 'u';
		for(;;) {
			if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
				if(++missed > LINK_MAX_MISSED_TICKS)
					return -1;
			} else {
				missed = 0;
				if(signature[0] != peer[0] || signature[1] != peer[1] ||
					signature[2] != peer[2] || signature[3] != peer[3])
					return -1;
				if(signature[4] == peer[4] && signature[5] == peer[5])
					break;
			}
			sySynch(1);
		}

		/* "RN": wait for the other console to move on from "rn". */
		signature[6] = 'R';
		signature[7] = 'N';
		for(;;) {
			if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
				if(++missed > LINK_MAX_MISSED_TICKS)
					return -1;
			} else {
				missed = 0;
				if(peer[6] != 'r' || peer[7] != 'n')
					break;
			}
			sySynch(1);
		}
	}
	return 0;
}

/* Records the unit id the link opponent knows each party member by. */
Sint32 btPartyAssignOpponentSlots(void)
{
	Uint16		  activeMembers[8];
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Sint32		  partySize = btPartyPrepareMembers(activeMembers);
	Sint32		  memberSlot;
	Sint32		  unitId;

	for(memberSlot = 0; memberSlot < partySize; memberSlot++) {
		unitId = activeMembers[memberSlot];
		battle->opponentSlots[unitId] = memberSlot - FIRST_ENEMY_UNIT;
	}
}

/* Clears the first LINK_SIGNATURE_BYTES of the link transfer's payload, the
   signature halfwords btLinkBattleWaitSync and btLinkBattleSyncTurn write. */
void btClearSerialTransferPayload(void)
{
	syWordsClearEntry(SerialTransfer.payload, LINK_SIGNATURE_BYTES);
}
