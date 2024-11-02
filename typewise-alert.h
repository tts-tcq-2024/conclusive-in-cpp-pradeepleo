#pragma once

// Enum for cooling types
typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING
} CoolingType;

// Enum for breach types
typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

// Enum for alert targets
typedef enum {
  TO_CONTROLLER,
  TO_EMAIL
} AlertTarget;

// Struct to represent the battery characteristics
typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

// Struct to hold temperature limits
typedef struct {
  int lowerLimit;
  int upperLimit;
} TemperatureLimits;

// Function declarations
TemperatureLimits getTemperatureLimits(CoolingType coolingType);
BreachType inferBreach(double value, double lowerLimit, double upperLimit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);
void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);
void sendToController(BreachType breachType);
void sendToEmail(BreachType breachType);
void notifyByEmail(const char* message);

#endif // TYPEWISE_ALERT_H
