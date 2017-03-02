#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QLabel>
#include "va.h"
#include "airports.h"

enum State {
    OFFLINE = -1,
    CONNECTED = 0,
    PREFLIGHT,
    TAXITORWY,
    CLIMB,
    CRUISE,
    DESCEND,
    TAXITOGATE,
    POSTFLIGHT
};

struct Status {
    qint64 time, rref;
    float realTime, flightTime, pauseTime;
    float pitch, bank, heading, vs, ias, gs, lat, lon, asl, agl, g;
    float flaps, zfw, fuel, distance, completed, remaining;
    char engine[8];
    bool gear;
    bool bea, nav, ldn, str, txi;
    float winddeg, windknots, oat;
};

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
    void uiUpdate();

    void on_connectButton_clicked();
    void on_startButton_clicked();
    void on_endButton_clicked();

    void on_callsign_textChanged(const QString &arg1);
    void on_password_textChanged(const QString &arg1);

    void on_conSim_clicked();

    void on_applyWeight_stateChanged(int arg1);

private:
    void connectToSim();
    void newEvent(QString desc, bool critical = 0);
    void taxi();
    void takeoff();
    void climb();
    void cruise();
    void descend();
    void landing();
    void deboard();
    void engineStart(int e);
    void engineStop(int e);
    void refuel(float prev, float cur);
    void overspeed();
    void stall();
    void paused();
    void unpaused();

    Ui::MainWindow *ui;
    QLabel *sRealTime, *sFlightTime;
    QUdpSocket* sock;
    VA va;
    Airports airports;
    QPair<double,double> dep, arr;
    QTimer timer, uiTimer;
    State state;
    qint32 pilot;
    qint64 startTime;
    float startLat, startLon;
    float maxG;
    QString flight;
    float startFuel, groundAGL;
    Status cur, onTakeoff, onLanding;
    qint32 trackId, eventId, critEvents;

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

void memcat(char** ptr, const char* data, int len);
void memcat(char** ptr, const char* data);
void memcat(char** ptr, const int data);
void sendDRef(QUdpSocket* sock, const char *name, qint32 id, qint32 freq = 20);
void setDRef(QUdpSocket* sock, const char* name, float val);
double greatcircle(QPair<double,double> src, QPair<double,double> tgt);

#endif // MAINWINDOW_H
