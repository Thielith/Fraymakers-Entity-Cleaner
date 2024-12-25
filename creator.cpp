#include "creator.h"

Json::Value entityDataCreator::createAnimations(entityData* data){
	Json::Value animations(Json::arrayValue);
	for(animation animationData : *data->animations){
		Json::Value animation;
		animation["$id"] = animationData.id;
		animation["name"] = animationData.name;
		animation["pluginMetadata"] = Json::Value(Json::objectValue);

		Json::Value layers(Json::arrayValue);
		for(std::string layerID : animationData.layerIDs){
			layers.append(layerID);
		}
		animation["layers"] = layers;

		animations.append(animation);
	}

	return animations;
}

Json::Value entityDataCreator::createLayers(entityData* data){
	Json::Value layers(Json::arrayValue);
	for(layer* layerData : *data->layers){
		if(isUsed(layerData->id, data->animations)) continue;

		Json::Value layer;
		layer["$id"] = layerData->id;
		layer["hidden"] = layerData->hidden;
		layer["locked"] = layerData->locked;
		layer["name"] = layerData->name;
		layer["type"] = translator.toString(layerData->type);

		Json::Value keyframes(Json::arrayValue);
		for(std::string keyframeID : layerData->keyframeIDs){
			keyframes.append(keyframeID);
		}
		layer["keyframes"] = keyframes;

		Json::Value restOfSections = createLayerDataFromType(layerData);
		for(std::string name : restOfSections.getMemberNames()){
			layer[name] = restOfSections[name];
		}

		layers.append(layer);
	}

	return layers;
}
bool entityDataCreator::isUsed(std::string id, std::list<animation>* animations){
	for(animation animation : *animations){
		for(std::string layerID : animation.layerIDs){
			if(!layerID.compare(id)){
				return false;
			}
		}
	}
	return true;
}
Json::Value entityDataCreator::createLayerDataFromType(layer* data){
	if(layerScript* script = dynamic_cast<layerScript*>(data)){
		Json::Value output;
		output["language"] = script->language;
		return output;
	}
	else if(layerCollisionBox* collisionBox = dynamic_cast<layerCollisionBox*>(data)){
		Json::Value output;
		output["defaultAlpha"] = collisionBox->defaultAlpha;
		output["defaultColor"] = collisionBox->defaultColor;
		output["pluginMetadata"]["com.fraymakers.FraymakersMetadata"]["collisionBoxType"]
			= translator.toString(collisionBox->collisionBoxType);
		output["pluginMetadata"]["com.fraymakers.FraymakersMetadata"]["index"]
			= collisionBox->collisionBoxIndex;
		return output;
	}
	else if(layerCollisionBody* collisionBody = dynamic_cast<layerCollisionBody*>(data)){
		Json::Value output;
		output["defaultAlpha"] = collisionBody->defaultAlpha;
		output["defaultColor"] = collisionBody->defaultColor;
		output["defaultHead"] = collisionBody->defaultHead;
		output["defaultHipWidth"] = collisionBody->defaultHipWidth;
		output["defaultHipXOffset"] = collisionBody->defaultHipXOffset;
		output["defaultHipYOffset"] = collisionBody->defaultHipYOffset;
		output["defaultFoot"] = collisionBody->defaultFoot; 
		return output;
	}
	return Json::Value(Json::objectValue);
}


Json::Value entityDataCreator::createKeyframes(entityData* data){
	Json::Value keyframes(Json::arrayValue);
	for(keyframe* keyframeData : *data->keyframes){
		if(isUsed(keyframeData->id, data->layers)) continue;

		Json::Value keyframe;
		keyframe["$id"] = keyframeData->id;
		keyframe["length"] = keyframeData->length;
		keyframe["type"] = translator.toString(keyframeData->type);

		Json::Value restOfSections = createKeyframeDataFromType(keyframeData);
		for(std::string name : restOfSections.getMemberNames()){
			keyframe[name] = restOfSections[name];
		}

		keyframes.append(keyframe);
	}

	return keyframes;
}
bool entityDataCreator::isUsed(std::string id, std::list<layer*>* layers){
	for(layer* layer : *layers){
		for(std::string keyframeID : layer->keyframeIDs){
			if(!keyframeID.compare(id)){
				return false;
			}
		}
	}
	return true;
}
Json::Value entityDataCreator::createKeyframeDataFromType(keyframe* data){
	if(keyframeScript* script = dynamic_cast<keyframeScript*>(data)){
		Json::Value output;
		output["code"] = script->code;
		return output;
	}
	else if(keyframeAnimated* animated = dynamic_cast<keyframeAnimated*>(data)){
		Json::Value output;
		output["symbol"] = animated->symbolID;
		output["tweenType"] = translator.toString(animated->tweenType);
		output["tweened"] = animated->tweened;
		return output;
	}
	else if(keyframeLabel* label = dynamic_cast<keyframeLabel*>(data)){
		Json::Value output;
		output["name"] = label->name;
		return output;
	}
	return Json::Value(Json::objectValue);
}


