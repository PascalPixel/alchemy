#!/usr/bin/env bun
// Enforce the canonical Full-C Byte Share suffix against the metric report
// staged in Git's index. The working tree is deliberately not consulted.
import { DEFAULT_TARGET, parseDecompTarget, type DecompTargetId } from "../lib/decomp_targets.ts";
import { currentProgress, formatSubject, kilobytes, parseSubject } from "../lib/full_c_progress.ts";
import { canonicalJson } from "../lib/canonical_json.ts";

interface MetricReport {
  format: 1;
  metric: "full-c-byte-share";
  target: DecompTargetId;
  full_c_bytes: number;
  executable_bytes: number;
  remaining_bytes: number;
  audit: "complete";
}

const LEGACY_REGION = /\[[0-9]{1,3}(?:,[0-9]{3})* of [0-9]{1,3}(?:,[0-9]{3})*\]$/;
const LEGACY_C_BYTES_AND_REGION =
  /\[[0-9]{1,3}(?:,[0-9]{3})* C bytes\]\s+\[[0-9]{1,3}(?:,[0-9]{3})* of [0-9]{1,3}(?:,[0-9]{3})*\]$/;
const LEGACY_ROM_BYTES = /\[[0-9,]+ of 8,388,608 bytes\]$/;
// The byte-exact `[C n/total bytes]` form this suffix carried before the
// kilobyte `[ ☀️ n / total ]` rendering. Every commit up to the changeover has
// it, so it must remain a recognized parent or the first new-format commit
// would look like it followed an unhooked one.
const LEGACY_FULL_C_BYTES = /\[C [0-9]{1,3}(?:,[0-9]{3})*\/[0-9]{1,3}(?:,[0-9]{3})* bytes\]$/;
const DENOMINATOR_CORRECTION = /^metrics: correct executable denominator\b/;
// Recognized transition for resuming the counter chain after commits that
// bypassed this hook (hooks are opt-in per clone, so an unhooked contributor
// can leave any number of unsuffixed parents). The restore commit itself must
// carry a correct current suffix; everything after it chains normally. This
// is deliberately an explicit, greppable subject form rather than silent
// tolerance: the history should show where and why the chain restarted.
const COUNTER_RESTORE = /^metrics: restore Full-C counter chain\b/;
// Recognized form for a DELIBERATE decrease. Reverting work that does not
// verify legitimately removes exact bytes, and until now the chain had no way
// to say so: the only escapes were a denominator correction (wrong meaning) or
// a chain restore (only valid after an unsuffixed parent). Silent tolerance is
// not acceptable here either, so this is an explicit, greppable subject form
// carrying the true post-revert suffix, exactly like COUNTER_RESTORE. The
// subject must still state the real number; this waives the monotonicity rule,
// not the accuracy rule.
const COUNTER_REVERT = /^metrics: revert reduces Full-C\b/;

function legacySubject(subject: string): boolean {
  return LEGACY_C_BYTES_AND_REGION.test(subject) ||
    LEGACY_REGION.test(subject) ||
    LEGACY_ROM_BYTES.test(subject) ||
    LEGACY_FULL_C_BYTES.test(subject);
}

function validatedReport(value: unknown, target: DecompTargetId): MetricReport {
  const report = value as Partial<MetricReport>;
  if (report.format !== 1 || report.metric !== "full-c-byte-share" ||
      report.target !== target || report.audit !== "complete") {
    throw new Error(`staged ${target} Full-C report is missing, incomplete, or has the wrong format`);
  }
  for (const [label, count] of [
    ["full_c_bytes", report.full_c_bytes],
    ["executable_bytes", report.executable_bytes],
    ["remaining_bytes", report.remaining_bytes],
  ] as const) {
    if (!Number.isSafeInteger(count) || count! < 0) throw new Error(`staged report ${label} is invalid`);
  }
  if (report.full_c_bytes! > report.executable_bytes! ||
      report.full_c_bytes! + report.remaining_bytes! !== report.executable_bytes) {
    throw new Error("staged report arithmetic is invalid");
  }
  return report as MetricReport;
}

