#include <string>
#include <unistd.h>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int _hours;
    int _minutes;
    int _seconds;
    _hours = seconds/3600;
    _minutes = (seconds%3600)/60;
    _seconds = seconds%60;

    return std::to_string(_hours) + ":" + std::to_string(_minutes) + ":" + std::to_string(_seconds);
}