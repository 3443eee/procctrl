#include "procctrl.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <pid>\n";
        std::cerr << "Example: " << argv[0] << " 1234\n";
        return 1;
    }
    
    // Parse PID from argument
    unsigned long pid_value = std::strtoul(argv[1], nullptr, 10);
    
#ifdef _WIN32
    DWORD pid = static_cast<DWORD>(pid_value);
#else
    pid_t pid = static_cast<pid_t>(pid_value);
#endif
    
    if (pid <= 0) {
        std::cerr << "Error: Invalid PID\n";
        return 1;
    }
    
    if (!procctrl::process_exists(pid)) {
        std::cerr << "Error: Process " << pid_value << " does not exist\n";
        return 1;
    }
    
    std::cout << "Freezing process " << pid_value << "...\n";
    
    if (procctrl::set_process_suspended(pid, true)) {
        std::cout << "Successfully froze process " << pid_value << "\n";
#ifdef _WIN32
        std::cout << "To unfreeze, run this program again or use NtResumeProcess\n";
#else
        std::cout << "To unfreeze, run this program again or send SIGCONT\n";
#endif
        return 0;
    } else {
        std::cerr << "Failed to freeze process " << pid_value << "\n";
        return 1;
    }
}