export function checkCommitProgress(
  message: string,
  reportValue: unknown,
  previousSubject?: string,
  expectedDenominator?: number,
): void {
  const [subject = ""] = message.split(/\r?\n/, 1);
  const report = validatedReport(reportValue, (reportValue as MetricReport).target);
  const parsed = parseSubject(subject);
  if (!parsed) {
    throw new Error("commit subject must end with " +
      formatSubject(report.full_c_bytes, report.executable_bytes));
  }
  if (parsed.fullCKilobytes !== kilobytes(report.full_c_bytes) ||
      parsed.executableKilobytes !== kilobytes(report.executable_bytes)) {
    throw new Error(
      `commit suffix is stale; expected ${formatSubject(report.full_c_bytes, report.executable_bytes)}`,
    );
  }
  if (expectedDenominator !== undefined && parsed.executableKilobytes !== kilobytes(expectedDenominator)) {
    throw new Error(`executable denominator changed from ${expectedDenominator} to ${parsed.executableKilobytes}`);
  }
  if (previousSubject === undefined) return;
  const previous = parseSubject(previousSubject);
  if (previous) {
    if (parsed.executableKilobytes !== previous.executableKilobytes) {
      if (!DENOMINATOR_CORRECTION.test(subject)) {
        throw new Error(
          `executable denominator changed from ${previous.executableKilobytes} to ${parsed.executableKilobytes}; ` +
          "use an explicit metrics: correct executable denominator commit",
        );
      }
    }
    if (parsed.fullCKilobytes < previous.fullCKilobytes) {
      if (COUNTER_REVERT.test(subject)) return;
      throw new Error(
        `Full-C bytes regressed from ${previous.fullCKilobytes} to ${parsed.fullCKilobytes}; ` +
          "if this is a deliberate revert of work that does not verify, say so with a " +
          "'metrics: revert reduces Full-C' commit carrying the true current suffix",
      );
    }
    return;
  }
  if (!legacySubject(previousSubject)) {
    if (COUNTER_RESTORE.test(subject)) return;
    throw new Error(
      "previous commit has neither canonical Full-C nor recognized transition syntax; " +
      "resume the chain with a 'metrics: restore Full-C counter chain' commit carrying the current suffix",
    );
  }
}

function git(args: string[]): string {
  const process = Bun.spawnSync(["git", ...args], { stdout: "pipe", stderr: "pipe" });
  if (process.exitCode !== 0) {
    throw new Error(process.stderr.toString().trim() || `git ${args.join(" ")} failed`);
  }
  return process.stdout.toString();
}

function stagedReport(target: DecompTargetId): MetricReport {
  const path = `metrics/${target}-progress.json`;
  let text: string;
  try {
    text = git(["show", `:${path}`]);
  } catch {
    throw new Error(`stage ${path} before committing`);
  }
  return validatedReport(JSON.parse(text), target);
}

/**
 * True when the committed content of the derived progress report already
 * matches what regenerating it right now would produce.
 *
 * `git show :path` reads the INDEX -- the exact bytes this commit will carry --
 * so a report that is already correct passes whether or not it was touched,
 * and a report that would change fails unless the new content is staged.
 */
function reportIsCurrent(
  target: DecompTargetId,
  readCommitted: (path: string) => string | null = (path) => {
    try {
      return git(["show", `:${path}`]);
    } catch {
      return null; // not tracked, or not in the index at all
    }
  },
  regenerate: (target: DecompTargetId) => string =
    (id) => canonicalJson(currentProgress(id)),
): boolean {
  const committed = readCommitted(`metrics/${target}-progress.json`);
  if (committed === null) return false;
  return committed.trim() === regenerate(target).trim();
}

function stagedPaths(): string[] {
  const unmerged = git(["ls-files", "-u"]).trim();
  if (unmerged) throw new Error("cannot validate Full-C progress with unmerged index entries");
  return git(["diff", "--cached", "--name-only", "--diff-filter=ACMRT"])
    .split(/\r?\n/).filter(Boolean);
}

function reportRequired(paths: readonly string[], target: DecompTargetId): boolean {
  const registryRoots = target === "gs1-en"
    ? /^(?:src\/|asm\/|include\/|assets\/code\/resource_.+_(?:overlay\.s|c_[0-9a-f]{8}\.c)$|metrics\/gs1-en-executable\.json$)/
    : /^(?:games\/gs2\/(?:src|asm|include)\/|metrics\/gs2-en-executable\.json$)/;
  return paths.some((path) => registryRoots.test(path));
}

