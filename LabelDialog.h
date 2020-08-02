#pragma once

#ifndef __LABEL_DIALOG__
#define __LABEL_DIALOG__

//QT lib
#include <QWidget>
#include <QPixmap>
#include "ui_LabelDialog.h"


class LabelDialog : public QWidget
{
	Q_OBJECT

public:
	LabelDialog(QWidget *parent = Q_NULLPTR);
	~LabelDialog();

	bool get_is_open();
	void set_is_opne(bool is_open);
	void draw_prev_img(QPixmap prev_img);
private:
	bool m_is_open = false;
	Ui::LabelDialog ui;
};

#endif