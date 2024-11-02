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
    TemperatureLimits limits[] = {
        {0, 35},  // PASSIVE_COOLING
        {0, 45},  // HI_ACTIVE_COOLING
        {0, 40}   // MED_ACTIVE_COOLING
    };
    return limits[coolingType];
}

// Function to check if the value is too low or too high
BreachType checkBreach(double value, int lowerLimit, int upperLimit) {
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
    return checkBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

// Helper function to send email notifications
void notifyByEmail(const char* message) {
    printf("To: %s\n", EMAIL_RECIPIENT);
    printf("Hi, %s\n", message);
}

// Function to handle alerts
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
        notifyByEmail("the temperature is too low");
    } else if (breachType == TOO_HIGH) {
        notifyByEmail("the temperature is too high");
    }
    // No action needed for NORMAL case
}
