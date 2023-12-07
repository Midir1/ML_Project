#include "NeuralNetworkJson.h"
#include "NeuralNetwork.h"

void FNeuralNetworkJson::SerializeNbNeuronsToJson(FNeuralNetwork const& NeuralNetwork)
{
	const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("NbInputs"), NeuralNetwork.GetNbInputs());
	JsonObject->SetNumberField(TEXT("NbHidden"), NeuralNetwork.GetNbHidden());
	JsonObject->SetNumberField(TEXT("NbOutputs"), NeuralNetwork.GetNbOutputs());

	SerializeToJson(JsonObject, NbNeuronsText);
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
	
	SerializeToJson(JsonObject, NeuronsWeightsText);
}

void FNeuralNetworkJson::SerializeToJson(TSharedPtr<FJsonObject> const& JsonObject, FString const& JsonFileName)
{
	FString JsonString;
	
	FFileHelper::SaveStringToFile(*JsonString, *(FPaths::ProjectDir() + "/Json/" + JsonFileName + ".json"));
	
	const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	FFileHelper::SaveStringToFile(*JsonString, *(FPaths::ProjectDir() + "/Json/" + JsonFileName + ".json"));
}

TArray<uint32> const& FNeuralNetworkJson::DeserializeNbNeuronsJson(TSharedPtr<FJsonObject> const& JsonObject)
{
	const uint32 NbInputs = JsonObject->GetNumberField(TEXT("NbInputs"));
	const uint32 NbHidden = JsonObject->GetNumberField(TEXT("NbHidden"));
	const uint32 NbOutputs = JsonObject->GetNumberField(TEXT("NbOutputs"));

	static TArray<uint32> NbNeurons;
	
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
		
	static TArray<double> Weights;
		
	for (uint32 i = 0; i < TotalInputsWeights; i++)
	{
		FString InputWeightString = FString::Printf(TEXT("Input Weight %d"), i);
		Weights.Add(JsonObject->GetNumberField(InputWeightString));
	}
		
	for (uint32 i = 0; i < TotalHiddenWeights; i++)
	{
		FString HiddenWeightString = FString::Printf(TEXT("Hidden Weight %d"), i);
		Weights.Add(JsonObject->GetNumberField(HiddenWeightString));
	}

	return Weights;
}

bool FNeuralNetworkJson::DeserializeJson(TArray<uint32>& NbNeurons, TArray<double>& Weights)
{
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectDir() + "/Json/" + NbNeuronsText + ".json"));
	
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		NbNeurons = DeserializeNbNeuronsJson(JsonObject);
	}

	FString JsonString1;
	FFileHelper::LoadFileToString(JsonString1, *(FPaths::ProjectDir() + "/Json/" + NeuronsWeightsText + ".json"));
	
	TSharedPtr<FJsonObject> JsonObject1;
	const TSharedRef<TJsonReader<>> JsonReader1 = TJsonReaderFactory<>::Create(JsonString1);

	if(FJsonSerializer::Deserialize(JsonReader1, JsonObject1) && JsonObject1.IsValid())
	{
		Weights = DeserializeNeuronsWeightsJson(JsonObject1,
			NbNeurons[0], NbNeurons[1], NbNeurons[2]);

		return true;
	}

	return false;
}