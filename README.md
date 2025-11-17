# procctrl

**procctrl** is a simple, **cross-platform, header-only C++ library** to manage **process suspension and resumption**. It supports **Windows** and **Linux** (macOS is not supported).  

---

## Features

- Suspend or resume a process by **PID**
- Suspend or resume all processes by **executable name**
- Check if a process **exists** and if it can be **controlled**
- Get **parent PID** or **all descendants** (process tree)
- Automatically handles **sandboxed applications** on Linux (e.g., Snap/Flatpak)
- Pure **header-only**, no compilation required

---

## How It Works

### Windows
- Uses **NT API functions** (`NtSuspendProcess` / `NtResumeProcess`) from `ntdll.dll`
- Attempts to enable **SeDebugPrivilege** to control other users' processes
- Falls back gracefully if permissions are insufficient

### Linux
- Non-sandboxed apps: sends `SIGSTOP` (suspend) and `SIGCONT` (resume)
- Sandboxed apps in **cgroup v2**: writes `1`/`0` to `cgroup.freeze` for freezing/thawing
- Traverses `/proc` to find processes and parent/child relationships
- Supports cgroup-aware suspension to prevent partial freezes for multi-process applications

---

## Installation

Simply **include the header** in your project:

```cpp
#include "procctrl.hpp"
