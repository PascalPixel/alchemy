/* Events: per-tick and VBlank functions in priority order. Part of
   system.c. */

/* Empties the event table: every slot gets no function, priority
   EVENT_PRIORITY_EMPTY and no flags. */
void syInitEvent(void)
{
	EVENT *event = EventBuf;
	Sint32 i;

	EventsInitialized = 0;
	EventsRunning = 0;
	for(i = 0; i < EVENT_COUNT; i++) {
		event->func = 0;
		event->pri |= EVENT_PRIORITY_EMPTY;
		event->flg = 0;
		event++;
	}
	EventsInitialized = 1;
}

/* Unused: copies `byteCnt` bytes a word at a time. */
UNUSED void syCopyEventWords(Uint32 *dst, Uint32 *src, Uint32 byteCnt)
{
	Uint32 idx;
	byteCnt >>= 2;
	for(idx = 0; idx < byteCnt; idx++)
		*dst++ = *src++;
}

/* Bubble-sorts the event table by priority, highest first, so each pass runs
   its events from the highest priority down; removed events
   (EVENT_PRIORITY_REMOVED) rise to the front. */
void sySortEvents(void)
{
	EVENT  saved;
	EVENT *event = EventBuf;
	Sint32 pass;
	Sint32 idx;

	/* BUG: the last pass, which would settle the first two events, never runs,
	   so the events in slots 0 and 1 can stay out of order */
#if FIX_TASK_SORT_LAST_PASS
	for(pass = EVENT_COUNT - 1; pass > 0; pass--) {
#else
	for(pass = EVENT_COUNT - 1; pass > 1; pass--) {
#endif
		event = EventBuf;
		for(idx = 0; idx < pass; idx++) {
			if((Sint16)event[1].pri > (Sint16)event->pri) {
				memcpy(&saved, event, sizeof(saved));
				memcpy(event, event + 1, sizeof(saved));
				memcpy(event + 1, &saved, sizeof(saved));
			}
			event++;
		}
	}
}

/* Unused: returns the table index of the event running `func`, or -1. */
UNUSED Sint32 syFindEvent(EVENT_FUNC func)
{
	Sint32 result;
	EVENT *event;
	Uint32 savedIme;
	Sint32 i;

	result = -1;
	event = EventBuf;
	IRQ_LOCK(savedIme);
	for(i = 0; i < EVENT_COUNT; i++, event++) {
		if(event->func == (Uint32)func) {
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Adds an event running `func` with priority `pri`, or gives an event
   already running it the new priority, and re-sorts the table. Returns its
   slot, or -1 when all EVENT_COUNT slots are taken. */
Sint32 sySetEvent(EVENT_FUNC func, Sint32 pri)
{
	Uint32 savedIme;
	Sint32 idx;
	EVENT *event;
	Sint32 i;

	idx = -1;
	event = EventBuf;
	/* DEAD: nothing uses the status read here; the ROM keeps the load. */
	(void)EventsRunning;
	IRQ_LOCK(savedIme);
	/* An event already running func takes the new priority; else a free
	   slot. */
	for(i = 0; i < EVENT_COUNT; i++, event++) {
		if(event->func == (Uint32)func) {
			event->pri = pri;
			idx = i;
			break;
		}
	}
	event = EventBuf;
	if(idx == -1) {
		for(i = 0; i < EVENT_COUNT; i++, event++) {
			if(event->func == 0) {
				event->func = (Uint32)func;
				event->pri = pri;
				event->flg = 0;
				idx = i;
				break;
			}
		}
	}
	sySortEvents();
	IRQ_UNLOCK(savedIme);
	return idx;
}

/* Unused: an empty function. */
UNUSED void syEventNoOperationA(void)
{
}

/* Unused: an empty function. */
UNUSED void syEventNoOperationB(void)
{
}

/* Removes the event running `func`: clears its function and gives it priority
   EVENT_PRIORITY_REMOVED, which no pass matches. Returns its old index, or
   -1. */
Sint32 syCloseEvent(EVENT_FUNC func)
{
	EVENT *event;
	Uint32 savedIme;
	Sint32 result;
	Sint32 i;

	event = EventBuf;
	result = -1;
	IRQ_LOCK(savedIme);
	for(i = 0; i < EVENT_COUNT; i++, event++) {
		if(event->func == (Uint32)func) {
			event->func = 0;
			event->pri = EVENT_PRIORITY_REMOVED;
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Suspends the event running `func`, or every event when `func` is null, by
   setting EVENT_SUSPENDED in its priority's high byte, which then matches
   neither pass. Returns the last slot suspended, or -1. */
Sint32 sySuspendEvent(EVENT_FUNC func)
{
	EVENT *event;
	Uint32 savedIme;
	Sint32 result;
	Sint32 i;

	event = EventBuf;
	result = -1;
	IRQ_LOCK(savedIme);
	for(i = 0; i < EVENT_COUNT; i++, event++) {
		if(func != 0) {
			if(event->func != (Uint32)func)
				continue;
		}
		syIOWrite8(EVENT_STATE_HIGH(event) | EVENT_SUSPENDED,
			&EVENT_STATE_HIGH(event));
		result = i;
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Suspends every event whose function is in an overlay (EVENT_IN_OVERLAY)
   unless its flags hold EVENT_FLAG_KEEP_RUNNING, as sySuspendEvent does. */
Sint32 sySuspendOverlayEvents(void)
{
	EVENT *event;
	Uint32 savedIme;
	Sint32 result;
	Sint32 i;

	event = EventBuf;
	result = -1;
	IRQ_LOCK(savedIme);
	for(i = 0; i < EVENT_COUNT; i++, event++) {
		if(EVENT_IN_OVERLAY(event) &&
			(event->flg & EVENT_FLAG_KEEP_RUNNING) == 0) {
			syIOWrite8(EVENT_STATE_HIGH(event) | EVENT_SUSPENDED,
				&EVENT_STATE_HIGH(event));
			result = i;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Stores `mask` as the flags of the event running `func`
   (EVENT_FLAG_KEEP_RUNNING). Returns its slot, or -1. */
Sint32 sySetEventFlags(EVENT_FUNC func, Uint32 mask)
{
	EVENT *event;
	Uint32 savedIme;
	Sint32 result;
	Sint32 i;

	result = -1;
	event = EventBuf;
	IRQ_LOCK(savedIme);
	for(i = 0; i < EVENT_COUNT; i++, event++) {
		if(event->func == (Uint32)func) {
			event->flg = mask;
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Resumes the event running `func`, or every event when `func` is null, by
   clearing EVENT_SUSPENDED. Returns the last slot resumed, or -1. */
Sint32 syResumeEvent(EVENT_FUNC func)
{
	EVENT *event;
	Uint32 savedIme;
	Sint32 result;
	Sint32 i;

	result = -1;
	event = EventBuf;
	IRQ_LOCK(savedIme);
	for(i = 0; i < EVENT_COUNT; i++, event++) {
		if(func == 0 || event->func == (Uint32)func) {
			EVENT_STATE_HIGH(event) &= ~EVENT_SUSPENDED;
			result = i;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Resumes every event whose function is in an overlay, flagged or not. Returns
   the last index changed, or -1. */
Sint32 syResumeOverlayEvents(void)
{
	EVENT *event;
	Uint32 savedIme;
	Sint32 result;
	Sint32 i;

	result = -1;
	event = EventBuf;
	IRQ_LOCK(savedIme);
	for(i = 0; i < EVENT_COUNT; i++, event++) {
		if(EVENT_IN_OVERLAY(event)) {
			EVENT_STATE_HIGH(event) &= ~EVENT_SUSPENDED;
			result = i;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Runs one pass of the events: every event whose priority's high byte matches
   `pass`'s (EVENT_PASS_VBLANK or EVENT_PASS_TICK), in table order. */
void syExecuteEvent(Sint32 pass)
{
	EVENT *event = EventBuf;
	Sint32 i;

	/* A pass matches the high byte only; the low byte has already ranked the
	   events through the sort, and a suspended event's odd high byte matches no
	   pass. */
	pass >>= 8;
	if(EventsInitialized == 1) {
		i = EVENT_COUNT + 1;
		event--;
		for(;;) {
			if(--i == 0)
				return;
			event++;
			if(EVENT_STATE_HIGH(event) == pass) {
				/* UB: events return void, and the stored word is not a pointer
				   object. */
#if FIX_FUNCTION_SIGNATURES
				EVENT_FUNC func = (EVENT_FUNC)event->func;
#else
				EVENT_INT_FUNC func = *(EVENT_INT_FUNC *)&event->func;
#endif

				func();
			}
		}
	}
}

/* Returns how many events run in pass `pass` (EVENT_PASS_VBLANK or
   EVENT_PASS_TICK); before the event table is set up, the pass's high byte. */
Sint32 syCountEventsInPass(Sint32 pass)
{
	Uint32 cursor;
	EVENT *event;
	Sint32 cnt;
	Sint32 i;
	Uint8 *stat = &EventsInitialized;
	Uint32 observed;

	cnt = 0;
	observed = *stat;
	cursor = (Uint32)EventBuf;
	pass >>= 8;
	if(observed == 1) {
		i = EVENT_COUNT + 1;
		cursor -= sizeof(*event);
		for(;;) {
			if(--i == 0)
				return cnt;
			cursor += sizeof(*event);
			event = (EVENT *)cursor;
			if(EVENT_STATE_HIGH(event) == pass)
				cnt++;
		}
	}
	return pass;
}

/* Runs pass `pass`'s events in table order, up to the first event of the tick
   pass: the VBlank handler's pass, whose events sort before the tick's. */
void syRunPassBeforeTickEvents(Sint32 pass)
{
	EVENT *event = EventBuf;
	Sint32 remaining;

	pass >>= 8;
	if(EventsInitialized == 1) {
		remaining = EVENT_COUNT + 1;
		event--;
		for(;;) {
			if(--remaining == 0)
				return;
			event++;
			if(EVENT_STATE_HIGH(event) == EVENT_PASS_TICK >> 8)
				return;
			if(EVENT_STATE_HIGH(event) == pass) {
				EVENT_INT_FUNC func = *(EVENT_INT_FUNC *)&event->func;

				func();
			}
		}
	}
}
