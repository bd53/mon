import subprocess
import sys
import termios
import tty
from pathlib import Path

binary = Path("build/mon")

if not binary.exists():
    print(f"Error: {binary} does not exist. Build first.")
    sys.exit(1)

fd = sys.stdin.fileno()
old_settings = termios.tcgetattr(fd)

try:
    subprocess.run([str(binary)], check=True)
except subprocess.CalledProcessError as e:
    print(f"Program exited with error code {e.returncode}.")
    sys.exit(e.returncode)
except KeyboardInterrupt:
    pass
finally:
    termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    sys.exit(130) if 'KeyboardInterrupt' in str(sys.exc_info()[0]) else None