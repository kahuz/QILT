#include "LabelDialog.h"

LabelDialog::LabelDialog(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

LabelDialog::~LabelDialog()
{
}

bool LabelDialog::get_is_open()
{
	return m_is_open;
}

void LabelDialog::set_is_opne(bool is_open)
{
	m_is_open = is_open;
}

void LabelDialog::draw_prev_img(QPixmap prev_img)
{
	ui.prev_label->setPixmap(prev_img);
	ui.prev_label->update();
}
