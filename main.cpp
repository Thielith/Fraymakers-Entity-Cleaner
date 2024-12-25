#include "main.h"

using namespace std;

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
			filePathAndName = path;
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

	entityDataExtractor extractor;
	entityData* data = extractor.extractEntityData(filePathAndName);

	entityDataCreator creator;
	Json::Value output;
	output["animations"] = creator.createAnimations(data);
	output["export"] = data->shouldExport;
	output["guid"] = data->guid;	
	output["id"] = data->id;	
	output["keyframes"] = creator.createKeyframes(data);
	output["layers"] = creator.createLayers(data);
	output["paletteMap"] = creator.createPaletteMap(data);
	output["pluginMetadata"]["com.fraymakers.FraymakersMetadata"] = creator.createObject(data);
	output["plugins"][0] = "com.fraymakers.FraymakersMetadata";
	output["symbols"] = creator.createSymbols(data);
	output["tags"] = creator.createTags(data);
	output["terrains"] = Json::arrayValue;
	output["tilesets"] = Json::arrayValue;
	output["version"] = data->version;
	// creator.printData(output, "symbols");

	string newFileName = filePathAndName.stem().concat("_cleaned");
	newFileName.append(filePathAndName.extension());
	filesystem::path outputFileAndPath = filePathAndName;
	string outputFile = outputFileAndPath.filename();
	outputFileAndPath.replace_filename(newFileName);

	Json::StreamWriterBuilder settings;
	settings["indentation"] = "  ";
	Json::StreamWriter* writer(settings.newStreamWriter());
	
	ofstream fileOutput;
	if(outputPathDefined)	fileOutput.open(outputPathUser + newFileName);
	else 					fileOutput.open(outputFileAndPath);
	
	writer->write(output, &fileOutput);
}

void printHelpMessage(){
	cout << "Usage:  program -f [file] [options]\n"
		 << endl;

	cout << "Options:\n"
		 << setw(30) << left << "-h:" << "Prints program usage" << endl
		 << setw(30) << left << "-f, --file [path]" << "Path to .entity file" << endl
		 << setw(30) << left << "-o, --output [path]" << "Outputs cleaned .entity file to [path]" << endl

		 << endl;
}
