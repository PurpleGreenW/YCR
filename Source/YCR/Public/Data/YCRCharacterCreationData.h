#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "YCRCharacterCreationData.generated.h"

USTRUCT(BlueprintType)
struct FYCRHairstyleData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HairstyleID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> HairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
};

UCLASS()
class YCR_API UYCRCharacterCreationData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Creation")
	TArray<FYCRHairstyleData> AvailableHairstyles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Creation")
	int32 MaxNameLength = 16;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Creation")
	int32 MinNameLength = 3;
};