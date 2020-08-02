#include "LabelSettingDialog.h"

LabelSettingDialog::LabelSettingDialog(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	init_value();

	for (int i = 0; i < COMMON_COLOR_MAX; i++)
	{
		ui.color_box->addItem(QString::fromStdString(m_color_list[i]));
		ui.thickness_box->addItem(QString::number(i+1));
	}

	connect(ui.apply_btn, SIGNAL(pressed()), this, SLOT(on_pressed_apply_btn()));
}

LabelSettingDialog::~LabelSettingDialog()
{
}

void LabelSettingDialog::init_value()
{
	m_label_name = "";
	m_label_thickness = 0;
	m_label_color = 0;
	label_cnt = 0;
	is_setting = false;
}

int LabelSettingDialog::get_label_color()
{
	return m_label_color;
}

int LabelSettingDialog::get_label_thickness()
{
	return m_label_thickness;
}

bool LabelSettingDialog::get_is_setting()
{
	return is_setting;
}

void LabelSettingDialog::increase_label_cnt()
{
	label_cnt++;
}

std::string LabelSettingDialog::get_label_name()
{
	std::string label_name = m_label_name;
	label_name.append(std::to_string(label_cnt));
	return label_name;
}

bool LabelSettingDialog::on_pressed_apply_btn()
{
	this->init_value();

	m_label_name = ui.name_edit->toPlainText().toStdString();
	m_label_thickness = ui.thickness_box->currentIndex() + 1;
	m_label_color = ui.color_box->currentIndex();
	is_setting = true;
	this->close();
	return true;
}
