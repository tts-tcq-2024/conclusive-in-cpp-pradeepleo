#include "typewise-alert.h"
#include <stdio.h>

// Constants for email notifications
const char* EMAIL_RECIPIENT = "a.b@c.com";

// Struct to hold temperature limits
typedef struct {
    int lowerLimit;
    int upperLimit;
} TemperatureLimits;

// Function to get temperature limits based on cooling type
TemperatureLimits getTemperatureLimits(CoolingType coolingType) {
    switch (coolingType) {
        case PASSIVE_COOLING:
            return (TemperatureLimits){0, 35};
        case HI_ACTIVE_COOLING:
            return (TemperatureLimits){0, 45};
        case MED_ACTIVE_COOLING:
            return (TemperatureLimits){0, 40};
        default:
            return (TemperatureLimits){0, 0};  // Fallback to safe limits
    }
}

// Function to infer breach type
BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if (value < lowerLimit) {
        return TOO_LOW;
    }
    if (value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

// Refactored function to classify temperature breach
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    TemperatureLimits limits = getTemperatureLimits(coolingType);
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

// Function to handle alerts based on the alert target
void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);

    if (alertTarget == TO_CONTROLLER) {
        sendToController(breachType);
    } else if (alertTarget == TO_EMAIL) {
        sendToEmail(breachType);
    }
}

// Function to send alerts to the controller
void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

// Function to send email alerts
void sendToEmail(BreachType breachType) {
    if (breachType == TOO_LOW) {
        printf("To: %s\n", EMAIL_RECIPIENT);
        printf("Hi, the temperature is too low\n");
    } else if (breachType == TOO_HIGH) {
        printf("To: %s\n", EMAIL_RECIPIENT);
        printf("Hi, the temperature is too high\n");
    }
    // No action needed for NORMAL case
}
