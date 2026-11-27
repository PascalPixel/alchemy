/* @file */
/* Tasks: per-frame and VBlank functions in priority order. */
/* Part of system.c. */

/* Empties the task table: every slot gets no function, priority */
/* TASK_PRIORITY_EMPTY and no flags. */
void syInitEvent(void) {
	TASK *task = TasksWork;
	s32 i;

	TasksInitialized = 0;
	TasksRunning = 0;
	for(i = 0; i < TASK_COUNT; i++) {
		task->func = 0;
		task->pri |= TASK_PRIORITY_EMPTY;
		task->flg = 0;
		task++;
	}
	TasksInitialized = 1;
}

/* Unused: copies `byteCount` bytes a word at a time. */
UNUSED void syTaskCopyWords(u32 *destination, u32 *source, u32 byteCount) {
	u32 index;
	byteCount >>= 2;
	for(index = 0; index < byteCount; index++)
		*destination++ = *source++;
}

/* Bubble-sorts the task table by priority, highest first, so each pass runs */
/* its tasks from the highest priority down; removed tasks */
/* (TASK_PRIORITY_REMOVED) rise to the front. */
void syTaskSort(void) {
	TASK saved;
	TASK *task = TasksWork;
	s32 pass;
	s32 index;

	/* BUG: the last pass, which would settle the first two tasks, never runs,
	 */
	/* so the tasks in slots 0 and 1 can stay out of order */
#ifdef BUGFIX
	for(pass = TASK_COUNT - 1; pass > 0; pass--) {
#else
	for(pass = TASK_COUNT - 1; pass > 1; pass--) {
#endif
		task = TasksWork;
		for(index = 0; index < pass; index++) {
			if((s16)task[1].pri > (s16)task->pri) {
				memcpy(&saved, task, sizeof(saved));
				memcpy(task, task + 1, sizeof(saved));
				memcpy(task + 1, &saved, sizeof(saved));
			}
			task++;
		}
	}
}

/* Unused: returns the table index of the task running `func`, or -1. */
UNUSED s32 syTaskFind(TASK_FUNC func) {
	s32 result;
	TASK *task;
	u32 savedIme;
	s32 i;

	result = -1;
	task = TasksWork;
	IRQ_LOCK(savedIme);
	for(i = 0; i < TASK_COUNT; i++, task++) {
		if(task->func == (u32)func) {
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Adds a task running `func` with priority `priority`, or gives a task */
/* already running it the new priority, and re-sorts the table. Returns its */
/* slot, or -1 when all TASK_COUNT slots are taken. */
s32 sySetEvent(TASK_FUNC func, s32 priority) {
	u32 savedIme;
	s32 index;
	TASK *task;
	s32 i;

	index = -1;
	task = TasksWork;
	/* DEAD: nothing uses the status read here; the ROM keeps the load. */
	(void)TasksRunning;
	IRQ_LOCK(savedIme);
	/* A task already running func takes the new priority; else a free slot. */
	for(i = 0; i < TASK_COUNT; i++, task++) {
		if(task->func == (u32)func) {
			task->pri = priority;
			index = i;
			break;
		}
	}
	task = TasksWork;
	if(index == -1) {
		for(i = 0; i < TASK_COUNT; i++, task++) {
			if(task->func == 0) {
				task->func = (u32)func;
				task->pri = priority;
				task->flg = 0;
				index = i;
				break;
			}
		}
	}
	syTaskSort();
	IRQ_UNLOCK(savedIme);
	return index;
}

/* Unused: an empty function. */
UNUSED void syTaskIdle(void) {}

/* Unused: an empty function. */
UNUSED void syTaskEmpty(void) {}

/* Removes the task running `func`: clears its function and gives it priority */
/* TASK_PRIORITY_REMOVED, which no pass matches. Returns its old index, or -1.
 */
s32 syCloseEvent(TASK_FUNC func) {
	TASK *task;
	u32 savedIme;
	s32 result;
	s32 i;

	task = TasksWork;
	result = -1;
	IRQ_LOCK(savedIme);
	for(i = 0; i < TASK_COUNT; i++, task++) {
		if(task->func == (u32)func) {
			task->func = 0;
			task->pri = TASK_PRIORITY_REMOVED;
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Suspends the task running `func`, or every task when `func` is null, by */
/* setting TASK_SUSPENDED in its priority's high byte, which then matches */
/* neither pass. Returns the last slot suspended, or -1. */
s32 syTaskSuspend(TASK_FUNC func) {
	TASK *task;
	u32 savedIme;
	s32 result;
	s32 i;

	task = TasksWork;
	result = -1;
	IRQ_LOCK(savedIme);
	for(i = 0; i < TASK_COUNT; i++, task++) {
		if(func != 0) {
			if(task->func != (u32)func)
				continue;
		}
		syIoWrite8(TASK_STATE_HIGH(task) | TASK_SUSPENDED,
				   &TASK_STATE_HIGH(task));
		result = i;
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Suspends every task whose function is in an overlay (TASK_IN_OVERLAY) */
/* unless its flags hold TASK_FLAG_KEEP_RUNNING, as syTaskSuspend does. */
s32 syTaskSuspendOverlayTasks(void) {
	TASK *task;
	u32 savedIme;
	s32 result;
	s32 i;

	task = TasksWork;
	result = -1;
	IRQ_LOCK(savedIme);
	for(i = 0; i < TASK_COUNT; i++, task++) {
		if(TASK_IN_OVERLAY(task) && (task->flg & TASK_FLAG_KEEP_RUNNING) == 0) {
			syIoWrite8(TASK_STATE_HIGH(task) | TASK_SUSPENDED,
					   &TASK_STATE_HIGH(task));
			result = i;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Stores `mask` as the flags of the task running `func` */
/* (TASK_FLAG_KEEP_RUNNING). Returns its slot, or -1. */
s32 syTaskSetFlags(TASK_FUNC func, u32 mask) {
	TASK *task;
	u32 savedIme;
	s32 result;
	s32 i;

	result = -1;
	task = TasksWork;
	IRQ_LOCK(savedIme);
	for(i = 0; i < TASK_COUNT; i++, task++) {
		if(task->func == (u32)func) {
			task->flg = mask;
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Resumes the task running `func`, or every task when `func` is null, by */
/* clearing TASK_SUSPENDED. Returns the last slot resumed, or -1. */
s32 syTaskResume(TASK_FUNC func) {
	TASK *task;
	u32 savedIme;
	s32 result;
	s32 i;

	result = -1;
	task = TasksWork;
	IRQ_LOCK(savedIme);
	for(i = 0; i < TASK_COUNT; i++, task++) {
		if(func == 0 || task->func == (u32)func) {
			TASK_STATE_HIGH(task) &= ~TASK_SUSPENDED;
			result = i;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Resumes every task whose function is in an overlay, flagged or not. Returns
 */
/* the last index changed, or -1. */
s32 syTaskResumeOverlayTasks(void) {
	TASK *task;
	u32 savedIme;
	s32 result;
	s32 i;

	result = -1;
	task = TasksWork;
	IRQ_LOCK(savedIme);
	for(i = 0; i < TASK_COUNT; i++, task++) {
		if(TASK_IN_OVERLAY(task)) {
			TASK_STATE_HIGH(task) &= ~TASK_SUSPENDED;
			result = i;
		}
	}
	IRQ_UNLOCK(savedIme);
	return result;
}

/* Runs one pass of the tasks: every task whose priority's high byte matches */
/* `pass`'s (TASK_PASS_VBLANK or TASK_PASS_FRAME), in table order. */
void syExecuteEvent(s32 pass) {
	TASK *task = TasksWork;
	s32 i;

	/* A pass matches the high byte only; the low byte has already ranked the */
	/* tasks through the sort, and a suspended task's odd high byte matches no
	 */
	/* pass. */
	pass >>= 8;
	if(TasksInitialized == 1) {
		i = TASK_COUNT + 1;
		task--;
		for(;;) {
			if(--i == 0)
				return;
			task++;
			if(TASK_STATE_HIGH(task) == pass) {
				/* UB: tasks return void, and the stored word is not a pointer
				 * object. */
#ifdef UBFIX
				TASK_FUNC func = (TASK_FUNC)task->func;
#else
				TASK_INT_FUNC func = *(TASK_INT_FUNC *)&task->func;
#endif

				func();
			}
		}
	}
}

/* Returns how many tasks run in pass `pass` (TASK_PASS_VBLANK or */
/* TASK_PASS_FRAME); before the task table is set up, the pass's high byte. */
s32 syTaskCountInPass(s32 pass) {
	u32 cursor;
	TASK *task;
	s32 count;
	s32 i;
	u8 *status = &TasksInitialized;
	u32 observed;

	count = 0;
	observed = *status;
	cursor = (u32)TasksWork;
	pass >>= 8;
	if(observed == 1) {
		i = TASK_COUNT + 1;
		cursor -= sizeof(*task);
		for(;;) {
			if(--i == 0)
				return count;
			cursor += sizeof(*task);
			task = (TASK *)cursor;
			if(TASK_STATE_HIGH(task) == pass)
				count++;
		}
	}
	return pass;
}

/* Runs pass `pass`'s tasks in table order, up to the first task of the frame */
/* pass: the VBlank handler's pass, whose tasks sort before the frame's. */
void syTaskRunPassBeforeFrameTasks(s32 pass) {
	TASK *task = TasksWork;
	s32 remaining;

	pass >>= 8;
	if(TasksInitialized == 1) {
		remaining = TASK_COUNT + 1;
		task--;
		for(;;) {
			if(--remaining == 0)
				return;
			task++;
			if(TASK_STATE_HIGH(task) == TASK_PASS_FRAME >> 8)
				return;
			if(TASK_STATE_HIGH(task) == pass) {
				TASK_INT_FUNC func = *(TASK_INT_FUNC *)&task->func;

				func();
			}
		}
	}
}
