#ifndef ALCHEMY_TASK_H
#define ALCHEMY_TASK_H

/// @file
/// Tasks, their priorities and the task table, as in The Broken Seal's
/// task.h, with room for 24 tasks.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

enum TaskLimits { TASK_COUNT = 24 };

typedef void (*TaskFunc)(void);

/// A task: its routine's address, its priority (see TASK_PASS_) and its
/// flags, where bit 0 keeps it running while overlay tasks are suspended.
struct Task {
  u32 func;
  u16 priority;
  u8 flags;
  u8 reserved;
};

STATIC_ASSERT(sizeof(struct Task) == 8);
STATIC_ASSERT(offsetof(struct Task, priority) == 4);
STATIC_ASSERT(offsetof(struct Task, flags) == 6);

/// A task's priority: the high byte picks the pass that runs it, the low byte
/// ranks it within the pass (higher runs first). An odd high byte means the
/// task is suspended, so no pass matches it.
///
/// The two passes: the VBlank handler runs TASK_PASS_VBLANK's tasks, and
/// WaitFrames runs TASK_PASS_FRAME's once a frame.
#define TASK_PASS_VBLANK 0x480
#define TASK_PASS_FRAME 0xc80

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->priority)[1])

/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it Task_Add: differ 14 in tbs-en, 59
 * instructions for 61. */
extern volatile u8 gTasksRunning;
extern u8 gTasksInitialized; ///< 1 once the task table is set up
extern struct Task gTasks[];

void Oam_AddSpriteAtDepth(void *entry, s32 priority);

void System_CallWorkEntryWithArgs(s32 arg0, s32 arg1);
void PaletteQueue_Start(void);

void Task_ResetAll(void);
void Task_CopyWords(u32 *destination, u32 *source, u32 byte_count);
void Task_Sort(void);
s32 Task_Find(TaskFunc func);
s32 Task_Add(TaskFunc func, s32 order);
void Task_Idle(void);
void Task_Empty(void);
s32 Task_Remove(TaskFunc func);
s32 Task_SuspendAll(TaskFunc func);
s32 Task_SuspendOverlayTasks(void);
s32 Task_SetFlags(TaskFunc func, u32 mask);
s32 Task_ResumeAll(TaskFunc func);
s32 Task_ResumeOverlayTasks(void);
s32 Menu_ResumeFieldTasks(void);
s32 Menu_SuspendFieldTasks(void);

void Task_RunPass(s32 pass);
s32 Task_CountInPass(s32 pass);
void Task_RunPassBeforeVBlankTasks(s32 pass);

#endif
