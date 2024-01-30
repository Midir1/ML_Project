#include "Agent.h"
#include "NeuralNetworkDataWidget.h"
#include "NeuralNetworkJson.h"
#include "Kismet/KismetSystemLibrary.h"

AAgent::AAgent()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAgent::BeginPlay()
{
	Super::BeginPlay();

	LinetraceArray.SetNum(NbInputs);
	
	InitializeNeuralNetwork();
	GetWorldSettings()->SetTimeDilation(TimeDilation);
}

void AAgent::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	LinetraceArray[0] = LineTrace(GetActorForwardVector());
	LinetraceArray[1] = LineTrace(GetActorForwardVector() + GetActorRightVector() * 0.1f);
	LinetraceArray[2] = LineTrace(GetActorForwardVector() + GetActorRightVector() * 0.2f);
	LinetraceArray[3] = LineTrace(GetActorForwardVector() + GetActorRightVector() * 0.3f);
	LinetraceArray[4] = LineTrace(GetActorForwardVector() - GetActorRightVector() * 0.1f);
	LinetraceArray[5] = LineTrace(GetActorForwardVector() - GetActorRightVector() * 0.2f);
	LinetraceArray[6] = LineTrace(GetActorForwardVector() - GetActorRightVector() * 0.3f);

	EntriesTick();
	NeuralNetwork.Train(Entry);
	
	if(!NeuralNetwork.IsTrainingDone())
	{
		OutputsValuesTick(false);
	}
	else
	{
		OutputsValuesTick(true);
		SaveNeuronsDataToJson();
	}
}

void AAgent::InitializeNeuralNetwork()
{
	NeuronsConfiguration.NbInputs = NbInputs;
	NeuronsConfiguration.NbHidden = NbHidden;
	NeuronsConfiguration.NbOutputs = NbOutputs;
	
	NetworkConfiguration.LearningRate = LearningRate;
	NetworkConfiguration.Momentum = Momentum;
	NetworkConfiguration.MaxEpochs = MaxEpochs;

	const uint32 TotalNeurons = NbInputs + NbHidden + NbOutputs;
	TArray<uint32> NbNeurons;
	TArray<double> Weights;

	if(FNeuralNetworkJson::DeserializeJson(NbNeurons, Weights)
		&& TotalNeurons == NbNeurons[0] + NbNeurons[1] + NbNeurons[2])
	{
		NeuralNetwork = FNeuralNetwork(NeuronsConfiguration, NetworkConfiguration, Weights);
	}
	else
	{
		NeuralNetwork = FNeuralNetwork(NeuronsConfiguration, NetworkConfiguration);
	}
	
}
void AAgent::EntriesTick()
{
	Entry = FTrainingEntry();
		
	uint32 i = 0;
	double x = 0.375f;
	double y = 0.875f;
	double XStep = 0.125f;
	double YStep = 0.125f;

	while (i < NbInputs)
	{
		Entry.Inputs.Add(LinetraceArray[i]);

		x += XStep;
		y += YStep;

		if(x == 1.0f || x == 0.0f)
		{
			XStep = -XStep;
		}

		if(y == 1.0f || y == 0.0f)
		{
			YStep = -YStep;
		}
			
		if(LinetraceArray[i])
		{
			Entry.ExpectedOutputs.Add(x);
			Entry.ExpectedOutputs.Add(y);
			Entry.ExpectedOutputs.Add(0.5);

			return;
		}

		i++;
	}

	Entry.ExpectedOutputs.Add(0.5);
	Entry.ExpectedOutputs.Add(0.5);
	Entry.ExpectedOutputs.Add(1.0);
}

void AAgent::OutputsValuesTick(const bool TrainingOver)
{
	const float Distance = Spheres[0]->GetActorLocation().Y - GetActorLocation().Y;
	
	if(NeuralNetwork.GetNbOutputs() > 0 && Controller != nullptr)
	{
		float const MoveValueX = 2.0f * static_cast<float>(NeuralNetwork.GetOutputsValuesClamped()[0]) - 1.0f;
		float const MoveValueY = 2.0f * static_cast<float>(NeuralNetwork.GetOutputsValuesClamped()[1]) - 1.0f;
		float const RotValue = (2.0f * static_cast<float>(NeuralNetwork.GetOutputsValuesClamped()[2]) - 1.0f) * 0.5f;
		
		AddMovementInput(GetActorRightVector(), MoveValueX);
		AddMovementInput(GetActorForwardVector(), MoveValueY);
		AddControllerYawInput(RotValue);
		
		NeuralNetworkDataWidget->SetWidgetData(Distance, MoveValueX, MoveValueY, TrainingOver);
	}
}

void AAgent::SaveNeuronsDataToJson()
{
	if(!JsonSaved)
	{
		JsonSaved = true;
		FNeuralNetworkJson::SerializeToJson(NeuralNetwork);
	}
}

uint8 AAgent::LineTrace(FVector LineAngle) const
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + LineAngle * 2000.0f;

	FHitResult HitResult;
	
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		this,
		StartLocation,
		EndLocation,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (bHit && Spheres.Contains(HitResult.GetActor()))
	{
		return true;
	}

	return false;
}