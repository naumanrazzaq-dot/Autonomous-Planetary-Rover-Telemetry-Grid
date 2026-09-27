#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Base Abstract Class
class PlanetaryProbe {
protected:
    string probeID;
    double batteryLevelPct;

public:
    static int activeProbesOnSurface;
    static int totalSamplesCollected;

    PlanetaryProbe(string id, double battery)
        : probeID(id), batteryLevelPct(battery) {
        activeProbesOnSurface++;
    }

    virtual ~PlanetaryProbe() {
        cout << "[HIBERNATION] Probe " << probeID << " entered deep sleep. Communications offline." << endl;
        activeProbesOnSurface--;
    }

    // Pure Virtual Interfaces
    virtual double calculateMissionEfficiency() const = 0;
    virtual void transmitTelemetry() const = 0;
};

// Static initializations outside class
int PlanetaryProbe::activeProbesOnSurface = 0;
int PlanetaryProbe::totalSamplesCollected = 0;

// Derived Class 1: Ground Drilling & Soil Analyzer Rover
class DrillingRover : public PlanetaryProbe {
private:
    int soilSamplesExtracted;
    double drillBitTemperatureC;

public:
    DrillingRover(string id, double battery, int samples, double temp)
        : PlanetaryProbe(id, battery),
          soilSamplesExtracted(samples),
          drillBitTemperatureC(temp) {
        totalSamplesCollected += samples;
    }

    ~DrillingRover() override {
        cout << " -> Retracting core drill chassis and sealing canister for " << probeID << "..." << endl;
    }

    double calculateMissionEfficiency() const override {
        if (batteryLevelPct <= 0.0) return 0.0;
        // Output samples per remaining unit battery, penalized if overheating
        double penalty = (drillBitTemperatureC > 120.0) ? 0.8 : 1.0;
        return (soilSamplesExtracted / batteryLevelPct) * 100.0 * penalty;
    }

    void transmitTelemetry() const override {
        cout << "\n==============================================" << endl;
        cout << "   SUBSURFACE DRILLING ROVER: " << probeID << endl;
        cout << "==============================================" << endl;
        cout << "  Battery Reserve       : " << fixed << setprecision(1) << batteryLevelPct << " %" << endl;
        cout << "  Core Samples Mined    : " << soilSamplesExtracted << " units" << endl;
        cout << "  Drill Bit Temperature : " << drillBitTemperatureC << " °C" << endl;
        cout << "  Mission Efficiency    : " << fixed << setprecision(2) << calculateMissionEfficiency() << " pts" << endl;
        cout << "  Drill Status          : " << (drillBitTemperatureC > 120.0 ? "WARNING: OVERHEATING" : "NOMINAL") << endl;
        cout << "==============================================" << endl;
    }
};

// Derived Class 2: Atmospheric & Radiation Scout Drone
class AtmosphericScout : public PlanetaryProbe {
private:
    double altitudeMeters;
    double atmosphericPressureKPa;
    int atmosphericScansLogged;

public:
    AtmosphericScout(string id, double battery, double alt, double pressure, int scans)
        : PlanetaryProbe(id, battery),
          altitudeMeters(alt),
          atmosphericPressureKPa(pressure),
          atmosphericScansLogged(scans) {}

    ~AtmosphericScout() override {
        cout << " -> Anchoring landing skids and shielding spectrometer for " << probeID << "..." << endl;
    }

    double calculateMissionEfficiency() const override {
        if (batteryLevelPct <= 0.0) return 0.0;
        // Efficiency scales with scans conducted safely above surface
        return (atmosphericScansLogged * altitudeMeters) / (batteryLevelPct * 10.0);
    }

    void transmitTelemetry() const override {
        cout << "\n==============================================" << endl;
        cout << "   AERIAL RECON SCOUT: " << probeID << endl;
        cout << "==============================================" << endl;
        cout << "  Battery Reserve       : " << fixed << setprecision(1) << batteryLevelPct << " %" << endl;
        cout << "  Current Altitude      : " << altitudeMeters << " meters" << endl;
        cout << "  Atmospheric Pressure  : " << atmosphericPressureKPa << " kPa" << endl;
        cout << "  Spectrometric Scans   : " << atmosphericScansLogged << endl;
        cout << "  Recon Efficiency      : " << fixed << setprecision(2) << calculateMissionEfficiency() << " pts" << endl;
        cout << "  Flight Condition      : " << (altitudeMeters >= 50.0 ? "CRUISE STABLE" : "GROUND PROXIMITY") << endl;
        cout << "==============================================" << endl;
    }
};

int main() {
    cout << "\n>>> ESTABLISHING DEEP-SPACE RELAY DOWNLINK <<<\n" << endl;

    const int TOTAL_PROBES = 2;
    PlanetaryProbe* surfaceFleet[TOTAL_PROBES];

    // Probe 1: Drill Rover on Mars Basin (Battery 78.5%, 14 samples, 105.0°C drill temp)
    surfaceFleet[0] = new DrillingRover("PERSEVERANCE-X", 78.5, 14, 105.0);

    // Probe 2: Aerial Scout (Battery 62.0%, 120.0m altitude, 0.636 kPa pressure, 42 scans)
    surfaceFleet[1] = new AtmosphericScout("INGENUITY-II", 62.0, 120.0, 0.636, 42);

    // Dynamic execution loop
    for (int i = 0; i < TOTAL_PROBES; i++) {
        surfaceFleet[i]->transmitTelemetry();
    }

    cout << "\n----------------------------------------------" << endl;
    cout << "Active Probes on Surface    : " << PlanetaryProbe::activeProbesOnSurface << endl;
    cout << "Total Geological Samples    : " << PlanetaryProbe::totalSamplesCollected << " units" << endl;
    cout << "----------------------------------------------\n" << endl;

    cout << ">>> DUSK REACHED: COMMENCING HIBERNATION SEQUENCE <<<\n" << endl;

    // Polymorphic heap cleanup
    for (int i = 0; i < TOTAL_PROBES; i++) {
        delete surfaceFleet[i];
        surfaceFleet[i] = nullptr;
    }

    cout << "\n----------------------------------------------" << endl;
    cout << "Active Probes After Hibernation : " << PlanetaryProbe::activeProbesOnSurface << endl;
    cout << "----------------------------------------------" << endl;

    return 0;
}
