#include "BusPredictions.h"
namespace Chris03
{
BusPredictions::BusPredictions(BusPredictionsConfig &config)
    : busConfig_(config)
{
}

BusPredictions::~BusPredictions()
{
}

void BusPredictions::setOnUpdateCallback(OnUpdateCallback callback)
{
    onUpdateCallback_ = callback;
}

void BusPredictions::update()
{
    WiFiClient wifiClient;

    if (wifiClient.connect("webservices.nextbus.com", 80))
    {
        Serial.println(F("Connected to server"));

        wifiClient.printf("GET /service/publicJSONFeed?command=predictions&a=%s&stopId=%s HTTP/1.1\n", busConfig_.agency, busConfig_.stopId);
        wifiClient.println("Host: webservices.nextbus.com");
        wifiClient.println("Connection: close");

        if (wifiClient.println() == 0)
        {
            Serial.println(F("Failed to send request"));
            return;
        }

        // Check HTTP status
        char status[32] = {0};
        wifiClient.readBytesUntil('\r', status, sizeof(status));
        if (strcmp(status, "HTTP/1.1 200 OK") != 0)
        {
            Serial.print(F("Unexpected response: "));
            Serial.println(status);
            return;
        }

        // Skip HTTP headers
        char endOfHeaders[] = "\r\n\r\n";
        if (!wifiClient.find(endOfHeaders))
        {
            Serial.println(F("Invalid response"));
            return;
        }

        parseJson(wifiClient);

        if (onUpdateCallback_ != NULL)
        {
            onUpdateCallback_(busPredictions_);
        }
    }
}

void BusPredictions::parseJson(WiFiClient &wifiClient)
{
    DynamicJsonBuffer jsonBuffer(5000);
    JsonObject &jsonRoot = jsonBuffer.parseObject(wifiClient);

    JsonArray &jsonPredictionArray = jsonRoot["predictions"];
    Serial.printf("Predictions: %i\n", jsonPredictionArray.size());

    // Clear previous values
    busPredictions_.clear();

    for (uint i = 0; i < jsonPredictionArray.size(); ++i)
    {
        JsonObject &jsonPrediction = jsonPredictionArray[i];
        const char *routeTitle = jsonPrediction["routeTitle"];
        const char *routeTag = jsonPrediction["routeTag"];

        Serial.printf("%s - %s\n", routeTag, routeTitle);

        // Get each prediction
        JsonArray &jsonDirectionPredictionsArray = jsonPrediction["direction"]["prediction"];

        if (jsonDirectionPredictionsArray.success())
        {
            for (uint j = 0; j < jsonDirectionPredictionsArray.size(); ++j)
            {
                const char *minutes = jsonDirectionPredictionsArray[j]["minutes"];
                Serial.printf("  %s min.  ", minutes);
                addBusPrediction(routeTitle, routeTag, minutes);
            }
        }
        else
        {
            // Strange API: when only one item the value is an object instead of an array
            JsonObject &jsonDirectionPredictionsObject = jsonPrediction["direction"]["prediction"];

            if (jsonDirectionPredictionsObject.success())
            {
                const char *minutes = jsonDirectionPredictionsObject["minutes"];
                Serial.printf("  %s min.  ", minutes);
                addBusPrediction(routeTitle, routeTag, minutes);
            }
        }
    }

    // Sort predictions
    std::sort(busPredictions_.begin(), busPredictions_.end());
}

void BusPredictions::addBusPrediction(const char *routeTitle, const char *routeTag, const char *minutes)
{
    unsigned char minutesNumber = String(minutes).toInt();
    String routeTagStr = String(routeTag);

    if (isInArray(routeTagStr, busConfig_.ignoredRoutes))
    {
        Serial.println(F("Ignored route, not added."));
    }
    else if (minutesNumber > busConfig_.showMinutesMax || minutesNumber < busConfig_.showMinutesMin)
    {
        Serial.println(F("Outside time limits, not added."));
    }
    else
    {
        // Create copies of values because they will not exist after parse is done
        BusPredictionItem p;
        p.minutes = minutesNumber;
        p.routeTag = routeTagStr;
        p.routeTitle = htmlUnescape(routeTitle);
        busPredictions_.push_back(p);
        Serial.printf("Added. Count: %i\n", busPredictions_.size());
    }
}

bool BusPredictions::isInArray(const String &value, const std::vector<String> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

String BusPredictions::htmlUnescape(const char *url)
{
    String dest = "";
    const char *src;

    src = url;

    while (*src != '\0')
    {
        if (*src == '&')
        {
            const char *end;
            end = strchr(src, ';');
            src++;

            if (!end)
                return "";

            if (src[0] == '#')
            {
                int id, n;

                src++;

                if (src[0] == 'x')
                    n = sscanf(src + 1, "%x", &id);
                else
                    n = sscanf(src, "%u", &id);
                if (n != 1)
                    return "";

                dest.concat((char)id);
            }

            src = end + 1;
        }
        else
        {
            dest.concat(*src);
            src++;
        }
    }

    return dest;
}
}