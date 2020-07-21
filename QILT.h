#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QILT.h"

class QILT : public QMainWindow
{
    Q_OBJECT

public:
    QILT(QWidget *parent = Q_NULLPTR);

private:
    Ui::QILTClass ui;
};
