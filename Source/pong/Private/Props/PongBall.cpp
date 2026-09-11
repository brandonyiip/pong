#include "Props/PongBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

APongBall::APongBall() {
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(16.0f);
	RootComponent = CollisionComp;

	CollisionComp->SetCollisionProfileName(TEXT("PhysicsActor"));
	CollisionComp->SetNotifyRigidBodyCollision(true);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(RootComponent);
	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APongBall::BeginPlay() {
	Super::BeginPlay();

	InitialSpawnLocation = GetActorLocation();

	if (CollisionComp) {
		// Enable physics and re-assert notification after physics initialization
		CollisionComp->SetSimulatePhysics(true);
		CollisionComp->SetNotifyRigidBodyCollision(true);

		// 2D constraint and rotation locks
		CollisionComp->SetConstraintMode(EDOFMode::XYPlane);
		CollisionComp->BodyInstance.bLockXRotation = true;
		CollisionComp->BodyInstance.bLockYRotation = true;
		CollisionComp->BodyInstance.bLockZRotation = true;

		// Bind hit delegate
		CollisionComp->OnComponentHit.RemoveDynamic(this, &APongBall::OnBallHit);
		CollisionComp->OnComponentHit.AddDynamic(this, &APongBall::OnBallHit);

		ResetBall();
	}
}

void APongBall::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APongBall::OnBallHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Inside OnBallHit!"));
	}

	if (!OtherActor || OtherActor == this) return;

	if (OtherActor->ActorHasTag(TEXT("Paddle"))) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("inside of paddle"));
		}

		FVector CurrentVelocity = CollisionComp->GetPhysicsLinearVelocity();
		float CurrentSpeed = CurrentVelocity.Size();

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentSpeed: %.2f"), CurrentSpeed));
		}

		float NewSpeed = CurrentSpeed + SpeedIncrement;
		FVector NewVelocity = CurrentVelocity.GetSafeNormal() * NewSpeed;
		CollisionComp->SetPhysicsLinearVelocity(NewVelocity);
	}
}

void APongBall::ResetBall() {
	CollisionComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
	CollisionComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	SetActorLocation(InitialSpawnLocation);

	float LaunchDirectionX = -1.0f;

	float RandomAngleDegrees = FMath::RandRange(-MaxLaunchAngleDegrees, MaxLaunchAngleDegrees);
	float AngleRadians = FMath::DegreesToRadians(RandomAngleDegrees);

	FVector LaunchDirection;
	LaunchDirection.X = FMath::Sin(AngleRadians);
	LaunchDirection.Y = LaunchDirectionX * FMath::Cos(AngleRadians);
	LaunchDirection.Z = 0.0f;

	LaunchDirection.Normalize();

	CollisionComp->SetPhysicsLinearVelocity(LaunchDirection * InitialSpeed);
	CollisionComp->WakeRigidBody();
}