function selfTest(): void {
  const report: MetricReport = {
    format: 1,
    metric: "full-c-byte-share",
    target: "gs1-en",
    full_c_bytes: 123456,
    executable_bytes: 1234567,
    remaining_bytes: 1111111,
    audit: "complete",
  };
  checkCommitProgress(
    "metrics: transition [ ☀️ 123 / 1,234 ]",
    report,
    "src: prior [95,358 C bytes] [1,368 of 2,000]",
  );
  // The byte-exact predecessor form is a recognized parent, so the changeover
  // commit chains normally instead of needing restore syntax.
  checkCommitProgress(
    "metrics: kilobyte suffix [ \u2600\ufe0f 123 / 1,234 ]",
    report,
    "src: prior [C 316,342/1,345,860 bytes]",
  );
  checkCommitProgress(
    "docs: repeat [ ☀️ 123 / 1,234 ]",
    report,
    "metrics: prior [ ☀️ 123 / 1,234 ]",
  );
  const invalid: Array<[string, MetricReport, string?]> = [
    ["missing suffix", report],
    ["bad commas [ ☀️ 1234 / 1,234 ]", report],
    ["missing unit [ ☀️ 123 / 1,234 bytes]", report],
    ["spaces [ ☀️ 123/1,234 ]", report],
    ["stale [ ☀️ 122 / 1,234 ]", report],
    ["wrong total [ ☀️ 123 / 1,235 ]", report],
    ["regress [ ☀️ 123 / 1,234 ]", report,
      "prior [ ☀️ 124 / 1,234 ]"],
    ["post-transition legacy [123 of 456]", report,
      "prior [ ☀️ 123 / 1,234 ]"],
  ];
  for (const [message, metric, previous] of invalid) {
    let rejected = false;
    try {
      checkCommitProgress(message, metric, previous);
    } catch {
      rejected = true;
    }
    if (!rejected) throw new Error(`invalid commit progress was accepted: ${message}`);
  }
  const stale = { ...report, full_c_bytes: 1, remaining_bytes: 1234566 };
  let rejected = false;
  try {
    checkCommitProgress("stale report [ ☀️ 123 / 1,234 ]", stale);
  } catch {
    rejected = true;
  }
  if (!rejected) throw new Error("staged/report mismatch was accepted");
  checkCommitProgress(
    "metrics: correct executable denominator after boundary audit [ ☀️ 123 / 1,235 ]",
    { ...report, executable_bytes: 1235567, remaining_bytes: 1112111 },
    "prior [ ☀️ 123 / 1,234 ]",
  );
  // --- the report-currency rule, both branches -------------------------------
  // Intent: the derived report must not be STALE. Not: it must appear in every
  // commit. A change that touches executable source without altering compiled
  // output regenerates to an identical file, so there is nothing to stage.
  const freshReport = '{"full_c_bytes":1}';
  const staleReport = '{"full_c_bytes":0}';
  if (!reportIsCurrent("gs1-en", () => freshReport, () => freshReport)) {
    throw new Error("self-test: an already-current report was rejected");
  }
  // Trailing-whitespace differences are not staleness.
  if (!reportIsCurrent("gs1-en", () => freshReport + "\n", () => freshReport)) {
    throw new Error("self-test: whitespace-only difference treated as stale");
  }
  if (reportIsCurrent("gs1-en", () => staleReport, () => freshReport)) {
    throw new Error("self-test: a stale report was accepted");
  }
  // A report absent from the index is stale, not current.
  if (reportIsCurrent("gs1-en", () => null, () => freshReport)) {
    throw new Error("self-test: a missing report was accepted");
  }

  // --- an unsuffixed parent subject is REPORTED, never a crash ---------------
  // parseSubject returns undefined for a subject carrying no counter. main()'s
  // denominator guard once compared that against null, so `undefined !== null`
  // was true and the property read threw a TypeError before the designed
  // diagnostic could fire -- which blocked every commit after such a parent.
  if (parseSubject("docs") !== undefined) {
    throw new Error("self-test: parseSubject no longer returns undefined for an unsuffixed subject");
  }
  let unsuffixedRejected = false;
  try {
    checkCommitProgress("subject [ ☀️ 123 / 1,234 ]", report, "docs");
  } catch (error) {
    unsuffixedRejected = true;
    if (error instanceof TypeError) {
      throw new Error("self-test: unsuffixed parent subject crashed instead of being reported");
    }
  }
  if (!unsuffixedRejected) throw new Error("self-test: unsuffixed parent subject was accepted");
  // The explicit restore syntax is the one sanctioned way past such a parent,
  // and it must still carry a correct current suffix itself.
  checkCommitProgress(
    "metrics: restore Full-C counter chain [ ☀️ 123 / 1,234 ]",
    report,
    "docs",
  );
  // A deliberate revert may legitimately lower the counter. It is accepted only
  // with the explicit subject form, and only while still stating the true
  // number: the waiver covers monotonicity, never accuracy.
  checkCommitProgress(
    "metrics: revert reduces Full-C after backing out unverifiable work [ ☀️ 123 / 1,234 ]",
    report,
    "src: prior [ ☀️ 200 / 1,234 ]",
  );
  let bareRegressionRejected = false;
  try {
    checkCommitProgress(
      "src: quiet regression [ ☀️ 123 / 1,234 ]",
      report,
      "src: prior [ ☀️ 200 / 1,234 ]",
    );
  } catch { bareRegressionRejected = true; }
  if (!bareRegressionRejected) throw new Error("self-test: unannounced Full-C regression was accepted");
  let staleRevertRejected = false;
  try {
    checkCommitProgress(
      "metrics: revert reduces Full-C [ ☀️ 122 / 1,234 ]",
      report,
      "src: prior [ ☀️ 200 / 1,234 ]",
    );
  } catch { staleRevertRejected = true; }
  if (!staleRevertRejected) throw new Error("self-test: revert syntax accepted a wrong suffix");
  let staleRestoreRejected = false;
  try {
    checkCommitProgress(
      "metrics: restore Full-C counter chain [ ☀️ 122 / 1,234 ]",
      report,
      "docs",
    );
  } catch {
    staleRestoreRejected = true;
  }
  if (!staleRestoreRejected) throw new Error("self-test: restore commit with a stale suffix was accepted");
  console.log("self-test=ok suffix=full-c-byte-share");
}

