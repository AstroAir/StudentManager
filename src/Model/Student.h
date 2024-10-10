#pragma once

#include <string>

struct Student {
    int index;
    std::string studentId;
    std::string name;
    bool answered = false;  // 用于问答互动时记录回答状态
};