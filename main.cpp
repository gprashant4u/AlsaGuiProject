#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QPushButton>
#include "AlsaManager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QWidget window;
    window.setWindowTitle("ALSA Pro Interface");
    QVBoxLayout *layout = new QVBoxLayout(&window);

    AlsaManager alsa;
    alsa.initAudio();

    // Volume Control
    layout->addWidget(new QLabel("Master Volume:"));
    QSlider *volSlider = new QSlider(Qt::Horizontal);
    volSlider->setRange(0, 100);
    volSlider->setValue(50);
    layout->addWidget(volSlider);

    // Frequency Control
    layout->addWidget(new QLabel("Frequency (Hz):"));
    QDial *freqDial = new QDial();
    freqDial->setRange(200, 2000);
    layout->addWidget(freqDial);

    QPushButton *btn = new QPushButton("Start Audio Stream");
    layout->addWidget(btn);

    // Signals
    QObject::connect(volSlider, &QSlider::valueChanged, [&](int val) {
        alsa.setVolume(val);
    });

    window.resize(400, 300);
    window.show();
    return a.exec();
}