async function main(argv: string[]): Promise<void> {
  if (argv[0] === "--self-test") return selfTest();
  if (argv.includes("--help") || argv.includes("-h")) {
    console.log("usage: check_commit_progress.ts [--target TARGET] COMMIT_MESSAGE");
    return;
  }
  let target: DecompTargetId = DEFAULT_TARGET;
  const targetIndex = argv.indexOf("--target");
  if (targetIndex >= 0) {
    target = parseDecompTarget(argv[targetIndex + 1]);
    argv.splice(targetIndex, 2);
  }
  const path = argv[0];
  if (!path) throw new Error("usage: check_commit_progress.ts [--target TARGET] COMMIT_MESSAGE");
  const paths = stagedPaths();
  const reportPath = `metrics/${target}-progress.json`;
  // The rule is that the derived report must not be STALE -- not that it must
  // appear in every commit. Those differ for a change that touches executable
  // source without altering compiled output (a comment added to an exact `src/`
  // file, say): regenerating produces an identical file, so there is nothing
  // for git to stage and the old "must be staged" form was unsatisfiable.
  //
  // So: regenerate, and require no diff against what this commit will contain.
  // If regeneration changes the report it must be staged; if it changes
  // nothing, the requirement is already met.
  if (reportRequired(paths, target) && !reportIsCurrent(target)) {
    throw new Error(
      `${reportPath} is stale: regenerate it (bun tools/lib/full_c_progress.ts --write-report) ` +
      "and stage it with executable-source changes",
    );
  }
  const report = stagedReport(target);
  const previous = git(["log", "-1", "--format=%s"]).trim() || undefined;
  const previousMetric = previous === undefined ? null : parseSubject(previous);
  // parseSubject returns undefined for a subject that carries no counter (a
  // commit that bypassed this hook). Guard both empties: one such commit on the
  // branch would otherwise crash the hook for every commit that follows it.
  if (previousMetric != null && kilobytes(report.executable_bytes) !== previousMetric.executableKilobytes &&
      !paths.includes(`metrics/${target}-executable.json`)) {
    throw new Error(`denominator correction requires staged metrics/${target}-executable.json`);
  }
  checkCommitProgress(await Bun.file(path).text(), report, previous);
}

if (import.meta.main) await main(Bun.argv.slice(2));
