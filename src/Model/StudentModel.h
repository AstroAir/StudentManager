// StudentModel.h
#pragma once

#include <QAbstractTableModel>
#include <vector>
#include "Student.h"

class StudentModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit StudentModel(QObject *parent = nullptr);

    void setStudents(const std::vector<Student> &students);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    std::vector<Student> students;
};
