#pragma once

#include <QWidget>
#include "ui_LabelDialog.h"

class LabelDialog : public QWidget
{
	Q_OBJECT

public:
	LabelDialog(QWidget *parent = Q_NULLPTR);
	~LabelDialog();

private:
	Ui::LabelDialog ui;
};
