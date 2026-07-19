#!/usr/bin/env bun

const ROM_SIZE = 8_388_608;
const SUFFIX = /\[([0-9]{1,3}(?:,[0-9]{3})*) of 8,388,608 bytes\]$/;
const DELTA = /^Source coverage delta: \+([0-9]{1,3}(?:,[0-9]{3})*) bytes\.$/m;

function commas(value: number): string {
  return value.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}

function count(value: string, label: string): number {
  const parsed = Number(value.replaceAll(",", ""));
  if (!Number.isSafeInteger(parsed) || parsed < 0 || commas(parsed) !== value) {
    throw new Error(`${label} is not a canonical byte count`);
  }
  return parsed;
}

function subjectTotal(subject: string): number | undefined {
  const match = subject.match(SUFFIX);
  return match === null ? undefined : count(match[1], "commit total");
}

export function checkCommitProgress(message: string, previousSubject?: string): void {
  const [subject = ""] = message.split(/\r?\n/, 1);
  const total = subjectTotal(subject);
  if (total === undefined) throw new Error("commit subject lacks the cumulative progress suffix");
  if (total > ROM_SIZE) throw new Error("commit total exceeds the canonical ROM size");
  const deltaMatch = message.match(DELTA);
  if (deltaMatch === null) throw new Error("commit body lacks the source coverage delta");
  const delta = count(deltaMatch[1], "commit delta");
  if (previousSubject === undefined) return;
  const previous = subjectTotal(previousSubject);
  if (previous !== undefined && total !== previous + delta) {
    throw new Error(`commit total ${commas(total)} must equal ${commas(previous)} + ${commas(delta)}`);
  }
}

function selfTest(): void {
  checkCommitProgress(
    "assets: test [1,100,000 of 8,388,608 bytes]\n\nSource coverage delta: +100,000 bytes.\n",
    "assets: prior [1,000,000 of 8,388,608 bytes]",
  );
  for (const message of [
    "assets: delta [100,000 of 8,388,608 bytes]\n\nSource coverage delta: +100,000 bytes.\n",
    "assets: malformed [1100000 of 8,388,608 bytes]\n\nSource coverage delta: +100,000 bytes.\n",
    "assets: missing [1,100,000 of 8,388,608 bytes]\n",
  ]) {
    let rejected = false;
    try {
      checkCommitProgress(message, "assets: prior [1,000,000 of 8,388,608 bytes]");
    } catch {
      rejected = true;
    }
    if (!rejected) throw new Error("invalid commit progress was accepted");
  }
}

async function main(args: string[]): Promise<void> {
  if (args[0] === "--self-test") {
    selfTest();
    console.log("self-test=ok");
    return;
  }
  const path = args[0];
  if (path === undefined) throw new Error("usage: check_commit_progress.ts COMMIT_MESSAGE");
  const previous = Bun.spawnSync(["git", "log", "-1", "--format=%s"], { stdout: "pipe", stderr: "pipe" });
  if (previous.exitCode !== 0) throw new Error(previous.stderr.toString().trim() || "cannot read previous commit");
  checkCommitProgress(await Bun.file(path).text(), previous.stdout.toString().trim() || undefined);
}

if (import.meta.main) await main(Bun.argv.slice(2));
