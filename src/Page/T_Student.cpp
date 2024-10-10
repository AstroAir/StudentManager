#include "T_Student.h"

#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <algorithm>
#include <random>

#include "ElaPushButton.h"
#include "ElaTableView.h"

#include "Model/StudentModel.h"  // 新增的学生数据模型头文件

T_StudentPage::T_StudentPage(QWidget *parent)
    : T_BasePage(parent), rng(std::random_device{}()) {
    setupUI();
}

T_StudentPage::~T_StudentPage() {}

void T_StudentPage::setupUI() {
    // 创建主布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 创建表格控件
    tableView = new ElaTableView(this);
    studentModel = new StudentModel(this);  // 使用自定义的学生数据模型
    tableView->setModel(studentModel);
    tableView->horizontalHeader()->setStretchLastSection(true);

    // 创建按钮控件
    addButton = new ElaPushButton("添加学生", this);
    deleteButton = new ElaPushButton("删除学生", this);
    randomSelectButton = new ElaPushButton("随机点名", this);
    groupButton = new ElaPushButton("随机分组", this);
    questionButton = new ElaPushButton("问答互动", this);
    searchButton = new ElaPushButton("搜索学生", this);

    // 连接按钮的点击事件到槽函数
    connect(addButton, &ElaPushButton::clicked, this, &T_StudentPage::addStudent);
    connect(deleteButton, &ElaPushButton::clicked, this,
            &T_StudentPage::deleteStudent);
    connect(randomSelectButton, &ElaPushButton::clicked, this,
            &T_StudentPage::randomSelect);
    connect(groupButton, &ElaPushButton::clicked, this,
            &T_StudentPage::randomGroup);
    connect(questionButton, &ElaPushButton::clicked, this,
            &T_StudentPage::questionAnswerInteraction);
    connect(searchButton, &ElaPushButton::clicked, this,
            &T_StudentPage::searchStudent);

    // 将控件添加到布局
    layout->addWidget(tableView);
    layout->addWidget(addButton);
    layout->addWidget(deleteButton);
    layout->addWidget(randomSelectButton);
    layout->addWidget(groupButton);
    layout->addWidget(questionButton);
    layout->addWidget(searchButton);

    centralWidget->setWindowTitle("学生管理系统");
    auto *centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addLayout(layout);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);
}

void T_StudentPage::addStudent() {
    bool ok;

    // 获取学生学号
    QString studentId = QInputDialog::getText(
        this, "添加学生", "请输入学生学号:", QLineEdit::Normal, "", &ok);
    if (!ok || studentId.isEmpty())
        return;

    // 获取学生姓名
    QString name = QInputDialog::getText(
        this, "添加学生", "请输入学生姓名:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;

    // 添加到students容器
    Student student{static_cast<int>(students.size() + 1),
                    studentId.toStdString(), name.toStdString()};
    students.push_back(student);

    // 更新表格显示
    studentModel->setStudents(students);
}

void T_StudentPage::showStudents() { studentModel->setStudents(students); }

void T_StudentPage::deleteStudent() {
    // 选择要删除的学生行
    QModelIndex currentIndex = tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "删除失败", "请先选择要删除的学生。");
        return;
    }

    // 删除students容器中的对应学生
    students.erase(students.begin() + currentIndex.row());

    // 更新表格显示
    studentModel->setStudents(students);
}

void T_StudentPage::randomSelect() {
    if (students.empty()) {
        QMessageBox::information(this, "随机点名", "没有学生信息可以点名。");
        return;
    }

    std::uniform_int_distribution<> dist(0,
                                         static_cast<int>(students.size()) - 1);
    int randomIndex = dist(rng);
    const Student &selectedStudent = students[randomIndex];

    QMessageBox::information(
        this, "随机点名",
        QString("学号: %1\n姓名: %2")
            .arg(QString::fromStdString(selectedStudent.studentId))
            .arg(QString::fromStdString(selectedStudent.name)));
}

void T_StudentPage::randomGroup() {
    if (students.empty()) {
        QMessageBox::information(this, "随机分组", "没有学生信息可以分组。");
        return;
    }

    bool ok;
    int groupSize = QInputDialog::getInt(this, "随机分组", "请输入每组人数:", 2,
                                         1, 100, 1, &ok);
    if (!ok)
        return;

    std::vector<Student> shuffledStudents = students;
    std::shuffle(shuffledStudents.begin(), shuffledStudents.end(), rng);

    QString result;
    int groupCount = 0;
    for (size_t i = 0; i < shuffledStudents.size(); ++i) {
        if (i % groupSize == 0) {
            groupCount++;
            result += QString("\n小组 %1:\n").arg(groupCount);
        }
        result +=
            QString(" 学号: %1 姓名: %2\n")
                .arg(QString::fromStdString(shuffledStudents[i].studentId))
                .arg(QString::fromStdString(shuffledStudents[i].name));
    }

    QMessageBox::information(this, "随机分组", result);
}

void T_StudentPage::questionAnswerInteraction() {
    if (students.empty()) {
        QMessageBox::information(this, "问答互动", "没有学生信息可以互动。");
        return;
    }

    std::vector<Student *> unanswered;
    for (auto &student : students) {
        if (!student.answered) {
            unanswered.push_back(&student);
        }
    }

    if (unanswered.empty()) {
        QMessageBox::information(this, "问答互动", "所有学生已回答过问题。");
        return;
    }

    std::uniform_int_distribution<> dist(
        0, static_cast<int>(unanswered.size()) - 1);
    Student *selectedStudent = unanswered[dist(rng)];

    bool ok;
    QString answer = QInputDialog::getText(
        this, "问答互动",
        QString("提问学生: %1 (%2)\n请输入回答:")
            .arg(QString::fromStdString(selectedStudent->name))
            .arg(QString::fromStdString(selectedStudent->studentId)),
        QLineEdit::Normal, "", &ok);
    if (ok && !answer.isEmpty()) {
        selectedStudent->answered = true;
    }
}

void T_StudentPage::searchStudent() {
    bool ok;
    QString searchKey = QInputDialog::getText(
        this, "搜索学生", "请输入学号或姓名:", QLineEdit::Normal, "", &ok);
    if (!ok || searchKey.isEmpty())
        return;

    std::vector<Student> result;
    for (const auto &student : students) {
        if (QString::fromStdString(student.studentId)
                .contains(searchKey, Qt::CaseInsensitive) ||
            QString::fromStdString(student.name)
                .contains(searchKey, Qt::CaseInsensitive)) {
            result.push_back(student);
        }
    }

    if (result.empty()) {
        QMessageBox::information(this, "搜索结果", "未找到匹配的学生。");
    } else {
        studentModel->setStudents(result);  // 显示搜索结果
    }
}