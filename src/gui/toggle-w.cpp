#include "toggle-w.h"


ToggleW::ToggleW(QWidget *parent): QWidget(parent){
	main_gb = new QGroupBox(this);
	
	toggle_chb = new QCheckBox(this);
	connect(toggle_chb, SIGNAL(stateChanged(int)), this, SLOT(toggle_slot(int)));
	toggle_chb->setChecked(false);

	layout = new QVBoxLayout(this);
	layout->addWidget(toggle_chb);
	layout->addWidget(main_gb);

	main_gb->setVisible(false);

	updateGeometry();

	layout = new QVBoxLayout(main_gb);
}

ToggleW::ToggleW(const char* name, QWidget *parent): ToggleW(parent){
	toggle_chb->setText(name);
}

void ToggleW::addWidget(QWidget *w){
	layout->addWidget(w);
}

void ToggleW::toggle_slot(int val){
	main_gb->setVisible(val);
	updateGeometry();
	toggled();
}