// Events: per-tick and VBlank functions in priority order. Part of system.c.

#if !FIX_FUNCTION_SIGNATURES
typedef Sint32 (*EVENT_INT_FUNC)(void);
#endif

// Events: up to EVENT_COUNT functions run each tick by sySynch() or each VBlank, kept sorted by
// their priority (see EVENT_PASS_TICK).

// Empties the event table: every slot gets no function, priority EVENT_PRIORITY_EMPTY and no flags.
void syInitEvent(void)
{
	EVENT *event = EventBuf;
	Sint32 i;

	EventsInitialized = 0;
	EventsRunning     = 0;
	for (i = 0; i < EVENT_COUNT; i++)
	{
		event->callback = 0;
		event->priority = EVENT_PRIORITY_EMPTY;
		event->flags    = 0;
		event++;
	}
	EventsInitialized = 1;
}

// Unused: copies `byteCount` bytes a word at a time.
UNUSED void syCopyEventWords(Uint32 *dest, Uint32 *src, Uint32 byteCount)
{
	Uint32 index;
	byteCount >>= 2;
	for (index = 0; index < byteCount; index++)
		*dest++ = *src++;
}

// Bubble-sorts the event table by priority, highest first, so each pass runs its events from the
// highest priority down; removed events (EVENT_PRIORITY_REMOVED) rise to the front.
void sySortEvents(void)
{
	EVENT  saved;
	// Default: the table's start, which every pass sets again.
	EVENT *event = EventBuf;
	Sint32 pass;
	Sint32 index;

	// BUG: the last pass, which would settle the first two events, never runs, so the events in
	// slots 0 and 1 can stay out of order
#if FIX_TASK_SORT_LAST_PASS
	for (pass = EVENT_COUNT - 1; pass > 0; pass--)
	{
#else
	for (pass = EVENT_COUNT - 1; pass > 1; pass--)
	{
#endif
		event = EventBuf;
		for (index = 0; index < pass; index++)
		{
			if ((Sint16)event[1].priority > (Sint16)event->priority)
			{
				memcpy(&saved, event, sizeof(saved));
				memcpy(event, event + 1, sizeof(saved));
				memcpy(event + 1, &saved, sizeof(saved));
			}
			event++;
		}
	}
}

// Unused: returns the table index of the event running `callback`, or -1.
UNUSED Sint32 syFindEvent(EVENT_FUNC callback)
{
	Sint32 result;
	EVENT *event;
	Uint32 savedInterruptMaster;
	Sint32 i;

	result = -1;
	event  = EventBuf;
	IRQ_LOCK(savedInterruptMaster);
	for (i = 0; i < EVENT_COUNT; i++, event++)
	{
		if (event->callback == (Uint32)callback)
		{
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(savedInterruptMaster);
	return result;
}

// Adds an event running `callback` with priority `priority`, or gives an event already running it
// the new priority, and re-sorts the table.
Sint32 sySetEvent(EVENT_FUNC callback, Sint32 priority)
{
	Uint32 savedInterruptState;
	Sint32 index;
	EVENT *event;
	Sint32 i;

	// Adding re-sorts the table, which would shift the events after the new one if it came during a
	// pass; no event adds one.
	index = -1;
	event = EventBuf;
	// DEAD: nothing uses the status read here; the ROM keeps the load, as The Lost Age's sySetEvent
	// does.
	(void)EventsRunning;
	IRQ_LOCK(savedInterruptState);
	// Update an existing event before looking for an unused event slot.
	for (i = 0; i < EVENT_COUNT; i++, event++)
	{
		if (event->callback == (Uint32)callback)
		{
			event->priority = priority;
			index           = i;
			break;
		}
	}
	event = EventBuf;
	if (index == -1)
	{
		for (i = 0; i < EVENT_COUNT; i++, event++)
		{
			if (event->callback == 0)
			{
				event->callback = (Uint32)callback;
				event->priority = priority;
				event->flags    = 0;
				index           = i;
				break;
			}
		}
	}
	sySortEvents();
	IRQ_UNLOCK(savedInterruptState);
	return index;
}

// Unused: an empty function.
UNUSED void syEventNoOperationA(void)
{
}

// Unused: an empty function.
UNUSED void syEventNoOperationB(void)
{
}

// Removes the event running `callback`: clears its function and gives it priority
// EVENT_PRIORITY_REMOVED, which no pass matches. Returns its old index, or -1.
Sint32 syCloseEvent(EVENT_FUNC callback)
{
	EVENT *event;
	Uint32 savedInterruptMaster;
	Sint32 result;
	Sint32 i;

	result = -1;
	event  = EventBuf;
	IRQ_LOCK(savedInterruptMaster);
	for (i = 0; i < EVENT_COUNT; i++, event++)
	{
		if (event->callback == (Uint32)callback)
		{
			event->callback = 0;
			event->priority = EVENT_PRIORITY_REMOVED;
			result          = i;
			break;
		}
	}
	IRQ_UNLOCK(savedInterruptMaster);
	return result;
}

// Suspends the event running `callback`, or every event when `callback` is null, by setting
// EVENT_SUSPENDED in its priority's high byte, which then matches neither pass.
Sint32 sySuspendEvent(EVENT_FUNC callback)
{
	EVENT *event;
	Uint32 savedInterruptMaster;
	Sint32 result;
	Sint32 i;

	result = -1;
	event  = EventBuf;
	IRQ_LOCK(savedInterruptMaster);
	for (i = 0; i < EVENT_COUNT; i++, event++)
	{
		if (callback != 0)
		{
			if (event->callback != (Uint32)callback)
				continue;
		}
		EVENT_STATE_HIGH(event) |= EVENT_SUSPENDED;
		result                   = i;
	}
	IRQ_UNLOCK(savedInterruptMaster);
	return result;
}

// Suspends every event whose function is in an overlay (EVENT_IN_OVERLAY) unless its
// EVENT_FLAG_KEEP_RUNNING asks to keep it running, as sySuspendEvent does.
Sint32 sySuspendOverlayEvents(void)
{
	EVENT *event;
	Uint32 savedInterruptMaster;
	Sint32 result;
	Sint32 i;

	result = -1;
	event  = EventBuf;
	IRQ_LOCK(savedInterruptMaster);
	for (i = 0; i < EVENT_COUNT; i++, event++)
	{
		// An empty slot (function 0) never matches.
		if (EVENT_IN_OVERLAY(event) && (event->flags & EVENT_FLAG_KEEP_RUNNING) == 0)
		{
			EVENT_STATE_HIGH(event) |= EVENT_SUSPENDED;
			result                   = i;
		}
	}
	IRQ_UNLOCK(savedInterruptMaster);
	return result;
}

// Stores `mask` as the flags of the event running `callback`; EVENT_FLAG_KEEP_RUNNING keeps it
// running while the overlay events are suspended. Returns its index, or -1.
Sint32 sySetEventFlags(EVENT_FUNC callback, Uint32 mask)
{
	EVENT *event;
	Uint32 savedInterruptMaster;
	Sint32 result;
	Sint32 i;

	result = -1;
	event  = EventBuf;
	IRQ_LOCK(savedInterruptMaster);
	for (i = 0; i < EVENT_COUNT; i++, event++)
	{
		if (event->callback == (Uint32)callback)
		{
			event->flags = mask;
			result       = i;
			break;
		}
	}
	IRQ_UNLOCK(savedInterruptMaster);
	return result;
}

// Resumes the event running `callback`, or every event when `callback` is null, by clearing
// EVENT_SUSPENDED in its priority's high byte.
Sint32 syResumeEvent(EVENT_FUNC callback)
{
	EVENT *event;
	Uint32 savedInterruptMaster;
	Sint32 result;
	Sint32 i;

	result = -1;
	event  = EventBuf;
	IRQ_LOCK(savedInterruptMaster);
	for (i = 0; i < EVENT_COUNT; i++, event++)
	{
		if (callback == 0 || event->callback == (Uint32)callback)
		{
			EVENT_STATE_HIGH(event) &= ~EVENT_SUSPENDED;
			result                   = i;
		}
	}
	IRQ_UNLOCK(savedInterruptMaster);
	return result;
}

// Resumes every event whose function is in an overlay, flagged or not. Returns the last index
// changed, or -1.
Sint32 syResumeOverlayEvents(void)
{
	EVENT *event;
	Uint32 savedInterruptMaster;
	Sint32 result;
	Sint32 i;

	result = -1;
	event  = EventBuf;
	IRQ_LOCK(savedInterruptMaster);
	for (i = 0; i < EVENT_COUNT; i++, event++)
	{
		if (EVENT_IN_OVERLAY(event))
		{
			EVENT_STATE_HIGH(event) &= ~EVENT_SUSPENDED;
			result                   = i;
		}
	}
	IRQ_UNLOCK(savedInterruptMaster);
	return result;
}

// Runs one pass of the events: every event whose priority's high byte matches `pass`'s
// (EVENT_PASS_TICK or EVENT_PASS_VBLANK), in table order.
void syExecuteEvent(Sint32 pass)
{
	EVENT *event = EventBuf;
	Sint32 i;

	// A pass matches the high byte only; the low byte has already ranked the events through the
	// sort, and a suspended event's odd high byte matches no pass
	pass >>= 8;
	if (EventsInitialized == 1)
	{
		i = EVENT_COUNT + 1;
		event--;
		for (;;)
		{
			if (--i == 0)
				return;
			event++;
			if (EVENT_STATE_HIGH(event) == pass)
			{
				// UB: events return void, and the stored word is not a pointer object.
#if FIX_FUNCTION_SIGNATURES
				EVENT_FUNC callback = (EVENT_FUNC)event->callback;
#else
				EVENT_INT_FUNC callback = *(EVENT_INT_FUNC *)&event->callback;
#endif

				callback();
			}
		}
	}
}
