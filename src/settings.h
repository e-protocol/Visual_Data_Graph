#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSlider>
#include <QTextBrowser>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QCheckBox>

QT_BEGIN_NAMESPACE
namespace Ui { class settings; }
QT_END_NAMESPACE

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    //Dialog Window Settings
    QSlider *sliderPrecision = nullptr;
    QCheckBox *checkRound = nullptr;
    QTextBrowser *textPrecision = nullptr;
    QDialogButtonBox *box = nullptr;
    bool is_Round = true;
    int getPrecision() {return m_precision;}
    void setPrecision(int precision) {m_precision = precision;}

private:
    Ui::settings *ui = nullptr;
    int m_precision = 2;

public slots:
    void changeSliderValue();
    void changeCheckBox();
};

#endif // SETTINGS_H
