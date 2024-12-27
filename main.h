#include <iomanip>
#include <fstream>
#include <filesystem>

#include "creator.h"
#include "fraymakers-entity-data-extractor/extractor.h"

void printHelpMessage();
void clean(entityData*);
bool isUsed(std::string, std::list<animation>);
bool isUsed(std::string, std::list<layer*>);
bool isUsed(std::string, std::list<keyframe*>);