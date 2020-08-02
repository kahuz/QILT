#pragma once
#ifndef __LABEL_SETTING__
#define __LABEL_SETTING__
#include <iostream>
#include <string>
#include <QWidget>
#include "ui_LabelSettingDialog.h"
#include "common.h"

class LabelSettingDialog : public QWidget
{
	Q_OBJECT

public:
	LabelSettingDialog(QWidget *parent = Q_NULLPTR);
	~LabelSettingDialog();
	void init_value();
	int get_label_color();
	int get_label_thickness();
	bool get_is_setting();
	void increase_label_cnt();
	std::string get_label_name();
private slots:
	bool on_pressed_apply_btn();

private:
	std::string m_color_list[COMMON_COLOR_MAX] = { "Black", "White", "Red", "Green", "Blue", "Yellow" };
	std::string m_label_name;
	int label_cnt;
	int m_label_thickness;
	int m_label_color;
	bool is_setting;
	Ui::LabelSettingDialog ui;
};
#endif