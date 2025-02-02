#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>

#include "format.h
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string tag;
  string total_mem, free_mem;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tag;
      if (tag == "MemTotal:")
        linestream >> total_mem;
      else
        linestream >> free_mem;
        break;
    }
  }

  return (stoi(total_mem) - stoi(free_mem)) / stoi(total_mem);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string up_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> up_time;
  }
  return stol(up_time);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> cpu_utilize = LinuxParser::CpuUtilization();
  long total_jiffies = 0;
  for(int i = LinuxParser::CPUStates::kUser_; i <= LinuxParser::CPUStates::kGuestNice_; i++) {
    total_jiffies += stol(cpu_utilize[i]);
  }
  return total_jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilize;
  string cpu_data;
  string cpu_line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, cpu_line);
    std::istringstream linestream(cpu_line);
    linestream >> cpu_data;
    while (linestream >> cpu_data)
    {
      cpu_utilize.push_back(cpu_data);
    }
  }
  return cpu_utilize;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string tag, total_processes;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tag >> total_processes;
      if(tag == "processes") {
        break;
      }
    }
  }
  return stoi(total_processes);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string tag, running_processes;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tag >> running_processes;
      if(tag == "procs_running") {
        break;
      }
    }
  }
  return stoi(running_processes);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string tag, ram_Size;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tag >> ram_Size;
        if(tag == "VmSize") {
          break;
        }
    }
  }
  return ram_Size;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string tag, user_id;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tag >> user_id;
        if(tag == "Uid") {
          break;
        }
    }
  }
  return user_id;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string user_id, x, user;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> user_id;
      if(user_id == LinuxParser::Uid(pid)) {
        break;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string uptime_in_tick;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    if(std::getline(stream, line)) {
      std::istringstream linestream(line);
      int count = 0;
      while(linestream >> uptime_in_tick) {
        count++;
        if(count == 22) {
          break;
        }
      }
    }
  }
  return stol(uptime_in_tick)/sysconf(_SC_CLK_TCK);
}