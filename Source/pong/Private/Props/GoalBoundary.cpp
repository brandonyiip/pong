// Fill out your copyright notice in the Description page of Project Settings.

#include "Props/GoalBoundary.h"
#include "Props/PongBall.h"
#include "GameState/PongGameState.h"

// Sets default values
AGoalBoundary::AGoalBoundary() {
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	// Sets the collision channel to "Trigger" so it registers overlaps without physical blocking
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AGoalBoundary::BeginPlay() {
	Super::BeginPlay();

	// 1. Subscribe GoalBoundary to BoxComponent's overlap event
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGoalBoundary::OnBoxOverlap);

	APongGameState* GS = GetWorld()->GetGameState<APongGameState>();

	if (GS) {
		// Correct binding method for native UObject instances
		OnGoalScored.AddUObject(GS, &APongGameState::HandleGoalScored);
	}
}

void AGoalBoundary::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!OtherActor || OtherActor == this) {
		return;
	}

	APongBall* Ball = Cast<APongBall>(OtherActor);
	if (Ball) {
		OnGoalScored.Broadcast(this, Ball);
	}
}

void AGoalBoundary::SetPlayerNumber(int32 PN) {
	this->PlayerNumber = PN;
}

// Called every frame
void AGoalBoundary::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

