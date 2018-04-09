namespace Chris03
{
struct BusPredictionsConfig
{
    char stopId[8] = "";
    char agency[10] = "";
    uint8 showMinutesMin = 0;
    uint8 showMinutesMax = 15;
    uint8 refreshIntervalSeconds = 30;
    std::vector<String> ignoredRoutes;

    void parseJson(const String &jsonConfig)
    {
        if (jsonConfig != NULL)
        {
            DynamicJsonBuffer jsonBuffer(500);
            JsonObject &root = jsonBuffer.parseObject(jsonConfig);

            strncpy(agency, root["busAgency"].as<const char *>(), sizeof(agency));
            strncpy(stopId, root["busStop"], sizeof(stopId));

            refreshIntervalSeconds = root["refreshInterval"];
            showMinutesMin = root["showMin"];
            showMinutesMax = root["showMax"];

            JsonArray &ignoredRoutesArray = root["ignoredRoutes"];

            ignoredRoutes.clear();
            for (int i = 0; i < ignoredRoutesArray.size(); ++i)
            {
                ignoredRoutes.push_back(ignoredRoutesArray[i]);
            }
        }
    }
};
}