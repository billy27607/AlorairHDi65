#include <AlorairHDi65.h>
#include <SimpleCLI.h>

SimpleCLI cli;

// Command declarations
Command statusCmd;
Command powerOnCmd;
Command powerOffCmd;
Command setTargetHumidityCmd;
Command pumpOutCmd;

// Function prototypes
void statusCallback(cmd *);
void powerOnCallback(cmd *);
void powerOffCallback(cmd *);
void setTargetHumidityCallback(cmd *);
void pumpOutCallback(cmd *);
void errorCallback(cmd_error *e);

String inputString = "";     // A string to hold incoming data
bool stringComplete = false; // Whether the string is complete

// instantiate dehumidifier

AlorairHDi65 alorair(D7);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // Initialize commands
    statusCmd = cli.addCommand("status", statusCallback);
    powerOnCmd = cli.addCommand("poweron", powerOnCallback);
    powerOffCmd = cli.addCommand("poweroff", powerOffCallback);
    setTargetHumidityCmd = cli.addCommand("set_target_humidity", setTargetHumidityCallback);
    setTargetHumidityCmd.addPositionalArgument("value");
    pumpOutCmd = cli.addCommand("pumpout", pumpOutCallback);

    // start the alorair
    alorair.begin();

    // Initial prompt
    Serial.print(">>>>>");
}

void loop()
{
    // Check if the string is complete
    if (stringComplete)
    {
        // Parse the input string
        cli.parse(inputString.c_str());

        if (cli.errored())
        {
            CommandError cmdError = cli.getError();

            Serial.print("ERROR: ");
            Serial.println(cmdError.toString());

            if (cmdError.hasCommand())
            {
                Serial.print("Did you mean \"");
                Serial.print(cmdError.getCommand().toString());
                Serial.println("\"?");
            }
        }

        // Clear the string
        inputString = "";
        stringComplete = false;

        // Print the prompt again
        Serial.print(">>>>>");
    }

    // Read incoming data
    while (Serial.available())
    {
        char inChar = (char)Serial.read();
        Serial.print(inChar);
        inputString += inChar;
        if (inChar == '\n')
        {
            stringComplete = true;
        }
    }
}

// Callback in case of an error
void errorCallback(cmd_error *e)
{
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand())
    {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}

void statusCallback(cmd *c)
{
    Command cmd(c);
    if (alorair.status())
        alorair.printStatus();
    else
        Serial.println("response timeout");
}

void powerOnCallback(cmd *c)
{
    Command cmd(c);
    if (alorair.setPower(true))
    {
        delay(1000);
        alorair.status();
        alorair.printStatus();
    }
    else
        Serial.println("response timeout");
}

void powerOffCallback(cmd *c)
{
    Command cmd(c);
    if (alorair.setPower(false))
    {
        delay(1000);
        alorair.status();
        alorair.printStatus();
    }
    else
        Serial.println("response timeout");
}

void setTargetHumidityCallback(cmd *c)
{
    Command cmd(c);
    Argument arg = cmd.getArgument("value");
    int targetHumidity = arg.getValue().toInt();
    if (targetHumidity < 0)
        targetHumidity = 0;
    if (targetHumidity > 100)
        targetHumidity = 100;
    if (alorair.setTargetHumidity(targetHumidity))
    {
        delay(1000);
        alorair.status();
        alorair.printStatus();
    }
    else
        Serial.println("response timeout");
}

void pumpOutCallback(cmd *c)
{
    Command cmd(c);
    if (alorair.pumpOut())
    {
        delay(1000);
        alorair.status();
        alorair.printStatus();
    }
    else
        Serial.println("response timeout");
}
