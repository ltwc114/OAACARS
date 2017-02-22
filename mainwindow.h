#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include "va.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void gotUpdate();
    void sendUpdate();

    void on_connectButton_clicked();
    void on_startButton_clicked();

    void on_endButton_clicked();

    void on_callsign_textChanged(const QString &arg1);

    void on_password_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QUdpSocket* sock;
    VA va;
    QTimer timer;
    qint32 state; // connected, preflight, taxi-to-runway, climb, cruise, descend, taxi-to-gate, postflight, -1=offline
    qint32 pilot;
    QString flight;
    qint64 startTime;
    float startLat, startLon;
    float startFuel, maxG;

    struct States {
        float heading, lat, lon;
        float flaps;
        bool engine[4];
        bool gear;
    } states;

    struct Landing {
        float vs, ias, g, bank, pitch, winddeg, windknots, oat, flaps;
        bool bea, nav, ldn, str;
    } landing;

    struct Mistakes {
        bool crash, beaconOff, iasLow, lightsLow, lightsHigh, overspeed, pause,
            refuel, slew, taxiLights, takeoffLights, landingLights,
            landingAirport, stall, taxiSpeed, qnhTakeoff, qnhLanding;

        void reset() {
            crash = beaconOff = iasLow = lightsLow = lightsHigh = overspeed = pause
                    = refuel = slew = taxiLights = takeoffLights = landingLights
                    = landingAirport = stall = taxiSpeed = qnhTakeoff = qnhLanding = 0;
        }
    } mistakes;
};

#endif // MAINWINDOW_H
