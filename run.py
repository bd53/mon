import subprocess
import sys
import os
from pathlib import Path

guided = Path("scripts/guided.py")

if not guided.exists():
    print(f"Error: {guided} does not exist.")
    sys.exit(1)

try:
    os.execv(sys.executable, [sys.executable, str(guided)])
except KeyboardInterrupt:
    sys.exit(130)