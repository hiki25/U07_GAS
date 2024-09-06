#include "CPlayerController.h"

void ACPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ACPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateChanged.Broadcast(PlayerState);
	//Todo. WB_Credit에서 변경된 PS(NULL) 인지 아닌지를 잘 받아오는지 확인
}
