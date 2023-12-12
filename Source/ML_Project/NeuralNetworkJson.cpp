#include "NeuralNetworkJson.h"
#include "NeuralNetwork.h"

void FNeuralNetworkJson::SerializeToJson(FNeuralNetwork const& NeuralNetwork)
{
	const TSharedPtr<FJsonObject> NbNeuronsJsonObject = MakeShareable(new FJsonObject);
	const TSharedPtr<FJsonObject> NeuronsWeightsJsonObject = MakeShareable(new FJsonObject);
	
	NbNeuronsToJsonObject(NeuralNetwork, NbNeuronsJsonObject);
	NeuronsWeightsToJsonObject(NeuralNetwork, NeuronsWeightsJsonObject);
	
	SaveJsonObjectToFile(NbNeuronsJsonObject, NbNeuronsText);
	SaveJsonObjectToFile(NeuronsWeightsJsonObject, NeuronsWeightsText);
}

bool FNeuralNetworkJson::DeserializeJson(TArray<uint32>& NbNeurons, TArray<double>& Weights)
{
	FString JsonString;
	TSharedPtr<FJsonObject> JsonObject;
	
	if (FFileHelper::LoadFileToString(JsonString, *NbNeuronsJsonPath))
	{
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
		
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			NbNeurons = JsonObjectToNbNeurons(JsonObject);
		}
	}

	if (FFileHelper::LoadFileToString(JsonString, *NeuronsWeightsJsonPath))
	{
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			Weights = JsonObjectToNeuronsWeights(JsonObject, NbNeurons[0], NbNeurons[1],
				NbNeurons[2]);
            
			return true;
		}
	}

	return false;
}

void FNeuralNetworkJson::NbNeuronsToJsonObject(FNeuralNetwork const& NeuralNetwork, TSharedPtr<FJsonObject> const& JsonObject)
{
	JsonObject->SetNumberField(NbInputsText, NeuralNetwork.GetNbInputs());
	JsonObject->SetNumberField(NbHiddenText, NeuralNetwork.GetNbHidden());
	JsonObject->SetNumberField(NbOutputsText, NeuralNetwork.GetNbOutputs());
}

void FNeuralNetworkJson::NeuronsWeightsToJsonObject(FNeuralNetwork const& NeuralNetwork,
	TSharedPtr<FJsonObject> const& JsonObject)
{
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
}

void FNeuralNetworkJson::SaveJsonObjectToFile(TSharedPtr<FJsonObject> const& JsonObject, FString const& JsonFileName)
{
	FString JsonString;
	const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	FFileHelper::SaveStringToFile(*JsonString, *(FPaths::ProjectDir() + "/Json/" + JsonFileName + ".json"));
}

TArray<uint32> const& FNeuralNetworkJson::JsonObjectToNbNeurons(TSharedPtr<FJsonObject> const& JsonObject)
{
	const uint32 NbInputs = JsonObject->GetNumberField(NbInputsText);
	const uint32 NbHidden = JsonObject->GetNumberField(NbHiddenText);
	const uint32 NbOutputs = JsonObject->GetNumberField(NbOutputsText);

	static TArray<uint32> NbNeurons;
	
	NbNeurons.Add(NbInputs);
	NbNeurons.Add(NbHidden);
	NbNeurons.Add(NbOutputs);

	return NbNeurons;
}

TArray<double> const& FNeuralNetworkJson::JsonObjectToNeuronsWeights(TSharedPtr<FJsonObject> const& JsonObject,
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