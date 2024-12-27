#ifndef ENTITY_DATA_EXTRACTOR_CREATOR_H
#define ENTITY_DATA_EXTRACTOR_CREATOR_H

#include <iostream>

#include "fraymakers-entity-data-extractor/struct-definitions.h"
#include <json/json.h>

class entityDataCreator {
	public:
		void printData(Json::Value, std::string);
		void printData(Json::Value);

		Json::Value createAnimations(entityData*);

		Json::Value createLayers(entityData*);
		Json::Value createLayerDataFromType(layer*);

		Json::Value createKeyframes(entityData*);
		Json::Value createKeyframeDataFromType(keyframe*);

		Json::Value createSymbols(entityData*);
		Json::Value createSymbolDataFromType(symbol*);

		Json::Value createPaletteMap(entityData*);
		Json::Value createPlugins(entityData*);
		Json::Value createPluginMetadata(entityData*);
		Json::Value createPluginMetadataFromType(pluginMetadataEntry*);
		Json::Value createTags(entityData*);
	private:
		enumTranslate translator;
};

#endif