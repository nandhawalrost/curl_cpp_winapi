#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::string readFileAsString(const std::string& filePath) {
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filePath << std::endl;
        return "";  // Return an empty string if the file fails to open
    }

    std::stringstream buffer;
    buffer << file.rdbuf();  // Read the file's buffer into the stringstream
    return buffer.str();     // Convert the stringstream to a string and return
}

int main() {
    // The cURL command you want to execute
    const char* curlCommand = "curl -H \"Content-Type: application/json\" --data @body_example.json your_endpoint -o file.txt";

    // Prepare the process and startup info
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Prepare the command line argument
    char cmd[256];
    strcpy_s(cmd, sizeof(cmd), "cmd.exe /C ");
    strcat_s(cmd, sizeof(cmd), curlCommand);

    // Create the process
    if (CreateProcess(NULL,   // No module name (use command line)
                      cmd,    // Command line
                      NULL,   // Process handle not inheritable
                      NULL,   // Thread handle not inheritable
                      FALSE,  // Set handle inheritance to FALSE
                      0,      // No creation flags
                      NULL,   // Use parent's environment block
                      NULL,   // Use parent's starting directory 
                      &si,    // Pointer to STARTUPINFO structure
                      &pi)    // Pointer to PROCESS_INFORMATION structure
       ) {
        // Wait until child process exits.
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles.
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // read file.txt
        std::string filePath = "file.txt";  // The file you want to read
        std::string fileContent = readFileAsString(filePath);

        if (!fileContent.empty()) {
            std::cout << "File content:\n" << fileContent << std::endl;
        } else {
            std::cout << "File is empty or could not be read." << std::endl;
        }
    } else {
        std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
        return 1;
    }

    return 0;
}