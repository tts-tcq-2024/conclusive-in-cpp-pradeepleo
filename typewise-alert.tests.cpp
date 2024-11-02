#include <gtest/gtest.h>
#include "typewise-alert.h"

// Test for getTemperatureLimits function
TEST(TypeWiseAlertTestSuite, GetTemperatureLimitsReturnsCorrectValues) {
    TemperatureLimits passiveLimits = getTemperatureLimits(PASSIVE_COOLING);
    EXPECT_EQ(passiveLimits.lowerLimit, 0);
    EXPECT_EQ(passiveLimits.upperLimit, 35);

    TemperatureLimits hiActiveLimits = getTemperatureLimits(HI_ACTIVE_COOLING);
    EXPECT_EQ(hiActiveLimits.lowerLimit, 0);
    EXPECT_EQ(hiActiveLimits.upperLimit, 45);

    TemperatureLimits medActiveLimits = getTemperatureLimits(MED_ACTIVE_COOLING);
    EXPECT_EQ(medActiveLimits.lowerLimit, 0);
    EXPECT_EQ(medActiveLimits.upperLimit, 40);
}


// Test for checkBreach function
TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    EXPECT_EQ(checkBreach(25, 0, 35), NORMAL);
    EXPECT_EQ(checkBreach(-1, 0, 35), TOO_LOW);
    EXPECT_EQ(checkBreach(36, 0, 35), TOO_HIGH);
}

// Test for classifyTemperatureBreach function
TEST(TypeWiseAlertTestSuite, ClassifyTemperatureBreachWorksCorrectly) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 20), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, -5), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 40), TOO_HIGH);

    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 20), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, -5), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50), TOO_HIGH);

    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 20), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, -5), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 45), TOO_HIGH);
}

// Mock functions to capture output for testing sendToController and sendToEmail
class OutputCapture {
public:
    static std::string capturedOutput;
    static void CaptureOutput() {
        capturedOutput.clear();
        testing::internal::CaptureStdout();
    }
    static std::string GetCapturedOutput() {
        capturedOutput = testing::internal::GetCapturedStdout();
        return capturedOutput;
    }
};

std::string OutputCapture::capturedOutput;

TEST(TypeWiseAlertTestSuite, SendToControllerPrintsCorrectly) {
    OutputCapture::CaptureOutput();
    sendToController(TOO_LOW);
    std::string output = OutputCapture::GetCapturedOutput();
    EXPECT_EQ(output, "feed : 1\n"); // Expected format for TOO_LOW (1)

    OutputCapture::CaptureOutput();
    sendToController(TOO_HIGH);
    output = OutputCapture::GetCapturedOutput();
    EXPECT_EQ(output, "feed : 2\n"); // Expected format for TOO_HIGH (2)

    OutputCapture::CaptureOutput();
    sendToController(NORMAL);
    output = OutputCapture::GetCapturedOutput();
    EXPECT_EQ(output, "feed : 0\n"); // Expected format for NORMAL (0)
}

TEST(TypeWiseAlertTestSuite, SendToEmailPrintsCorrectly) {
    OutputCapture::CaptureOutput();
    sendToEmail(TOO_LOW);
    std::string output = OutputCapture::GetCapturedOutput();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");

    OutputCapture::CaptureOutput();
    sendToEmail(TOO_HIGH);
    output = OutputCapture::GetCapturedOutput();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");

    OutputCapture::CaptureOutput();
    sendToEmail(NORMAL);
    output = OutputCapture::GetCapturedOutput();
    EXPECT_EQ(output, ""); // No output expected for NORMAL
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertCallsCorrectFunctions) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING};

    // Test alert to controller
    OutputCapture::CaptureOutput();
    checkAndAlert(TO_CONTROLLER, batteryChar, 40);
    std::string output = OutputCapture::GetCapturedOutput();
    EXPECT_EQ(output, "feed : 2\n"); // TOO_HIGH for PASSIVE_COOLING

    // Test alert to email
    OutputCapture::CaptureOutput();
    checkAndAlert(TO_EMAIL, batteryChar, -5);
    output = OutputCapture::GetCapturedOutput();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");
}

