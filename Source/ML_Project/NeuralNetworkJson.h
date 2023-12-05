#pragma once

class FNeuralNetwork;

class FNeuralNetworkJson
{
public:
	static void SerializeNbNeuronsToJson(FNeuralNetwork const&NeuralNetwork);
	static void SerializeNeuronsWeightsToJson(FNeuralNetwork const&NeuralNetwork);
	static void SerializeToJson(TSharedPtr<FJsonObject> const& JsonObject, FString const& JsonFileName);
	// static TArray<uint32> const& FNeuralNetworkJson::DeserializeNbNeuronsJson(TSharedPtr<FJsonObject> const& JsonObject);
	// static TArray<double> const& FNeuralNetworkJson::DeserializeNeuronsWeightsJson(
	// 	TSharedPtr<FJsonObject> const& JsonObject,const uint32 NbInputs, const uint32 NbHidden, const uint32 NbOutputs);
	// static void DeserializeJson(FString const& JsonFileName);
};