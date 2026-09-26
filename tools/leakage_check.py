from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]

SKIP_DIRS = {
    ".git",
    "build",
    ".cache",
    "__pycache__",
}

TEXT_EXTENSIONS = {
    ".c", ".h", ".cpp", ".hpp",
    ".py", ".txt", ".md",
    ".cmake", ".yml", ".yaml",
    ".json", ".toml"
}

SECRET_PATTERNS = [
    re.compile(r"-----BEGIN [A-Z ]*PRIVATE KEY-----"),
    re.compile(r"ghp_[A-Za-z0-9]{20,}"),
    re.compile(r"github_pat_[A-Za-z0-9_]{20,}"),
    re.compile(r"sk-[A-Za-z0-9_-]{20,}"),
    re.compile(r"AKIA[0-9A-Z]{16}"),
    re.compile(r"(?i)authorization\s*:\s*bearer\s+\S+"),
    re.compile(r"(?i)(password|passwd|secret|api[_-]?key)\s*=\s*['\"][^'\"]+['\"]"),
]

violations = []

for path in ROOT.rglob("*"):
    if not path.is_file():
        continue

    if any(part in SKIP_DIRS for part in path.parts):
        continue

    if path.suffix.lower() not in TEXT_EXTENSIONS and path.name != "CMakeLists.txt":
        continue

    try:
        text = path.read_text(encoding="utf-8")
    except UnicodeDecodeError:
        continue

    for pattern in SECRET_PATTERNS:
        if pattern.search(text):
            violations.append(
                f"{path.relative_to(ROOT)} : possible sensitive data"
            )

if violations:
    print("MOBIUS leakage check: FAIL")
    for item in violations:
        print(" -", item)
    sys.exit(1)

print("MOBIUS leakage check: PASS")