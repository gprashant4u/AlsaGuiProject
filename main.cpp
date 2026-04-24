#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "AlsaManager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QWidget window;
    window.setWindowTitle("ALSA GUI Integration");
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QPushButton *btn = new QPushButton("Play Test Tone (440Hz)");
    
    AlsaManager alsa;
    if(!alsa.initAudio()) {
        btn->setText("ALSA Init Failed!");
    }

    QObject::connect(btn, &QPushButton::clicked, [&]() {
        alsa.playTone(440, 500);
    });

    layout->addWidget(btn);
    window.resize(300, 100);
    window.show();
    return a.exec();
}
