#include "NeuralNetworkJson.h"
#include "NeuralNetwork.h"

void FNeuralNetworkJson::SerializeNbNeuronsToJson(FNeuralNetwork const& NeuralNetwork)
{
	const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("NbInputs"), NeuralNetwork.GetNbInputs());
	JsonObject->SetNumberField(TEXT("NbHidden"), NeuralNetwork.GetNbHidden());
	JsonObject->SetNumberField(TEXT("NbOutputs"), NeuralNetwork.GetNbOutputs());

	SerializeToJson(JsonObject, TEXT("NbNeurons"));
}

void FNeuralNetworkJson::SerializeNeuronsWeightsToJson(FNeuralNetwork const& NeuralNetwork)
{
	const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	
	int i = 0;
	for (const double InputWeight : NeuralNetwork.GetInputsWeights())
	{
		FString InputString = FString::Printf(TEXT("Input Weight %d"), i++);
		JsonObject->SetNumberField(InputString, InputWeight);
	}

	i = 0;
	for (const double HiddenWeight : NeuralNetwork.GetHiddenWeights())
	{
		FString HiddenString = FString::Printf(TEXT("Hidden Weight %d"), i++);
		JsonObject->SetNumberField(HiddenString, HiddenWeight);
	}
	
	SerializeToJson(JsonObject, TEXT("NeuronsWeights"));
}

void FNeuralNetworkJson::SerializeToJson(TSharedPtr<FJsonObject> const& JsonObject, FString const& JsonFileName)
{
	FString JsonString;
	const FString JsonPath = FString::Printf(TEXT("/Json/") + JsonFileName + ".json");
	
	FFileHelper::SaveStringToFile(*JsonString, *(FPaths::ProjectDir() + JsonPath));
	
	const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	FFileHelper::SaveStringToFile(*JsonString, *(FPaths::ProjectDir() + JsonPath));
}

TArray<uint32> const& FNeuralNetworkJson::DeserializeNbNeuronsJson(TSharedPtr<FJsonObject> const& JsonObject)
{
	TArray<uint32> NbNeurons;

	const uint32 NbInputs = JsonObject->GetNumberField(TEXT("NbInputs"));
	const uint32 NbHidden = JsonObject->GetNumberField(TEXT("NbHidden"));
	const uint32 NbOutputs = JsonObject->GetNumberField(TEXT("NbOutputs"));
		
	NbNeurons.Add(NbInputs);
	NbNeurons.Add(NbHidden);
	NbNeurons.Add(NbOutputs);

	return NbNeurons;
}

TArray<double> const& FNeuralNetworkJson::DeserializeNeuronsWeightsJson(TSharedPtr<FJsonObject> const& JsonObject,
	const uint32 NbInputs, const uint32 NbHidden, const uint32 NbOutputs)
{
	const uint32 TotalInputsWeights = (NbInputs + 1) * (NbHidden + 1);
	const uint32 TotalHiddenWeights = (NbHidden + 1) * NbOutputs;
		
	TArray<double> Weights;
		
	for (int i = 0; i < TotalInputsWeights; i++)
	{
		FString InputWeightString = FString::Printf(TEXT("Input Weight %d"), i++);
		Weights.Add(JsonObject->GetNumberField(InputWeightString));
	}
		
	for (int i = 0; i < TotalHiddenWeights; i++)
	{
		FString HiddenWeightString = FString::Printf(TEXT("Hidden Weight %d"), i++);
		Weights.Add(JsonObject->GetNumberField(HiddenWeightString));
	}

	return Weights;
}

//TODO : Deserialize into to differents files and return two results in NeuralNetwork::LoadWeights
// void FNeuralNetworkJson::DeserializeJson()
// {
// 	FString JsonString;
// 	TSharedPtr<FJsonObject> JsonObject;
// 	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
//
// 	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
// 	{
// 		DeserializeNbNeuronsJson(JsonObject);
// 		
// 		
// 	}
//
// 	//Error Case
// 	return {};
// }