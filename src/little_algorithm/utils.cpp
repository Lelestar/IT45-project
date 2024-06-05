#include "utils.h"
#include <fstream>
#include <string>

using namespace std;

std::string last_message;
int message_count = 0;

void log_message(const std::string& message) {
    if (message == last_message) {
        message_count++;
    } else {
        if (message_count > 1) {
            cout << last_message << " (" << message_count << " times)\n";
        } else if (!last_message.empty()) {
            cout << last_message << "\n";
        }
        last_message = message;
        message_count = 1;
    }
}

void flush_log() {
    if (message_count > 1) {
        cout << last_message << " (" << message_count << " times)\n";
    } else if (!last_message.empty()) {
        cout << last_message << "\n";
    }
    last_message.clear();
    message_count = 0;
}
