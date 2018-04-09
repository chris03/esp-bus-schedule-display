namespace Chris03
{
struct BusPredictionItem
{
    String routeTitle;
    String routeTag;
    uint8_t minutes;

    bool operator<(const BusPredictionItem &other) const
    {
        return minutes < other.minutes;
    }

    bool operator>(const BusPredictionItem &other) const
    {
        return minutes > other.minutes;
    }
};
}