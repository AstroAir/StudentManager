// StudentModel.cpp
#include "StudentModel.h"

StudentModel::StudentModel(QObject *parent)
    : QAbstractTableModel(parent) {}

void StudentModel::setStudents(const std::vector<Student> &students) {
    beginResetModel();
    this->students = students;
    endResetModel();
}

int StudentModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(students.size());
}

int StudentModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;  // 编号、学号、姓名
}

QVariant StudentModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    const Student &student = students.at(static_cast<size_t>(index.row()));
    switch (index.column()) {
    case 0:
        return student.index;
    case 1:
        return QString::fromStdString(student.studentId);
    case 2:
        return QString::fromStdString(student.name);
    default:
        return QVariant();
    }
}

QVariant StudentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "编号";
        case 1:
            return "学号";
        case 2:
            return "姓名";
        default:
            return QVariant();
        }
    }
    return QVariant();
}