#ifndef CHRIS03_BUSPREDICTIONS_H
#define CHRIS03_BUSPREDICTIONS_H

#include "ArduinoJson.h"
#include <WiFiClient.h>

#include "BusPredictionItem.h"
#include "BusPredictionsConfig.h"

namespace Chris03
{
class BusPredictions
{
public:
  typedef std::function<void(std::vector<BusPredictionItem> &)> OnUpdateCallback;

  BusPredictions(BusPredictionsConfig &config);
  ~BusPredictions();

  void update();
  void setOnUpdateCallback(OnUpdateCallback callback);

private:
  std::vector<BusPredictionItem> busPredictions_;
  BusPredictionsConfig &busConfig_;
  OnUpdateCallback onUpdateCallback_;

  void addBusPrediction(const char *routeTitle, const char *routeTag, const char *minutes);
  void parseJson(WiFiClient &wifiClient);
  String htmlUnescape(const char *url);
  bool isInArray(const String &value, const std::vector<String> &array);
};
}
#endif