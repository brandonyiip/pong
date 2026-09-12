#include "Props/PongBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

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

		CollisionComp->BodyInstance.bUseCCD = true;
		CollisionComp->OnComponentHit.AddDynamic(this, &APongBall::OnBallHit);

		// Initial ball spawn delay
		ResetBall();
	}
}

void APongBall::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APongBall::ResetBall() {
	if (!CollisionComp) return;

	// 1. Freeze movement and reset position immediately
	CollisionComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
	CollisionComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	SetActorLocation(InitialSpawnLocation);

	// 2. Clear any existing timer to avoid duplicate triggers
	GetWorldTimerManager().ClearTimer(LaunchTimerHandle);

	// 3. Schedule LaunchBall after ResetDelaySeconds
	GetWorldTimerManager().SetTimer(
		LaunchTimerHandle,
		this,
		&APongBall::LaunchBall,
		ResetDelaySeconds,
		false
	);
}

void APongBall::LaunchBall() {
	if (!CollisionComp) return;

	// Calculate random launch angle
	float LaunchDirectionX = FMath::RandBool() ? 1.0f : -1.0f;

	float RandomAngleDegrees = FMath::RandRange(-MaxLaunchAngleDegrees, MaxLaunchAngleDegrees);
	float AngleRadians = FMath::DegreesToRadians(RandomAngleDegrees);

	FVector LaunchDirection;
	LaunchDirection.X = FMath::Sin(AngleRadians);
	LaunchDirection.Y = LaunchDirectionX * FMath::Cos(AngleRadians);
	LaunchDirection.Z = 0.0f;

	LaunchDirection.Normalize();

	// Apply initial launch velocity
	CollisionComp->SetPhysicsLinearVelocity(LaunchDirection * InitialSpeed);
	CollisionComp->WakeRigidBody();
}

void APongBall::OnBallHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (!OtherActor || OtherActor == this || !CollisionComp) return;

	// Check if the hit object is a paddle
	if (OtherActor->ActorHasTag(TEXT("Paddle"))) {
		// 1. Get current ball speed and calculate clean incremented speed
		FVector CurrentVelocity = CollisionComp->GetPhysicsLinearVelocity();
		float CurrentSpeed = CurrentVelocity.Size();

		float NewSpeed = FMath::Min(CurrentSpeed + SpeedIncrement, MaxSpeed);

		// 2. Determine horizontal direction strictly by spatial position (X-axis)
		float DirectionX = (GetActorLocation().X >= OtherActor->GetActorLocation().X) ? 1.0f : -1.0f;

		// 3. Calculate vertical bounce angle based on impact height relative to paddle center
		float DeltaY = GetActorLocation().Y - OtherActor->GetActorLocation().Y;
		float PaddleHalfHeight = 60.0f; // Adjust to match paddle extent
		float NormalizedY = FMath::Clamp(DeltaY / PaddleHalfHeight, -1.0f, 1.0f);

		// 4. Construct clean 2D direction vector on XY Plane
		FVector NewDirection = FVector(DirectionX, NormalizedY, 0.0f).GetSafeNormal();

		// 5. DIRECT OVERRIDE: Replaces all physics impulse calculated by moving paddles
		CollisionComp->SetPhysicsLinearVelocity(NewDirection * NewSpeed);
	}
}

void APongBall::SetPlayerNumber(int32 PN) {
	this->PlayerNumber = PN;
}