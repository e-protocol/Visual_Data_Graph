#include <settings.h>
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    this->setGeometry(0,0,400,100); //set dialog window size

    box = new QDialogButtonBox(this);
    box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    //sliders
    sliderPrecision = new QSlider(Qt::Horizontal,this);
    sliderPrecision->setRange(2,10);
    sliderPrecision->setFixedWidth(200);
    sliderPrecision->setValue(m_precision);

    //checkBox
    checkRound = new QCheckBox("Rounded values",this);
    checkRound->setChecked(true);

    //name option
    textPrecision = new QTextBrowser(this);
    textPrecision->setMaximumWidth(150);
    textPrecision->setMaximumHeight(30);
    textPrecision->setText("Precision: " + QString::number(sliderPrecision->value()));
    textPrecision->setStyleSheet("background:transparent");

    //add information about applying changes
    QTextBrowser *info = new QTextBrowser(this);
    info->setText("*Changes will be applied on new graph draw");
    info->setMaximumWidth(this->width());
    info->setStyleSheet("background:transparent");

    connect(box,&QDialogButtonBox::accepted,this,&QDialog::accept);
    connect(box,&QDialogButtonBox::rejected,this,&QDialog::reject);

    this->setWindowTitle("Settings");

    QFormLayout *layout = new QFormLayout();
    layout->addRow(textPrecision,sliderPrecision);
    layout->addRow(checkRound);
    layout->addRow(info);

    connect(sliderPrecision,&QSlider::valueChanged,this,&Settings::changeSliderValue);
    connect(checkRound,&QCheckBox::clicked,this,&Settings::changeCheckBox);

    layout->addWidget(box);
    this->setLayout(layout);
}
Settings::~Settings()
{
    delete ui;
}

void Settings::changeSliderValue()
{
    textPrecision->setText("Precision: " + QString::number(sliderPrecision->value()));
}

void Settings::changeCheckBox()
{
    if(checkRound->isChecked())
        is_Round = true;
    else
        is_Round = false;
}
