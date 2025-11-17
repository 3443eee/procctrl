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
```

No compilation or linking is required.

---

## Usage Example

```cpp
#include "procctrl.hpp"
#include <iostream>

int main() {
    auto pid = procctrl::find_process_by_name("firefox");
    if (pid != -1) {
        procctrl::set_process_suspended(pid, true);  // suspend
        std::cout << "Process suspended!" << std::endl;
        procctrl::set_process_suspended(pid, false); // resume
        std::cout << "Process resumed!" << std::endl;
    }
}
```

You can also suspend or resume all processes by name:

```cpp
int suspended_count = procctrl::suspend_processes_by_name("firefox");
int resumed_count = procctrl::resume_processes_by_name("firefox");
```

---

## Functions

### Windows-only helpers
- `bool enable_debug_privilege()`  
  Enables `SeDebugPrivilege` to control processes owned by other users.

- `void init_nt_functions()`  
  Loads NT functions (`NtSuspendProcess` / `NtResumeProcess`) from `ntdll.dll` and enables debug privileges.

### Linux-only helpers
- `bool is_cgroup_v2_available()`  
  Checks if cgroup v2 is mounted on the system.

- `int get_max_pid()`  
  Returns the maximum PID value configured on the system (default 32768 if unavailable).

- `std::string get_cgroup_v2_path(pid_t pid)`  
  Returns the cgroup v2 path for a process, or an empty string if unavailable.

### Cross-platform functions
- `bool process_exists(pid_t pid)`  
  Returns true if the process exists.

- `bool can_control_process(pid_t pid)`  
  Returns true if the current user can suspend/resume the process.

- `pid_t find_process_by_name(const std::string& exe_name)`  
  Returns the PID of the first process with the given executable name, or -1 if not found.

- `std::vector<pid_t> find_all_processes_by_name(const std::string& exe_name)`  
  Returns all PIDs matching the executable name.

- `pid_t get_parent_pid(pid_t pid)`  
  Returns the parent PID of the given process, or -1 on error.

- `bool set_process_suspended(pid_t pid, bool suspend)`  
  Suspends (`true`) or resumes (`false`) a single process by PID. Handles sandboxed apps and cgroups on Linux.

- `int suspend_processes_by_name(const std::string& exe_name)`  
  Suspends all processes with the given name. On Linux, only freezes each cgroup once. Returns the number successfully suspended.

- `int resume_processes_by_name(const std::string& exe_name)`  
  Resumes all processes with the given name. On Linux, only thaws each cgroup once. Returns the number successfully resumed.

- `std::vector<pid_t> get_process_tree(pid_t root_pid)`  
  Returns a vector containing the root PID and all descendant PIDs.

---

## License

MIT License. See `LICENSE` file.

---

## Credits

Based on work from:  
- [craftwar/suspend](https://github.com/craftwar/suspend)  
- [Spencer-Macro-Utilities](https://github.com/Spencer0187/Spencer-Macro-Utilities/tree/main/visual%20studio/Resource%20Files/Suspend_Input_Helper_Source)

