#pragma once

class FNeuralNetwork;

class FNeuralNetworkJson
{
public:
	static void SerializeNbNeuronsToJson(FNeuralNetwork const&NeuralNetwork);
	static void SerializeNeuronsWeightsToJson(FNeuralNetwork const&NeuralNetwork);
	static void SerializeToJson(TSharedPtr<FJsonObject> const& JsonObject, FString const& JsonFileName);
	
	static TArray<uint32> const& FNeuralNetworkJson::DeserializeNbNeuronsJson(TSharedPtr<FJsonObject> const& JsonObject);
	static TArray<double> const& FNeuralNetworkJson::DeserializeNeuronsWeightsJson(
		TSharedPtr<FJsonObject> const& JsonObject,const uint32 NbInputs, const uint32 NbHidden, const uint32 NbOutputs);
	static bool DeserializeJson(TArray<uint32>& NbNeurons, TArray<double>& Weights);

private :
	inline static FString NbInputsText = TEXT("NbInputs");
	inline static FString NbHiddenText = TEXT("NbHidden");
	inline static FString NbOutputsText = TEXT("NbOutputs");
	inline static FString NbNeuronsText = TEXT("NbNeurons");
	inline static FString NeuronsWeightsText = TEXT("NeuronsWeights");
};