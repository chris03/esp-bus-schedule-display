#include <FS.h>

using namespace fs;

const char *CONFIG_FILENAME = "/config.json";

bool saveConfig(const char *json)
{
    bool saved = false;
    File f = SPIFFS.open(CONFIG_FILENAME, "w+");

    if (f)
    {
        f.print(json);
        f.close();
        saved = true;
    }
    else
    {
        Serial.println(F("Failed to open config file"));
    }

    return saved;
}

String loadConfig()
{
    String json;
    File f = SPIFFS.open(CONFIG_FILENAME, "r");

    if (f)
    {
        json = f.readString();
        f.close();
    }
    else
    {
        Serial.println(F("Failed to open config file"));
    }

    return json;
}

void loadBusConfig()
{
    String jsonConfig = loadConfig();

    Serial.println(F("Loaded Config:"));
    Serial.println(jsonConfig);

    busConfig.parseJson(jsonConfig);

    Serial.printf("Agency: %s  Stop: %s  Show between: %i and %i  Refresh interval: %i\n", busConfig.agency, busConfig.stopId, busConfig.showMinutesMin, busConfig.showMinutesMax, busConfig.refreshIntervalSeconds);
}