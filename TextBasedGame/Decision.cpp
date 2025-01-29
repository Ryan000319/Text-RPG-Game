#include "Decision.h"

std::unique_ptr<Decision> DecisionFactory::createDecision(
    const std::string& text,
    bool requiresCheck,
    int checkThreshold,
    const std::string& stat,
    const std::string& successText,
    const std::string& failText,
    DecisionEffect* successEffect,
    DecisionEffect* failureEffect,
    Item* droppedItem) {

    return std::make_unique<Decision>(
        text,
        requiresCheck,
        checkThreshold,
        stat,
        successText,
        failText,
        successEffect,
        failureEffect,
        droppedItem);
}
