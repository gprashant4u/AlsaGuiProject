#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "AlsaManager.h"
#include <QPainter>
#include <QPainterPath>

// Simple Widget to draw the sine wave
class WaveformWidget : public QWidget {
public:
    int frequency = 440;
    WaveformWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumHeight(150);
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::black); // Background
        painter.setPen(QPen(Qt::green, 2));   // "Matrix" Green

        QPainterPath path;
        path.moveTo(0, height() / 2);

        // Draw the sine wave based on the current frequency
        for (int x = 0; x < width(); x++) {
            double angle = 2.0 * M_PI * frequency * (x / (double)width()) / 100.0;
            int y = (height() / 2) + (sin(angle) * (height() / 3));
            path.lineTo(x, y);
        }
        painter.drawPath(path);
    }
};
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
	
	//Waveform Widget
	WaveformWidget *visualizer = new WaveformWidget();
	layout->addWidget(visualizer);

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
	
	// Update connection:
	QObject::connect(freqDial, &QDial::valueChanged, [&](int val) {
    visualizer->frequency = val;
    visualizer->update(); // Redraws the widget
	});

    window.resize(400, 300);
    window.show();
    return a.exec();
}