Json::Value entityDataCreator::createSymbols(entityData* data){
	Json::Value symbols(Json::arrayValue);
	for(symbol* symbolData : *data->symbols){
		if(isUsed(symbolData->id, data->keyframes)) continue;

		Json::Value symbol;
		symbol["$id"] = symbolData->id;
		symbol["alpha"] = 
			(symbolData->alpha == -1.0) ?
			Json::Value::null :
			symbolData->alpha;
		symbol["type"] = translator.toString(symbolData->type);

		Json::Value restOfSections = createSymbolDataFromType(symbolData);
		for(std::string name : restOfSections.getMemberNames()){
			symbol[name] = restOfSections[name];
		}

		symbols.append(symbol);
	}

	return symbols;
}
bool entityDataCreator::isUsed(std::string id, std::list<keyframe*>* keyframes){
	for(keyframe* keyframe : *keyframes){
		keyframeAnimated* animated = dynamic_cast<keyframeAnimated*>(keyframe);
		if(!animated) continue;

		if(!(animated->symbolID).compare(id)){
			return false;
		}
	}
	return true;
}
Json::Value entityDataCreator::createSymbolDataFromType(symbol* data){
	if(symbolImage* image = dynamic_cast<symbolImage*>(data)){
		Json::Value output;
		output["x"] = image->x;
		output["y"] = image->y;
		output["rotation"] = image->rotation;
		output["scaleX"] = image->scaleX;
		output["scaleY"] = image->scaleY;
		output["pivotX"] = image->pivotX;
		output["pivotY"] = image->pivotY;
		output["imageAsset"] = image->imageAssetID;
		return output;
	}
	else if(symbolCollisionBox* collisionBox = dynamic_cast<symbolCollisionBox*>(data)){
		Json::Value output;
		output["color"] = 
			(!collisionBox->color.compare("null")) ?
			Json::Value::null :
			collisionBox->color;
		output["x"] = collisionBox->x;
		output["y"] = collisionBox->y;
		output["rotation"] = collisionBox->rotation;
		output["scaleX"] = collisionBox->scaleX;
		output["scaleY"] = collisionBox->scaleY;
		output["pivotX"] = collisionBox->pivotX;
		output["pivotY"] = collisionBox->pivotY;
		return output;
	}
	else if(symbolCollisionBody* collisionBody = dynamic_cast<symbolCollisionBody*>(data)){
		Json::Value output;
		output["color"] = 
			(!collisionBody->color.compare("null")) ?
			Json::Value::null :
			collisionBody->color;
		output["head"] = collisionBody->head;
		output["hipWidth"] = collisionBody->hipWidth;
		output["hipXOffset"] = collisionBody->hipXOffset;
		output["hipYOffset"] = collisionBody->hipYOffset;
		output["foot"] = collisionBody->foot;
		return output;
	}
	else if(symbolPoint* point = dynamic_cast<symbolPoint*>(data)){
		Json::Value output;
		output["color"] = point->color;
		output["x"] = point->x;
		output["y"] = point->y;
		output["rotation"] = point->rotation;
		return output;
	}
	else if(symbolLineSegment* lineSegment = dynamic_cast<symbolLineSegment*>(data)){
		Json::Value output;
		output["color"] = lineSegment->color;
		output["points"][0] = lineSegment->x1;
		output["points"][1] = lineSegment->y1;
		output["points"][2] = lineSegment->x2;
		output["points"][3] = lineSegment->y2;
		return output;
	}
	return Json::Value(Json::objectValue);
}


Json::Value entityDataCreator::createPaletteMap(entityData* data){
	Json::Value output;
	std::string collectionID = data->paletteMap->palletteCollectionID;
	output["paletteCollection"] =
		(!collectionID.compare("null")) ?
		Json::Value::null :
		collectionID;
	
	std::string ID = data->paletteMap->paletteMapID;
	output["paletteMap"] = 
		(!ID.compare("null")) ?
		Json::Value::null :
		ID;
	return output;
}

Json::Value entityDataCreator::createObject(entityData* data){
	Json::Value output;
	output["objectType"] = translator.toString(data->objectType);
	output["version"] = data->objectVersion;
	return output;
}


Json::Value entityDataCreator::createTags(entityData* data){
	Json::Value output;

	if(!data->tags.front().compare("null")){
		output = Json::arrayValue;
		return output;
	}

	unsigned int tagIndex = 0;
	for(std::string tag : data->tags){
		output[tagIndex] = tag;
		tagIndex++;
	}

	return output;
}


void entityDataCreator::printData(Json::Value data, std::string section){
	std::cout << data[section] << std::endl;
}
void entityDataCreator::printData(Json::Value data){
	std::cout << data << std::endl;
}
