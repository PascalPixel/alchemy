// Tool role: library; imported by tools/build_assets.ts, tools/compose_scene.ts, tools/f0_archive.ts (+3 more).
import { existsSync, readdirSync, statSync, unlinkSync } from "node:fs";
import { basename, extname, isAbsolute, relative, resolve } from "node:path";

export const IMAGE_SUFFIXES = [".png", ".jpg", ".jpeg", ".gif", ".bmp", ".webp"] as const;

function globRegex(pattern: string): RegExp {
  const escaped = pattern.replace(/[.+^${}()|[\]\\]/g, "\\$&").replace(/\*/g, ".*").replace(/\?/g, ".");
  return new RegExp(`^${escaped}$`);
}

export function prune_files(directory: string, pattern: string, keep: Iterable<string>): string[] {
  const expected = new Set(Array.from(keep, (name) => basename(name)));
  const removed: string[] = [];
  if (!existsSync(directory)) return removed;
  const regex = globRegex(pattern);
  for (const name of readdirSync(directory).sort()) {
    const path = resolve(directory, name);
    if (regex.test(name) && statSync(path).isFile() && !expected.has(name)) {
      unlinkSync(path);
      removed.push(path);
    }
  }
  return removed;
}

export interface AssetRegion { sources?: string[] }

export function unused_tracked_images(
  root: string,
  regions: Iterable<AssetRegion>,
  ignoredPrefixes: Iterable<string> = [],
): string[] {
  root = resolve(root);
  if (!existsSync(resolve(root, ".git"))) return [];
  const process = Bun.spawnSync(["git", "ls-files", "-z", "--", ...IMAGE_SUFFIXES.map((suffix) => `*${suffix}`)], { cwd: root });
  if (process.exitCode !== 0) throw new Error(process.stderr.toString());
  const tracked = new Set(process.stdout.toString().split("\0").filter(Boolean).map((name) => name.replaceAll("\\", "/")));
  const sources = new Set<string>();
  for (const region of regions) for (const name of region.sources ?? []) {
    if (!IMAGE_SUFFIXES.includes(extname(name).toLowerCase() as typeof IMAGE_SUFFIXES[number])) continue;
    sources.add((isAbsolute(name) ? relative(root, resolve(name)) : name).replaceAll("\\", "/"));
  }
  const ignored = Array.from(ignoredPrefixes, (prefix) => prefix.replaceAll("\\", "/"));
  return Array.from(tracked).filter((name) =>
    !sources.has(name) && !ignored.some((prefix) => name.startsWith(prefix)) && existsSync(resolve(root, name)),
  ).sort();
}
