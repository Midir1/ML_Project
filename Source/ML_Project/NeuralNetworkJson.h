#pragma once

class FNeuralNetwork;

class FNeuralNetworkJson
{
public:
	static void SerializeToJson(FNeuralNetwork const& NeuralNetwork);
	static bool DeserializeJson(TArray<uint32>& NbNeurons, TArray<double>& Weights);

private :
	static void NbNeuronsToJsonObject(FNeuralNetwork const& NeuralNetwork, TSharedPtr<FJsonObject> const& JsonObject);
	static void NeuronsWeightsToJsonObject(FNeuralNetwork const& NeuralNetwork, TSharedPtr<FJsonObject> const& JsonObject);
	static void SaveJsonObjectToFile(TSharedPtr<FJsonObject> const& JsonObject, FString const& JsonFileName);
	
	static TArray<uint32> const& FNeuralNetworkJson::JsonObjectToNbNeurons(TSharedPtr<FJsonObject> const& JsonObject);
	static TArray<double> const& FNeuralNetworkJson::JsonObjectToNeuronsWeights(
		TSharedPtr<FJsonObject> const& JsonObject,const uint32 NbInputs, const uint32 NbHidden, const uint32 NbOutputs);
	
	inline static FString NbInputsText = "NbInputs";
	inline static FString NbHiddenText = "NbHidden";
	inline static FString NbOutputsText = "NbOutputs";
	inline static FString NbNeuronsText = "NbNeurons";
	inline static FString NeuronsWeightsText = "NeuronsWeights";
	inline static FString NbNeuronsJsonPath = FPaths::ProjectDir() + "/Json/" + NbNeuronsText + ".json";
	inline static FString NeuronsWeightsJsonPath = FPaths::ProjectDir() + "/Json/" + NeuronsWeightsText + ".json";
};