#ifndef T_STUDENT_H
#define T_STUDENT_H

#include "T_BasePage.h"

#include <random>
#include <string>
#include <vector>

#include "Model/StudentModel.h"

class ElaPushButton;
class ElaTableView;

class T_StudentPage : public T_BasePage {
    Q_OBJECT

public:
    T_StudentPage(QWidget *parent = nullptr);
    ~T_StudentPage();

private slots:
    void addStudent();                 // 添加学生信息
    void showStudents();               // 显示学生列表
    void deleteStudent();              // 删除学生信息
    void randomSelect();               // 随机点名
    void randomGroup();                // 随机分组
    void questionAnswerInteraction();  // 问答互动模式
    void searchStudent();              // 搜索学生信息

private:
    std::vector<Student> students;    // 存储学生信息
    ElaTableView *tableView;        // 表格控件，用于显示学生列表
    StudentModel *studentModel;     // 学生数据模型
    ElaPushButton *addButton;           // 添加学生按钮
    ElaPushButton *deleteButton;        // 删除学生按钮
    ElaPushButton *randomSelectButton;  // 随机点名按钮
    ElaPushButton *groupButton;         // 随机分组按钮
    ElaPushButton *questionButton;      // 问答互动按钮
    ElaPushButton *searchButton;        // 搜索学生按钮

    std::mt19937 rng;  // 随机数生成器
    void setupUI();    // 设置UI布局
    void updateTable(const std::vector<Student> &studentsToShow);  // 更新表格
};

#endif  // T_STUDENT_H
