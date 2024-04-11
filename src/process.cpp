#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;


// TODO: Return this process's ID
// int Process::Pid() { return 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    int total_process_time, elapsed_time;
    string line;
    vector<string> process_cpu_utilize;
    std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(this->pid_) + LinuxParser::kStatFilename);
    if(std::getline(stream, line)) {
        std::istringstream linestream(line);
        int count = 0;
        while(linestream >> line) {
            count++
            if(count >= 14) {
                process_cpu_utilize.push_back(line);
            }
            if(count > 17) break;
        }
    }

    total_process_time = stoi(process_cpu_utilize[0]) + stoi(process_cpu_utilize[1]) + stoi(process_cpu_utilize[2]) + stoi(process_cpu_utilize[3]);
    elapsed_time = LinuxParser::UpTime() - LinuxParser::UpTime(this->pid_);
    return 100 * (total_process_time / sysconf(_SC_CLK_TCK)) / elapsed_time;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(this->pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(this->pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(this->pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(this->pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return this->CpuUtilization() < a.CpuUtilization();
}