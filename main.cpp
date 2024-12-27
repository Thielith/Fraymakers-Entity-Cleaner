#include "main.h"
#include <fraymakers-entity-data-extractor/struct-definitions.h>

using namespace std;

void printHelpMessage(){
	cout << "Usage:  program -f [file] [options]\n"
		 << endl;

	cout << "Options:\n"
		 << setw(30) << left << "-h:" << "Prints program usage" << endl
		 << setw(30) << left << "-f, --file [path]" << "Path to .entity file" << endl
		 << setw(30) << left << "-o, --output [path]" << "Outputs cleaned .entity file to [path]" << endl

		 << endl;
}

int main(int argc, char* argv[]){
	if(argc < 2){
		printHelpMessage();
		return 0;
	}

	bool fileDefined = false;
	filesystem::path filePathAndName;
	bool outputPathDefined = false;
	string outputPathUser = "";

	int i=1;
	while(i<argc){
		// cout << "Argument " << i << ": " << argv[i] << endl;
		string argument(argv[i]);

		if(!argument.compare("-h")){
			printHelpMessage();
			return 0;
		}
		else if(!fileDefined && (!argument.compare("--file") || !argument.compare("-f"))){
			fileDefined = true;

			string path(argv[i+1]);
			filePathAndName.assign(path);
			i++;
		}else if(!outputPathDefined && (!argument.compare("--output") || !argument.compare("-o"))){
			outputPathDefined = true;

			string path(argv[i+1]);
			outputPathUser = path;
			i++;
		}

		i++;
	}
	
	if(!fileDefined){
		cout << "Error: No file defined.\n";
		return -1;
	}

	cout << "Extracting...\n";
	entityDataExtractor extractor;
	entityData* data = extractor.extractEntityData(filePathAndName.string());

	cout << "Cleaning...\n";
	clean(data);

	cout << "Assembling...\n";
	entityDataCreator creator;
	Json::Value output;
	output["animations"] = creator.createAnimations(data);
	output["export"] = data->shouldExport;
	output["guid"] = data->guid;
	output["id"] = data->id;
	output["keyframes"] = creator.createKeyframes(data);
	output["layers"] = creator.createLayers(data);
	output["paletteMap"] = creator.createPaletteMap(data);
	output["pluginMetadata"] = creator.createPluginMetadata(data);
	output["plugins"] = creator.createPlugins(data);
	output["symbols"] = creator.createSymbols(data);
	output["tags"] = creator.createTags(data);
	output["terrains"] = Json::arrayValue;
	output["tilesets"] = Json::arrayValue;
	output["version"] = data->version;
	// creator.printData(output, "symbols");

	cout << "Creating...";
	string newFileName = filePathAndName.stem().concat("_cleaned").string();
	newFileName.append(filePathAndName.extension().string());
	filesystem::path outputFileAndPath = filePathAndName;
	string outputFile = outputFileAndPath.filename().string();
	outputFileAndPath.replace_filename(newFileName);
	
	ofstream fileOutput;
	if(outputPathDefined)	fileOutput.open(outputPathUser + newFileName);
	else 					fileOutput.open(outputFileAndPath);
	
	Json::StyledWriter writer;
	fileOutput << writer.write(output);
	fileOutput.close();
}

void clean(entityData* data){
	list<layer*> copyOfLayers(*data->layers);
	for(layer* layerData : copyOfLayers){
		if(!isUsed(layerData->id, *data->animations)){
			(*data->layers).remove(layerData);
		}
	}

	list<keyframe*> copyOfKeyframes(*data->keyframes);
	for(keyframe* keyframeData : copyOfKeyframes){
		if(!isUsed(keyframeData->id, *data->layers))
			data->keyframes->remove(keyframeData);
	}

	list<symbol*> copyOfSymbols(*data->symbols);
	for(symbol* symbolData : copyOfSymbols){
		if(!isUsed(symbolData->id, *data->keyframes))
			data->symbols->remove(symbolData);
	}
}

bool isUsed(string targetLayerID, list<animation> animations){
	for(animation animation : animations){
		for(std::string layerID : animation.layerIDs){
			if(!layerID.compare(targetLayerID)){
				return true;
			}
		}
	}
	return false;
}
bool isUsed(string targetKeyframeID, list<layer*> layers){
	for(layer* layer : layers){
		for(std::string keyframeID : layer->keyframeIDs){
			if(!keyframeID.compare(targetKeyframeID)){
				return true;
			}
		}
	}
	return false;
}
bool isUsed(string symbolID, list<keyframe*> keyframes){
	for(keyframe* keyframe : keyframes){
		keyframeAnimated* animated = dynamic_cast<keyframeAnimated*>(keyframe);
		if(!animated) continue;

		if(!(animated->symbolID).compare(symbolID)){
			return true;
		}
	}
	return false;
}
