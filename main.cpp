#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "AlsaManager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QWidget window;
    window.setWindowTitle("ALSA Pro Interface");
    QVBoxLayout *layout = new QVBoxLayout(&window);

    AlsaManager alsa;
    alsa.initAudio();

    // Volume UI
    layout->addWidget(new QLabel("Master Volume:"));
    QSlider *volSlider = new QSlider(Qt::Horizontal);
    volSlider->setRange(0, 100);
    volSlider->setValue(50);
    layout->addWidget(volSlider);

    // Frequency UI
    layout->addWidget(new QLabel("Frequency (Hz):"));
    QDial *freqDial = new QDial();
    freqDial->setRange(200, 2000);
    freqDial->setValue(440); // Standard 'A' note
    layout->addWidget(freqDial);

    // Toggle Button
    QPushButton *btn = new QPushButton("Start Audio Stream");
    btn->setCheckable(true);
    layout->addWidget(btn);

    // Continuous Audio Timer
    QTimer *audioTimer = new QTimer();
    
    // This is the "Heartbeat" - it plays sound repeatedly while the button is ON
    QObject::connect(audioTimer, &QTimer::timeout, [&]() {
        alsa.playTone(freqDial->value(), 50); // Play a tiny 50ms slice
    });

    // Toggle Logic
    QObject::connect(btn, &QPushButton::toggled, [&](bool checked) {
        if (checked) {
            audioTimer->start(45); // Trigger every 45ms (slight overlap for smoothness)
            btn->setText("Stop Audio Stream");
        } else {
            audioTimer->stop();
            btn->setText("Start Audio Stream");
        }
    });

    // Volume Signal
    QObject::connect(volSlider, &QSlider::valueChanged, [&](int val) {
        alsa.setVolume(val);
    });

    window.resize(400, 300);
    window.show();
    return a.exec();
}