from pathlib import Path
import subprocess
import sys
import termios
import tty

project_root = Path(__file__).parent.parent.resolve()
binary = project_root / "build"

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
    if 'KeyboardInterrupt' in str(sys.exc_info()[0]):
        sys.exit(130)